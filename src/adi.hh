#ifndef ADI_HH
#define ADI_HH

#include <QObject>
#include <QFile>
#include <QString>
#include <QDateTime>
#include <QList>
#include <QFileSystemWatcher>
#include <QSet>
#include <QHash>


class LogFile: QObject
{
  Q_OBJECT

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

  class DXCCRecord {
  public:
    DXCCRecord();
    DXCCRecord(const QString &band, const QString &mode);
    void insert(const QString &band, const QString &mode);
    Match isNew(const QString &band, const QString &mode) const;

  protected:
    QHash< QString, QSet<QString> > _band_mode;
  };

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
  QSet<QString> _QSOs;
  QHash<int, DXCCRecord> _DXCCs;
  QFileSystemWatcher _watcher;
};

#endif // ADI_HH
