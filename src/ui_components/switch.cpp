#include "ui_components/switch.h"

void SwitchUIComponent::paint(QPainter &painter) {
  QRect baseRect(getPosition() * context.gridDelta,
                 QSize(1, 2) * context.gridDelta);

  for (auto i = 0; i < component->getWidth(); i++) {
    QRect rect = baseRect.translated(i * context.gridDelta, 0);

    if (component->getBitN(i)) {
      painter.fillRect(rect, Qt::green);
    } else {
      painter.fillRect(rect, Qt::gray);
    }

    painter.setPen(QPen(Qt::black, 2.5, Qt::SolidLine, Qt::PenCapStyle::RoundCap,
            Qt::PenJoinStyle::RoundJoin));
    painter.drawRect(rect);
  }
}

QRect SwitchUIComponent::getBoundingBox() {
  return QRect(getPosition() * context.gridDelta,
               QSize(component->getWidth(), 2) * context.gridDelta);
}

void SwitchUIComponent::onMove(QPoint oldPosition) {
  wireManager.removePin(oldPosition.x() + component->getWidth(),
                        oldPosition.y() + 1);

  createPins();
}

void SwitchUIComponent::onClick(QMouseEvent *event, QPoint translation) {
  if (event->button() == Qt::LeftButton) {
    QRect baseRect(getPosition() * context.gridDelta,
                   QSize(1, 2) * context.gridDelta);

    for (auto i = 0u; i < component->getWidth(); i++) {
      QRect rect = baseRect.translated(i * context.gridDelta, 0);

      if (rect.contains(event->pos() - translation)) {
        component->setBitN(i, !component->getBitN(i));
      }
    }
  }
}

void SwitchUIComponent::createPins() {
  wireManager.placePin(getPosition().x() + component->getWidth(),
                       getPosition().y() + 1, component,
                       mcircuit::Switch::OUTPUT_PIN_ID);
}

void SwitchUIComponent::processProperty(const PropertyId &property,
                                        PropertyValue value) {
  if (property == "width") {
    wireManager.removePin(getPosition().x() + (int)component->getWidth(),
                          getPosition().y() + 1);

    setProperty(property, value);

    component->setWidth(value.toUInt());

    createPins();
  }
}

void SwitchUIComponent::init() {
  UIComponent::init();

  setProperty("width", 1u);
}
