#ifndef TRAYICON_HH
#define TRAYICON_HH

#include <QSystemTrayIcon>
#include "application.hh"
#include "popup.hh"
#include "mainwindow.hh"


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
	void onNewSKCC(const Spot &spot);
	void onNewFriend(const Spot &spot);

protected:
	Application *_app;
	PopUp *_popup;
	SpotWindow *_spotWindow;
};

#endif // TRAYICON_HH
