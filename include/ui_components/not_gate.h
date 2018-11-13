#ifndef NOT_GATE_UI_COMPONENT_H
#define NOT_GATE_UI_COMPONENT_H

#include "ui_component.h"
#include <QtWidgets>
#include <mcircuit/components/not_gate.h>

class NotGateUIComponent
    : public UIComponent<mcircuit::NotGate, NotGateUIComponent> {
public:
  using UIComponent::UIComponent;

  void paint(QPainter &painter) override;
  QRect getBoundingBox() override;

  void onMove(QPoint oldPosition) override;

  void createPins() override;
};

#endif // NOT_GATE_H
