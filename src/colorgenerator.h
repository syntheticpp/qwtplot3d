#ifndef __COLORGENERATOR_H__
#define __COLORGENERATOR_H__

#include <qstring.h>
#include "types.h"


class Color
{
public:
	virtual ~Color();
	virtual RGBA operator()(double x, double y, double z) {return RGBA(0,0,0,0);}
	virtual void setAlpha(double a) {}
};

class StandardColor : public Color
{
public:
	explicit StandardColor(Data const& data, int size = 100);
	RGBA operator()(double x, double y, double z);
	ColorVector const& colVector() const { return colors_;}
	void setAlpha(double a);

private:
	Data const& data_;
	ColorVector colors_;
};


#endif // __COLORGENERATOR_H__
