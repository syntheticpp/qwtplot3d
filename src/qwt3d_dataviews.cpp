#if defined(_MSC_VER) /* MSVC Compiler */
#pragma warning ( disable : 4305 )
#pragma warning ( disable : 4786 )
#endif

#include "qwt3d_plot.h"

using namespace std;
using namespace Qwt3D;

/*!
  Update OpenGL data representation
*/
void Plot3D::updateData()
{
	GLStateBewarer dt(GL_DEPTH_TEST, true);
	GLStateBewarer ls(GL_LINE_SMOOTH, true);

	calculateHull();	

	SaveGlDeleteLists(DisplayLists[DataObject], 1); // nur Daten
	
	DisplayLists[DataObject] = glGenLists(1);
	glNewList(DisplayLists[DataObject], GL_COMPILE);

	this->createFloorData();
	
	if (plotStyle() != NOPLOT)
		this->createData();
		
	glEndList();
}
