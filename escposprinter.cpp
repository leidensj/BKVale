#include "EscPosPrinter.h"
#include "settings.h"
#include <QDebug>

EscPosPrinter::EscPosPrinter(QObject *parent)
: QObject(parent)
, m_bema(false)
, m_port(9100)
{

}

EscPosPrinter::~EscPosPrinter()
{
  disconnectPrinter();
}

bool EscPosPrinter::connectToPrinter(QString& error)
{
  error.clear();
  disconnectPrinter(); // Limpa qualquer conexão anterior

  Settings s;
  s.load();
  if (s.m_serialPort.isEmpty() && s.m_ethernetIP.isEmpty())
  {
    error = QObject::tr("É necessário selecionar uma porta serial ou endereço para se conectar à impressora.");
    return false;
  }

  m_type = s.m_bIsPrinterEthernet ? PrinterType::Network : PrinterType::USB;
  m_addressOrPath = s.m_bIsPrinterEthernet ? s.m_ethernetIP : s.m_serialPort;
  m_port = s.m_ethernetPort == 0 ? 9100 : s.m_ethernetPort;

  if (m_type == Network)
  {
    m_tcpSocket = new QTcpSocket(this);
    m_tcpSocket->connectToHost(m_addressOrPath, m_port);
    bool ok = m_tcpSocket->waitForConnected(2000);
    if (!ok)
      error = tr("Erro de timeout ao conectar à impressora");
    return ok;
  }
  else if (m_type == USB)
  {
#ifdef Q_OS_WIN
    m_usbFile = new QFile("\\\\.\\" + m_addressOrPath, this); // ex: "COM5"
#else
    m_usbFile = new QFile(m_addressOrPath, this); // ex: "/dev/usb/lp0"
#endif
    bool ok = m_usbFile->open(QIODevice::WriteOnly);
    if (!ok)
      error = tr("Erro de timeout ao conectar à impressora");
    return ok;
  }

  return false;
}

void EscPosPrinter::disconnectPrinter()
{
    if (m_tcpSocket) {
        m_tcpSocket->disconnectFromHost();
        m_tcpSocket->deleteLater();
        m_tcpSocket = nullptr;
    }

    if (m_usbFile) {
        m_usbFile->close();
        m_usbFile->deleteLater();
        m_usbFile = nullptr;
    }
}

bool EscPosPrinter::isConnected() const
{
    if (m_type == Network && m_tcpSocket)
        return m_tcpSocket->state() == QTcpSocket::ConnectedState;
    if (m_type == USB && m_usbFile)
        return m_usbFile->isOpen();
    return false;
}

bool EscPosPrinter::printRawData(const QByteArray &data, QString& error)
{
    error.clear();
    if (!isConnected()) return false;

    bool ok = true;
    if (m_type == Network && m_tcpSocket)
    {
        qint64 written = m_tcpSocket->write(data);
        m_tcpSocket->flush();
        ok = written == data.size();
        if (!ok)
          error = tr("Erro ao imprimir. Endereço %1 | Porta %2").arg(m_addressOrPath, m_port);
    }

    if (m_type == USB && m_usbFile)
    {
        qint64 written = m_usbFile->write(data);
        m_usbFile->flush();
        ok = written == data.size();
        if (!ok)
          error = tr("Erro ao imprimir. Porta %1").arg(m_addressOrPath);
    }

    return ok;
}
