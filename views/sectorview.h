#ifndef SECTORVIEW_H
#define SECTORVIEW_H

#include "items/sector.h"
#include "jitemview.h"

class QPushButton;
class JLineEdit;
class DatabasePicker;

class SectorView : public JItemView
{
  Q_OBJECT

public:
  explicit SectorView(QWidget* parent = 0);

public slots:
  void getItem(JItemSQL& o) const;

protected slots:
  void setItem(const JItemSQL& o);

private:
  JLineEdit* m_edName;
  DatabasePicker* m_imagePicker;
};

#endif // SECTORVIEW_H
