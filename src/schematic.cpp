#include "schematic.h"

void Schematic::notifySchematicChanged() {
  for (auto &uiComponent : references) {
    uiComponent->notifySchematicChanged(component.get());
  }
}
