#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>

#define PRINTER_MODEL_NONE 0x0
#define PRINTER_MODEL_BEMATECH4200TH 0x1
#define PRINTER_MODEL_ELGINI9 0x2

struct Settings
{
  Settings();
  QString m_databaseHostName;
  int m_databasePort;
  bool m_bIsPrinterEthernet;
  QString m_serialPort;
  QString m_ethernetIP;
  int m_printerModel;
  int m_ethernetPort;
  qlonglong m_purchaseStoreId;

  void clear();
  void save() const;
  void load();
};

#endif // SETTINGS_H
