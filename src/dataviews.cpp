#if defined(_MSC_VER) /* MSVC Compiler */
#pragma warning ( disable : 4305 )
#pragma warning ( disable : 4786 )
#endif

#include "qwt_plot3d.h"

using namespace std;
using namespace Qwt3D;

/*!
  Generate an OpenGL display list for the object to be shown
*/
void 
Plot3D::updateData()
{
	calculateHull();
	updateFloorData();
	
	glDisable(GL_LINE_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	GLStateBewarer ls(GL_LINE_SMOOTH, false);
	GLStateBewarer dt(GL_DEPTH_TEST, true);

	SaveGlDeleteLists(objectList_[DataObject], 1); // nur Daten
	
	if (plotStyle() == NOPLOT && !normals())
		return;

	objectList_[DataObject] = glGenLists(1);
	glNewList(objectList_[DataObject], GL_COMPILE);

		if (plotStyle() != NOPLOT)
		{
			if (meshtype() == GRID)
				updateGridData();
			else
				updateCellData();
		}
		if (meshtype() == GRID)
			updateGridNormals();
		else
			updateCellNormals();

	glEndList();
}



void 
Plot3D::updateFloorData()
{
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LINE_SMOOTH);

	SaveGlDeleteLists(objectList_[FloorObject], 1);
	
	if (floorStyle() == NOFLOOR)
		return;
		
	objectList_[FloorObject] = glGenLists(1);
	glNewList(objectList_[FloorObject], GL_COMPILE);

	if (meshtype() == GRID)
	{
		switch (floorstyle_)
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
		switch (floorstyle_)
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

	glEndList();
}


