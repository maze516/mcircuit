#ifndef WIRE_MANAGER_H
#define WIRE_MANAGER_H

#include <array>
#include <map>
#include <mcircuit/algorithm.h>
#include <mcircuit/components/custom_component.h>
#include <optional>
#include <queue>
#include <set>
#include <unordered_set>

namespace mcircuit {

class WireManager {
  struct LinePoint {
    int position;
    unsigned index;
    int endPosition;
    bool fixed = false;
    bool removal = false;

    bool operator<(const LinePoint &other) const {
      return position == other.position ? index > other.index
                                        : position < other.position;
    }
  };

  using Wires = std::array<std::map<int, std::set<LinePoint>>, 2>;

  // TODO: Should probably make tuple hashable and use unordered_map
  using Pins = std::map<std::tuple<int, int>, ComponentPin>;

  using Point = std::tuple<int, int>;

public:
  enum Orientation { Horizontal, Vertical };

  WireManager(CustomComponent *rootComponent);
  void placePin(int x, int y, Component *component, unsigned pin);
  void placePin(int x, int y, ComponentPin data);
  void removePin(int x, int y);
  void overlap(int x, int y);

  void placeWire(Orientation orientation, int p, int r1, int r2);
  void removeWire(Orientation orientation, int p, int r1, int r2);

  const Wires &getWires() const;
  const Pins &getPins() const;
  const std::set<Point> &getOverlaps() const;

  //  bool isConnected(const Point &p1, const Point &p2);

private:
  // Handler is a callable with the signature (int nx, int ny)
  template <typename Handler> void walk(int x, int y, Handler &&handler);
  void propagate(int x, int y);

  template <typename Handler>
  void propagateHelper(int x, int y, Handler &&handler);

  void depropagate(int x, int y);
  void swipe(Orientation orientation, int p, bool secondSwipe = false);
  void connect(Orientation orientation, int p, int r1, int r2);

  bool canOverlap(int x, int y);

  CustomComponent *rootComponent;

  Wires wires;

  unsigned newIndex = 0;

  Pins pins;

  std::set<Point> overlaps;
};
} // namespace mcircuit

#endif // WIRE_MANAGER_H
