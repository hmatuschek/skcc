#include "settings.hh"
#include "application.hh"
#include <QTableView>
#include <QSortFilterProxyModel>
#include "settingsdialog.hh"


int main(int argc, char *argv[])
{
  Settings settings;
  while (! settings.isComplete()) {
    SettingsDialog dialog;
    dialog.setModal(true);
    if (QDialog::Rejected == dialog.exec())
      return 0;
  }

  Application app(argc, argv);

  app.exec();

  return 0;
}

