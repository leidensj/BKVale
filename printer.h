#ifndef PRINTER_H
#define PRINTER_H

#include <QString>
#include <QSerialPort>

class Printer
{
    QSerialPort m_serialPort;
public:
    Printer();
    ~Printer();
    bool connect(const QString& strPortName, QString& strError);
    void disconnect();
    bool initialize(QString& strError);
    bool write(const QString& strMsg, QString& strError);
};

namespace PrinterHelpers
{
    bool print(const QSerialPort& serialPort, const QString& text, QString& error);
    bool printHeader(const QSerialPort& serialPort, QString& error);
}
#endif // PRINTER_H
