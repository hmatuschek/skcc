#include "application.hh"
#include "settings.hh"
#include "bands.hh"
#include "settingsdialog.hh"
#include "menu.hh"

#include <QSound>
#include <QInputDialog>


Application::Application(int &argc, char *argv[])
    : QApplication(argc, argv)
{
	// Do not quit application if the last window is closed.
    setQuitOnLastWindowClosed(false);

    // Set application name
    setApplicationName("skcc");
    setOrganizationName("io.github.hmatuschek");
    setOrganizationDomain("io.github.hmatuschek");

    Settings settings;

    _spots = new SpotTable(settings.call(), settings.locator(), settings.logFile(),
                           settings.clusterHost(), settings.clusterPort(), settings.showSelfSpots(),
                           settings.showBeaconSpots(), settings.maxSpotterDist(),
                           settings.maxSpotAge(), settings.minLogMatch(), settings.maxSpeed(),
                           settings.minSNR(), this);

    _spots->bands().clear();
    if (settings.band2200m()) _spots->bands().insert(BAND_2200M);
    if (settings.band630m())  _spots->bands().insert(BAND_630M);
    if (settings.band160m())  _spots->bands().insert(BAND_160M);
    if (settings.band80m())   _spots->bands().insert(BAND_80M);
    if (settings.band60m())   _spots->bands().insert(BAND_60M);
    if (settings.band40m())   _spots->bands().insert(BAND_40M);
    if (settings.band30m())   _spots->bands().insert(BAND_30M);
    if (settings.band20m())   _spots->bands().insert(BAND_20M);
    if (settings.band17m())   _spots->bands().insert(BAND_17M);
    if (settings.band15m())   _spots->bands().insert(BAND_15M);
    if (settings.band12m())   _spots->bands().insert(BAND_12M);
    if (settings.band10m())   _spots->bands().insert(BAND_10M);
    if (settings.band6m())    _spots->bands().insert(BAND_6M);

    _spots->friends() = settings.friends();
}

Application::~Application() {
}

const SpotTable *
Application::spots() const {
  return _spots;
}

SpotTable *
Application::spots(){
  return _spots;
}

