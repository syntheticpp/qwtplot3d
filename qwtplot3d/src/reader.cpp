#if defined(_MSC_VER) /* MSVC Compiler */
#pragma warning ( disable : 4786 )
#endif

#include <fstream>
#include <float.h>
#include <stdio.h>
#include <math.h>

#include "reader.h"

using namespace std;


namespace
{
	FILE* open(QString fname)
	{
		FILE* file = fopen(fname.latin1(), "r");
		if (!file) 
		{
			fprintf(stderr, "NativeReader::read: cannot open data file \"%s\"\n", fname.latin1());
		}
		return file;
	}

  int read_char (FILE * fp, bool skipcomments = true)
  {
    int c;
  
    if ((c = fgetc (fp)) == EOF)
      return (c);
    if (skipcomments)
		{
			if (c == '#')
			{
				do
				{
					if ((c = fgetc (fp)) == EOF)
						return (c);
				}
				while (c != '\n' && c != '\r');
			}
		}
    return (c);
  }

  char* read_field (FILE * fp, bool skipcomments = true)
  {
    static char buf[71];
    int c, i;
  
    do
    {
      if ((c = read_char (fp,skipcomments)) == EOF)
        return (NULL);
    }
    while (isspace (c));
    for (i = 0; i < 70 && !isspace (c); ++i)
    {
      buf[i] = c;
      if ((c = read_char (fp,skipcomments)) == EOF)
        break;
    }
    buf[i] = '\0';
    return (buf);
  }


  //! set to data begin
  bool extract_info(FILE* fp, int& xmesh, int& ymesh, double& xmin, double& xmax, double& ymin, double& ymax)
  {
    char* p;
  
    // find out the size
    if ((p = read_field (fp)) == 0)
      return false;
    xmesh = atoi (p);

    if ((p = read_field (fp)) == 0)
      return false;
    ymesh = atoi (p);

    if (xmesh < 1 || ymesh < 1)
      return false;
    
		// ... and the limits
    if ((p = read_field (fp)) == 0)
      return false;
    xmin = atof (p);

    if ((p = read_field (fp)) == 0)
      return false;
    xmax = atof (p);
    
		if ((p = read_field (fp)) == 0)



      return false;
    ymin = atof (p);

    if ((p = read_field (fp)) == 0)
      return false;
    ymax = atof (p);

    if (xmin > xmax || ymin > ymax)
      return false;

    return true;
  }

  //! find out what the magic string is and compare
	bool check_magic(FILE* fp, const char* val)
	{
    char* p;
    if ((p = read_field (fp,false)) == 0)
        return false;
  
    if (strcmp (p, val ) != 0)
        return false;	
		return true;
	}
  
	//! find out what the type is
	bool check_type(FILE* fp, const char* val)
	{
    char* p;
    if ((p = read_field (fp)) == 0)
        return false;
  
    if (strcmp (p, val ) != 0)
        return false;	
		return true;
	}

	GLfloat** allocateData(int rows, int columns, GLfloat initval)
	{
 		GLfloat** data         = new GLfloat* [columns] ;
 
		for (int i = 0; i < columns; ++i) 
		{
			data[i]         = new GLfloat [rows];
			for (int j = 0; j < rows; ++j) 
			{
				data[i][j] = initval;
			}
		}
		return data;
	}

}

NativeReader::NativeReader(QString fname)
: magicstring("jk:11051895-17021986")
{
	setFileName(fname);
	rtype_ = NativeReader::XYZFILE;
}

bool 
NativeReader::read_mesh_data_(Data& res, QString fname)
{
	FILE* file = open(fname);

	if (!file)
		return false;


	if (!check_magic(file, magicstring))
		return false;
	if (!check_type(file, "MESH"))
		return false;
 	/* get the extents of the mesh */
 	if (!extract_info(file, xmesh_, ymesh_, minx_, maxx_, miny_, maxy_))
		return false;
	
	res.setSize(xmesh_, ymesh_);
	res.setMin(DBL_MAX);
	res.setMax(-DBL_MAX);
	
	/* allocate some space for the mesh */
 	GLfloat** data = allocateData(res.rows(), res.columns(), 0);

  /* get the data */
  res.setMax(0); res.setMin(0);
  for (int j = 0; j < res.rows(); j++) 
	{
    for (int i = 0; i < res.columns(); i++) 
		{
      if (fscanf(file, "%f", &data[i][j]) != 1) 
			{
				fprintf(stderr, "NativeReader::read: error in data file \"%s\"\n", fname.latin1());
				return false;
      }

      /* find the maximum data value */
      if (res.maximum() < data[i][j]) 
				res.setMax(data[i][j]);

      /* find the minimum data value */
      if (res.minimum() > data[i][j]) 
				res.setMin(data[i][j]);
    }
  }

  /* close the file */
  fclose(file);

	convert2VertexData(res, data);

	return true;
}


bool 
NativeReader::read_xyz_data_(Data& res, QString fname)   
{
	FILE* file = open(fname);

	if (!file)
		return false;

	vector<XYZ> grid; 
	XYZ tmp, minval, maxval; 

	int fres = 3;
	while	(fres == 3)
	{		
		fres = fscanf(file, "%f%f%f", &tmp.x, &tmp.y, &tmp.z);
		if (fres == EOF)
			break;
		if ( fres != 3 ) 
		{
			fprintf(stderr, "NativeReader::read: error in data file \"%s\"\n", fname.latin1());
			return false;
    }
		grid.push_back(tmp);
  }
	vector<float> unique_x(grid.size()),
								unique_y(grid.size());

	unsigned int j,i;
	for (j = 0; j!= grid.size(); ++j)

	{
		unique_x[j] = grid[j].x;
		unique_y[j] = grid[j].y;
	}

	unsigned xs = unique_x.size();
	unsigned ys = unique_y.size();

	sort(unique_x.begin(),unique_x.end());
	sort(unique_y.begin(),unique_y.end());	
	unique_x.erase(unique(unique_x.begin(), unique_x.end()), unique_x.end()); 
	unique_y.erase(unique(unique_y.begin(), unique_y.end()), unique_y.end());

	xs = unique_x.size();
	ys = unique_y.size();

	minx_ = unique_x[0];
	maxx_ = unique_x.back();
	miny_ = unique_y[0];
	maxy_ = unique_y.back();

	minval = MIN(grid);
	maxval = MAX(grid);


	res.setSize(unique_x.size(), unique_y.size());

	/* allocate some space for the mesh */
 	GLfloat** data = allocateData(res.rows(), res.columns(), minval.z);

  /* get the data */
  res.setMax(maxval.z); 
	res.setMin(minval.z);

	for (unsigned k = 0; k < grid.size(); ++k) 
	{
    i = lower_bound(unique_x.begin(), unique_x.end(),grid[k].x)-unique_x.begin();
		j =	lower_bound(unique_y.begin(), unique_y.end(),grid[k].y)-unique_y.begin();
		data[i][j] = grid[k].z; 
  }
	fclose(file);

	
	convert2VertexData(res, data);

	return true;
}

bool 
NativeReader::createData(Data& result)
{
	if (fileName_.isEmpty())
		return false;

	result.clear();
	switch (rtype_)
	{
	case XYZFILE:
		return read_xyz_data_(result, fileName_);	
	case MESHFILE:
		return read_mesh_data_(result, fileName_);
	default:
		return false;
	}
}
