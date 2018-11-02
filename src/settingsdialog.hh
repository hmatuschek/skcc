#ifndef SETTINGSDIALOG_HH
#define SETTINGSDIALOG_HH

#include <QDialog>
#include <QLineEdit>
#include <QTableWidget>
#include "colorbutton.hh"

class SettingsDialog : public QDialog
{
  Q_OBJECT

public:
  explicit SettingsDialog(QWidget *parent = nullptr);

public slots:
	void accept();

private slots:
	void onSelectLogFile();
  void onAddFriend();
  void onRemFriend();

private:
	QLineEdit *_call;
	QLineEdit *_locator;
	QLineEdit *_clusterHost;
	QLineEdit *_clusterPort;
	QLineEdit *_logFile;
  QTableWidget *_friends;
  ColorButton *_newDXCC, *_selfSpot, *_friendSpot, *_newBand, *_newSlot, *_newQSO, *_worked;
};

#endif // SETTINGSDIALOG_HH
