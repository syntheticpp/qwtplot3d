#include "coordsys.h"

const double CoordinateSystem::SQRT1_2 = 0.70710678118654752440084436210485;

using namespace std;
using namespace Qwt3d;

CoordinateSystem::CoordinateSystem(Triple first, Triple second, COORDSTYLE st)
{
	axes = std::vector<Axis>(12);
  setStyle(st);
	init(first,second);

	setLineWidth(1.5);
	setAxesColor(RGBA(0,0,0,1));
	setNumberFont("Times", 12);
	setNumberColor(RGBA(0,0.0,0));
}

CoordinateSystem::~CoordinateSystem()
{
	destroy();
}

void
CoordinateSystem::destroy()
{
	for (unsigned i=0; i!=axes.size(); ++i)
		axes[i].setLabelString("");

	detachAll();
}

void
CoordinateSystem::init(Triple first, Triple second)
{
	destroy();
		
	Triple dv = second - first;
	
	setPosition(first, second);
	
	double majl =  dv.length() / 100; // 1 %
	setTicLength(majl, 0.6 * majl);

	axes[X1].setPosition(first, first+Triple(dv.x,    0,     0));												// front bottom x
	axes[Y1].setPosition(first, first+Triple(   0, dv.y,     0));												// bottom left  y
	axes[Z1].setPosition (first+Triple(   0, dv.y, 0), first+Triple(   0, dv.y, dv.z));	// back left z
	axes[X1].setTicOrientation(0,-1,0);
	axes[Y1].setTicOrientation(-1,0,0);
	axes[Z1].setTicOrientation(-1,0,0);
	
	// remaining x axes 
	axes[X2].setPosition(first+Triple( 0,    0, dv.z), first+Triple( dv.x,    0, dv.z));	// front top x
	axes[X3].setPosition(first+Triple( 0, dv.y, dv.z), second);		  											// back top x
	axes[X4].setPosition(first+Triple( 0, dv.y,    0), first+Triple( dv.x, dv.y,    0));	// back bottom x
	axes[X2].setTicOrientation(0,1,0);
	axes[X3].setTicOrientation(0,-1,0);
	axes[X4].setTicOrientation(0,-1,0);
	
	// remaining y axes 
	axes[Y2].setPosition(first+Triple(dv.x, 0,    0), first+Triple(dv.x, dv.y,  0)); // bottom right y
	axes[Y3].setPosition(first+Triple(dv.x, 0, dv.z), second);											 // top right y
	axes[Y4].setPosition(first+Triple(0,    0, dv.z), first+Triple(0,  dv.y, dv.z)); // top left y
	axes[Y2].setTicOrientation(-1,0,0);
	axes[Y3].setTicOrientation(-1,0,0);

	axes[Y4].setTicOrientation (1,0,0);

	// remaining z axes 
	axes[Z2].setPosition(first, first+Triple(   0,    0,  dv.z));												// front left z
	axes[Z4].setPosition(first+Triple(dv.x, dv.y, 0), second );                         // back right z
	axes[Z3].setPosition(first+Triple(dv.x,    0, 0), first+Triple(dv.x,    0, dv.z));	// front right z
	axes[Z2].setTicOrientation(SQRT1_2,SQRT1_2,0);
	axes[Z4].setTicOrientation(-SQRT1_2,-SQRT1_2,0);
	axes[Z3].setTicOrientation(-SQRT1_2,SQRT1_2,0);

	setLabelFont("Times", 14, QFont::Bold);
	
	setStyle(style_);
}

void 
CoordinateSystem::positionateLabel(AXIS num, LabelPixmap::ANCHOR an, QString label)
{
	Axis& ax = axes[num];
//	ax.setLabelString(label);
  Triple diff = ax.end() - ax.begin();
	Triple center = ax.begin() + diff/2;
	
	double fac = 6*(second()-first()).length() / 100;
	ax.setLabelPosition(center +  fac * ax.ticOrientation(), an);
}

void 
CoordinateSystem::draw()
{	
//	saveGLState();
//	glEnable(GL_LINE_SMOOTH);
	Drawable::draw();
//	glDisable(GL_LINE_SMOOTH);
//	restoreGLState();
}

void 
CoordinateSystem::postDraw()
{
	positionateLabel(X1, LabelPixmap::TopCenter, "X1-Axis");
	positionateLabel(Y1, LabelPixmap::CenterRight, "Y1-Axis");
	positionateLabel(Z1, LabelPixmap::CenterRight, "Z1-Axis");
	
	positionateLabel(X2, LabelPixmap::BottomCenter, "X2-Axis");
	positionateLabel(X3, LabelPixmap::BottomCenter, "X3-Axis");
	positionateLabel(X4, LabelPixmap::TopCenter, "X4-Axis");
	
	positionateLabel(Y2, LabelPixmap::CenterLeft, "Y2-Axis");
	positionateLabel(Y3, LabelPixmap::CenterLeft, "Y3-Axis");
	positionateLabel(Y4, LabelPixmap::CenterRight, "Y4-Axis");
	
	positionateLabel(Z2, LabelPixmap::CenterRight, "Z2-Axis");
	positionateLabel(Z3, LabelPixmap::CenterLeft, "Z3-Axis");
	positionateLabel(Z4, LabelPixmap::CenterLeft, "Z4-Axis");

	Drawable::postDraw();
}


void 
CoordinateSystem::setPosition(Triple first, Triple second)
{
	first_ = first;
	second_ = second;
}

void 
CoordinateSystem::setTicLength(double major, double minor)
{
	for (unsigned i=0; i!=axes.size(); ++i)
		axes[i].setTicLength(major, minor);
}

void 
CoordinateSystem::setAutoScale(bool val)
{
	for (unsigned i=0; i!=axes.size(); ++i)
		axes[i].setAutoScale(val);
}

void 
CoordinateSystem::setAxesColor(RGBA val)
{
	for (unsigned i=0; i!=axes.size(); ++i)
		axes[i].setColor(val);
}

void 
CoordinateSystem::setNumberFont(QString const& family, int pointSize, int weight, bool italic)
{
	for (unsigned i=0; i!=axes.size(); ++i)
		axes[i].setNumberFont(family,pointSize,weight,italic);
}

void 
CoordinateSystem::setNumberFont(QFont const& font)
{
	for (unsigned i=0; i!=axes.size(); ++i)
		axes[i].setNumberFont(font);
}

void 
CoordinateSystem::setNumberColor(RGBA val)
{
	for (unsigned i=0; i!=axes.size(); ++i)
		axes[i].setNumberColor( val);
}

void 
CoordinateSystem::setLabelFont(QFont const& font)
{
	for (unsigned i=0; i!=axes.size(); ++i)
		axes[i].setLabelFont(font);
}


void 
CoordinateSystem::setLabelFont(QString const& family, int pointSize, int weight, bool italic)
{
	setLabelFont(QFont(family,pointSize,weight,italic));
}

void 
CoordinateSystem::setLabelColor(RGBA val)
{
	for (unsigned i=0; i!=axes.size(); ++i)
		axes[i].setLabelColor(val);
}

void 
CoordinateSystem::setLineWidth(double val, double majfac, double minfac)
{
	lineWidth_ = val;
	majLineWidth_ = majfac * val;
	minLineWidth_ = minfac * val;
}

void 
CoordinateSystem::setStyle(COORDSTYLE s) 
{ 
	style_ = s;

	switch (s)
	{
		case NOCOORD:
			{
				for (unsigned i=0; i!=axes.size(); ++i)
					detach (&axes[i]);
			}
			break;
		case BOX:
			{
				for (unsigned i=0; i!=axes.size(); ++i)
					attach (&axes[i]);
			}
			break;
		case FRAME:
			{
				for (unsigned i=0; i!=axes.size(); ++i)
					detach (&axes[i]);
				attach(&axes[X1]);
				attach(&axes[Y1]);
				attach(&axes[Z1]);
			}
			break;
		default:
			break;
	}
}
