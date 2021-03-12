#include "hsc.hh"

HSCMembers::HSCMembers(QObject *parent)
  : WebFile("hsclist.txt", QUrl("http://hsc.dj1yfk.de/db/hsc_list_by_number.csv"), 7, parent),
    _members()
{
  connect(this, SIGNAL(updated()), this, SLOT(listUpdated()));

  update();
}

void
HSCMembers::listUpdated()
{
  QFile file(filename());
  if (! file.open(QIODevice::ReadOnly)) {
    qDebug() << "HSC: Cannot open member list at" << filename() << file.errorString();
    return;
  }

  // skip first line
  QString line = QString::fromUtf8(file.readLine()).simplified();
  do {
    line = QString::fromUtf8(file.readLine()).simplified();
    QStringList row = line.split(";");
    if (3 <= row.size()) {
      MemberShip mem = MEMB_NONE;
      if ("-" != row.at(2))
        mem = (MemberShip) (mem|MEMB_HSC);
      if ("-" != row.at(3))
        mem = (MemberShip) (mem|MEMB_SHSC);
      if ("-" != row.at(4))
        mem = (MemberShip) (mem|MEMB_VHSC);
      if ("-" != row.at(5))
        mem = (MemberShip) (mem|MEMB_EHSC);
      _members.insert(row.at(1),mem);
    }
  } while (! line.isEmpty());

  qDebug() << "HSC: Loaded" << _members.size() << "member entries.";

  file.close();
}

bool
HSCMembers::isMember(const QString &call) const {
  return _members.contains(call);
}

HSCMembers::MemberShip
HSCMembers::membership(const QString &call) const {
  if (! isMember(call))
    return MEMB_NONE;
  return _members[call];
}
