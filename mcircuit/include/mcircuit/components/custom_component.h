#ifndef CUSTOM_COMPONENT_H
#define CUSTOM_COMPONENT_H

#include "mcircuit/component.h"
#include <exception>
#include <map>
#include <memory>
#include <set>
#include <stack>
#include <unordered_map>
#include <unordered_set>

namespace mcircuit {
using ComponentPin = std::tuple<mcircuit::Component *, mcircuit::Pin *>;

struct PinWidthIncompatibleError : std::runtime_error {
  PinWidthIncompatibleError(ComponentPin componentPin1,
                            ComponentPin componentPin2)
      : std::runtime_error("incompatible pin widths"),
        componentPin1{std::move(componentPin1)}, componentPin2{std::move(
                                                     componentPin2)} {}

  ComponentPin componentPin1;
  ComponentPin componentPin2;
};

class CustomComponent : public Component {
  using Id = std::tuple<unsigned, unsigned>;

public:
  CustomComponent(unsigned id);

  void updateState() override;

  void addComponent(Component *component);

  Component *getComponent(unsigned id);

  void connect(const ComponentPin &cp1, const ComponentPin &cp2);
  void connect(Component *c1, Pin *p1, Component *c2, Pin *p2);
  void connect(Component *c1, unsigned pin1, Component *c2, unsigned pin2);

  void disconnect(const ComponentPin &cp1, const ComponentPin &cp2);
  void disconnect(Component *c1, Pin *p1, Component *c2, Pin *p2);
  void disconnect(Component *c1, unsigned pin1, Component *c2, unsigned pin2);

  Pin *addInputPin(Pin pin);
  Pin *addOutputPin(Pin pin);

  std::tuple<Component *, Pin *> getFromId(unsigned componentId,
                                           unsigned pinId);

  Component *clone(unsigned id) override;

private:
  void updateNearPins(const Id &id);

  bool hasConnections(const Id &id);

  void toposort(unsigned componentId, std::stack<unsigned> &sortedComponentIds,
                std::unordered_set<unsigned> &visited);

  std::unordered_map<unsigned, std::unique_ptr<Component>> components;

  std::map<Id, std::set<Id>> wires;
  std::set<Id> pinStateDefined;
};
} // namespace mcircuit

#endif // CUSTOM_COMPONENT_H
