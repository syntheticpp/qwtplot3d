#if defined(_MSC_VER) /* MSVC Compiler */
#pragma warning ( disable : 4305 )
#pragma warning ( disable : 4786 )
#endif

#include "qwt_plot3d.h"


void 
QwtPlot3D::mousePressEvent( QMouseEvent *e )
{
	lastMouseMovePosition_ = e->pos();
	mpressed_ = true;
}

void 
QwtPlot3D::mouseReleaseEvent( QMouseEvent *e )
{
	mpressed_ = false;
}

void 
QwtPlot3D::mouseMoveEvent( QMouseEvent *e )
{
	if (!mpressed_)
		return;
		
	ButtonState bstate = e->state();
	QPoint diff = e->pos() - lastMouseMovePosition_;

	setRotationMouse(bstate, 3, diff);	
	setScaleMouse(bstate, 5, diff);	
	setShiftMouse(bstate, 4, diff);	
		
	lastMouseMovePosition_ = e->pos();
}

void 
QwtPlot3D::setRotationMouse(ButtonState bstate, double accel, QPoint diff)
{
	// Rotation
	double w = max(1,width());
	double h = max(1,height());
		
	double relx = accel*360 * diff.x() / w; 
	double relyz = accel*360 * diff.y() / h; 
	
	double new_xrot = xRotation();
	double new_yrot = yRotation();
	double new_zrot = zRotation();
	
	if ( bstate == xrot_mstate_ )
		new_xrot = int(xRotation() + relyz) % 360; 
	if ( bstate == yrot_mstate_ )
		new_yrot = int(yRotation() + relx) % 360; 
	if ( bstate == zrot_mstate_ )
		new_zrot = int(zRotation() + relx) % 360; 
		
	setRotation(new_xrot, new_yrot, new_zrot); 
}

void 
QwtPlot3D::setScaleMouse(ButtonState bstate, double accel, QPoint diff)
{
	// Scale
		double w = max(1,width());
		double h = max(1,height());

		double relx = diff.x() * accel / w; relx = exp(relx) - 1;
		double relyz = diff.y() * accel / h; relyz = exp(relyz) - 1; 

		double new_xscale = xScale();
		double new_yscale = yScale();
		double new_zscale = zScale();

		if ( bstate == xscale_mstate_)
			new_xscale = max(0.0,xScale() + relx);
		if ( bstate == yscale_mstate_)
			new_yscale = max(0.0,yScale() - relyz);
		if ( bstate == zscale_mstate_)
			new_zscale = max(0.0,zScale() - relyz);

		setScale(new_xscale, new_yscale, new_zscale); 

		if ( bstate == zoom_mstate_)
			setZoom(max(0.0,zoom() - relyz));
}

void 
QwtPlot3D::setShiftMouse(ButtonState bstate, double accel, QPoint diff)
{
	// Shift
	double w = max(1,width());
	double h = max(1,height());

	double relx = diff.x() * accel / w; 
	double relyz = diff.y() * accel / h;

	double new_xshift = xViewportShift();
	double new_yshift = yViewportShift();

	if ( bstate == xshift_mstate_)
		new_xshift = xViewportShift() + relx;
	if ( bstate == yshift_mstate_)
		new_yshift = yViewportShift() - relyz;

	setViewportShift(new_xshift, new_yshift); 
}

void 
QwtPlot3D::wheelEvent( QWheelEvent *e )
{
	
	double accel = 0.05;
	
	double step =  accel * e->delta() / WHEEL_DELTA ;
	step = exp(step)-1;

	if ( e->state() & Qt::ShiftButton )
		setScale(xScale(),yScale(), max(0.0,zScale() + step));
	else
		setZoom(max(0.0,zoom() + step ));
}

/**
	Sets the key/mousebutton combination for data/coordinatesystem moves inside the widget\n\n
	default behaviour:\n

	\code
	rotate around x axis: Qt::LeftButton 
	rotate around y axis: Qt::LeftButton | Qt::ShiftButton
	rotate around z axis: Qt::LeftButton 
	scale x:              Qt::LeftButton | Qt::AltButton 
	scale x:              Qt::LeftButton | Qt::AltButton 
	scale x:              Qt::LeftButton | Qt::AltButton | Qt::ShiftButton
	zoom:                 Qt::LeftButton | Qt::AltButton | Qt::ControlButton
	shifting along x:     Qt::LeftButton | Qt::ControlButton 
	shifting along y:     Qt::LeftButton | Qt::ControlButton
	\endcode
*/
void 
QwtPlot3D::assignMouse(int xrot, int yrot, int zrot,
											 int xscale, int yscale, int zscale,
											 int zoom, int xshift, int yshift)
{
	xrot_mstate_   = 	xrot;  
  yrot_mstate_   =  yrot;  
  zrot_mstate_   =  zrot;  
  xscale_mstate_ =  xscale;
  yscale_mstate_ =  yscale;
  zscale_mstate_ =  zscale;
  zoom_mstate_   =  zoom;  
  xshift_mstate_ =  xshift;
  yshift_mstate_ =  yshift;
}
