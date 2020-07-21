#ifndef DXCC_HH
#define DXCC_HH

#include "webfile.hh"
#include <QObject>
#include <QString>
#include <QRegularExpression>

int dxcc_from_call(const QString &call);
QString dxcc_name_from_call(const QString &call);


class DXCCList: public QObject
{
  Q_OBJECT

public:
  typedef struct {
    QRegularExpression pattern;
    QString name;
    int     id;
  } Rule;

protected:
  explicit DXCCList(QObject *parent=0);
  static DXCCList *_singleton;

public:
  virtual ~DXCCList();

  void addRule(const Rule &rule);
  void addRule(const QRegularExpression &rule, const QString &name, int id);

  int dxcc(const QString &call);
  QString dxcc_name(const QString &call);

  static DXCCList *instance();

protected:
  QList<Rule> _rules;
  QHash<QString, int> _dxcc_cache;
  QHash<int, QString> _dxcc_name;
};


class DXCCListReader
{
protected:
  typedef enum {
    START,
    PATTERN_START, PATTERN_COMMENT,
    NAME_START, NAME_SPACE, NAME_END,
    CONTINENT_START, CONTINENT_END,
    ITU_START, ITU_END,
    CQ_START, CQ_END,
    DXCC_START, DXCC_END
  } State;

public:
  DXCCListReader(DXCCList &list);

  bool read(const QString &line);

protected:
  DXCCList &_list;
};



#endif // DXCC_HH
