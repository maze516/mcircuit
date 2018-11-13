#include "ui_components/led.h"

void LEDUIComponent::paint(QPainter &painter) {
  QRect rect(getPosition() * context.gridDelta,
             QSize(2 * context.gridDelta, 2 * context.gridDelta));
  QPainterPath path;
  path.addEllipse(rect);

  if (component->getPin(mcircuit::LED::INPUT_PIN_ID)->getState() > 0) {
    painter.fillPath(path, Qt::green);
  } else {
    painter.fillPath(path, Qt::gray);
  }
  painter.setPen(Qt::black);
  painter.drawEllipse(rect);
}

QRect LEDUIComponent::getBoundingBox() {
  return QRect(getPosition() * context.gridDelta,
               QSize(context.gridDelta, context.gridDelta) * 2);
}

void LEDUIComponent::onMove(QPoint oldPosition) {
  wireManager.removePin(oldPosition.x(), oldPosition.y() + 1);

  createPins();
}

void LEDUIComponent::createPins() {
  wireManager.placePin(getPosition().x(), getPosition().y() + 1, component,
                       mcircuit::LED::INPUT_PIN_ID);
}
