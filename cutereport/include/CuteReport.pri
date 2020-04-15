INCLUDEPATH += "C:/Program Files (x86)/ExaroLogic/CuteReport/development/include/cutereport" 
LIBS += -L"C:/Program Files (x86)/ExaroLogic/CuteReport/development" 
win32: CONFIG(debug, debug|release): LIBS += -lCuteReportCored -lCuteReportWidgetsd 
else: LIBS += -lCuteReportCore -lCuteReportWidgets 
