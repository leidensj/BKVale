#ifndef ESCPOSPRINTER_H
#define ESCPOSPRINTER_H

#include <QObject>
#include <QTcpSocket>
#include <QFile>
#include <QTimer>

class EscPosPrinter : public QObject
{
    Q_OBJECT

public:
    enum PrinterType {
        USB,
        Network
    };

    explicit EscPosPrinter(QObject *parent = nullptr);
    ~EscPosPrinter();

    bool connectToPrinter(QString& error);
    void disconnectPrinter();
    bool isConnected() const;

    bool printRawData(const QByteArray &data, QString& error);

private:
    PrinterType m_type;
    QString m_addressOrPath;
    int m_port;
    bool m_bema;

    QTcpSocket *m_tcpSocket = nullptr;
    QFile *m_usbFile = nullptr;
};


#endif // ESCPOSPRINTER_H
