#ifndef qwt3d_meshplot_h__2005_7_10_begin_guarded_code
#define qwt3d_meshplot_h__2005_7_10_begin_guarded_code

#include "qwt3d_surfaceplot.h"

namespace Qwt3D
{
//! A class representing plots based on points, edges, cells etc.
/**
	A MeshPlot is more general compared with a GridPlot, in providing 
  surface elements built not from a fixed rectangular grid. There is a cost, 
  as usual. In this case, it means efficiency.
*/
class QWT3D_EXPORT MeshPlot : public SurfacePlot
{
public:
  MeshPlot( QWidget * parent = 0, const QGLWidget * shareWidget = 0 );
  virtual ~MeshPlot();

  bool appendDataSet(Qwt3D::TripleField const& data, Qwt3D::CellField const& poly);
  int facets() const;
  //virtual int cells(unsigned dimension) const

protected:
  void createNormals();
  void drawEnrichment(Qwt3D::Enrichment& p);
  void createOpenGlData();

private:
  class MeshData : public Data
  {
  public:
    MeshData() {datatype_p=Qwt3D::POLYGON;}
    ~MeshData();

	  bool empty() const { return cells.empty();}
  	
	  Triple const& operator()(unsigned cellnumber, unsigned vertexnumber);
  	
	  CellField cells;   //!< polygon/cell mesh 
	  TripleField    nodes; //!< point cloud
	  TripleField    normals; //!< mesh normals
  };
  MeshData* data_;

  FLOORSTYLE floorstyle_;
  
  void data2Floor();
  void isolines2Floor();
  void setColorFromVertex(int node, bool skip = false);
};

} // ns
	
#endif /* include guard */
