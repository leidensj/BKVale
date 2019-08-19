#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QSerialPort>

struct Settings
{
  Settings();
  QString m_databaseHostName;
  int m_databasePort;
  bool m_bIsPrinterEthernet;
  QString m_serialPort;
  QString m_ethernetIP;
  int m_ethernetPort;
  QSerialPort::BaudRate m_serialBaudRate;
  QSerialPort::DataBits m_serialDataBits;
  QSerialPort::FlowControl m_serialFlowControl;
  QSerialPort::Parity m_serialParity;
  QSerialPort::StopBits m_serialStopBits;
  qlonglong m_purchaseStoreId;

  void clear();
  void save() const;
  void load();
};

#endif // SETTINGS_H
