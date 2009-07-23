# pro file for building the makefile for qwtplot3d
#

include (qwtplot3d.pri)

CONFIG          += qt warn_on opengl thread zlib
MOC_DIR          = tmp
OBJECTS_DIR      = tmp
INCLUDEPATH      = include
DEPENDPATH       = include src
QT              += opengl

#win32:CONFIG    += static
win32:CONFIG    += dll
win32:CONFIG    += exceptions

win32:dll:DEFINES    += QT_DLL QWT3D_DLL QWT3D_MAKEDLL
win32:QMAKE_CXXFLAGS += $$QMAKE_CFLAGS_STL

# Comment the next line, if you have zlib on your windows system
win32:CONFIG    -= zlib

# Comment the next line, if you do not want debug message output
#DEFINES -= QT_NO_DEBUG_OUTPUT

linux-g++:TMAKE_CXXFLAGS += -fno-exceptions
unix:VERSION     = 0.3.0

CONFIG(debug, debug|release) {
    BUILD_SUBDIR = lib/debug
} else {
    BUILD_SUBDIR = lib/release
}

win32 {
    win32-g++ {
        DESTDIR  = $$BUILD_SUBDIR
    } else {
        DESTDIR  = $$BUILD_SUBDIR/VisualStudio
        TEMPLATE = vclib
    }

    OBJECTS_DIR  = $$DESTDIR
    RCC_DIR      = $$DESTDIR/tmp
    MOC_DIR      = $$DESTDIR/tmp
}

unix { 
    linux-g++ {
        EXTN_SUBDIR = linux
    } else { 
        EXTN_SUBDIR = unix
    }
    DESTDIR      = $$BUILD_SUBDIR/$$EXTN_SUBDIR

    OBJECTS_DIR  = $$DESTDIR
    RCC_DIR      = $$DESTDIR/tmp
    MOC_DIR      = $$DESTDIR/tmp
}

# zlib support for gl2ps
zlib {
  DEFINES += GL2PS_HAVE_ZLIB
  win32:LIBS += zlib.lib
	unix:LIBS  += -lz
}

# install
target.path = lib
INSTALLS += target
