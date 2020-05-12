#ifndef DATABASESELECTOR_H
#define DATABASESELECTOR_H

#include <QDialog>
#include "databaseviewer.h"

class DatabaseSelector : public QDialog
{
  Q_OBJECT

public:
  explicit DatabaseSelector(const QString& tableName,
                            bool bMultiSelector = false,
                            QWidget* parent = 0);

  DatabaseViewer* getViewer() const;

protected:
  void closeEvent(QCloseEvent * e);

private:
  DatabaseViewer* m_viewer;
};

#endif // DATABASESELECTOR_H
