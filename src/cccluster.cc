#include "cccluster.hh"
#include <QRegExp>
#include <QDateTime>

#if defined(Q_OS_LINUX)
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#endif

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
  : QTcpSocket(parent), _state(CONNECTING), _host(host), _port(port), _call(call),
    _rbnpattern("^DX de ([A-Za-Z0-9]+).*: ([0-9\\.]+) ([A-Z0-9/]+) CW "
                 "([0-9]+) dB ([0-9]+) WPM (CQ|DX|BEACON|NCDXF B) ([0-9][0-9])([0-9][0-9])Z$"),
    _callsignPattern("([A-Za-z0-9]+/)?([A-Z0-9]+[0-9][A-Z]+)(/(p|m|mm|am|qrp|qrpp|[0-9]))?$", Qt::CaseInsensitive)
{
  _reconnect.setInterval(10000);
  _reconnect.setSingleShot(true);

  connect(this, SIGNAL(connected()), this, SLOT(onConnected()));
  connect(this, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
  connect(this, SIGNAL(error(QAbstractSocket::SocketError)),
          this, SLOT(onError(QAbstractSocket::SocketError)));
  connect(this, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
  connect(&_reconnect, SIGNAL(timeout()), this, SLOT(reconnect()));

  setSocketOption(QAbstractSocket::KeepAliveOption, 1);

  reconnect();
}


CCCluster::~CCCluster() {
  // pass...
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

#if defined(Q_OS_LINUX)
  int enableKeepAlive = 1;
  int fd = socketDescriptor();
  setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &enableKeepAlive, sizeof(enableKeepAlive));
  int maxIdle = 10; /* seconds */
  setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, &maxIdle, sizeof(maxIdle));
  int count = 3;  // send up to 3 keepalive packets out, then disconnect if no response
  setsockopt(fd, SOL_TCP, TCP_KEEPCNT, &count, sizeof(count));
  int interval = 2;   // send a keepalive packet out every 2 seconds (after the 5 second idle period)
  setsockopt(fd, SOL_TCP, TCP_KEEPINTVL, &interval, sizeof(interval));
#endif
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
        QString fullcall = _rbnpattern.cap(3), call = fullcall;
        if (_callsignPattern.exactMatch(fullcall))
          call = _callsignPattern.cap(2);
        Spot spot = { fullcall, call, _rbnpattern.cap(1), _rbnpattern.cap(2).toDouble(),
                      "CW", readSpotType(_rbnpattern.cap(6)), _rbnpattern.cap(4).toInt(),
                      _rbnpattern.cap(5).toInt(), QTime(_rbnpattern.cap(7).toInt(),
                      _rbnpattern.cap(8).toInt()), QDateTime::currentDateTimeUtc() };
        qDebug() << "Got " << spot.call << "as" << spot.full_call
                 << "from" << spot.spotter << "on" << spot.freq
                 << "with" << spot.db << "dB and" << spot.wpm << "wpm at" << spot.time;
        emit newSpot(spot);
      }
    }

    line = QString::fromLatin1(readLine()).simplified();
  }
}

void
CCCluster::onDisconnected() {
  qDebug() << "CCCluster: TCP socked disconnected:" << errorString();
  _state = CCCluster::ERROR;
  _reconnect.start();
}

void
CCCluster::onError(QAbstractSocket::SocketError err) {
  qDebug() << "CCCluster: TCP Error: " << err << errorString();
  _state = CCCluster::ERROR;
  _reconnect.start();
}
