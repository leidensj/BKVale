#include "postitdialog.h"
#include <QLayout>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QMessageBox>
#include "widgets/jlineedit.h"
#include "escposprinter.h"
#include "escpos.h"

PostItDialog::PostItDialog(QWidget* parent)
 : QDialog(parent)
 , m_title(nullptr)
 , m_subtitle(nullptr)
 , m_text(nullptr)
 , m_btnPrint(nullptr)
 , m_btnNew(nullptr)
{
  setWindowFlags(Qt::Window);
  setWindowTitle(tr("Post It"));
  setWindowIcon(QIcon(":/icons/res/postit2.png"));

  m_title = new JLineEdit(Text::Input::ASCII, false);
  m_title->setPlaceholderText(tr("Título"));
  m_subtitle = new JLineEdit(Text::Input::ASCII, false);
  m_subtitle->setPlaceholderText(tr("Subtítulo"));
  m_text = new QPlainTextEdit;
  m_text->setPlaceholderText(tr("Texto"));
  m_btnNew = new QPushButton;
  m_btnNew->setIconSize(QSize(24,24));
  m_btnNew->setIcon(QIcon(":/icons/res/file.png"));
  m_btnNew->setFlat(true);
  m_btnNew->setToolTip(tr("Novo"));
  m_btnPrint = new QPushButton;
  m_btnPrint->setIconSize(QSize(24,24));
  m_btnPrint->setIcon(QIcon(":/icons/res/printer.png"));
  m_btnPrint->setFlat(true);
  m_btnPrint->setToolTip(tr("Imprimir"));

  auto hl = new QHBoxLayout;
  hl->addWidget(m_btnNew);
  hl->addWidget(m_btnPrint);
  hl->setAlignment(Qt::AlignLeft);
  auto vl = new QVBoxLayout;
  vl->addLayout(hl);
  vl->addWidget(m_title);
  vl->addWidget(m_subtitle);
  vl->addWidget(m_text);

  connect(m_btnNew, SIGNAL(clicked(bool)), this, SLOT(create()));
  connect(m_btnPrint, SIGNAL(clicked(bool)), this, SLOT(print()));

  setLayout(vl);
}

void PostItDialog::print()
{
  EscPos ep;
  if (!m_title->text().isEmpty())
  {
    ep.align(true);
    ep.doublefont(true);
    ep.str(m_title->text() + "\n\n\n");
    ep.align(false);
    ep.doublefont(false);
  }
  if (!m_subtitle->text().isEmpty())
  {
    ep.align(true);
    ep.expand(true);
    ep.str(m_subtitle->text() + "\n\n\n");
    ep.align(false);
    ep.expand(false);
  }

  if (!m_text->toPlainText().isEmpty())
    ep.str(m_text->toPlainText() + "\n\n\n");

  ep.cut(true);
  EscPosPrinter printer;
  QString error;
  bool ok = printer.connectToPrinter(error);
  if (ok)
    ok = printer.printRawData(ep.m_ba, error);
  if (!ok)
    QMessageBox::warning(this, tr("Erro ao imprimir"), error, QMessageBox::Ok);
}

void PostItDialog::create()
{
  m_title->clear();
  m_subtitle->clear();
  m_text->setPlainText("");
  m_title->setFocus();
}
