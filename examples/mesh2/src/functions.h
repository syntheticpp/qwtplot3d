#ifndef __EXAMPLE_H__
#define __EXAMPLE_H__

#include <math.h>
#include "../../../include/qwt3d_surfaceplot.h"
#include "../../../include/qwt3d_function.h"

using namespace Qwt3D;


class Rosenbrock : public Function
{
public:

	Rosenbrock(SurfacePlot* pw)
	:Function(pw)
	{
	}

	double operator()(double x, double y)
	{
		return log((1-x)*(1-x) + 100 * (y - x*x)*(y - x*x)) / 8;
	}
};

class Hat : public Function
{
public:

	Hat(SurfacePlot* pw)
	:Function(pw)
	{
	//	setMaxZ(0.8);     
	}
	
	double operator()(double x, double y)
	{
		return 1.0 / (x*x+y*y+0.5);
	}
};

class Saddle : public Function
{
public:

	Saddle()
	:Function()
	{
	//	setMaxZ(0.8);     
	}
	
	double operator()(double x, double y)
	{
		return x*x - y*y;
	}
};

class Mex : public Function
{
public:

	Mex()
	:Function()
	{
	//	setMaxZ(0.8);     
	}
	
	double operator()(double x, double y)
	{
		double n = sqrt(x*x+y*y); 

		if (n < DBL_MIN)
			return 20;
		
		return 20 * sin(sqrt(x*x+y*y)) / n;
	}
};

#endif // __EXAMPLE_H__
