#include "printutils.h"
#include <QDate>
#include <QTime>
#include <QHostInfo>
#include "escpos.h"

namespace
{
  void noteAppendHeader(const Note& note, int number, QString& strNote)
  {
    strNote += ESC_EXPAND_ON
               ESC_ALIGN_CENTER
               "BAITAKAO"
               ESC_LF
               "RUA SINIMBU 175 LOURDES"
               ESC_LF
               "32221034 32281666"
               ESC_LF
               "WWW.BAITAKAO.COM.BR"
               ESC_LF
               ESC_VERT_TAB
               ESC_EXPAND_OFF;

    strNote += note.m_bCash
               ? "PAGAMENTO A VISTA"
               : "ORDEM DE RECEBIMENTO" ESC_LF "DE MERCADORIA";

    strNote += ESC_LF
               ESC_VERT_TAB
               ESC_ALIGN_LEFT
               "Numero     "
               ESC_DOUBLE_FONT_ON +
               Note::strNumber(number) +
               ESC_LF
               "Data       "
               ESC_DOUBLE_FONT_ON +
               note.strDate() +
               ESC_LF
               "           " +
               note.strDayOfWeek() +
               ESC_LF
               "Fornecedor "
               ESC_DOUBLE_FONT_ON +
               note.m_supplier +
               ESC_LF +
               ESC_VERT_TAB;
  }

  void noteAppendFooter(const Note& note,
                        const QString& user,
                        QString& strNote)
  {
    strNote += ESC_LF
               ESC_ALIGN_CENTER
               ESC_DOUBLE_FONT_ON
               "TOTAL R$" +
               note.strTotal() +
               ESC_LF
               "Emissao: " +
               QDate::currentDate().toString("dd/MM/yyyy ") +
               QTime::currentTime().toString("hh:mm:ss") +
               " @ " +
               QHostInfo::localHostName().toUpper() +
               ESC_LF
               ESC_LF
               ESC_LF
               ESC_ALIGN_CENTER
               "________________________________"
               ESC_LF
               "Assinatura " +
               user +
               ESC_LF;

    if (note.m_bCash)
    {
      strNote += ESC_LF
                 ESC_LF
                 "________________________________"
                 ESC_LF
                 "Assinatura " +
                 note.m_supplier +
                 ESC_LF;
    }
  }

  void noteAppendBody(const Note& note, QString& strNote)
  {
    if (note.m_items.size() == 0)
      return;

    strNote += ESC_ALIGN_LEFT;
    for (int i = 0; i != note.m_items.size(); ++i)
    {
      QString item;
      {
        QString itemPt1 = note.m_items.at(i).strAmmount() +
                          note.m_items.at(i).m_unity +
                          " x R$" +
                          note.m_items.at(i).strPrice();
        QString itemPt2 = "R$" + note.m_items.at(i).strSubtotal();
        const int n = TABLE_WIDTH - (itemPt1.length() + itemPt2.length());
        for (int j = 0; j < n; ++j)
          itemPt1 += ".";
        item = itemPt1 + ESC_STRESS_ON + itemPt2 + ESC_STRESS_OFF;
      }
      strNote += note.m_items.at(i).m_description + ESC_LF + item + ESC_LF;
    }
  }
}

bool Printer::print(QIODevice* printer,
                    InterfaceType type,
                    const QString& msg,
                    QString& error)
{
  error.clear();

  if (printer == nullptr)
  {
    error = "Erro interno. Contacte o administrador.";
    return false;
  }
  QByteArray data;
  QDataStream out(&data, QIODevice::WriteOnly);
  out.setVersion(QDataStream::Qt_4_3);
  if (type == InterfaceType::Serial)
  {
    out << msg.toUtf8();
  }
  else if (type == InterfaceType::Ethernet)
  {
    out << quint16(0) << msg.toUtf8();
    out.device()->seek(0);
    out << quint16(data.size() - sizeof(quint16));
  }
  auto nBytes = printer->write(data);
  if (nBytes != data.size())
  {
    error = QObject::tr("Erro ao imprimir:\n'%1'.").arg(printer->errorString());
  }
  else if (!printer->waitForBytesWritten(30000))
    error = QObject::tr("Falha de timeout.");
  else
    return true;

  return false;
}

QString Printer::strCmdInit()
{
  return ESC_INIT;
}

QString Printer::strCmdFullCut()
{
  return ESC_FULL_CUT;
}

QString NotePrinter::build(const Note& note,
                           int number,
                           const QString& user)
{
  QString strNote1;
  noteAppendHeader(note, number, strNote1);
  noteAppendBody(note, strNote1);
  noteAppendFooter(note, user, strNote1);
  if (note.m_bCash)
  {
    return strNote1 + ESC_LF ESC_FULL_CUT;
  }
  else
  {
    QString strNote2(strNote1);
    strNote1 += "1 via" ESC_LF ESC_LF ESC_PARTIAL_CUT;
    strNote2 += "2 via" ESC_LF ESC_LF ESC_FULL_CUT;
    return strNote1 + strNote2;
  }
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
      str += "________________________________________________"
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
         "BAITAKAO"
         ESC_LF
         "RUA SINIMBU 175 LOURDES"
         ESC_LF
         "32221034 32281666"
         ESC_LF
         "WWW.BAITAKAO.COM.BR"
         ESC_LF
         ESC_VERT_TAB
         ESC_EXPAND_OFF
         "RELATORIO DE CONSUMO"
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
           ESC_LF "________________________________________________" ESC_LF;
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
         "BAITAKAO"
         ESC_LF
         "RUA SINIMBU 175 LOURDES"
         ESC_LF
         "32221034 32281666"
         ESC_LF
         "WWW.BAITAKAO.COM.BR"
         ESC_LF
         ESC_VERT_TAB
         ESC_EXPAND_OFF
         "RELATORIO DE CONSUMO"
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

