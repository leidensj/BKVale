#include "printutils.h"
#include <QDate>
#include <QTime>
#include "escpos.h"

namespace
{
  void noteAppendHeader(const Note& note, QString& strNote)
  {
    strNote += ESC_EXPAND_ON
               ESC_ALIGN_CENTER
               "BAITAKÃO"
               ESC_LF
               "RUA SINIMBU 175 LOURDES"
               ESC_LF
               "32221034 32281666"
               ESC_LF
               "WWW.BAITAKAO.COM.BR"
               ESC_LF
               ESC_VERT_TAB
               ESC_EXPAND_OFF
               "ORDEM DE RECEBIMENTO"
               ESC_LF
               "DE MERCADORIA"
               ESC_LF
               ESC_VERT_TAB
               ESC_ALIGN_LEFT
               "Número     "
               ESC_DOUBLE_FONT_ON +
               QString::number(note.m_number) +
               ESC_LF
               "Data       "
               ESC_DOUBLE_FONT_ON +
               QDate::fromJulianDay(note.m_date).toString("dd/MM/yyyy") +
               ESC_DOUBLE_FONT_OFF +
               QDate::fromJulianDay(note.m_date).toString(" (dddd)") +
               ESC_LF
               "Fornecedor "
               ESC_DOUBLE_FONT_ON +
               note.m_supplier +
               ESC_LF +
               ESC_VERT_TAB;
  }

  void noteAppendFooter(const Note& note, QString& strNote)
  {
    strNote += ESC_LF
               ESC_ALIGN_CENTER
               ESC_DOUBLE_FONT_ON
               "TOTAL R$" +
               Note::format(note.m_total) +
               ESC_LF
               ESC_VERT_TAB
               "Emissão: " +
               QDate::currentDate().toString("dd/MM/yyyy ") +
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

  void noteAppendBody(const Note& note, QString& strNote)
  {
    NoteItems items(note.m_items);
    if (items.m_size == 0)
      return;

    strNote += ESC_ALIGN_LEFT;
    for (int i = 0; i != items.m_size; ++i)
    {
      QString item;
      {
        QString itemPt1 = items.at(i, NoteColumn::Ammount) +
                          items.at(i, NoteColumn::Unity) +
                          " x R$" +
                          items.at(i, NoteColumn::UnitValue);
        QString itemPt2 = "R$" +
                          items.at(i, NoteColumn::SubTotal);
        const int n = TABLE_WIDTH - (itemPt1.length() + itemPt2.length());
        for (int j = 0; j < n; ++j)
          itemPt1 += " ";
        item = itemPt1 + ESC_STRESS_ON + itemPt2 + ESC_STRESS_OFF;
      }
      strNote += items.at(i, NoteColumn::Description) + ESC_LF +
              item + ESC_LF "────────────────────────────────────────────────" ESC_LF;
    }
  }
}

bool Printer::init(QSerialPort& printer,
                   QString& error)
{
  error.clear();
  QString msg = QString(ESC_INIT) + ESC_PORTUGUESE;
  return print(printer, msg, error);
}

bool Printer::print(QSerialPort& printer,
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

void Printer::fullCut(QSerialPort& printer)
{
  QString error;
  print(printer, ESC_FULL_CUT, error);
}

QString NotePrinter::build(const Note& note)
{
  QString strNote1;
  noteAppendHeader(note, strNote1);
  noteAppendBody(note, strNote1);
  noteAppendFooter(note, strNote1);
  QString strNote2(strNote1);
  strNote1 += "1ª via" ESC_LF ESC_LF ESC_PARTIAL_CUT;
  strNote2 += "2ª via" ESC_LF ESC_LF ESC_FULL_CUT;
  return strNote1 + strNote2;
}

QString ReminderPrinter::build(const Reminder& r)
{
  QString str, title, msg;

  switch (r.m_capitalization)
  {
    case Reminder::Capitalization::Normal:
      title = r.m_title;
      msg = r.m_message;
      break;
    case Reminder::Capitalization::AllLowercase:
      title = r.m_title.toLower();
      msg = r.m_message.toLower();
      break;
    case Reminder::Capitalization::AllUppercase:
    default:
      title = r.m_title.toUpper();
      msg = r.m_message.toUpper();
      break;
  }

  if (!title.isEmpty())
  {
    str += ESC_DOUBLE_FONT_ON
           ESC_ALIGN_CENTER +
           title +
           ESC_LF
           ESC_DOUBLE_FONT_OFF;
    if (!msg.isEmpty())
    {
      str += "────────────────────────────────────────────────"
             ESC_LF;
    }
    str += ESC_ALIGN_LEFT;
  }

  if (!msg.isEmpty())
  {
    if (r.m_size == Reminder::Size::Large)
      str += ESC_EXPAND_ON;
    str += msg;
    if (r.m_size == Reminder::Size::Large)
      str += ESC_EXPAND_OFF;
    str += ESC_LF ESC_LF;
  }

  str += ESC_FULL_CUT;
  return str;
}

QString ConsumptionPrinter::build(qint64 date,
                                  const QVector<Consumption>& vConsumption,
                                  const QVector<Item>& vItem,
                                  double total)
{
  if (vConsumption.size() != vItem.size())
    return "";

  QString str;
  str += ESC_EXPAND_ON
         ESC_ALIGN_CENTER
         "BAITAKÃO"
         ESC_LF
         "RUA SINIMBU 175 LOURDES"
         ESC_LF
         "32221034 32281666"
         ESC_LF
         "WWW.BAITAKAO.COM.BR"
         ESC_LF
         ESC_VERT_TAB
         ESC_EXPAND_OFF
         "RELATÓRIO DE CONSUMO"
         ESC_LF
         ESC_VERT_TAB
         ESC_EXPAND_ON +
         QDate::fromJulianDay(date).toString("dd/MM/yyyy") +
         QDate::fromJulianDay(date).toString(" dddd") +
         ESC_LF
         ESC_EXPAND_OFF
         ESC_ALIGN_LEFT;

  for (int i = 0; i != vConsumption.size(); ++i)
  {
    QString subStr;
    {
      QString subStr1 = QString::number(vConsumption.at(i).m_ammount, 'f', 3) +
                        vItem.at(i).m_unity +
                        " x R$" +
                        QString::number(vConsumption.at(i).m_price, 'f', 2);
      QString subStr2 = "R$" + QString::number(vConsumption.at(i).m_total, 'f', 2);
      const int n = TABLE_WIDTH - (subStr1.length() + subStr2.length());
      for (int j = 0; j < n; ++j)
        subStr1 += " ";
      subStr = subStr1 + ESC_STRESS_ON + subStr2 + ESC_STRESS_OFF;
    }
    str += vItem.at(i).m_description + ESC_LF + subStr +
           ESC_LF "────────────────────────────────────────────────" ESC_LF;
  }

  str += ESC_LF
         ESC_ALIGN_CENTER
         ESC_DOUBLE_FONT_ON
         "TOTAL R$" +
         QString::number(total, 'f', 2) +
         ESC_DOUBLE_FONT_OFF
         ESC_LF
         ESC_LF
         ESC_FULL_CUT;

  return str;
}

QString ConsumptionPrinter::build(const QVector<qint64>& vDate,
                                  const QVector<double>& vSubTotal,
                                  double total)
{
  if (vDate.size() != vSubTotal.size())
    return "";

  QString str;
  str += ESC_EXPAND_ON
         ESC_ALIGN_CENTER
         "BAITAKÃO"
         ESC_LF
         "RUA SINIMBU 175 LOURDES"
         ESC_LF
         "32221034 32281666"
         ESC_LF
         "WWW.BAITAKAO.COM.BR"
         ESC_LF
         ESC_VERT_TAB
         ESC_EXPAND_OFF
         "RELATÓRIO DE CONSUMO"
         ESC_LF
         ESC_VERT_TAB
         ESC_ALIGN_LEFT;

  for (int i = 0; i != vDate.size(); ++i)
  {
    QString pt1 = QDate::fromJulianDay(vDate.at(i)).toString("dd/MM/yyyy") +
                  QDate::fromJulianDay(vDate.at(i)).toString(" (dddd)");
    QString pt2 = "R$" + QString::number(vSubTotal.at(i), 'f', 2);
    const int n = TABLE_WIDTH - (pt1.length() + pt2.length());
    for (int j = 0; j < n; ++j)
      pt1 += " ";
    str += pt1 + pt2 + ESC_LF;
  }

  str += ESC_LF
         ESC_LF
         ESC_ALIGN_CENTER
         ESC_DOUBLE_FONT_ON
         "TOTAL R$" +
         QString::number(total, 'f', 2) +
         ESC_DOUBLE_FONT_OFF
         ESC_LF
         ESC_LF
         ESC_FULL_CUT;

  return str;
}

