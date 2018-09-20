#include "adi.hh"
#include <QDebug>
#include "dxcc.hh"


bool LogFile::Record::isValid() const {
  return !( call.isEmpty() || (! time.isValid()) || band.isEmpty() || mode.isEmpty() );
}

LogFile::LogFile(const QString &filename)
  : _filename(filename)
{
  update();
}

void
LogFile::update() {
  _log.clear();

  QFile file(_filename);
  if (! file.open(QIODevice::ReadOnly)) {
    qDebug() << "Cannot open" << _filename;
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
  _log.append(qso);
}

int LogFile::readRecord(const QString data, int start) {
  QRegExp tag("<([a-zA-Z_]+)(?::([0-9]+)(?::[0-9]+)?)?>");
  Record qso;

  while (0 <= (start = tag.indexIn(data, start))) {
    QString name = tag.cap(1).toLower();
    if ("eor" == name.toLower()) {
      if (qso.isValid()) {
        start += tag.matchedLength();
        insert(qso);
      } else {
        start = -1;
      }
      return start;
    }

    int len = tag.cap(2).toInt();
    QString value = data.mid(start+tag.matchedLength(), len);
    if ("call" == name)
      qso.call = value.toUpper();
    if ("qso_date" == name)
      qso.time.setDate(QDate::fromString(value, "yyyyMMdd"));
    if ("time_on" == name)
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
  int dxcc = dxcc_from_call(call);

  Match match = NEW_DXCC;
  foreach (Record qso, _log) {
    if ((NEW_DXCC == match) && (dxcc == qso.dxcc))
      match = NEW_BAND;
    if ((NEW_BAND == match) && (dxcc == qso.dxcc) && (band == qso.band))
      match = NEW_SLOT;
    if ((NEW_SLOT == match) && (dxcc == qso.dxcc) && (band == qso.band) && (mode == qso.mode))
      match = NEW_QSO;
    if (call == qso.call)
      return WORKED;
  }
  return match;
}
