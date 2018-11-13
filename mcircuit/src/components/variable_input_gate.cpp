#include "mcircuit/components/variable_input_gate.h"

namespace mcircuit {
const unsigned VariableInputGate::INPUT_PIN_ID_START = 1;
const unsigned VariableInputGate::OUTPUT_PIN_ID = 0;

VariableInputGate::VariableInputGate(unsigned id)
    : Component(id), width(1), numInputs(1) {
  addInputPin({VariableInputGate::getInputId(0)});
  addOutputPin({OUTPUT_PIN_ID});
}

void VariableInputGate::setWidth(unsigned width) {
  this->width = width;

  for (auto i = 0u; i < numInputs; i++) {
    getPin(getInputId(i))->setWidth(width);
  }

  getPin(OUTPUT_PIN_ID)->setWidth(width);
}

unsigned VariableInputGate::getWidth() const { return width; }

void VariableInputGate::setNumInputs(unsigned numInputs) {
  if (numInputs > VariableInputGate::numInputs) {
    for (auto i = VariableInputGate::numInputs; i < numInputs; i++) {
      addInputPin({getInputId(i)});
    }
  } else {
    for (auto i = VariableInputGate::numInputs - 1; i >= numInputs; i++) {
      removePin(getInputId(i));
    }
  }

  VariableInputGate::numInputs = numInputs;
}

unsigned VariableInputGate::getNumInputs() const { return numInputs; }

unsigned VariableInputGate::getInputId(unsigned index) {
  return INPUT_PIN_ID_START + index;
}
} // namespace mcircuit
