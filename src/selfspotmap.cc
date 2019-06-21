#include "selfspotmap.hh"
#include "application.hh"
#include "bands.hh"


/* ******************************************************************************************** *
 * Implementation of SelfSpotMap
 * ******************************************************************************************** */
SelfSpotMapView::SelfSpotMapView(const QString &qthLoc, Application &app, QWidget *parent)
  : MapView(qthLoc, parent), _app(app), _qthLoc(qthLoc), _clearTimer()
{
  Settings settings;
  this->addPoint(_qthLoc, settings.qthColor());

  _clearTimer.setInterval(10000UL);
  _clearTimer.setSingleShot(false);
  _clearTimer.start();

  connect(_app.spots(), SIGNAL(newSelfSpot(const Spot &, const QString &)),
          this, SLOT(onSelfSpot(const Spot &, const QString &)));
  connect(&_clearTimer, SIGNAL(timeout()), this, SLOT(onClearSpots()));
}

bool
SelfSpotMapView::hasSpotter(const QString &call) const {
  Spot spot;
  foreach (spot, _spots) {
    if (call == spot.spotter)
      return true;
  }
  return false;
}

void
SelfSpotMapView::onSelfSpot(const Spot &spot, const QString &spotterLoc) {
  Spot ospot;
  foreach (ospot, _spots) {
    if (spot.spotter == ospot.spotter) {
      ospot.rxtime = spot.rxtime;
      return;
    }
  }

  QColor color;
  Settings settings;
  QString band = freq2band(spot.freq);
  if (BAND_2200M == band)
    color = settings.spot2200mColor();
  else if (BAND_630M == band)
    color = settings.spot630mColor();
  else if (BAND_160M == band)
    color = settings.spot160mColor();
  else if (BAND_80M == band)
    color = settings.spot80mColor();
  else if (BAND_60M == band)
    color = settings.spot60mColor();
  else if (BAND_40M == band)
    color = settings.spot40mColor();
  else if (BAND_30M == band)
    color = settings.spot30mColor();
  else if (BAND_20M == band)
    color = settings.spot20mColor();
  else if (BAND_17M == band)
    color = settings.spot17mColor();
  else if (BAND_15M == band)
    color = settings.spot15mColor();
  else if (BAND_12M == band)
    color = settings.spot12mColor();
  else if (BAND_10M == band)
    color = settings.spot10mColor();
  else if (BAND_6M == band)
    color = settings.spot6mColor();
  else
    color = Qt::gray;
  color.setAlpha(50+(205*std::min(20, std::max(0, spot.db)))/20);
  addArc(_qthLoc, spotterLoc, color);
  _spots.push_back(spot);
  update();
}

void
SelfSpotMapView::onClearSpots() {
  bool needsUpdate = false;
  QDateTime now = QDateTime::currentDateTimeUtc();
  QList<Spot>::iterator si = _spots.begin();
  QList< QPair<QColor, QVector<QPoint>> >::iterator ai = _arcs.begin();
  for (; si!=_spots.end();) {
    if (si->rxtime.secsTo(now) > Settings().maxSpotAge()) {
      si = _spots.erase(si);
      ai = _arcs.erase(ai);
      needsUpdate = true;
    } else {
      si++; ai++;
    }
  }
  if (needsUpdate)
    update();
}
