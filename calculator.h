#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QChar>

namespace Calculator
{
enum class Button
{
  Nop,
  Num0,
  Num1,
  Num2,
  Num3,
  Num4,
  Num5,
  Num6,
  Num7,
  Num8,
  Num9,
  Plus,
  Min,
  Div,
  Mul,
  Eq,
  Dec
};

inline bool isOP(Button button)
{
  switch(button)
  {
    case Button::Plus:
    case Button::Min:
    case Button::Div:
    case Button::Mul:
      return true;
    default:
      return false;
  }
}

inline bool isDigit(Button button)
{
  switch(button)
  {
    case Button::Num0:
    case Button::Num1:
    case Button::Num2:
    case Button::Num3:
    case Button::Num4:
    case Button::Num5:
    case Button::Num6:
    case Button::Num7:
    case Button::Num8:
    case Button::Num9:
      return true;
    default:
      return false;
  }
}

inline bool isEqual(Button button)
{
  return button == Button::Eq;
}

inline bool isDecimal(Button button)
{
  return button == Button::Dec;
}

inline QChar toChar(Button button)
{
  switch(button)
  {
    case Button::Num0:
      return '0';
    case Button::Num1:
      return '1';
    case Button::Num2:
      return '2';
    case Button::Num3:
      return '3';
    case Button::Num4:
      return '4';
    case Button::Num5:
      return '5';
    case Button::Num6:
      return '6';
    case Button::Num7:
      return '7';
    case Button::Num8:
      return '8';
    case Button::Num9:
      return '9';
    case Button::Plus:
      return '+';
    case Button::Min:
      return '-';
    case Button::Div:
      return '/';
    case Button::Mul:
      return '*';
    case Button::Eq:
      return '=';
    case Button::Dec:
      return '.';
    default:
      return ' ';
  }
}
}

#endif // CALCULATOR_H
