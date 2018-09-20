#ifndef BANDS_HH
#define BANDS_HH

#include <QString>

#define BAND_2200M "2200M"
#define BAND_630M  "630M"
#define BAND_160M  "160M"
#define BAND_80M   "80M"
#define BAND_60M   "60M"
#define BAND_40M   "40M"
#define BAND_30M   "30M"
#define BAND_20M   "20M"
#define BAND_17M   "17M"
#define BAND_15M   "15M"
#define BAND_12M   "12M"
#define BAND_10M   "10M"
#define BAND_6M    "6M"


QString freq2band(double f);

#endif // BANDS_HH
