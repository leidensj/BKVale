#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSerialPort>

enum class InterfaceType : int
{
  Serial,
  Ethernet
};

struct Settings
{
  Settings();
  QString m_databaseDir;
  InterfaceType m_interfaceType;
  bool m_bConnectOnStartup;
  QString m_serialPort;
  QSerialPort::BaudRate m_serialBaudRate;
  QSerialPort::DataBits m_serialDataBits;
  QSerialPort::FlowControl m_serialFlowControl;
  QSerialPort::Parity m_serialParity;
  QSerialPort::StopBits m_serialStopBits;
  QString m_ethernetIP;
  int m_ethernetPort;
  int m_notesDefaultNumber;

  QString databasePath() const;

  void clear();
  void save() const;
  void load();
};

#endif // SETTINGS_H
