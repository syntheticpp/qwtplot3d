# This file will be straightened in the future, adapt to your needs
# A recent qwt is needed (>=0.4.2 with installed Qwt designer plugin)
# otherwise you have to include some qwt headers in the uic generated
# files manually
#

TARGET          = Mesh2
win32:TEMPLATE        = vcapp
CONFIG         += qt warn_on thread
MOC_DIR         = moc
OBJECTS_DIR     = obj 
INCLUDEPATH    += ../../src 

win32:LIBS     += ../../lib/qwtplot3d.lib opengl32.lib glu32.lib
unix:LIBS += -lqwtplot3d -lGLU -lGL -L/home/micha/programming/lib

linux-g++:QMAKE_CXXFLAGS += -fno-exceptions
win32:QMAKE_CXXFLAGS     += -GX 

FORMS =  \
		src/mesh2mainwindowbase.ui \

SOURCES =  \
		src/main.cpp \
		src/mesh2mainwindow.cpp \

HEADERS =  \
		src/mesh2mainwindow.h \
		src/functions.h

