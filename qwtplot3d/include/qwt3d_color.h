#ifndef __COLORGENERATOR_H__
#define __COLORGENERATOR_H__

#include <qstring.h>
#include "qwt3d_global.h"
#include "qwt3d_types.h"

namespace Qwt3D
{

//! Abstract base class for color functors
/*!
Use your own color model by providing an implementation of operator()(double x, double y, double z)
Colors destructor has been declared \c protected, in order to use only heap based objects. Plot3D 
will handle the objects destruction.
See StandardColor for an example
*/
class QWT3D_EXPORT Color
{
public:
	virtual Qwt3D::RGBA operator()(double x, double y, double z) = 0; //!< Implement your color model here
	virtual void setAlpha(double a) {}; //!< Sometimes you need an alpha channel, the default does nothing
	virtual void createVector(Qwt3D::ColorVector& vec, unsigned size=0, Triple xyz=Triple()) {};

	void destroy() const { delete this;}

protected:
	virtual ~Color(){} //!< Allow heap based objects only
};



class Plot3D;
//! Standard color model for Plot3D - implements the data driven operator()(double x, double y, double z)
/*!
The class has a ColorVector representing z values, which will be used by operator()(double x, double y, double z)
*/
class QWT3D_EXPORT StandardColor : public Color
{
public:
	//! Initialize with data and set up a ColorVector with a size of 100 z values (default);
	explicit StandardColor(Plot3D* data, int size = 100);
	Qwt3D::RGBA operator()(double x, double y, double z); //!< get z dependend Color from ColorVector
	void setColorVector(Qwt3D::ColorVector const& cv);
	void reset(int size=100); //!< reset the standard colors; 
	void setAlpha(double a);
	void createVector(Qwt3D::ColorVector& vec, unsigned size=0, Triple xyz=Triple()) {vec = colors_;}

protected:
	Qwt3D::ColorVector colors_;
	Qwt3D::Plot3D* data_;
};

} // ns

#endif // __COLORGENERATOR_H__
