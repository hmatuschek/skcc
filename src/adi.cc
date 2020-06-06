#include "adi.hh"
#include <QDebug>
#include "dxcc.hh"


bool LogFile::Record::isValid() const {
  return !( call.isEmpty() || band.isEmpty() || mode.isEmpty() );
}



LogFile::DXCCRecord::DXCCRecord()
  : _band_mode()
{
  // pass..
}

LogFile::DXCCRecord::DXCCRecord(const QString &band, const QString &mode)
  : _band_mode()
{
  insert(band, mode);
}

void
LogFile::DXCCRecord::insert(const QString &band, const QString &mode) {
  if (! _band_mode.contains(band)) {
    _band_mode.insert(band, QSet<QString>());
  }
  _band_mode[band].insert(mode);
}

LogFile::Match
LogFile::DXCCRecord::isNew(const QString &band, const QString &mode) const {
  if (! _band_mode.contains(band))
    return NEW_BAND;
  if (! _band_mode[band].contains(mode))
    return NEW_SLOT;
  return NEW_QSO;
}



LogFile::LogFile(const QString &filename)
  : _filename(filename), _watcher()
{
  _watcher.addPath(_filename);
  update();

  connect(&_watcher, SIGNAL(fileChanged(const QString &)), this, SLOT(update()));
}

void
LogFile::update() {
  _QSOs.clear();
  _DXCCs.clear();

  QFile file(_filename);
  if (! file.open(QIODevice::ReadOnly)) {
    qDebug() << "LOG: Cannot open" << _filename;
    return;
  }

  QString data = QString::fromUtf8(file.readAll());
  int start = 0;
  if (! data.startsWith('<')) {
    start = data.indexOf("<eoh>", 0, Qt::CaseInsensitive);
    if (0 < start)
      start += 5;
  }

  while (0 <= (start = readRecord(data, start)) ) {
    // pass...
  }
}


void
LogFile::insert(const Record &qso) {
  _QSOs.insert(qso.call);
  int dxcc = dxcc_from_call(qso.call);
  if (! _DXCCs.contains(dxcc)) {
    _DXCCs.insert(dxcc, DXCCRecord(qso.band, qso.mode));
  }
}

int LogFile::readRecord(const QString data, int start) {
  QRegExp tag("<([a-zA-Z_]+)(?::([0-9]+)(?::[0-9]+)?)?>");
  Record qso;

  while (0 <= (start = tag.indexIn(data, start))) {
    QString name = tag.cap(1).toLower();
    if ("eor" == name.toLower()) {
      if (qso.isValid()) {
        //qDebug() << "Found QSO" << qso.call << "in log.";
        start += tag.matchedLength();
        insert(qso);
      } else {
        qDebug() << "Skip invalid QSO" << qso.call << qso.band << qso.dxcc
                 << qso.mode << qso.time << "in log.";
        start = -1;
      }
      return start;
    }

    int len = tag.cap(2).toInt();
    QString value = data.mid(start+tag.matchedLength(), len);
    if ("call" == name)
      qso.call = value.toUpper();
    if ("qso_date_on" == name)
      qso.time.setDate(QDate::fromString(value, "yyyyMMdd"));
    if ((! qso.time.date().isValid()) && ("qso_date_off" == name))
      qso.time.setDate(QDate::fromString(value, "yyyyMMdd"));
    if ("time_on" == name)
      qso.time.setTime(QTime::fromString(value, "hhmmss"));
    if ((!qso.time.time().isValid()) && ("time_on" == name))
      qso.time.setTime(QTime::fromString(value, "hhmmss"));
    if ("band" == name)
      qso.band = value.toUpper();
    if ("mode" == name)
      qso.mode = value.toUpper();
    if ("dxcc" == name)
      qso.dxcc = value.toInt();
    start += tag.matchedLength() + len;
  }

  return start;
}

LogFile::Match
LogFile::isNew(const QString &call, const QString &band, const QString &mode) const
{
  if (_QSOs.contains(call))
    return WORKED;
  int dxcc = dxcc_from_call(call);
  if (! _DXCCs.contains(dxcc))
    return NEW_DXCC;
  return _DXCCs[dxcc].isNew(band, mode);
}
