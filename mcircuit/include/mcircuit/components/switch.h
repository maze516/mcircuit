#ifndef SWITCH_H
#define SWITCH_H

#include "mcircuit/component.h"

namespace mcircuit {
class Switch : public Component {
public:
  const static unsigned OUTPUT_PIN_ID;

  Switch(unsigned id);

  void updateState() override;

  unsigned getWidth() const;
  void setWidth(unsigned int width);

  void setBitN(unsigned int n, bool state);
  bool getBitN(unsigned int n);
  unsigned getState();

  Component *clone(unsigned id) override;

private:
  unsigned width;
};
} // namespace mcircuit

#endif // SWITCH_H
