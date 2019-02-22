#include "colorbutton.hh"
#include <QColorDialog>

ColorButton::ColorButton(const QColor &color, QWidget *parent)
	: QToolButton(parent), _color(color)
{
	setColor(color);
	connect(this, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
}

const QColor &
ColorButton::color() const {
  return _color;
}

void
ColorButton::setColor(const QColor &color) {
  _color = color;
  QPixmap pixmap(48,48);
  pixmap.fill(_color);
  setIcon(QIcon(pixmap));
}

void
ColorButton::onClicked() {
	QColorDialog dialog(color());
	if (QDialog::Accepted != dialog.exec())
		return;
	setColor(dialog.selectedColor());
}
