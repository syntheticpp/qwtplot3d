#if defined(_MSC_VER) /* MSVC Compiler */
#pragma warning ( disable : 4786 )
#endif

#include <stdio.h>
#include <qtextstream.h>

#include "qwt3d_surfaceplot.h"
#include "qwt3d_io.h"

using namespace std;
using namespace Qwt3D;


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
  bool extract_info(FILE* fp, unsigned int& xmesh, unsigned int& ymesh, double& xmin, double& xmax, double& ymin, double& ymax)
  {
    char* p;
  
    // find out the size
    if ((p = read_field (fp)) == 0)
      return false;
    xmesh = (unsigned int)atoi(p);

    if ((p = read_field (fp)) == 0)
      return false;
    ymesh = (unsigned int)atoi (p);

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

	double** allocateData(int columns, int rows)
	{
 		double** data         = new double* [columns] ;
 
		for (int i = 0; i < columns; ++i) 
		{
			data[i]         = new double [rows];
		}
		return data;
	}
	
	void deleteData(double**data, int columns)
	{
		for ( unsigned int i = 0; i < columns; i++) 
		{
			delete [] data[i];
		}
		delete data;
	}
}

NativeReader::NativeReader(SurfacePlot* pw, QString fname)
: magicstring("jk:11051895-17021986")
{
	if (!pw)
	{
		fprintf(stderr,"NativeReader: no valid Plot3D Widget");
		return;
	}
	plotwidget_ = pw;
	setFileName(fname);
}

bool NativeReader::collectInfo(FILE*& file, unsigned& xmesh, unsigned& ymesh, 
															 double& minx, double& maxx, double& miny, double& maxy)
{
	if (fileName_.isEmpty() || !plotwidget_)
		return false;
	
	file = open(fileName_);
	
	if (!file)
		return false;
	
	
	if (
				(!check_magic(file, magicstring))
			||(!check_type(file, "MESH"))
			||(!extract_info(file, xmesh, ymesh, minx, maxx, miny, maxy))
		 )
	{
		fclose(file);
		return false;
	}
 
	return true;
}


bool 
NativeReader::read(double minz, double maxz)
{
	
	FILE* file;
	unsigned int xmesh, ymesh;
	double minx, maxx, miny, maxy;
	
	if ( !collectInfo(file, xmesh, ymesh, minx, maxx, miny, maxy) )
		return false;
	
	/* allocate some space for the mesh */
 	double** data = allocateData(xmesh, ymesh);

	for (unsigned int j = 0; j < ymesh; j++) 
	{
    for (unsigned int i = 0; i < xmesh; i++) 
		{
      if (fscanf(file, "%lf", &data[i][j]) != 1) 
			{
				fprintf(stderr, "NativeReader::read: error in data file \"%s\"\n", fileName_.latin1());
				return false;
      }

			if (data[i][j] > maxz)
				data[i][j] = maxz;
			else if (data[i][j] < minz)
				data[i][j] = minz;
    }
  }

  /* close the file */
  fclose(file);

	plotwidget_->createDataRepresentation(data, xmesh, ymesh, minx, maxx, miny, maxy);
	deleteData(data,xmesh);

	return true;
}

