#ifndef qwt3d_gridplot_h__2005_7_10_begin_guarded_code
#define qwt3d_gridplot_h__2005_7_10_begin_guarded_code

#include "qwt3d_surfaceplot.h"

namespace Qwt3D
{
//! A class representing  grid-generated surfaces
/**
  A GridPlot ...
  
*/
class QWT3D_EXPORT GridPlot : public SurfacePlot
{
  Q_OBJECT

public:
  GridPlot( QWidget * parent = 0, const QGLWidget * shareWidget = 0 );
  ~GridPlot();
  int	resolution() const {return resolution_p;} //!< Returns data resolution (1 means all data)

  bool appendDataSet(Qwt3D::Triple** data, unsigned int columns, unsigned int rows,
                     bool uperiodic = false, bool vperiodic = false);
  bool appendDataSet(double** data, unsigned int columns, unsigned int rows,
                     double minx, double maxx, double miny, double maxy);
      
signals:
  void resolutionChanged(int);

public slots:
  void setResolution( int );

protected:  
  void createOpenGlData();
  void drawEnrichment(Qwt3D::Enrichment& p);

  int resolution_p;

private:
  class GridData : public Data
  {
  public:
    GridData();
    GridData(unsigned int columns, unsigned int rows);//!< see setSize()
    ~GridData();

    int columns() const;
    int rows() const;

    bool empty() const { return vertices.empty();}
    void setSize(unsigned int columns, unsigned int rows); //!< destroys content and set new size, elements are uninitialized
    
    DataMatrix vertices;		//!< mesh vertices
    DataMatrix normals;		//!< mesh normals
    void setPeriodic(bool u, bool v) {uperiodic_ = u; vperiodic_ = v;}
    bool uperiodic() const {return uperiodic_;} 
    bool vperiodic() const {return vperiodic_;} 
    Triple point(unsigned int col, unsigned int row) const
    {
      return Triple(vertices[col][row][0],vertices[col][row][1],vertices[col][row][2]);
    }

  private:
    bool uperiodic_, vperiodic_;
    void clear();
  };
  
  GridData* data_;
  void createNormals();
  void data2Floor();
  void isolines2Floor();
  void setColorFromVertex(int ix, int iy, bool skip = false);
  void calcNormals(GridData& gdata);
  void sewPeriodic(GridData& gdata);
  void readIn(GridData& gdata, Triple** data, unsigned int columns, unsigned int rows);
  void readIn(GridData& gdata, double** data, unsigned int columns, unsigned int rows, 
              double minx, double maxx, double miny, double maxy);
};

} // ns

  
#endif /* include guard */
