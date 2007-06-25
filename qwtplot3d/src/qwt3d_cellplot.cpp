#if defined(_MSC_VER) /* MSVC Compiler */
#pragma warning ( disable : 4305 )
#pragma warning ( disable : 4786 )
#endif

#include "qwt3d_cellplot.h"
#include "qwt3d_enrichment_std.h"

using namespace std;
using namespace Qwt3D;

/**
Initializes with dataNormals()==false, NOFLOOR, resolution() == 1
*/
CellPlot::CellPlot( QWidget * parent, const QGLWidget * shareWidget)
    : Plot3D( parent, shareWidget) 
{
}

