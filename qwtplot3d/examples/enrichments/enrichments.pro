include( ../common.pro )

CONFIG += opengl

FORMS =  src/enrichmentmainwindowbase.ui

SOURCES =  src/main.cpp \
		       src/enrichmentmainwindow.cpp \
		       src/enrichments.cpp 
		       #src/enrichments2.cpp

HEADERS =  src/enrichmentmainwindow.h \
					 src/enrichments.h 
 		       #src/enrichments2.h 
	 

