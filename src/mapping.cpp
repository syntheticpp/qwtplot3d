#if defined(_MSC_VER) /* MSVC Compiler */
#pragma warning ( disable : 4305 )
#pragma warning ( disable : 4786 )
#endif

#include "qwt3d_surfaceplot.h"

using namespace Qwt3D;


/*! 
	Convert user grid data to internal vertex structure.
	See also NativeReader::read() and Function::create()
*/
bool 
SurfacePlot::createDataRepresentation(double** data, unsigned int columns, unsigned int rows
																				, double minx, double maxx, double miny, double maxy)
{
	meshtype_ = GRID;
	
	int i,j;
	
	actualCellData_->clear();
	actualGridData_->setSize(columns,rows);
	
	double dx = (maxx - minx) / (actualGridData_->columns() - 1);
	double dy = (maxy - miny) / (actualGridData_->rows() - 1);

	double tmin = DBL_MAX;
	double tmax = -DBL_MAX;

	/* fill out the vertex array for the mesh. */
	for (i = 0; i != actualGridData_->columns(); ++i) 
	{
		for (j = 0; j != actualGridData_->rows(); ++j) 
		{
			actualGridData_->vertices[i][j][0] = minx + i*dx; 
			actualGridData_->vertices[i][j][1] = miny + j*dy;
			actualGridData_->vertices[i][j][2] = data[i][j];

			if (data[i][j] > tmax)
				tmax = data[i][j];
			if (data[i][j] < tmin)
				tmin = data[i][j];
 		}
	}

	// normals
	  
	Triple u, v, n;  // for cross product

	for (i = 1; i != actualGridData_->columns() - 1; ++i) 
	{
		for (j = 1; j != actualGridData_->rows() - 1; ++j) 
		{
			/*	get two vectors to cross */
      u = Triple(
									actualGridData_->vertices[i+1][j][0] - actualGridData_->vertices[i][j][0],
									actualGridData_->vertices[i+1][j][1] - actualGridData_->vertices[i][j][1],
									actualGridData_->vertices[i+1][j][2] - actualGridData_->vertices[i][j][2]
								);

      v = Triple(
									actualGridData_->vertices[i][j+1][0] - actualGridData_->vertices[i][j][0],
									actualGridData_->vertices[i][j+1][1] - actualGridData_->vertices[i][j][1],
									actualGridData_->vertices[i][j+1][2] - actualGridData_->vertices[i][j][2]
								);
      /* get the normalized cross product */ 
      n = normalizedcross(u,v); // right hand system here !
      
      u = Triple(
									actualGridData_->vertices[i][j+1][0] - actualGridData_->vertices[i][j][0],
									actualGridData_->vertices[i][j+1][1] - actualGridData_->vertices[i][j][1],
									actualGridData_->vertices[i][j+1][2] - actualGridData_->vertices[i][j][2]
								);
			v = Triple(
									actualGridData_->vertices[i-1][j][0] - actualGridData_->vertices[i][j][0],
									actualGridData_->vertices[i-1][j][1] - actualGridData_->vertices[i][j][1],
									actualGridData_->vertices[i-1][j][2] - actualGridData_->vertices[i][j][2]
								);

      n += normalizedcross(u,v); 


      u = Triple(
									actualGridData_->vertices[i-1][j][0] - actualGridData_->vertices[i][j][0],
									actualGridData_->vertices[i-1][j][1] - actualGridData_->vertices[i][j][1],
									actualGridData_->vertices[i-1][j][2] - actualGridData_->vertices[i][j][2]
								);

      v = Triple(
									actualGridData_->vertices[i][j-1][0] - actualGridData_->vertices[i][j][0],
									actualGridData_->vertices[i][j-1][1] - actualGridData_->vertices[i][j][1],
									actualGridData_->vertices[i][j-1][2] - actualGridData_->vertices[i][j][2]
								);
			
      n += normalizedcross(u,v); 

      u = Triple(
									actualGridData_->vertices[i][j-1][0] - actualGridData_->vertices[i][j][0],
									actualGridData_->vertices[i][j-1][1] - actualGridData_->vertices[i][j][1],
									actualGridData_->vertices[i][j-1][2] - actualGridData_->vertices[i][j][2]
								);

      v = Triple(
									actualGridData_->vertices[i+1][j][0] - actualGridData_->vertices[i][j][0],
									actualGridData_->vertices[i+1][j][1] - actualGridData_->vertices[i][j][1],
									actualGridData_->vertices[i+1][j][2] - actualGridData_->vertices[i][j][2]
								);

      n += normalizedcross(u,v);
			n.normalize();

			actualGridData_->normals[i][j][0] = n.x;
			actualGridData_->normals[i][j][1] = n.y;
			actualGridData_->normals[i][j][2] = n.z;
		}
	} 
	
  /* fill in the normals on the top/bottom edge of the mesh (simply
     copy the one below/above it). */
  for (i = 0; i < actualGridData_->columns(); ++i) 
	{
    actualGridData_->normals[i][0][0] = actualGridData_->normals[i][1][0];
    actualGridData_->normals[i][0][1] = actualGridData_->normals[i][1][1];
    actualGridData_->normals[i][0][2] = actualGridData_->normals[i][1][2];

    actualGridData_->normals[i][actualGridData_->rows()-1][0] = actualGridData_->normals[i][actualGridData_->rows()-2][0];
    actualGridData_->normals[i][actualGridData_->rows()-1][1] = actualGridData_->normals[i][actualGridData_->rows()-2][1];
    actualGridData_->normals[i][actualGridData_->rows()-1][2] = actualGridData_->normals[i][actualGridData_->rows()-2][2];
  }

  /* fill in the normals on the left/right edge of the mesh (simply
     copy the one right/left of it). */
  for (j = 0; j < actualGridData_->rows(); ++j) 
	{
    actualGridData_->normals[0][j][0] = actualGridData_->normals[1][j][0];
    actualGridData_->normals[0][j][1] = actualGridData_->normals[1][j][1];
    actualGridData_->normals[0][j][2] = actualGridData_->normals[1][j][2];

    actualGridData_->normals[actualGridData_->columns()-1][j][0] = actualGridData_->normals[actualGridData_->columns()-2][j][0];
    actualGridData_->normals[actualGridData_->columns()-1][j][1] = actualGridData_->normals[actualGridData_->columns()-2][j][1];
    actualGridData_->normals[actualGridData_->columns()-1][j][2] = actualGridData_->normals[actualGridData_->columns()-2][j][2];
  }
  
	
	ParallelEpiped hull = 
		ParallelEpiped(
										Triple(	
														actualGridData_->vertices[0][0][0], 
														actualGridData_->vertices[0][0][1], 
														tmin
													), 
										Triple(
														actualGridData_->vertices[actualGridData_->columns()-1][actualGridData_->rows()-1][0], 
														actualGridData_->vertices[actualGridData_->columns()-1][actualGridData_->rows()-1][1], 
														tmax
													)
									);

	actualGridData_->setHull(hull);

	updateData();
	updateNormals();
	createCoordinateSystem();

	return true;
}	


/*! 
	Convert user (non-rectangular) mesh based data to internal structure.
	See also Qwt3D::TripleField and Qwt3D::CellField
*/
bool 
SurfacePlot::createDataRepresentation(TripleField const& data, CellField const& poly, MESHTYPE mtype)
{
	if (mtype == GRID)
		return false;
	
	meshtype_ = mtype;
	
	actualGridData_->clear();
	actualCellData_->clear();
	
	unsigned tsize = tesselationSize(poly);
	
	actualCellData_->nodes = data;
	actualCellData_->cells = poly;
	actualCellData_->normals = TripleField(actualCellData_->nodes.size());

	unsigned i;

//  normals for the moment
	Triple n, u, v;
	for ( i = 0; i < poly.size(); ++i) 
	{
		if (poly[i].size() < 3)
			n = Triple(0,0,0);
		else
		{
			for (unsigned j = 0; j < poly[i].size(); ++j) 
			{
				unsigned jj = (j+1) % poly[i].size(); 
				unsigned pjj = (j) ? j-1 : poly[i].size()-1;
				u = actualCellData_->nodes[poly[i][jj]]-actualCellData_->nodes[poly[i][j]];		
				v = actualCellData_->nodes[poly[i][pjj]]-actualCellData_->nodes[poly[i][j]];
				n = normalizedcross(u,v);
				actualCellData_->normals[poly[i][j]] += n;
			}
		}
	}
	for ( i = 0; i != actualCellData_->normals.size(); ++i) 
	{
		actualCellData_->normals[i].normalize();
	}  
	
	ParallelEpiped hull(Triple(DBL_MAX,DBL_MAX,DBL_MAX),Triple(-DBL_MAX,-DBL_MAX,-DBL_MAX));

	for (i = 0; i!=data.size(); ++i)
	{
		if (data[i].x < hull.minVertex.x)
			hull.minVertex.x = data[i].x;
		if (data[i].y < hull.minVertex.y)
			hull.minVertex.y = data[i].y;
		if (data[i].z < hull.minVertex.z)
			hull.minVertex.z = data[i].z;
		
		if (data[i].x > hull.maxVertex.x)
			hull.maxVertex.x = data[i].x;
		if (data[i].y > hull.maxVertex.y)
			hull.maxVertex.y = data[i].y;
		if (data[i].z > hull.maxVertex.z)
			hull.maxVertex.z = data[i].z;
	}

	actualCellData_->setHull(hull);

	updateData();
	updateNormals();
	createCoordinateSystem();

	return true;
}	