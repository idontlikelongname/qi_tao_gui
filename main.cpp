#include <QApplication>

#include "dialog.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  Dialog main_dialog;
  main_dialog.setWindowState(main_dialog.windowState() | Qt::WindowMaximized);
  main_dialog.show();

  return app.exec();
}
