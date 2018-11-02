#include "settings.hh"
#include "application.hh"
#include <QTableView>
#include <QSortFilterProxyModel>
#include "settingsdialog.hh"
#include "trayicon.hh"

int main(int argc, char *argv[])
{
  Settings settings;
  while (! settings.isComplete()) {
    QApplication app(argc, argv);
    SettingsDialog dialog;
    dialog.setModal(true);
    if (QDialog::Rejected == dialog.exec())
      return 0;
  }

  Application app(argc, argv);

  TrayIcon icon(&app);
  icon.show();

  app.exec();

  return 0;
}

