#ifndef __vectorfield_2003_05_27_09_06__
#define __vectorfield_2003_05_27_09_06__

#include "types.h"

class Color;
class VectorField
{
public:
	
	explicit VectorField(Color* col);
	~VectorField();

	void configureArrows(int segs, double relconelength, double relconerad, double relstemrad);
	void drawArrows();	
	void drawArrow(Triple const& beg, Triple const& end, RGBA col);	

	TripleVector bases;
	TripleVector tops;

private:

	GLUquadricObj *hat;
	GLUquadricObj *disk;
	GLUquadricObj *base;
	GLUquadricObj *bottom;

	double calcRotation(Triple& axis, Triple const& beg, Triple const& end);

	Color* colors;

	int segments;
	double rel_cone_length;
	
	double rel_cone_radius;
	double rel_stem_radius;

	void drawArrow(Triple const& beg, Triple const& end);	

	static const double PI;
};

#endif  // __vectorfield_2003_05_27_09_06__
