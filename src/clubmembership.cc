#include "clubmembership.hh"
#include "skcc.hh"
#include "agcw.hh"
#include "hsc.hh"


Membership::Membership(unsigned int bits) {
  this->bits = bits;
}

Membership::operator unsigned int() const {
  return bits;
}

Membership::operator unsigned int() {
  return bits;
}

bool
Membership::any() const {
  return skcc || agcw || hsc || vhsc || shsc || ehsc;
}

QStringList
Membership::names() const {
  QStringList n;
  if (skcc)
    n.append("skcc");
  if (agcw)
    n.append("agcw");
  if (ehsc)
    n.append("ehsc");
  if (shsc)
    n.append("shsc");
  if (vhsc)
    n.append("vhsc");
  if (hsc)
    n.append("hsc");
  return n;
}

Membership
Membership::operator |(const Membership &other) {
  return Membership(bits | other.bits);
}

Membership
Membership::operator &(const Membership &other) {
  return Membership(bits & other.bits);
}




ClubMembership::ClubMembership(QObject *parent)
  : QObject(parent), _skcc(new SKCCMembers(this)), _agcw(new AGCWMembers(this)),
    _hsc(new HSCMembers(this))
{
  // pass...
}

bool
ClubMembership::isMember(const QString &call) const {
  return _skcc->isMember(call) | _agcw->isMember(call) | _hsc->isMember(call);
}

Membership
ClubMembership::membership(const QString &call) const {
  return _skcc->membership(call) | _agcw->membership(call) | _hsc->membership(call);
}
