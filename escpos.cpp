#include "escpos.h"
#include "settings.h"

EscPos::EscPos()
: m_bema(false)
{
  Settings s;
  s.load();
  m_bema = s.m_bema;
  init();
}
void EscPos::init() { m_ba.append(ESC_INIT, 2); }
void EscPos::str(const QString& str) { m_ba.append(str.toLocal8Bit()); }

void EscPos::bold(bool b) { m_bema ? boldBEMA(b) : boldESCPOS(b); }
void EscPos::doublefont(bool b) { m_bema ? doublefontBEMA(b) : doublefontESCPOS(b); }
void EscPos::align(bool center) { m_bema ? alignBEMA(center) : alignESCPOS(center); }
void EscPos::cut(bool partial) { m_bema ? cutBEMA(partial) : cutESCPOS(partial); }
void EscPos::expand(bool b) { m_bema ? expandBEMA(b) : expandESCPOS(b); }
void EscPos::tab() { m_bema ? tabBEMA() : tabBEMA();}

void EscPos::boldESCPOS(bool b) { b ? m_ba.append(ESC_BOLD_ON, 3) : m_ba.append(ESC_BOLD_OFF, 3); }
void EscPos::doublefontESCPOS(bool b) { b ? m_ba.append(ESC_DOUBLEFONT_ON, 3) : m_ba.append(ESC_DOUBLEFONT_OFF, 3); }
void EscPos::alignESCPOS(bool center) { center ? m_ba.append(ESC_ALIGNCENTER, 3) : m_ba.append(ESC_ALIGNLEFT, 3); }
void EscPos::cutESCPOS(bool partial) { partial ? m_ba.append(ESC_PARTIALCUT, 3) : m_ba.append(ESC_FULLCUT, 3); }
void EscPos::expandESCPOS(bool b) { /*TODO*/ }

void EscPos::boldBEMA(bool b) { b ? m_ba.append(ESC_STRESS_ON, 2) : m_ba.append(ESC_STRESS_OFF, 2); }
void EscPos::doublefontBEMA(bool b) { b ? m_ba.append(ESC_DOUBLE_FONT_ON, 6) : m_ba.append(ESC_DOUBLE_FONT_OFF, 6); }
void EscPos::alignBEMA(bool center) { center ? m_ba.append(ESC_ALIGN_CENTER, 3) : m_ba.append(ESC_ALIGN_LEFT, 3); }
void EscPos::cutBEMA(bool partial) { partial ? m_ba.append(ESC_PARTIAL_CUT, 2) : m_ba.append(ESC_FULL_CUT, 2); }
void EscPos::tabBEMA() { m_ba.append(ESC_VERT_TAB, 3); }
void EscPos::expandBEMA(bool b) { b ? m_ba.append(ESC_EXPAND_ON, 3) : m_ba.append(ESC_EXPAND_OFF, 3); }
