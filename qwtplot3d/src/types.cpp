#if defined(_MSC_VER) /* MSVC Compiler */
#pragma warning ( disable : 4786 )
#endif



#include <fstream.h>
ofstream ofs("aaa.txt");

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


// convex hull

namespace {

typedef double coord;


int ccw(coord **P, int i, int j, int k) {
	coord	a = P[i][0] - P[j][0],
		b = P[i][1] - P[j][1],
		c = P[k][0] - P[j][0],
		d = P[k][1] - P[j][1];
	return a*d - b*c <= 0;	   /* true if points i, j, k counterclockwise */
}


#define CMPM(c,A,B) \
	v = (*(coord**)A)[c] - (*(coord**)B)[c];\
	if (v>0) return 1;\
	if (v<0) return -1;

int cmpl(const void *a, const void *b) {
	double v;
	CMPM(0,a,b);
	CMPM(1,b,a);
	return 0;
}

int cmph(const void *a, const void *b) {return cmpl(b,a);}


int make_chain(coord** V, int n, int (*cmp)(const void*, const void*)) {
	int i, j, s = 1;
	coord* t;

	qsort(V, n, sizeof(coord*), cmp);
	for (i=2; i<n; i++) {
		for (j=s; j>=1 && ccw(V, i, j, j-1); j--){}
		s = j+1;
		t = V[s]; V[s] = V[i]; V[i] = t;
	}
	return s;
}

int _ch2d(coord **P, int n)  {
	int u = make_chain(P, n, cmpl);		/* make lower hull */
	if (!n) return 0;
	P[n] = P[0];
	return u+make_chain(P+u, n-u+1, cmph);	/* make upper hull */
}


} // ns anon


void convexhull2d( std::vector<int>& idx, const std::vector<Tuple>& src )
{
    idx.clear();
    if (src.empty())
        return;
    if (src.size()==1)
    {
        idx.push_back(0);
        return;
    }
    coord** points = new coord*[src.size()+1] ;
    coord* P = new coord[src.size()*2];

		ofs << "-->" << endl; 
    int i;
		for (i=0; i<(int)src.size(); ++i)
    {
			ofs << src[i].x << " " << src[i].y << endl; 
        points[i] = &P[2*i];
        points[i][0] = src[i].x;
        points[i][1] = src[i].y;
    }
		ofs << "<--" << endl; 

    coord* start = points[0];
    int m = _ch2d( points, src.size() );
    idx.resize(m);
    
		for (i=0; i<m; ++i)
 		{
			idx[i] = (points[i] - start)/2;
		}
    delete [] points;
		delete [] P;
}
