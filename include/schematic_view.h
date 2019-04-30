#ifndef CIRCUITVIEW_H
#define CIRCUITVIEW_H

#include "context.h"
#include "controllers/wire_drawing_controller.h"
#include "mcircuit/wire_manager.h"
#include "renderer.h"
#include "schematic.h"
#include "ui_component.h"
#include <QWidget>
#include <condition_variable>
#include <mcircuit/components/custom_component.h>
#include <mutex>
#include <thread>

class SchematicView : public QWidget {
  Q_OBJECT

public:
  enum class Mode { Editing, Interactive };

  SchematicView(Context &context);

  void paintEvent(QPaintEvent *event) override;

  void mouseMoveEvent(QMouseEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;

  void keyReleaseEvent(QKeyEvent *event) override;

  void setPlacingComponent(UIComponentBase *component);

  void stopSimulation();

  void setMode(Mode mode);

  void setSchematic(Schematic *schematic);

signals:
  void uiComponentSelected(UIComponentBase *component);

private:
  UIComponentBase *getComponentOnPosition(const QPoint &position);
  UIComponentBase *getComponentUnderCursor();

  Schematic *schematic = nullptr;

  WireDrawingController wireDrawingController;

  Context &context;

  Renderer renderer;

  QPoint mousePosition;

  QTimer timer, benchTimer;
  std::atomic_uint ticks = 0;
  unsigned freq = 0;

  std::unique_ptr<UIComponentBase> placingComponent;

  std::atomic_bool running;
  std::thread simulationThread;
  std::mutex mutex;

  Mode mode;

  bool panning = false;
  QPoint panStart;
  QPoint transl;
  QPoint translDelta;
  QPoint translStart;
};

#endif // CIRCUITVIEW_H
