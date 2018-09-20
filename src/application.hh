#ifndef APPLICATION_HH
#define APPLICATION_HH

#include <QApplication>
#include <QAction>
#include <QSystemTrayIcon>

#include "popup.hh"
#include "spottable.hh"
#include "mainwindow.hh"


class Application : public QApplication
{
	Q_OBJECT

public:
  Application(int &argc, char *argv[]);
  virtual ~Application();

private slots:
  void onClusterConnected();
  void onClusterDisconnected();

	void onBandToggled(QAction *action);
	void onMatchSelected(QAction *action);
	void onMaxAgeTriggered();
	void onMaxSpeedTriggered();
	void onMinSNRTriggered();
	void onMaxSpotterDistTriggered();
	void onNotifyToggled(QAction *action);
	void onNotificationSoundToggled(bool enable);
	void onShowSelfSpotsToggled(bool enable);
	void onShowSettings();

	void onNewDXCC(const Spot &spot);
	void onNewBand(const Spot &spot);
	void onNewSKCC(const Spot &spot);
  void onNewFriend(const Spot &spot);

private:
  SpotTable *_spots;
  QSystemTrayIcon *_trayIcon;
  SpotWindow *_spotWindow;
  QAction *_maxAge;
  QAction *_maxSpeed;
  QAction *_minSNR;
  QAction *_maxDist;
  PopUp   *_popup;
};

#endif // APPLICATION_HH
