#ifndef __READER_H__
#define __READER_H__

#include <float.h>
#include <qstring.h>


class QwtPlot3D;

class NativeReader
{

public:
		
	explicit NativeReader(QwtPlot3D* plotWidget, QString fname = "");

	void setFileName(QString fname) {fileName_ = fname;}
	bool read(double minz = -DBL_MAX, double maxz = DBL_MAX);


	const char* magicstring;

	

private:
	QString fileName_;
	QwtPlot3D* plotwidget_;
	bool collectInfo(FILE*& file, unsigned& xmesh, unsigned& ymesh, 
									 double& minx, double& maxx, double& miny, double& maxy);
};




#endif