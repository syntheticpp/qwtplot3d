#ifndef qwt3d_graphplot_h__2004_03_06_01_57_begin_guarded_code
#define qwt3d_graphplot_h__2004_03_06_01_57_begin_guarded_code

#include "qwt3d_cellplot.h"
#include "qwt3d_data.h"

namespace Qwt3D
{

//! TODO
class QWT3D_EXPORT GraphPlot : public CellPlot
{
//    Q_OBJECT

public:
  GraphPlot( QWidget * parent = 0, const QGLWidget * shareWidget = 0 );
  ~GraphPlot();

	//bool appendDataSet(Qwt3D::TripleField const& nodes, Qwt3D::EdgeField const& edges);

protected:
	//virtual void createOpenGlData();

private:
  //Qwt3D::GraphData* actualData_;
};

} // ns


#endif
