#include "rbn.hh"
#include "locator.hh"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

RBNSpotterList::RBNSpotterList(const QString &myloc, int selfupdate, QObject *parent)
    : QObject(parent), _my_cla(0), _my_lon(0), _timer()
{
  setLocator(myloc);

	connect(&_WebCtrl, SIGNAL (finished(QNetworkReply*)), this, SLOT (listDownloaded(QNetworkReply*)));
  connect(&_timer, SIGNAL(timeout()), this, SLOT(update()));

  // get spotter list now
	update();

  // if a self-update period is given in minutes
  if (0 < selfupdate) {
    // -> enable timer
    _timer.setInterval(selfupdate*60*1000);
    _timer.setSingleShot(false);
    _timer.start();
  }
}

void
RBNSpotterList::setLocator(const QString &loc) {
  loc2deg(loc, _my_lon, _my_cla);
  _spotter_dist.clear();
}

void
RBNSpotterList::update() {
  qDebug() << "Update spotter list.";
  _spotter.clear();
  QNetworkRequest request(QUrl("http://reversebeacon.net/nodes/detail_json.php"));
	_WebCtrl.get(request);
}

void
RBNSpotterList::listDownloaded(QNetworkReply *reply) {
  // parse JSON document
  QJsonParseError error;
  QJsonDocument document = QJsonDocument::fromJson(reply->readAll(), &error);
  if (QJsonParseError::NoError != error.error) {
    qWarning() << "JSON parse error" << error.errorString();
    return;
  }
  // Get spotter list
  QJsonArray list = document.array();
  // add spotter
  for (QJsonArray::iterator item=list.begin(); item!=list.end(); item++) {
    if (! item->isObject())
      continue;
    QJsonObject obj = item->toObject();
    if ((! obj.contains("call")) || (! obj.contains("grid")))
      continue;
    _spotter[obj.value("call").toString()] = obj.value("grid").toString();
    qDebug() << "Add skimmer" << obj.value("call") << "@" << obj.value("grid");
  }
  // delete request reply later on
  reply->deleteLater();
  qDebug() << "Got" << _spotter.size() << "skimmer.";
  emit listUpdated();
}

bool
RBNSpotterList::hasSpotter(const QString &call) const {
  return _spotter.contains(call);
}

QString
RBNSpotterList::spotterGrid(const QString &call) const {
  return _spotter[call];
}

double
RBNSpotterList::spotterDist(const QString &call) {
  if (! _spotter.contains(call))
    return -1;
  if (_spotter_dist.contains(call))
    return _spotter_dist[call];
  double slon,scla;
  loc2deg(_spotter[call], slon, scla);
  double dist = great_circle_distance(slon, scla, _my_lon, _my_cla);
  _spotter_dist.insert(call, dist);
  return dist;
}

