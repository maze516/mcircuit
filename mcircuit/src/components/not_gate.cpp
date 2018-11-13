#include "mcircuit/components/not_gate.h"

namespace mcircuit {
const unsigned NotGate::INPUT_PIN_ID = 0;
const unsigned NotGate::OUTPUT_PIN_ID = 1;

NotGate::NotGate(unsigned id) : Component(id), width(1) {
  addInputPin({INPUT_PIN_ID});
  addOutputPin({OUTPUT_PIN_ID});
}

void NotGate::setWidth(unsigned width) {
  this->width = width;
  getPin(INPUT_PIN_ID)->setWidth(width);
  getPin(OUTPUT_PIN_ID)->setWidth(width);
}

unsigned NotGate::getWidth() const { return width; }

void NotGate::updateState() {
  getPin(OUTPUT_PIN_ID)->setState(~getPin(INPUT_PIN_ID)->getState());
  getPin(OUTPUT_PIN_ID)->normalizeState();
}

Component *NotGate::clone(unsigned id) {
  auto cloned = new NotGate(id);

  cloned->setWidth(getWidth());

  return cloned;
}
} // namespace mcircuit
