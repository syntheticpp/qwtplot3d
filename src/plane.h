#ifndef __PLANE_H__
#define __PLANE_H__

#include "drawable.h"
#include "colorgenerator.h"

class Quad : public Drawable
{

public:
	
	Quad();
	virtual void draw();
	virtual void setPosition(Triple one, Triple two, Triple three, Triple four);
	Triple one() const { return one_;}
	Triple two() const { return two_;}
	Triple three() const { return three_;}
	Triple four() const { return four_;}

protected:
	Triple one_, two_, three_, four_;
};

class ColorLegend : public Quad
{

public:
	
	ColorLegend();

	void draw();

	ColorVector colors;
};


#endif // __PLANE_H__
