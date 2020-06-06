#include "dxcc.hh"
#include <QVector>
#include <QPair>
#include <QRegularExpression>
#include <QHash>
#include <QChar>


int dxcc_from_call(const QString &call) {
  return DXCCList::instance()->dxcc(call);
}

QString dxcc_name_from_call(const QString &call) {
  return DXCCList::instance()->dxcc_name(call);
}



/* ********************************************************************************************* *
 * DXCCListReader object
 * ********************************************************************************************* */
DXCCListReader::DXCCListReader(DXCCList &list)
  : _list(list)
{
  // pass...
}

bool
DXCCListReader::read(const QString &line) {
  State state = START;
  bool  read_footnote = false;
  QList<QRegularExpression> pattern;
  QList<QString> contients, itu, cq;
  QString name;
  int id = -1;

  QString buffer;
  buffer.reserve(256);

  foreach (QChar c,  line) {
    if (read_footnote) {
      if (c.isNumber() || c.isLetter()) {
        // pass..
      } else if (')'==c) {
        read_footnote = false;
      } else {
        qDebug() << "Unexpected Footnote char " << c << "in line" << line;
        return false;
      }
      continue;
    }

    switch (state) {
      case START:
        if (c.isSpace()) {
          state = START;
        } else if (c.isUpper() || c.isDigit() || ('['==c)) {
          state = PATTERN_START;
          buffer.append(c);
        } else {
          qDebug() << "Unexpected Pattern char " << c << "in line" << line;
          return false;
        }
        break;

      case PATTERN_START:
        if (c.isUpper() || c.isDigit() || ('-'==c) || ('/'==c) || ('['==c) || (']'==c)) {
          state = PATTERN_START;
          buffer.append(c);
        } else if ('_' == c) {
          buffer.append("[0-9]");
        } else if (',' == c) {
          buffer.prepend("^("); buffer.append(")");
          pattern.append(QRegularExpression(buffer, QRegularExpression::CaseInsensitiveOption));
          buffer.clear();
          state = PATTERN_START;
        } else if (('*'==c) || ('#'==c) || ('^'==c) || c.isSpace()) {
          buffer.prepend("^("); buffer.append(")");
          pattern.append(QRegularExpression(buffer, QRegularExpression::CaseInsensitiveOption));
          buffer.clear();
          state = PATTERN_COMMENT;
        } else if ('('==c) {
          read_footnote = true;
        } else {
          qDebug() << "Unexpected Pattern char " << c << "in line" << line;
          return false;
        }
        break;

      case PATTERN_COMMENT:
        if (('*'==c) || ('#'==c) || ('^'==c) || c.isSpace()) {
          state = PATTERN_COMMENT;
        } else if ('('==c) {
          read_footnote = true;
        } else if (c.isLetter()) {
          state = NAME_START;
          buffer.append(c);
        } else {
          qDebug() << "Unexpected Pattern char " << c << "in line" << line;
          return false;
        }
        break;

      case NAME_START:
        if (! c.isSpace()) {
          buffer.append(c);
          state = NAME_START;
        } else if (' ' == c) {
          state = NAME_SPACE;
        } else {
          qDebug() << "Unexpected Name char " << c << "in line" << line;
          return false;
        }
        break;

      case NAME_SPACE:
        if (! c.isSpace()) {
          buffer.append(' '); buffer.append(c);
          state = NAME_START;
        } else {
          name = buffer; buffer.clear();
          state = NAME_END;
        }
        break;

      case NAME_END:
        if (c.isUpper()) {
          buffer.append(c);
          state = CONTINENT_START;
        } else if ('('==c)  {
          state = CONTINENT_START;
          read_footnote = true;
        } else if (c.isSpace()) {
          state = NAME_END;
        } else {
          qDebug() << "Unexpected Name char " << c << "in line" << line;
          return false;
        }
        break;

      case CONTINENT_START:
        if (c.isUpper()) {
          state = CONTINENT_START;
          buffer.append(c);
        } else if (',' == c) {
          contients.append(buffer); buffer.clear();
        } else if (c.isSpace()) {
          contients.append(buffer); buffer.clear();
          state = CONTINENT_END;
        } else {
          qDebug() << "Unexpected Contient char " << c << "in line" << line;
          return false;
        }
        break;

      case CONTINENT_END:
        if (c.isSpace()) {
          state = CONTINENT_END;
        } else if (c.isNumber()) {
          state = ITU_START;
          buffer.append(c);
        } else if ('(' == c) {
          state = ITU_START;
          read_footnote = true;
        } else {
          qDebug() << "Unexpected Contient char " << c << "in line" << line;
          return false;
        }
        break;

      case ITU_START:
        if (c.isSpace()) {
          state = ITU_END;
          itu.append(buffer);
          buffer.clear();
        } else if (c.isNumber() || ('-'==c)) {
          buffer.append(c);
        } else if (','==c) {
          itu.append(buffer);
          buffer.clear();
        } else {
          qDebug() << "Unexpected ITU char " << c << "in line" << line;
          return false;
        }
        break;

      case ITU_END:
        if (c.isSpace()) {
          state = ITU_END;
        } else if (c.isNumber()) {
          buffer.append(c);
          state = CQ_START;
        } else if ('(' == c) {
          state = CQ_START;
          read_footnote = true;
        } else {
          qDebug() << "Unexpected ITU char " << c << "in line" << line;
          return false;
        }
        break;

      case CQ_START:
        if (c.isSpace()) {
          state = CQ_END;
          cq.append(buffer);
          buffer.clear();
        } else if (c.isNumber() || ('-'==c)) {
          buffer.append(c);
        } else if (','==c) {
          cq.append(buffer);
          buffer.clear();
        } else {
          qDebug() << "Unexpected CQ char " << c << "in line" << line;
          return false;
        }
        break;

      case CQ_END:
        if (c.isSpace()) {
          state = CQ_END;
        } else if (c.isNumber()) {
          buffer.append(c);
          state = DXCC_START;
        } else if ('(' == c) {
          state = DXCC_START;
          read_footnote = true;
        } else {
          qDebug() << "Unexpected CQ char " << c << "in line" << line;
          return false;
        }
        break;

      case DXCC_START:
        if (c.isSpace()) {
          state = DXCC_END;
          id = buffer.toInt();
          buffer.clear();
        } else if (c.isNumber()) {
          buffer.append(c);
        } else {
          qDebug() << "Unexpected DXCC char " << c << "in line" << line;
          return false;
        }
        break;

      case DXCC_END:
        if (c.isSpace()) {
          state = DXCC_END;
        } else {
          qDebug() << "Unexpected DXCC char " << c << "in line" << line;
          return false;
        }
        break;
    }
  }

  if (0 > id)
    return false;

  qDebug() << "*** Got" << pattern << "for" << name  << "in cont." << contients << "ITU" << itu
           << "CQ" << cq << "and with ID" << id;

  foreach (QRegularExpression rule, pattern) {
    if (rule.pattern().isEmpty() || (! rule.isValid()))
      continue;
    _list.addRule(rule, name, id);
  }

  return true;
}


/* ********************************************************************************************* *
 * DXCCList object
 * ********************************************************************************************* */
DXCCList *DXCCList::_singleton = nullptr;


DXCCList::DXCCList(QObject *parent)
  : QObject(parent)
{
  QFile file(":/dxcc/dxcc.txt");
  if (! file.open(QIODevice::ReadOnly)) {
    qDebug() << "DXCC: Cannot open DXCC list " << file.fileName() << file.errorString();
    return;
  }

  DXCCListReader reader(*this);
  do {
    QString line = file.readLine();
    reader.read(line);
  } while (! file.atEnd());
  qDebug() << "DXCC: Loaded" << _rules.size() << "rules from" << file.fileName();
}

DXCCList::~DXCCList() {
  // pass...
}

DXCCList *
DXCCList::instance() {
  if (nullptr == DXCCList::_singleton)
    DXCCList::_singleton = new DXCCList();
  return DXCCList::_singleton;
}
void
DXCCList::addRule(const QRegularExpression &rule, const QString &name, int id) {
  _rules.append( { rule, name, id } );
  _dxcc_name[id] = name;
}

void
DXCCList::addRule(const Rule &rule) {
  _rules.append( rule );
  _dxcc_name[rule.id] = rule.name;
}

int
DXCCList::dxcc(const QString &call) {
  if (_dxcc_cache.contains(call))
    return _dxcc_cache[call];

  int match_len = 0;
  Rule match;
  foreach (Rule rule, _rules) {
    QRegularExpressionMatch m = rule.pattern.match(call);
    if (! m.hasMatch())
      continue;
    int len = m.captured(1).length();
    if (len > match_len) {
      match = rule;
      match_len = len;
    }
  }

  if (0 == match_len)
    return -1;
  _dxcc_cache[call] = match.id;
  return match.id;
}


QString
DXCCList::dxcc_name(const QString &call) {
  int nr = dxcc(call);
  if (! _dxcc_name.contains(nr))
    return "Unknown DXCC";
  return _dxcc_name[nr];
}

