#ifndef __COLORGENERATOR_H__
#define __COLORGENERATOR_H__

#include <qstring.h>
#include "types.h"


//! ABC for color functors
/*!
Use your own color model by providing an implementation of operator()(double x, double y, double z)
Colors destructor has been declared \c protected, in order to use only heap based objects. QwtPlot3D 
will handle the objects destruction.
See StandardColor for an example
*/
class Color
{
public:
	virtual RGBA operator()(double x, double y, double z) = 0; //!< implement your color model here
	virtual void setAlpha(double a) {}; //!< sometimes you need an alpha channel, the default doues nothing

	void destroy() const { delete this;}

protected:
	virtual ~Color(){} // allow heap based objects only
};

//! Standard color model for QwtPlot3D - implements a data driven operator()(double x, double y, double z)
/*!
The class has an colorvector representing z values, which will be used by operator()(double x, double y, double z)
*/
class StandardColor : public Color
{
public:
	//! initialize with data and set up a ColorVector with a size of 100 z values (default);
	explicit StandardColor(Data const& data, int size = 100);
	RGBA operator()(double x, double y, double z); //!< get z dependend Color from ColorVector
	ColorVector const& colVector() const { return colors_;}
	void setAlpha(double a);

protected:
	ColorVector colors_;

private:
	Data const& data_;
};


#endif // __COLORGENERATOR_H__
