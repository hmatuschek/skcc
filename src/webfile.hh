#ifndef WEBFILE_HH
#define WEBFILE_HH

#include <QObject>
#include <QFileInfo>
#include <QNetworkAccessManager>


class WebFile : public QObject
{
  Q_OBJECT

public:
	explicit WebFile(const QString &fileName, const QUrl &url, int maxAge, QObject *parent = nullptr);

	QString filename() const;

public slots:
	void update();

signals:
	void updated();

protected slots:
	void downloaded(QNetworkReply *reply);

protected:
	QFileInfo _fileInfo;
	QUrl _url;
	int _maxAge;
	QNetworkAccessManager _WebCtrl;
};

#endif // WEBFILE_HH
