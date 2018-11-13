#ifndef WIRE_DRAWING_CONTROLLER_H
#define WIRE_DRAWING_CONTROLLER_H

#include "mcircuit/wire_manager.h"
#include <QtWidgets>

class WireDrawingController {
public:
  enum class State { Inactive, Placing, Unallowed };

  WireDrawingController();

  void setWireManager(mcircuit::WireManager *wireManager);

  State getState() const;

  void startWirePlacement(const QPoint &position);

  void setWireEndPosition(const QPoint &position);

  void endWirePlacement();

  std::tuple<QLine, QLine> getPreviewWire();

private:
  State state;
  mcircuit::WireManager *wireManager;

  QPoint wireStart;
  QPoint wireEnd;

  QLine wire1;
  QLine wire2;
};

#endif
