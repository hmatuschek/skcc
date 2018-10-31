#ifndef WEBFILE_HH
#define WEBFILE_HH

#include <QObject>
#include <QFileInfo>
#include <QNetworkAccessManager>

/** This class downloades and updates some HTTP resource locally.
 * This class downloads the specified URL and stores it under the given filename. The file will
 * be updated once @c maxAge days has been passed. */
class WebFile : public QObject
{
  Q_OBJECT

public:
	explicit WebFile(const QString &fileName, const QUrl &url, int maxAge, QObject *parent = nullptr);

  /** Returns the full file path. */
	QString filename() const;

public slots:
  /** Downloads and updates the resource if necessary. */
	void update();

signals:
  /** Gets emitted if a sufficiently up-to-date file is present. */
	void updated();

protected slots:
  /** Gets called once the download is complete. */
	void downloaded(QNetworkReply *reply);

protected:
  /** Holds the file info object for the local copy. */
	QFileInfo _fileInfo;
  /** The URL of the resource to download. */
	QUrl _url;
  /** Maximum age in days. */
	int _maxAge;
  /** Download manager. */
	QNetworkAccessManager _WebCtrl;
};

#endif // WEBFILE_HH
