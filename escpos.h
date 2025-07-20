#ifndef ESC_H
#define ESC_H

#include <QString>
#include <QByteArray>
#include "settings.h"

#define ESC                 "\x1b"
#define ESC_ALIGN_CENTER    "\x1b\x61\x31"
#define ESC_ALIGN_LEFT      "\x1b\x61\x30"
#define ESC_FULL_CUT        "\x1b\x77"
#define ESC_PARTIAL_CUT     "\x1b\x6d"
#define ESC_STRESS_ON       "\x1b\x45"
#define ESC_STRESS_OFF      "\x1b\x46"
#define ESC_LF              "\n"
#define ESC_VERT_TAB        "\x1b\x4a\x40"

#define ESC_INIT            "\x1b\x40" //ESC @
#define ESC_DOUBLE_FONT_ON  "\x1b\x64\x31\x1b\x57\x31"
#define ESC_DOUBLE_FONT_OFF "\x1b\x64\x30\x1b\x57\x30"
#define ESC_PORTUGUESE      "\x1b\x74\x08"
#define ESC_CODEPAGE850     "\x1b\x74\x02"
#define ESC_REVERSE_ON      "\x1b\x7d\x31"
#define ESC_REVERSE_OFF     "\x1b\x7d\x30"
#define ESC_EXPAND_ON       "\x1b\x57\x31"
#define ESC_EXPAND_OFF      "\x1b\x57\x30"

#define ESC_BOLD_OFF        "\x1B\x45\x00"
#define ESC_BOLD_ON         "\x1B\x45\x01"
#define ESC_DOUBLEFONT_ON   "\x1D\x21\x11"
#define ESC_DOUBLEFONT_OFF  "\x1D\x21\x00"
#define ESC_ALIGNLEFT       "\x1B\x61\x00"
#define ESC_ALIGNCENTER     "\x1B\x61\x01"
#define ESC_FULLCUT         "\x1D\x56\x00"
#define ESC_PARTIALCUT      "\x1D\x56\x01"
#define ESC_EXPANDON        "\x1D\x21\x11"
#define ESC_EXPANDOFF       "\x1D\x21\x00"

//GS H
#define ESC_BARCODE_HRI_OFF "\x1d\x48\x00"
//GS H
#define ESC_BARCODE_HRI_ON  "\x1d\x48\x01"
 //GS h
#define ESC_BARCODE_HEIGHT  "\x1d\x68"
//GS k
#define ESC_BARCODE_CODE39  "\x1d\x6b\x45"
//GS k
#define ESC_BARCODE_CODE93  "\x1d\x6b\x48"

//Reset
#define ESC_RESET "\x1d\xf8\x46"

#define TABLE_WIDTH           48
#define TABLE_MAX_VALUE       10000
#define TABLE_FULL_LINE "________________________________________________"

class EscPos
{
public:
  EscPos();
  void str(const QString& str);
  QByteArray m_ba;

  void bold(bool b);
  void doublefont(bool b);
  void align(bool center = false);
  void cut(bool partial = false);
  void expand(bool b);

private:
  void init();
  void boldESCPOS(bool b);
  void doublefontESCPOS(bool b);
  void expandESCPOS(bool b);
  void alignESCPOS(bool center);
  void cutESCPOS(bool partial);

  void boldBEMA(bool b);
  void doublefontBEMA(bool b);
  void alignBEMA(bool center);
  void cutBEMA(bool partial);
  void expandBEMA(bool b);
  void tabBEMA();

  int m_printerModel;
};

#endif // ESC_H
