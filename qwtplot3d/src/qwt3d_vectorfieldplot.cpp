#if defined(_MSC_VER) /* MSVC Compiler */
/* 'identifier' : truncation from 'type1' to 'type2' */
#pragma warning ( disable : 4305 ) 
#endif


#include "qwt3d_vectorfieldplot.h"

using namespace std;
using namespace Qwt3D;

VectorFieldPlot::VectorFieldPlot( QWidget * parent, const QGLWidget * shareWidget)
    : Plot3D( parent, shareWidget) 
{
}

VectorFieldPlot::~VectorFieldPlot()
{
//  delete actualData_;
}

void VectorFieldPlot::createOpenGlData()
{
  if (plotStyle() == NOPLOT)
    return;


}
