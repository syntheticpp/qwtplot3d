TARGET          = Mesh2
win32:TEMPLATE        = vcapp
CONFIG         += qt warn_on thread
MOC_DIR         = moc
OBJECTS_DIR     = obj 
INCLUDEPATH    += ../../src 
#DESTDIR         = v:\lib

win32:LIBS     += ../../lib/qwtplot3d.lib qwt.lib opengl32.lib glu32.lib
unix:LIBS += -lqwtplot3d -lqwt -lGLU -lGL -L/home/micha/programming/lib
unix:INCLUDEPATH += /usr/include/qwt ${HOME}/programming/include/qwt

linux-g++:QMAKE_CXXFLAGS += -fno-exceptions
win32:QMAKE_CXXFLAGS     += -GX 

FORMS =  \
		src/mesh2mainwindowbase.ui \
		src/alphadlg.ui

SOURCES =  \
		src/main.cpp \
		src/mesh2mainwindow.cpp \
		src/alphadlgimpl.cpp \
		src/functions.cpp

HEADERS =  \
		src/mesh2mainwindow.h \
		src/alphadlgimpl.h \
		src/functions.h
		
RC_FILE = myapp.rc		

