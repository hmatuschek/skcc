#ifndef CLUBMEMBERSHIP_HH
#define CLUBMEMBERSHIP_HH

#include <QObject>

// Forward declaration
class SKCCMembers;
class AGCWMembers;
class HSCMembers;


struct __attribute__((packed)) Membership {
  union {
    unsigned int skcc : 1,
      agcw : 1,
      hsc  : 1,
      vhsc : 1,
      shsc : 1,
      ehsc : 1;
    unsigned int bits;
  };

  Membership(unsigned int bits=0);
  operator unsigned int() const;
  operator unsigned int();

  bool any() const;
  QStringList names() const;

  Membership operator |(const Membership &other);
  Membership operator &(const Membership &other);
};


class ClubMembership : public QObject
{
  Q_OBJECT

public:
  explicit ClubMembership(QObject *parent = nullptr);

  bool isMember(const QString &call) const;
  Membership membership(const QString &call) const;

protected:
  SKCCMembers *_skcc;
  AGCWMembers *_agcw;
  HSCMembers  *_hsc;
};

#endif // CLUBMEMBERSHIP_HH
