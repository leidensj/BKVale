#include "settings.h"
#include "defines.h"
#include <QSettings>

#define COMPANY_NAME "LEICO"
#define APP_NAME     "BaitaAssistente"
#define DATABASE_NAME    "BaitaAssistente.db"

#define DATABASE_DIR           "databaseDir"
#define IS_ETHERNET            "isethernet"
#define SERIAL_PORT            "serial/port"
#define ETHERNET_IP            "ethernet/ip"
#define ETHERNET_PORT          "ethernet/port"
#define NOTES_DEFAULT_NUMBER   "notes/defaultnumber"
#define NOTES_PINCODE_REQUIRED "notes/pincoderequired"

Settings::Settings()
{
  clear();
}

void Settings::clear()
{
  m_databaseDir.clear();
  m_bIsPrinterEthernet = false;
  m_serialPort.clear();
  m_ethernetIP.clear();
  m_ethernetPort = 9100;
  m_notesDefaultNumber = 1000;
  m_notesPincodeRequired = true;
}

void Settings::save() const
{
  QSettings settings(COMPANY_NAME, APP_NAME);
  settings.setValue(DATABASE_DIR, m_databaseDir);
  settings.setValue(IS_ETHERNET, m_bIsPrinterEthernet);
  settings.setValue(SERIAL_PORT, m_serialPort);
  settings.setValue(ETHERNET_IP, m_ethernetIP);
  settings.setValue(ETHERNET_PORT, m_ethernetPort);
  settings.setValue(NOTES_DEFAULT_NUMBER, m_notesDefaultNumber);
  settings.setValue(NOTES_PINCODE_REQUIRED, m_bIsPrinterEthernet);
}

void Settings::load()
{
  QSettings settings(COMPANY_NAME, APP_NAME);
  m_databaseDir = settings.value(DATABASE_DIR).toString();
  m_bIsPrinterEthernet = settings.value(IS_ETHERNET, false).toBool();
  m_serialPort = settings.value(SERIAL_PORT).toString();
  m_ethernetIP = settings.value(ETHERNET_IP).toString();
  m_ethernetPort = settings.value(ETHERNET_PORT, 9100).toInt();
  m_notesDefaultNumber = settings.value(NOTES_DEFAULT_NUMBER, 1000).toInt();
}

QString Settings::databasePath() const
{
  return m_databaseDir + "/" + DATABASE_NAME;
}
