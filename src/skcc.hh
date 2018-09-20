#ifndef SKCC_HH
#define SKCC_HH

#include <QObject>
#include "webfile.hh"


/** Downloads the current list of SKCC members. */
class SKCCMembers: public WebFile
{
	Q_OBJECT

public:
  /** Constructs the list of current SKCC members. */
	explicit SKCCMembers(QObject *parent=nullptr);

  /** Returns @c true if the given call is a SKCC member. */
	bool isMember(const QString &call) const;
  /** Returns the ID of the given SKCC member. */
	int memberID(const QString &call) const;


private slots:
	void listUpdated();

private:
  QNetworkAccessManager _WebCtrl;
  QHash<QString, int> _members;
};

#endif // SKCC_HH
