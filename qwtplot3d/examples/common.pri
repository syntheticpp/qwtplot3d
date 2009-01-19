TEMPLATE     = app
CONFIG      += qt warn_on thread release
UI_DIR       = tmp
MOC_DIR      = tmp
OBJECTS_DIR  = tmp
INCLUDEPATH += ../../include 
DEPENDPATH   = $$INCLUDEPATH
DESTDIR      = ../bin

#unix:LIBS += -lqwtplot3d -L../../lib
unix:LIBS += ../../lib/libqwtplot3d.a
linux-g++:QMAKE_CXXFLAGS += -fno-exceptions

CONFIG(debug, debug|release) {
  DESTDIR = ../bin/debug
} else {
  DESTDIR = ../bin/release
}

win32 {
  win32-g++ {
    LIBS += ../../lib/qwtplot3d.dll
  } else {
    LIBS += ../../lib/qwtplot3d.lib
    TEMPLATE  = vcapp
    CONFIG(debug, debug|release) {
      QMAKE_LFLAGS += /NODEFAULTLIB:msvcrt
    }
  }
  DEFINES  += QT_DLL QWT3D_DLL
  RC_FILE = ../icon.rc
}

MYVERSION = $$[QMAKE_VERSION] 
ISQT4 = $$find(MYVERSION, ^[2-9])

!isEmpty( ISQT4 ) {
  RESOURCES     = ../images.qrc
  QT += opengl
}

isEmpty( ISQT4 ) {
  CONFIG += opengl
}
