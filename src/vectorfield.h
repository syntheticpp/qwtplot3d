#ifndef __vectorfield_2003_05_27_09_06__
#define __vectorfield_2003_05_27_09_06__

#include "types.h"

namespace Qwt3D
{

class Color;
class VectorField
{
public:
	
	explicit VectorField(Color* col);
	~VectorField();

	void configureArrows(int segs, double relconelength, double relconerad, double relstemrad);
	void setQuality(int val);
	void drawArrows();	
	void drawArrow(Qwt3D::Triple const& beg, Qwt3D::Triple const& end, Qwt3D::RGBA col);	

	Qwt3D::TripleVector bases;
	Qwt3D::TripleVector tops;

private:

	GLUquadricObj *hat;
	GLUquadricObj *disk;
	GLUquadricObj *base;
	GLUquadricObj *bottom;

	double calcRotation(Qwt3D::Triple& axis, Qwt3D::Triple const& beg, Qwt3D::Triple const& end);

	Color* colors;

	int segments;
	double rel_cone_length;
	
	double rel_cone_radius;
	double rel_stem_radius;

	void drawArrow(Qwt3D::Triple const& beg, Qwt3D::Triple const& end);	

	static const double PI;
};

} // ns

#endif  // __vectorfield_2003_05_27_09_06__
