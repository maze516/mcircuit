#include "mcircuit/wire_manager.h"
#include <iostream>

namespace mcircuit {
WireManager::WireManager(mcircuit::CustomComponent *root)
    : rootComponent(root) {}

void WireManager::placePin(int x, int y, Component *component, unsigned pin) {
  placePin(x, y, {component, component->getPin(pin)});
}

void WireManager::placePin(int x, int y, ComponentPin data) {
  pins[{x, y}] = std::move(data);
  propagate(x, y);
}

void WireManager::removePin(int x, int y) {
  depropagate(x, y);
  pins.erase({x, y});
}

void WireManager::overlap(int x, int y) {
  for (auto [p, cp] : pins) {
    auto [x, y] = p;
    depropagate(x, y);
  }

  if (overlaps.find({x, y}) == overlaps.end())
    overlaps.insert({x, y});
  else
    overlaps.erase({x, y});

  swipe(Vertical, x);
  swipe(Horizontal, y); // TODO: Check if this one is needed.

  for (auto [p, cp] : pins) {
    auto [x, y] = p;
    propagate(x, y);
  }
}

void WireManager::placeWire(Orientation orientation, int p, int r1, int r2) {
  if (r1 == r2)
    return;

  if (r1 > r2)
    std::swap(r1, r2);

  for (auto [p, cp] : pins) {
    auto [x, y] = p;
    depropagate(x, y);
  }

  connect(orientation, p, r1, r2);

  swipe(orientation, p);

  for (auto [p, cp] : pins) {
    auto [x, y] = p;
    propagate(x, y);
  }
}

void WireManager::removeWire(WireManager::Orientation orientation, int p,
                             int r1, int r2) {
  if (r1 == r2)
    return;

  if (r1 > r2)
    std::swap(r1, r2);

  for (auto [p, cp] : pins) {
    auto [x, y] = p;
    depropagate(x, y);
  }

  // Insert temporary points of removal
  wires[orientation][p].insert(LinePoint{r1, 0, r2, true, true});
  wires[orientation][p].insert(LinePoint{r2, 0, r1, true, true});

  swipe(orientation, p);

  for (auto [p, cp] : pins) {
    auto [x, y] = p;
    propagate(x, y);
  }
}

const WireManager::Wires &WireManager::getWires() const { return wires; }

const WireManager::Pins &WireManager::getPins() const { return pins; }

const std::set<WireManager::Point> &WireManager::getOverlaps() const {
  return overlaps;
}

void WireManager::propagate(int x, int y) {
  propagateHelper(x, y, [this](ComponentPin cp1, ComponentPin cp2) {
    rootComponent->connect(cp1, cp2);
  });
}

void WireManager::depropagate(int x, int y) {
  propagateHelper(x, y, [this](ComponentPin cp1, ComponentPin cp2) {
    rootComponent->disconnect(cp1, cp2);
  });
}

void WireManager::swipe(WireManager::Orientation orientation, int p,
                        bool secondSwipe) {
  auto points = std::move(wires[orientation][p]);
  std::unordered_set<int> orthosToSwipe;

  for (auto &[p2, points2] : wires[1 - orientation]) {
    for (auto point : points2) {
      if (point.position <= p && point.endPosition >= p) {
        Point overlapPoint;
        if (orientation == Vertical)
          overlapPoint = {p, p2};
        else
          overlapPoint = {p2, p};
        if (overlaps.find(overlapPoint) != overlaps.end())
          continue;
      }

      if (point.position <= p && point.endPosition >= p) {
        // Fixed points should always have index 0 to be prioritized in the set
        points.insert(LinePoint{p2, 0, p2, true});
        orthosToSwipe.insert(p2);
      }
    }
  }

  std::unordered_set<unsigned> indexes;
  std::set<LinePoint> newPoints;

  std::optional<int> start;

  bool activeRemoval = false;

  for (auto &point : points) {
    if (point.fixed) {
      if (activeRemoval && point.removal) {
        start = point.position;
        activeRemoval = false;
        continue;
      }

      if (start) {
        if (point.position - *start > 0) {
          newPoints.insert(LinePoint{*start, newIndex, point.position});
          newPoints.insert(LinePoint{point.position, newIndex, *start});
          newIndex++;
        }

        if (point.removal) {
          activeRemoval = true;
        }

        if (indexes.size() > 0 && !point.removal)
          start = point.position;
        else
          start = std::nullopt;
      }

      continue;
    }

    if (!start) {
      start = point.position;
    }

    if (indexes.find(point.index) != indexes.end()) {
      indexes.erase(point.index);
    } else {
      indexes.insert(point.index);
    }

    if (indexes.size() == 0 && start && point.position - *start > 0) {
      newPoints.insert(LinePoint{*start, newIndex, point.position});
      newPoints.insert(LinePoint{point.position, newIndex, *start});
      newIndex++;
      start = std::nullopt;
    }
  }

  wires[orientation][p] = newPoints;

  if (!secondSwipe) {
    for (auto p : orthosToSwipe) {
      swipe(static_cast<Orientation>(1 - orientation), p, true);
    }
  }
}

void WireManager::connect(Orientation orientation, int p, int r1, int r2) {
  auto &points = wires[orientation][p];
  points.insert(LinePoint{r1, newIndex, r2});
  points.insert(LinePoint{r2, newIndex, r1});
  newIndex++;
}

bool WireManager::canOverlap(int x, int y) { return true; }

template <typename Handler>
void WireManager::walk(int x, int y, Handler &&handler) {
  std::queue<Point> q;
  std::set<Point> visited;
  q.emplace(x, y);

  while (!q.empty()) {
    auto [x, y] = std::move(q.front());
    q.pop();

    handler(x, y);

    visited.insert({x, y});

    for (auto p : wires[Vertical][x]) {
      if (p.position == y &&
          visited.find({x, p.endPosition}) == visited.end()) {
        q.emplace(x, p.endPosition);
      }
    }

    for (auto p : wires[Horizontal][y]) {
      if (p.position == x &&
          visited.find({p.endPosition, y}) == visited.end()) {
        q.emplace(p.endPosition, y);
      }
    }
  }
}

template <typename Handler>
void WireManager::propagateHelper(int x, int y, Handler &&handler) {
  auto cp1 = pins[{x, y}];
  auto [component, pin] = cp1;

  if (!component->isOutput(pin->getId()))
    return;

  walk(x, y, [this, cp1, handler](int nx, int ny) {
    if (pins.find({nx, ny}) == pins.end())
      return;

    auto cp2 = pins[{nx, ny}];
    auto [component2, pin2] = cp2;

    if (!component2->isInput(pin2->getId()))
      return;

    handler(cp1, cp2);
  });
}

} // namespace mcircuit
