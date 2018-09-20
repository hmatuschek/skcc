#ifndef POPUP_H
#define POPUP_H

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QPropertyAnimation>
#include <QTimer>


class PopUp : public QWidget
{
	Q_OBJECT

	Q_PROPERTY(qint16 popupDuration MEMBER _popupDuration)
	Q_PROPERTY(float popupOpacity READ getPopupOpacity WRITE setPopupOpacity)

	void setPopupOpacity(float opacity);
	float getPopupOpacity() const;

public:
	explicit PopUp(QWidget *parent = nullptr);

protected:
	void paintEvent(QPaintEvent *event);    // The background will be drawn through the redraw method

public slots:
	void setPopupText(const QString& text); // Setting text notification
	void show();

private slots:
    void hideAnimation();                   // Slot start the animation hide
    void hide();

private:
	QLabel _label;
	QGridLayout _layout;
	QPropertyAnimation _animation;
	float _popupOpacity;
	QTimer *_timer;
	quint16 _popupDuration;
};


#endif // POPUP_H
