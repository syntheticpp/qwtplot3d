#if defined(_MSC_VER) /* MSVC Compiler */
#pragma warning ( disable : 4305 )
#endif

#include "qwt3d_colorlegend.h"

using namespace Qwt3D;

ColorLegend::ColorLegend()
{
	axis_.setNumbers(true);
	axis_.setScale(true);
	axis_.setNumberColor(RGBA(0,0,0,1));
	axis_.setNumberAnchor(LabelPixmap::CenterRight);
	axis_.setNumberFont(QFont("Courier",8));

	caption.setFont("Courier", 10, QFont::Bold);
	caption.setColor(RGBA(0,0,0,1));
	//axis_.setAutoScale(false);
	axisposition_ = ColorLegend::Left;
	orientation_ = ColorLegend::BottomTop;
}

void 
ColorLegend::setLimits(double start, double stop)
{
	axis_.setLimits(start, stop);
}

void 
ColorLegend::setMajors(int majors)
{
	axis_.setMajors(majors);
}

void 
ColorLegend::setMinors(int minors)
{
	axis_.setMinors(minors);
}


void
ColorLegend::setOrientation(ORIENTATION or, POSITION pos)
{
	orientation_ = or;
	axisposition_ = pos;

	if (or==BottomTop)
	{
		if (pos == Bottom || pos == Top)
			axisposition_ = Left;
	}
	else
	{
		if (pos == Left || pos == Right)
			axisposition_ = Bottom;
	}
			
}

void 
ColorLegend::setGeometry(Tuple relMin, Tuple relMax)
{
	double ot = .99;

	getMatrices(modelMatrix, projMatrix, viewport);
	pe_.minVertex = relativePosition(Triple(relMin.x, relMin.y, ot));
	pe_.maxVertex = relativePosition(Triple(relMax.x, relMax.y, ot));

	double diff;
	Triple b;
	Triple e;

	switch (axisposition_) 
	{
	case ColorLegend::Left:
		b = pe_.minVertex;
		e = pe_.maxVertex; e.x = b.x;
		axis_.setTicOrientation(-1,0,0);
		axis_.setNumberAnchor(LabelPixmap::CenterRight);
		diff = pe_.maxVertex.x - pe_.minVertex.x;
		break;
	case ColorLegend::Right:
		e = pe_.maxVertex;
		b = pe_.minVertex; b.x = e.x; 
		axis_.setTicOrientation(+1,0,0);
		axis_.setNumberAnchor(LabelPixmap::CenterLeft);
		diff = pe_.maxVertex.x - pe_.minVertex.x;
		break;
	case ColorLegend::Top:
		e = pe_.maxVertex;
		b = pe_.minVertex; b.z = e.z; 
		axis_.setTicOrientation(0,0,+1);
		axis_.setNumberAnchor(LabelPixmap::BottomCenter);
		diff = pe_.maxVertex.z - pe_.minVertex.z;
		break;
	case ColorLegend::Bottom:
		b = pe_.minVertex;
		e = pe_.maxVertex; e.z = b.z;
		axis_.setTicOrientation(0,0,-1);
		axis_.setNumberAnchor(LabelPixmap::TopCenter);
		diff = pe_.maxVertex.z - pe_.minVertex.z;
		break;
	default:
		break;
	}
	
	axis_.setPosition(b,e);
	diff /= 10;
	
	axis_.setTicLength(diff, 0.6*diff);

	Triple c;
	c.x = pe_.minVertex.x + ((pe_.maxVertex-pe_.minVertex) / 2).x;
	c.z = pe_.maxVertex.z;
	c.z += (pe_.maxVertex.z-pe_.minVertex.z)/20;
	c.y = pe_.maxVertex.y;

	caption.setPosition(c, LabelPixmap::BottomCenter);	
	caption.update();
}

void 
ColorLegend::draw()
{
	if (colors.empty())
		return;

	saveGLState();
  	
	Triple one = pe_.minVertex;
	Triple two = pe_.maxVertex; 
	
	double h = (orientation_ == ColorLegend::BottomTop)
	         ? (two-one).z / colors.size()
					 : (two-one).x / colors.size(); 

	//glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(0.0, 0.0);

	glColor4d(0, 0, 0, 1);
	glBegin(GL_LINE_LOOP);
		glVertex3d(one.x, one.y, one.z);
		glVertex3d(one.x, one.y, two.z);
		glVertex3d(two.x, one.y, two.z);
		glVertex3d(two.x, one.y, one.z);
	glEnd();


	unsigned size = colors.size();
	RGBA rgb;
	
	if (orientation_ == ColorLegend::BottomTop)
	{
		for (unsigned i=1; i<=size; ++i) 
		{
			rgb = colors[i-1];
			glColor4d(rgb.r,rgb.g,rgb.b,rgb.a);
	 		glBegin( GL_POLYGON );
				glVertex3d( one.x, one.y, one.z+(i-1)*h );
				glVertex3d( one.x, one.y, one.z+i*h );
				glVertex3d( two.x, one.y, one.z+i*h );
				glVertex3d( two.x, one.y, one.z+(i-1)*h );
			glEnd();
		}
	}
	else
	{
		for (unsigned i=1; i<=size; ++i) 
		{
			rgb = colors[i-1];
			glColor4d(rgb.r,rgb.g,rgb.b,rgb.a);
	 		glBegin( GL_POLYGON );
				glVertex3d( one.x+(i-1)*h, one.y, one.z );
				glVertex3d( one.x+i*h, one.y, one.z );
				glVertex3d( one.x+i*h, one.y, two.z );
				glVertex3d( one.x+(i-1)*h, one.y, two.z );
			glEnd();
		}
	}
			
	restoreGLState();
	
	axis_.draw();
	caption.draw();
}
