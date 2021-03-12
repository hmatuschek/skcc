#ifndef HSCMEMBERS_HH
#define HSCMEMBERS_HH

#include "webfile.hh"

class HSCMembers: public WebFile
{
  Q_OBJECT
public:
  typedef enum {
    MEMB_NONE  = 0,
    MEMB_HSC   = 1,
    MEMB_SHSC  = 2,
    MEMB_VHSC  = 4,
    MEMB_EHSC  = 8,
  } MemberShip;

public:
  explicit HSCMembers(QObject *parent=nullptr);

  /** Returns @c true if the given call is a HSC member. */
  bool isMember(const QString &call) const;
  MemberShip membership(const QString &call) const;

private slots:
  void listUpdated();

private:
  QNetworkAccessManager _WebCtrl;
  QHash<QString, MemberShip> _members;
};

#endif // HSCMEMBERS_HH
