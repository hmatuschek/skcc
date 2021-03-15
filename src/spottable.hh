#ifndef SPOTTABLE_HH
#define SPOTTABLE_HH

#include <QAbstractTableModel>
#include <QSet>
#include <QTimer>

#include "cccluster.hh"
#include "rbn.hh"
#include "adi.hh"
#include "clubmembership.hh"
#include "settings.hh"

/** Central table holding all current spots matching the chosen conditions. */
class SpotTable : public QAbstractTableModel
{
	Q_OBJECT

public:
  /** Constructs a spot table.
   * @param call Specifies your call.
   * @param loc Specifies your grid locator.
   * @param logfile Specifies your log file to consult.
   * @param cluster Specifies the hostname of RBN cluster.
   * @param port Specifies the cluster port.
   * @param showSelf If @c true the table will show also selfspots irrespective of the chosen
   *        conditions.
   * @param maxdist Specifies the maximum allowed distance for a spotter in km.
   * @param maxage Specifies the maximum age of spots in seconds.
   * @param minMatch Specifies the minimum logfile match for a spot to be added to the table. This
   *        filter allows for showing only new DXCCs, Bands etc.
   * @param maxSpeed Specifies the maximum speed in WPM.
   * @param minSNR Specifies the minimum SNR for spots.
   * @param parent Specifies the @c QObject parent. */
	explicit SpotTable(const QString &call, const QString &loc, const QString &logfile,
                     const QString &cluster, uint16_t port,
	                   bool showSelf=true, bool showBeacon=true, int maxdist=-1, int maxage=-1,
	                   LogFile::Match minMatch=LogFile::WORKED, int maxSpeed=-1, int minSNR=-1,
	                   QObject *parent = nullptr);

	int columnCount(const QModelIndex &parent) const;
	int rowCount(const QModelIndex &parent) const;
	QVariant data(const QModelIndex &index, int role) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const;

	bool showSelfSpots() const;
	void setShowSelfSpots(bool enable);

  bool showBeaconSpots() const;
	void setShowBeaconSpots(bool enable);

  int maxDist() const;
	void setMaxDist(int dist);

	int maxAge() const;
	void setMaxAge(int age);

	LogFile::Match minMatch() const;
	void setMinMatch(LogFile::Match match);

	const QSet<QString> &bands() const;
	QSet<QString> &bands();

  const Friends &friends() const;
	Friends &friends();

	int maxSpeed() const;
	void setMaxSpeed(int wpm);

	int minSNR() const;
	void setMinSNR(int db);

  void setLocator(const QString &loc);
  void setCall(const QString &call);

protected slots:
	void onNewSpot(const Spot &spot);
  void removeOldSpots();

signals:
  void connected();
  void disconnected();
	void newDXCC(const Spot &spot);
	void newBand(const Spot &spot);
  void newMembership(const Spot &spot, const Membership &membership);
  void newFriend(const Spot &spot);
  void newSelfSpot(const Spot &spot, const QString &spotterLoc);

protected:
	CCCluster _cluster;
  RBNSpotterList *_spotterlist;
	LogFile _logfile;
  ClubMembership _memberships;

	QList< QList<Spot> > _spots;

	QString _call;

	bool _showSelf;
  bool _showBeaconSpots;
	int _maxAge;
	int _maxDist;
	int _maxSpeed;
	int _minSNR;
	LogFile::Match _minMatch;
	QSet<QString> _bands;
  Friends _friends;

  QTimer _cleanupTimer;
};

#endif // SPOTTABLE_HH
