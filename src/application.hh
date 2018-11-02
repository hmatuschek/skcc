#ifndef APPLICATION_HH
#define APPLICATION_HH

#include <QApplication>
#include "spottable.hh"


class Application : public QApplication
{
	Q_OBJECT

public:
  Application(int &argc, char *argv[]);
  virtual ~Application();

  const SpotTable *spots() const;
  SpotTable *spots();

private:
  SpotTable *_spots;
};

#endif // APPLICATION_HH
