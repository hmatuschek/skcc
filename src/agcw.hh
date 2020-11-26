#ifndef AGCWMEMBERS_HH
#define AGCWMEMBERS_HH

#include "webfile.hh"

class AGCWMembers: public WebFile
{
  Q_OBJECT

public:
  explicit AGCWMembers(QObject *parent=nullptr);

  /** Returns @c true if the given call is a AGCW member. */
  bool isMember(const QString &call) const;
  /** Returns the ID of the given AGCW member. */
  int memberID(const QString &call) const;


private slots:
  void listUpdated();

private:
  QNetworkAccessManager _WebCtrl;
  QHash<QString, int> _members;
};

#endif // AGCWMEMBERS_HH
