#include "colorbutton.hh"
#include <QColorDialog>

ColorButton::ColorButton(const QColor &color, QWidget *parent)
	: QPushButton(parent)
{
  setMinimumSize(50,15);
	setColor(color);
	connect(this, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
}

QColor
ColorButton::color() {
  return palette().color(QPalette::Button);
}

void
ColorButton::setColor(const QColor &color) {
  QPalette palette = this->palette();
  palette.setColor(QPalette::Button, color);
  setAutoFillBackground(true);
  setPalette(palette);
  update();
}

void
ColorButton::onClicked() {
	QColorDialog dialog(color());
	if (QDialog::Accepted != dialog.exec())
		return;
	setColor(dialog.selectedColor());
}
