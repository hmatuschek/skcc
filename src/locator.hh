#ifndef LOCATOR_HH
#define LOCATOR_HH

#include <QString>

#define R_EARTH 6371.2

bool loc2deg(const QString &loc, double &lon, double &cla);
double great_circle_distance(double l1, double c1, double l2, double c2, double R=R_EARTH);


#endif // LOCATOR_HH
