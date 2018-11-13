#include "ui_components/led_matrix.h"

void LEDMatrixUIComponent::paint(QPainter &painter) {
  auto pos = getPosition();
  auto startPos =
      QPoint(pos.x() * context.gridDelta - context.gridDelta / 2,
             (pos.y() - component->getLEDHeight()) * context.gridDelta);

  for (auto i = 0; i < component->getLEDWidth(); i++) {
    for (auto j = 0; j < component->getLEDHeight(); j++) {
      QColor color = Qt::gray;

      if (component->isPixelOn(i, j)) {
        color = Qt::green;
      }

      QRect rect(startPos + QPoint(i, j) * context.gridDelta,
                 QSize(1, 1) * context.gridDelta);

      painter.fillRect(rect, color);
    }
  }
}

QRect LEDMatrixUIComponent::getBoundingBox() {
  auto pos = getPosition();

  return QRect(
      QPoint(pos.x() * context.gridDelta,
             (pos.y() - component->getLEDHeight()) * context.gridDelta),
      QSize(component->getLEDWidth(), component->getLEDHeight()) *
          context.gridDelta);
}

void LEDMatrixUIComponent::onMove(QPoint oldPosition) {
  for (auto i = 0; i < component->getNumInputs(); i++) {
    wireManager.removePin(oldPosition.x() + i, oldPosition.y());
  }

  createPins();
}

void LEDMatrixUIComponent::createPins() {
  for (auto i = 0; i < component->getNumInputs(); i++) {
    wireManager.placePin(getPosition().x() + i, getPosition().y(), component,
                         i);
  }
}

void LEDMatrixUIComponent::init() {
  UIComponent::init();

  setProperty("width", 1u);
  setProperty("height", 1u);
}

void LEDMatrixUIComponent::processProperty(
    const UIComponentBase::PropertyId &property,
    UIComponentBase::PropertyValue value) {
  const auto &position = getPosition();

  if (property == "width") {
    for (auto i = 0; i < component->getNumInputs(); i++) {
      wireManager.removePin(position.x() + i, position.y());
    }

    auto typedValue = value.toUInt();

    setProperty("width", typedValue);

    component->setLEDWidth(typedValue);

    createPins();
  } else if (property == "height") {
    for (auto i = 0; i < component->getNumInputs(); i++) {
      wireManager.removePin(position.x() + i, position.y());
    }

    auto typedValue = value.toUInt();

    setProperty("height", typedValue);

    component->setLEDHeight(typedValue);

    createPins();
  }
}
