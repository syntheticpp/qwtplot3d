#ifndef GLTEXOBJ_H
#define GLTEXOBJ_H

#include "coordsys.h"


class QwtPlot3D : public QGLWidget
{
    Q_OBJECT

public:

		//! Not really used at this point, provided for future extensions
		enum MESHTYPE
		{
			GRID,
      TRIANGLE,
			QUAD,
			POLYGON
		};	
	
    QwtPlot3D( QWidget* parent = 0, const char* name = 0, MESHTYPE  = GRID );
    virtual ~QwtPlot3D();

	  void updateData();
		void createCoordinateSystem(Triple beg, Triple end);
		void updateCoordinateSystem();
		CoordinateSystem* coordinates() { return &coord; } //!< \return Pointer to CoordinateSystem object
		
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
		int			resolution() const {return resolution_;} //!< \return data resolution (1 means all data)
		void		setPlotStyle( Qwt3D::PLOTSTYLE val );
		Qwt3D::PLOTSTYLE plotStyle() const { return plotstyle_; }//!< \return Plotting style
		void		setFloorStyle( Qwt3D::FLOORSTYLE val );
		Qwt3D::FLOORSTYLE floorStyle() const { return floorstyle_; }//!< \return Floor style 
		void setIsolines(int isolines);
		int isolines() const { return isolines_;} //!< \return Number of isolines
		
		void setBackgroundColor(RGBA rgba);
		void setMeshColor(RGBA rgba);
		RGBA meshColor() const {return meshcolor_;} //!< \return Color for data mesh
		void setDataColor(Color* col);
		void modifyStandardColorAlpha(double d);
		bool hasData() const { return !actualGridData_->empty() || !actualCellData_->empty() ; } //!< \return Valid data available (true) or not (false)
		MESHTYPE meshtype() const { return meshtype_; }	

		void calculateHull();
		ParallelEpiped hull() const { return hull_;} //!< \return rectangular hull \see calculateHull()

		void showColorLegend(bool);
		void createColorLegend(ColorVector const&, Triple a = Triple(), Triple b = Triple(), Triple c = Triple(), Triple d = Triple());

		void setCoordinateStyle(Qwt3D::COORDSTYLE st);
		void setPolygonOffset(double d);
		double polygonOffset() const {return polygonOffset_;} //!< \return Relative value for polygon offset [0..1] \see setPolygonOffset()
		
		void setCaptionPosition(double rely, double relx = 0.5, LabelPixmap::ANCHOR = LabelPixmap::TopCenter);
		void setCaptionFont(const QString& family, int pointSize, int weight = QFont::Normal, bool italic = false);
		void setCaptionColor(RGBA col) {title_.setColor(col);} //!< Set caption color
		void setTitle(const QString& title) {title_.setString(title);} //!< Set caption text (one row only)

		
		void assignMouse(int xrot, int yrot, int zrot,
										 int xscale, int yscale, int zscale,
										 int zoom, int xshift, int yshift);
		
		bool mouseEnabled() const; //!< Returns true, if the widget accept mouse input from the user

		bool createDataRepresentation(double** data, unsigned int columns, unsigned int rows
																			,double minx, double maxx, double miny, double maxy);
		
		bool createDataRepresentation(TripleVector const& data, Tesselation const& poly, MESHTYPE mtype = POLYGON);


signals:
		
		void rotationChanged( double xAngle, double yAngle, double zAngle );
		void shiftChanged( double xShift, double yShift, double zShift );
		void vieportShiftChanged( double xShift, double yShift );
		void scaleChanged( double xScale, double yScale, double zScale );
		void zoomChanged(double);
		
		void resolutionChanged(int);
		void projectionChanged(bool);

public slots:

		void		setRotation( double xVal, double yVal, double zVal );
		void		setShift( double xVal, double yVal, double zVal );
		void		setViewportShift( double xVal, double yVal );
		void		setScale( double xVal, double yVal, double zVal );
		void		setZoom( double );
		
		void    setOrtho(bool);
		void		setResolution( int );

		void enableMouse(bool val=true); //!< Enable mouse input
		void disableMouse(bool val =true); //!< Disable mouse input

		bool    saveContent(QString fileName, QString format);


protected:
		
		//! No copies
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
	  void updateGridData();
	  void updateCellData();

		void updateFloorData();
		void GridData2Floor();
		void GridIsolines2Floor();
		void Grid2Floor();
		void CellData2Floor();
		void CellIsolines2Floor();
		void Cell2Floor();

		int resolution_;
		RGBA meshcolor_;
		RGBA bgcolor_;
		Qwt3D::PLOTSTYLE plotstyle_;
		Qwt3D::FLOORSTYLE floorstyle_;
		bool ortho_;
		double polygonOffset_;
		int isolines_;


		enum OBJECTS
		{
			DataObject				= 0,
			LegendObject			= 1,
			CoordObject				= 2,
			FloorObject       = 3
		};

		std::vector<GLuint> objectList_;

		GridData* actualGridData_;
		CellData* actualCellData_;

		ParallelEpiped hull_;

		Color* dataColor_;
		ColorLegend legend_;

		LabelPixmap title_;
		double titlerelx_, titlerely_;
		LabelPixmap::ANCHOR titleanchor_;

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
