#include <float.h>
#include "types.h"
#include "datasource.h"

DataSource::DataSource()
{
	setMinZ(-DBL_MAX);
	setMaxZ(DBL_MAX);
	setMesh(0,0);
	setDomain(0,0,0,0);
}

DataSource::~DataSource()
{ 
}

void 
DataSource::setMesh(int xm, int ym) 
{ 
	xmesh_ = xm; ymesh_ = ym; 
}

void 
DataSource::setDomain(double xmin, double xmax, double ymin, double ymax)
{
	minx_ = xmin;
	maxx_ = xmax;
	miny_ = ymin;
	maxy_ = ymax;
}

void 
DataSource::setMinZ(double val) 
{ 
	minz_ = val;
}

void 
DataSource::setMaxZ(double val) 
{ 
	maxz_ = val;
}


/*! 
	fill vertex structures in res and fit to extrema, eventually deletes 'data' !
*/
void 
DataSource::convert2VertexData(Data& res, GLfloat**& data)
{
	int i,j;

	double dx = (maxx_ - minx_) / (res.columns() - 1);
	double dy = (maxy_ - miny_) / (res.rows() - 1);

	if (res.maximum() > maxz_)
		res.setMax(maxz_);
	if (res.minimum() < minz_)
		res.setMin(minz_);

	for (j = 0; j < res.rows(); ++j) 
	{
		for (i = 0; i < res.columns(); ++i) 
		{
			if (data[i][j] < minz_)
				data[i][j] = minz_;
			if (data[i][j] > maxz_)
				data[i][j] = maxz_;
		}
	}
	// normals
	  
	GLfloat u[3], v[3], n[3], l;  // for cross product
	DataMatrix data_normals = std::vector<DataRow>(res.columns());
	{
		for (i=0; i!=data_normals.size(); ++i)
		{
			data_normals[i] = DataRow(res.rows());
			for (j=0; j!=data_normals[i].size(); ++j)
			{
				data_normals[i][j] = new GLfloat[3];
			}
		}
	}

	/* fill out the vertex array for the mesh. */
	for (i = 0; i < res.columns() - 1; ++i) 
	{
		for (j = 0; j < res.rows() - 1; ++j) 
		{
			/* assign the data to vertices.  some of the vertices will be
				 overwritten in subsequent iterations of the loop, but this is
				 okay, since they will be identical. */
			res.vertices[i][j][0] = minx_ + i*dx; 
			res.vertices[i][j][1] = miny_ + j*dy;
			res.vertices[i][j][2] = data[i][j];
    
			res.vertices[i][j+1][0] = res.vertices[i][j][0];
			res.vertices[i][j+1][1] = miny_ + (j+1)*dy;
			res.vertices[i][j+1][2] = data[i][j+1];
    
			res.vertices[i+1][j][0] = minx_ + (i+1)*dx;
			res.vertices[i+1][j][1] = res.vertices[i][j][1];
			res.vertices[i+1][j][2] = data[i+1][j];

			/*	get two vectors to cross */
      u[0] = res.vertices[i][j+1][0] - res.vertices[i][j][0];
      u[1] = res.vertices[i][j+1][1] - res.vertices[i][j][1];
      u[2] = res.vertices[i][j+1][2] - res.vertices[i][j][2];

      v[0] = res.vertices[i+1][j][0] - res.vertices[i][j][0];
      v[1] = res.vertices[i+1][j][1] - res.vertices[i][j][1];
      v[2] = res.vertices[i+1][j][2] - res.vertices[i][j][2];

      /* get the normalized cross product */ 
      normalizedcross(u, v, n);
      
     /*  put the facet normal in the i, j position for later averaging
         with other normals.*/ 
      data_normals[i][j][0] = n[0];
      data_normals[i][j][1] = n[1];
      data_normals[i][j][2] = n[2];     
		}
	}

  /* fill in the last vertex & it's facet normal */
  res.vertices[i][j][0] = minx_ + i*dx; 
  res.vertices[i][j][1] = miny_ + j*dy;
  res.vertices[i][j][2] = data[i][j];
  
  data_normals[i][j][0] = n[0];
  data_normals[i][j][1] = n[1];
  data_normals[i][j][2] = n[2];
	
  /* calculate normals for the mesh */
  for (i = 1; i < res.columns() - 1; ++i) 
	{
    for (j = 1; j < res.rows() - 1; ++j) 
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
      
      l = (GLfloat)sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
      res.normals[i][j][0] = n[0] /= l;
      res.normals[i][j][1] = n[1] /= l;
      res.normals[i][j][2] = n[2] /= l;
    }
  }

  /* fill in the normals on the top/bottom edge of the mesh (simply
     copy the one below/above it). */
  for (i = 0; i < res.columns(); ++i) 
	{
    res.normals[i][0][0] = res.normals[i][1][0];
    res.normals[i][0][1] = res.normals[i][1][1];
    res.normals[i][0][2] = res.normals[i][1][2];

    res.normals[i][res.rows()-1][0] = res.normals[i][res.rows()-2][0];
    res.normals[i][res.rows()-1][1] = res.normals[i][res.rows()-2][1];
    res.normals[i][res.rows()-1][2] = res.normals[i][res.rows()-2][2];
  }

  /* fill in the normals on the left/right edge of the mesh (simply
     copy the one right/left of it). */
  for (j = 0; j < res.rows(); ++j) 
	{
    res.normals[0][j][0] = res.normals[1][j][0];
    res.normals[0][j][1] = res.normals[1][j][1];
    res.normals[0][j][2] = res.normals[1][j][2];

    res.normals[res.columns()-1][j][0] = res.normals[res.columns()-2][j][0];
    res.normals[res.columns()-1][j][1] = res.normals[res.columns()-2][j][1];
    res.normals[res.columns()-1][j][2] = res.normals[res.columns()-2][j][2];
  }
  
	
	/* free the mesh data */
	for (i = 0; i < res.columns(); ++i) 
	{
		delete [] data[i];
		for (j = 0; j < res.rows(); ++j)
		{
			delete [] data_normals[i][j];
		}
	}
	delete [] data;
}	