#ifndef COMPONENT_H
#define COMPONENT_H

#include "mcircuit/pin.h"
#include <array>
#include <unordered_map>

namespace mcircuit {
class Component {
public:
  enum PinType : std::size_t { Input, Output };

  Component(unsigned id);

  unsigned getId() const;
  virtual void updateState() = 0;

  Pin *getPin(unsigned id);

  bool isInput(unsigned id) const;
  bool isOutput(unsigned id) const;
  bool pinWithIdExists(unsigned id) const;

  std::unordered_map<unsigned, Pin> &getOutputs();
  std::unordered_map<unsigned, Pin> &getInputs();

  virtual Component *clone(unsigned id) = 0;

  virtual ~Component() = default;

protected:
  std::array<std::unordered_map<unsigned, Pin>, 2> pins;

  Pin *addInputPin(Pin pin);
  Pin *addOutputPin(Pin pin);
  Pin *addPin(PinType type, Pin pin);
  void removePin(unsigned id);

private:
  unsigned id;
};
} // namespace mcircuit

#endif // COMPONENT_H
