#include "mcircuit/component.h"

namespace mcircuit {
Component::Component(unsigned id) : id(id) {}

unsigned Component::getId() const { return id; }

Pin *Component::getPin(unsigned id) {
  if (isInput(id))
    return &pins[Input].at(id);
  if (isOutput(id))
    return &pins[Output].at(id);
  return nullptr;
}

bool Component::isInput(unsigned id) const {
  return pins[Input].find(id) != pins[Input].end();
}

bool Component::isOutput(unsigned id) const {
  return pins[Output].find(id) != pins[Output].end();
}

std::unordered_map<unsigned, Pin> &Component::getOutputs() {
  return pins[Output];
}

std::unordered_map<unsigned, Pin> &Component::getInputs() {
  return pins[Input];
}

Pin *Component::addPin(PinType type, Pin pin) {
  // Maybe throw here
  if (pinWithIdExists(pin.getId()))
    return nullptr;

  return &pins[type].emplace(pin.getId(), std::move(pin)).first->second;
}

void Component::removePin(unsigned id) {
  pins[Input].erase(id);
  pins[Output].erase(id);
}

Pin *Component::addInputPin(Pin pin) { return addPin(Input, std::move(pin)); }

Pin *Component::addOutputPin(Pin pin) { return addPin(Output, std::move(pin)); }

bool Component::pinWithIdExists(unsigned id) const {
  return isInput(id) || isOutput(id);
}
} // namespace mcircuit
