#ifndef VARIABLE_INPUT_GATE_UI_COMPONENT_H
#define VARIABLE_INPUT_GATE_UI_COMPONENT_H

#include "ui_component.h"
#include <QtWidgets>
#include <mcircuit/components/basic_gate.h>

class BasicGateUIComponent
    : public UIComponent<mcircuit::BasicGate, BasicGateUIComponent> {
public:
  using UIComponent::UIComponent;

  void paint(QPainter &painter) override;
  QRect getBoundingBox() override;
  void onMove(QPoint oldPosition) override;

  void createPins() override;
};

#endif // VARIABLE_INPUT_GATE_H
