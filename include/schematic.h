#ifndef SCHEMATIC_H
#define SCHEMATIC_H

#include "ui_component.h"
#include <map>
#include <mcircuit/components/custom_component.h>
#include <mcircuit/wire_manager.h>
#include <memory>

struct Schematic {
  Schematic(mcircuit::CustomComponent *component)
      : component{component}, wireManager{component} {}

  std::unique_ptr<mcircuit::CustomComponent> component;
  std::map<unsigned, std::unique_ptr<UIComponentBase>> uiComponents;
  mcircuit::WireManager wireManager;
};

#endif // SCHEMATIC_H
