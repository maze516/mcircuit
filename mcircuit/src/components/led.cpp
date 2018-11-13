#include "mcircuit/components/led.h"

namespace mcircuit {
const unsigned LED::INPUT_PIN_ID = 0;

LED::LED(unsigned id) : Component(id), width(1) { addInputPin({INPUT_PIN_ID}); }

void LED::setWidth(unsigned width) {
  this->width = width;
  getPin(INPUT_PIN_ID)->setWidth(width);
}

unsigned LED::getWidth() const { return width; }

void LED::updateState() {}

Component *LED::clone(unsigned id) {
  auto cloned = new LED(id);

  cloned->setWidth(getWidth());

  return cloned;
}
} // namespace mcircuit
