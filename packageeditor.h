#ifndef PACKAGEEDITOR_H
#define PACKAGEEDITOR_H

#include <QDialog>
#include "product.h"

class JLineEdit;
class JDoubleSpinBox;
class QGroupBox;

class PackageEditor : public QDialog
{
  Q_OBJECT

public:
  explicit PackageEditor(const Package& package,
                         const QString& productUnity,
                         QWidget* parent = 0);
  Package getPackage() const;
  void setPackage(const Package& package,
                  const QString& productUnity);

private:
  QGroupBox* m_grpIsPackage;
  JLineEdit* m_edUnity;
  JDoubleSpinBox* m_spnAmmount;
};

#endif // PACKAGEEDITOR_H
