#include "coordsys.h"


using namespace std;
using namespace Qwt3d;


CoordinateSystem::CoordinateSystem(Triple first, Triple second, COORDSTYLE st)
{
	autodecoration_ = true;
	axes = std::vector<Axis>(12);
  setStyle(st);
	init(first,second);

	setLineWidth(1.5);
	setAxesColor(RGBA(0,0,0,1));
	setNumberFont("Courier", 12);
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
	
	axes[X1].setLimits(first.x, second.x);
	axes[X2].setLimits(first.x, second.x);
	axes[X3].setLimits(first.x, second.x);
	axes[X4].setLimits(first.x, second.x);
	
	axes[Y1].setLimits(first.y, second.y);
	axes[Y2].setLimits(first.y, second.y);
	axes[Y3].setLimits(first.y, second.y);
	axes[Y4].setLimits(first.y, second.y);
	
	axes[Z1].setLimits(first.z, second.z);
	axes[Z2].setLimits(first.z, second.z);
	axes[Z3].setLimits(first.z, second.z);
	axes[Z4].setLimits(first.z, second.z);

	// remaining x axes 
	axes[X2].setPosition(first+Triple( 0,    0, dv.z), first+Triple( dv.x,    0, dv.z));	// front top x
	axes[X3].setPosition(first+Triple( 0, dv.y, dv.z), second);		  											// back top x
	axes[X4].setPosition(first+Triple( 0, dv.y,    0), first+Triple( dv.x, dv.y,    0));	// back bottom x
	axes[X2].setTicOrientation(0,-1,0);
	axes[X3].setTicOrientation(0,1,0);
	axes[X4].setTicOrientation(0,1,0);
	
	// remaining y axes 
	axes[Y2].setPosition(first+Triple(dv.x, 0,    0), first+Triple(dv.x, dv.y,  0)); // bottom right y
	axes[Y3].setPosition(first+Triple(dv.x, 0, dv.z), second);											 // top right y
	axes[Y4].setPosition(first+Triple(0,    0, dv.z), first+Triple(0,  dv.y, dv.z)); // top left y
	axes[Y2].setTicOrientation(1,0,0);
	axes[Y3].setTicOrientation(1,0,0);
	axes[Y4].setTicOrientation (-1,0,0);

	// remaining z axes 
	axes[Z2].setPosition(first, first+Triple(   0,    0,  dv.z));												// front left z
	axes[Z4].setPosition(first+Triple(dv.x, dv.y, 0), second );                         // back right z
	axes[Z3].setPosition(first+Triple(dv.x,    0, 0), first+Triple(dv.x,    0, dv.z));	// front right z
	axes[Z2].setTicOrientation(-1,0,0);
	axes[Z4].setTicOrientation(1,0,0);
	axes[Z3].setTicOrientation(1,0,0);

	setLabelFont("Courier", 14, QFont::Bold);
	
	setStyle(style_);
}

void 
CoordinateSystem::positionateLabel(Axis& ax, LabelPixmap::ANCHOR an)
{
  Triple diff = ax.end() - ax.begin();
	Triple center = ax.begin() + diff/2;
	
	double fac = 6*(second()-first()).length() / 100;
	ax.setLabelPosition(center +  fac * ax.ticOrientation(), an);
	ax.setNumberAnchor(an);
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
	if (autoDecoration())
		chooseAxesForAutoDecoration();

	Drawable::postDraw();
}



//! build convex hull (6 axes: 2 x, 2 y, 2 z) and choose one of them at a time for scales, labels etc.  
void 
CoordinateSystem::chooseAxesForAutoDecoration()
{
	vector<Triple> beg(axes.size());
	vector<Triple> end(axes.size());
	vector<Tuple> src(2*axes.size());

	unsigned i;
	// collect axes viewport coordinates and initialize
	for (i=0; i!=axes.size(); ++i)
	{
		if (style() != NOCOORD)
			attach(&axes[i]);
		
		beg[i] = World2ViewPort(axes[i].begin());
		end[i] = World2ViewPort(axes[i].end());
		src[i] = Tuple(beg[i].x, beg[i].y);
		src[axes.size()+i] = Tuple(end[i].x, end[i].y);

		axes[i].setScale(false);
		axes[i].setNumbers(false);
		axes[i].setLabel(false);
	}

	vector<int> idx;
	convexhull2d(idx,src);

	int rem_x = -1;
	int rem_y = -1;
	int rem_z = -1;


	bool left;

	int choice_x = -1; 
	int	choice_y = -1;
	int choice_z = -1;

  int other_x = -1;
  int other_y = -1;
  int other_z = -1;

	//traverse convex hull
	for (unsigned k=0; k!=idx.size(); ++k)
	{
		Triple one, two;
		
		if (idx[k] >= axes.size()) // is end point
			one = end[idx[k]-axes.size()];
		else                       // is begin point  
			one = beg[idx[k]];

		unsigned int next = idx[(k+1) % idx.size()];  // next point in cv (considered as ring buffer of points)

		if (next >= axes.size()) 
			two = end[next-axes.size()];
		else
			two = beg[next];
		
		for (i=0; i!=axes.size(); ++i)
		{			
			if (
					(one == beg[i] && two == end[i])
					||
					(two == beg[i] && one == end[i])
				 )
			{
				if (i==X1 || i==X2 || i==X3 || i==X4)  // x Achsen
				{
					if (rem_x>=0) // schon zweite Achse der konvexen Huelle ?
					{
						// untere der beiden x Achsen
						double y = min(min(end[rem_x].y,end[i].y),min(beg[rem_x].y,beg[i].y));
						choice_x = (y == beg[i].y || y == end[i].y) ? i : rem_x;
												
						other_x = (choice_x == i) ? rem_x : i;
						left = (beg[choice_x].x < beg[other_x].x || end[choice_x].x < end[other_x].x) 
							? true
							: false;
						
						autoDecorateExposedAxis(axes[choice_x], left);

						rem_x = -1;
					}
					else
					{
						rem_x = i;
					}
				}
				else if (i==Y1 || i==Y2 || i==Y3 || i==Y4)
				{
					if (rem_y>=0)
					{
						// untere der beiden y Achsen
						double y = min(min(end[rem_y].y,end[i].y),min(beg[rem_y].y,beg[i].y));
						choice_y = (y == beg[i].y || y == end[i].y) ? i : rem_y;
						
						other_y = (choice_y == i) ? rem_y : i;
						left = (beg[choice_y].x < beg[other_y].x || end[choice_y].x < end[other_y].x) 
							? true
							: false;
						autoDecorateExposedAxis(axes[choice_y], left);

						rem_y = -1;
					}
					else
					{
						rem_y = i;
					}
				}
				else if (i==Z1 || i==Z2 || i==Z3 || i==Z4)
				{
					if (rem_z>=0)
					{
						// hintere der beiden z Achsen
						double z = max(max(end[rem_z].z,end[i].z),max(beg[rem_z].z,beg[i].z));
						choice_z = (z == beg[i].z || z == end[i].z) ? i : rem_z;

						other_z = (choice_z == i) ? rem_z : i;
												
						rem_z = -1;

					}
					else
					{
						rem_z = i;
					}
				}
			}
		} // for axes
	} // for idx

	// fit z axis in - the onthewall axis if the decorated axes build a continous line, the opposite else 
	if (choice_x>=0 && choice_y>=0 && choice_z>=0)
	{
		left = (beg[choice_z].x < beg[other_z].x || end[choice_z].x < end[other_z].x) 
			? true
			: false;
		

		if (
					axes[choice_z].begin() == axes[choice_x].begin() 
			||	axes[choice_z].begin() == axes[choice_x].end()
			||	axes[choice_z].begin() == axes[choice_y].begin() 
			||	axes[choice_z].begin() == axes[choice_y].end()
			||	axes[choice_z].end() == axes[choice_x].begin() 
			||	axes[choice_z].end() == axes[choice_x].end()
			||	axes[choice_z].end() == axes[choice_y].begin() 
			||	axes[choice_z].end() == axes[choice_y].end()
			
			)
		{
			autoDecorateExposedAxis(axes[choice_z], left);
		}

		else
		{
			autoDecorateExposedAxis(axes[other_z], !left);
			choice_z = other_z; // for FRAME
		}
	}
	
	if (style() == FRAME)
	{
		for (i=0; i!=axes.size(); ++i)
		{
			if (i!=choice_x && i!=choice_y && i!=choice_z)
				detach(&axes[i]);
		}
	}

}


void 
CoordinateSystem::autoDecorateExposedAxis(Axis& ax, bool left)
{
	Triple diff = World2ViewPort(ax.end()) - World2ViewPort(ax.begin());

	diff = Triple(diff.x,diff.y,0); // projection
	
	double s = diff.length();
	
	if (!s)
		return;

	ax.setScale(true);
	ax.setNumbers(true);
	ax.setLabel(true);

	const double SQRT_2 = 0.7071067;
	double sina = fabs(diff.y / s);


	if (left) // leftmost (compared with antagonist in CV)  axis -> draw decorations on the left side
	{
		if ( diff.x >= 0 && diff.y >= 0 && sina < SQRT_2)          // 0..Pi/4 
		{
			positionateLabel(ax, LabelPixmap::BottomCenter);
			ax.setNumberAnchor(LabelPixmap::BottomCenter);
		}
		else if ( diff.x >= 0 && diff.y >= 0  && !left)            // octant 2
		{
			positionateLabel(ax, LabelPixmap::CenterRight);
			ax.setNumberAnchor(LabelPixmap::CenterRight);
		}
		else if ( diff.x <= 0 && diff.y >= 0  && sina >= SQRT_2)    // octant 3
		{
			positionateLabel(ax, LabelPixmap::CenterRight);
			ax.setNumberAnchor(LabelPixmap::CenterRight);
		}
		else if ( diff.x <= 0 && diff.y >= 0 )                      // octant 4
		{
			positionateLabel(ax, LabelPixmap::TopCenter);
			ax.setNumberAnchor(LabelPixmap::TopCenter);
		}
		else if ( diff.x <= 0 && diff.y <= 0  && sina <= SQRT_2)    // octant 5
		{
			positionateLabel(ax, LabelPixmap::BottomCenter);
			ax.setNumberAnchor(LabelPixmap::BottomCenter);
		}
		else if ( diff.x <= 0 && diff.y <= 0)                      // octant 6
		{
			positionateLabel(ax, LabelPixmap::CenterRight);
			ax.setNumberAnchor(LabelPixmap::CenterRight);
		}
		else if ( diff.x >= 0 && diff.y <= 0  && sina >= SQRT_2)    // octant 7
		{
			positionateLabel(ax, LabelPixmap::CenterRight);
			ax.setNumberAnchor(LabelPixmap::CenterRight);
		}
		else if ( diff.x >= 0 && diff.y <= 0)                      // octant 8
		{
			positionateLabel(ax, LabelPixmap::TopCenter);
			ax.setNumberAnchor(LabelPixmap::TopCenter);
		}
	}	
	else // rightmost axis
	{
		if ( diff.x >= 0 && diff.y >= 0 && sina <= SQRT_2)
		{
			positionateLabel(ax, LabelPixmap::TopCenter);
			ax.setNumberAnchor(LabelPixmap::TopCenter);
		}
		else if ( diff.x >= 0 && diff.y >= 0  && !left) 
		{
			positionateLabel(ax, LabelPixmap::CenterLeft);
			ax.setNumberAnchor(LabelPixmap::CenterLeft);
		}
		else if ( diff.x <= 0 && diff.y >= 0  && sina >= SQRT_2) 
		{
			positionateLabel(ax, LabelPixmap::CenterLeft);
			ax.setNumberAnchor(LabelPixmap::CenterLeft);
		}
		else if ( diff.x <= 0 && diff.y >= 0) 
		{
			positionateLabel(ax, LabelPixmap::BottomCenter);
			ax.setNumberAnchor(LabelPixmap::BottomCenter);
		}
		else if ( diff.x <= 0 && diff.y <= 0  && sina <= SQRT_2) 
		{
			positionateLabel(ax, LabelPixmap::TopCenter);
			ax.setNumberAnchor(LabelPixmap::TopCenter);
		}
		else if ( diff.x <= 0 && diff.y <= 0) 
		{
			positionateLabel(ax, LabelPixmap::CenterLeft);
			ax.setNumberAnchor(LabelPixmap::CenterLeft);
		}
		else if ( diff.x >= 0 && diff.y <= 0  && sina >= SQRT_2) 
		{
			positionateLabel(ax, LabelPixmap::CenterLeft);
			ax.setNumberAnchor(LabelPixmap::CenterLeft);
		}
		else if ( diff.x >= 0 && diff.y <= 0) 
		{
			positionateLabel(ax, LabelPixmap::BottomCenter);
			ax.setNumberAnchor(LabelPixmap::BottomCenter);
		}
	}
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
//				attach(&axes[X1]);
//				attach(&axes[Y1]);
//				attach(&axes[Z1]);
			}
			break;
		default:
			break;
	}
}
