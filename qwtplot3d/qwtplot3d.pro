TARGET          = qwtplot3d
TEMPLATE        = lib
CONFIG         += qt warn_on thread debug
MOC_DIR         = moc
OBJECTS_DIR     = obj 

DESTDIR = lib

unix:QMAKESPEC = linux-g++
win32:TEMPLATE  = vclib

linux-g++:QMAKE_CXXFLAGS += -fno-exceptions
win32:QMAKE_CXXFLAGS     += -GX 

HEADERS += src/qwt_plot3d.h \
					 src/reader.h \
					 src/types.h \
					 src/labelpixmap.h \
					 src/axis.h \
					 src/coordsys.h \
					 src/drawable.h \
					 src/plane.h \
					 src/functiongenerator.h \
					 src/colorgenerator.h \
					 src/datasource.h

SOURCES += src/compares.cpp \
					 src/qwt_plot3d.cpp \
					 src/reader.cpp \
					 src/labelpixmap.cpp \
					 src/types.cpp \
					 src/axis.cpp \
					 src/coordsys.cpp \
					 src/drawable.cpp \
					 src/plane.cpp \
					 src/functiongenerator.cpp \
					 src/colorgenerator.cpp \
					 src/datasource.cpp
