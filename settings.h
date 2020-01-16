#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>

struct Settings
{
  Settings();
  QString m_databaseHostName;
  int m_databasePort;
  bool m_bIsPrinterEthernet;
  QString m_serialPort;
  QString m_ethernetIP;
  int m_ethernetPort;
  qlonglong m_purchaseStoreId;

  void clear();
  void save() const;
  void load();
};

#endif // SETTINGS_H
