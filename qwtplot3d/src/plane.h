#ifndef __PLANE_H__
#define __PLANE_H__

#include "drawable.h"
#include "colorgenerator.h"

namespace Qwt3D
{

class Quad : public Drawable
{

public:
	
	Quad();
	virtual void draw();
	virtual void setPosition(Qwt3D::Triple one, Qwt3D::Triple two, Qwt3D::Triple three, Qwt3D::Triple four);
	Qwt3D::Triple one() const { return one_;}
	Qwt3D::Triple two() const { return two_;}
	Qwt3D::Triple three() const { return three_;}
	Qwt3D::Triple four() const { return four_;}

protected:
	Qwt3D::Triple one_, two_, three_, four_;
};

class ColorLegend : public Quad
{

public:
	
	ColorLegend();

	void draw();

	Qwt3D::ColorVector colors;
};

} // ns

#endif // __PLANE_H__
