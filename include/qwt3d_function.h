#ifndef __FUNCTIONGENERATOR_H__
#define __FUNCTIONGENERATOR_H__

#include <qstring.h>
#include "qwt3d_global.h"

namespace Qwt3D
{

class Plot3D;

//! Abstract base class for mathematical functions
/**
	A Function encapsulates a mathematical function with rectangular domain. The user has to adapt the pure virtual operator() 
	to get a working object. Also, the client code should call setDomain, setMesh and create for reasonable operating conditions.
*/
class QWT3D_EXPORT Function
{

public:
	
  Function(); //!< Constructs Function object w/o assigned SurfacePlot.
  explicit Function(SurfacePlot* plotWidget); //!< Constructs Function object and assign a SurfacePlot
	virtual ~Function(); //!< Destructor.
	virtual double operator()(double x, double y) = 0; //!< Overwrite this.
	virtual QString name() const { return QString(""); } //!< Issues a String descriptor.
		
	//! Creates data representation for plotWidget or the actual assigned SurfacePlot (if plotWidget == 0).
	virtual bool create(SurfacePlot* plotWidget = 0);

	void setMesh(unsigned int columns, unsigned int rows); //!< Sets number of rows and columns. 
	void setDomain(double minx, double maxx, double miny, double maxy); //!< Sets x-y domain boundaries.
	void setMinZ(double val); //!< Sets minimal z value.
	void setMaxZ(double val); //!< Sets maximal z value.
	void assign(SurfacePlot* plotWidget); //!< Assigns the object to another widget. To see the changes, you have to call this function before create(0).

private:
	SurfacePlot* plotwidget_;
	double minx_, maxx_, miny_, maxy_, minz_, maxz_;
	unsigned int xmesh_, ymesh_;

	void init(SurfacePlot* plotWidget = 0);

};

} // ns

#endif // __FUNCTIONGENERATOR_H__
