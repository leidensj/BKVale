#include "settings.h"
#include "defines.h"
#include <QSettings>

#define COMPANY_NAME "LEICO"
#define APP_NAME     "BaitaAssistente"
#define DATABASE_NAME    "BaitaAssistente.db"

#define DATABASE_DIR         "databaseDir"
#define INTERFACE_TYPE       "interfaceType"
#define CONNECT_STARTUP      "connectStartup"
#define SERIAL_PORT          "serial/port"
#define SERIAL_BAUDRATE      "serial/baudRate"
#define SERIAL_DATABITS      "serial/dataBits"
#define SERIAL_FLOWCONTROL   "serial/flowControl"
#define SERIAL_PARITY        "serial/parity"
#define SERIAL_STOPBITS      "serial/stopBits"
#define ETHERNET_IP          "ethernet/ip"
#define ETHERNET_PORT        "ethernet/port"
#define NOTES_DEFAULT_NUMBER "notes/defaultnumber"

Settings::Settings()
{
  clear();
}

void Settings::clear()
{
  m_databaseDir.clear();
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
  m_notesDefaultNumber = NOTE_DEFAULT_NUMBER;
}

void Settings::save() const
{
  QSettings settings(COMPANY_NAME, APP_NAME);
  settings.setValue(DATABASE_DIR, m_databaseDir);
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
  settings.setValue(NOTES_DEFAULT_NUMBER, m_notesDefaultNumber);
}

void Settings::load()
{
  QSettings settings(COMPANY_NAME, APP_NAME);
  m_databaseDir = settings.value(DATABASE_DIR).toString();
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
  m_notesDefaultNumber = settings.value(NOTES_DEFAULT_NUMBER, NOTE_DEFAULT_NUMBER).toInt();
}

QString Settings::databasePath() const
{
  return m_databaseDir + "/" + DATABASE_NAME;
}
