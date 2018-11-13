#ifndef NOTGATE_H
#define NOTGATE_H

#include "mcircuit/component.h"

namespace mcircuit {
class NotGate : public Component {
public:
  const static unsigned INPUT_PIN_ID;
  const static unsigned OUTPUT_PIN_ID;

  NotGate(unsigned id);

  void setWidth(unsigned width);

  unsigned getWidth() const;

  void updateState() override;

  Component *clone(unsigned id) override;

private:
  unsigned width;
};
} // namespace mcircuit

#endif // NOTGATE_H
