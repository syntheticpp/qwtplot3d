#include <qapplication.h>
#include <qsplitter.h>
#include <qtimer.h>

#include "autoswitch.h"


//--------------------------------------------------------------------
//              autoswitch.cpp
//
//      Demonstrates autoswitching axes with a cutted saddle as data
//--------------------------------------------------------------------

Plot::Plot(QWidget* pw)
:QwtPlot3D(pw)
{
	Saddle saddle(this);
	
	saddle.create();

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

	
	updateData();
	updateCoordinates();

  QTimer* timer = new QTimer( this );
	connect( timer, SIGNAL(timeout()), this, SLOT(rotate()) );

	timer->start(20);

}

void Plot::rotate()
{
		setRotation(
			int(xRotation() + 1) % 720,
			int(yRotation() + 1) % 720,
			int(zRotation() + 1) % 720
			);
}


using namespace Qwt3d;

int main(int argc, char **argv)
{
    QApplication a(argc, argv);
		QSplitter* spl = new QSplitter(QSplitter::Horizontal);

		Plot* plot1 = new Plot(spl);
		plot1->setFloorStyle(FLOORISO);
		plot1->setCoordinateStyle(BOX);
		plot1->setTitle("Autoswitching axes");
		plot1->setBackgroundColor(RGBA(1,220./255, 180./255));

		Plot* plot2 = new Plot(spl);
		plot2->setPlotStyle(WIREFRAME);
		plot2->setFloorStyle(FLOORDATA);
		plot2->setCoordinateStyle(FRAME);
		plot2->setBackgroundColor(RGBA(1,220./255, 180./255));

		a.setMainWidget(spl);
		spl->resize(800,400);
    spl->show();
    return a.exec(); 
}
