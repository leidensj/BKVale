#include "printutils.h"
#include <QDate>
#include <QTime>
#include <QHostInfo>
#include <QImage>
#include <QPixmap>
#include "escpos.h"

namespace
{

  char decToHex[] = { '\x00', '\x01', '\x02', '\x03', '\x04', '\x05', '\x06', '\x07',
                      '\x08', '\x09', '\x0A', '\x0B', '\x0C', '\x0D', '\x0E', '\x0F',
                      '\x10', '\x11', '\x12', '\x13', '\x14', '\x15', '\x16', '\x17',
                      '\x18', '\x19', '\x1A', '\x1B', '\x1C', '\x1D', '\x1E', '\x1F',
                      '\x20', '\x21', '\x22', '\x23', '\x24', '\x25', '\x26', '\x27',
                      '\x28', '\x29', '\x2A', '\x2B', '\x2C', '\x2D', '\x2E', '\x2F',
                      '\x30', '\x31', '\x32', '\x33', '\x34', '\x35', '\x36', '\x37',
                      '\x38', '\x39', '\x3A', '\x3B', '\x3C', '\x3D', '\x3E', '\x3F',
                      '\x40', '\x41', '\x42', '\x43', '\x44', '\x45', '\x46', '\x47',
                      '\x48', '\x49', '\x4A', '\x4B', '\x4C', '\x4D', '\x4E', '\x4F',
                      '\x50', '\x51', '\x52', '\x53', '\x54', '\x55', '\x56', '\x57',
                      '\x58', '\x59', '\x5A', '\x5B', '\x5C', '\x5D', '\x5E', '\x5F',
                      '\x60', '\x61', '\x62', '\x63', '\x64', '\x65', '\x66', '\x67',
                      '\x68', '\x69', '\x6A', '\x6B', '\x6C', '\x6D', '\x6E', '\x6F',
                      '\x70', '\x71', '\x72', '\x73', '\x74', '\x75', '\x76', '\x77',
                      '\x78', '\x79', '\x7A', '\x7B', '\x7C', '\x7D', '\x7E', '\x7F',
                      '\x80', '\x81', '\x82', '\x83', '\x84', '\x85', '\x86', '\x87',
                      '\x88', '\x89', '\x8A', '\x8B', '\x8C', '\x8D', '\x8E', '\x8F',
                      '\x90', '\x91', '\x92', '\x93', '\x94', '\x95', '\x96', '\x97',
                      '\x98', '\x99', '\x9A', '\x9B', '\x9C', '\x9D', '\x9E', '\x9F',
                      '\xA0', '\xA1', '\xA2', '\xA3', '\xA4', '\xA5', '\xA6', '\xA7',
                      '\xA8', '\xA9', '\xAA', '\xAB', '\xAC', '\xAD', '\xAE', '\xAF',
                      '\xB0', '\xB1', '\xB2', '\xB3', '\xB4', '\xB5', '\xB6', '\xB7',
                      '\xB8', '\xB9', '\xBA', '\xBB', '\xBC', '\xBD', '\xBE', '\xBF',
                      '\xC0', '\xC1', '\xC2', '\xC3', '\xC4', '\xC5', '\xC6', '\xC7',
                      '\xC8', '\xC9', '\xCA', '\xCB', '\xCC', '\xCD', '\xCE', '\xCF',
                      '\xD0', '\xD1', '\xD2', '\xD3', '\xD4', '\xD5', '\xD6', '\xD7',
                      '\xD8', '\xD9', '\xDA', '\xDB', '\xDC', '\xDD', '\xDE', '\xDF',
                      '\xE0', '\xE1', '\xE2', '\xE3', '\xE4', '\xE5', '\xE6', '\xE7',
                      '\xE8', '\xE9', '\xEA', '\xEB', '\xEC', '\xED', '\xEE', '\xEF',
                      '\xF0', '\xF1', '\xF2', '\xF3', '\xF4', '\xF5', '\xF6', '\xF7',
                      '\xF8', '\xF9', '\xFA', '\xFB', '\xFC', '\xFD', '\xFE', '\xFF' };

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
               ESC_DOUBLE_FONT_ON +
               note.strNumber() + " "
               ESC_DOUBLE_FONT_OFF
               ESC_LF;


    if (0 < note.strNumber().length() && note.strNumber().length() < 256)
    {
      strNote += ESC_BARCODE_HRI_OFF
                 ESC_BARCODE_HEIGHT +
                 QString(decToHex[120]) +
                 ESC_BARCODE_CODE93 +
                 QString(decToHex[note.strNumber().length()]) +
                 note.strNumber() +
                 ESC_LF;
    }

    strNote += ESC_ALIGN_LEFT
               "Data       "
               ESC_DOUBLE_FONT_ON +
               note.strDate() +
               ESC_DOUBLE_FONT_OFF
               " " +
               note.strDayOfWeek() +
               ESC_LF
               "Fornecedor "
               ESC_DOUBLE_FONT_ON +
               (note.m_supplier.m_id.isValid()
               ? (note.m_supplier.m_alias.isEmpty()
               ? note.m_supplier.m_name
               : note.m_supplier.m_alias)
               : "Nao informado") +
               ESC_DOUBLE_FONT_OFF
               ESC_LF
               ESC_VERT_TAB;
  }

  void noteAppendFooter(const Note& note,
                        const QString& user,
                        QString& strNote)
  {
    if (note.m_disccount != 0)
    {
      strNote += ESC_EXPAND_ON ESC_LF;
      strNote +=   "Subtotal:  " + note.strSubTotal() + ESC_LF;
      if (note.m_disccount > 0)
        strNote += "Acrescimo: " + note.strDisccount();
      else if(note.m_disccount < 0)
        strNote += "Desconto:  " + note.strDisccount() ;
      strNote += ESC_EXPAND_OFF ESC_LF;
    }

    if (!note.m_observation.isEmpty())
      strNote += ESC_LF ESC_EXPAND_ON "Observacoes: " ESC_EXPAND_OFF +
                 note.m_observation + ESC_LF;
    strNote += ESC_LF
               ESC_ALIGN_CENTER
               ESC_DOUBLE_FONT_ON
               "TOTAL " +
               note.strTotal() +
               ESC_DOUBLE_FONT_OFF
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
                 (note.m_supplier.m_id.isValid()
                  ? (note.m_supplier.strAliasName())
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
        itemPt1 += " x " + note.m_vNoteItem.at(i).strPrice();
        QString itemPt2 = note.m_vNoteItem.at(i).strSubtotal();
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
  else if (!printer->waitForBytesWritten(10000))
    error = QObject::tr("Falha de timeout.");
  else
    return true;

  return false;
}

bool Printer::printString(QIODevice* printer,
                          bool bIsEthernet,
                          const QString& msg,
                          QString& error)
{
  error.clear();

  QByteArray data;
  if (bIsEthernet)
  {
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_3);
    out << quint16(0) << msg.toUtf8();
    out.device()->seek(0);
    out << quint16(data.size() - sizeof(quint16));
  }
  else
  {
    data = msg.toUtf8();
  }

  return printByteArray(printer, data, error);
}

QString Printer::strCmdInit()
{
  return ESC_CLEAR ESC_INIT ESC_CODEPAGE850;
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
           ESC_DOUBLE_FONT_OFF
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
  }

  if (!r.m_barcode.isEmpty() && 0 < r.m_barcode.length() && r.m_barcode.length() < 256)
  {
    if (!str.isEmpty())
      str += ESC_LF;
    str += r.m_bBarcodeHRI ? ESC_BARCODE_HRI_ON : ESC_BARCODE_HRI_OFF;
    str += ESC_BARCODE_HEIGHT +
           QString(decToHex[120]) +
           ESC_BARCODE_CODE93 +
           QString(decToHex[r.m_barcode.length()]) +
           r.m_barcode;
  }

  if (!str.isEmpty())
    str += ESC_LF ESC_LF ESC_FULL_CUT;

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
         ESC_DOUBLE_FONT_OFF
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

  if (lst.m_supplier.m_id.isValid())
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
        str += lst.m_supplier.m_vPhone.at(i).strFormattedPhone() + ESC_LF;
      }
    }
  }

  str += ESC_LF;

  if (lst.m_vItem.size() != 0)
    str += ESC_ALIGN_LEFT
           ESC_DOUBLE_FONT_ON
           "[ EST ][ COM ][ REC ]"
           ESC_LF;

  for (int i = 0; i != lst.m_vItem.size(); ++i)
  {
    str += ESC_DOUBLE_FONT_OFF +
           lst.m_vItem.at(i).m_product.m_name;
    if (!lst.m_supplier.m_id.isValid() && lst.m_vItem.at(i).m_supplier.isValid())
      str += ESC_LF "Fornecedor: " +
             lst.m_vItem.at(i).m_supplier.strAliasName();
    if (lst.m_vItem.at(i).m_bPrice)
      str += ESC_LF "Preco sugerido: " +
             lst.m_vItem.at(i).strPrice();
    str += ESC_LF
           ESC_DOUBLE_FONT_ON
           "[     ][     ]";
    if (lst.m_vItem.at(i).m_bAmmount)
      str += "[" + JItem::st_strFmt(lst.m_vItem.at(i).m_ammount) +
             lst.m_vItem.at(i).m_package.strUnity(lst.m_vItem.at(i).m_product.m_unity) + "]";
    str += ESC_DOUBLE_FONT_OFF;
    str += ESC_LF;
  }

  str += ESC_ALIGN_LEFT;

  if (lst.m_nLines > 0)
  {
    for (int i = 0; i != lst.m_nLines; ++i)
      str += ESC_VERT_TAB TABLE_FULL_LINE ESC_LF;
  }

  str += ESC_LF
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
         ESC_DOUBLE_FONT_OFF
         ESC_LF
         ESC_EXPAND_ON
         "DATA E HORA"
         ESC_EXPAND_OFF
         ESC_LF
         ESC_DOUBLE_FONT_ON +
         QDateTime::fromString(res.m_dateTime, Qt::ISODate).toString("dd/MM/yyyy HH:mm") +
         ESC_DOUBLE_FONT_OFF
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
         "Telefone: " +
         res.m_phone +
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

QString DiscountPrinter::build(const Discount& o)
{
  QString str;
  str +=  ESC_EXPAND_ON
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
         "CODIGO DE DESCONTO"
         ESC_EXPAND_OFF
         ESC_LF
         ESC_VERT_TAB
         ESC_BARCODE_HRI_ON
         ESC_BARCODE_HEIGHT +
         QString(decToHex[120]) +
         ESC_BARCODE_CODE93 +
         QString(decToHex[o.m_code.length()]) +
         o.m_code +
         ESC_LF
         ESC_VERT_TAB
         ESC_ALIGN_CENTER;

  if (o.m_bExpires)
    str += "Data de expiracao: " + o.strExpDate();
  else
    str += "Sem data de expiracao";

  str += ESC_LF
         ESC_VERT_TAB
         ESC_FULL_CUT;

  return str;
}

QString DiscountPrinter::buildRedeem(const Discount& o)
{
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
         ESC_DOUBLE_FONT_ON
         "DESCONTO"
         ESC_LF
         ESC_VERT_TAB;

  switch (o.m_type)
  {
    case Discount::Type::Value:
    {
      str += JItem::st_strMoney(o.m_value) +
             ESC_DOUBLE_FONT_OFF;
    } break;
    case Discount::Type::Percentage:
    {
      str += JItem::st_strPercentage(o.m_percentage) +
             ESC_DOUBLE_FONT_OFF;
    } break;
    case Discount::Type::Product:
    {
      str += ESC_DOUBLE_FONT_OFF ESC_ALIGN_LEFT;
      for (int i = 0; i != o.m_items.size(); ++i)
      {
        str += o.m_items.at(i).m_product.m_name + ESC_LF;
        QString ammount = o.m_items.at(i).strAmmount() + o.m_items.at(i).m_product.m_unity;
        QString aux;
        const int n = TABLE_WIDTH - ammount.length();
        for (int j = 0; j < n; ++j)
          aux += ".";
        str += aux + ESC_STRESS_ON + ammount + ESC_STRESS_OFF;
      }
    } break;
    case Discount::Type::None:
    default:
    {
      str += ESC_EXPAND_ON "ERRO: Tipe de desconto nao definido." ESC_EXPAND_OFF;
    } break;
  }

  str += ESC_LF
         ESC_VERT_TAB;

  if (!o.m_description.isEmpty())
  {
    str += o.m_description +
           ESC_LF
           ESC_VERT_TAB;
  }

  str += ESC_FULL_CUT;
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
