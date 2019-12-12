#ifndef ICONS_H
#define ICONS_H

#include <QIcon>

class IconProvider
{
protected:
  IconProvider();

public:
  typedef enum {
    TRAY_ICON = 0,
    TRAY_ICON_INACTIVE,
    WINDOW_ICON,
    MENU_FILTER_ICON,
    MENU_LOG_ICON,
    MENU_SPOT_ICON,
    MENU_NOTIFY_ICON,
    MENU_SPOT_WINDOW_ICON,
    MENU_MAP_WINDOW_ICON,
    MENU_SETTINGS_ICON,
    MENU_ABOUT_ICON,
    MENU_QUIT_ICON
  } Icon;

  typedef enum {
    LIGHT_THEME = 0,
    DARK_THEME
  } Theme;

public:
  static const QIcon &get(Icon icon);

protected:
  static IconProvider *_provider;
  QVector< QVector<QIcon> > _icons;
};

#endif // ICONS_H
