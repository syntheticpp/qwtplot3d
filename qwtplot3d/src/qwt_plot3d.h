#ifndef GLTEXOBJ_H
#define GLTEXOBJ_H

#include "coordsys.h"


class QwtPlot3D : public QGLWidget
{
    Q_OBJECT

public:

		enum MESHTYPE
		{
			REGULAR,
      IRREGULAR
		};	
	
    QwtPlot3D( QWidget* parent = 0, const char* name = 0, MESHTYPE  = REGULAR );
    virtual ~QwtPlot3D();

		void updateCoordinates();
	  void updateData();
		void createCoordinateSystem(Triple beg, Triple end);
		
		//! \name Movements
		//@{
		double xRotation() const { return xRot_;}
		double yRotation() const { return yRot_;}
		double zRotation() const { return zRot_;}

		double xShift() const { return xShift_;}
		double yShift() const { return yShift_;}
		double zShift() const { return zShift_;}
		
		double xViewportShift() const { return xVPShift_;}
		double yViewportShift() const { return yVPShift_;}
		
		double xScale() const { return xScale_;}
		double yScale() const { return yScale_;}
		double zScale() const { return zScale_;}

		double zoom() const { return zoom_;}
		//@}

		bool ortho() const { return ortho_; } 
		void		setPlotStyle( Qwt3d::PLOTSTYLE val );
		Qwt3d::PLOTSTYLE plotStyle() const { return plotstyle_; } 
		void		setFloorStyle( Qwt3d::FLOORSTYLE val );
		Qwt3d::FLOORSTYLE floorStyle() const { return floorstyle_; } 
		void setIsolines(int isolines);
		int isolines() const { return isolines_;}
		
		void setBackgroundColor(RGBA rgba);
		void setMeshColor(RGBA rgba);
		RGBA meshColor() const {return meshcolor_;}
		void setDataColor(Color* col);
		void modifyStandardColorAlpha(double d);
		bool hasData() const { return !actualData_.empty(); }

		void calculateHull();
		Triple hullFirst() const { return datafirst_;}
		Triple hullSecond() const { return datasecond_;}

		void showColorLegend(bool);
		void createColorLegend(ColorVector const&, Triple a = Triple(), Triple b = Triple(), Triple c = Triple(), Triple d = Triple());

		void setCoordinateStyle(Qwt3d::COORDSTYLE st);
		void setPolygonOffset(double d);
		double polygonOffset() const {return polygonOffset_;}
		CoordinateSystem* coordinates() { return &coord; }
		
		void setCaptionPosition(double rely, double relx = 0.5, LabelPixmap::ANCHOR = LabelPixmap::TopCenter);
		void setCaptionFont(const QString& family, int pointSize, int weight = QFont::Normal, bool italic = false);
		void setCaptionColor(RGBA col) {title_.setColor(col);}
		void setTitle(const QString& title) {title_.setString(title);}

		
		void assignMouse(int xrot, int yrot, int zrot,
										 int xscale, int yscale, int zscale,
										 int zoom, int xshift, int yshift);
		
		bool mouseEnabled() const; //!< returns true, if the widget accept mouse input from the user

		void createInternalRepresentation(double** data, unsigned int columns, unsigned int rows
																			,double minx, double maxx, double miny, double maxy);

		GridData const& data() const {return actualData_;}

signals:
		
		void rotationChanged( double xAngle, double yAngle, double zAngle );
		void shiftChanged( double xShift, double yShift, double zShift );
		void vieportShiftChanged( double xShift, double yShift );
		void scaleChanged( double xScale, double yScale, double zScale );
		void zoomChanged(double);
		
		void resolutionChanged(double);
		void projectionChanged(bool);

public slots:

		void		setRotation( double xAngle, double yAngle, double zAngle );
		void		setShift( double xShift, double yShift, double zShift );
		void		setViewportShift( double xShift, double yShift );
		void		setScale( double x, double y, double z);
		void		setZoom( double );
		
		void    setOrtho(bool);
		void		setResolution( int );

		void enableMouse(bool val=true); //!< enable mouse input
		void disableMouse(bool val =true); //!< disable mouse input

		bool    dump(QString fileName, QString format); //!< Dumps grabbed frame buffer to file


protected:
		
		//! no copies
		QwtPlot3D(QwtPlot3D const&);
		QwtPlot3D& operator=(QwtPlot3D const&);
    
		void		initializeGL();
    void		paintGL();
    void		resizeGL( int w, int h );

		void mousePressEvent( QMouseEvent *e );
		void mouseReleaseEvent( QMouseEvent *e );
		void mouseMoveEvent( QMouseEvent *e );
		void wheelEvent( QWheelEvent *e );		
		
		CoordinateSystem coord;


private:

		MESHTYPE meshtype_;
		
    GLdouble xRot_, yRot_, zRot_, xShift_, yShift_, zShift_, zoom_, xScale_, yScale_, zScale_, xVPShift_, yVPShift_;
		
		void createCoordinateSystem();
		void updateFloorData();

		int resolution_;
		RGBA meshcolor_;
		RGBA bgcolor_;
		Qwt3d::PLOTSTYLE plotstyle_;
		Qwt3d::FLOORSTYLE floorstyle_;
		bool ortho_;
		double polygonOffset_;
		int isolines_;


		enum OBJECTS
		{
			DataObject				= 0,
			LegendObject			= 1,
			CoordSystemObject = 2,
			FloorObject
		};

		std::vector<GLuint> objectList_;

		GridData actualData_;
		Triple datafirst_, datasecond_;

		Color* dataColor_;
		ColorLegend legend_;

		LabelPixmap title_;
		double titlerelx_, titlerely_;
		LabelPixmap::ANCHOR titleanchor_;

		void calcFloorListAsData();
		void calcFloorListAsIsolines();

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


		void SaveGlDeleteLists(GLuint& list, GLsizei range)
		{
			if (glIsList(list))
				glDeleteLists(list, range);
			list = 0;
		}



};
 
#endif // GLTEXOBJ_H
