#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QTableView>
#include "spottable.hh"
#include "popup.hh"


class SpotWindow : public QTableView
{
  Q_OBJECT

public:
  explicit SpotWindow(SpotTable *spots, QWidget *parent = nullptr);

protected slots:
  void onSectionResized(int logicalIndex, int oldSize, int newSize);

protected:
	SpotTable *_spottable;
};

#endif // MAINWINDOW_HH
