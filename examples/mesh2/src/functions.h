#ifndef __EXAMPLE_H__
#define __EXAMPLE_H__

#include <math.h>
#include "../src/functiongenerator.h"

class QwtPlot3D;

class Rosenbrock : public Function
{
public:

	Rosenbrock(QwtPlot3D* pw)
	:Function(pw)
	{
	}

	double operator()(double x, double y)
	{
		return log((1-x)*(1-x) + 100 * (y - x*x)*(y - x*x)) / 8;
	}

	QString name() const { return QString("Rosenbrock"); }
};

class Hat : public Function
{
public:

	Hat(QwtPlot3D* pw)
	:Function(pw)
	{
	//	setMaxZ(0.8);     
	}
	
	double operator()(double x, double y)
	{
		return 1.0 / (x*x+y*y+0.5);
	}

	QString name() const { return QString("Hat"); }
};

class Gauss : public Function
{
public:

	Gauss(QwtPlot3D* pw)
	:Function(pw)
	{
	//	setMaxZ(0.8);     
	}
	
	double operator()(double x, double y)
	{
		return exp(-(x*x+y*y)/2);
	}

	QString name() const { return QString("Gauss"); }
};

#endif // __EXAMPLE_H__
