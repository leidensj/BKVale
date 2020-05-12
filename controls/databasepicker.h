#ifndef DATABASEPICKER_H
#define DATABASEPICKER_H

#include <QFrame>
#include <QSqlDatabase>
#include "items/jitemsql.h"
#include "databaseviewer.h"

class QPushButton;
class JLineEdit;
class DatabaseSelector;
class JClickLabel;

class DatabasePicker : public QFrame
{
  Q_OBJECT

public:
  explicit DatabasePicker(const QString& tableName, bool bMultiPicker = false, QWidget* parent = nullptr);
  Id getFirstId() const;
  QString getFirstName() const;
  const Ids& getIds() const;
  const Names& getNames() const;
  void setPlaceholderText(bool bSet);
  DatabaseViewer* getViewer() const;
  QString getTableText() const;

public slots:
  void clear();
  bool addItem(const JItemSQL& o);
  bool addItem(Id id, const QString& name, const QByteArray& arImage);

private slots:
  void setSelectorItems();
  void searchItem();
  void showImage();

signals:
  void changedSignal();

private:
  bool m_bMultiPicker;
  DatabaseSelector* m_selector;
  JLineEdit* m_edText;
  JClickLabel* m_lblImage;
  Ids m_ids;
  Names m_names;
};

#endif // DATABASEPICKER_H
