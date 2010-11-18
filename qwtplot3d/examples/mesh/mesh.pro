include( ../common.pro )

SOURCES = src/main.cpp \
		src/meshmainwindow.cpp \
		src/colormapreader.cpp \
		src/lightingdlg.cpp 

HEADERS =  src/meshmainwindow.h \
		src/functions.h \
		src/colormapreader.h \
		src/lightingdlg.h 

SOURCES += src/designerworkaround.cpp
HEADERS += src/designerworkaround.h 
FORMS =  src/meshmainwindowbase.ui \
         src/lightingdlgbase.ui

