#include "bands.hh"

QString freq2band(double f) {
	if ((f>=135.7) && (f<=137.8))
		return "2200M";
	else if ((f>=472.0) && (f<=479.0))
		return "630M";
	else if ((f>=1810) && (f<=2000))
		return "160M";
	else if ((f>=3500) && (f<=3800))
		return "80M";
	else if ((f>=5258.5) && (f<=5406.5))
		return "60M";
	else if ((f>=7000) && (f<=7200))
		return "40M";
	else if ((f>=10100) && (f<=10150))
		return "30M";
	else if ((f>=14000) && (f<=14350))
		return "20M";
	else if ((f>=18068) && (f<=18168))
		return "17M";
	else if ((f>=21000) && (f<=21450))
		return "15M";
	else if ((f>=24890) && (f<=24990))
		return "12M";
	else if ((f>=28000) && (f<=29700))
		return "10M";
  return "";
}
