#if defined(_MSC_VER) /* MSVC Compiler */
#pragma warning ( disable : 4305 )
#pragma warning ( disable : 4786 )
#endif

#include "qwt_plot3d.h"


/*!
  Set the rotation angle of the object to \e degrees around the X axis.
*/
void 
QwtPlot3D::setRotation( double xVal, double yVal, double zVal )
{
  if (xRot_ == xVal && yRot_ == yVal && zRot_ == zVal)
		return;
	
	xRot_ = xVal;
	yRot_ = yVal;
	zRot_ = zVal;
  
	updateGL();
	emit rotationChanged(xVal, yVal, zVal);
}

void 
QwtPlot3D::setShift( double xVal, double yVal, double zVal )
{
  if (xShift_ == xVal && yShift_ == yVal && zShift_ == zVal)
		return;
	
	xShift_ = xVal;
	yShift_ = yVal;
	zShift_ = zVal;
	updateGL();
	emit shiftChanged(xVal, yVal, zVal);
}

void 
QwtPlot3D::setViewportShift( double xVal, double yVal )
{
  if (xVPShift_ == xVal && yVPShift_ == yVal)
		return;
	
	if (xVal < -2)
		xVPShift_ = -2;
	else if (xVal > 2)
		xVPShift_ = 2;
	else
		xVPShift_ = xVal;

	if (yVal < -2)
		yVPShift_ = -2;
	else if (yVal > 2)
		yVPShift_ = 2;
	else
		yVPShift_ = yVal;
		
	updateGL();
	emit vieportShiftChanged(xVPShift_, yVPShift_);
}

void 
QwtPlot3D::setScale( double xVal, double yVal, double zVal )
{
  if (xScale_ == xVal && yScale_ == yVal && zScale_ == zVal)
		return;
	
	xScale_ = xVal;
	yScale_ = yVal;
	zScale_ = zVal;
	updateGL();
	emit scaleChanged(xVal, yVal, zVal);
}

void 
QwtPlot3D::setZoom( double val )
{
  if (zoom_ == val)
		return;

  zoom_ = val;
 	updateGL();
	emit zoomChanged(val);
}