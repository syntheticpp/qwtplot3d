TARGET       = simpleplot
TEMPLATE     = app
CONFIG      += qt warn_on debug thread
MOC_DIR      = moc
OBJECTS_DIR  = obj
INCLUDEPATH    += ../../src 

unix:LIBS += -lqwtplot3d -lqwt -lGLU -lGL -L/home/micha/programming/lib
unix:INCLUDEPATH += /usr/include/qwt ${HOME}/programming/include/qwt

win32:TEMPLATE  = vcapp
win32:DEFINES  += QT_DLL QWT_DLL
win32:LIBS     += ../../lib/qwtplot3d.lib qwt.lib opengl32.lib glu32.lib

linux-g++:QMAKE_CXXFLAGS += -fno-exceptions
win32:QMAKE_CXXFLAGS     += -GX 

SOURCES = simpleplot.cpp
