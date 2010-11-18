TEMPLATE     = app
CONFIG      += qt warn_on thread debug
QT += opengl
UI_DIR = tmp
MOC_DIR      = tmp
OBJECTS_DIR  = tmp
INCLUDEPATH    += ../../include 
DEPENDPATH	= $$INCLUDEPATH
DESTDIR = ../bin

win32{
  win32-msvc2008{
    !build_pass {
       message(Ignore warnings regarding parsing problems for /MP switch)
    }
    QMAKE_CXXFLAGS += /MP
  }
  
  contains (CONFIG, debug)  {
	  DESTDIR = ../bin/debug
	}
	!contains (CONFIG, release)  {
	  DESTDIR = ../bin/release
	}
}

unix:LIBS += -lqwtplot3d -L../../lib
win32:LIBS += ../../lib/qwtplot3d.lib

win32:TEMPLATE  = vcapp
win32:DEFINES  += QT_DLL QWT3D_DLL

#win32:QMAKE_CXXFLAGS     += -GX 
linux-g++:QMAKE_CXXFLAGS += -fno-exceptions

RC_FILE = ../icon.rc
RESOURCES     = ../images.qrc
