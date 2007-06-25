include( ../common.pro )

SOURCES = src/main.cpp \
		src/mesh2mainwindow.cpp \
		src/colormapreader.cpp \
		src/lightingdlg.cpp 

HEADERS =  src/mesh2mainwindow.h \
		src/functions.h \
		src/colormapreader.h \
		src/lightingdlg.h 

SOURCES += src/designerworkaround.cpp
HEADERS += src/designerworkaround.h 
FORMS =  src/mesh2mainwindowbase.ui \
         src/lightingdlgbase.ui

