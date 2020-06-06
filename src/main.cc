#include "settings.hh"
#include "application.hh"
#include <QTableView>
#include <QSortFilterProxyModel>
#include "settingsdialog.hh"
#include "trayicon.hh"

#include <QDir>
#include <QStandardPaths>
#include <QFile>
#include <QtGlobal>

void logFileMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
  QDir appdir(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));
  QString filename(appdir.absolutePath() + "/rbnc.log");
  QFile logFile(filename);
  logFile.open(QFile::Append|QFile::Text);
  QTextStream stream(&logFile);

  QByteArray localMsg = msg.toLocal8Bit();
  switch (type) {
  case QtDebugMsg: stream << "Debug: "; break;
  case QtInfoMsg: stream << "Info: "; break;
  case QtWarningMsg: stream << "Warning: "; break;
  case QtCriticalMsg: stream << "Critical: "; break;
  case QtFatalMsg: stream << "Fatal: "; break;
  }
  stream << "(" << context.file << ":" << context.line << ", " << context.function << "): "
         << localMsg.constData() << "\n";

  logFile.close();
}

int main(int argc, char *argv[])
{
  qInstallMessageHandler(logFileMessageOutput); // Install the handler

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

