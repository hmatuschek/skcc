#include "map.hh"
#include <QPainter>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QPoint>
#include "locator.hh"
#include <cmath>
#include <QDebug>
#include <QShortcut>
#include <QDate>
#include <QStandardPaths>
#include <QIcon>
#include "icons.hh"

QVector<QUrl> MapView::_mapURLs = {
  QUrl("https://eoimages.gsfc.nasa.gov/images/imagerecords/73000/73580/world.topo.bathy.200401.3x5400x2700.png"),
  QUrl("https://eoimages.gsfc.nasa.gov/images/imagerecords/73000/73605/world.topo.bathy.200402.3x5400x2700.png"),
  QUrl("https://eoimages.gsfc.nasa.gov/images/imagerecords/73000/73630/world.topo.bathy.200403.3x5400x2700.png"),
  QUrl("https://eoimages.gsfc.nasa.gov/images/imagerecords/73000/73655/world.topo.bathy.200404.3x5400x2700.png"),
  QUrl("https://eoimages.gsfc.nasa.gov/images/imagerecords/73000/73701/world.topo.bathy.200405.3x5400x2700.png"),
  QUrl("https://eoimages.gsfc.nasa.gov/images/imagerecords/73000/73726/world.topo.bathy.200406.3x5400x2700.png"),
  QUrl("https://eoimages.gsfc.nasa.gov/images/imagerecords/73000/73751/world.topo.bathy.200407.3x5400x2700.png"),
  QUrl("https://eoimages.gsfc.nasa.gov/images/imagerecords/73000/73776/world.topo.bathy.200408.3x5400x2700.png"),
  QUrl("https://eoimages.gsfc.nasa.gov/images/imagerecords/73000/73801/world.topo.bathy.200409.3x5400x2700.png"),
  QUrl("https://eoimages.gsfc.nasa.gov/images/imagerecords/73000/73826/world.topo.bathy.200410.3x5400x2700.png"),
  QUrl("https://eoimages.gsfc.nasa.gov/images/imagerecords/73000/73884/world.topo.bathy.200411.3x5400x2700.png"),
  QUrl("https://eoimages.gsfc.nasa.gov/images/imagerecords/73000/73909/world.topo.bathy.200412.3x5400x2700.png")
};


MapView::MapView(const QString &centerLoc, QWidget *parent)
  : QWidget(parent), _scale(1), _map(),
    _scrollDelta(0), _dragStart(), _downloadedMap(nullptr)
{
  setWindowTitle(tr("Map"));
  setWindowIcon(IconProvider::get(IconProvider::WINDOW_ICON));

  QString mapname = QString("world_%1_5k.png").arg(QDate::currentDate().month(), 2, 10, QChar('0'));
  QUrl mapurl(_mapURLs[QDate::currentDate().month()-1]);
  QString mappath = QStandardPaths::locate(QStandardPaths::AppDataLocation, mapname);
  if (mappath.isEmpty()) {
    _downloadedMap = new WebFile(mapname, mapurl, -1, this);
    _downloadedMap->update();
    mappath = ":/map/map.png";
  }
  _map.load(mappath);

  new QShortcut(QKeySequence("Ctrl+0"), this, SLOT(resetZoom()));
  new QShortcut(QKeySequence("Ctrl++"), this, SLOT(zoomIn()));
  new QShortcut(QKeySequence("Ctrl+-"), this, SLOT(zoomOut()));

  double lon, lat;
  loc2deg(centerLoc, lon, lat);
  lon += 180;
  double x = _map.width()*lon/360, y = _map.height()*lat/180;
  _maprect = QRect(QPoint(x,y)-QPoint(width(),height())/2, size());
}

QPoint
MapView::toLogical(const QPoint &p) const {
  return p/_scale + _maprect.topLeft();
}

QPoint
MapView::toWindow(const QPoint &p) const {
  return (p-_maprect.topLeft())*_scale;
}

void
MapView::addPoint(const QString &at, const QColor &color) {
  double l1, c1;
  loc2deg(at, l1, c1);
  l1 = l1/180+180; c1 = c1/180;
  _points.push_back(QPair<QColor, QPoint>(color, QPoint(_map.width()*l1, _map.height()*c1)));
}

void
MapView::addArc(const QString &from, const QString &to, const QColor &color) {
  double l1, c1, l2, c2;
  loc2deg(from, l1, c1);
  l1 = M_PI*l1/180; c1 = M_PI/2 - M_PI*c1/180;
  loc2deg(to, l2, c2);
  l2 = M_PI*l2/180; c2 = M_PI/2 - M_PI*c2/180;

  double dl = l2-l1, dc = c2-c1;
  double a  = std::sin(dc/2)*std::sin(dc/2) + std::cos(c1)*std::cos(c2)*std::sin(dl/2)*std::sin(dl/2);
  double d  = 2*std::atan2(std::sqrt(a),std::sqrt(1-a));
  int    N  = 100;
  double df = 1./N, f = df;
  QVector<QPoint> arc; arc.reserve(100);
  arc.push_back(QPoint(_map.width()*(l1+M_PI)/M_PI/2, _map.height()*(M_PI/2-c1)/M_PI));
  for (int i=0; i<N; i++, f+=df){
    double a = std::sin((1-f)*d)/std::sin(d);
    double b = std::sin(f*d)/std::sin(d);
    double x = a*std::cos(c1)*std::cos(l1)+b*std::cos(c2)*std::cos(l2);
    double y = a*std::cos(c1)*std::sin(l1)+b*std::cos(c2)*std::sin(l2);
    double z = a*std::sin(c1)+b*std::sin(c2);
    double l = std::atan2(y,x);
    double c = std::atan2(z,std::sqrt(x*x+y*y));
    arc.push_back(QPoint(_map.width()*(l+M_PI)/M_PI/2, _map.height()*(M_PI/2-c)/M_PI));
  }
  _arcs.push_back(QPair<QColor, QVector<QPoint>>(color,arc));
}

void
MapView::resetZoom() {
  zoom(1);
}

void
MapView::zoomIn() {
  zoom(_scale * 1.25);
}

void
MapView::zoomOut() {
  zoom(_scale * 0.8);
}

void
MapView::zoom(double scale, const QPoint &pt) {
  QPoint ac = _maprect.center()-_maprect.topLeft();
  if (! pt.isNull())
    ac = toLogical(pt)-_maprect.topLeft();
  double rx = double(ac.x())/_maprect.width();
  double ry = double(ac.y())/_maprect.height();
  _scale = scale;
  if (_scale > 3)
    _scale = 3;
  if (_scale < 0.1)
    _scale = 0.1;
  QSize size = this->size()/_scale;
  _maprect.setSize(size);
  // Ensure relative position remains the same.
  QPoint nc(std::round(rx*_maprect.width()),
            std::round(ry*_maprect.height()));
  _maprect.translate(ac-nc);
  update();
}

void
MapView::mousePressEvent(QMouseEvent *evt) {
  QWidget::mousePressEvent(evt);
  if (evt->button() == Qt::LeftButton)
    _dragStart = QPoint(evt->x(), evt->y());
  else
    _dragStart = QPoint();
}

void
MapView::mouseMoveEvent(QMouseEvent *evt) {
  QWidget::mouseMoveEvent(evt);
  if (_dragStart.isNull())
    return;
  _maprect.translate((_dragStart-evt->pos())/_scale);
  _dragStart = evt->pos();
  update();
}

void
MapView::mouseReleaseEvent(QMouseEvent *evt) {
  QWidget::mouseReleaseEvent(evt);
  _dragStart = QPoint();
}

void
MapView::wheelEvent(QWheelEvent *evt) {
  _scrollDelta += evt->angleDelta().y();
  _scale -= double(_scrollDelta)/2880;
  _scrollDelta = 0;
  zoom(_scale, evt->pos());
}

void
MapView::resizeEvent(QResizeEvent *evt) {
  QWidget::resizeEvent(evt);
  QPoint c = _maprect.center();
  QSize size = evt->size()/_scale;
  _maprect.setSize(size);
  _maprect.setTopLeft(c-QPoint(size.width()/2, size.height()/2));
  update();
}

void
MapView::paintEvent(QPaintEvent *evt) {
  QPainter painter;
  painter.begin(this);
  painter.setRenderHint(QPainter::Antialiasing, true);

  painter.drawImage(evt->rect(), _map, _maprect);

  QPen pen; pen.setWidth(2);
  QPair<QColor, QPoint> pt;
  foreach (pt, _points) {
    pen.setColor(pt.first);
    painter.setPen(pen);
    painter.drawEllipse(QPointF(toWindow(pt.second)), 5,5);
  }

  pen.setWidth(2);
  QPair<QColor, QVector<QPoint>> arc;
  foreach (arc, _arcs) {
    pen.setColor(arc.first);
    painter.setPen(pen);
    QPainterPath path;
    path.moveTo(toWindow(arc.second[0]));
    for (int i=1; i<arc.second.size(); i++){
      path.lineTo(toWindow(arc.second[i]));
    }
    painter.drawPath(path);
    painter.drawEllipse(toWindow(arc.second.back()), 3,3);
  }
  QWidget::paintEvent(evt);
  painter.end();
}

