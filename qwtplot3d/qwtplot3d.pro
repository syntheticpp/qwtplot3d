# pro file for building the makefile for qwtplot3d
#
# Qt <  3.x: tmake qwt.pro -o Makefile
# Qt >= 3.x: qmake qwt.pro 
#


TARGET            = qwtplot3d
TEMPLATE          = lib
#VERSION           = 0.1.4
CONFIG           += qt warn_on thread gl2ps
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
SOURCES += src/qwt3d_axis.cpp \
           src/qwt3d_color.cpp \
           src/qwt3d_coordsys.cpp \
           src/qwt3d_dataviews.cpp \
           src/qwt3d_dataviews_cell.cpp \
           src/qwt3d_dataviews_grid.cpp \
           src/qwt3d_drawable.cpp \
           src/qwt3d_function.cpp \
           src/qwt3d_labelpixmap.cpp \
           src/qwt3d_mapping.cpp \
           src/qwt3d_mousekeyboard.cpp \
           src/qwt3d_movements.cpp \
           src/qwt3d_colorlegend.cpp \
           src/qwt3d_plot.cpp \
           src/qwt3d_reader.cpp \
           src/qwt3d_surfaceplot.cpp \
           src/qwt3d_types.cpp \
           src/qwt3d_vectorfield.cpp \
           src/qwt3d_autoscaler.cpp 

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
           include/qwt3d_autoscaler.h


#gl2ps support
HEADERS+=include/qwt3d_gl2ps.h
SOURCES+=src/qwt3d_gl2ps.cpp
gl2ps {
	INCLUDEPATH   += 3rdparty/gl2ps
	SOURCES	      += 3rdparty/gl2ps/gl2ps.c \
                   3rdparty/gl2ps/gl2pdf.c
  HEADERS       += 3rdparty/gl2ps/gl2ps.h \
                   3rdparty/gl2ps/gl2ps_p.h    
}

