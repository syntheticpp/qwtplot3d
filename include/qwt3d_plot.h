#ifndef __plot3d_2003_06_09_12_14__
#define __plot3d_2003_06_09_12_14__

#include "qwt3d_global.h"
#include "qwt3d_coordsys.h"

namespace Qwt3D
{

//! Base class for all plotting widgets
/*!
	Plot3D handles all the common features for plotting widgets - coordinate system, transformations, mouse handling,
	labeling etc.. It contains some pure virtual functions and is, in so far, an abstract base class.
	The class provides interfaces for data handling and implements basic data controlled color allocation.
*/
class QWT3D_EXPORT Plot3D : public QGLWidget
{
    Q_OBJECT

public:
	
    Plot3D( QWidget* parent = 0, const char* name = 0 );
    virtual ~Plot3D();

	  void updateData();
		void createCoordinateSystem(Qwt3D::Triple beg, Qwt3D::Triple end);
		void updateCoordinateSystem();
		CoordinateSystem* coordinates() { return &coord; } //!< \return Pointer to CoordinateSystem object
		ColorLegend* legend() { return &legend_;} //!< \return Pointer to ColorLegend object
		
		double xRotation() const { return xRot_;}  //!< \return Rotation around X axis [-360..360] (some angles are equivalent)
		double yRotation() const { return yRot_;}  //!< \return Rotation around Y axis [-360..360] (some angles are equivalent)
		double zRotation() const { return zRot_;}  //!< \return Rotation around Z axis [-360..360] (some angles are equivalent)

		double xShift() const { return xShift_;} //!< \return Shift along X axis (object coordinates)
		double yShift() const { return yShift_;} //!< \return Shift along Y axis (object coordinates)
		double zShift() const { return zShift_;} //!< \return Shift along Z axis (object coordinates)
		
		double xViewportShift() const { return xVPShift_;} //!< \return Relative shift [-1..1] along X axis (view coordinates)
		double yViewportShift() const { return yVPShift_;} //!< \return Relative shift [-1..1] along Y axis (view coordinates)
		
		double xScale() const { return xScale_;} //!< \return Scaling for X values [0..inf]
		double yScale() const { return yScale_;} //!< \return Scaling for Y values [0..inf]
		double zScale() const { return zScale_;} //!< \return Scaling for Z values [0..inf]

		double zoom() const { return zoom_;} //!< \return Zoom [0..inf]

		bool ortho() const { return ortho_; } //!< \return Orthogonal (true) or perspective (false) projection
		void		setPlotStyle( Qwt3D::PLOTSTYLE val );
		Qwt3D::PLOTSTYLE plotStyle() const { return plotstyle_; }//!< \return Plotting style
		void		setShading( Qwt3D::SHADINGSTYLE val );
		Qwt3D::SHADINGSTYLE shading() const { return shading_; }//!< \return Shading style
		void		setFloorStyle( Qwt3D::FLOORSTYLE val );
		Qwt3D::FLOORSTYLE floorStyle() const { return floorstyle_; }//!< \return Floor style 
		void setIsolines(int isolines);
		int isolines() const { return isolines_;} //!< \return Number of isolines
	
		void setBackgroundColor(Qwt3D::RGBA rgba); //!< Sets widgets background color
		RGBA backgroundRGBAColor() const {return bgcolor_;} //!< \return The widgets background color
		void setMeshColor(Qwt3D::RGBA rgba); //!< Sets color for data mesh
		Qwt3D::RGBA meshColor() const {return meshcolor_;} //!< \return Color for data mesh
		void setMeshLineWidth(double lw); //!< Sets line width for data mesh
		double meshLineWidth() const {return meshLineWidth_;} //!< \return line width for data mesh
		void setDataColor(Color* col); //!< Sets new data color object
		virtual bool hasData() const = 0;
		
		virtual void calculateHull() = 0;
		Qwt3D::ParallelEpiped hull() const { return hull_;} //!< \return rectangular hull \see calculateHull()

		void showColorLegend(bool);
		void updateColorLegend();
		
		void setCoordinateStyle(Qwt3D::COORDSTYLE st); //!< Sets style of coordinate system. See Qwt3D::COORDSTYLE
		void setPolygonOffset(double d);
		double polygonOffset() const {return polygonOffset_;} //!< \return Relative value for polygon offset [0..1] \see setPolygonOffset()
		
		void setTitlePosition(double rely, double relx = 0.5, Qwt3D::ANCHOR = Qwt3D::TopCenter);
		void setTitleFont(const QString& family, int pointSize, int weight = QFont::Normal, bool italic = false);
		void setTitleColor(Qwt3D::RGBA col) {title_.setColor(col);} //!< Set caption color
		void setTitle(const QString& title) {title_.setString(title);} //!< Set caption text (one row only)

		
		void assignMouse(int xrot, int yrot, int zrot,
										 int xscale, int yscale, int zscale,
										 int zoom, int xshift, int yshift);
		
		bool mouseEnabled() const; //!< Returns true, if the widget accept mouse input from the user


signals:
		
		void rotationChanged( double xAngle, double yAngle, double zAngle );
		void shiftChanged( double xShift, double yShift, double zShift );
		void vieportShiftChanged( double xShift, double yShift );
		void scaleChanged( double xScale, double yScale, double zScale );
		void zoomChanged(double);
		
		void projectionChanged(bool);

public slots:

		void		setRotation( double xVal, double yVal, double zVal );
		void		setShift( double xVal, double yVal, double zVal );
		void		setViewportShift( double xVal, double yVal );
		void		setScale( double xVal, double yVal, double zVal );
		void		setZoom( double );
		
		void    setOrtho(bool);
		
		void		enableMouse(bool val=true); //!< Enable mouse input
		void		disableMouse(bool val =true); //!< Disable mouse input
		
		bool    savePixmap(QString fileName, QString format); //!<  Saves content to pixmap format
		bool    saveVector(QString fileName, QString format, bool notext = false, int sorttype = -1); //!<  Saves content to vector format

protected:

		Color* dataColor;
		    
		void		initializeGL();
    void		paintGL();
    void		resizeGL( int w, int h );

		void mousePressEvent( QMouseEvent *e );
		void mouseReleaseEvent( QMouseEvent *e );
		void mouseMoveEvent( QMouseEvent *e );
		void wheelEvent( QWheelEvent *e );		
		
		CoordinateSystem coord;

		virtual void createData() = 0;
		virtual void createFloorData() = 0;
	  
		void createCoordinateSystem();
		void setHull(Qwt3D::ParallelEpiped p) {hull_ = p;}

		enum OBJECTS
		{
			DataObject,
			LegendObject,
			CoordObject,
			NormalObject,
			DisplayListSize
		};
		std::vector<GLuint> DisplayLists;


private:
		
    GLdouble xRot_, yRot_, zRot_, xShift_, yShift_, zShift_, zoom_, xScale_, yScale_, zScale_, xVPShift_, yVPShift_;
		
		Qwt3D::RGBA meshcolor_;
		double meshLineWidth_;
		Qwt3D::RGBA bgcolor_;
		Qwt3D::PLOTSTYLE plotstyle_;
		Qwt3D::SHADINGSTYLE shading_;
		Qwt3D::FLOORSTYLE floorstyle_;
		bool ortho_;
		double polygonOffset_;
		int isolines_;
		bool displaylegend_;

		Qwt3D::ParallelEpiped hull_;

		ColorLegend legend_;

		Label title_;
		Qwt3D::Tuple titlerel_;
		Qwt3D::ANCHOR titleanchor_;

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
};


} // ns
 

#endif  // __plot3d_2003_06_09_12_14__
