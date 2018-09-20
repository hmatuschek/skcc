#ifndef ADI_HH
#define ADI_HH

#include <QFile>
#include <QString>
#include <QDateTime>
#include <QList>


class LogFile
{
public:
	typedef enum {
		WORKED,
		NEW_QSO,
		NEW_SLOT,
		NEW_BAND,
		NEW_DXCC
	} Match;

	typedef struct {
		QString   call;
		QDateTime time;
		QString   band;
		QString   mode;
		int       dxcc;

		bool isValid() const;
	} Record;

public:
  LogFile(const QString &filename);

  void insert(const Record &qso);
  Match isNew(const QString &call, const QString &band, const QString &mode) const;

public slots:
  void update();

private:
  int readRecord(const QString data, int start);

private:
  QString _filename;
  QList<Record> _log;
};

#endif // ADI_HH
