include( ../common.pro )

CONFIG += opengl

FORMS =  src/mesh2mainwindowbase.ui \
         src/lightingdlgbase.ui

SOURCES = src/main.cpp \
		src/mesh2mainwindow.cpp \
		src/colormapreader.cpp \
		src/lightingdlg.cpp \
		src/mesh.cpp

HEADERS =  src/mesh2mainwindow.h \
		src/functions.h \
		src/colormapreader.h \
		src/lightingdlg.h \
		src/femreader.h
