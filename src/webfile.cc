#include "webfile.hh"
#include <QDir>
#include <QStandardPaths>
#include <QNetworkReply>


WebFile::WebFile(const QString &fileName, const QUrl &url, int maxAge, QObject *parent)
  : QObject(parent), _url(url), _maxAge(maxAge)
{
  QDir appdir(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));
  if (! appdir.exists())
    appdir.mkpath(".");
  _fileInfo.setFile(appdir.absolutePath() + "/" + fileName);

  connect(&_WebCtrl, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloaded(QNetworkReply*)));
}


QString
WebFile::filename() const {
  return _fileInfo.absoluteFilePath();
}

void
WebFile::update() {
  if (_fileInfo.exists() && ((_maxAge > 0) && (_fileInfo.lastModified().daysTo(QDateTime::currentDateTime()) < _maxAge)) ) {
    emit updated();
  } else {
    QNetworkRequest request(_url);
    request.setRawHeader("User-Agent","Mozilla /5.0 (linux-gnu)");
    _WebCtrl.get(request);
  }
}


void
WebFile::downloaded(QNetworkReply *reply) {
  if (QNetworkReply::NoError != reply->error()) {
    qDebug() << "WebFile: Cannot load '" << _url.toString() << "':" << reply->errorString();
    return;
  }

  QFile file(_fileInfo.absoluteFilePath());
  if (!file.open(QIODevice::WriteOnly)) {
    qDebug() << "Cannot open file" << _fileInfo.absoluteFilePath();
    return;
  }

  qDebug() << "File downloaded to" << _fileInfo.absoluteFilePath();

  file.write(reply->readAll());
  file.close();

  reply->deleteLater();

  emit updated();
}
