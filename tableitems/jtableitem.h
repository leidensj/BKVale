#ifndef JTABLEITEM_H
#define JTABLEITEM_H

#include <QTableWidgetItem>

#define JTABLEITEMCUSTOMDATA 0x200

class JTableItem : public QTableWidgetItem
{
public:
  virtual void evaluate() = 0;
  virtual void erase() = 0;
  virtual void activate() = 0;
  virtual void setValue(const QVariant& v) = 0;
  virtual QVariant getValue() const { return data(Qt::UserRole); }
  void setReadOnly(bool b) { setFlags(b ? (flags() & ~Qt::ItemIsEditable) : (flags() |= Qt::ItemIsEditable )); }
  void setCustomData(const QVariant var) { setData(JTABLEITEMCUSTOMDATA, var); }
  QVariant getCustomData() const { return data(JTABLEITEMCUSTOMDATA); }
};

#endif // JTABLEITEM_H
