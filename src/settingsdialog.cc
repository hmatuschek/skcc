#include "settingsdialog.hh"
#include "settings.hh"
#include <QLineEdit>
#include <QRegExpValidator>
#include <QDialogButtonBox>
#include <QIntValidator>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QFileDialog>
#include <QTabWidget>
#include <QInputDialog>
#include <QDebug>
#include <QHeaderView>
#include <QPushButton>


SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
{
	setWindowTitle(tr("Settings"));
	Settings settings;

  QVBoxLayout *layout = new QVBoxLayout();
  QTabWidget *tabs = new QTabWidget();
  layout->addWidget(tabs);

  QWidget *tab = new QWidget();
  tabs->addTab(tab, tr("Settings"));
  QVBoxLayout *tlayout = new QVBoxLayout();
  tab->setLayout(tlayout);

	_call = new QLineEdit(settings.call());
	_locator = new QLineEdit(settings.locator());
  _locator->setValidator(new QRegExpValidator(QRegExp("[A-Za-z]{2}[0-9]{2}[A-Z]{0,2}")));
	_clusterHost = new QLineEdit(settings.clusterHost());
	_clusterPort = new QLineEdit(QString::number(settings.clusterPort()));
  _clusterPort->setValidator(new QIntValidator(0, 65535));
	_logFile = new QLineEdit(settings.logFile());

  QGroupBox *box = new QGroupBox(tr("Operator"));
  QFormLayout *form = new QFormLayout();
  form->addRow(tr("Call"), _call);
  form->addRow(tr("Locator"), _locator);
  box->setLayout(form);
  tlayout->addWidget(box);

  box = new QGroupBox(tr("Cluster"));
  form = new QFormLayout();
  form->addRow(tr("Host"), _clusterHost);
  form->addRow(tr("Port"), _clusterPort);
  box->setLayout(form);
  tlayout->addWidget(box);

  box = new QGroupBox(tr("Log File"));
  QHBoxLayout *hbox = new QHBoxLayout();
  hbox->addWidget(_logFile);
  QPushButton *button = new QPushButton(tr("..."));
  button->setToolTip(tr("Select a log file."));
  box->setLayout(hbox);
  tlayout->addWidget(box);


  tab = new QWidget();
  tabs->addTab(tab, tr("Friends"));
  tlayout = new QVBoxLayout();
  tab->setLayout(tlayout);

  Friends friends = settings.friends();
  _friends = new QTableWidget(int(friends.size()), 3);
  _friends->setSelectionMode(QAbstractItemView::SingleSelection);
  _friends->setSelectionBehavior(QAbstractItemView::SelectRows);
  _friends->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Call")));
  _friends->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Name")));
  _friends->setHorizontalHeaderItem(2, new QTableWidgetItem(tr("Comment")));
  _friends->verticalHeader()->setVisible(false);

  int i=0;
  for (Friends::iterator item=friends.begin(); item != friends.end(); i++, item++) {
    qDebug() << "Show" << i << item->call() << item->name() << item->comment();
    _friends->setItem(i, 0, new QTableWidgetItem(item->call()));
    _friends->setItem(i, 1, new QTableWidgetItem(item->name()));
    _friends->setItem(i, 2, new QTableWidgetItem(item->comment()));
  }
  _friends->setSortingEnabled(true);

  tlayout->addWidget(_friends);

  hbox = new QHBoxLayout();
  QPushButton *add = new QPushButton(" + ");
  add->setToolTip(tr("Add a call to the list of friends."));
  QPushButton *rem = new QPushButton(" - ");
  rem->setToolTip(tr("Remove the selected call form the list of friends."));
  hbox->addWidget(add); hbox->addWidget(rem);
  tlayout->addLayout(hbox);
  connect(add, SIGNAL(clicked(bool)), this, SLOT(onAddFriend()));
  connect(rem, SIGNAL(clicked(bool)), this, SLOT(onRemFriend()));

  tab = new QWidget();
  tabs->addTab(tab, tr("Colors"));
  form = new QFormLayout();

  _selfSpot = new ColorButton(settings.selfSpotColor());
  _friendSpot = new ColorButton(settings.friendSpotColor());
  _newDXCC = new ColorButton(settings.newDXCCColor());
  _newBand = new ColorButton(settings.newBandColor());
  _newSlot = new ColorButton(settings.newSlotColor());
  _newQSO = new ColorButton(settings.newQSOColor());
  _worked = new ColorButton(settings.workedColor());
  form->addRow(tr("self spot"), _selfSpot);
  form->addRow(tr("friend spot"), _friendSpot);
  form->addRow(tr("new DXCC"), _newDXCC);
  form->addRow(tr("new band"), _newBand);
  form->addRow(tr("new slot"), _newSlot);
  form->addRow(tr("new QSO"), _newQSO);
  form->addRow(tr("worked"), _worked);
  tab->setLayout(form);

  QDialogButtonBox *bb = new QDialogButtonBox(QDialogButtonBox::Save|QDialogButtonBox::Cancel);
  layout->addWidget(bb);

  connect(bb, SIGNAL(accepted()), this, SLOT(accept()));
  connect(bb, SIGNAL(rejected()), this, SLOT(reject()));
  connect(button, SIGNAL(clicked(bool)), this, SLOT(onSelectLogFile()));

  setLayout(layout);
}


void
SettingsDialog::onSelectLogFile() {
  QString filename = QFileDialog::getOpenFileName(nullptr, tr("Select log file"), "", "ADIF File (*.adi,*.adif)");
  if (filename.isEmpty())
    return;
  _logFile->setText(filename);
}

void
SettingsDialog::onAddFriend() {
  QString call = QInputDialog::getText(nullptr, tr("Friend call"), tr("Enter friend call sign:"));
  if (call.isEmpty())
    return;
  _friends->insertRow(_friends->rowCount());
  _friends->setItem(_friends->rowCount()-1, 0, new QTableWidgetItem(call));
}

void
SettingsDialog::onRemFriend() {
  /*QList<QListWidgetItem *> items = _friends->selectedItems();
  foreach (QListWidgetItem *item, items) {
    _friends->removeItemWidget(item);
  }*/
  qDebug() << "Remove friend" << _friends->currentItem()->text();
  _friends->removeRow(_friends->currentRow());
}

void
SettingsDialog::accept() {
  QDialog::accept();

  Settings settings;
  settings.setCall(_call->text().simplified());
  settings.setLocator(_locator->text().simplified());
  settings.setClusterHost(_clusterHost->text().simplified());
  settings.setClusterPort(quint16(_clusterPort->text().toUInt()));
  settings.setLogFile(_logFile->text());
  settings.setSelfSpotColor(_selfSpot->color());
  settings.setFriendSpotColor(_friendSpot->color());
  settings.setNewDXCCColor(_newDXCC->color());
  settings.setNewBandColor(_newBand->color());
  settings.setNewSlotColor(_newSlot->color());
  settings.setNewQSOColor(_newQSO->color());
  settings.setWorkedColor(_worked->color());

  Friends friends;
  for (int i=0; i<_friends->rowCount(); i++) {
    QString call, name, comment;
    if (QTableWidgetItem *item = _friends->item(i, 0))
      call = item->text();
    if (QTableWidgetItem *item = _friends->item(i, 1))
      name = item->text();
    if (QTableWidgetItem *item = _friends->item(i, 2))
      comment = item->text();
    if (! call.isEmpty())
      friends.addFriend(call, name, comment);
  }
  settings.setFriends(friends);
}
