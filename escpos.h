#ifndef ESC_H
#define ESC_H

#include <QString>

#define ESC                 "\x1b"
#define ESC_ALIGN_CENTER    "\x1b\x61\x31"
#define ESC_ALIGN_LEFT      "\x1b\x61\x30"
#define ESC_FULL_CUT        "\x1b\x77"
#define ESC_PARTIAL_CUT     "\x1b\x6d"
#define ESC_STRESS_ON       "\x1b\x45"
#define ESC_STRESS_OFF      "\x1b\x46"
#define ESC_LF              "\n"
#define ESC_VERT_TAB        "\x1b\x4a\x40"
#define ESC_INIT            "\x02\x1b\x40\x02"
#define ESC_DOUBLE_FONT_ON  "\x1b\x64\x31\x1b\x57\x31"
#define ESC_DOUBLE_FONT_OFF "\x1b\x64\x30\x1b\x57\x30"
#define ESC_CLEAR           "\x02"
#define ESC_PORTUGUESE      "\x1b\x74\x08"
#define ESC_CODEPAGE850     "\x1b\x74\x02"
#define ESC_REVERSE_ON      "\x1b\x7d\x31"
#define ESC_REVERSE_OFF     "\x1b\x7d\x30"
#define ESC_EXPAND_ON       "\x1b\x57\x31"
#define ESC_EXPAND_OFF      "\x1b\x57\x30"

#define ESC_BARCODE_HRI_OFF "\x1d\x48\x30"
#define ESC_BARCODE_HRI_ON  "\x1d\x48\x31"
#define ESC_BARCODE_HEIGHT  "\x1d\x68"
#define ESC_BARCODE_CODE39  "\x1d\x6b\x45"
#define ESC_BARCODE_CODE93  "\x1d\x6b\x48"

#define TABLE_WIDTH           48
#define TABLE_MAX_VALUE       10000
#define TABLE_FULL_LINE "________________________________________________"

#endif // ESC_H
