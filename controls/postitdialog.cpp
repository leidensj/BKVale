#include "postitdialog.h"
#include <QLayout>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QMessageBox>
#include <QCheckBox>
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
 , m_cbTitleSize(nullptr)
 , m_cbSubtitleSize(nullptr)
 , m_cbTextSize(nullptr)
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
  m_cbTitleSize = new QCheckBox;
  m_cbTitleSize->setIcon(QIcon(":/icons/res/text.png"));
  m_cbTitleSize->setTristate(true);
  m_cbTitleSize->setCheckState(Qt::Unchecked);
  m_cbSubtitleSize = new QCheckBox;
  m_cbSubtitleSize->setIcon(QIcon(":/icons/res/text.png"));
  m_cbSubtitleSize->setTristate(true);
  m_cbSubtitleSize->setChecked(Qt::Unchecked);
  m_cbTextSize = new QCheckBox;
  m_cbTextSize->setIcon(QIcon(":/icons/res/text.png"));
  m_cbTextSize->setTristate(true);
  m_cbTextSize->setChecked(Qt::Checked);

  auto hl = new QHBoxLayout;
  hl->addWidget(m_btnNew);
  hl->addWidget(m_btnPrint);
  hl->setAlignment(Qt::AlignLeft);
  auto vl = new QVBoxLayout;
  vl->addLayout(hl);
  vl->addWidget(m_cbTitleSize);
  vl->addWidget(m_title);
  vl->addWidget(m_cbSubtitleSize);
  vl->addWidget(m_subtitle);
  vl->addWidget(m_cbTextSize);
  vl->addWidget(m_text);

  connect(m_btnNew, SIGNAL(clicked(bool)), this, SLOT(create()));
  connect(m_btnPrint, SIGNAL(clicked(bool)), this, SLOT(print()));
  connect(m_cbTitleSize, SIGNAL(checkStateChanged(Qt::CheckState)), this, SLOT(updateSize(Qt::CheckState)));
  connect(m_cbSubtitleSize, SIGNAL(checkStateChanged(Qt::CheckState)), this, SLOT(updateSize(Qt::CheckState)));
  connect(m_cbTextSize, SIGNAL(checkStateChanged(Qt::CheckState)), this, SLOT(updateSize(Qt::CheckState)));

  setLayout(vl);
  m_cbTitleSize->setCheckState(Qt::Checked);
  m_cbSubtitleSize->setCheckState(Qt::PartiallyChecked);
  m_cbTextSize->setCheckState(Qt::Unchecked);
}

void PostItDialog::print()
{
  EscPos ep;
  if (!m_title->text().isEmpty())
  {
    ep.align(true);
    ep.doublefont(m_cbTitleSize->checkState() == Qt::Checked);
    if (m_cbTitleSize->checkState() != Qt::Checked)
      ep.expand(m_cbTitleSize->checkState() == Qt::PartiallyChecked);
    ep.str(m_title->text() + "\n\n\n");
    ep.align(false);
    ep.doublefont(false);
    ep.expand(false);
  }
  if (!m_subtitle->text().isEmpty())
  {
    ep.align(true);
    ep.doublefont(m_cbSubtitleSize->checkState() == Qt::Checked);
    if (m_cbSubtitleSize->checkState() != Qt::Checked)
      ep.expand(m_cbSubtitleSize->checkState() == Qt::PartiallyChecked);
    ep.str(m_subtitle->text() + "\n\n\n");
    ep.align(false);
    ep.doublefont(false);
    ep.expand(false);
  }

  bool bAlreadyCut = false;
  if (!m_text->toPlainText().isEmpty())
  {
    ep.doublefont(m_cbTextSize->checkState() == Qt::Checked);
    if (m_cbTextSize->checkState() != Qt::Checked)
      ep.expand(m_cbTextSize->checkState() == Qt::PartiallyChecked);
    QStringList lst = m_text->toPlainText().split("*CORTE*");
    for (int i = 0; i != lst.size(); ++ i)
    {
      ep.str(lst.at(i));
      ep.cut(true);
      bAlreadyCut = true;
    }
    ep.doublefont(false);
    ep.expand(false);
  }

  if (!bAlreadyCut)
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
  m_cbTitleSize->setCheckState(Qt::Checked);
  m_cbSubtitleSize->setCheckState(Qt::PartiallyChecked);
  m_cbTextSize->setCheckState(Qt::Unchecked);
}

void PostItDialog::updateSize(Qt::CheckState ck)
{
  QCheckBox* cb = qobject_cast<QCheckBox*>(sender());
  if (cb != nullptr)
  {
    QString text;
    switch (ck)
    {
      case Qt::Checked:
        text = tr("Tamanho da fonte: GRANDE");
        break;
      case Qt::PartiallyChecked:
        text = tr("Tamanho da fonte: MÉDIA");
        break;
      case Qt::Unchecked:
        text = tr("Tamanho da fonte: PEQUENA");
        break;
      default:
        break;
    }
    cb->setText(text);
  }
}
