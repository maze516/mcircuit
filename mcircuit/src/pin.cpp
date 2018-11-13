#include "mcircuit/pin.h"

namespace mcircuit {
unsigned Pin::getId() const { return id; }

Pin::Pin(unsigned id, unsigned width, unsigned state)
    : id{id}, width{width}, state{state} {}

unsigned Pin::getState() const { return state; }

void Pin::setState(unsigned state) { Pin::state = state; }

unsigned Pin::getWidth() const { return width; }

void Pin::setWidth(unsigned width) { Pin::width = width; }

void Pin::normalizeState() { state &= (1 << width) - 1; }

Pin Pin::clone(unsigned id) { return Pin{id, width, 0}; }
} // namespace mcircuit
