TEMPLATE     = app
CONFIG      += qt warn_on thread
MOC_DIR      = tmp
OBJECTS_DIR  = tmp
INCLUDEPATH    += ../../include 
DEPENDPATH	= $$INCLUDEPATH

unix:LIBS += -lqwtplot3d -L./../../lib
win32:LIBS += ./../../lib/qwtplot3d.lib

win32:TEMPLATE  = vcapp
win32:DEFINES  += QT_DLL QWT_DLL
win32:QMAKE_CXXFLAGS     += -GX 
linux-g++:QMAKE_CXXFLAGS += -fno-exceptions
