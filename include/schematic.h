#ifndef SCHEMATIC_H
#define SCHEMATIC_H

#include "ui_component.h"
#include <map>
#include <mcircuit/components/custom_component.h>
#include <mcircuit/wire_manager.h>
#include <memory>

struct Schematic {
  std::map<unsigned, std::unique_ptr<UIComponentBase>> uiComponents;
  mcircuit::CustomComponent component;
  mcircuit::WireManager wireManager;
};

#endif // SCHEMATIC_H
