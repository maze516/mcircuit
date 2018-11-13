#include "mcircuit/components/basic_gate.h"

namespace mcircuit {
BasicGate::BasicGate(unsigned id, BasicGate::Type type)
    : VariableInputGate(id), type(type) {
  setNumInputs(2);
}

BasicGate::Type BasicGate::getType() const { return type; }

void BasicGate::updateState() {
  unsigned result;

  switch (type) {
  case Type::And:
    result = ~0u;
    for (auto i = 0u; i < getNumInputs(); i++) {
      result &= getPin(getInputId(i))->getState();
    }

    break;
  case Type::Or:
    result = 0u;

    for (auto i = 0u; i < getNumInputs(); i++) {
      result |= getPin(getInputId(i))->getState();
    }

    break;
  case Type::Xor:
    result = 0u;

    for (auto i = 0u; i < getNumInputs(); i++) {
      result ^= getPin(getInputId(i))->getState();
    }

    break;
  case Type::Nand:
    result = ~0u;

    for (auto i = 0u; i < getNumInputs(); i++) {
      result &= getPin(getInputId(i))->getState();
    }

    result = ~result;

    break;
  case Type::Nor:
    result = 0;

    for (auto i = 0u; i < getNumInputs(); i++) {
      result |= getPin(getInputId(i))->getState();
    }

    result = ~result;

    break;
  case Type::Xnor:
    result = 0u;

    for (auto i = 0u; i < getNumInputs(); i++) {
      result ^= getPin(getInputId(i))->getState();
    }

    result = ~result;

    break;
  }

  getPin(OUTPUT_PIN_ID)->setState(result);
  getPin(OUTPUT_PIN_ID)->normalizeState();
}

Component *BasicGate::clone(unsigned id) {
  auto cloned = new BasicGate(id, type);

  cloned->setNumInputs(getNumInputs());
  cloned->setWidth(getWidth());

  return cloned;
}
} // namespace mcircuit
