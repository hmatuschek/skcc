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
  hbox = new QHBoxLayout();

  form = new QFormLayout();
  _selfSpot = new ColorButton(settings.selfSpotColor());
  _friendSpot = new ColorButton(settings.friendSpotColor());
  _beaconSpot = new ColorButton(settings.beaconSpotColor());
  _newDXCC = new ColorButton(settings.newDXCCColor());
  _newBand = new ColorButton(settings.newBandColor());
  _newSlot = new ColorButton(settings.newSlotColor());
  _newQSO = new ColorButton(settings.newQSOColor());
  _worked = new ColorButton(settings.workedColor());
  form->addRow(tr("self spot"), _selfSpot);
  form->addRow(tr("friend spot"), _friendSpot);
  form->addRow(tr("beacon spot"), _beaconSpot);
  form->addRow(tr("new DXCC"), _newDXCC);
  form->addRow(tr("new band"), _newBand);
  form->addRow(tr("new slot"), _newSlot);
  form->addRow(tr("new QSO"), _newQSO);
  form->addRow(tr("worked"), _worked);
  hbox->addLayout(form);

  form = new QFormLayout();
  _qthColor  = new ColorButton(settings.qthColor());
  _spot2200m = new ColorButton(settings.spot2200mColor());
  _spot630m  = new ColorButton(settings.spot630mColor());
  _spot160m  = new ColorButton(settings.spot160mColor());
  _spot80m   = new ColorButton(settings.spot80mColor());
  _spot60m   = new ColorButton(settings.spot60mColor());
  _spot40m   = new ColorButton(settings.spot40mColor());
  form->addRow(tr("QTH"),   _qthColor);
  form->addRow(tr("2200m"), _spot2200m);
  form->addRow(tr("630m"),  _spot630m);
  form->addRow(tr("160m"),  _spot160m);
  form->addRow(tr("80m"),   _spot80m);
  form->addRow(tr("60m"),   _spot60m);
  form->addRow(tr("40m"),   _spot40m);
  hbox->addLayout(form);

  form = new QFormLayout();
  _spot30m = new ColorButton(settings.spot30mColor());
  _spot20m = new ColorButton(settings.spot20mColor());
  _spot17m = new ColorButton(settings.spot17mColor());
  _spot15m = new ColorButton(settings.spot15mColor());
  _spot12m = new ColorButton(settings.spot12mColor());
  _spot10m = new ColorButton(settings.spot10mColor());
  _spot6m  = new ColorButton(settings.spot6mColor());
  form->addRow(tr("30m"), _spot30m);
  form->addRow(tr("20m"), _spot20m);
  form->addRow(tr("17m"), _spot17m);
  form->addRow(tr("15m"), _spot15m);
  form->addRow(tr("12m"), _spot12m);
  form->addRow(tr("10m"), _spot10m);
  form->addRow(tr("6m"),  _spot6m);
  hbox->addLayout(form);

  tab->setLayout(hbox);

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
  settings.setBeaconSpotColor(_beaconSpot->color());
  settings.setNewDXCCColor(_newDXCC->color());
  settings.setNewBandColor(_newBand->color());
  settings.setNewSlotColor(_newSlot->color());
  settings.setNewQSOColor(_newQSO->color());
  settings.setWorkedColor(_worked->color());
  settings.setQTHColor(_qthColor->color());
  settings.setSpot2200mColor(_spot2200m->color());
  settings.setSpot630mColor(_spot630m->color());
  settings.setSpot160mColor(_spot160m->color());
  settings.setSpot80mColor(_spot80m->color());
  settings.setSpot60mColor(_spot60m->color());
  settings.setSpot40mColor(_spot40m->color());
  settings.setSpot30mColor(_spot30m->color());
  settings.setSpot20mColor(_spot20m->color());
  settings.setSpot17mColor(_spot17m->color());
  settings.setSpot15mColor(_spot15m->color());
  settings.setSpot12mColor(_spot12m->color());
  settings.setSpot10mColor(_spot10m->color());
  settings.setSpot6mColor(_spot6m->color());

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
