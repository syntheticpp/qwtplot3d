TARGET          = print
TEMPLATE = app
win32:TEMPLATE        = vcapp
CONFIG         += qt warn_on thread
MOC_DIR         = moc
OBJECTS_DIR     = obj 
INCLUDEPATH    += ../../include 

win32:LIBS     += qwtplot3d.lib opengl32.lib glu32.lib
unix:LIBS += -lqwtplot3d -lGLU -lGL -L/home/micha/programming/lib

linux-g++:QMAKE_CXXFLAGS += -fno-exceptions
win32:QMAKE_CXXFLAGS     += -GX 


# Input

FORMS =  src/printmainwindowbase.ui

SOURCES =  src/main.cpp \
		       src/printmainwindow.cpp 

HEADERS =  src/printmainwindow.h 

