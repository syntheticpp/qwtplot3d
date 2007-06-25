#ifndef qwt3d_dynamicgraphplot_h__2005_12_10_begin_guarded_code
#define qwt3d_dynamicgraphplot_h__2005_12_10_begin_guarded_code
	
#include "qwt3d_graphplot.h"
//#include "qwt3d_data.h"

namespace Qwt3D
{

  //! TODO
  class QWT3D_EXPORT DynamicGraphPlot : public GraphPlot
  {
    //    Q_OBJECT

  public:
    DynamicGraphPlot( QWidget * parent = 0, const QGLWidget * shareWidget = 0 );
    ~DynamicGraphPlot();

    //bool appendDataSet(Qwt3D::TripleField const& nodes, Qwt3D::EdgeField const& edges);

  protected:
    //virtual void createOpenGlData();

  private:
    //Qwt3D::GraphData* actualData_;
  };

} // ns


#endif
