#include <qapplication.h>
#include <qwt_plot3d.h>
#include "../src/functiongenerator.h"
#include <math.h>

//-----------------------------------------------------------------
//              simpleplot.cpp
//
//      A simple example which shows how to use QwtPlot3D
//-----------------------------------------------------------------

using namespace Qwt3d;

class Rosenbrock : public Function
{
public:

	Rosenbrock(QwtPlot3D* pw)
	:Function(pw)
	{
		setMinZ(-100);
	}

	double operator()(double x, double y)
	{
		return log((1-x)*(1-x) + 100 * (y - x*x)*(y - x*x)) / 8;
	}

	QString name() const { return QString("Rosenbrock"); }
	QString formula() const { return QString("log((1-x)*(1-x) + 100 * (y - x*x)*(y - x*x))"); }
};


class Plot : public QwtPlot3D
{
public:
    Plot();
};


Plot::Plot()
{
  setTitle("A Simple QwtPlot3D Demonstration");
	Rosenbrock rosenbrock(this);
	
	rosenbrock.setMesh(41,31);
	rosenbrock.setDomain(-1.73,1.5,-1.5,1.5);
	rosenbrock.setMinZ(-10);
	
	rosenbrock.create();

	setRotation(30,0,15);
	setScale(1,1,1);
	setShift(0.15,0,0);
	setZoom(1);

	for (unsigned i=0; i!=coordinates()->axes.size(); ++i)
	{
		coordinates()->axes[i].setMajors(7);
		coordinates()->axes[i].setMinors(4);
	}
	
	
	coordinates()->axes[X1].setLabelString("x-axis");
	coordinates()->axes[Y1].setLabelString("y-axis");
	coordinates()->axes[Z1].setLabelString(QChar (0x38f)); // Omega - see http://www.unicode.org/charts/

	//The following code part is clumsy, it will be integrated in the coordinate systems style in the future

	Triple first = coordinates()->first();
	Triple second = coordinates()->second();

	coordinates()->axes[X1].setLimits(first.x, second.x);
	coordinates()->axes[Y1].setLimits(first.y, second.y);
	coordinates()->axes[Z1].setLimits(first.z, second.z);

	coordinates()->axes[X1].setScale(true);
	coordinates()->axes[Y1].setScale(true);
	coordinates()->axes[Z1].setScale(true);

	coordinates()->axes[X1].setNumbers(true);
	coordinates()->axes[Y1].setNumbers(true);
	coordinates()->axes[Z1].setNumbers(true);
	
	coordinates()->axes[X1].setNumberAnchor(LabelPixmap::TopCenter);
	coordinates()->axes[Y1].setNumberAnchor(LabelPixmap::CenterRight);
	coordinates()->axes[Z1].setNumberAnchor(LabelPixmap::CenterRight);

//	setFloorStyle(FLOORDATA);
	setCoordinateStyle(BOX);
	
	updateData();
	updateCoordinates();
}

int main(int argc, char **argv)
{
    QApplication a(argc, argv);
    Plot plot;
    a.setMainWidget(&plot);
		plot.resize(800,600);
    plot.show();
    return a.exec(); 
}
