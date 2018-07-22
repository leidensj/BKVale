#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>

struct Settings
{
  Settings();
  QString m_databaseHostName;
  bool m_bIsPrinterEthernet;
  QString m_serialPort;
  QString m_ethernetIP;
  int m_ethernetPort;
  int m_notesDefaultNumber;
  bool m_notesPincodeRequired;

  void clear();
  void save() const;
  void load();
};

#endif // SETTINGS_H
