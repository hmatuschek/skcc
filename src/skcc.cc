#include "skcc.hh"
#include <QStandardPaths>
#include <QDir>

SKCCMembers::SKCCMembers(QObject *parent)
    : WebFile("skcclist.txt", QUrl("http://www.skccgroup.com/membership_data/skccdata.txt"), 7, parent),
      _members()
{
  connect(this, SIGNAL(updated()), this, SLOT(listUpdated()));

  update();
}

void
SKCCMembers::listUpdated()
{
  QFile file(filename());
  if (! file.open(QIODevice::ReadOnly)) {
    qDebug() << "SKCC: Cannot open member list at" << filename() << file.errorString();
    return;
  }

  // skip first line
  QString line = QString::fromUtf8(file.readLine()).simplified();
  do {
    line = QString::fromUtf8(file.readLine()).simplified();
    QStringList row = line.split("|");
    if (6 < row.size()) {
      _members.insert(row.at(1),row.at(6).toInt());
    }
  } while (! line.isEmpty());

  qDebug() << "SKCC: Loaded" << _members.size() << "member entries.";

  file.close();
}

bool
SKCCMembers::isMember(const QString &call) const {
  return _members.contains(call);
}

int
SKCCMembers::memberID(const QString &call) const {
  return _members[call];
}
