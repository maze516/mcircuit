#include "mcircuit/components/custom_component.h"
#include <iostream>

namespace mcircuit {
CustomComponent::CustomComponent(unsigned id) : Component(id) {}

void CustomComponent::updateState() {
  pinStateDefined.clear();

  for (const auto &[pinId, _] : getInputs()) {
    updateNearPins({0, pinId});
  }

  std::stack<unsigned> sortedComponentIds;
  std::unordered_set<unsigned> visited;

  for (const auto &[id, component] : components) {
    if (visited.find(id) == visited.end()) {
      toposort(id, sortedComponentIds, visited);
    }
  }

  while (!sortedComponentIds.empty()) {
    auto componentId = sortedComponentIds.top();
    sortedComponentIds.pop();

    auto component = getComponent(componentId);

    component->updateState();

    for (auto &[pinId, _] : component->getOutputs()) {
      updateNearPins({componentId, pinId});
    }
  }
}

void CustomComponent::addComponent(Component *component) {
  components.emplace(component->getId(), component);
}

Component *CustomComponent::getComponent(unsigned id) {
  if (id == 0)
    return this;
  return components.at(id).get();
}

void CustomComponent::connect(const ComponentPin &cp1,
                              const ComponentPin &cp2) {
  auto [c1, p1] = cp1;
  auto [c2, p2] = cp2;

  connect(c1, p1, c2, p2);
}

void CustomComponent::connect(Component *c1, Pin *p1, Component *c2, Pin *p2) {
  if (p1->getWidth() != p2->getWidth())
    throw PinWidthIncompatibleError({c1, p1}, {c2, p2});
  std::fprintf(stderr, "connecting (%d %d) -> (%d %d)\n", c1->getId(),
               p1->getId(), c2->getId(), p2->getId());
  wires[{c1->getId(), p1->getId()}].insert({c2->getId(), p2->getId()});
}

void CustomComponent::connect(Component *c1, unsigned p1, Component *c2,
                              unsigned p2) {
  connect(c1, c1->getPin(p1), c2, c2->getPin(p2));
}

void CustomComponent::disconnect(const ComponentPin &cp1,
                                 const ComponentPin &cp2) {
  auto [c1, p1] = cp1;
  auto [c2, p2] = cp2;

  disconnect(c1, p1, c2, p2);
}

void CustomComponent::disconnect(Component *c1, Pin *p1, Component *c2,
                                 Pin *p2) {
  wires[{c1->getId(), p1->getId()}].erase({c2->getId(), p2->getId()});
}

void CustomComponent::disconnect(Component *c1, unsigned p1, Component *c2,
                                 unsigned p2) {
  disconnect(c1, c1->getPin(p1), c2, c2->getPin(p2));
}

Pin *CustomComponent::addInputPin(Pin pin) { return addInputPin(pin); }

Pin *CustomComponent::addOutputPin(Pin pin) { return addOutputPin(pin); }

std::tuple<Component *, Pin *> CustomComponent::getFromId(unsigned componentId,
                                                          unsigned pinId) {
  Component *component;

  if (componentId == 0)
    component = this;
  else
    component = components.at(componentId).get();

  return {component, component->getPin(pinId)};
}

Component *CustomComponent::clone(unsigned id) {
  auto cloned = new CustomComponent(id);

  for (auto &input : getInputs()) {
    cloned->addInputPin(input.second.clone(input.first));
  }

  for (auto &outputs : getOutputs()) {
    cloned->addOutputPin(outputs.second.clone(outputs.first));
  }

  for (auto &component : components) {
    cloned->addComponent(component.second->clone(component.first));
  }

  cloned->wires = wires;

  return cloned;
}

void CustomComponent::updateNearPins(const Id &id) {
  const auto &[componentId, pinId] = id;

  if (hasConnections(id)) {
    for (auto &id : wires.at(id)) {
      auto [targetComponentId, targetPinId] = id;
      auto pin = getComponent(targetComponentId)->getPin(targetPinId);
      auto state = getComponent(componentId)->getPin(pinId)->getState();

      if (pinStateDefined.find(id) != pinStateDefined.end()) {
        // Wired OR
        pin->setState(pin->getState() | state);
      } else {
        pin->setState(state);
        pinStateDefined.insert(id);
      }
    }
  }
} // namespace mcircuit

bool CustomComponent::hasConnections(const Id &id) {
  return wires.find(id) != wires.end();
}

void CustomComponent::toposort(unsigned componentId,
                               std::stack<unsigned> &sortedComponentIds,
                               std::unordered_set<unsigned> &visited) {
  auto component = getComponent(componentId);

  visited.insert(componentId);

  for (auto &[pinId, pin] : component->getOutputs()) {
    if (hasConnections({componentId, pinId})) {
      for (auto &[targetComponentId, targetPinId] :
           wires.at({componentId, pinId})) {
        if (visited.find(targetComponentId) == visited.end()) {
          toposort(targetComponentId, sortedComponentIds, visited);
        }
      }
    }
  }

  sortedComponentIds.push(componentId);
}
} // namespace mcircuit
