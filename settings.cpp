#include "settings.h"
#include <QSettings>

#define COMPANY_NAME "LEICO"
#define APP_NAME     "BaitaAssistente"

#define FILE_PATH          "filePath"
#define SERIAL_PORT        "serial/port"
#define SERIAL_BAUDRATE    "serial/baudRate"
#define SERIAL_DATABITS    "serial/dataBits"
#define SERIAL_FLOWCONTROL "serial/flowControl"
#define SERIAL_PARITY      "serial/parity"
#define SERIAL_STOPBITS    "serial/stopBits"

Settings::Settings()
{
  clear();
}

void Settings::clear()
{
  m_filePath.clear();
  m_serialPort.clear();
  m_serialBaudRate = QSerialPort::Baud9600;
  m_serialDataBits = QSerialPort::Data8;
  m_serialFlowControl = QSerialPort::NoFlowControl;
  m_serialParity = QSerialPort::NoParity;
  m_serialStopBits = QSerialPort::OneStop;
}

void Settings::save() const
{
  QSettings settings(COMPANY_NAME, APP_NAME);
  settings.setValue(FILE_PATH, m_filePath);
  settings.setValue(SERIAL_PORT, m_serialPort);
  settings.setValue(SERIAL_BAUDRATE, (int)m_serialBaudRate);
  settings.setValue(SERIAL_DATABITS, (int)m_serialDataBits);
  settings.setValue(SERIAL_FLOWCONTROL, (int)m_serialFlowControl);
  settings.setValue(SERIAL_PARITY, (int)m_serialParity);
  settings.setValue(SERIAL_STOPBITS, (int)m_serialStopBits);
}

void Settings::load()
{
  QSettings settings(COMPANY_NAME, APP_NAME);
  m_filePath = settings.value(FILE_PATH).toString();
  m_serialPort = settings.value(SERIAL_PORT).toString();
  m_serialBaudRate = (QSerialPort::BaudRate)settings.value(SERIAL_BAUDRATE,
                                                           (int)QSerialPort::Baud9600).toInt();
  m_serialDataBits = (QSerialPort::DataBits)settings.value(SERIAL_DATABITS,
                                                           (int)QSerialPort::Data8).toInt();
  m_serialFlowControl = (QSerialPort::FlowControl)settings.value(SERIAL_FLOWCONTROL,
                                                                 (int)QSerialPort::NoFlowControl).toInt();
  m_serialParity = (QSerialPort::Parity)settings.value(SERIAL_PARITY,
                                                       (int)QSerialPort::NoParity).toInt();
  m_serialStopBits = (QSerialPort::StopBits)settings.value(SERIAL_STOPBITS,
                                                           (int)QSerialPort::OneStop).toInt();
}
