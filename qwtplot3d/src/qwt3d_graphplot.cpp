#if defined(_MSC_VER) /* MSVC Compiler */
/* 'identifier' : truncation from 'type1' to 'type2' */
#pragma warning ( disable : 4305 ) 
#endif


#include "qwt3d_graphplot.h"

using namespace std;
using namespace Qwt3D;

GraphPlot::GraphPlot( QWidget * parent, const QGLWidget * shareWidget)
    : CellPlot( parent, shareWidget) 
{
//   actualData_ = new GraphData();
//   actualData_p = actualData_;
}

GraphPlot::~GraphPlot()
{
  //delete actualData_;
}

