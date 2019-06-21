#ifndef SELFSPOTMAP_HH
#define SELFSPOTMAP_HH

#include "map.hh"
#include "cccluster.hh"
#include <QTimer>

class Application;

class SelfSpotMapView: public MapView
{
	Q_OBJECT

public:
	explicit SelfSpotMapView(const QString &qthLoc, Application &app, QWidget *parent = nullptr);

  bool hasSpotter(const QString &call) const;

protected slots:
  void onSelfSpot(const Spot &spot, const QString &spotterLoc);
  void onClearSpots();

protected:
	Application &_app;
	QString _qthLoc;
  QTimer _clearTimer;
  QList<Spot> _spots;
};


#endif // SELFSPOTMAP_HH
