#ifndef qwt3d_staticgraphplot_h__2005_12_10_begin_guarded_code
#define qwt3d_staticgraphplot_h__2005_12_10_begin_guarded_code
	
#include "qwt3d_graphplot.h"
//#include "qwt3d_data.h"

namespace Qwt3D
{

//! TODO
class QWT3D_EXPORT StaticGraphPlot : public GraphPlot
{
//    Q_OBJECT

public:
  StaticGraphPlot( QWidget * parent = 0, const QGLWidget * shareWidget = 0 );
  ~StaticGraphPlot();

	bool appendDataSet(Qwt3D::TripleField const& nodes, Qwt3D::EdgeField const& edges);

protected:
	virtual void createOpenGlData();

private:
  class StaticGraphData : public Data
  {
  public:
    StaticGraphData();
    ~StaticGraphData();

    bool empty() const; 

    TripleField nodes;
    EdgeField edges;
  };
  
  StaticGraphData* data_;
};

} // ns


#endif
