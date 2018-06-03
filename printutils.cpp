#include "printutils.h"
#include <QDate>
#include <QTime>
#include <QHostInfo>
#include <QImage>
#include <QPixmap>
#include "escpos.h"

namespace
{
  void noteAppendHeader(const Note& note,
                        QString& strNote)
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
               note.strNumber() +
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
               (note.m_supplier.isValidId()
               ? (note.m_supplier.m_alias.isEmpty()
               ? note.m_supplier.m_name
               : note.m_supplier.m_alias)
               : "Nao informado") +
               ESC_LF +
               ESC_VERT_TAB;
  }

  void noteAppendFooter(const Note& note,
                        const QString& user,
                        QString& strNote)
  {
    if (note.m_disccount != 0)
    {
      strNote += ESC_EXPAND_ON ESC_ALIGN_CENTER ESC_LF;
      strNote += "Subtotal: R$" + note.strSubTotal() + ESC_LF;
      if (note.m_disccount > 0)
        strNote += "Acrescimo: R$" + note.strDisccount();
      else if(note.m_disccount < 0)
        strNote += "Desconto: R$" + note.strDisccount() ;
      strNote += ESC_EXPAND_OFF ESC_ALIGN_LEFT ESC_LF;
    }

    if (!note.m_observation.isEmpty())
      strNote += ESC_LF ESC_EXPAND_ON "Observacoes: " ESC_EXPAND_OFF +
                 note.m_observation + ESC_LF;
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
                 (note.m_supplier.isValidId()
                  ? (note.m_supplier.m_alias.isEmpty()
                     ? note.m_supplier.m_name
                     : note.m_supplier.m_alias)
                  : "fornecedor") +
                 ESC_LF;
    }
  }

  void noteAppendBody(const Note& note,
                      QString& strNote)
  {
    strNote += ESC_ALIGN_LEFT;
    for (int i = 0; i != note.m_vNoteItem.size(); ++i)
    {
      QString item;
      {
        QString itemPt1 = note.m_vNoteItem.at(i).strAmmount();
        itemPt1 += note.m_vNoteItem.at(i).m_package.strUnity(note.m_vNoteItem.at(i).m_product.m_unity);
        itemPt1 += " x R$" + note.m_vNoteItem.at(i).strPrice();
        QString itemPt2 = "R$" + note.m_vNoteItem.at(i).strSubtotal();
        const int n = TABLE_WIDTH - (itemPt1.length() + itemPt2.length());
        for (int j = 0; j < n; ++j)
          itemPt1 += ".";
        item = itemPt1 + ESC_STRESS_ON + itemPt2 + ESC_STRESS_OFF;
      }
      strNote += note.m_vNoteItem.at(i).m_product.m_name + ESC_LF + item + ESC_LF;
    }
  }
}

bool Printer::printByteArray(QIODevice* printer,
                             QByteArray& data,
                             QString& error)
{
  error.clear();

  if (printer == nullptr)
  {
    error = "Erro interno. Contacte o administrador.";
    return false;
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

bool Printer::printString(QIODevice* printer,
                          InterfaceType type,
                          const QString& msg,
                          QString& error)
{
  error.clear();

  QByteArray data;
  if (type == InterfaceType::Serial)
  {
    data = msg.toUtf8();
  }
  else if (type == InterfaceType::Ethernet)
  {
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_3);
    out << quint16(0) << msg.toUtf8();
    out.device()->seek(0);
    out << quint16(data.size() - sizeof(quint16));
  }

  return printByteArray(printer, data, error);
}

QString Printer::strCmdInit()
{
  return ESC_INIT ESC_CODEPAGE850;
}

QString Printer::strCmdFullCut()
{
  return ESC_FULL_CUT;
}

QString NotePrinter::build(const Note& note,
                           const QString& user)
{
  QString strNote1;
  noteAppendHeader(note, strNote1);
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
    str += ESC_ALIGN_CENTER
           ESC_DOUBLE_FONT_ON +
           title +
           ESC_LF;
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
                                  const QVector<Product>& vProduct,
                                  double total)
{
  if (vConsumption.size() != vProduct.size())
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
                        vProduct.at(i).m_unity +
                        " x R$" +
                        QString::number(vConsumption.at(i).m_price, 'f', 2);
      QString subStr2 = "R$" + QString::number(vConsumption.at(i).m_total, 'f', 2);
      const int n = TABLE_WIDTH - (subStr1.length() + subStr2.length());
      for (int j = 0; j < n; ++j)
        subStr1 += " ";
      subStr = subStr1 + ESC_STRESS_ON + subStr2 + ESC_STRESS_OFF;
    }
    str += vProduct.at(i).m_name + ESC_LF + subStr +
           ESC_LF "________________________________________________" ESC_LF;
  }

  str += ESC_LF
         ESC_ALIGN_CENTER
         ESC_DOUBLE_FONT_ON
         "TOTAL R$" +
         QString::number(total, 'f', 2) +
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
         ESC_LF
         ESC_LF
         ESC_FULL_CUT;

  return str;
}

QString ShoppingListPrinter::build(const ShoppingList& lst)
{
  QString str;
  str += ESC_EXPAND_ON
         ESC_ALIGN_CENTER
         "LISTA DE COMPRAS"
         ESC_EXPAND_OFF
         ESC_LF
         ESC_DOUBLE_FONT_ON +
         lst.m_title +
         ESC_LF
         ESC_EXPAND_ON +
         QDate::currentDate().toString("dd/MM/yyyy") +
         ESC_EXPAND_OFF
         ESC_LF +
         QDate::currentDate().toString("dddd") +
         ESC_LF
         ESC_ALIGN_LEFT;

  if (!lst.m_description.isEmpty())
    str += ESC_VERT_TAB
           ESC_EXPAND_ON
           "Descricao: "
           ESC_EXPAND_OFF +
           lst.m_description +
           ESC_LF;

  if (lst.m_supplier.isValidId())
  {
    str += ESC_VERT_TAB
           ESC_ALIGN_CENTER
           "Fornecedor: "
           ESC_EXPAND_ON +
           lst.m_supplier.strAliasName() +
           ESC_EXPAND_OFF
           ESC_ALIGN_LEFT
           ESC_LF;

    for (int i = 0; i != lst.m_supplier.m_vPhone.size(); ++i)
    {
      if (!lst.m_supplier.m_vPhone.at(i).m_number.isEmpty())
      {
        if (!lst.m_supplier.m_vPhone.at(i).m_name.isEmpty())
          str += lst.m_supplier.m_vPhone.at(i).m_name + " ";
        str += lst.m_supplier.m_vPhone.at(i).getFormattedPhone() + ESC_LF;
      }
    }

    if (lst.m_bCallSupplier)
      str += "Ligar para o fornecedor" ESC_LF;

    if (lst.m_bSupplierCalls)
      str += "Fornecedor liga" ESC_LF;

    if (lst.m_bWhatsapp)
      str += "Whatsapp" ESC_LF;

    if (lst.m_bVisit)
      str += "Visita presencial" ESC_LF;
  }

  str += ESC_LF;

  if (lst.m_vItem.size() != 0)
    str += ESC_EXPAND_ON
           "| EST | COM | REC "
           ESC_LF;

  for (int i = 0; i != lst.m_vItem.size(); ++i)
  {
    str += ESC_EXPAND_OFF
           ESC_ALIGN_LEFT +
           lst.m_vItem.at(i).m_product.m_name +
           ESC_LF
           ESC_EXPAND_ON
           "|     |     | ";
    if (lst.m_bPrintAmmount)
      str += lst.m_vItem.at(i).strAmmount() +
             lst.m_vItem.at(i).m_package.strUnity(lst.m_vItem.at(i).m_product.m_unity);
    str += ESC_LF
           "|     |     | ";
    if (lst.m_bPrintPrice)
      str += "R$ " + lst.m_vItem.at(i).strPrice();

    str += ESC_EXPAND_OFF
           ESC_LF;
  }

  str += ESC_ALIGN_LEFT
         ESC_LF
         ESC_VERT_TAB
         ESC_FULL_CUT;

  return str;
}

QString ReservationPrinter::build(const Reservation& res)
{
  QString str;
  str += ESC_EXPAND_ON
         ESC_ALIGN_CENTER
         "RESERVA"
         ESC_EXPAND_OFF
         ESC_LF
         ESC_DOUBLE_FONT_ON +
         QString::number(res.m_number) +
         ESC_LF
         ESC_EXPAND_ON
         "DATA E HORA"
         ESC_EXPAND_OFF
         ESC_LF
         ESC_DOUBLE_FONT_ON +
         QDateTime::fromString(res.m_dateTime, Qt::ISODate).toString("dd/MM/yyyy HH:mm") +
         ESC_LF
         ESC_VERT_TAB
         ESC_ALIGN_LEFT
         "Nome: "
         ESC_EXPAND_ON +
         res.m_name +
         ESC_EXPAND_OFF
         ESC_LF
         "Local: "
         ESC_EXPAND_ON +
         res.m_location +
         ESC_EXPAND_OFF
         ESC_LF
         "Quantidade: "
         ESC_EXPAND_ON +
         QString::number(res.m_ammount) +
         " pessoas" +
         ESC_EXPAND_OFF
         ESC_LF
         "Observacoes: " +
         res.m_observation +
         ESC_LF
         ESC_VERT_TAB
         ESC_FULL_CUT;

  return str;
}

// C++
#include <iostream>
#include <iomanip>
#include <vector>

// Qt
#include <QCoreApplication>
#include <QImage>

using namespace std;

class ESCPOSImage {
private:
    int m_width;
    int m_height;
    int m_gs_x;
    int m_gs_y;
    int m_gs_k;
    // Actual bytes for image - could have used QByteArray for this, I guess.
    vector <unsigned char> m_bytes;

    // This turns on a pixel a position x, y as you would expect.
    // The bytes in the bitmap is stored in a weird "y first", "x second" order.
    // The bit fiddling here takes care of it.
    void setPixel( int x, int y ) {
        size_t byte_index = x * m_gs_y + y / 8;
        int bit_no = y % 8;
        // Swap msb/lsb order. This probably only works on machines with "normal" endianess.....
        unsigned char bit = 1 << ( 7 - bit_no );
        m_bytes.at( byte_index ) = m_bytes.at( byte_index ) | bit;
    }

public:
    ESCPOSImage( const QImage & image ) {
        // Set up x and y as pr. epson manual
        m_width = image.width();
        m_height = image.height();

        m_gs_x = m_width / 8 + (m_width % 8 != 0?1:0);
        m_gs_y = m_height / 8 + (m_height % 8 != 0?1:0);

         if ( m_gs_x > 255 || m_gs_y > 255 ) {
            // You may want to write an error message here
            throw "Too large on dimension";
        }

        m_gs_k = m_gs_x * m_gs_y * 8;
        // Bit unsure about this limit. It depends on the actual printer....
        if ( m_gs_k > (3072*8) ) {
            // You may want to write an error message here
            throw "Too large on area";
        }

        vector<unsigned char> bytes( m_gs_k, 0 ); // Blank all bytes.
        m_bytes = bytes;

        // Iterate over the image, turn on any pixels that are set in the monochromo image.
        for ( int i_y = 0; i_y < m_height; ++i_y ) {
            for ( int i_x = 0; i_x < m_width; ++i_x ) {
                if ( image.pixelIndex( i_x, i_y ) == Qt::color1 ) {
                    setPixel( i_x, i_y );
                }
            }
        }
    }

    // Access internal representation. Should be const something, I guess.
    vector<unsigned char> & getBytes() {
        return m_bytes;
    }

    // Bytes suitable to send to the printer to define the bitmap.
    QByteArray getGSStar() {
        QByteArray res( m_bytes.size() + 4, 0 );
        res[0] = 29;
        res[1] = '*';
        res[2] = (unsigned char) m_gs_x;
        res[3] = (unsigned char) m_gs_y;
        for ( size_t i = 0; i < m_bytes.size(); ++i ) {
            res[ 4 + i ] = m_bytes.at( i );
        }
        return res;
    }

};

QByteArray Printer::imageToPrintVersion(const QByteArray& arImage)
{
  QByteArray ar;
  QPixmap pxImage;
  pxImage.loadFromData(arImage);
  pxImage = pxImage.scaled(96, 96);
  QImage monoImg = pxImage.toImage().convertToFormat(QImage::Format_Mono,
                                                     Qt::MonoOnly);
  if (!monoImg.isNull())
  {
    ESCPOSImage posImage(monoImg);
    ar = posImage.getGSStar();
    QString str("\x1D\x2F\x38" ESC_FULL_CUT);
    ar.append(str.toUtf8());
  }

  return ar;
}
