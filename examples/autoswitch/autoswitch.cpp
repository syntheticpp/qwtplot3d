#include <qapplication.h>
#include <qsplitter.h>
#include <qtimer.h>

#include "autoswitch.h"


//--------------------------------------------------------------------
//              autoswitch.cpp
//
//      Demonstrates autoswitching axes with a cutted saddle as data
//--------------------------------------------------------------------

Plot::Plot(QWidget* pw, int updateinterval)
:QwtPlot3D(pw)
{
	setRotation(30,0,15);
	setScale(1,1,1);
	setShift(0.1,0,0);
	setZoom(0.8);
	coordinates()->setNumberFont("Courier",8);

	for (unsigned i=0; i!=coordinates()->axes.size(); ++i)
	{
		coordinates()->axes[i].setMajors(7);
		coordinates()->axes[i].setMinors(4);
	}
	
	
	coordinates()->axes[Qwt3d::X1].setLabelString("x");
	coordinates()->axes[Qwt3d::Y1].setLabelString("y");
	coordinates()->axes[Qwt3d::Z1].setLabelString("z"); 
	coordinates()->axes[Qwt3d::X2].setLabelString("x");
	coordinates()->axes[Qwt3d::Y2].setLabelString("y");
	coordinates()->axes[Qwt3d::Z2].setLabelString("z"); 
	coordinates()->axes[Qwt3d::X3].setLabelString("x");
	coordinates()->axes[Qwt3d::Y3].setLabelString("y");
	coordinates()->axes[Qwt3d::Z3].setLabelString("z"); 
	coordinates()->axes[Qwt3d::X4].setLabelString("x");
	coordinates()->axes[Qwt3d::Y4].setLabelString("y");
	coordinates()->axes[Qwt3d::Z4].setLabelString("z"); 


  QTimer* timer = new QTimer( this );
	connect( timer, SIGNAL(timeout()), this, SLOT(rotate()) );

	timer->start(updateinterval);

}

void Plot::rotate()
{
	int prec = 3;
		
	setRotation(
			(int(prec*xRotation() + 1) % (360*prec))/double(prec),
			(int(prec*yRotation() + 1) % (360*prec))/double(prec),
			(int(prec*zRotation() + 1) % (360*prec))/double(prec)
			);
}


using namespace Qwt3d;

int main(int argc, char **argv)
{
    QApplication a(argc, argv);
		QSplitter* spl = new QSplitter(QSplitter::Horizontal);

		Plot* plot1 = new Plot(spl,30);
		plot1->setFloorStyle(FLOORISO);
		plot1->setCoordinateStyle(BOX);
		Saddle saddle(plot1);
		saddle.create();
		plot1->setTitle("Autoswitching axes");
		plot1->setBackgroundColor(RGBA(1,1, 157./255));


		Plot* plot2 = new Plot(spl,80);
		plot2->setPlotStyle(FILLED);
		plot2->setFloorStyle(FLOORISO);
		plot2->setCoordinateStyle(FRAME);
		plot2->setZoom(0.8);
		Hat hat(plot2);
		hat.create();
		plot2->setBackgroundColor(RGBA(1,1, 157./255));


		a.setMainWidget(spl);
		spl->resize(800,400);
    spl->show();
    return a.exec(); 
}
