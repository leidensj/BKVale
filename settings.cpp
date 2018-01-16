#include "settings.h"
#include <QSettings>

#define COMPANY_NAME "LEICO"
#define APP_NAME     "BaitaAssistente"
#define FILE_NAME    "BaitaAssistente.db"

#define FILE_PATH          "filePath"
#define INTERFACE_TYPE     "interfaceType"
#define CONNECT_STARTUP    "connectStartup"
#define SERIAL_PORT        "serial/port"
#define SERIAL_BAUDRATE    "serial/baudRate"
#define SERIAL_DATABITS    "serial/dataBits"
#define SERIAL_FLOWCONTROL "serial/flowControl"
#define SERIAL_PARITY      "serial/parity"
#define SERIAL_STOPBITS    "serial/stopBits"
#define ETHERNET_IP        "ethernet/ip"
#define ETHERNET_PORT      "ethernet/port"

Settings::Settings()
{
  clear();
}

void Settings::clear()
{
  m_fileDir.clear();
  m_interfaceType = InterfaceType::Serial;
  m_bConnectOnStartup = false;
  m_serialPort.clear();
  m_serialBaudRate = QSerialPort::Baud9600;
  m_serialDataBits = QSerialPort::Data8;
  m_serialFlowControl = QSerialPort::NoFlowControl;
  m_serialParity = QSerialPort::NoParity;
  m_serialStopBits = QSerialPort::OneStop;
  m_ethernetIP.clear();
  m_ethernetPort = 9100;
}

void Settings::save() const
{
  QSettings settings(COMPANY_NAME, APP_NAME);
  settings.setValue(FILE_PATH, m_fileDir);
  settings.setValue(INTERFACE_TYPE, (int)m_interfaceType);
  settings.setValue(CONNECT_STARTUP, m_bConnectOnStartup);
  settings.setValue(SERIAL_PORT, m_serialPort);
  settings.setValue(SERIAL_BAUDRATE, (int)m_serialBaudRate);
  settings.setValue(SERIAL_DATABITS, (int)m_serialDataBits);
  settings.setValue(SERIAL_FLOWCONTROL, (int)m_serialFlowControl);
  settings.setValue(SERIAL_PARITY, (int)m_serialParity);
  settings.setValue(SERIAL_STOPBITS, (int)m_serialStopBits);
  settings.setValue(ETHERNET_IP, m_ethernetIP);
  settings.setValue(ETHERNET_PORT, m_ethernetPort);
}

void Settings::load()
{
  QSettings settings(COMPANY_NAME, APP_NAME);
  m_fileDir = settings.value(FILE_PATH).toString();
  m_interfaceType = (InterfaceType)settings.value(INTERFACE_TYPE, (int)InterfaceType::Serial).toInt();
  m_bConnectOnStartup = settings.value(CONNECT_STARTUP, false).toBool();
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
  m_ethernetIP = settings.value(ETHERNET_IP).toString();
  m_ethernetPort = settings.value(ETHERNET_PORT, 9100).toInt();
}

QString Settings::filePath() const
{
  return m_fileDir + "/" + FILE_NAME;
}
