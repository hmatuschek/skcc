#ifndef MENU_HH
#define MENU_HH

#include <QMenu>
#include <QAction>
#include "application.hh"


class Menu: public QMenu
{
	Q_OBJECT

public:
	Menu(Application *app, QWidget *parent=nullptr);

	inline QAction *showSpotWindow() { return _showSpotWindow; }
	inline QAction *showSettings() { return _showSettings; }

protected slots:
	void onBandToggled(QAction *action);
	void onMatchSelected(QAction *action);
	void onMaxAgeTriggered();
	void onMaxSpeedTriggered();
	void onMinSNRTriggered();
	void onMaxSpotterDistTriggered();
	void onShowSelfSpotsToggled(bool enabled);
	void onNotifyToggled(QAction *action);
	void onNotificationSoundToggled(bool enabled);

protected:
	Application *_app;
	QAction *_maxAge, *_maxSpeed, *_minSNR, *_maxDist, *_showSpotWindow, *_showSettings;
};

#endif // MENU_HH
