/********************************************************************
    created:   2003/09/10
    filename:  main.cpp
	
    author:    Micha Bieber	
*********************************************************************/

#include <qapplication.h>
#include <qgl.h>
#include "printmainwindow.h"


int main( int argc, char** argv )
{
	QApplication app( argc, argv );
	
  if ( !QGLFormat::hasOpenGL() ) 
	{
		qWarning( "This system has no OpenGL support. Exiting." );     
		return -1;
  }

	printMainWindow mainwindow;
	
	app.setMainWidget(&mainwindow);

	mainwindow.show();

	return app.exec();
}