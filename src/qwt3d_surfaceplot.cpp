#include "qwt3d_surfaceplot.h"

using namespace Qwt3D;

SurfacePlot::SurfacePlot( QWidget* parent, const char* name, MESHTYPE mt )
    : Plot3D( parent, name )
{
	meshtype_ = mt;
	resolution_ = 1;
	
	datanormals_ = false;
	normalLength_ = 0.02;
	normalQuality_ = 3;

		
	actualGridData_ = new GridData();
	actualCellData_ = new CellData();
}

/*!
  Release allocated resources
*/

SurfacePlot::~SurfacePlot()
{
	delete actualGridData_;
	delete actualCellData_;
}

/**
	Calculate the smallest x-y-z parallelepiped enclosing the data.
	It can be accessed by hull();
*/
void 
SurfacePlot::calculateHull()
{
	if (meshtype_ == GRID)
	{
		if (actualGridData_->empty())
			return;
		setHull(actualGridData_->hull());
	}
	else
	{
		if (actualCellData_->empty())
			return;
		setHull(actualCellData_->hull());
	}
}

void 
SurfacePlot::showNormals(bool b)
{
	datanormals_ = b;
}

/**
Values < 0 or > 1 are ignored
*/
void 
SurfacePlot::setNormalLength(double val)
{
	if (val<0 || val>1)
		return;
	normalLength_ = val;
}

/**
Values < 3 are ignored 
*/
void 
SurfacePlot::setNormalQuality(int val) 
{
	if (val<3)
		return;
	normalQuality_ = val;
}

/*!
  Set data resolution (res == 1 original resolution) and updates widget
	If res < 1, the function does nothing
*/
void 
SurfacePlot::setResolution( int res )
{
	if ((resolution_ == res) || res < 1)
		return;
	
	resolution_ = res;
	updateNormals();
	updateData();
	updateGL();

	emit resolutionChanged(res);
}

void 
SurfacePlot::updateNormals()
{
	SaveGlDeleteLists(DisplayLists[NormalObject], 1); 
	
	if (plotStyle() == NOPLOT && !normals())
		return;

	DisplayLists[NormalObject] = glGenLists(1);
	glNewList(DisplayLists[NormalObject], GL_COMPILE);

	if (meshtype() == GRID)
		updateGridNormals();
	else
		updateCellNormals();
		
	glEndList();
}

void 
SurfacePlot::createData()
{
	if (plotStyle() != NOPLOT)
	{
		if (meshtype() == GRID)
			updateGridData();
		else
			updateCellData();
	}
}

void 
SurfacePlot::createFloorData()
{
	if (meshtype() == GRID)
	{
		switch (floorStyle())
		{
		case FLOORDATA:
			GridData2Floor();
			break;
		case FLOORISO:
			GridIsolines2Floor();
			break;
		case FLOORMESH:
			Grid2Floor();
			break;
		default:
			break;
		}
	}
	else // cells
	{
		switch (floorStyle())
		{
		case FLOORDATA:
			CellData2Floor();
			break;
		case FLOORISO:
			CellIsolines2Floor();
			break;
		case FLOORMESH:
			Cell2Floor();
			break;
		default:
			break;
		}
	}
}