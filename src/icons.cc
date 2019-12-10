#include "icons.hh"

IconProvider *IconProvider::_provider = nullptr;


IconProvider::IconProvider()
{
  _icons.append(QVector<QIcon>());
  _icons.back().append(QIcon("://icons/bullhorn-inv-8x.png"));
  _icons.back().append(QIcon("://icons/bullhorn-gray-2x.png"));
  _icons.back().append(QIcon("://icons/bullhorn-8x.png"));
  _icons.back().append(QIcon("://icons/monitor-inv-2x.png"));
  _icons.back().append(QIcon("://icons/book-inv-2x.png"));
  _icons.back().append(QIcon("://icons/list-inv-2x.png"));
  _icons.back().append(QIcon("://icons/book-inv-2x.png"));
  _icons.back().append(QIcon("://icons/browser-inv-2x.png"));
  _icons.back().append(QIcon("://icons/globe-inv-2x.png"));
  _icons.back().append(QIcon("://icons/wrench-inv-2x.png"));
  _icons.back().append(QIcon("://icons/power-standby-inv-2x.png"));
  _icons.back().append(QIcon("://icons/power-standby-inv-2x.png"));

  _icons.append(QVector<QIcon>());
  _icons.back().append(QIcon("://icons/bullhorn-8x.png"));
  _icons.back().append(QIcon("://icons/bullhorn-gray-2x.png"));
  _icons.back().append(QIcon("://icons/bullhorn-8x.png"));
  _icons.back().append(QIcon("://icons/monitor-2x.png"));
  _icons.back().append(QIcon("://icons/book-2x.png"));
  _icons.back().append(QIcon("://icons/list-2x.png"));
  _icons.back().append(QIcon("://icons/book-2x.png"));
  _icons.back().append(QIcon("://icons/browser-2x.png"));
  _icons.back().append(QIcon("://icons/globe-2x.png"));
  _icons.back().append(QIcon("://icons/wrench-2x.png"));
  _icons.back().append(QIcon("://icons/power-standby-2x.png"));
  _icons.back().append(QIcon("://icons/power-standby-2x.png"));
}

const QIcon &
IconProvider::get(Icon icon) {
  if (nullptr == _provider)
    _provider = new IconProvider();
  Theme theme = DARK_THEME;
#ifdef Q_OS_MACOSX
  theme = LIGHT_THEME;
#endif
  return _provider->_icons[theme][icon];
}
