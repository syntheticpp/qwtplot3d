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
	
  Function(); //!< Construct Function object w/o assigned SurfacePlot
  explicit Function(SurfacePlot* plotWidget); //!< Construct Function object and assign a SurfacePlot
	virtual ~Function(); 
	virtual double operator()(double x, double y) = 0; //!< Overwrite this
	virtual QString name() const { return QString(""); } //!< Issue a String descriptor
		
	//! Create data representation for plotWidget or the actual assigned SurfacePlot (if plotWidget == 0)
	virtual bool create(SurfacePlot* plotWidget = 0);

	void setMesh(unsigned int columns, unsigned int rows); //!< Set number of rows and columns 
	void setDomain(double minx, double maxx, double miny, double maxy); //!< Set x-y domain boundaries
	void setMinZ(double val); //!< Set minimal z value
	void setMaxZ(double val); //!< Set maximal z value
	void assign(SurfacePlot* plotWidget); //!< Assign to other widget. To see the changes, you have to call this function before create(0)

private:
	SurfacePlot* plotwidget_;
	double minx_, maxx_, miny_, maxy_, minz_, maxz_;
	unsigned int xmesh_, ymesh_;

	void init(SurfacePlot* plotWidget = 0);

};

} // ns

#endif // __FUNCTIONGENERATOR_H__
