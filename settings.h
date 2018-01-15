#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSerialPort>

struct Serial
{
  QString m_port;
  QSerialPort::BaudRate m_baudRate;
  QSerialPort::DataBits m_dataBits;
  QSerialPort::FlowControl m_flowControl;
  QSerialPort::Parity m_parity;
  QSerialPort::StopBits m_stopBits;

  Serial();
  void clear();
};

struct Settings
{
  Settings();
  QString m_filePath;
  Serial m_serial;

  void clear();
  void save() const;
  void load();
};

#endif // SETTINGS_H
