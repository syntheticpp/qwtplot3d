#ifndef __FUNCTIONGENERATOR_H__
#define __FUNCTIONGENERATOR_H__

#include <qstring.h>

namespace Qwt3D
{

class Plot3D;

class Function
{

public:
	
  Function(Plot3D* plotWidget);
	virtual ~Function();
	virtual double operator()(double x, double y) = 0;
	virtual QString name() const { return QString(""); }
		
	virtual bool create();

	void setMesh(unsigned int columns,unsigned int rows);
	void setDomain(double minx, double maxx, double miny, double maxy);
	void setMinZ(double val);
	void setMaxZ(double val);
	void assign(Plot3D* plotWidget); //!< assign to other widget to see the changes, you have to call create() afterwards

private:
	Plot3D* plotwidget_;
	double minx_, maxx_, miny_, maxy_, minz_, maxz_;
	unsigned int xmesh_, ymesh_;

};

} // ns

#endif // __FUNCTIONGENERATOR_H__
