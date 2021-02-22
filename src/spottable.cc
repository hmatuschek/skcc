#include "spottable.hh"
#include <QDateTime>
#include <QColor>
#include "bands.hh"
#include "dxcc.hh"
#include <QFont>
#include "adi.hh"


SpotTable::SpotTable(const QString &call, const QString &locator, const QString &logfile,
                     const QString &cluster, uint16_t port,
                     bool showSelf, bool showBeacon, int maxdist, int maxage, LogFile::Match minMatch, int maxSpeed,
                     int minSNR, QObject *parent)
    : QAbstractTableModel(parent), _cluster(call, cluster, port),
      _spotterlist(), _logfile(logfile), _skcc(), _agcw(), _spots(), _call(call), _locator(locator),
      _showSelf(showSelf), _showBeaconSpots(showBeacon), _maxAge(maxage), _maxDist(maxdist),
      _maxSpeed(maxSpeed), _minSNR(minSNR),
      _minMatch(minMatch)
{
  _bands << BAND_2200M << BAND_630M << BAND_160M << BAND_80M << BAND_60M << BAND_40M << BAND_30M
         << BAND_20M << BAND_17M << BAND_15M << BAND_12M << BAND_10M << BAND_6M;

	connect(&_cluster, SIGNAL(newSpot(Spot)), this, SLOT(onNewSpot(Spot)));
  connect(&_cluster, SIGNAL(connected()), this, SIGNAL(connected()));
  connect(&_cluster, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
}

int
SpotTable::rowCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
	return _spots.size();
}

int
SpotTable::columnCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
	return 7;
}

QVariant
SpotTable::data(const QModelIndex &index, int role) const {
	if (index.row() >= _spots.size())
    return QVariant();
  if (_spots.at(index.row()).isEmpty())
    return QVariant();

  QList<Spot> spots = _spots.at(index.row());
  QString call = spots.first().full_call;
  bool skcc = _skcc.isMember(call);
  bool agcw = _agcw.isMember(call);
  bool beacon = ( BEACON_SPOT == spots.first().type );
  int  sc = spots.size();
  int  db = spots.first().db;
  QString spotter = spots.first().spotter;
  double  dist = _spotterlist.spotterDist(spotter, _locator);
  for (QList<Spot>::iterator it = spots.begin(); it != spots.end(); it++) {
    double d2 = _spotterlist.spotterDist(it->spotter, _locator);
    if (d2<dist) {
      db = it->db;
      spotter = it->spotter;
    }
  }

  if (Qt::DisplayRole == role) {
    QStringList prefixlst;
    if (skcc) prefixlst.append("skcc");
    if (agcw) prefixlst.append("agcw");
    if (beacon) prefixlst.append("b");
    QString prefix = "";
    if (prefixlst.size())
      prefix = QString("[%1] ").arg(prefixlst.join(", "));
    switch (index.column()) {
      case 0: return prefix + call;
      case 1: return _spots.at(index.row()).last().freq;
      case 2: return db;
      case 3: return _spots.at(index.row()).first().wpm;
      case 4: return dxcc_name_from_call(call);
      case 5: return spotter + ((1<sc) ? QString(" + %1").arg(sc-1) : QString());
      case 6: return _spots.at(index.row()).last().time.toString();
    }
  } else if (Qt::EditRole == role) {
    switch (index.column()) {
      case 0: return call;
      case 1: return _spots.at(index.row()).first().freq;
      case 2: return db;
      case 3: return _spots.at(index.row()).first().wpm;
      case 4: return dxcc_name_from_call(call);
      case 5: return spotter;
      case 6: return _spots.at(index.row()).last().rxtime;
    }
  } else if (Qt::BackgroundRole == role) {
    Settings settings;
    if (_spots.at(index.row()).first().full_call == _call)
      return settings.selfSpotColor();
    if (_friends.contains(_spots.at(index.row()).first().full_call.toUpper()))
      return settings.friendSpotColor();
    if (BEACON_SPOT == _spots.at(index.row()).first().type)
      return settings.beaconSpotColor();
    if (0 > dxcc_from_call(_spots.at(index.row()).first().full_call))
      return settings.newQSOColor();
    LogFile::Match match = _logfile.isNew(
          _spots.at(index.row()).first().full_call, freq2band(_spots.at(index.row()).first().freq),
          _spots.at(index.row()).first().mode);
    switch (match) {
      case LogFile::NEW_DXCC: return settings.newDXCCColor();
      case LogFile::NEW_BAND: return settings.newBandColor();
      case LogFile::NEW_SLOT: return settings.newSlotColor();
      case LogFile::NEW_QSO: return settings.newQSOColor();
      case LogFile::WORKED: return settings.workedColor();
    }
  } else if (Qt::FontRole == role) {
    return QFont("Helvetica [Cronyx]", 12);
  }

	return QVariant();
}

QVariant
SpotTable::headerData(int section, Qt::Orientation orientation, int role) const {
  if ((Qt::Horizontal != orientation) || (Qt::DisplayRole != role))
    return QVariant();
  switch (section) {
    case 0: return "Spot";
    case 1: return "Freq. [kHz]";
    case 2: return "SNR [dB]";
    case 3: return "Speed [WPM]";
    case 4: return "DXCC";
    case 5: return "Spotter";
    case 6: return "Time";
  }

  return QVariant();
}


const QSet<QString> &
SpotTable::bands() const {
  return _bands;
}

QSet<QString> &
SpotTable::bands() {
  return _bands;
}

const Friends &
SpotTable::friends() const {
  return _friends;
}

Friends &
SpotTable::friends() {
  return _friends;
}

bool
SpotTable::showSelfSpots() const {
  return _showSelf;
}

void
SpotTable::setShowSelfSpots(bool enable) {
  _showSelf = enable;
}

bool
SpotTable::showBeaconSpots() const {
  return _showBeaconSpots;
}

void
SpotTable::setShowBeaconSpots(bool enable) {
  _showBeaconSpots = enable;
}

LogFile::Match
SpotTable::minMatch() const {
  return _minMatch;
}

void
SpotTable::setMinMatch(LogFile::Match match) {
  _minMatch = match;
}

int
SpotTable::maxDist() const {
  return _maxDist;
}

void
SpotTable::setMaxDist(int dist) {
  _maxDist = dist;
}

int
SpotTable::maxAge() const {
  return _maxAge;
}

void
SpotTable::setMaxAge(int age) {
  _maxAge = age;
}

int
SpotTable::maxSpeed() const {
  return _maxSpeed;
}

void
SpotTable::setMaxSpeed(int wpm) {
  _maxSpeed = wpm;
}

int
SpotTable::minSNR() const {
  return _minSNR;
}

void
SpotTable::setMinSNR(int db) {
  _minSNR = db;
}

void
SpotTable::onNewSpot(const Spot &spot)
{
  /* Ok, there will be a lot of spots hitting this function. Remember, all spots all over the world
   * are fed into this function. Consequently, it should be relatively efficient to filter all
   * spots that are irrelevant w.r.t. the filter settings. Do only fancy stuff after the majority of
   * spots are ignored. */

  // First remove all old spots if maxAge is specified
  if (0 < _maxAge) {
    QDateTime now = QDateTime::currentDateTimeUtc();

    int row = 0;
    for (QList<QList<Spot> >::iterator it=_spots.begin(); it!=_spots.end(); it++, row++)
    {
      int spotRemIdx = -1;
      for (QList<Spot>::iterator sit=it->begin(); sit!=it->end(); sit++) {
        if (sit->rxtime.secsTo(now)>_maxAge)
          spotRemIdx++;
        else
          break;
      }
      for (int i=0; i<=spotRemIdx; i++)
        it->removeFirst();

      if (it->size() && (0<=spotRemIdx))
        emit dataChanged(index(row, 0), index(row, 6));
    }

    row = 0;
    for (QList<QList<Spot> >::iterator it=_spots.begin(); it!=_spots.end();) {
      if (it->empty()) {
        beginRemoveRows(QModelIndex(), row, row);
        it = _spots.erase(it);
        endRemoveRows();
      } else {
        it++;
        row++;
      }
    }
  }

  // If spot is a self-spot -> accept anyway
  if (spot.call == _call) {
    if (_spotterlist.hasSpotter(spot.spotter))
      emit newSelfSpot(spot, _spotterlist.spotterGrid(spot.spotter));
    else
      qDebug() << "Uknown skimmer" << spot.spotter;
    goto accept;
  }

  // If spot is a friend-spot -> accept anyway
  if (_friends.contains(spot.call.toUpper())) {
    emit newFriend(spot);
    goto accept;
  }

  // Otherwise filter by settings

  // If spot-band is enabled
  if (! _bands.contains(freq2band(spot.freq)))
    return;

  // Filter by spotter distance (if enabled)
  if (0 < _maxDist) {
    if (! _spotterlist.hasSpotter(spot.spotter))
      return;
    if (_spotterlist.spotterDist(spot.spotter, _locator) > _maxDist)
      return;
  }

  // Handle beacon spots
  if (BEACON_SPOT == spot.type) {
    // accept if beacons are enabled
    if (_showBeaconSpots)
      goto accept;
    return;
  }

  // Filter by speed
  if ((0<_maxSpeed) && (spot.wpm>_maxSpeed))
    return;

  // Filter by SNR
  if ((0<_minSNR) && (spot.db<_minSNR))
    return;

  // Filter by log match (e.g., new call, band or dxcc)
  if (_minMatch > _logfile.isNew(spot.full_call, freq2band(spot.freq), spot.mode))
    return;

  // If the spot is accepted
accept:
  int row=0;
  // Check for duplicate spots and aggregate spots by call
  bool isDup = false, hasSpot = false;
  for (QList<QList<Spot> >::iterator item = _spots.begin(); item != _spots.end(); item++, row++) {
    if (item->first().full_call == spot.full_call) {
      hasSpot = true;
      isDup = (item->last().time == spot.time);
      item->append(spot);
      emit dataChanged(index(row, 0), index(row, 6));
      break;
    }
  }

  if (! hasSpot) {
    // if a new spot, append row
    beginInsertRows(QModelIndex(), _spots.size(), _spots.size());
    QList<Spot> tmp; tmp.append(spot);
    _spots.append(tmp);
    endInsertRows();
  }

  // If a known spot -> do not notify
  if (isDup)
    return;

  LogFile::Match logmatch = _logfile.isNew(spot.full_call, freq2band(spot.freq), spot.mode);
  switch (logmatch) {
    case LogFile::NEW_DXCC:
      qDebug() << "Emit new DXCC...";
      emit newDXCC(spot);
      break;
    case LogFile::NEW_BAND:
      qDebug() << "Emit new band...";
      emit newBand(spot);
      break;
    default:
      break;
  }

  if ((LogFile::WORKED != logmatch) && _skcc.isMember(spot.full_call)) {
    qDebug() << "Emit new SKCC...";
    emit newSKCC(spot);
  }
  if ((LogFile::WORKED != logmatch) && _agcw.isMember(spot.full_call)) {
    qDebug() << "Emit new AGCW...";
    emit newAGCW(spot);
  }
}
