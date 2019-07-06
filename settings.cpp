#include "settings.h"
#include "defines.h"
#include <QSettings>

#define COMPANY_NAME "LEICO"
#define APP_NAME     "BaitaAssistente"

#define DATABASE_HOST_NAME     "databaseHostName"
#define DATABASE_PORT          "databasePort"
#define IS_ETHERNET            "isethernet"
#define SERIAL_PORT            "serial/port"
#define ETHERNET_IP            "ethernet/ip"
#define ETHERNET_PORT          "ethernet/port"

#define DEFAULT_DATABASE_PORT 5432

Settings::Settings()
{
  clear();
}

void Settings::clear()
{
  m_databaseHostName.clear();
  m_databasePort = DEFAULT_DATABASE_PORT;
  m_bIsPrinterEthernet = false;
  m_serialPort.clear();
  m_ethernetIP.clear();
  m_ethernetPort = 9100;
}

void Settings::save() const
{
  QSettings settings(COMPANY_NAME, APP_NAME);
  settings.setValue(DATABASE_HOST_NAME, m_databaseHostName);
  settings.setValue(DATABASE_PORT, m_databasePort);
  settings.setValue(IS_ETHERNET, m_bIsPrinterEthernet);
  settings.setValue(SERIAL_PORT, m_serialPort);
  settings.setValue(ETHERNET_IP, m_ethernetIP);
  settings.setValue(ETHERNET_PORT, m_ethernetPort);
}

void Settings::load()
{
  QSettings settings(COMPANY_NAME, APP_NAME);
  m_databaseHostName = settings.value(DATABASE_HOST_NAME).toString();
  m_databasePort = settings.value(DATABASE_PORT, DEFAULT_DATABASE_PORT).toInt();
  m_bIsPrinterEthernet = settings.value(IS_ETHERNET, false).toBool();
  m_serialPort = settings.value(SERIAL_PORT).toString();
  m_ethernetIP = settings.value(ETHERNET_IP).toString();
  m_ethernetPort = settings.value(ETHERNET_PORT, 9100).toInt();
}
