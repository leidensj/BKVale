#ifndef DATABASEPICKER_H
#define DATABASEPICKER_H

#include <QFrame>
#include <QString>
#include <QVector>
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

  Id getId() const;
  const QVector<Id>& getIds() const;
  const QVector<QString>& getNames() const;
  void setPlaceholderText(bool bSet);
  DatabaseViewer* getViewer() const;
  QString getTableText() const;

public slots:
  void clear();
  void setItems(const QVector<JItemSQL*>& v);
  bool setItem(const JItemSQL& o);
  bool setItem(Id id,
               const QString& name,
               const QByteArray& arImage);

private slots:
  void searchItem();
  void showImage();

signals:
  void changedSignal();

private:
  bool m_bMultiPicker;
  DatabaseSelector* m_selector;
  JLineEdit* m_edText;
  JClickLabel* m_lblImage;
  QVector<Id> m_ids;
  QVector<QString> m_names;
};

#endif // DATABASEPICKER_H
