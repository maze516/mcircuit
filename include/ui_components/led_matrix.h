#ifndef LED_MATRIX_UI_COMPONENT_H
#define LED_MATRIX_UI_COMPONENT_H

#include "ui_component.h"
#include <mcircuit/components/led_matrix.h>

class LEDMatrixUIComponent
    : public UIComponent<mcircuit::LEDMatrix, LEDMatrixUIComponent> {
public:
  using UIComponent::UIComponent;

  void paint(QPainter &painter) override;
  QRect getBoundingBox() override;

  void onMove(QPoint oldPosition) override;

  void createPins() override;

  void init() override;
  void processProperty(const PropertyId &property,
                       PropertyValue value) override;
};

#endif // LED_MATRIX_UI_COMPONENT_H
