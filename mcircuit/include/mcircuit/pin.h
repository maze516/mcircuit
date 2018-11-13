#ifndef PIN_H
#define PIN_H

namespace mcircuit {
class Pin {
public:
  Pin(unsigned id, unsigned width = 1, unsigned state = 0);

  unsigned getId() const;

  unsigned getState() const;
  void setState(unsigned state);

  unsigned getWidth() const;
  void setWidth(unsigned width);

  void normalizeState();

  Pin clone(unsigned id);

private:
  unsigned id;
  unsigned width;
  unsigned state;
};
} // namespace mcircuit

#endif // PIN_H
