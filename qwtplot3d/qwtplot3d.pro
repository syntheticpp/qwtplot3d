##############################################
# QwtPlot3D Widget Library
# Copyright (C) 2003   Micha Bieber
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the LPGL (GNU LESSER GENERAL PUBLIC LICENSE)
##############################################

# pro file for building the makefile ord VC6 project file
#
# Unixes etc.
# Qt <  3.x: tmake qwtplot3d.pro -o Makefile
# Qt >= 3.x: qmake qwtplot3d.pro 
#
# Windows -> dsp.bat 
#
# tested with Qt 3.05 (Win) 3.1 (Debian)

TARGET          = qwtplot3d
TEMPLATE        = lib
VERSION					= 0.1
CONFIG         += qt warn_on thread debug
MOC_DIR         = moc
OBJECTS_DIR     = obj 

DESTDIR = lib

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
					 src/mapping.cpp \
					 src/mousekeyboard.cpp \
					 src/movements.cpp \
					 src/dataviews.cpp
