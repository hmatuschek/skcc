#include "mainwindow.hh"
#include <QMenuBar>
#include <QAction>
#include <QActionGroup>
#include <QSortFilterProxyModel>
#include <QTableView>
#include <QInputDialog>
#include <QHeaderView>
#include <QSound>

#include "bands.hh"
#include "settings.hh"
#include "adi.hh"


SpotWindow::SpotWindow(SpotTable *spots, QWidget *parent)
    : QTableView(parent), _spottable(spots)
{
  setWindowTitle(tr("Spot table"));
  setWindowIcon(QIcon("://icons/bullhorn-8x.png"));
  QSortFilterProxyModel *filter = new QSortFilterProxyModel();
  filter->setSortRole(Qt::EditRole);
	filter->setSourceModel(_spottable);
  filter->setFilterKeyColumn(6);

	setModel(filter);
  setSortingEnabled(true);
  sortByColumn(6, Qt::DescendingOrder);
  setShowGrid(false);
  verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

  setColumnWidth(0, 100);
  setColumnWidth(1, 70);
  setColumnWidth(2, 40);
  setColumnWidth(3, 40);
  setColumnWidth(4, 170);
  setColumnWidth(5, 100);
  setColumnWidth(6, 70);
  setMinimumWidth(columnWidth(0)+columnWidth(1)+columnWidth(2)+
                  columnWidth(3)+columnWidth(4)+columnWidth(5)+
                  columnWidth(6)+1);
  setMinimumHeight(480);

  connect(filter, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(scrollToTop()));
  connect(filter, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(scrollToTop()));
}


