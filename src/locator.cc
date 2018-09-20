#include "locator.hh"
#include <cmath>


bool
loc2deg(const QString &loc, double &lon, double &cla)
{
	lon = cla = 0;
	if (2 > loc.size())
		return false;

	QChar l = loc[0].toUpper();
	QChar c = loc[1].toUpper();
	lon += double(int(l.toLatin1())-'A')*20;
	cla += double(int(c.toLatin1())-'A')*10;

	if (4 > loc.size()) {
		lon = lon - 180;
		cla = 180 - cla;
		return true;
	}

	l = loc[2].toUpper();
	c = loc[3].toUpper();
	lon += double(int(l.toLatin1())-'0')*2;
	cla += double(int(c.toLatin1())-'0')*1;

	if (6 > loc.size()){
		lon = lon - 180;
		cla = 180 - cla;
		return true;
	}

	l = loc[4].toUpper();
	c = loc[5].toUpper();
	lon += double(int(l.toLatin1())-'A')/12;
	cla += double(int(c.toLatin1())-'A')/24;

	lon = lon - 180;
	cla = 180 - cla;
	return true;
}


double great_circle_distance(double l1, double c1, double l2, double c2, double R) {
  l1 = M_PI*l1/180; c1 = M_PI*(90-c1)/180;
  l2 = M_PI*l2/180; c2 = M_PI*(90-c2)/180;
  double dc = c2 - c1;
  double dl = l2 - l1;
  double a = std::sin(dc/2)*std::sin(dc/2) + std::cos(c1)*std::cos(c2)*std::sin(dl/2)*std::sin(dl/2);
  double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1-a));
  return R*c;
}
