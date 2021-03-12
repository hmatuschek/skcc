#include "menu.hh"
#include <QActionGroup>
#include <QInputDialog>
#include "bands.hh"
#include "icons.hh"
#include "aboutdialog.hh"


Menu::Menu(Application *app, QWidget *parent)
    : QMenu(parent), _app(app)
{
    addSection(tr("Filter"));
    QMenu *menu = addMenu(IconProvider::get(IconProvider::MENU_FILTER_ICON), tr("Band"));
    QActionGroup *agroup = new QActionGroup(this);
    agroup->setExclusive(false);
    connect(agroup, SIGNAL(triggered(QAction*)), this, SLOT(onBandToggled(QAction*)));

    QAction *action = new QAction(tr("2200m"), this);
    action->setCheckable(true);
    action->setChecked(_app->spots()->bands().contains(BAND_2200M));
    action->setData(BAND_2200M);
    agroup->addAction(action);
    menu->addAction(action);

    action = new QAction(tr("630m"), this);
    action->setCheckable(true);
    action->setChecked(_app->spots()->bands().contains(BAND_630M));
    action->setData(BAND_630M);
    agroup->addAction(action);
    menu->addAction(action);

    action = new QAction(tr("160m"), this);
    action->setCheckable(true);
    action->setChecked(_app->spots()->bands().contains(BAND_160M));
    action->setData(BAND_160M);
    agroup->addAction(action);
    menu->addAction(action);

    action = new QAction(tr("80m"), this);
    action->setCheckable(true);
    action->setChecked(_app->spots()->bands().contains(BAND_80M));
    action->setData(BAND_80M);
    agroup->addAction(action);
    menu->addAction(action);

    action = new QAction(tr("60m"), this);
    action->setCheckable(true);
    action->setChecked(_app->spots()->bands().contains(BAND_60M));
    action->setData(BAND_60M);
    agroup->addAction(action);
    menu->addAction(action);

    action = new QAction(tr("40m"), this);
    action->setCheckable(true);
    action->setChecked(_app->spots()->bands().contains(BAND_40M));
    action->setData(BAND_40M);
    agroup->addAction(action);
    menu->addAction(action);

    action = new QAction(tr("30m"), this);
    action->setCheckable(true);
    action->setChecked(_app->spots()->bands().contains(BAND_30M));
    action->setData(BAND_30M);
    agroup->addAction(action);
    menu->addAction(action);

    action = new QAction(tr("20m"), this);
    action->setCheckable(true);
    action->setChecked(_app->spots()->bands().contains(BAND_20M));
    action->setData(BAND_20M);
    agroup->addAction(action);
    menu->addAction(action);

    action = new QAction(tr("17m"), this);
    action->setCheckable(true);
    action->setChecked(_app->spots()->bands().contains(BAND_17M));
    action->setData(BAND_17M);
    agroup->addAction(action);
    menu->addAction(action);

    action = new QAction(tr("15m"), this);
    action->setCheckable(true);
    action->setChecked(_app->spots()->bands().contains(BAND_15M));
    action->setData(BAND_15M);
    agroup->addAction(action);
    menu->addAction(action);

    action = new QAction(tr("12m"), this);
    action->setCheckable(true);
    action->setChecked(_app->spots()->bands().contains(BAND_12M));
    action->setData(BAND_12M);
    agroup->addAction(action);
    menu->addAction(action);

    action = new QAction(tr("10m"), this);
    action->setCheckable(true);
    action->setChecked(_app->spots()->bands().contains(BAND_10M));
    action->setData(BAND_10M);
    agroup->addAction(action);
    menu->addAction(action);

    action = new QAction(tr("6m"), this);
    action->setCheckable(true);
    action->setChecked(_app->spots()->bands().contains(BAND_6M));
    action->setData(BAND_6M);
    agroup->addAction(action);
    menu->addAction(action);


    menu = addMenu(IconProvider::get(IconProvider::MENU_LOG_ICON), tr("Log"));
    agroup = new QActionGroup(this);
    connect(agroup, SIGNAL(triggered(QAction*)), this, SLOT(onMatchSelected(QAction*)));

    action = new QAction(tr("Only new DXCC"), this);
    action->setCheckable(true);
    action->setChecked(LogFile::NEW_DXCC == _app->spots()->minMatch());
    action->setData(uint(LogFile::NEW_DXCC));
    agroup->addAction(action);
    menu->addAction(action);

    action = new QAction(tr("Only new band"), this);
    action->setCheckable(true);
    action->setChecked(LogFile::NEW_BAND == _app->spots()->minMatch());
    action->setData(uint(LogFile::NEW_BAND));
    agroup->addAction(action);
    menu->addAction(action);

    action = new QAction(tr("Only new slot"), this);
    action->setCheckable(true);
    action->setChecked(LogFile::NEW_SLOT == _app->spots()->minMatch());
    action->setData(uint(LogFile::NEW_SLOT));
    agroup->addAction(action);
    menu->addAction(action);

    action = new QAction(tr("Only new QSO"), this);
    action->setCheckable(true);
    action->setChecked(LogFile::NEW_QSO == _app->spots()->minMatch());
    action->setData(uint(LogFile::NEW_QSO));
    agroup->addAction(action);
    menu->addAction(action);

    action = new QAction(tr("Show all"), this);
    action->setCheckable(true);
    action->setChecked(LogFile::WORKED == _app->spots()->minMatch());
    action->setData(uint(LogFile::WORKED));
    agroup->addAction(action);
    menu->addAction(action);

    menu = addMenu(IconProvider::get(IconProvider::MENU_SPOT_ICON), tr("Spot"));
    _maxAge = new QAction(tr("Max. age: %1s").arg(_app->spots()->maxAge()), this);
    connect(_maxAge, SIGNAL(triggered(bool)), this, SLOT(onMaxAgeTriggered()));
    menu->addAction(_maxAge);

    _maxSpeed = new QAction(tr("Max. speed: %1wpm").arg(_app->spots()->maxSpeed()), this);
    connect(_maxSpeed, SIGNAL(triggered(bool)), this, SLOT(onMaxSpeedTriggered()));
    menu->addAction(_maxSpeed);

    _minSNR = new QAction(tr("Min. SNR: %1db").arg(_app->spots()->minSNR()), this);
    connect(_minSNR, SIGNAL(triggered(bool)), this, SLOT(onMinSNRTriggered()));
    menu->addAction(_minSNR);

    _maxDist = new QAction(tr("Max. spotter dist.: %1km").arg(_app->spots()->maxDist()), this);
    connect(_maxDist, SIGNAL(triggered(bool)), this, SLOT(onMaxSpotterDistTriggered()));
    menu->addAction(_maxDist);

    action = menu->addAction(tr("Show self spots"));
    action->setCheckable(true);
    action->setChecked(Settings().showSelfSpots());
    connect(action, SIGNAL(toggled(bool)), this, SLOT(onShowSelfSpotsToggled(bool)));

    action = menu->addAction(tr("Show beacon spots"));
    action->setCheckable(true);
    action->setChecked(Settings().showBeaconSpots());
    connect(action, SIGNAL(toggled(bool)), this, SLOT(onShowBeaconSpotsToggled(bool)));

    menu = addMenu(IconProvider::get(IconProvider::MENU_NOTIFY_ICON), tr("Notify"));
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

    addSection(tr("Windows"));
    _showSpotWindow = addAction(IconProvider::get(IconProvider::MENU_SPOT_WINDOW_ICON), tr("Show spot window"));
    _showMapWindow = addAction(IconProvider::get(IconProvider::MENU_MAP_WINDOW_ICON), tr("Show map window"));
    _showSettings = addAction(IconProvider::get(IconProvider::MENU_SETTINGS_ICON), tr("Settings ..."));
    addAction(IconProvider::get(IconProvider::MENU_ABOUT_ICON), tr("About RBNC ..."), this, SLOT(onShowAbout()));
    addSeparator();
    addAction(IconProvider::get(IconProvider::MENU_QUIT_ICON), tr("Quit"), _app, SLOT(quit()));
}


void
Menu::onBandToggled(QAction *action) {
  bool checked = action->isChecked();
  QString band = action->data().toString();

  if (checked)
    _app->spots()->bands().insert(band);
  else
    _app->spots()->bands().remove(band);

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
Menu::onMatchSelected(QAction *action) {
  _app->spots()->setMinMatch(LogFile::Match(action->data().toUInt()));
  Settings().setMinLogMatch(LogFile::Match(action->data().toUInt()));
}

void
Menu::onMaxAgeTriggered() {
  bool ok=false;
  int maxAge = QInputDialog::getInt(
        nullptr, tr("Set max. age"), tr("Set maximum age of spots in seconds."),
        _app->spots()->maxAge(), -1, 2147483647, 1, &ok);
  if (ok) {
    _maxAge->setText(tr("Max. age: %1s").arg(maxAge));
    _app->spots()->setMaxAge(maxAge);
    Settings().setMaxSpotAge(maxAge);
  }
}

void
Menu::onMaxSpeedTriggered() {
  bool ok=false;
  int maxSpeed = QInputDialog::getInt(
        nullptr, tr("Set max. speed"), tr("Set maximum CW speed in WPM."),
        _app->spots()->maxSpeed(), -1, 100, 1, &ok);
  if (ok) {
    _maxSpeed->setText(tr("Max. speed: %1wpm").arg(maxSpeed));
    _app->spots()->setMaxSpeed(maxSpeed);
    Settings().setMaxSpeed(maxSpeed);
  }
}

void
Menu::onMinSNRTriggered() {
  bool ok=false;
  int minSNR = QInputDialog::getInt(
        nullptr, tr("Set min. SNR"), tr("Set minimum signal SNR in dB."),
        _app->spots()->minSNR(), -1, 1000, 1, &ok);
  if (ok) {
    _minSNR->setText(tr("Min. SNR: %1db").arg(minSNR));
    _app->spots()->setMinSNR(minSNR);
    Settings().setMinSNR(minSNR);
  }
}

void
Menu::onMaxSpotterDistTriggered() {
  bool ok=false;
  int maxDist = QInputDialog::getInt(
        nullptr, tr("Set max. Spotter dist."), tr("Set maximum spotter distance in km."),
        _app->spots()->maxDist(), -1, 2147483647, 1, &ok);
  if (ok) {
    _maxDist->setText(tr("Max. spotter sist.: %1km").arg(maxDist));
    _app->spots()->setMaxDist(maxDist);
    Settings().setMaxSpotterDist(maxDist);
  }
}

void
Menu::onNotifyToggled(QAction *action) {
  bool enabled = action->isChecked();
  if ("DXCC" == action->data().toString())
    Settings().setNotifyOnNewDXCC(enabled);
  else if ("Band" == action->data().toString())
    Settings().setNotifyOnNewBand(enabled);
  else if ("Friend" == action->data().toString())
    Settings().setNotifyOnNewFriend(enabled);
}

void
Menu::onNotificationSoundToggled(bool enable) {
  Settings().setNotificationSoundEnabled(enable);
}

void
Menu::onShowSelfSpotsToggled(bool enable) {
  _app->spots()->setShowSelfSpots(enable);
  Settings().setShowSelfSpots(enable);
}

void
Menu::onShowBeaconSpotsToggled(bool enable) {
  _app->spots()->setShowBeaconSpots(enable);
  Settings().setShowBeaconSpots(enable);
}

void
Menu::onShowAbout() {
  AboutDialog dialog;
  dialog.exec();
}
