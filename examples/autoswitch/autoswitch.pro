TARGET       = autoswitch
TEMPLATE     = app
CONFIG      += qt warn_on thread
MOC_DIR      = moc
OBJECTS_DIR  = obj
INCLUDEPATH    += ../../include 

unix:LIBS += -lqwtplot3d -lGLU -lGL -L/home/micha/programming/lib

win32:TEMPLATE  = vcapp
win32:LIBS     += qwtplot3d.lib opengl32.lib glu32.lib

linux-g++:QMAKE_CXXFLAGS += -fno-exceptions
win32:QMAKE_CXXFLAGS     += -GX 

SOURCES = autoswitch.cpp
HEADERS = autoswitch.h
