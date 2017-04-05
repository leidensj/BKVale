#include "settings.h"

Settings::Settings()
{
  clear();
}

void Settings::clear()
{
  port.clear();
  baudRate = QSerialPort::Baud9600;
  dataBits = QSerialPort::Data8;
  flowControl = QSerialPort::NoFlowControl;
  parity = QSerialPort::NoParity;
  stopBits = QSerialPort::OneStop;
}
