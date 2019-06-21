#ifndef MAP_HH
#define MAP_HH

#include <QWidget>

class MapView : public QWidget
{
  Q_OBJECT

public:
	explicit MapView(const QString &centerLoc, QWidget *parent = nullptr);

  void addPoint(const QString &at, const QColor &color=Qt::black);
	void addArc(const QString &from, const QString &to, const QColor &color=Qt::black);

	QPoint toLogical(const QPoint &p) const;
	QPoint toWindow(const QPoint &p) const;

public slots:
	void resetZoom();
	void zoomIn();
	void zoomOut();
	void zoom(double scale, const QPoint &pt=QPoint());

protected:
	void wheelEvent(QWheelEvent *evt);
	void mousePressEvent(QMouseEvent *evt);
	void mouseMoveEvent(QMouseEvent *evt);
	void mouseReleaseEvent(QMouseEvent *evt);
	void resizeEvent(QResizeEvent *evt);
	void paintEvent(QPaintEvent *evt);

protected:
	double _scale;
	QRect _maprect;

	QImage _map;

	int _scrollDelta;
	QPoint _dragStart;
  QList< QPair<QColor, QPoint> > _points;
	QList< QPair<QColor, QVector<QPoint>> > _arcs;
};

#endif // MAP_HH
