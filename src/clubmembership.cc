#include "clubmembership.hh"
#include "skcc.hh"
#include "agcw.hh"
#include "hsc.hh"


Membership::Membership(unsigned int bits) {
  (unsigned int &)(*this) = bits;
}

Membership::operator unsigned int() const {
  return *(unsigned int *)(this);
}

Membership::operator unsigned int() {
  return *(unsigned int *)(this);
}

bool
Membership::any() const {
  return skcc | agcw | hsc | vhsc | shsc | ehsc;
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
  else if (shsc)
    n.append("shsc");
  else if (vhsc)
    n.append("vhsc");
  if (hsc)
    n.append("hsc");
  return n;
}

Membership
Membership::operator |(const Membership &other) {
  return Membership((unsigned int)(*this) | (unsigned int)(other));
}

Membership
Membership::operator &(const Membership &other) {
  return Membership((unsigned int)(*this) & (unsigned int)(other));
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
