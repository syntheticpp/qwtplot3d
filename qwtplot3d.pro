# pro file for building the makefile for qwtplot3d
#
# Qt <  3.x: tmake qwt.pro -o Makefile
# Qt >= 3.x: qmake qwt.pro 
#

TARGET            = qwtplot3d
TEMPLATE          = lib
#VERSION           = 0.1.3
CONFIG           += qt warn_on thread
MOC_DIR           = moc
OBJECTS_DIR       = obj
INCLUDEPATH       = include
DEPENDPATH        = include src

unix:DESTDIR      = lib
win32:DESTDIR      = ../../lib

win32:TEMPLATE    = vclib
win32:CONFIG     += dll 
win32:DEFINES    += QT_DLL QWT3D_DLL QWT3D_MAKEDLL
win32:LIBS     += opengl32.lib glu32.lib
 
win32:QMAKE_CXXFLAGS     += -GX 
linux-g++:TMAKE_CXXFLAGS += -fno-exceptions

# Input
SOURCES += src/axis.cpp \
           src/colorgenerator.cpp \
           src/coordsys.cpp \
           src/dataviews.cpp \
           src/dataviews_cell.cpp \
           src/dataviews_grid.cpp \
           src/drawable.cpp \
           src/functiongenerator.cpp \
           src/labelpixmap.cpp \
           src/mapping.cpp \
           src/mousekeyboard.cpp \
           src/movements.cpp \
           src/colorlegend.cpp \
           src/plot3d.cpp \
           src/reader.cpp \
           src/surfaceplot.cpp \
           src/types.cpp \
           src/vectorfield.cpp \
           src/autoscaler.cpp \
           src/text.cpp

HEADERS += include/qwt3d_color.h \
           include/qwt3d_function.h \
           include/qwt3d_global.h \
           include/qwt3d_io.h \
           include/qwt3d_surfaceplot.h \
           include/qwt3d_vectorfield.h \
           include/qwt3d_types.h \
					 include/qwt3d_axis.h \
           include/qwt3d_coordsys.h \
           include/qwt3d_drawable.h \
           include/qwt3d_femreader.h \
           include/qwt3d_helper.h \
           include/qwt3d_labelpixmap.h \
           include/qwt3d_openglhelper.h \
           include/qwt3d_colorlegend.h \
           include/qwt3d_plot.h \
           include/qwt3d_reader.h \
           include/qwt3d_autoscaler.h \
           include/qwt3d_text.h 
