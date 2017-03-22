#include "printutils.h"
#include <QDate>
#include <QTime>

#define ESC              "\x1b"
#define ESC_ALIGN_CENTER "\x1b\x61\x31"
#define ESC_ALIGN_LEFT   "\x1b\x61\x30"
#define ESC_FULL_CUT     "\x1b\x77"
#define ESC_PARTIAL_CUT  "\x1b\x6d"
#define ESC_STRESS_ON    "\x1b\x45"
#define ESC_STRESS_OFF   "\x1b\x46"
#define ESC_LF           "\n"
#define ESC_VERT_TAB     "\x1b\x4a\x40"
#define ESC_INIT         "\x1b\x40"
#define ESC_DOUBLE_FONT  "\x1b\x0e\x1b\x56"
#define ESC_PORTUGUESE   "\x1b\x74\x08"
#define ESC_REVERSE_ON   "\x1b\x7d\x31"
#define ESC_REVERSE_OFF  "\x1b\x7d\x30"
#define ESC_EXPAND_ON    "\x1b\x57\x31"
#define ESC_EXPAND_OFF   "\x1b\x57\x30"

#define TABLE_WIDTH           48
#define TABLE_MAX_VALUE       10000

namespace
{
  void appendHeader(const Note& note, QString& strNote)
  {
    strNote += ESC_EXPAND_ON
               ESC_ALIGN_CENTER
               "BaitaKão"
               ESC_LF
               "Rua Sinimbu 175 Lourdes"
               ESC_LF
               "32221034 32281666"
               ESC_EXPAND_OFF
               ESC_LF
               ESC_VERT_TAB
               ESC_ALIGN_LEFT
               "Número do vale.."
               ESC_DOUBLE_FONT +
               QString::number(note.m_number) +
               ESC_LF
               "Data do vale...."
               ESC_DOUBLE_FONT +
               QDate::fromJulianDay(note.m_date).toString("dd/MM/yyyy") +
               ESC_LF
               "Fornecedor......"
               ESC_DOUBLE_FONT +
               note.m_supplier +
               ESC_LF +
               ESC_VERT_TAB;
  }

  void appendFooter(const Note& note, QString& strNote)
  {
    strNote += ESC_LF
               ESC_ALIGN_CENTER
               ESC_DOUBLE_FONT
               "TOTAL R$" +
               Note::format(note.m_total) +
               ESC_LF
               ESC_VERT_TAB
               "Emissão: " +
               QDate::fromJulianDay(note.m_date).toString("dd/MM/yyyy ") +
               QTime::currentTime().toString("hh:mm:ss") +
               ESC_LF
               ESC_LF
               ESC_LF
               ESC_ALIGN_CENTER
               "────────────────────────────────"
               ESC_LF
               "Assinatura"
               ESC_LF;
  }

  void appendBody(const Note& note, QString& strNote)
  {
    NoteItems items(note.m_items);
    if (items.m_size == 0)
      return;

    strNote += ESC_ALIGN_LEFT;
    for (int i = 0; i != items.m_size; ++i)
    {
      QString item;
      {
        QString itemPt1 = items.at(i, Column::Ammount) +
                          items.at(i, Column::Unity) +
                          " x R$" +
                          items.at(i, Column::UnitValue);
        QString itemPt2 = "R$" +
                          items.at(i, Column::SubTotal);
        const int n = TABLE_WIDTH - (itemPt1.length() + itemPt2.length());
        for (int j = 0; j < n; ++j)
          itemPt1 += " ";
        item = itemPt1 + ESC_STRESS_ON + itemPt2 + ESC_STRESS_OFF;
      }
      strNote += items.at(i, Column::Description) + ESC_LF +
              item + ESC_LF "────────────────────────────────────────────────" ESC_LF;
    }
  }
}

bool PrintUtils::print(QSerialPort& printer,
                       const QString& msg,
                       QString& error)
{
  error.clear();
  QByteArray data(msg.toUtf8());
  bool bSuccess = false;
  auto nBytes = printer.write(data);

  if (nBytes != data.size())
  {
    error = QObject::tr("Erro '%1' ao imprimir: %2").arg(
              QString::number(printer.error()),
              printer.errorString());
  }
  else if (!printer.waitForBytesWritten(5000))
  {
    error = QObject::tr("Falha de timeout.");
  }
  else
  {
    bSuccess = true;
  }

  return bSuccess;
}

bool PrintUtils::initPrinter(QSerialPort& printer,
                             QString& error)
{
  error.clear();
  QString msg = QString(ESC_INIT) + ESC_PORTUGUESE;
  return print(printer, msg, error);
}

QString PrintUtils::buildNote(const Note& note)
{
  QString strNote1;
  appendHeader(note, strNote1);
  appendBody(note, strNote1);
  appendFooter(note, strNote1);
  QString strNote2(strNote1);
  strNote1 += "1ª via" ESC_LF ESC_LF ESC_PARTIAL_CUT;
  strNote2 += "2ª via" ESC_LF ESC_LF ESC_FULL_CUT;
  return strNote1 + strNote2;
}



