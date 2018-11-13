#include "controllers/wire_drawing_controller.h"
#include <iostream>

WireDrawingController::WireDrawingController() { state = State::Inactive; }

void WireDrawingController::setWireManager(mcircuit::WireManager *wireManager) {
  this->wireManager = wireManager;
}

WireDrawingController::State WireDrawingController::getState() const {
  return state;
}

void WireDrawingController::startWirePlacement(const QPoint &position) {
  wireStart = position;
  wireEnd = position;

  state = State::Placing;
}

void WireDrawingController::setWireEndPosition(const QPoint &position) {
  if (state != State::Placing)
    return;

  wireEnd = position;

  auto delta = wireEnd - wireStart;
  auto midPoint = wireStart + QPoint(delta.x(), 0);

  wire1.setP1(wireStart);
  wire1.setP2(midPoint);

  wire2.setP1(midPoint);
  wire2.setP2(midPoint + QPoint(0, delta.y()));
}

void WireDrawingController::endWirePlacement() {
  if (wireStart == wireEnd) {
    wireManager->overlap(wireStart.x(), wireStart.y());
  } else {

    wireManager->placeWire(mcircuit::WireManager::Horizontal, wire1.y1(),
                           wire1.x1(), wire1.x2());
    wireManager->placeWire(mcircuit::WireManager::Vertical, wire2.x1(),
                           wire2.y1(), wire2.y2());
  }

  state = State::Inactive;
}

std::tuple<QLine, QLine> WireDrawingController::getPreviewWire() {
  return std::make_tuple(wire1, wire2);
}
