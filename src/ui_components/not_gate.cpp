#include "ui_components/not_gate.h"

void NotGateUIComponent::paint(QPainter &painter) {
  auto gridDelta = context.gridDelta;

  QRect rect(QPoint(0, 0), QSize(3, 2) * gridDelta);
  rect.translate(getPosition() * gridDelta);

  painter.fillRect(rect, Qt::white);
  painter.setPen(Qt::black);
  painter.drawRect(rect);

  auto font = painter.font();
  font.setPixelSize(15);

  painter.setFont(font);

  painter.drawText(rect, Qt::AlignCenter, "1");
}

QRect NotGateUIComponent::getBoundingBox() {
  return QRect(getPosition() * context.gridDelta,
               QSize(3, 2) * context.gridDelta);
}

void NotGateUIComponent::onMove(QPoint oldPosition) {
  int x = oldPosition.x();
  int y = oldPosition.y();

  wireManager.removePin(x, y + 1);
  wireManager.removePin(x + 3, y + 1);

  createPins();
}

void NotGateUIComponent::createPins() {
  wireManager.placePin(getPosition().x(), getPosition().y() + 1, component,
                       mcircuit::NotGate::INPUT_PIN_ID);

  wireManager.placePin(getPosition().x() + 3, getPosition().y() + 1, component,
                       mcircuit::NotGate::OUTPUT_PIN_ID);
}
