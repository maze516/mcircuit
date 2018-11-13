#ifndef BASIC_GATE_H
#define BASIC_GATE_H

#include "variable_input_gate.h"

namespace mcircuit {
class BasicGate : public VariableInputGate {
public:
  enum class Type { And, Or, Xor, Nand, Nor, Xnor };

  BasicGate(unsigned id, Type type);

  Type getType() const;

  void updateState() override;
  Component *clone(unsigned id) override;

private:
  Type type;
};
} // namespace mcircuit

#endif // BASIC_GATES_H
