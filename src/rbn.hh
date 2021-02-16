#ifndef RBN_HH
#define RBN_HH

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QHash>
#include <QTimer>


/** Downloads the current list of Reverse Beacon Network (RBN) skimmers.
 * This class also can update the list regularily. */
class RBNSpotterList: public QObject
{
	Q_OBJECT

public:
  /** Constructor. */
	explicit RBNSpotterList(int selfupdate=30, QObject *parent=0);

  /** Returns if the given call is in the list of skimmers. */
	bool hasSpotter(const QString &call) const;
  /** Returns the grid locator of the given skimmer. */
	QString spotterGrid(const QString &call) const;
  /** Returns the distance to the given skimmer from the specified grid locator. */
	double spotterDist(const QString &call, const QString grid) const;

public slots:
  /** Updates the list of skimmers. */
	void update();

signals:
  /** Gets emitted once the list has been updated. */
	void listUpdated();

private slots:
  /** Internal call-back to handle downloaded spotter list. */
	void listDownloaded(QNetworkReply* reply);

private:
  /** Network access. */
  QNetworkAccessManager _WebCtrl;
  /** Maps spotter call to locator. */
  QHash<QString, QString> _spotter;
  /** Timer to update spotter list regularily (if enabled). */
  QTimer _timer;
};


#endif // RBN_HH
