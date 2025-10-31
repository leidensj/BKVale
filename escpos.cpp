#include "escpos.h"

namespace {
QString removeAccents(const QString &str)
{
  QString result;
  for (QChar& c : str.normalized(QString::NormalizationForm_D))
    if (!c.isMark())
      result.append(c);
  return result;
}
}

EscPos::EscPos()
: m_printerModel(PRINTER_MODEL_NONE)
{
  Settings s;
  s.load();
  m_printerModel = s.m_printerModel;
  init();
}

void EscPos::init()
{
  m_ba.append(ESC_INIT, 2);
}

void EscPos::str(const QString& str)
{
  m_ba.append(removeAccents(str).toLocal8Bit());
}

void EscPos::bold(bool b)
{
  switch (m_printerModel)
  {
    case PRINTER_MODEL_ELGINI9:
      b ? m_ba.append(ESC_BOLD_ON, 3) : m_ba.append(ESC_BOLD_OFF, 3);
      break;
    case PRINTER_MODEL_BEMATECH4200TH:
      b ? m_ba.append(ESC_STRESS_ON, 2) : m_ba.append(ESC_STRESS_OFF, 2);
      break;
    case PRINTER_MODEL_NONE:
    default:
      break;
  }
}

void EscPos::doublefont(bool b)
{
  switch (m_printerModel)
  {
    case PRINTER_MODEL_ELGINI9:
      b ? m_ba.append(ESC_DOUBLEFONT_ON, 3) : m_ba.append(ESC_DOUBLEFONT_OFF, 3);
      break;
    case PRINTER_MODEL_BEMATECH4200TH:
     b ? m_ba.append(ESC_DOUBLE_FONT_ON, 6) : m_ba.append(ESC_DOUBLE_FONT_OFF, 6);
      break;
    case PRINTER_MODEL_NONE:
    default:
      break;
  }
}
void EscPos::align(bool center)
{
  switch (m_printerModel)
  {
    case PRINTER_MODEL_ELGINI9:
      center ? m_ba.append(ESC_ALIGNCENTER, 3) : m_ba.append(ESC_ALIGNLEFT, 3);
      break;
    case PRINTER_MODEL_BEMATECH4200TH:
      center ? m_ba.append(ESC_ALIGN_CENTER, 3) : m_ba.append(ESC_ALIGN_LEFT, 3);
      break;
    case PRINTER_MODEL_NONE:
    default:
      break;
  }
}

void EscPos::cut(bool partial)
{
  doublefont(false);
  expand(false);
  align(false);
  str("\n\n");
  switch (m_printerModel)
  {
    case PRINTER_MODEL_ELGINI9:
      partial ? m_ba.append(ESC_PARTIALCUT, 3) : m_ba.append(ESC_FULLCUT, 3);
      break;
    case PRINTER_MODEL_BEMATECH4200TH:
      partial ? m_ba.append(ESC_PARTIAL_CUT, 2) : m_ba.append(ESC_FULL_CUT, 2);
      break;
    case PRINTER_MODEL_NONE:
    default:
      break;
  }
}

void EscPos::expand(bool b)
{
  switch (m_printerModel)
  {
    case PRINTER_MODEL_ELGINI9:
      b ? m_ba.append(ESC_EXPANDON, 3) :  m_ba.append(ESC_EXPANDOFF, 3);
      break;
    case PRINTER_MODEL_BEMATECH4200TH:
      b ? m_ba.append(ESC_EXPAND_ON, 3) : m_ba.append(ESC_EXPAND_OFF, 3);
      break;
    case PRINTER_MODEL_NONE:
    default:
      break;
  }
}
