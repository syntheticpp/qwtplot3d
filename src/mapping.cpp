#if defined(_MSC_VER) /* MSVC Compiler */
#pragma warning ( disable : 4305 )
#pragma warning ( disable : 4786 )
#endif

#include "qwt_plot3d.h"

/*! 
	Convert user grid data to internal vertex structure.
	See also NativeReader::read() and Function::create()
*/
bool 
QwtPlot3D::createDataRepresentation(double** data, unsigned int columns, unsigned int rows
																				, double minx, double maxx, double miny, double maxy)
{
	meshtype_ = GRID;
	
	int i,j;
	
	actualCellData_->clear();
	actualGridData_->setSize(columns,rows);
	
	double dx = (maxx - minx) / (actualGridData_->columns() - 1);
	double dy = (maxy - miny) / (actualGridData_->rows() - 1);


	// normals
	  
	GLdouble u[3], v[3], n[3], l;  // for cross product
	DataMatrix data_normals = std::vector<DataRow>(actualGridData_->columns());
	{
		for (i=0; i!=(int)data_normals.size(); ++i)
		{
			data_normals[i] = DataRow(actualGridData_->rows());
			for (j=0; j!=(int)data_normals[i].size(); ++j)
			{
				data_normals[i][j] = new GLdouble[3];
			}
		}
	}

	double tmin = DBL_MAX;
	double tmax = -DBL_MAX;

	/* fill out the vertex array for the mesh. */
	for (i = 0; i < actualGridData_->columns() - 1; ++i) 
	{
		for (j = 0; j < actualGridData_->rows() - 1; ++j) 
		{
			/* assign the data to vertices.  some of the vertices will be
				 overwritten in subsequent iterations of the loop, but this is
				 okay, since they will be identical. */
			actualGridData_->vertices[i][j][0] = minx + i*dx; 
			actualGridData_->vertices[i][j][1] = miny + j*dy;
			actualGridData_->vertices[i][j][2] = data[i][j];

			if (data[i][j] > tmax)
				tmax = data[i][j];
			if (data[i][j] < tmin)
				tmin = data[i][j];

    
			actualGridData_->vertices[i][j+1][0] = actualGridData_->vertices[i][j][0];
			actualGridData_->vertices[i][j+1][1] = miny + (j+1)*dy;
			actualGridData_->vertices[i][j+1][2] = data[i][j+1];
    
			actualGridData_->vertices[i+1][j][0] = minx + (i+1)*dx;
			actualGridData_->vertices[i+1][j][1] = actualGridData_->vertices[i][j][1];
			actualGridData_->vertices[i+1][j][2] = data[i+1][j];

			/*	get two vectors to cross */
      u[0] = actualGridData_->vertices[i+1][j][0] - actualGridData_->vertices[i][j][0];
      u[1] = actualGridData_->vertices[i+1][j][1] - actualGridData_->vertices[i][j][1];
      u[2] = actualGridData_->vertices[i+1][j][2] - actualGridData_->vertices[i][j][2];

      v[0] = actualGridData_->vertices[i][j+1][0] - actualGridData_->vertices[i][j][0];
      v[1] = actualGridData_->vertices[i][j+1][1] - actualGridData_->vertices[i][j][1];
      v[2] = actualGridData_->vertices[i][j+1][2] - actualGridData_->vertices[i][j][2];

      /* get the normalized cross product */ 
      normalizedcross(u, v, n); // right hand system here !
      
     /*  put the facet normal in the i, j position for later averaging
         with other normals.*/ 
      data_normals[i][j][0] = n[0];
      data_normals[i][j][1] = n[1];
      data_normals[i][j][2] = n[2];     
		}
	}

  /* fill in the last vertex & it's facet normal */
  actualGridData_->vertices[i][j][0] = minx + i*dx; 
  actualGridData_->vertices[i][j][1] = miny + j*dy;
  actualGridData_->vertices[i][j][2] = data[i][j];
  
  data_normals[i][j][0] = n[0];
  data_normals[i][j][1] = n[1];
  data_normals[i][j][2] = n[2];
	
  /* calculate normals for the mesh */
  for (i = 1; i < actualGridData_->columns() - 1; ++i) 
	{
    for (j = 1; j < actualGridData_->rows() - 1; ++j) 
		{
      /* average all the neighboring normals. */
      n[0] = data_normals[i-1][j-1][0];
      n[1] = data_normals[i-1][j-1][1];
      n[2] = data_normals[i-1][j-1][2];

      n[0] += data_normals[i][j-1][0];
      n[1] += data_normals[i][j-1][1];
      n[2] += data_normals[i][j-1][2];

      n[0] += data_normals[i-1][j][0];
      n[1] += data_normals[i-1][j][1];
      n[2] += data_normals[i-1][j][2];

      n[0] += data_normals[i][j][0];
      n[1] += data_normals[i][j][1];
      n[2] += data_normals[i][j][2];
      
      l = (GLdouble)sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
      actualGridData_->normals[i][j][0] = n[0] /= l;
      actualGridData_->normals[i][j][1] = n[1] /= l;
      actualGridData_->normals[i][j][2] = n[2] /= l;
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
  
	
	/* free the mesh data */
	for (i = 0; i < actualGridData_->columns(); ++i) 
	{
		for (j = 0; j < actualGridData_->rows(); ++j)
		{
			delete [] data_normals[i][j];
		}
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
	createCoordinateSystem();

	return true;
}	




/*! 
	Convert user (non-rectangular) mesh based data to internal structure.
	See also Qwt3D::TripleVector and Qwt3D::Tesselation
*/
bool 
QwtPlot3D::createDataRepresentation(TripleVector const& data, Tesselation const& poly, MESHTYPE mtype)
{
	if (mtype == GRID)
		return false;
	
	meshtype_ = mtype;
	
	actualGridData_->clear();
	actualCellData_->clear();
	
	unsigned tsize = tesselationSize(poly);
	
	actualCellData_->nodes = data;
	actualCellData_->cells = poly;
	actualCellData_->normals = TripleVector(actualCellData_->nodes.size());

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
				actualCellData_->normals[poly[i][j]].normalize();
			}

		}
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
	createCoordinateSystem();

	return true;
}	