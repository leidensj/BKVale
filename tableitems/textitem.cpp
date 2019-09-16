#include "textitem.h"
#include "widgets/jregexpvalidator.h"

TextItem::TextItem(Text::Input input, bool toUpper)
  : m_input(input)
  , m_toUpper(toUpper)
{

}

void TextItem::setValue(const QVariant& v)
{
  setData(Qt::UserRole, v.toString());
  setText(v.toString());
}

void TextItem::evaluate()
{
  JRegExpValidator val(m_toUpper, QRegExp(Text::getRegEx(m_input)));
  QString str = text();
  if (m_toUpper)
    str.toUpper();
  int pos = str.length();
  bool bValid = val.validate(str, pos) == QValidator::State::Acceptable;
  if (bValid)
    setValue(str);
  else
    setValue(getValue());
}

void TextItem::erase()
{
  setText("");
}

void TextItem::activate()
{

}
