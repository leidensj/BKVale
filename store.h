#ifndef STORE_H
#define STORE_H

#include "jitem.h"
#include "person.h"

struct Store : public JItem
{
  Person m_person;
  Address m_address;
  Phone m_phone;
  QString m_name;

  void clear()
  {
    m_person.clear();
    m_address.clear();
    m_phone.clear();
    m_name.clear();
  }

  Store()
  {
    clear();
  }
};

#endif // STORE_H
