#include <QCoreApplication>
#include "dxcc.hh"
#include <iostream>
#include <QFile>

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);

	if (argc < 2) {
		std::cerr << "Usage: dxcc_test FILENAME" << std::endl;
		return -1;
	}

	DXCCList lst;
	DXCCListReader reader(lst);

	QFile file(argv[1]);
	if (! file.open(QIODevice::ReadOnly)) {
		std::cerr << "Cannot open file '" << argv[1] << "'.";
		return -1;
	}

	do {
		QString line = file.readLine();
		reader.read(line);
	} while (! file.atEnd());

	return 0;
}
