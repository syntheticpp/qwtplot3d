#if defined(_MSC_VER) /* MSVC Compiler */
#pragma warning ( disable : 4305 )
#endif

#include "plane.h"
using namespace Qwt3D;


Quad::Quad()
{
	setPosition(Triple(), Triple(), Triple(), Triple());
	setColor (0,0,0,0);
}

void 
Quad::setPosition(Triple one, Triple two, Triple three, Triple four)
{
	one_   = one;
	two_   = two;
	three_ = three;
	four_	 = four;
}


void 
Quad::draw()
{
	saveGLState();
		
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

/* todo draw border
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);
	glColor4d(0, 0, 0, 1);
	glBegin(GL_LINE_LOOP);
		glVertex3d( one_.x, one_.y, one_.z );
		glVertex3d( two_.x, two_.y, two_.z );
		glVertex3d( three_.x, three_.y, three_.z );
		glVertex3d( four_.x, four_.y, four_.z );
	glEnd();
*/


	glPolygonMode(GL_FRONT_AND_BACK, GL_QUADS);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(0.2,1.0);
	
	glColor4d(color.r,color.g,color.b,color.a);
	glBegin( GL_POLYGON );
		glVertex3d( one_.x, one_.y, one_.z );
		glVertex3d( two_.x, two_.y, two_.z );
		glVertex3d( three_.x, three_.y, three_.z );
		glVertex3d( four_.x, four_.y, four_.z );
  glEnd();

	restoreGLState();
}


// ColorLegend

ColorLegend::ColorLegend()
:Quad()
{
}

void 
ColorLegend::draw()
{
	if (colors.empty())
		return;

	saveGLState();

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  
	Triple dv2 = (four_-one_) / colors.size(); 
	Triple dv3 = (three_-two_) / colors.size(); 

	for (unsigned i=1; i<=colors.size(); ++i) 
	{
    glColor4d(colors[i-1].r,colors[i-1].g,colors[i-1].b,colors[i-1].a);
	 	glBegin( GL_POLYGON );
			glVertex3d( one_.x+(i-1)*dv2.x, one_.y+(i-1)*dv2.y, one_.z+(i-1)*dv2.z );
			glVertex3d( one_.x+i*dv2.x, one_.y+i*dv2.y, one_.z+i*dv2.z );
			glVertex3d( two_.x+i*dv3.x, two_.y+i*dv3.y, two_.z+i*dv3.z );
			glVertex3d( two_.x+(i-1)*dv3.x, two_.y+(i-1)*dv3.y, two_.z+(i-1)*dv3.z );
		glEnd();
  }

	restoreGLState();
}
