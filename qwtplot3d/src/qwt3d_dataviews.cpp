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
	makeCurrent();
  GLStateBewarer dt(GL_DEPTH_TEST, true);
	GLStateBewarer ls(GL_LINE_SMOOTH, true);

	calculateHull();	

	SaveGlDeleteLists(displaylists_p[DataObject], 1); // nur Daten
	
	displaylists_p[DataObject] = glGenLists(1);
	glNewList(displaylists_p[DataObject], GL_COMPILE);
	
  this->createEnrichments();
	this->createData();
		
	glEndList();
}
