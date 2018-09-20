#include "application.hh"
#include "settings.hh"
#include "bands.hh"
#include "settingsdialog.hh"

#include <QMenu>
#include <QActionGroup>
#include <QSound>
#include <QInputDialog>


Application::Application(int &argc, char *argv[])
    : QApplication(argc, argv)
{
	// Do not quit application if the last window is closed.
    setQuitOnLastWindowClosed(false);

    // Set application name
    setApplicationName("skcc");
    setOrganizationName("io.github.hmatuschek");
    setOrganizationDomain("io.github.hmatuschek");

    Settings settings;

    _spots = new SpotTable(settings.call(), settings.locator(), settings.logFile(),
                           settings.clusterHost(), settings.clusterPort(),
                           settings.showSelfSpots(), settings.maxSpotterDist(),
                           settings.maxSpotAge(), settings.minLogMatch(),
                           settings.maxSpeed(), settings.minSNR(), this);
    connect(_spots, SIGNAL(connected()), this, SLOT(onClusterConnected()));
    connect(_spots, SIGNAL(disconnected()), this, SLOT(onClusterDisconnected()));

    _spots->bands().clear();
    if (settings.band2200m()) _spots->bands().insert(BAND_2200M);
    if (settings.band630m())  _spots->bands().insert(BAND_630M);
    if (settings.band160m())  _spots->bands().insert(BAND_160M);
    if (settings.band80m())   _spots->bands().insert(BAND_80M);
    if (settings.band60m())   _spots->bands().insert(BAND_60M);
    if (settings.band40m())   _spots->bands().insert(BAND_40M);
    if (settings.band30m())   _spots->bands().insert(BAND_30M);
    if (settings.band20m())   _spots->bands().insert(BAND_20M);
    if (settings.band17m())   _spots->bands().insert(BAND_17M);
    if (settings.band15m())   _spots->bands().insert(BAND_15M);
    if (settings.band12m())   _spots->bands().insert(BAND_12M);
    if (settings.band10m())   _spots->bands().insert(BAND_10M);
    if (settings.band6m())    _spots->bands().insert(BAND_6M);

    _spots->friends() = settings.friends();

    QMenu *ctx = new QMenu();

    ctx->addSection(tr("Filter"));
    QMenu *menu = ctx->addMenu(QIcon("://icons/monitor-2x.png"), tr("Band"));
    QActionGroup *agroup = new QActionGroup(this);
    agroup->setExclusive(false);
    connect(agroup, SIGNAL(triggered(QAction*)), this, SLOT(onBandToggled(QAction*)));

    QAction *action = new QAction(tr("2200m"), this);
    action->setCheckable(true);
    action->setChecked(_spots->bands().contains(BAND_2200M));
    action->setData(BAND_2200M);
    agroup->addAction(action);
    menu->addAction(action);

    action = new QAction(tr("630m"), this);
    action->setCheckable(true);
    action->setChecked(_spots->bands().contains(BAND_630M));
    action->setData(BAND_630M);
    agroup->addAction(action);
    menu->addAction(action);

    action = new QAction(tr("160m"), this);
    action->setCheckable(true);
    action->setChecked(_spots->bands().contains(BAND_160M));
    action->setData(BAND_160M);
    agroup->addAction(action);
    menu->addAction(action);

    action = new QAction(tr("80m"), this);
    action->setCheckable(true);
    action->setChecked(_spots->bands().contains(BAND_80M));
    action->setData(BAND_80M);
    agroup->addAction(action);
    menu->addAction(action);

    action = new QAction(tr("60m"), this);
    action->setCheckable(true);
    action->setChecked(_spots->bands().contains(BAND_60M));
    action->setData(BAND_60M);
    agroup->addAction(action);
    menu->addAction(action);

    action = new QAction(tr("40m"), this);
    action->setCheckable(true);
    action->setChecked(_spots->bands().contains(BAND_40M));
    action->setData(BAND_40M);
    agroup->addAction(action);
    menu->addAction(action);

    action = new QAction(tr("30m"), this);
    action->setCheckable(true);
    action->setChecked(_spots->bands().contains(BAND_30M));
    action->setData(BAND_30M);
    agroup->addAction(action);
    menu->addAction(action);

    action = new QAction(tr("20m"), this);
    action->setCheckable(true);
    action->setChecked(_spots->bands().contains(BAND_20M));
    action->setData(BAND_20M);
    agroup->addAction(action);
    menu->addAction(action);

    action = new QAction(tr("17m"), this);
    action->setCheckable(true);
    action->setChecked(_spots->bands().contains(BAND_17M));
    action->setData(BAND_17M);
    agroup->addAction(action);
    menu->addAction(action);

    action = new QAction(tr("15m"), this);
    action->setCheckable(true);
    action->setChecked(_spots->bands().contains(BAND_15M));
    action->setData(BAND_15M);
    agroup->addAction(action);
    menu->addAction(action);

    action = new QAction(tr("12m"), this);
    action->setCheckable(true);
    action->setChecked(_spots->bands().contains(BAND_12M));
    action->setData(BAND_12M);
    agroup->addAction(action);
    menu->addAction(action);

    action = new QAction(tr("10m"), this);
    action->setCheckable(true);
    action->setChecked(_spots->bands().contains(BAND_10M));
    action->setData(BAND_10M);
    agroup->addAction(action);
    menu->addAction(action);

    action = new QAction(tr("6m"), this);
    action->setCheckable(true);
    action->setChecked(_spots->bands().contains(BAND_6M));
    action->setData(BAND_6M);
    agroup->addAction(action);
    menu->addAction(action);


    menu = ctx->addMenu(QIcon("://icons/book-2x.png"), tr("Log"));
    agroup = new QActionGroup(this);
    connect(agroup, SIGNAL(triggered(QAction*)), this, SLOT(onMatchSelected(QAction*)));

    action = new QAction(tr("Only new DXCC"), this);
    action->setCheckable(true);
    action->setChecked(LogFile::NEW_DXCC == _spots->minMatch());
    action->setData(uint(LogFile::NEW_DXCC));
    agroup->addAction(action);
    menu->addAction(action);

    action = new QAction(tr("Only new band"), this);
    action->setCheckable(true);
    action->setChecked(LogFile::NEW_BAND == _spots->minMatch());
    action->setData(uint(LogFile::NEW_BAND));
    agroup->addAction(action);
    menu->addAction(action);

    action = new QAction(tr("Only new slot"), this);
    action->setCheckable(true);
    action->setChecked(LogFile::NEW_SLOT == _spots->minMatch());
    action->setData(uint(LogFile::NEW_SLOT));
    agroup->addAction(action);
    menu->addAction(action);

    action = new QAction(tr("Only new QSO"), this);
    action->setCheckable(true);
    action->setChecked(LogFile::NEW_QSO == _spots->minMatch());
    action->setData(uint(LogFile::NEW_QSO));
    agroup->addAction(action);
    menu->addAction(action);

    action = new QAction(tr("Show all"), this);
    action->setCheckable(true);
    action->setChecked(LogFile::WORKED == _spots->minMatch());
    action->setData(uint(LogFile::WORKED));
    agroup->addAction(action);
    menu->addAction(action);


    menu = ctx->addMenu(QIcon("://icons/list-2x.png"), tr("Spot"));
    _maxAge = new QAction(tr("Max. age: %1s").arg(_spots->maxAge()));
    connect(_maxAge, SIGNAL(triggered(bool)), this, SLOT(onMaxAgeTriggered()));
    menu->addAction(_maxAge);

    _maxSpeed = new QAction(tr("Max. speed: %1wpm").arg(_spots->maxSpeed()));
    connect(_maxSpeed, SIGNAL(triggered(bool)), this, SLOT(onMaxSpeedTriggered()));
    menu->addAction(_maxSpeed);

    _minSNR = new QAction(tr("Min. SNR: %1db").arg(_spots->minSNR()));
    connect(_minSNR, SIGNAL(triggered(bool)), this, SLOT(onMinSNRTriggered()));
    menu->addAction(_minSNR);

    _maxDist = new QAction(tr("Max. spotter dist.: %1km").arg(_spots->maxDist()));
    connect(_maxDist, SIGNAL(triggered(bool)), this, SLOT(onMaxSpotterDistTriggered()));
    menu->addAction(_maxDist);

    action = menu->addAction(tr("Show self spots"));
    action->setCheckable(true);
    action->setChecked(Settings().showSelfSpots());
    connect(action, SIGNAL(toggled(bool)), this, SLOT(onShowSelfSpotsToggled(bool)));

    menu = ctx->addMenu(QIcon("://icons/bell-2x.png"), tr("Notify"));
    agroup = new QActionGroup(this);
    agroup->setExclusive(false);
    connect(agroup, SIGNAL(triggered(QAction*)), this, SLOT(onNotifyToggled(QAction*)));

    action = new QAction(tr("on new DXCC"), this);
    action->setCheckable(true);
    action->setChecked(Settings().notifyOnNewDXCC());
    action->setData("DXCC");
    agroup->addAction(action);
    menu->addAction(action);

    action = new QAction(tr("on new Band"), this);
    action->setCheckable(true);
    action->setChecked(Settings().notifyOnNewBand());
    action->setData("Band");
    agroup->addAction(action);
    menu->addAction(action);

    action = new QAction(tr("on new SKCC"), this);
    action->setCheckable(true);
    action->setChecked(Settings().notifyOnNewSKCC());
    action->setData("SKCC");
    agroup->addAction(action);
    menu->addAction(action);

    action = new QAction(tr("on new Friend"), this);
    action->setCheckable(true);
    action->setChecked(Settings().notifyOnNewFriend());
    action->setData("Friend");
    agroup->addAction(action);
    menu->addAction(action);

    menu->addSeparator();
    action = new QAction(tr("Play sound"), this);
    action->setCheckable(true);
    action->setChecked(Settings().notificationSoundEnabled());
    connect(action, SIGNAL(toggled(bool)), this, SLOT(onNotificationSoundToggled(bool)));
    menu->addAction(action);

    ctx->addSection(tr("Windows"));
    _spotWindow = new SpotWindow(_spots);
    _spotWindow->setVisible(false);
    ctx->addAction(QIcon("://icons/browser-2x.png"), tr("Show spot window"), _spotWindow, SLOT(show()));
    ctx->addAction(QIcon("://icons/wrench-2x.png"), tr("Settings ..."), this, SLOT(onShowSettings()));

    ctx->addSeparator();
    ctx->addAction(QIcon("://icons/power-standby-2x.png"), tr("Quit"), this, SLOT(quit()));

    // setup tray icon
    _trayIcon = new QSystemTrayIcon(this);
    _trayIcon->setIcon(QIcon("://icons/bullhorn-gray-2x.png"));
    _trayIcon->setContextMenu(ctx);
    _trayIcon->show();

    _popup = new PopUp();
    _popup->setProperty("popupDuration", 5000);
    connect(_spots, SIGNAL(newDXCC(Spot)), this, SLOT(onNewDXCC(Spot)));
    connect(_spots, SIGNAL(newBand(Spot)), this, SLOT(onNewBand(Spot)));
    connect(_spots, SIGNAL(newSKCC(Spot)), this, SLOT(onNewSKCC(Spot)));
}

Application::~Application() {
  _popup->deleteLater();
  _spotWindow->deleteLater();
}

void
Application::onClusterConnected() {
  _trayIcon->setIcon(QIcon("://icons/bullhorn-2x.png"));
}

void
Application::onClusterDisconnected() {
  _trayIcon->setIcon(QIcon("://icons/bullhorn-gray-2x.png"));
  _popup->setPopupText(tr("Disconnected from cluster!"));
}

void
Application::onBandToggled(QAction *action) {
  bool checked = action->isChecked();
  QString band = action->data().toString();

  if (checked)
    _spots->bands().insert(band);
  else
    _spots->bands().remove(band);

  if (BAND_2200M == band)
    Settings().setBand2200m(checked);
  else if (BAND_630M == band)
    Settings().setBand630m(checked);
  else if (BAND_160M == band)
    Settings().setBand160m(checked);
  else if (BAND_80M == band)
    Settings().setBand80m(checked);
  else if (BAND_60M == band)
    Settings().setBand60m(checked);
  else if (BAND_40M == band)
    Settings().setBand40m(checked);
  else if (BAND_30M == band)
    Settings().setBand30m(checked);
  else if (BAND_20M == band)
    Settings().setBand20m(checked);
  else if (BAND_17M == band)
    Settings().setBand17m(checked);
  else if (BAND_15M == band)
    Settings().setBand15m(checked);
  else if (BAND_12M == band)
    Settings().setBand12m(checked);
  else if (BAND_10M == band)
    Settings().setBand10m(checked);
  else if (BAND_6M == band)
    Settings().setBand6m(checked);
}

void
Application::onMatchSelected(QAction *action) {
  _spots->setMinMatch(LogFile::Match(action->data().toUInt()));
  Settings().setMinLogMatch(LogFile::Match(action->data().toUInt()));
}

void
Application::onMaxAgeTriggered() {
  bool ok=false;
  int maxAge = QInputDialog::getInt(nullptr, tr("Set max. age"), tr("Set maximum age of spots in seconds."),
                                    _spots->maxAge(), -1, 2147483647, 1, &ok);
  if (ok) {
    _maxAge->setText(tr("Max. age: %1s").arg(maxAge));
    _spots->setMaxAge(maxAge);
    Settings().setMaxSpotAge(maxAge);
  }
}

void
Application::onMaxSpeedTriggered() {
  bool ok=false;
  int maxSpeed = QInputDialog::getInt(nullptr, tr("Set max. speed"), tr("Set maximum CW speed in WPM."),
                                    _spots->maxSpeed(), -1, 100, 1, &ok);
  if (ok) {
    _maxSpeed->setText(tr("Max. speed: %1wpm").arg(maxSpeed));
    _spots->setMaxSpeed(maxSpeed);
    Settings().setMaxSpeed(maxSpeed);
  }
}

void
Application::onMinSNRTriggered() {
  bool ok=false;
  int minSNR = QInputDialog::getInt(nullptr, tr("Set min. SNR"), tr("Set minimum signal SNR in dB."),
                                    _spots->minSNR(), -1, 1000, 1, &ok);
  if (ok) {
    _minSNR->setText(tr("Min. SNR: %1db").arg(minSNR));
    _spots->setMinSNR(minSNR);
    Settings().setMinSNR(minSNR);
  }
}

void
Application::onMaxSpotterDistTriggered() {
  bool ok=false;
  int maxDist = QInputDialog::getInt(nullptr, tr("Set max. Spotter dist."), tr("Set maximum spotter distance in km."),
                                     _spots->maxDist(), -1, 2147483647, 1, &ok);
  if (ok) {
    _maxDist->setText(tr("Max. spotter sist.: %1km").arg(maxDist));
    _spots->setMaxDist(maxDist);
    Settings().setMaxSpotterDist(maxDist);
  }
}

void
Application::onNotifyToggled(QAction *action) {
  bool enabled = action->isChecked();
  if ("DXCC" == action->data().toString())
    Settings().setNotifyOnNewDXCC(enabled);
  else if ("Band" == action->data().toString())
    Settings().setNotifyOnNewBand(enabled);
  else if ("SKCC" == action->data().toString())
    Settings().setNotifyOnNewSKCC(enabled);
  else if ("Friend" == action->data().toString())
    Settings().setNotifyOnNewFriend(enabled);
}

void
Application::onNotificationSoundToggled(bool enable) {
  Settings().setNotificationSoundEnabled(enable);
}

void
Application::onShowSelfSpotsToggled(bool enable) {
  _spots->setShowSelfSpots(enable);
  Settings().setShowSelfSpots(enable);
}

void
Application::onShowSettings() {
  SettingsDialog dialog;
  if (QDialog::Accepted == dialog.exec()) {
    Settings settings;
    _spots->friends() = settings.friends();
  }
}

void
Application::onNewDXCC(const Spot &spot) {
  if (! Settings().notifyOnNewDXCC())
    return;

  qDebug() << "Notify new DXCC...";
  _popup->setPopupText(tr("New DXCC <b>%1</b> on <b>%2kHz</b> (%3dB, %4WPM)").arg(spot.spot)
                       .arg(spot.freq).arg(spot.db).arg(spot.wpm));
  _popup->show();

  if (Settings().notificationSoundEnabled())
    QSound::play(":/sound/single_chime.wav");
}

void
Application::onNewBand(const Spot &spot) {
  if (! Settings().notifyOnNewBand())
    return;

  qDebug() << "Notify new Band...";
  _popup->setPopupText(tr("New Band <b>%1</b> on <b>%2kHz</b> (%3dB, %4WPM)").arg(spot.spot)
                       .arg(spot.freq).arg(spot.db).arg(spot.wpm));
  _popup->show();

  if (Settings().notificationSoundEnabled())
    QSound::play(":/sound/single_chime.wav");
}

void
Application::onNewSKCC(const Spot &spot) {
  if (! Settings().notifyOnNewSKCC())
    return;
  qDebug() << "Notify new SKCC...";
  _popup->setPopupText(tr("New SKCC <b>%1</b> on <b>%2kHz</b> (%3dB, %4WPM)").arg(spot.spot)
                       .arg(spot.freq).arg(spot.db).arg(spot.wpm));
  _popup->show();

  if (Settings().notificationSoundEnabled())
    QSound::play(":/sound/single_chime.wav");
}

void
Application::onNewFriend(const Spot &spot) {
  if (! Settings().notifyOnNewFriend())
    return;
  qDebug() << "Notify Friend" << spot.spot;
  Friend fri = _spots->friends()[spot.spot];
  if (fri.name().isEmpty()) {
    _popup->setPopupText(tr("Friend <b>%1</b> on <b>%2kHz</b> (%3dB, %4WPM)").arg(spot.spot)
                         .arg(spot.freq).arg(spot.db).arg(spot.wpm));
  } else {
    _popup->setPopupText(tr("%1 <b>%2</b> on <b>%3kHz</b> (%4dB, %5WPM)")
                         .arg(fri.name()).arg(spot.spot)
                         .arg(spot.freq).arg(spot.db).arg(spot.wpm));

  }
  _popup->show();

  if (Settings().notificationSoundEnabled())
    QSound::play(":/sound/single_chime.wav");
}
