#ifndef qwt3d_cellplot_h__2005_04_17_19_03_begin_guarded_code
#define qwt3d_cellplot_h__2005_04_17_19_03_begin_guarded_code

#include "qwt3d_plot.h"

namespace Qwt3D
{
//! A class representing plots based on points, edges, polygons etc.
/**
	CellPlot covers the mathematical concept of dimension-dependent 'cells'. 
  For our purposes, cells are the following:
  \li 0-cell <-> point
  \li 1-cell <-> edge
  \li 2-cell <-> filled closed polygon    (surface element)
  \li 3-cell <-> filled closed polyhedron (volume element)
  
  Inherited classes specify the concept more precisely, e.g. SurfacePlot implements 
  specific sets of 0-,1- and 2-cells usually associated with surfaces. GraphPlot provides 
  visualization for mathematical graphs being composed from 0- and 1-cells 
  (nodes and edges respectively).   
*/
class QWT3D_EXPORT CellPlot : public Plot3D
{
public:
  CellPlot( QWidget * parent = 0, const QGLWidget * shareWidget = 0 );
};

} // ns

#endif /* include guard */
