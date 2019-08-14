#include "jdatabasecombobox.h"
#include "items/jitemex.h"
#include "models/jmodel.h"
#include <QMessageBox>

JDatabaseComboBox::JDatabaseComboBox(const QString& tableName,
                                     int modelColumn,
                                     QWidget* parent)
  : QComboBox(parent)
{
  JModel* pModel = JItemEx::model(tableName, this);
  if (pModel == nullptr)
  {
    QMessageBox::critical(this, tr("Erro"), tr("Modelo não implementado"), QMessageBox::Ok);
    exit(EXIT_FAILURE);
  }

  pModel->select();
  setModel(pModel);
  setModelColumn(modelColumn);
}

void JDatabaseComboBox::refresh()
{
  JModel* pModel = dynamic_cast<JModel*>(model());
  if (pModel != nullptr)
    pModel->select();
  if (count() > 0)
    setCurrentIndex(0);
}

Id JDatabaseComboBox::getCurrentId() const
{
  JModel* pModel = dynamic_cast<JModel*>(model());
  if (pModel != nullptr)
  {
    QModelIndexList lst = view()->selectionModel()->selectedIndexes();
    if (lst.count() > 0)
      return Id(pModel->index(lst.at(0).row(), SQL_COLID_NUMBER).data(Qt::EditRole).toLongLong());
  }
  return Id();
}

void JDatabaseComboBox::setCurrentId(const Id& id)
{
  JModel* pModel = dynamic_cast<JModel*>(model());
  if (pModel != nullptr)
  {
    QModelIndexList lst = pModel->match(pModel->index(0, 0), Qt::EditRole, id.get(), 1, Qt::MatchExactly);
    if (lst.size() != 0)
      setCurrentIndex(lst.at(0).row());
  }
}
