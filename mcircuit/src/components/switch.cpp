#include "mcircuit/components/switch.h"

namespace mcircuit {
const unsigned Switch::OUTPUT_PIN_ID = 0;

Switch::Switch(unsigned id) : Component(id) {
  addOutputPin({OUTPUT_PIN_ID});
  width = 1;
}

void Switch::updateState() {}

unsigned Switch::getWidth() const { return width; }

void Switch::setWidth(unsigned width) {
  this->width = width;
  getPin(OUTPUT_PIN_ID)->setWidth(width);
}

void Switch::setBitN(unsigned n, bool state) {
  auto pin = getPin(OUTPUT_PIN_ID);

  pin->setState((pin->getState() & ~(1u << n)) |
                (static_cast<unsigned>(state) << n));
}

bool Switch::getBitN(unsigned n) {
  auto pin = getPin(OUTPUT_PIN_ID);

  return static_cast<bool>((pin->getState() & (1u << n)) >> n);
}

unsigned Switch::getState() { return getPin(OUTPUT_PIN_ID)->getState(); }

Component *Switch::clone(unsigned id) {
  auto cloned = new Switch(id);

  cloned->setWidth(getWidth());

  return cloned;
}

} // namespace mcircuit
