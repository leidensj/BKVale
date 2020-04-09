#include "jlineedit.h"
#include "libraries/tinyexpr.h"
#include "jregexpvalidator.h"

JLineEdit::JLineEdit(Text::Input input, int flags, QWidget* parent)
  : QLineEdit(parent)
  , m_flags(flags)
{
  if (input != Text::Input::All)
    setValidator(new JRegExpValidator(flags & (int)Flags::ToUpper,
                                      QRegExp(getRegEx(input)), this));
}

void JLineEdit::keyPressEvent(QKeyEvent *event)
{
  switch (event->key())
  {
    case Qt::Key_Enter:
    case Qt::Key_Return:
    {
      if (m_flags & (int)Flags::EnterAsTab)
        focusNextChild();
      emit enterSignal();
    } break;
    case Qt::Key_Down:
    {
      if (m_flags & (int)Flags::ArrowsAsTab)
        focusNextChild();
      emit keyDownSignal();
    } break;
    case Qt::Key_Up:
    {
      if (m_flags && (int)Flags::ArrowsAsTab)
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

JExpLineEdit::JExpLineEdit(Data::Type type,
                           int flags,
                           double defaultValue,
                           QWidget* parent)
  : JLineEdit(Text::Input::All, flags, parent)
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
