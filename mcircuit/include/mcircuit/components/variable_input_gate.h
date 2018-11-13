#ifndef VARIABLE_INPUT_GATE_H
#define VARIABLE_INPUT_GATE_H

#include "mcircuit/component.h"

namespace mcircuit {
class VariableInputGate : public Component {
public:
  const static unsigned INPUT_PIN_ID_START;
  const static unsigned OUTPUT_PIN_ID;

  VariableInputGate(unsigned id);

  virtual void setWidth(unsigned width);
  unsigned getWidth() const;

  virtual void setNumInputs(unsigned numInputs);
  unsigned getNumInputs() const;

  unsigned static getInputId(unsigned index);

  virtual ~VariableInputGate() = default;

private:
  unsigned width;
  unsigned numInputs;
};
} // namespace mcircuit

#endif // VARIABLE_INPUT_GATE_H
