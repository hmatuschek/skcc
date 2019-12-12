#include "cccluster.hh"
#include <QRegExp>
#include <QDateTime>

SpotType readSpotType(const QString &type) {
  if (("BEACON" == type) || ("NCDXF B" == type)) {
    return BEACON_SPOT;
  } else if ("DX" == type) {
    return DX_SPOT;
  }
  return CQ_SPOT;
}

/* ******************************************************************************************** *
 * Implementation of CCCluster
 * ******************************************************************************************** */
CCCluster::CCCluster(const QString &call, const QString &host, quint16 port, QObject *parent)
  : QTcpSocket(parent), _network(), _state(CONNECTING), _host(host), _port(port), _call(call),
    _rbnpattern("^DX de ([A-Za-Z0-9]+).*: ([0-9\\.]+) ([A-Z0-9/]+) CW "
                 "([0-9]+) dB ([0-9]+) WPM (CQ|DX|BEACON|NCDXF B) ([0-9][0-9])([0-9][0-9])Z$")
{
  _reconnect.setInterval(10000);
  _reconnect.setSingleShot(true);

  connect(this, SIGNAL(connected()), this, SLOT(onConnected()));
  connect(this, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
  connect(this, SIGNAL(error(QAbstractSocket::SocketError)),
          this, SLOT(onError(QAbstractSocket::SocketError)));
  connect(this, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
  connect(&_reconnect, SIGNAL(timeout()), this, SLOT(reconnect()));
  connect(&_network, SIGNAL(onlineStateChanged(bool)), this, SLOT(onOnlineStateChanged(bool)));

  if (_network.isOnline())
    reconnect();
}


CCCluster::~CCCluster() {
  // pass...
}

void
CCCluster::onOnlineStateChanged(bool online) {
  if (online) {
    qDebug() << "Got online.";
    _reconnect.start();
  } else {
    qDebug() << "Got offline.";
    this->close();
  }
}
void
CCCluster::reconnect() {
  _state = CONNECTING;
  qDebug() << "CCCluster: Connecting to" << _host << _port << "...";
  connectToHost(_host, _port);
}

void
CCCluster::onConnected() {
  qDebug() << "CCCluster: ... connected.";
  _state = CONNECTED;
}

void
CCCluster::onReadyRead()
{
  // Read a line from stream
  QString line = QString::fromLatin1(readLine()).simplified();

  while (! line.isEmpty()) {
    // Wait for call prompt
    if ((CONNECTED == _state) && (line.endsWith("call:"))) {
      qDebug() << "Logged in.";
      // send call
      write(_call.toLatin1()); write("\r\n");
      _state = WAIT;
    } else if ((WAIT == _state) && (line.startsWith("DX"))) {
      //qDebug() << "Got line" << line;
      if (_rbnpattern.exactMatch(line)) {
        Spot spot = { _rbnpattern.cap(3), _rbnpattern.cap(1), _rbnpattern.cap(2).toDouble(),
                      "CW", readSpotType(_rbnpattern.cap(6)), _rbnpattern.cap(4).toInt(),
                      _rbnpattern.cap(5).toInt(), QTime(_rbnpattern.cap(7).toInt(),
                      _rbnpattern.cap(8).toInt()), QDateTime::currentDateTimeUtc() };
        /*qDebug() << "Got " << spot.spot << "from" << spot.spotter << "on" << spot.freq
                 << "with" << spot.db << "dB and" << spot.wpm << "wpm at" << spot.time;*/
        emit newSpot(spot);
      }
    }

    line = QString::fromLatin1(readLine()).simplified();
  }
}

void
CCCluster::onDisconnected() {
  qDebug() << "CCCluster: TCP socked disconnected:" << errorString();
  _state = ERROR;
  if (_network.isOnline())
    _reconnect.start();
}

void
CCCluster::onError(QAbstractSocket::SocketError err) {
  qDebug() << "CCCluster: TCP Error: " << err << errorString();
  _state = ERROR;
  if (_network.isOnline())
    _reconnect.start();
}
