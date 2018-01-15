#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSerialPort>

struct Settings
{
  Settings();
  QString m_filePath;
  QString m_serialPort;
  QSerialPort::BaudRate m_serialBaudRate;
  QSerialPort::DataBits m_serialDataBits;
  QSerialPort::FlowControl m_serialFlowControl;
  QSerialPort::Parity m_serialParity;
  QSerialPort::StopBits m_serialStopBits;

  void clear();
  void save() const;
  void load();
};

#endif // SETTINGS_H
