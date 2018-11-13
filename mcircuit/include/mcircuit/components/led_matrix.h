#ifndef LED_MATRIX_H
#define LED_MATRIX_H

#include "mcircuit/components/variable_input_gate.h"
#include <vector>

namespace mcircuit {
class LEDMatrix : public VariableInputGate {
public:
  LEDMatrix(unsigned id);

  void updateState();

  bool isPixelOn(unsigned x, unsigned y);

  Component *clone(unsigned id);

  void setLEDWidth(unsigned width);
  void setLEDHeight(unsigned height);

  unsigned getLEDWidth() const;
  unsigned getLEDHeight() const;

private:
  void resizePixels();

  unsigned width;
  unsigned numInputs;
  std::vector<bool> pixels;
};
} // namespace mcircuit

#endif // LED_MATRIX_H
