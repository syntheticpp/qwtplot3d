#ifndef __DATASOURCE_H__
#define __DATASOURCE_H__

#include "types.h"

class DataSource
{

public:
	
  DataSource();
	virtual ~DataSource();
	
	void setMesh(int xm, int ym);
	void setDomain(double xmin, double xmax, double ymin, double ymax); //!< call createData() afterwards
	void setMinZ(double val); //!< call createData() afterwards
	void setMaxZ(double val); //!< call createData() afterwards
	
	virtual	bool createData(Data& res) = 0;


protected:

	double minx_, maxx_, miny_, maxy_, minz_, maxz_;
	int xmesh_, ymesh_;

	void convert2VertexData(Data& res, GLfloat**& data);
};


#endif // __DATASOURCE_H__
