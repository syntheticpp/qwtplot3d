#ifndef __PLANE_H__
#define __PLANE_H__

#include "qwt3d_global.h"
#include "qwt3d_drawable.h"
#include "qwt3d_axis.h"
#include "qwt3d_color.h"

namespace Qwt3D
{

//! A flat color legend
/**

*/
class QWT3D_EXPORT ColorLegend : public Drawable
{

public:
	
	//! Possible anchor points for caption and axis
	enum POSITION
	{
		Top,
    Bottom,
		Left,
		Right
	};
	
	//! Orientation of the legend
	enum ORIENTATION
	{
    BottomTop,
		LeftRight
	};

	ColorLegend();

	void draw();
	void setGeometry(Qwt3D::Tuple relMin, Qwt3D::Tuple relMax);
	void setOrientation(ORIENTATION, POSITION);
	void setLimits(double start, double stop);
	void setMajors(int);
	void setMinors(int);

	Qwt3D::LabelPixmap caption;
	Qwt3D::ColorField colors;

private:
	
	Qwt3D::ParallelEpiped geometry() const { return pe_;}

	Qwt3D::ParallelEpiped pe_;
	Qwt3D::Axis axis_;
	POSITION axisposition_;
	ORIENTATION orientation_;
};

} // ns

#endif // __PLANE_H__
