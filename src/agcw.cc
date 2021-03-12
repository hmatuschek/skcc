#include "agcw.hh"

AGCWMembers::AGCWMembers(QObject *parent)
  : WebFile("agcwlist.txt", QUrl("https://www.agcw.de/wp-content/persist/Mitgliederliste.csv"), 7, parent),
    _members()
{
  connect(this, SIGNAL(updated()), this, SLOT(listUpdated()));

  update();
}

void
AGCWMembers::listUpdated()
{
  QFile file(filename());
  if (! file.open(QIODevice::ReadOnly)) {
    qDebug() << "AGCW: Cannot open member list at" << filename() << file.errorString();
    return;
  }

  // skip first line
  QString line = QString::fromUtf8(file.readLine()).simplified();
  do {
    line = QString::fromUtf8(file.readLine()).simplified();
    QStringList row = line.split(";");
    if (3 <= row.size()) {
      _members.insert(row.at(1),row.at(0).toInt());
    }
  } while (! line.isEmpty());

  qDebug() << "AGCW: Loaded" << _members.size() << "member entries.";

  file.close();
}

bool
AGCWMembers::isMember(const QString &call) const {
  return _members.contains(call);
}

int
AGCWMembers::memberID(const QString &call) const {
  return _members[call];
}

Membership
AGCWMembers::membership(const QString &call) const {
  Membership memb;
  if (! _members.contains(call))
    return memb;
  memb.agcw = 1;
  return memb;
}

