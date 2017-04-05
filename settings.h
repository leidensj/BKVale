#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSerialPort>

struct Settings
{
  QString port;
  QSerialPort::BaudRate baudRate;
  QSerialPort::DataBits dataBits;
  QSerialPort::FlowControl flowControl;
  QSerialPort::Parity parity;
  QSerialPort::StopBits stopBits;

  Settings();
  void clear();
};

#endif // SETTINGS_H
