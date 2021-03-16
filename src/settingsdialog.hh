#ifndef SETTINGSDIALOG_HH
#define SETTINGSDIALOG_HH

#include <QDialog>
#include <QLineEdit>
#include <QTableWidget>
#include "colorbutton.hh"
#include <QComboBox>
#include <QCheckBox>


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
  ColorButton *_newDXCC, *_selfSpot, *_friendSpot, *_beaconSpot, *_newBand, *_newSlot, *_newQSO, *_worked;
  ColorButton *_qthColor, *_spot2200m, *_spot630m, *_spot160m, *_spot80m, *_spot60m, *_spot40m;
  ColorButton *_spot30m, *_spot20m, *_spot17m, *_spot15m, *_spot12m, *_spot10m, *_spot6m;
  QComboBox *_iconTheme;
  QCheckBox *_showSKCC,*_showAGCW,*_showHSC,*_showVHSC,*_showSHSC,*_showEHSC;
  QCheckBox *_notifySKCC,*_notifyAGCW,*_notifyHSC,*_notifyVHSC,*_notifySHSC,*_notifyEHSC;
};

#endif // SETTINGSDIALOG_HH
