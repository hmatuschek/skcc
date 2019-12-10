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
#include "icons.hh"


SpotWindow::SpotWindow(SpotTable *spots, QWidget *parent)
    : QTableView(parent), _spottable(spots)
{
  Settings settings;
  setWindowTitle(tr("Spot table"));
  setWindowIcon(IconProvider::get(IconProvider::WINDOW_ICON));

  QSortFilterProxyModel *filter = new QSortFilterProxyModel();
  filter->setSortRole(Qt::EditRole);
	filter->setSourceModel(_spottable);
  filter->setFilterKeyColumn(6);

	setModel(filter);
  setSortingEnabled(true);
  sortByColumn(6, Qt::DescendingOrder);
  setShowGrid(false);
  verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  verticalHeader()->hide();

  for (int i=0; i<7; i++)
    setColumnWidth(i, settings.sectionSize(i));

  setMinimumWidth(columnWidth(0)+columnWidth(1)+columnWidth(2)+
                  columnWidth(3)+columnWidth(4)+columnWidth(5)+
                  columnWidth(6)+2);
  setMinimumHeight(500);
  resize(minimumWidth(), minimumHeight());

  connect(this->horizontalHeader(), SIGNAL(sectionResized(int,int,int)),
          this, SLOT(onSectionResized(int,int,int)));
  connect(filter, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(scrollToTop()));
  connect(filter, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(scrollToTop()));
}

void
SpotWindow::onSectionResized(int logicalIndex, int oldSize, int newSize) {
  Q_UNUSED(oldSize);
  Settings settings;
  settings.setSectionSize(logicalIndex, newSize);
  qDebug() << "Set section" << logicalIndex << "to width" << newSize;
}

