#ifndef SETTINGS_HH
#define SETTINGS_HH

#include <QSettings>
#include <QColor>
#include "adi.hh"

class Friend
{
public:
  Friend();
  Friend(const QString &call, const QString &name, const QString &comment);
  Friend(const Friend &other);

  const Friend &operator= (const Friend &other);

  inline const QString &call() const { return _call; }
  inline const QString &name() const { return _name; }
  inline const QString &comment() const { return _comment; }

protected:
  QString _call;
  QString _name;
  QString _comment;
};


class Friends
{
public:
  typedef QHash<QString, Friend>::const_iterator iterator;

public:
  Friends();
  Friends(const Friends &other);

  const Friends &operator= (const Friends &other);

  void addFriend(const QString &call, const QString &name, const QString &comment);
  bool contains(const QString &call) const;
  Friend operator[] (const QString &call) const;

  inline size_t size() const { return _friends.size(); }
  inline iterator begin() const { return _friends.begin(); }
  inline iterator end() const { return _friends.end(); }

protected:
  QHash<QString, Friend> _friends;
};



class Settings: public QSettings
{
	Q_OBJECT

public:
  Settings();

  bool isComplete() const;

  bool band2200m() const;
  void setBand2200m(bool enable);
  bool band630m() const;
  void setBand630m(bool enable);
  bool band160m() const;
  void setBand160m(bool enable);
  bool band80m() const;
  void setBand80m(bool enable);
  bool band60m() const;
  void setBand60m(bool enable);
  bool band40m() const;
  void setBand40m(bool enable);
  bool band30m() const;
  void setBand30m(bool enable);
  bool band20m() const;
  void setBand20m(bool enable);
  bool band17m() const;
  void setBand17m(bool enable);
  bool band15m() const;
  void setBand15m(bool enable);
  bool band12m() const;
  void setBand12m(bool enable);
  bool band10m() const;
  void setBand10m(bool enable);
  bool band6m() const;
  void setBand6m(bool enable);

  QString call() const;
  void setCall(const QString &call);

  QString locator() const;
  void setLocator(const QString &locator);

  QString clusterHost() const;
  void setClusterHost(const QString &addr);
  quint16 clusterPort() const;
  void setClusterPort(quint16 port);

  bool showSelfSpots() const;
  void setShowSelfSpots(bool show);

  bool showBeaconSpots() const;
  void setShowBeaconSpots(bool show);

  int maxSpotterDist() const;
  void setMaxSpotterDist(int dist);

  int maxSpotAge() const;
  void setMaxSpotAge(int age);

  QString logFile() const;
  void setLogFile(const QString filename);
  LogFile::Match minLogMatch() const;
  void setMinLogMatch(LogFile::Match match);

  int maxSpeed() const;
  void setMaxSpeed(int speed);

  int minSNR() const;
  void setMinSNR(int snr);

  bool notificationSoundEnabled() const;
  void setNotificationSoundEnabled(bool enable);
  bool notifyOnNewDXCC() const;
  void setNotifyOnNewDXCC(bool enable);
  bool notifyOnNewBand() const;
  void setNotifyOnNewBand(bool enable);
  bool notifyOnNewSKCC() const;
  void setNotifyOnNewSKCC(bool enable);
  bool notifyOnNewFriend() const;
  void setNotifyOnNewFriend(bool enable);

  Friends friends();
  void setFriends(const Friends &friends);

  QColor selfSpotColor() const;
  void setSelfSpotColor(const QColor &color);
  QColor friendSpotColor() const;
  void setFriendSpotColor(const QColor &color);
  QColor beaconSpotColor() const;
  void setBeaconSpotColor(const QColor &color);
  QColor newDXCCColor() const;
  void setNewDXCCColor(const QColor &color);
  QColor newBandColor() const;
  void setNewBandColor(const QColor &color);
  QColor newSlotColor() const;
  void setNewSlotColor(const QColor &color);
  QColor newQSOColor() const;
  void setNewQSOColor(const QColor &color);
  QColor workedColor() const;
  void setWorkedColor(const QColor &color);

  QColor qthColor() const;
  void setQTHColor(const QColor &color);
  QColor spot2200mColor() const;
  void setSpot2200mColor(const QColor &color);
  QColor spot630mColor() const;
  void setSpot630mColor(const QColor &color);
  QColor spot160mColor() const;
  void setSpot160mColor(const QColor &color);
  QColor spot80mColor() const;
  void setSpot80mColor(const QColor &color);
  QColor spot60mColor() const;
  void setSpot60mColor(const QColor &color);
  QColor spot40mColor() const;
  void setSpot40mColor(const QColor &color);
  QColor spot30mColor() const;
  void setSpot30mColor(const QColor &color);
  QColor spot20mColor() const;
  void setSpot20mColor(const QColor &color);
  QColor spot17mColor() const;
  void setSpot17mColor(const QColor &color);
  QColor spot15mColor() const;
  void setSpot15mColor(const QColor &color);
  QColor spot12mColor() const;
  void setSpot12mColor(const QColor &color);
  QColor spot10mColor() const;
  void setSpot10mColor(const QColor &color);
  QColor spot6mColor() const;
  void setSpot6mColor(const QColor &color);

  int sectionSize(int idx);
  void setSectionSize(int idx, int width);
};

#endif // SETTINGS_HH

