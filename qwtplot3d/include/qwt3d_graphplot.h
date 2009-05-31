#ifndef qwt3d_graphplot_h__2004_03_06_01_57_begin_guarded_code
#define qwt3d_graphplot_h__2004_03_06_01_57_begin_guarded_code

#include "qwt3d_plot3d.h"
#include "qwt3d_data.h"

namespace Qwt3D
{

//! TODO
class QWT3D_EXPORT GraphPlot : public Plot3D
{
//    Q_OBJECT

public:
  GraphPlot( QWidget * parent = 0, const QGLWidget * shareWidget = 0 );
  ~GraphPlot();

	bool appendDataSet(Qwt3D::TripleField const& nodes, Qwt3D::EdgeField const& edges);

protected:
	virtual void createOpenGlData();

private:
  class GraphData : public Data
  {
  public:
    GraphData();
    ~GraphData();

    bool empty() const; 

    TripleField nodes;
    EdgeField edges;
  };

  GraphData* data_;
};

} // ns


#endif
