# pro file for building the makefile for qwtplot3d
#

include (qwtplot3d.pri)

TARGET            = qwtplot3d
TEMPLATE          = lib
CONFIG           += qt debug warn_on thread 
QT               += opengl
#zlib
MOC_DIR           = tmp
OBJECTS_DIR       = tmp
INCLUDEPATH       = include
#DESTDIR      			= ./lib
DESTDIR      			= ./../../../../lib

win32 {
  TEMPLATE    = vclib
  CONFIG     += dll exceptions
  dll:DEFINES    += QT_DLL QWT3D_DLL QWT3D_MAKEDLL
  QMAKE_CXXFLAGS     += $$QMAKE_CFLAGS_STL
# Comment the next line, if you have zlib on your windows system
# CONFIG -= zlib
}

linux-g++:TMAKE_CXXFLAGS += -fno-exceptions
unix:VERSION = 0.3.0


# zlib support for gl2ps
zlib {
  DEFINES += GL2PS_HAVE_ZLIB
  win32:LIBS += zlib.lib
	unix:LIBS  += -lz
}