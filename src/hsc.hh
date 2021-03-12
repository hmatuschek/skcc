#ifndef HSCMEMBERS_HH
#define HSCMEMBERS_HH

#include "webfile.hh"
#include "clubmembership.hh"


class HSCMembers: public WebFile
{
  Q_OBJECT

public:
  explicit HSCMembers(QObject *parent=nullptr);

  /** Returns @c true if the given call is a HSC member. */
  bool isMember(const QString &call) const;
  Membership membership(const QString &call) const;

private slots:
  void listUpdated();

private:
  QNetworkAccessManager _WebCtrl;
  QHash<QString, Membership> _members;
};

#endif // HSCMEMBERS_HH
