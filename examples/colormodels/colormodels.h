#ifndef AUTOSWITCH_H__
#define AUTOSWITCH_H__

#include <qtimer.h>
#include <qwt_plot3d.h>
#include "../src/functiongenerator.h"

using namespace Qwt3D;

class Saddle : public Function
{
public:

	Saddle(Plot3D* pw)
	:Function(pw)
	{
		setDomain(-2.5,2.5,-2.5,2.5);
		setMaxZ(1.5);
		setMinZ(-1.5);
		setMesh(31,31);
	}

	double operator()(double x, double y)
	{
		return x*y;
	}

	QString name() const { return QString("Rosenbrock"); }
};


class Plot : public Plot3D
{
	Q_OBJECT

public:
    Plot(QWidget* pw = 0);

public slots:
	void changeColor();

};

#endif