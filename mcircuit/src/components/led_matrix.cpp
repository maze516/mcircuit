#include "mcircuit/components/led_matrix.h"

namespace mcircuit {

LEDMatrix::LEDMatrix(unsigned id)
    : VariableInputGate(id), width(1), numInputs(1) {
  resizePixels();
}

void LEDMatrix::updateState() {
  for (auto i = INPUT_PIN_ID_START; i < static_cast<unsigned>(getNumInputs());
       i++) {
    auto state = getPin(i)->getState();

    for (auto j = 0u; j < getWidth(); j++) {
      pixels[i + j * getLEDWidth()] = (state & (1 << j)) >> j;
    }
  }
}

bool LEDMatrix::isPixelOn(unsigned x, unsigned y) {
  return pixels[x + y * getLEDWidth()];
}

Component *LEDMatrix::clone(unsigned id) {
  auto cloned = new LEDMatrix(id);

  cloned->setLEDWidth(getLEDWidth());
  cloned->setLEDHeight(getLEDHeight());

  return cloned;
}

void LEDMatrix::setLEDHeight(unsigned height) {
  setWidth(height);

  resizePixels();
}

void LEDMatrix::setLEDWidth(unsigned width) {
  setNumInputs(width);

  resizePixels();
}

unsigned LEDMatrix::getLEDWidth() const { return getNumInputs(); }

unsigned LEDMatrix::getLEDHeight() const { return getWidth(); }

void LEDMatrix::resizePixels() {
  pixels = std::vector<bool>(
      static_cast<unsigned>(getLEDWidth() * getLEDHeight()), false);
}

} // namespace mcircuit
