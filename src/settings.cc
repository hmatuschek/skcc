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
  return value("clusterHost", "rbn.dh8wr.com").toString();
}

void
Settings::setClusterHost(const QString &addr) {
  setValue("clusterHost", addr);
}

quint16
Settings::clusterPort() const {
  return value("clusterPort", 7373).toUInt();
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
