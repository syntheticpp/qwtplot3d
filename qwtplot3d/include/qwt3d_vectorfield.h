#ifndef __vectorfield_2003_05_27_09_06__
#define __vectorfield_2003_05_27_09_06__

#include "qwt3d_global.h"
#include "qwt3d_types.h"

namespace Qwt3D
{

class Color;
//! 3D vector field. 
/**
	The class encapsulates a vector field including his OpenGL representation as arrow field. 
	The arrows can be configured in different aspects (color, shape, painting quality).
	
*/
class QWT3D_EXPORT VectorField
{
public:
	
	explicit VectorField(Color* col);
	~VectorField();

	void configureArrows(int segs, double relconelength, double relconerad, double relstemrad);
	void setQuality(int val);
	void drawArrows(); //!< Draws the vector field	

	Qwt3D::FreeVectorField elements; //!< The positions for all field elements

private:

	GLUquadricObj *hat;
	GLUquadricObj *disk;
	GLUquadricObj *base;
	GLUquadricObj *bottom;

	double calcRotation(Qwt3D::Triple& axis, Qwt3D::FreeVector const& vec);
	void drawArrow(Qwt3D::FreeVector const& vec, Qwt3D::RGBA col);//!< Draws a single vector arrow

	Color* colors;

	int segments;
	double rel_cone_length;
	
	double rel_cone_radius;
	double rel_stem_radius;

	void drawArrow(Qwt3D::FreeVector const& vec);	

	static const double PI;
};

} // ns

#endif  // __vectorfield_2003_05_27_09_06__
