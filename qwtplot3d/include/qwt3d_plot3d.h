#ifndef __plot3d_2003_06_09_12_14__
#define __plot3d_2003_06_09_12_14__

#include "qwt3d_data.h"
#include "qwt3d_extglwidget.h"
#include "qwt3d_coordsys.h"
#include "qwt3d_enrichment_std.h"

namespace Qwt3D
{
  
//! Base class for all plotting widgets
/*!
  Plot3D handles all the common features for plotting widgets beyond the low-level extensions of 
  ExtGLWidget - coordinate system, labeling and more. It contains some pure virtual functions and is - 
  in this respect - an abstract base class. It's nevertheless no pure interface.
	The class provides interfaces for basic data controlled color allocation.
*/
class QWT3D_EXPORT Plot3D : public ExtGLWidget
{
  Q_OBJECT

public:
  Plot3D ( QWidget * parent = 0, const QGLWidget * shareWidget = 0 );
  virtual ~Plot3D();

  QPixmap renderPixmap (int w=0, int h=0, bool useContext=false);	  
	void updateData(); //!< Recalculate data
	void createCoordinateSystem(Qwt3D::Triple beg, Qwt3D::Triple end);
	Qwt3D::CoordinateSystem* coordinates() { return &coordinates_p; } //!< Returns pointer to CoordinateSystem object
	Qwt3D::ColorLegend* legend() { return &legend_;} //!< Returns pointer to ColorLegend object
	
	void setPlotStyle( Qwt3D::PLOTSTYLE val); //!< Set plotting style
	Qwt3D::Enrichment* setPlotStyle( Qwt3D::Enrichment const& val);
	Qwt3D::PLOTSTYLE plotStyle() const { return plotstyle_; }//!< Returns plotting style
  //! Returns current Enrichment object used for plotting styles (if set, zero else)
  Qwt3D::Enrichment* userStyle() const { return userplotstyle_p; }
  void setShading( Qwt3D::SHADINGSTYLE val ); //!<Set shading style
	Qwt3D::SHADINGSTYLE shading() const { return shading_; }//!< Returns shading style
  //! Set number of isolines. The lines are equidistant between minimal and maximal Z value
  void setIsolines(unsigned steps) {isolinesZ_p.resize(steps); delayisolinecalculation_p = true;}
  //! Set user-defined isoline vector; 
  void setIsolines(const std::vector<double>& val) {isolinesZ_p = val; delayisolinecalculation_p = false;}
  unsigned isolines() const { return isolinesZ_p.size();} //!< Returns number of isolines

  void setSmoothMesh(bool val) {smoothdatamesh_p = val;} //!< Enables/disables smooth data mesh lines. Default is false
  bool smoothDataMesh() const {return smoothdatamesh_p;} //!< True if mesh antialiasing is on
	void setBackgroundColor(Qwt3D::RGBA rgba); //!< Sets widgets background color
	Qwt3D::RGBA backgroundRGBAColor() const {return bgcolor_;} //!< Returns the widgets background color
	void setMeshColor(Qwt3D::RGBA rgba); //!< Sets color for data mesh
	Qwt3D::RGBA meshColor() const {return meshcolor_;} //!< Returns color for data mesh
	void setMeshLineWidth(double lw); //!< Sets line width for data mesh
	double meshLineWidth() const {return meshLineWidth_;} //!< Returns line width for data mesh
	void setDataColor(Color* col); //!< Sets new data color object
  const Color* dataColor() const {return datacolor_p;} //!< Returns data color object

  virtual Qwt3D::Enrichment* addEnrichment(Qwt3D::Enrichment const&); //!< Add an Enrichment
  virtual bool degrade(Qwt3D::Enrichment*); //!< Remove an Enrichment

	Qwt3D::ParallelEpiped hull() const { return hull_;} //!< Returns rectangular hull   

	void showColorLegend(bool);
	
	void setCoordinateStyle(Qwt3D::COORDSTYLE st); //!< Sets style of coordinate system.
	void setPolygonOffset(double d);
	double polygonOffset() const {return polygonOffset_;} //!< Returns relative value for polygon offset [0..1]
	
	void setTitlePosition(double rely, double relx = 0.5, Qwt3D::ANCHOR = Qwt3D::TopCenter);
	void setTitleFont(const QString& family, int pointSize, int weight = QFont::Normal, bool italic = false);
	void setTitleColor(Qwt3D::RGBA col) {title_.setColor(col);} //!< Set caption color
	void setTitle(const QString& title) {title_.setString(title);} //!< Set caption text (one row only)

	
  //! Returns true if valid data available, false else
  bool hasData() const { return (actualData_p) ? !actualData_p->empty() : false;}

public slots:
	virtual bool save(QString const& fileName, QString const& format); //!<  Saves content

protected:
  typedef std::list<Qwt3D::Enrichment*> EnrichmentList;
  typedef EnrichmentList::iterator ELIT;
  
	void initializeGL();
  void paintGL();
  void resizeGL( int w, int h );

  Qwt3D::CoordinateSystem coordinates_p;
  Qwt3D::Color* datacolor_p;
  Qwt3D::Enrichment* userplotstyle_p;
  EnrichmentList elist_p;

  virtual	void calculateHull();
  virtual void createOpenGlData() = 0;
  virtual void drawEnrichment(Qwt3D::Enrichment&){}
  virtual void drawEnrichments();

	void createCoordinateSystem();
	void setHull(Qwt3D::ParallelEpiped const& h) {hull_ = h;}

	enum OBJECTS
	{
		DataObject,
		LegendObject,
		NormalObject,
		DisplayListSize // only to have a vector length ...
	};
	std::vector<GLuint> displaylists_p;
  Qwt3D::Data* actualData_p;
  std::vector<double> isolinesZ_p;
  bool delayisolinecalculation_p;


private:
	Qwt3D::RGBA meshcolor_;
	double meshLineWidth_;
	Qwt3D::RGBA bgcolor_;
	Qwt3D::PLOTSTYLE plotstyle_;
	Qwt3D::SHADINGSTYLE shading_;
	double polygonOffset_;
	bool displaylegend_;
  bool smoothdatamesh_p;

	Qwt3D::ParallelEpiped hull_;

  Qwt3D::ColorLegend legend_;

	Label title_;
	Qwt3D::Tuple titlerel_;
	Qwt3D::ANCHOR titleanchor_;
  bool renderpixmaprequest_;
};


} // ns
 

#endif
