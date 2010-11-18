system("qmake -project -nopwd -o qwtplot3d.pri -r ./src ./include")

include (qwtplot3d.pri)

TARGET            = qwtplot3d
TEMPLATE          = lib
CONFIG           += qt debug warn_on thread 
QT               += opengl
#zlib
MOC_DIR           = tmp
OBJECTS_DIR       = tmp
INCLUDEPATH       = include
DESTDIR      			= ./lib
#DESTDIR      			= ./../../../../../lib

win32 {
  !build_pass {
    win32-msvc | win32-msvc2002 {
      error(Unsupported Visual Studio version ( < 2003 ))
    }
  }
  
  win32-msvc2003 | win32-msvc2005 | win32-msvc2008  {
    TEMPLATE    = vclib
    CONFIG     += dll exceptions
    dll:DEFINES    += QT_DLL QWT3D_DLL QWT3D_MAKEDLL
    QMAKE_CXXFLAGS += $$QMAKE_CFLAGS_STL
    win32-msvc2008{
      !build_pass {
         message(Ignore warnings regarding parsing problems for /MP switch)
      }
      QMAKE_CXXFLAGS += /MP
    }
  }    
}

linux-g++:QMAKE_CXXFLAGS += -fno-exceptions
unix:VERSION = 0.3.0
