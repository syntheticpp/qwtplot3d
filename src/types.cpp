#if defined(_MSC_VER) /* MSVC Compiler */
#pragma warning ( disable : 4786 )
#endif

#include "float.h"
#include "types.h"
#include <algorithm>

Data::Data()
{
	setSize(0,0);
}

Data::Data(unsigned int columns, unsigned int rows)
{
	setSize(columns,rows);
}

Data::~Data()
{
	clear();		
}

Data::Data(Data const& rhs)
{
	setSize(rhs.columns(), rhs.rows());
	for ( int i=0; i!=columns(); ++i)
	{
		for ( int j=0; j!=rows(); ++j)
		{
			vertices[i][j][0] = rhs.vertices[i][j][0];
			vertices[i][j][1] = rhs.vertices[i][j][1];
			vertices[i][j][2] = rhs.vertices[i][j][2];
			normals[i][j][0] = rhs.normals[i][j][0];
			normals[i][j][1] = rhs.normals[i][j][1];
			normals[i][j][2] = rhs.normals[i][j][2];
		}
	}
	max_ = rhs.max_;
	min_ = rhs.min_;
}


Data& Data::operator=(Data const& rhs)
{
	if (this == &rhs)
		return *this;

	setSize(rhs.columns(), rhs.rows());
	for ( int i=0; i!=columns(); ++i)
	{
		for ( int j=0; j!=rows(); ++j)
		{
			vertices[i][j][0] = rhs.vertices[i][j][0];
			vertices[i][j][1] = rhs.vertices[i][j][1];
			vertices[i][j][2] = rhs.vertices[i][j][2];
			normals[i][j][0] = rhs.normals[i][j][0];
			normals[i][j][1] = rhs.normals[i][j][1];
			normals[i][j][2] = rhs.normals[i][j][2];
		}
	}
	max_ = rhs.max_;
	min_ = rhs.min_;

	return *this;
}



int 
Data::columns() const 
{ 
	return (int)vertices.size();	
}

int 
Data::rows() const 
{ 
	return (empty()) ? 0 : (int)vertices[0].size();	
}

void 
Data::clear()
{
	{
		for (unsigned i=0; i!=vertices.size(); ++i)
		{	
			for (unsigned j=0; j!=vertices[i].size(); ++j)
			{	
				delete [] vertices[i][j];	
			}
			vertices[i].clear();
		}
	}

	vertices.clear();

	{
		for (unsigned i=0; i!=normals.size(); ++i)
		{	
			for (unsigned j=0; j!=normals[i].size(); ++j)
			{	
				delete [] normals[i][j];	
			}
			normals[i].clear();
		}
	}
	
	normals.clear();
	
	max_ = -DBL_MAX;
	min_ = DBL_MAX;
}


void 
Data::setSize(unsigned int columns, unsigned int rows)
{
	clear();
	vertices = std::vector<DataRow>(columns);
	{
		for (unsigned int i=0; i!=vertices.size(); ++i)
		{
			vertices[i] = DataRow(rows);
			for (unsigned int j=0; j!=vertices[i].size(); ++j)
			{
				vertices[i][j] = new GLdouble[3];
			}
		}
	}
	normals = std::vector<DataRow>(columns);
	{
		for (unsigned int i=0; i!=normals.size(); ++i)
		{
			normals[i] = DataRow(rows);
			for (unsigned int j=0; j!=normals[i].size(); ++j)
			{
				normals[i][j] = new GLdouble[3];
			}
		}
	}
}

double Data::maximum() const { return max_;}
double Data::minimum() const { return min_;}

void Data::setMin(double minv) {min_ = minv;}
void Data::setMax(double maxv) {max_ = maxv;}


QColor 
GL2Qt(GLdouble r, GLdouble g, GLdouble b)
{
	return QColor(round(r * 255), round(g * 255), round(b * 255));	
}

 
RGBA Qt2GL(QColor col)
{
	QRgb qrgb = col.rgb();
	RGBA rgba;
	rgba.r = qRed(qrgb) / 255.0;
	rgba.g = qGreen(qrgb) / 255.0;
	rgba.b = qBlue(qrgb) / 255.0;
	rgba.a = qAlpha(qrgb) / 255.0;
	return rgba;	
}

/*!
  \brief Find the smallest value out of {1,2,5}*10^n with an integer number n
  which is greater than or equal to x

  \param x Input value
*/
double ceil125( double x)
{
    if (x == 0.0) 
        return 0.0;

    double sign = (x > 0) ? 1.0 : -1.0;
    double lx = log10(fabs(x));
    double p10 = floor(lx);

    double fr = pow(10.0, lx - p10);
    if (fr <=1.0)
       fr = 1.0;
    else if (fr <= 2.0)
       fr = 2.0;
    else if (fr <= 5.0)
       fr = 5.0;
    else
       fr = 10.0;

    return sign * fr * pow(10.0, p10);
}

/*!
  \brief Find the largest value out of {1,2,5}*10^n with an integer number n
  which is smaller than or equal to x
  
  \param x Input value
*/
double floor125( double x)
{
    if (x == 0.0) 
        return 0.0;
    
    double sign = (x > 0) ? 1.0 : -1.0;
    double lx = log10(fabs(x));
    double p10 = floor(lx);

    double fr = pow(10.0, lx - p10);
    if (fr >= 10.0)
       fr = 10.0;
    else if (fr >= 5.0)
       fr = 5.0;
    else if (fr >= 2.0)
       fr = 2.0;
    else
       fr = 1.0;

    return sign * fr * pow(10.0, p10);
} 

double round125( double x)
{
	double f = floor125(x);
	double c = ceil125(x);
	
	if ( x-f < c-x )
		return f;
	
	return c;
}

