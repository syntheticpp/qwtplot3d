#ifndef __READER_H__
#define __READER_H__

#include <float.h>
#include <qstring.h>

#include "qwt3d_global.h"

namespace Qwt3D
{

class SurfacePlot;

//! Object for reading of native files containig grid data
class QWT3D_EXPORT NativeReader
{

public:
		
	explicit NativeReader(SurfacePlot* plotWidget, QString fname = "");

	void setFileName(QString fname) {fileName_ = fname;}
	bool read(double minz = -DBL_MAX, double maxz = DBL_MAX);


	const char* magicstring;

	

private:
	QString fileName_;
	SurfacePlot* plotwidget_;
	bool collectInfo(FILE*& file, unsigned& xmesh, unsigned& ymesh, 
									 double& minx, double& maxx, double& miny, double& maxy);
};


} // ns

#endif
