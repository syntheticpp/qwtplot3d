#include <math.h>
#include "colorgenerator.h"
#include "vectorfield.h"

using namespace Qwt3D;

const double VectorField::PI = 3.14159265358979323846264338328;

VectorField::VectorField(Color* col)
:colors(col)
{
//	Q_ASSERT(colors);
	
	hat      = gluNewQuadric();
	disk     = gluNewQuadric();
	base    = gluNewQuadric();
	bottom   = gluNewQuadric();

	gluQuadricDrawStyle(hat,GLU_FILL);
	gluQuadricNormals(hat,GLU_SMOOTH);
	gluQuadricOrientation(hat,GLU_OUTSIDE);
	gluQuadricDrawStyle(disk,GLU_FILL);
	gluQuadricNormals(disk,GLU_SMOOTH);
	gluQuadricOrientation(disk,GLU_OUTSIDE);
	gluQuadricDrawStyle(base,GLU_FILL);
	gluQuadricNormals(base,GLU_SMOOTH);
	gluQuadricOrientation(base,GLU_OUTSIDE);
	gluQuadricDrawStyle(bottom,GLU_FILL);
	gluQuadricNormals(bottom,GLU_SMOOTH);
	gluQuadricOrientation(bottom,GLU_OUTSIDE);

	configureArrows(3, 0.4, 0.06, 0.02);
}

VectorField::~VectorField()
{
	gluDeleteQuadric(hat);
	gluDeleteQuadric(disk);
	gluDeleteQuadric(base);
	gluDeleteQuadric(bottom);
}

void VectorField::drawArrows()
{
	if(bases.size() != tops.size() || bases.empty() || !colors)
		return;

glEnable(GL_LIGHTING);
	RGBA col;
	for (unsigned i=0; i!=bases.size(); ++i)
	{
		col = (*colors)(bases[i].x, bases[i].y, bases[i].z);
		glColor4d(col.r, col.g, col.b, col.a);
		drawArrow(bases[i], tops[i]);
	}
glDisable(GL_LIGHTING);
}

void VectorField::configureArrows(int segs, double relconelength, double relconerad, double relstemrad)
{
	segments = segs;
	rel_cone_length = relconelength;
	rel_cone_radius = relconerad;
	rel_stem_radius = relstemrad;	
}

void VectorField::setQuality(int val)
{
	segments = val;
}

void VectorField::drawArrow(Triple const& beg, Triple const& end)
{	
	Triple vdiff = end-beg;
	double length = vdiff.length();
	
	double radius[2];
	radius[0] = rel_cone_radius * length;
	radius[1] = rel_stem_radius * length;
	
	GLint mode;
	glGetIntegerv(GL_MATRIX_MODE, &mode);

	glMatrixMode( GL_MODELVIEW );
  glPushMatrix();


	Triple axis;
	double phi = calcRotation(axis,beg,end);
	
	glTranslatef(beg.x, beg.y, beg.z);
  glRotatef(phi, axis.x, axis.y, axis.z);

	double baseheight = (1-rel_cone_length) * length;
	
	glTranslatef(0, 0, baseheight);

	gluCylinder(hat, radius[0], 0.0, rel_cone_length * length, segments,1);
	gluDisk(disk,radius[1],radius[0], segments,1);
	
	glTranslatef(0, 0, -baseheight);
	
	gluCylinder(base, radius[1],radius[1], baseheight,segments,1);
	gluDisk(disk,0,radius[1],segments,1);

  glPopMatrix();
	glMatrixMode(mode);
}

void VectorField::drawArrow(Triple const& beg, Triple const& end, RGBA color)
{
	glColor4d(color.r, color.g, color.b, color.a);
	
	Triple vdiff = end-beg;
	double length = vdiff.length();
	int segments = 16;
	double radius[2];
	
	GLint mode;
	glGetIntegerv(GL_MATRIX_MODE, &mode);
	glMatrixMode( GL_MODELVIEW );
  glPushMatrix();
	Triple axis;
	double phi = calcRotation(axis,beg,end);
	glTranslatef(beg.x, beg.y, beg.z);
  glRotatef(phi, axis.x, axis.y, axis.z);
	double pl = 0.4;
	double baseheight = (1-pl) * length;
	glTranslatef(0, 0, baseheight);
	radius[0] = 0.06 * length;
	gluCylinder(hat, radius[0], 0.0, pl * length, segments,1);
	radius[1] = 0.02 * length;
	gluDisk(disk,radius[1],radius[0], segments,1);
	
	glTranslatef(0, 0, -baseheight);
	gluCylinder(base, radius[1],radius[1], baseheight,segments,1);
	gluDisk(disk,0,radius[1],segments,1);
  glPopMatrix();
	glMatrixMode(mode);
}

//! transform a vector on the z axis with length |beg-end|, to get them in coincidence with the vector(beg,end)
/**
	\return Angle in degree to rotate
	\param axis   The axis to rotate around
	\param beg    result vector base point
	\param end    result vector top point
*/ 
double VectorField::calcRotation(Triple& axis, Triple const& beg, Triple const& end)
{
	Triple firstbeg(0.0,0.0,0.0);
	Triple firstend(0.0,0.0,(end-beg).length());
	
	Triple first = firstend - firstbeg;
	first.normalize();

	Triple second = end-beg;
	second.normalize();

	axis = normalizedcross(first,second);
	double cosphi = dotProduct(first,second);
	
	return 180 * acos(cosphi) / VectorField::PI;
}