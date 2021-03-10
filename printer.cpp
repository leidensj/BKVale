#include "printer.h"
#include <QDate>
#include <QTime>
#include <QHostInfo>
#include <QImage>
#include <QObject>
#include <QPixmap>
#include "escpos.h"
#include "settings.h"

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

  void appendFormInfo(const Form& o, QString& str)
  {
    if (!o.m_alias.isEmpty())
    {
      str += ESC_EXPAND_ON
             ESC_ALIGN_CENTER +
             o.m_alias +
             ESC_LF
             ESC_EXPAND_OFF;
    }
    str += ESC_ALIGN_CENTER +
           o.m_name +
           ESC_LF;
    if (!o.m_vAddress.isEmpty())
    {
      str += o.m_vAddress.at(0).name() +
             ESC_LF;
    }
    if (!o.m_vPhone.isEmpty())
    {
      str += o.m_vPhone.at(0).name() +
             ESC_LF;
    }
  }

  void purchaseAppendHeader(const Purchase& o, QString& text)
  {
    appendFormInfo(o.m_store.m_form, text);
    text += ESC_VERT_TAB;
    text += o.m_paymentMethod == Purchase::PaymentMethod::Cash
            ? "PAGAMENTO A VISTA"
            : "ORDEM DE RECEBIMENTO" ESC_LF "DE MERCADORIA";
    text += ESC_LF
            ESC_VERT_TAB
            ESC_DOUBLE_FONT_ON +
            o.strNumber() + " "
            ESC_DOUBLE_FONT_OFF
            ESC_LF;


    if (0 < o.strNumber().length() && o.strNumber().length() < 256)
    {
      text += ESC_BARCODE_HRI_OFF
                 ESC_BARCODE_HEIGHT +
                 QString(decToHex[120]) +
                 ESC_BARCODE_CODE93 +
                 QString(decToHex[o.strNumber().length()]) +
                 o.strNumber() +
                 ESC_LF;
    }

    text += ESC_ALIGN_LEFT
               "Data       "
               ESC_DOUBLE_FONT_ON +
               o.strDate() +
               ESC_DOUBLE_FONT_OFF
               " " +
               o.strDayOfWeek() +
               ESC_LF
               "Fornecedor "
               ESC_DOUBLE_FONT_ON +
               (o.m_supplier.m_id.isValid()
               ? (o.m_supplier.m_form.strAliasName())
               : "Nao informado") +
               ESC_DOUBLE_FONT_OFF
               ESC_LF
               ESC_VERT_TAB;
  }

  void purchaseAppendFooter(const Purchase& o, QString& text)
  {
    if (o.m_disccount != 0)
    {
      text += ESC_EXPAND_ON ESC_LF;
      text +=   "Subtotal:  " + o.strSubTotal() + ESC_LF;
      if (o.m_disccount > 0)
        text += "Acrescimo: " + o.strDisccount();
      else if(o.m_disccount < 0)
        text += "Desconto:  " + o.strDisccount() ;
      text += ESC_EXPAND_OFF ESC_LF;
    }

    QDateTime dt = DateTime::server();

    if (!o.m_observation.isEmpty())
      text += ESC_LF ESC_EXPAND_ON "Observacoes: " ESC_EXPAND_OFF + o.m_observation + ESC_LF;
    text += ESC_LF
               ESC_ALIGN_CENTER
               ESC_DOUBLE_FONT_ON
               "TOTAL " +
               o.strTotal() +
               ESC_DOUBLE_FONT_OFF
               ESC_LF
               "Emissao: " +
               dt.date().toString("dd/MM/yyyy ") +
               dt.time().toString("hh:mm:ss") +
               " @ " +
               QHostInfo::localHostName().toUpper() +
               ESC_LF
               ESC_LF
               ESC_LF
               ESC_ALIGN_CENTER
               "________________________________"
               ESC_LF
               "Assinatura " +
               o.m_employee.m_form.strAliasName() +
               ESC_LF;

    if (o.m_paymentMethod == Purchase::PaymentMethod::Cash)
    {
      text += ESC_LF ESC_LF "________________________________" ESC_LF
              "Assinatura " +
              (o.m_supplier.m_id.isValid()
               ? (o.m_supplier.m_form.strAliasName())
               : "fornecedor") +
              ESC_LF;
    }
  }

  void purchaseAppendBody(const Purchase& o, QString& text)
  {
    text += ESC_ALIGN_LEFT;
    for (int i = 0; i != o.m_vElement.size(); ++i)
    {
      QString item;
      {
        QString itemPt1 = o.m_vElement.at(i).strAmmount();
        itemPt1 += o.m_vElement.at(i).m_package.strUnity(o.m_vElement.at(i).m_product.m_unity);
        itemPt1 += " x " + o.m_vElement.at(i).strPrice();
        QString itemPt2 = o.m_vElement.at(i).strSubtotal();
        const int n = TABLE_WIDTH - (itemPt1.length() + itemPt2.length());
        for (int j = 0; j < n; ++j)
          itemPt1 += ".";
        item = itemPt1 + ESC_STRESS_ON + itemPt2 + ESC_STRESS_OFF;
      }
      text += o.m_vElement.at(i).m_product.m_name + ESC_LF + item + ESC_LF;
    }
  }
}

QString Printer::st_strFullCut()
{
  return ESC_FULL_CUT;
}

void Printer::disconnect()
{
  if (m_printerSerial.isOpen())
      m_printerSerial.close();
  if (m_printerTCP.isOpen())
    m_printerTCP.close();
}

bool Printer::connect(QString& error)
{
  if (m_printerSerial.isOpen())
    m_printerSerial.close();
  if (m_printerTCP.isOpen())
    m_printerTCP.close();

  Settings s;
  s.load();
  if (s.m_serialPort.isEmpty() &&
      s.m_ethernetIP.isEmpty())
  {
    error = QObject::tr("É necessário selecionar uma porta serial ou endereço para se conectar à impressora.");
    return false;
  }

  bool ok = true;
  if (s.m_bIsPrinterEthernet)
  {
    m_printerTCP.connectToHost(s.m_ethernetIP, (quint16)s.m_ethernetPort);
    ok = m_printerTCP.waitForConnected();
    if (ok)
      ok = printString(ESC_INIT, error);
    else
      error = m_printerTCP.errorString();
  }
  else
  {
    m_printerSerial.setPortName(s.m_serialPort);
    ok = m_printerSerial.open(QIODevice::ReadWrite);
    if (ok)
    {
      m_printerSerial.clear();
      ok = printString(ESC_INIT, error);
    }
    else
      error = m_printerSerial.errorString();
  }
  return ok;
}

bool Printer::printByteArray(QByteArray& data, QString& error)
{
  error.clear();
  Settings s;
  s.load();
  QIODevice* printer = s.m_bIsPrinterEthernet ? (QIODevice*)&m_printerTCP : (QIODevice*)&m_printerSerial;
  auto nBytes = printer->write(data);
  if (nBytes != data.size())
    error = QObject::tr("Erro ao imprimir:\n'%1'.").arg(printer->errorString());
  else if (!printer->waitForBytesWritten(10000))
    error = QObject::tr("Falha de timeout.");
  else
    return true;
  return false;
}

bool Printer::printString(const QString& msg, QString& error)
{
  error.clear();
  Settings s;
  s.load();
  QByteArray data;
  if (s.m_bIsPrinterEthernet)
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
  return printByteArray(data, error);
}

bool Printer::print(const QString& msg, QString& error)
{
  bool ok = connect(error);
  if (ok)
    ok = printString(msg, error);
  disconnect();
  return ok;
}

bool Printer::print(const Purchase& o, QString& error)
{
  QString str;
  purchaseAppendHeader(o, str);
  purchaseAppendBody(o, str);
  purchaseAppendFooter(o, str);
  if (o.m_paymentMethod == Purchase::PaymentMethod::Cash)
  {
    str += ESC_LF ESC_FULL_CUT;
  }
  else
  {
    QString strAux(str);
    str += "1 via" ESC_LF ESC_LF ESC_PARTIAL_CUT;
    strAux += "2 via" ESC_LF ESC_LF ESC_FULL_CUT;
    str += strAux;
  }
  return print(str, error);
}

bool Printer::print(const Reminder& r, QString& error)
{
  QString str, title, subject, msg;

  switch (r.m_capitalization)
  {
    case Reminder::Capitalization::Normal:
      title = r.m_title;
      msg = r.m_message;
      subject = r.m_subject;
      break;
    case Reminder::Capitalization::AllLowercase:
      title = r.m_title.toLower();
      msg = r.m_message.toLower();
      subject = r.m_subject.toLower();
      break;
    case Reminder::Capitalization::AllUppercase:
    default:
      title = r.m_title.toUpper();
      msg = r.m_message.toUpper();
      subject = r.m_subject.toUpper();
      break;
  }

  if (!title.isEmpty())
  {
    str += ESC_ALIGN_CENTER
           ESC_DOUBLE_FONT_ON +
           title +
           ESC_DOUBLE_FONT_OFF
           ESC_LF;
    if (!msg.isEmpty() || !subject.isEmpty() || r.m_bDate || r.m_bTime)
    {
      str += "________________________________________________"
             ESC_LF;
    }
    str += ESC_ALIGN_LEFT;
  }

  if (!subject.isEmpty())
  {
    str += ESC_ALIGN_CENTER
           ESC_EXPAND_ON +
           subject +
           ESC_EXPAND_OFF
           ESC_LF;
    if (!msg.isEmpty() || r.m_bDate || r.m_bTime )
    {
      str += "________________________________________________"
             ESC_LF;
    }
    str += ESC_ALIGN_LEFT;
  }

  if (r.m_bDate || r.m_bTime)
  {
    QString date = r.m_bDate ? r.m_date.toString("dd/MM/yyyy") : "";
    QString time = r.m_bTime ? r.m_time.toString("HH:mm:ss") : "";
    str += ESC_ALIGN_CENTER
           ESC_STRESS_ON +
           date + " " + time +
           ESC_STRESS_OFF
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
    str += ESC_ALIGN_LEFT;
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

  return print(str, error);
}

bool Printer::print(const ShoppingList& lst,  bool bPrintCount, QString& error)
{
  QDateTime dt = DateTime::server();
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
         dt.date().toString("dd/MM/yyyy") +
         ESC_EXPAND_OFF
         ESC_LF +
         dt.date().toString("dddd") +
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
           lst.m_supplier.m_form.strAliasName() +
           ESC_EXPAND_OFF
           ESC_ALIGN_LEFT
           ESC_LF;

    for (int i = 0; i != lst.m_supplier.m_form.m_vPhone.size(); ++i)
    {
      if (!lst.m_supplier.m_form.m_vPhone.at(i).m_number.isEmpty())
      {
        if (!lst.m_supplier.m_form.m_vPhone.at(i).m_name.isEmpty())
          str += lst.m_supplier.m_form.m_vPhone.at(i).m_name + " ";
        str += lst.m_supplier.m_form.m_vPhone.at(i).strFormattedPhone() + ESC_LF;
      }
    }
  }

  str += ESC_LF;

  if (lst.m_vItem.size() != 0 && bPrintCount)
    str += ESC_ALIGN_LEFT
           ESC_DOUBLE_FONT_ON
           "[ EST ][ COM ][ REC ]"
           ESC_LF
           ESC_DOUBLE_FONT_OFF;

  Id lastSupplier, supplier;
  for (int i = 0; i != lst.m_vItem.size(); ++i)
  {
    supplier = lst.m_vItem.at(i).m_supplier.m_id;
    bool bPrintSupplier = ((!lastSupplier.isValid() && supplier.isValid()) ||
                          (lastSupplier.isValid() && supplier.isValid() && lastSupplier != supplier)) &&
                          !lst.m_supplier.m_id.isValid();
    bool bPrintSeparator = ((lastSupplier.isValid() && !supplier.isValid()) ||
                           (lastSupplier.isValid() && supplier.isValid() && lastSupplier != supplier)) &&
                           !lst.m_supplier.m_id.isValid();
    lastSupplier = supplier;

    if (bPrintSeparator)
      str += TABLE_FULL_LINE ESC_LF;

    if (bPrintSupplier && !bPrintSeparator)
      str += TABLE_FULL_LINE ESC_LF;

    if (bPrintSupplier)
      str +=  ESC_ALIGN_CENTER "Fornecedor: " + lst.m_vItem.at(i).m_supplier.m_form.strAliasName() + ESC_LF ESC_ALIGN_LEFT;

    str += ESC_STRESS_ON + lst.m_vItem.at(i).m_product.m_name + ESC_STRESS_OFF ESC_LF;

    if (lst.m_vItem.at(i).m_price != 0.0)
      str += "Preco: " + lst.m_vItem.at(i).strPrice() + ESC_LF;

    if (bPrintCount)
    {
      str += ESC_DOUBLE_FONT_ON "[     ][     ]";
      if (lst.m_vItem.at(i).m_ammount != 0.0)
        str += "[" + Data::strFmt(lst.m_vItem.at(i).m_ammount) +
               lst.m_vItem.at(i).m_package.strUnity(lst.m_vItem.at(i).m_product.m_unity) + "]";
      str += ESC_DOUBLE_FONT_OFF ESC_LF;
    }
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

  return print(str, error);
}

bool Printer::print(const Coupon& o, QString& error)
{
  QString str;
  if (o.m_store.m_id.isValid())
  {
    appendFormInfo(o.m_store.m_form, str);
    str += ESC_LF;
  }

  if (!o.m_bRedeemed)
  {
    str += ESC_ALIGN_CENTER
           ESC_EXPAND_ON
           "CUPOM DE DESCONTO"
           ESC_EXPAND_OFF
           ESC_LF
           ESC_VERT_TAB
           ESC_ALIGN_LEFT
           "Utilize o codigo abaixo para ativar o desconto:"
           ESC_LF
           ESC_VERT_TAB
           ESC_ALIGN_CENTER
           ESC_DOUBLE_FONT_ON +
           o.m_code +
           ESC_DOUBLE_FONT_OFF
           ESC_LF
           ESC_VERT_TAB
           ESC_ALIGN_LEFT
           "Emissao:" +
           o.m_dtCreation.toString("dd/MM/yyyy hh:mm:ss") +
           " @ " + QHostInfo::localHostName().toUpper() +
           ESC_LF;
    if (o.m_bExpires)
      str += "Codigo valido ate o dia "
             ESC_EXPAND_ON +
             o.m_dtExpiration.toString("dd/MM/yyyy") +
             ESC_EXPAND_OFF;
  }
  else
  {
    str += ESC_ALIGN_CENTER
           ESC_EXPAND_ON
           "CUPOM DE DESCONTO"
           ESC_LF +
           o.m_code +
           ESC_LF
           ESC_VERT_TAB
           ESC_EXPAND_OFF
           ESC_ALIGN_LEFT;
    switch (o.m_type)
    {
      case Coupon::Type::Value:
      case Coupon::Type::Percentage:
        str += ESC_ALIGN_LEFT
               ESC_DOUBLE_FONT_ON +
               o.strCoupon() +
               ESC_DOUBLE_FONT_OFF
               ESC_LF
               ESC_VERT_TAB
               ESC_ALIGN_LEFT;
        break;
      default:
        break;
    }
    str += ESC_ALIGN_LEFT "Codigo resgatado no dia: " + o.m_dtRedeemed.toString("dd/MM/yyyy hh:mm:ss");
  }
  str += ESC_LF
         ESC_VERT_TAB
         ESC_FULL_CUT;

  return print(str, error);
}
