#ifndef __FUNCTIONGENERATOR_H__
#define __FUNCTIONGENERATOR_H__

#include <qstring.h>


class QwtPlot3D;

class Function
{

public:
	
  Function(QwtPlot3D* plotWidget);
	virtual ~Function();
	virtual double operator()(double x, double y) = 0;
	virtual QString name() const { return QString(""); }
		
	virtual bool create();

	void setMesh(unsigned int columns,unsigned int rows);
	void setDomain(double minx, double maxx, double miny, double maxy);
	void setMinZ(double val);
	void setMaxZ(double val);

private:
	QwtPlot3D* plotwidget_;
	double minx_, maxx_, miny_, maxy_, minz_, maxz_;
	unsigned int xmesh_, ymesh_;

};


#endif // __FUNCTIONGENERATOR_H__
