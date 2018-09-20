#include "rbn.hh"
#include "locator.hh"


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
	QNetworkRequest request(QUrl("http://reversebeacon.net/cont_includes/status.php?t=skt"));
	_WebCtrl.get(request);
}

void
RBNSpotterList::listDownloaded(QNetworkReply *reply) {
	QString data = QString::fromUtf8(reply->readAll()).simplified();
  data.remove('\r'); data.remove('\n');

	QRegExp re("<td[^>]*><a href=\"/dxsd1.php\\?f=[^>]*>\\s*([A-Z0-9/]*)\\s*</a>[^<]*</td>\\s*<td[^>]*>\\s*([0-9m,]*)</a></td>\\s*<td[^>]*>([0-9A-Za-z]*)</td>", Qt::CaseInsensitive);
	int start = 0;
	while (0 < (start = data.indexOf("online24h online7d total\">", start))) {
		start += 27;
		int end = data.indexOf("</tr>", start);
		if (0 > end) continue;
		int found = re.indexIn(data, start);
		if ((0>found) || (found>end))
			continue;
		QString spotter = re.cap(1);
		QString grid = re.cap(3);
    _spotter[spotter] = grid;
	}

  reply->deleteLater();

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

