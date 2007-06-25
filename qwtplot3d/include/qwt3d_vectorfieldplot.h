#ifndef qwt3d_vectorfieldplot_h__2005_12_16_begin_guarded_code
#define qwt3d_vectorfieldplot_h__2005_12_16_begin_guarded_code

#include "qwt3d_plot.h"
//#include "qwt3d_data.h"

namespace Qwt3D
{

//! TODO
class QWT3D_EXPORT VectorFieldPlot : public Plot3D
{
//    Q_OBJECT

public:
  VectorFieldPlot( QWidget * parent = 0, const QGLWidget * shareWidget = 0 );
  ~VectorFieldPlot();

	//bool appendDataSet(Qwt3D::TripleField const& nodes, Qwt3D::EdgeField const& edges);

protected:
	virtual void createOpenGlData();

private:
//  Qwt3D::GraphData* actualData_;
};

} // ns


#endif
