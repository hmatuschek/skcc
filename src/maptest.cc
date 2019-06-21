#include <QApplication>
#include "map.hh"

int main(int argc, char *argv[])
{
	QApplication app(argc,argv);

	MapView map("JO62TH");
  map.addArc("JO62TH", "RE57KA");

	map.show();

	app.exec();

	return 0;
}
