#ifndef PACKAGEEDITOR_H
#define PACKAGEEDITOR_H

#include <QDialog>
#include "items/product.h"

class JLineEdit;
class JExpLineEdit;
class QGroupBox;
class QLabel;

class PackageEditor : public QDialog
{
  Q_OBJECT

public:
  explicit PackageEditor(bool bUnityEditor = false, QWidget* parent = 0);
  Package getPackage() const;
  void setPackage(const Package& package, const QString& productUnity);

private:
  const bool m_bUnityEditor;
  QGroupBox* m_grpIsPackage;
  JLineEdit* m_edUnity;
  JExpLineEdit* m_edAmmount;
  QLabel* m_lblUnity;
};

#endif // PACKAGEEDITOR_H
