#include <float.h>
#include "types.h"
#include "functiongenerator.h"

Function::Function()
:DataSource()
{
}

Function::~Function()
{ 
}

bool 
Function::createData(Data& res)
{
	if ((xmesh_<=2) || (ymesh_<=2))
		return false;

	res.setSize(xmesh_, ymesh_);
	res.setMin(DBL_MAX);
	res.setMax(-DBL_MAX);
	
	/* allocate some space for the mesh */
 	GLfloat** data         = new GLfloat* [res.columns()] ;

	unsigned i,j;
	for ( i = 0; i < res.columns(); i++) 
	{
		data[i]         = new GLfloat [res.rows()];
	}
	
	/* get the data */

	double dx = (maxx_ - minx_) / (res.columns() - 1);
	double dy = (maxy_ - miny_) / (res.rows() - 1);
	
	for (i = 0; i < res.columns(); ++i) 
	{
		for (j = 0; j < res.rows(); ++j) 
		{
			data[i][j] = operator()(minx_ + i*dx, miny_ + j*dy);
			/* find the maximum data value */
			if (res.maximum() < data[i][j]) 
				res.setMax(data[i][j]);

			/* find the minimum data value */
			if (res.minimum() > data[i][j]) 
				res.setMin(data[i][j]);
		}
	}

	convert2VertexData(res, data);

	return true;
}

