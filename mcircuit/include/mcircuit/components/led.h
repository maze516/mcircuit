#ifndef LED_H
#define LED_H

#include "mcircuit/component.h"

namespace mcircuit {
class LED : public Component {
public:
  const static unsigned INPUT_PIN_ID;

  LED(unsigned id);

  void setWidth(unsigned width);
  unsigned getWidth() const;

  void updateState() override;

  Component *clone(unsigned id) override;

private:
  unsigned width;
};
} // namespace mcircuit

#endif // LED_H
