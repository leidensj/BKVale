#include "jlineedit.h"
#include "libraries/tinyexpr.h"
#include "jregexpvalidator.h"

JLineEdit::JLineEdit(Text::Input input, bool bToUpper, QWidget* parent)
  : QLineEdit(parent)
  , m_bToUpper(bToUpper)
  , m_bArrowsAndEnterAsTab(true)
{
  if (input != Text::Input::All)
    setValidator(new JRegExpValidator(bToUpper, QRegExp(getRegEx(input)), this));
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

void JLineEdit::setTextBlockingSignals(const QString& str)
{
  blockSignals(true);
  setText(str);
  blockSignals(false);
}

JExpLineEdit::JExpLineEdit(Data::Type type, bool bToUpper, double defaultValue, QWidget* parent)
  : JLineEdit(Text::Input::All, bToUpper, parent)
  , m_dataType(type)
  , m_defaultValue(defaultValue)
  , m_currentValue(defaultValue)
{
  connect(this, SIGNAL(editingFinished()), this, SLOT(evaluate()));
  evaluate();
}

double JExpLineEdit::getValue() const
{
  return m_currentValue;
}

void JExpLineEdit::evaluate()
{
  auto stdExp = QLineEdit::text().toStdString();
  int error = 0;
  double val = te_interp(stdExp.c_str(), &error);
  if (!error)
  {
    m_currentValue = val;
    setTextBlockingSignals(Data::str(val, m_dataType));
    emit valueChanged(m_currentValue);
  }

  if (m_currentValue == m_defaultValue)
    setTextBlockingSignals("");

  QPalette _palette = palette();
  _palette.setColor(QPalette::ColorRole::Text, m_currentValue >= 0 ? Qt::red : Qt::darkGreen);
  setPalette(_palette);
}

void JExpLineEdit::setText(const QString& text)
{
  QLineEdit::setText(text);
  evaluate();
}

void JExpLineEdit::setText(double val)
{
  setText(QString::number(val));
}

QString JExpLineEdit::text() const
{
  return Data::str(getValue(), m_dataType);
}
