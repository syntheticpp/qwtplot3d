#include "qwt3d_surfaceplot.h"
#include "qwt3d_function.h"

using namespace Qwt3D;

Function::Function()
{
	init(0);
}

Function::Function(SurfacePlot* pw)
{
	init(pw);
}

void 
Function::init(SurfacePlot* plotWidget)
{
	plotwidget_ = plotWidget;
	setMesh(0,0);
	setDomain(0,0,0,0);
	setMinZ(-DBL_MAX);
	setMaxZ(DBL_MAX);
}

Function::~Function()
{ 
}

void
Function::assign(SurfacePlot* plotWidget)
{
	Q_ASSERT(plotWidget);
	if (plotWidget && (plotWidget != plotwidget_))
		plotwidget_ = plotWidget;
}

void
Function::setMesh(unsigned int columns,unsigned int rows)
{
	xmesh_ = columns;
	ymesh_ = rows;
}

void
Function::	setDomain(double minx, double maxx, double miny, double maxy)
{
	minx_ = minx;
	maxx_ = maxx;
	miny_ = miny;
	maxy_ = maxy;
}

void
Function::	setMinZ(double val)
{
	minz_ = val;
}

void
Function::	setMaxZ(double val)
{
	maxz_ = val;
}

/**
For plotWidget != 0 the function permanently assigns her argument (In fact, assign(plotWidget) is called)
*/
bool 
Function::create(SurfacePlot* plotWidget)
{
	if (plotWidget)
		assign(plotWidget);

	if ((xmesh_<=2) || (ymesh_<=2) || !plotwidget_)
		return false;
	
	/* allocate some space for the mesh */
 	double** data         = new double* [xmesh_] ;

	unsigned i,j;
	for ( i = 0; i < xmesh_; i++) 
	{
		data[i]         = new double [ymesh_];
	}
	
	/* get the data */

	double dx = (maxx_ - minx_) / (xmesh_ - 1);
	double dy = (maxy_ - miny_) / (ymesh_ - 1);
	
	for (i = 0; i < xmesh_; ++i) 
	{
		for (j = 0; j < ymesh_; ++j) 
		{
			data[i][j] = operator()(minx_ + i*dx, miny_ + j*dy);
			
			if (data[i][j] > maxz_)
				data[i][j] = maxz_;
			else if (data[i][j] < minz_)
				data[i][j] = minz_;
		}
	}

	Q_ASSERT(plotwidget_);
	if (!plotwidget_)
	{
		fprintf(stderr,"Function: no valid Plot3D Widget assigned");
	}
	else
	{
		plotwidget_->createDataRepresentation(data, xmesh_, ymesh_, minx_, maxx_, miny_, maxy_);
	}

	for ( i = 0; i < xmesh_; i++) 
	{
		delete [] data[i];
	}

	delete data;

	return true;
}

