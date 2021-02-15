#include "rbn.hh"
#include "locator.hh"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

RBNSpotterList::RBNSpotterList(int selfupdate, QObject *parent)
    : QObject(parent), _timer()
{
	connect(&_WebCtrl, SIGNAL (finished(QNetworkReply*)), this, SLOT (listDownloaded(QNetworkReply*)));
  connect(&_timer, SIGNAL(timeout()), this, SLOT(update()));

	update();

  if (0<selfupdate) {
    _timer.setInterval(selfupdate*60*1000);
    _timer.setSingleShot(false);
    _timer.start();
  }
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
  QJsonParseError error;
  QJsonDocument document = QJsonDocument::fromJson(reply->readAll(), &error);
  QJsonArray list = document.array();

  for (QJsonArray::iterator item=list.begin(); item!=list.end(); item++) {
    if (! item->isObject())
      continue;
    QJsonObject obj = item->toObject();
    if ((! obj.contains("call")) || (! obj.contains("grid")))
      continue;
    _spotter[obj.value("call").toString()] = obj.value("grid").toString();
    qDebug() << "Add skimmer" << obj.value("call") << "@" << obj.value("grid");
  }

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
RBNSpotterList::spotterDist(const QString &call, const QString grid) const {
  if (! _spotter.contains(call))
    return -1;
  double slon,scla,mlon,mcla;
  loc2deg(_spotter[call], slon, scla);
  loc2deg(grid, mlon, mcla);
  return great_circle_distance(slon, scla, mlon, mcla);
}

