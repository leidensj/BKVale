#ifndef ITEMWIDGET_H
#define ITEMWIDGET_H

#include <QFrame>
#include "item.h"
#include <QSqlDatabase>
#include <QLineEdit>
#include <QTableView>

class QCheckBox;
class QPushButton;

class ItemTableView : public QTableView
{
  Q_OBJECT

public:
  ItemTableView(bool bEditMode, QWidget *parent = 0);

protected:
  void keyPressEvent(QKeyEvent* event);

signals:
  void enterKeyPressedSignal();
};

class FilterLineEdit : public QLineEdit
{
  Q_OBJECT

public:
  FilterLineEdit();

protected:
  void keyPressEvent(QKeyEvent *event);

public slots:
  void toUpper();

signals:
  void downArrowPressedSignal();
  void enterKeyPressedSignal();
  void changedSignal();
};

class ItemWidget : public QFrame
{
  Q_OBJECT

public:
  explicit ItemWidget(bool bEditMode, QWidget *parent = 0);
  ~ItemWidget();
  void setDatabase(QSqlDatabase db);

public slots:
  void refresh();
  void enableControls();
  void removeSelectedItem();
  void save(bool bSkipConfirmation);
  void discard(bool bSkipConfirmation);
  void create();
  void setFilter();
  void focusFilter();
  void containsPressed();
  void filterDownArrowPressed();
  void filterEnterKeyPressed();
  void itemSelected();

private:
  FilterLineEdit* m_filter;
  QCheckBox* m_contains;
  ItemTableView* m_table;
  QPushButton* m_create;
  QPushButton* m_save;
  QPushButton* m_discard;
  QPushButton* m_refresh;
  QPushButton* m_remove;
  QFrame* m_buttons;
  bool m_bEditMode;
  void confirm();
  Item item() const;
  ItemTableIndex currentSortIndicator() const;

signals:
  void itemSelectedSignal(const Item& item);
};

#endif // ITEMWIDGET_H
