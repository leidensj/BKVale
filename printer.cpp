#include "printer.h"
#include <QByteArray>

Printer::Printer()
{

}

Printer::~Printer()
{
    disconnect();
}

bool Printer::connect(const QString& portName, QString& strError)
{
    strError.clear();

    if (m_serialPort.isOpen())
    {
        if (m_serialPort.portName() == portName)
        {
            return true;
        }
        else
        {
            m_serialPort.close();
        }

    }

    m_serialPort.setPortName(portName);

    bool bRet = m_serialPort.open((QIODevice::ReadWrite));
    if (!bRet)
    {
        strError = QObject::tr("Erro ao abrir a porta: ") +
                m_serialPort.errorString();
        strError += " " + QString::number(m_serialPort.error());
    }

    return bRet;
}

void Printer::disconnect()
{
    if (m_serialPort.isOpen())
        m_serialPort.close();
}

bool Printer::write(const QString& msg, QString& strError)
{
    strError.clear();
    QByteArray data("\n" + msg.toLocal8Bit());
    bool bRet = false;
    auto nBytes = m_serialPort.write(data);
    if (nBytes == -1)
      strError = QObject::tr("Falha ao escrever.");
    else if (nBytes != data.size())
      strError = QObject::tr("Falha ao escrever todos os bytes.");
    else if (!m_serialPort.waitForBytesWritten(5000))
      strError = QObject::tr("Falha de timeout.");
    else
        bRet = true;
    return bRet;
}

bool Printer::initialize(QString& strError)
{
    strError.clear();
    QString strMsg = "\n\x1b@";
    strMsg += "\nhaha";
    return write(strMsg, strError);
}

/*bool PrinterHelpers::print(const QSerialPort& serialPort, const QString& text, QString& error)
{
    error.clear();

    QByteArray data(msg.toLocal8Bit());
    bool bRet = false;
    auto nBytes = serialPort.write(data);
    if (nBytes == -1 || nBytes != data.size())
      error = serialPort.errorString();
    else if (!m_serialPort.waitForBytesWritten(5000))
      strError = QObject::tr("Falha de timeout.");
    else
        bRet = true;

    return bRet;
}*/

/*bool PrinterHelpers::printHeader(const QSerialPort& serialPort, QString& error)
{

}*/
