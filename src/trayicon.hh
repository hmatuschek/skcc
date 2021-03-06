#ifndef TRAYICON_HH
#define TRAYICON_HH

#include <QSystemTrayIcon>
#include "application.hh"
#include "popup.hh"
#include "mainwindow.hh"
#include "selfspotmap.hh"


class TrayIcon: public QSystemTrayIcon
{
	Q_OBJECT

public:
	TrayIcon(Application *app);
	virtual ~TrayIcon();

protected slots:
	void onClusterConnected();
	void onClusterDisconnected();
	void onShowSettings();
	void onNewDXCC(const Spot &spot);
	void onNewBand(const Spot &spot);
  void onNewMembership(const Spot &spot, const Membership &memb);
	void onNewFriend(const Spot &spot);

protected:
	Application *_app;
	PopUp *_popup;
	SpotWindow *_spotWindow;
  SelfSpotMapView *_mapView;
};

#endif // TRAYICON_HH
