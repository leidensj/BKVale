#include "jlineedit.h"
#include "libraries/tinyexpr.h"
#include "jregexpvalidator.h"
#include <limits>

JLineEdit::JLineEdit(Text::Input input, bool bToUpper, QWidget* parent)
  : QLineEdit(parent)
  , m_bToUpper(bToUpper)
  , m_bArrowsAndEnterAsTab(true)
{
  if (input != Text::Input::All)
    setValidator(new JRegExpValidator(bToUpper, QRegularExpression(getRegEx(input)), this));
}

void JLineEdit::setArrowsAndEnterAsTab(bool b)
{
  m_bArrowsAndEnterAsTab = b;
}

void JLineEdit::keyPressEvent(QKeyEvent *event)
{
  switch (event->key())
  {
    case Qt::Key_Enter:
    case Qt::Key_Return:
    {
      if (m_bArrowsAndEnterAsTab)
        focusNextChild();
      emit enterSignal();
    } break;
    case Qt::Key_Down:
    {
      if (m_bArrowsAndEnterAsTab)
        focusNextChild();
      emit keyDownSignal();
    } break;
    case Qt::Key_Up:
    {
      if (m_bArrowsAndEnterAsTab)
        focusPreviousChild();
      emit keyUpSignal();
    } break;
    case Qt::Key_Delete:
    {
      emit deleteSignal();
      QLineEdit::keyPressEvent(event);
    } break;
    default:
      QLineEdit::keyPressEvent(event);
  }
}

JExpLineEdit::JExpLineEdit(Data::Type type, QWidget* parent)
  : JLineEdit(Text::Input::All, false, parent)
  , m_dataType(type)
  , m_currentValue(0.0)
  , m_minimumValue(std::numeric_limits<double>::lowest())
  , m_maximumValue(std::numeric_limits<double>::max())
{
  connect(this, &JExpLineEdit::editingFinished, [this](){ evaluate(text()); });
  evaluate("");
}

double JExpLineEdit::value() const
{
  return m_currentValue;
}

void JExpLineEdit::evaluate(const QString& value)
{
  if (m_currentText == value)
    return;

  auto stdExp = value.toStdString();
  int error = 0;
  double val = te_interp(stdExp.c_str(), &error);
  if (!error && val >= m_minimumValue && val <= m_maximumValue)
  {
    m_currentValue = val;
    m_currentText = Data::str(val, m_dataType);
    setText(m_currentText);
  }
  else
    m_currentValue = 0.0;

  if (m_currentValue == 0.0)
    m_currentText.clear();
  setText(m_currentText);

  emit valueChanged(m_currentValue);

  QPalette _palette = palette();
  _palette.setColor(QPalette::ColorRole::Text, m_currentValue >= 0.0 ? Qt::red : Qt::darkGreen);
  setPalette(_palette);
}

void JExpLineEdit::setValue(const QString& value)
{
  evaluate(value);
}

void JExpLineEdit::setValue(double value)
{
  setValue(QString::number(value));
}

QString JExpLineEdit::strValue() const
{
  return Data::str(value(), m_dataType);
}

void JExpLineEdit::setMinimum(double value)
{
  m_minimumValue = value;
}

void JExpLineEdit::setMaximum(double value)
{
  m_maximumValue = value;
}
