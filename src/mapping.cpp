#include "qwt_plot3d.h"


/*! 
	Convert user data in internal vertex structure.
	See also NativeReader::read() and Function::create()
*/
void 
QwtPlot3D::createInternalRepresentation(double** data, unsigned int columns, unsigned int rows
																				, double minx, double maxx, double miny, double maxy)
{
	int i,j;
	
	actualData_.setSize(columns,rows);
	
	double dx = (maxx - minx) / (actualData_.columns() - 1);
	double dy = (maxy - miny) / (actualData_.rows() - 1);


	// normals
	  
	GLdouble u[3], v[3], n[3], l;  // for cross product
	DataMatrix data_normals = std::vector<DataRow>(actualData_.columns());
	{
		for (i=0; i!=(int)data_normals.size(); ++i)
		{
			data_normals[i] = DataRow(actualData_.rows());
			for (j=0; j!=(int)data_normals[i].size(); ++j)
			{
				data_normals[i][j] = new GLdouble[3];
			}
		}
	}

	/* fill out the vertex array for the mesh. */
	for (i = 0; i < actualData_.columns() - 1; ++i) 
	{
		for (j = 0; j < actualData_.rows() - 1; ++j) 
		{
			/* assign the data to vertices.  some of the vertices will be
				 overwritten in subsequent iterations of the loop, but this is
				 okay, since they will be identical. */
			actualData_.vertices[i][j][0] = minx + i*dx; 
			actualData_.vertices[i][j][1] = miny + j*dy;
			actualData_.vertices[i][j][2] = data[i][j];

			if (data[i][j] > actualData_.maximum())
				actualData_.setMax(data[i][j]);
			if (data[i][j] < actualData_.minimum())
				actualData_.setMin(data[i][j]);

    
			actualData_.vertices[i][j+1][0] = actualData_.vertices[i][j][0];
			actualData_.vertices[i][j+1][1] = miny + (j+1)*dy;
			actualData_.vertices[i][j+1][2] = data[i][j+1];
    
			actualData_.vertices[i+1][j][0] = minx + (i+1)*dx;
			actualData_.vertices[i+1][j][1] = actualData_.vertices[i][j][1];
			actualData_.vertices[i+1][j][2] = data[i+1][j];

			/*	get two vectors to cross */
      u[0] = actualData_.vertices[i][j+1][0] - actualData_.vertices[i][j][0];
      u[1] = actualData_.vertices[i][j+1][1] - actualData_.vertices[i][j][1];
      u[2] = actualData_.vertices[i][j+1][2] - actualData_.vertices[i][j][2];

      v[0] = actualData_.vertices[i+1][j][0] - actualData_.vertices[i][j][0];
      v[1] = actualData_.vertices[i+1][j][1] - actualData_.vertices[i][j][1];
      v[2] = actualData_.vertices[i+1][j][2] - actualData_.vertices[i][j][2];

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
  actualData_.vertices[i][j][0] = minx + i*dx; 
  actualData_.vertices[i][j][1] = miny + j*dy;
  actualData_.vertices[i][j][2] = data[i][j];
  
  data_normals[i][j][0] = n[0];
  data_normals[i][j][1] = n[1];
  data_normals[i][j][2] = n[2];
	
  /* calculate normals for the mesh */
  for (i = 1; i < actualData_.columns() - 1; ++i) 
	{
    for (j = 1; j < actualData_.rows() - 1; ++j) 
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
      actualData_.normals[i][j][0] = n[0] /= l;
      actualData_.normals[i][j][1] = n[1] /= l;
      actualData_.normals[i][j][2] = n[2] /= l;
    }
  }

  /* fill in the normals on the top/bottom edge of the mesh (simply
     copy the one below/above it). */
  for (i = 0; i < actualData_.columns(); ++i) 
	{
    actualData_.normals[i][0][0] = actualData_.normals[i][1][0];
    actualData_.normals[i][0][1] = actualData_.normals[i][1][1];
    actualData_.normals[i][0][2] = actualData_.normals[i][1][2];

    actualData_.normals[i][actualData_.rows()-1][0] = actualData_.normals[i][actualData_.rows()-2][0];
    actualData_.normals[i][actualData_.rows()-1][1] = actualData_.normals[i][actualData_.rows()-2][1];
    actualData_.normals[i][actualData_.rows()-1][2] = actualData_.normals[i][actualData_.rows()-2][2];
  }

  /* fill in the normals on the left/right edge of the mesh (simply
     copy the one right/left of it). */
  for (j = 0; j < actualData_.rows(); ++j) 
	{
    actualData_.normals[0][j][0] = actualData_.normals[1][j][0];
    actualData_.normals[0][j][1] = actualData_.normals[1][j][1];
    actualData_.normals[0][j][2] = actualData_.normals[1][j][2];

    actualData_.normals[actualData_.columns()-1][j][0] = actualData_.normals[actualData_.columns()-2][j][0];
    actualData_.normals[actualData_.columns()-1][j][1] = actualData_.normals[actualData_.columns()-2][j][1];
    actualData_.normals[actualData_.columns()-1][j][2] = actualData_.normals[actualData_.columns()-2][j][2];
  }
  
	
	/* free the mesh data */
	for (i = 0; i < actualData_.columns(); ++i) 
	{
		for (j = 0; j < actualData_.rows(); ++j)
		{
			delete [] data_normals[i][j];
		}
	}
	updateData();
	createCoordinateSystem();
}	