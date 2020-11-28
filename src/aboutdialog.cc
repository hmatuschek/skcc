#include "aboutdialog.hh"
#include <QFile>
#include <QLabel>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include "config.h"

AboutDialog::AboutDialog()
{
  QFile about("://text/about.html");
  if (! about.open(QIODevice::ReadOnly))
    return;

  QLabel *txt = new QLabel(this);
  QString html = about.readAll();
  txt->setText(html.arg(VERSION_STRING));
  txt->setWordWrap(true);
  txt->setTextInteractionFlags(Qt::TextBrowserInteraction);

  QDialogButtonBox *bb = new QDialogButtonBox(QDialogButtonBox::Close);
  QBoxLayout *layout = new QVBoxLayout();
  layout->addWidget(txt);
  layout->addWidget(bb);
  this->setLayout(layout);

  connect(bb, SIGNAL(rejected()), this, SLOT(reject()));
  connect(bb, SIGNAL(accepted()), this, SLOT(accept()));
}
