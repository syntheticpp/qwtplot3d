#if defined(_MSC_VER) /* MSVC Compiler */
#pragma warning ( disable : 4786 )
#endif

#ifndef __DATATYPES_H__
#define __DATATYPES_H__

#include <string>
#include <vector>
#include <algorithm>

#ifdef _WIN32
	#include <windows.h>
#endif

#include <qgl.h>
#include <math.h>
#include <float.h>

namespace Qwt3d
{

enum PLOTSTYLE
{
	NOPLOT,
	WIREFRAME,
	HIDDENLINE,
	FILLED,
	FILLEDMESH
};

//! Not yet implemented
enum ELEMENTTYPE
{
	POINT,
	TRIANGLE,
  TRIANGLE2,
	QUAD
};

enum COORDSTYLE
{
	NOCOORD,
	BOX,
	FRAME		
};

enum FLOORSTYLE
{
	NOFLOOR,
	FLOORMESH,
	FLOORISO,
	FLOORDATA
};

//! The 12 axes
enum AXIS
{
	X1 = 0,
	X2 = 3,
	X3 = 4,
	X4 = 5,
	Y1 = 1,
	Y2 = 8,
	Y3 = 7,
	Y4 = 6,
	Z1 = 2,
	Z2 = 9,
	Z3 = 11,
	Z4 = 10
};

} // ns

inline int round(double d)
{
	return (d>0) ? int(d+0.5) : int(d-0.5);
}


//! Triple <tt>[x,y,z]</tt>
struct Triple
{
	explicit Triple(double xv = 0,double yv = 0,double zv = 0)
		: x(xv), y(yv), z(zv)
	{
	}
	
	double x,y,z;

	Triple& operator+=(Triple t)
	{
		x += t.x;
		y += t.y;
		z += t.z;

		return *this;
	}
	
	Triple& operator-=(Triple t)
	{
		x -= t.x;
		y -= t.y;
		z -= t.z;

		return *this;
	}
	Triple& operator*=(double d)
	{
		x *= d;
		y *= d;
		z *= d;

		return *this;
	}
	Triple& operator/=(double d)
	{
		x /= d;
		y /= d;
		z /= d;

		return *this;
	}
	Triple& operator*=(Triple t) // scale
	{
		x *= t.x;
		y *= t.y;
		z *= t.z;

		return *this;
	}

	
	bool operator==(Triple t) const
	{
		return (x==t.x) && (y==t.y) && (z==t.z);
	}
	bool operator!=(Triple t) const
	{
		return !operator==(t);
	}
	
	double length() const
	{
		double l2 = x*x + y*y + z*z;
		return (l2>DBL_EPSILON) ? sqrt(l2) : 0;
	}
	
	void normalize()
	{
		double l = length();
		if (l)
			*this /= l;
	}
};


inline const Triple operator+(const Triple& t, const Triple& t2)
{
	return Triple(t) += t2;
}
inline const Triple operator-(const Triple& t, const Triple& t2)
{
	return Triple(t) -= t2;
}
inline const Triple operator*(double d, const Triple& t)
{
	return Triple(t) *= d;
}
inline const Triple operator*(const Triple& t, double d)
{
	return Triple(t) *= d;
}
inline const Triple operator/(double d, const Triple& t)
{
	return Triple(t) /= d;
}
inline const Triple operator/(const Triple& t, double d)
{
	return Triple(t) /= d;
}
inline const Triple operator*(const Triple& t, const Triple& t2)
{
	return Triple(t) *= t2;
}

typedef double *Vertex;
typedef std::vector<Vertex> DataRow;
typedef std::vector<DataRow> DataMatrix;

//! Implements a matrix of z-Values with limit access functions 
class Data
{
public:
	
	Data();
	Data(unsigned int columns, unsigned int rows);//!< see setSize()
	~Data();

	Data(Data const&); 
	Data& operator=(Data const&); //!< deep copy

	int columns() const;
	int rows() const;

	void clear(); //!< destroy content
	bool empty() const { return vertices.empty();}
	void setSize(unsigned int columns, unsigned int rows); //!< destroys content and set new size, elements are uninitialized
	
	double maximum() const; //!< \return minimal z value
	double minimum() const; //!< \return maximal z value
	
	void setMin(double minv); //!< set maximum for z, not immediately, but the different generators take care and will cut
	void setMax(double maxv); //!< set minimum for z, not immediately, but the different generators take care and will cut 

	DataMatrix vertices;		//!< mesh vertices
	DataMatrix normals;		//!< mesh normals

private:

	double				max_;		/* maximum height value in mesh */
	double				min_;		/* minimum height value in mesh */
};


// colors

//! Red-Green-Blue-Alpha value
struct RGBA
{
	RGBA() 
		{}
	explicit RGBA(double rr, double gg, double bb, double aa = 1)
		: r(rr), g(gg), b(bb), a(aa)
		{}
	double r,g,b,a;
};

typedef std::vector<RGBA> ColorVector;


QColor GL2Qt(GLdouble r, GLdouble g, GLdouble b); //!< RGB -> QColor
RGBA Qt2GL(QColor col); //!< QColor -> RGBA


//! simplified glut routine (glProject): object coord --> windows coord 
inline Triple World2ViewPort(Triple obj,	bool* err = 0)
{
  Triple win;

  GLdouble modelMatrix[16];
  GLdouble projMatrix[16];
  GLint viewport[4];

	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX,	modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX,	projMatrix);
	
	int res = gluProject(obj.x, obj.y, obj.z, modelMatrix, projMatrix, viewport, &win.x, &win.y, &win.z);
	if (err)
		*err = (res) ? false : true;
	return win;
}

//! simplified glut routine (glUnProject): windows coord --> object coord 
inline Triple ViewPort2World(Triple win, bool* err = 0)
{
  Triple obj;
	
	GLdouble modelMatrix[16];
  GLdouble projMatrix[16];
  GLint viewport[4];

	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX,	modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX,	projMatrix);
	
	int res = gluUnProject(win.x, win.y, win.z, modelMatrix, projMatrix, viewport, &obj.x, &obj.y, &obj.z);

	if (err)
		*err = (res) ? false : true;
	return obj;
}

inline const GLubyte* gl_error()
{
	GLenum errcode;
	const GLubyte* err = 0;
	
	if ((errcode = glGetError()) != GL_NO_ERROR)
	{
		err = gluErrorString(errcode);
	}
	return err;
}

inline void normalizedcross(GLdouble* u, GLdouble* v, GLdouble* n)
{
  GLdouble l;

  /* compute the cross product (u x v for right-handed [ccw]) */
  n[0] = u[1] * v[2] - u[2] * v[1];
  n[1] = u[2] * v[0] - u[0] * v[2];
  n[2] = u[0] * v[1] - u[1] * v[0];

  /* normalize */
  l = (GLdouble)sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
  n[0] /= l;
  n[1] /= l;
  n[2] /= l;
}

double ceil125( double x);
double floor125( double x);
double round125( double x);


struct Tuple
{
	Tuple() : x(0), y(0) {}
	Tuple(double X, double Y) : x(X), y(Y) {}
	double x,y;
};

void convexhull2d( std::vector<int>& idx, const std::vector<Tuple>& src );

#endif