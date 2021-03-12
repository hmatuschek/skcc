#include "settings.hh"
#include <QSet>
#include <QDebug>


Settings::Settings()
    : QSettings("io.github.hmatuschek", "skcc")
{
	// pass...
}

bool
Settings::isComplete() const {
  return contains("call") && contains("locator") && contains("clusterHost") &&
      contains("clusterPort") && contains("logFile");
}


bool
Settings::band2200m() const {
  return value("band2200m",true).toBool();
}

void
Settings::setBand2200m(bool enable) {
  setValue("band2200m", enable);
}

bool
Settings::band630m() const {
  return value("band630m",true).toBool();
}

void
Settings::setBand630m(bool enable) {
  setValue("band630m", enable);
}

bool
Settings::band160m() const {
  return value("band160m",true).toBool();
}

void
Settings::setBand160m(bool enable) {
  setValue("band160m", enable);
}

bool
Settings::band80m() const {
  return value("band80m",true).toBool();
}

void
Settings::setBand80m(bool enable) {
  setValue("band80m", enable);
}

bool
Settings::band60m() const {
  return value("band60m",true).toBool();
}

void
Settings::setBand60m(bool enable) {
  setValue("band60m", enable);
}

bool
Settings::band40m() const {
  return value("band40m",true).toBool();
}

void
Settings::setBand40m(bool enable) {
  setValue("band40m", enable);
}

bool
Settings::band30m() const {
  return value("band30m",true).toBool();
}

void
Settings::setBand30m(bool enable) {
  setValue("band30m", enable);
}

bool
Settings::band20m() const {
  return value("band20m",true).toBool();
}

void
Settings::setBand20m(bool enable) {
  setValue("band20m", enable);
}

bool
Settings::band17m() const {
  return value("band17m",true).toBool();
}

void
Settings::setBand17m(bool enable) {
  setValue("band17m", enable);
}

bool
Settings::band15m() const {
  return value("band15m",true).toBool();
}

void
Settings::setBand15m(bool enable) {
  setValue("band15m", enable);
}

bool
Settings::band12m() const {
  return value("band12m",true).toBool();
}

void
Settings::setBand12m(bool enable) {
  setValue("band12m", enable);
}

bool
Settings::band10m() const {
  return value("band10m",true).toBool();
}

void
Settings::setBand10m(bool enable) {
  setValue("band10m", enable);
}

bool
Settings::band6m() const {
  return value("band6m",true).toBool();
}

void
Settings::setBand6m(bool enable) {
  setValue("band6m", enable);
}

QString
Settings::call() const {
  return value("call").toString();
}

void
Settings::setCall(const QString &call) {
  setValue("call", call);
}

QString
Settings::locator() const {
  return value("locator").toString();
}

void
Settings::setLocator(const QString &locator) {
  setValue("locator", locator);
}

QString
Settings::clusterHost() const {
  return value("clusterHost", "telnet.reversebeacon.net").toString();
}

void
Settings::setClusterHost(const QString &addr) {
  setValue("clusterHost", addr);
}

quint16
Settings::clusterPort() const {
  return value("clusterPort", 7000).toUInt();
}

void
Settings::setClusterPort(quint16 port) {
  setValue("clusterPort", port);
}

bool
Settings::showSelfSpots() const {
  return value("showSelf", true).toBool();
}

void
Settings::setShowSelfSpots(bool show) {
  setValue("showSelf", show);
}

bool
Settings::showBeaconSpots() const {
  return value("showBeacon", true).toBool();
}

void
Settings::setShowBeaconSpots(bool show) {
  setValue("showBeacon", show);
}

int
Settings::maxSpotterDist() const {
  return value("maxDist", 1000).toInt();
}

void
Settings::setMaxSpotterDist(int dist) {
  setValue("maxDist", dist);
}

int
Settings::maxSpotAge() const {
  return value("maxAge", 900).toInt();
}

void
Settings::setMaxSpotAge(int age) {
  setValue("maxAge", age);
}

QString
Settings::logFile() const {
  return value("logFile").toString();
}
void
Settings::setLogFile(const QString filename) {
  setValue("logFile", filename);
}

LogFile::Match
Settings::minLogMatch() const {
  return LogFile::Match(value("minMatch", LogFile::WORKED).toUInt());
}

void
Settings::setMinLogMatch(LogFile::Match match) {
  setValue("minMatch", uint(match));
}

int
Settings::maxSpeed() const {
  return value("maxSpeed", -1).toInt();
}

void
Settings::setMaxSpeed(int speed) {
  setValue("maxSpeed", speed);
}

int
Settings::minSNR() const {
  return value("minSNR", -1).toInt();
}

void
Settings::setMinSNR(int snr) {
  setValue("minSNR", snr);
}

bool
Settings::notificationSoundEnabled() const {
  return value("notify/sound", false).toBool();
}

void
Settings::setNotificationSoundEnabled(bool enable) {
  setValue("notify/sound", enable);
}

bool
Settings::notifyOnNewDXCC() const {
  return value("notify/onNewDXCC", false).toBool();
}

void
Settings::setNotifyOnNewDXCC(bool enable) {
  setValue("notify/onNewDXCC", enable);
}

bool
Settings::notifyOnNewBand() const {
  return value("notify/onNewBand", false).toBool();
}

void
Settings::setNotifyOnNewBand(bool enable) {
  setValue("notify/onNewBand", enable);
}

bool
Settings::notifyOnNewSKCC() const {
  return value("notify/onNewSKCC", true).toBool();
}

void
Settings::setNotifyOnNewSKCC(bool enable) {
  setValue("notify/onNewSKCC", enable);
}

bool
Settings::notifyOnNewAGCW() const {
  return value("notify/onNewAGCW", true).toBool();
}

void
Settings::setNotifyOnNewAGCW(bool enable) {
  setValue("notify/onNewAGCW", enable);
}

bool
Settings::notifyOnNewHSC() const {
  return value("notify/onNewHSC", true).toBool();
}

void
Settings::setNotifyOnNewHSC(bool enable) {
  setValue("notify/onNewHSC", enable);
}

bool
Settings::notifyOnNewFriend() const {
  return value("notify/onNewFriend", true).toBool();
}

void
Settings::setNotifyOnNewFriend(bool enable) {
  setValue("notify/onNewFriend", enable);
}

Friends
Settings::friends()
{
  Friends friends;
  int num = beginReadArray("friends");
  for (int i=0; i<num; i++) {
    setArrayIndex(i);
    QString call    = value("call").toString();
    QString name    = value("name").toString();
    QString comment = value("comment").toString();
    friends.addFriend(call, name, comment);
  }
  endArray();

  return friends;
}

void
Settings::setFriends(const Friends &friends) {
  beginWriteArray("friends");
  Friends::iterator item = friends.begin();
  for (int i=0; item!=friends.end(); item++, i++) {
    setArrayIndex(i);
    setValue("call", item->call());
    setValue("name", item->name());
    setValue("comment", item->comment());
  }
  endArray();
}

QColor
Settings::selfSpotColor() const {
  return value("colors/selfSpot", QColor(Qt::gray)).value<QColor>();
}
void
Settings::setSelfSpotColor(const QColor &color) {
  setValue("colors/selfSpot", color);
}

QColor
Settings::friendSpotColor() const {
  return value("colors/friendSpot", QColor(Qt::blue)).value<QColor>();
}
void
Settings::setFriendSpotColor(const QColor &color) {
  setValue("colors/friendSpot", color);
}

QColor
Settings::beaconSpotColor() const {
  return value("colors/beaconSpot", QColor(Qt::white)).value<QColor>();
}
void
Settings::setBeaconSpotColor(const QColor &color) {
  setValue("colors/beaconSpot", color);
}

QColor
Settings::newDXCCColor() const {
  return value("colors/newDXCC", QColor(Qt::magenta)).value<QColor>();
}
void
Settings::setNewDXCCColor(const QColor &color) {
  setValue("colors/newDXCC", color);
}

QColor
Settings::newBandColor() const {
  return value("colors/newBand", QColor(Qt::red)).value<QColor>();
}
void
Settings::setNewBandColor(const QColor &color) {
  setValue("colors/newBand", color);
}

QColor
Settings::newSlotColor() const {
  return value("colors/newSlot", QColor(Qt::yellow)).value<QColor>();
}
void
Settings::setNewSlotColor(const QColor &color) {
  setValue("colors/newSlot", color);
}

QColor
Settings::newQSOColor() const {
  return value("colors/newQSO", QColor(Qt::white)).value<QColor>();
}
void
Settings::setNewQSOColor(const QColor &color) {
  setValue("colors/newQSO", color);
}

QColor
Settings::workedColor() const {
  return value("colors/worked", QColor(Qt::green)).value<QColor>();
}
void
Settings::setWorkedColor(const QColor &color) {
  setValue("colors/worked", color);
}

QColor
Settings::qthColor() const {
  return value("colors/qth", QColor(Qt::red)).value<QColor>();
}
void
Settings::setQTHColor(const QColor &color) {
  setValue("colors/qth", color);
}

QColor
Settings::spot2200mColor() const {
  return value("colors/spot2200m", QColor(Qt::green)).value<QColor>();
}
void
Settings::setSpot2200mColor(const QColor &color) {
  setValue("colors/spot2200m", color);
}

QColor
Settings::spot630mColor() const {
  return value("colors/spot630m", QColor(Qt::green)).value<QColor>();
}
void
Settings::setSpot630mColor(const QColor &color) {
  setValue("colors/spot630m", color);
}

QColor
Settings::spot160mColor() const {
  return value("colors/spot160m", QColor(Qt::green)).value<QColor>();
}
void
Settings::setSpot160mColor(const QColor &color) {
  setValue("colors/spot160m", color);
}

QColor
Settings::spot80mColor() const {
  return value("colors/spot80m", QColor(Qt::green)).value<QColor>();
}
void
Settings::setSpot80mColor(const QColor &color) {
  setValue("colors/spot80m", color);
}

QColor
Settings::spot60mColor() const {
  return value("colors/spot60m", QColor(Qt::green)).value<QColor>();
}
void
Settings::setSpot60mColor(const QColor &color) {
  setValue("colors/spot60m", color);
}

QColor
Settings::spot40mColor() const {
  return value("colors/spot40m", QColor(Qt::green)).value<QColor>();
}
void
Settings::setSpot40mColor(const QColor &color) {
  setValue("colors/spot40m", color);
}

QColor
Settings::spot30mColor() const {
  return value("colors/spot30m", QColor(Qt::green)).value<QColor>();
}
void
Settings::setSpot30mColor(const QColor &color) {
  setValue("colors/spot30m", color);
}

QColor
Settings::spot20mColor() const {
  return value("colors/spot20m", QColor(Qt::green)).value<QColor>();
}
void
Settings::setSpot20mColor(const QColor &color) {
  setValue("colors/spot20m", color);
}

QColor
Settings::spot17mColor() const {
  return value("colors/spot17m", QColor(Qt::green)).value<QColor>();
}
void
Settings::setSpot17mColor(const QColor &color) {
  setValue("colors/spot17m", color);
}

QColor
Settings::spot15mColor() const {
  return value("colors/spot15m", QColor(Qt::green)).value<QColor>();
}
void
Settings::setSpot15mColor(const QColor &color) {
  setValue("colors/spot15m", color);
}

QColor
Settings::spot12mColor() const {
  return value("colors/spot12m", QColor(Qt::green)).value<QColor>();
}
void
Settings::setSpot12mColor(const QColor &color) {
  setValue("colors/spot12m", color);
}

QColor
Settings::spot10mColor() const {
  return value("colors/spot10m", QColor(Qt::green)).value<QColor>();
}
void
Settings::setSpot10mColor(const QColor &color) {
  setValue("colors/spot10m", color);
}

QColor
Settings::spot6mColor() const {
  return value("colors/spot6m", QColor(Qt::green)).value<QColor>();
}
void
Settings::setSpot6mColor(const QColor &color) {
  setValue("colors/spot6m", color);
}

int
Settings::sectionSize(int idx) {
  beginReadArray("SpotTableColumn");
  setArrayIndex(idx);
  int width = value("width", 100).toInt();
  endArray();
  return width;
}
void
Settings::setSectionSize(int idx, int width) {
  beginWriteArray("SpotTableColumn");
  setArrayIndex(idx);
  setValue("width", width);
  endArray();
}

IconProvider::Theme
Settings::iconTheme() const {
  IconProvider::Theme theme = IconProvider::LIGHT_THEME;
#if defined(Q_OS_MACOSX) || defined(Q_OS_WIN32)
  theme = IconProvider::DARK_THEME;
#endif
  return IconProvider::Theme(value("iconTheme", uint(theme)).toUInt());
}
void
Settings::setIconTheme(IconProvider::Theme theme) {
  setValue("iconTheme", uint(theme));
}


Friend::Friend()
  : _call(), _name(), _comment()
{
  // pass...
}

Friend::Friend(const QString &call, const QString &name, const QString &comment)
  : _call(call), _name(name), _comment(comment)
{
  // pass...
}

Friend::Friend(const Friend &other)
  : _call(other._call), _name(other._name), _comment(other._comment)
{
  // pass...
}

const Friend &
Friend::operator= (const Friend &other) {
  _call = other._call;
  _name = other._name;
  _comment = other._comment;
  return *this;
}

Friends::Friends()
  : _friends()
{
  // pass...
}

Friends::Friends(const Friends &other)
  : _friends(other._friends)
{
  // pass...
}

const Friends &
Friends::operator= (const Friends &other) {
  _friends = other._friends;
  return *this;
}

void
Friends::addFriend(const QString &call, const QString &name, const QString &comment)
{
  if (_friends.contains(call))
    return;
  _friends[call] = Friend(call, name, comment);
}

bool
Friends::contains(const QString &call) const {
  return _friends.contains(call);
}

Friend
Friends::operator[](const QString &call) const {
  return _friends[call];
}

