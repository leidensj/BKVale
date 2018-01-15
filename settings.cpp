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

Serial::Serial()
{

}

void Serial::clear()
{
  port.clear();
  baudRate = QSerialPort::Baud9600;
  dataBits = QSerialPort::Data8;
  flowControl = QSerialPort::NoFlowControl;
  parity = QSerialPort::NoParity;
  stopBits = QSerialPort::OneStop;
}

Settings::Settings()
{
  clear();
}

void Settings::clear()
{
  m_filePath.clear();
  m_serial.clear();
}

void Settings::save() const
{
  QSettings settings(COMPANY_NAME, APP_NAME);
  settings.setValue(FILE_PATH, m_filePath);
  settings.setValue(SERIAL_PORT, m_serial.m_port);
  settings.setValue(SERIAL_BAUDRATE, (int)m_serial.m_baudRate);
  settings.setValue(SERIAL_DATABITS, (int)m_serial.m_dataBits);
  settings.setValue(SERIAL_FLOWCONTROL, (int)m_serial.m_flowControl);
  settings.setValue(SERIAL_PARITY, (int)m_serial.m_parity);
  settings.setValue(SERIAL_STOPBITS, (int)m_serial.m_stopBits);
}

void Settings::load()
{
  QSettings settings(COMPANY_NAME, APP_NAME);
  m_filePath = settings.value(FILE_PATH).toString();
  m_serial.m_port = settings.value(SERIAL_PORT).toString();
  m_serial.m_baudRate = (QSerialPort::BaudRate)settings.value(SERIAL_BAUDRATE,
                                                              (int)QSerialPort::Baud9600).toInt();
  m_serial.m_dataBits = (QSerialPort::DataBits)settings.value(SERIAL_DATABITS,
                                                              (int)QSerialPort::Data8).toInt();
  m_serial.m_flowControl = (QSerialPort::FlowControl)settings.value(SERIAL_FLOWCONTROL,
                                                                    (int)QSerialPort::NoFlowControl).toInt();
  m_serial.m_parity = (QSerialPort::Parity)settings.value(SERIAL_PARITY,
                                                          (int)QSerialPort::NoParity).toInt();
  m_serial.m_stopBits = (QSerialPort::StopBits)settings.value(SERIAL_STOPBITS,
                                                              (int)QSerialPort::OneStop).toInt();
}
