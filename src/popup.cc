#include "popup.hh"
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>



PopUp::PopUp(QWidget *parent)
  : QWidget(parent), _popupDuration(3000)
{
  setWindowFlags(Qt::FramelessWindowHint |        // Disable window decoration
                 Qt::Tool |                       // Discard display in a separate window
                 Qt::WindowStaysOnTopHint);       // Set on top of all windows
  setAttribute(Qt::WA_TranslucentBackground);     // Indicates that the background will be transparent
  setAttribute(Qt::WA_ShowWithoutActivating);     // At the show, the widget does not get the focus automatically
  setAttribute(Qt::WA_MacAlwaysShowToolWindow);

  _animation.setTargetObject(this);                // Set the target animation
  _animation.setPropertyName("popupOpacity");      //
  connect(&_animation, &QAbstractAnimation::finished, this, &PopUp::hide);

  _label.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  _label.setStyleSheet("QLabel { color : white; "
                       "margin-top: 6px;"
                       "margin-bottom: 6px;"
                       "margin-left: 10px;"
                       "margin-right: 10px; }");

  _layout.addWidget(&_label, 0, 0);
  setLayout(&_layout);

  _timer = new QTimer();
  connect(_timer, &QTimer::timeout, this, &PopUp::hideAnimation);
}

void PopUp::paintEvent(QPaintEvent *event)
{
  Q_UNUSED(event)

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  QRect roundedRect;
  roundedRect.setX(rect().x() + 5);
  roundedRect.setY(rect().y() + 5);
  roundedRect.setWidth(rect().width() - 10);
  roundedRect.setHeight(rect().height() - 10);

  painter.setBrush(QBrush(QColor(0,0,0,180)));
  painter.setPen(Qt::NoPen);

  painter.drawRoundedRect(roundedRect, 10, 10);
}

void PopUp::setPopupText(const QString &text)
{
  _label.setText(text);    // Set the text in the Label
  adjustSize();            // With the recalculation notice sizes
}

void PopUp::show()
{
  setWindowOpacity(0.0);  // Set the transparency to zero

  _animation.setDuration(150);     // Configuring the duration of the animation
  _animation.setStartValue(0.0);   // The start value is 0 (fully transparent widget)
  _animation.setEndValue(1.0);     // End - completely opaque widget

  setGeometry(QApplication::desktop()->availableGeometry().width() - 36 - width()
              + QApplication::desktop()->availableGeometry().x(),
              QApplication::desktop()->availableGeometry().height() - 36 - height()
              + QApplication::desktop()->availableGeometry().y(),
              width(),
              height());
  QWidget::show();

  _animation.start();
  _timer->start(_popupDuration);
}

void PopUp::hideAnimation()
{
  _timer->stop();
  _animation.setDuration(1000);
  _animation.setStartValue(1.0);
  _animation.setEndValue(0.0);
  _animation.start();
}

void PopUp::hide()
{
  // If the widget is transparent, then hide it
  if (getPopupOpacity() == 0.0)
    QWidget::hide();
}

void PopUp::setPopupOpacity(float opacity)
{
  _popupOpacity = opacity;
  setWindowOpacity(opacity);
}

float PopUp::getPopupOpacity() const
{
  return _popupOpacity;
}
