#ifndef qwt3d_extglwidget_h__2004_10_23_14_51_begin_guarded_code
#define qwt3d_extglwidget_h__2004_10_23_14_51_begin_guarded_code

#include <qgl.h>
#include "qwt3d_global.h"
#include "qwt3d_helper.h"

class QMouseEvent;
class QWheelEvent;
class QKeyEvent;

namespace Qwt3D
{
  
//! An enhanced QGLWidget covering mousehandling etc.
/*!
*/
class QWT3D_EXPORT ExtGLWidget : public QGLWidget
{
    Q_OBJECT

public:
	
    ExtGLWidget( QWidget* parent = 0, const char* name = 0 );
    virtual ~ExtGLWidget() {}
		
		double xRotation() const { return xRot_;}  //!< Returns rotation around X axis [-360..360] (some angles are equivalent)
		double yRotation() const { return yRot_;}  //!< Returns rotation around Y axis [-360..360] (some angles are equivalent)
		double zRotation() const { return zRot_;}  //!< Returns rotation around Z axis [-360..360] (some angles are equivalent)

		double xShift() const { return xShift_;} //!< Returns shift along X axis (object coordinates)
		double yShift() const { return yShift_;} //!< Returns shift along Y axis (object coordinates)
		double zShift() const { return zShift_;} //!< Returns shift along Z axis (object coordinates)
		
		double xViewportShift() const { return xVPShift_;} //!< Returns relative shift [-1..1] along X axis (view coordinates)
		double yViewportShift() const { return yVPShift_;} //!< Returns relative shift [-1..1] along Y axis (view coordinates)
		
		double xScale() const { return xScale_;} //!< Returns scaling for X values [0..inf]
		double yScale() const { return yScale_;} //!< Returns scaling for Y values [0..inf]
		double zScale() const { return zScale_;} //!< Returns scaling for Z values [0..inf]

		double zoom() const { return zoom_;} //!< Returns zoom (0..inf)

		bool ortho() const { return ortho_; } //!< Returns orthogonal (true) or perspective (false) projection

		
		void assignMouse(int xrot, int yrot, int zrot,
										 int xscale, int yscale, int zscale,
										 int zoom, int xshift, int yshift);
		
		bool mouseEnabled() const; //!< Returns true, if the widget accept mouse input from the user
		void assignKeyboard(
       int xrot_n, int xrot_p
      ,int yrot_n, int yrot_p
      ,int zrot_n, int zrot_p
			,int xscale_n, int xscale_p 
      ,int yscale_n, int yscale_p
      ,int zscale_n, int zscale_p
			,int zoom_n, int zoom_p
      ,int xshift_n, int xshift_p
      ,int yshift_n, int yshift_p
      );
		
		bool keyboardEnabled() const; //!< Returns true, if the widget accept keyboard input from the user
    //! Sets speed for keyboard driven transformations
    void setKeySpeed(double rot, double scale, double shift); 
    //! Gets speed for keyboard driven transformations
    void keySpeed(double& rot, double& scale, double& shift) const; 
      
    
signals:
		
		//! Emitted, if the rotation is changed
    void rotationChanged( double xAngle, double yAngle, double zAngle ); 
		//! Emitted, if the shift is changed
		void shiftChanged( double xShift, double yShift, double zShift );
		//! Emitted, if the viewport shift is changed
		void vieportShiftChanged( double xShift, double yShift );
		//! Emitted, if the scaling is changed
    void scaleChanged( double xScale, double yScale, double zScale );
		//! Emitted, if the zoom is changed
		void zoomChanged(double);
		//! Emitted, if the projection mode is changed
    void projectionChanged(bool);

public slots:

		void	setRotation( double xVal, double yVal, double zVal ); 																														
		void	setShift( double xVal, double yVal, double zVal );    																														
		void	setViewportShift( double xVal, double yVal );         																														
		void	setScale( double xVal, double yVal, double zVal );    																														
		void	setZoom( double );                                    																														
		                                                              																														
		void  setOrtho(bool);                                       																														
    
		void	enableMouse(bool val=true); //!< Enable mouse input   																														
		void	disableMouse(bool val =true); //!< Disable mouse input																														
		void	enableKeyboard(bool val=true); //!< Enable keyboard input   																														
		void	disableKeyboard(bool val =true); //!< Disable keyboard input																														


protected:
		void mousePressEvent( QMouseEvent *e );
		void mouseReleaseEvent( QMouseEvent *e );
		void mouseMoveEvent( QMouseEvent *e );
		void wheelEvent( QWheelEvent *e );		
    void keyPressEvent( QKeyEvent *e );

private:
    GLdouble xRot_, yRot_, zRot_, xShift_, yShift_, zShift_, zoom_
             , xScale_, yScale_, zScale_, xVPShift_, yVPShift_;
		
		bool ortho_;
		
    // mouse
    
    QPoint lastMouseMovePosition_;
		bool mpressed_;

		int xrot_mstate_, 
				yrot_mstate_, 
				zrot_mstate_, 
				xscale_mstate_, 
				yscale_mstate_, 
				zscale_mstate_,
        zoom_mstate_,
				xshift_mstate_,
				yshift_mstate_;

		bool mouse_input_enabled_;

		void setRotationMouse(ButtonState bstate, double accel, QPoint diff);
		void setScaleMouse(ButtonState bstate, double accel, QPoint diff);
		void setShiftMouse(ButtonState bstate, double accel, QPoint diff);

    // keyboard

		bool kpressed_;

		int xrot_kstate_[2], 
				yrot_kstate_[2], 
				zrot_kstate_[2], 
				xscale_kstate_[2], 
				yscale_kstate_[2], 
				zscale_kstate_[2],
        zoom_kstate_[2],
				xshift_kstate_[2],
				yshift_kstate_[2];

		bool kbd_input_enabled_;
    double kbd_rot_speed_, kbd_scale_speed_, kbd_shift_speed_;

		void setRotationKeyboard(int kseq, double speed);
		void setScaleKeyboard(int kseq, double speed);
		void setShiftKeyboard(int kseq, double speed);
};


} // ns

#endif /* include guard */
