//
// Created by Matija on 11/11/2018.
//

#ifndef MCIRCUIT_UI_COMPONENT_RENDERER_H
#define MCIRCUIT_UI_COMPONENT_RENDERER_H

#include "context.h"
#include "controllers/wire_drawing_controller.h"
#include "mcircuit/wire_manager.h"
#include "ui_component.h"
#include <QtWidgets>

class Renderer {
public:
  Renderer(Context &context);

  void drawWires(const mcircuit::WireManager &wireManager) const;

  void drawPins(const mcircuit::WireManager &wireManager) const;

  void drawWireInspectTooltip(int x, int y,
                              mcircuit::WireManager &wireManager) const;

  void setPainter(QPainter *painter);

  void drawWireDrawingCursor(int x, int y) const;

  void drawWirePreview(WireDrawingController::State state,
                       std::tuple<QLine, QLine> lines) const;

  void drawGrid(const QPoint &transl) const;

  void drawPin(int x, int y) const;

private:
  void createGridPixmap();

  const QColor CIRCUIT_BACKGROUND_COLOR = Qt::white;
  const QColor CIRCUIT_GRID_COLOR = QColor::fromRgbF(0.5, 0.5, 0.5, 1.0);

  int gridDelta;

  QPainter *painter;
  QPixmap gridPixmap;
};

#endif // MCIRCUIT_UI_COMPONENT_RENDERER_H
