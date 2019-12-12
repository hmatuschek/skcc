#ifndef __CCCLUSTER_HH__
#define __CCCLUSTER_HH__

#include <QTcpSocket>
#include <QRegExp>
#include <QTime>
#include <QTimer>
#include <QNetworkConfigurationManager>


typedef enum {
  CQ_SPOT, DX_SPOT, BEACON_SPOT
} SpotType;

SpotType readSpotType(const QString &type);

/** Defines a spot. */
typedef struct
{
  QString   spot;    ///< Spot call sign.
  QString   spotter; ///< Spotter call sign.
  double    freq;    ///< Spot frequency.
  QString   mode;    ///< Spot mode (CW, etc).
  SpotType  type;    ///< Spot type (CQ, BEACON, etc).
  int       db;      ///< Spot signal strength.
  int       wpm;     ///< Spot speed.
  QTime     time;    ///< Spot time.
  QDateTime rxtime;  ///< Time of announcement on cluster.
} Spot;


/** Implements a simple interface to a CCCluster server.
 * This class connects to the specified host and emits a @c newSpot signal for every spot
 * announced by the cluster. */
class CCCluster: public QTcpSocket
{
  Q_OBJECT

protected:
  /** Possible states for the cluster connection. */
  typedef enum {
    CONNECTING,  ///< Connecting to the cluster.
    CONNECTED,   ///< Connected to the cluster.
    WAIT,        ///< Logged in and waiting for spots.
    ERROR        ///< An error state.
  } State;

public:
  /** Constructs a connection to the specified cluster with the given call.
   * @param call Specifies the call sign used to log to the cluster.
   * @param host Specifies the host name of the cluster.
   * @param port Specifies the port of the cluster.
   * @param parent Specifies the @c QObject parent. */
  CCCluster(const QString &call, const QString &host, quint16 port, QObject *parent=0);
  /** Destructor. */
  virtual ~CCCluster();

public slots:
  /** Reconnects to the cluster. */
  void reconnect();

signals:
  /** Gets emitted once a new spot is announced by the cluster. */
  void newSpot(const Spot &spot);

private slots:
  void onConnected();
  void onReadyRead();
  void onDisconnected();
  void onError(QAbstractSocket::SocketError err);
  void onOnlineStateChanged(bool online);

protected:
  QNetworkConfigurationManager _network;
  State _state;
  QString _host;
  quint16 _port;
  QString _call;
  QRegExp _rbnpattern;
  QByteArray _buffer;
  QTimer _reconnect;
};


#endif // __CCCLUSTER_HH__
