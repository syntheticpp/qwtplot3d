#include "axis.h"
#include <float.h>


Axis::Axis()
{
	init();
};

Axis::~Axis()
{
}

Axis::Axis(Triple beg, Triple end)
{
	init();
	setPosition(beg,end);
}

void 
Axis::init()
{
	detachAll();

  beg_ = Triple(0.0, 0.0, 0.0);  
  end_ = beg_;
	

	setLimits(0,0);

	setTicOrientation(0.0, 0.0, 0.0);
	setTicLength(0.0, 0.0);
	setColor(0.0, 0.0, 0.0);
	setLineWidth(1.0);
	setMajors(1);	
	setMinors(1);	
	symtics_ = false;
	drawNumbers_ = false;


	drawTics_ = false;
	autoscale_ = true;
	markerLabel_.clear();
	numberfont_ = QFont("Helvetica",12);
	numbercolor_ = RGBA(0,0,0,0);

	setNumberAnchor(LabelPixmap::Center);

	makeSimpleLabels(true);

	label_ = LabelPixmap();
}

void 
Axis::setPosition(const Triple& beg, const Triple& end)
{
	beg_ = beg;
	end_ = end;
}

void 
Axis::setMajors(int val)
{
	if (val == tmaj_)
		return;
	
	tmaj_ = (val<=0) ? 1 : val; // always >= 1

	markerLabel_.clear();
	markerLabel_ = std::vector<LabelPixmap>(tmaj_+1);
}

void 
Axis::setMinors(int val)
{
	tmin_ = (val<=0) ? 1 : val; // always >= 1
}

void 
Axis::setTicLength(double majorl, double minorl)
{
	lmaj_ = majorl;
	lmin_ = minorl;

}

void 
Axis::setTicOrientation(double tx, double ty, double tz)
{
	setTicOrientation(Triple(tx,ty,tz));
}

void 
Axis::setTicOrientation(const Triple& val)
{
	orientation_ = val;
	orientation_.normalize();
}

void
Axis::setLineWidth(double val, double majfac, double minfac)
{
	lineWidth_ = val;
	majLineWidth_ = majfac * lineWidth_;
	minLineWidth_ = minfac * lineWidth_;
}

void 
Axis::draw()
{
	Drawable::draw();

	saveGLState();

//	glEnable(GL_LINE_SMOOTH);
	glLineWidth( lineWidth_ );
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4d(color.r,color.g,color.b,color.a);		

	drawBase();
	drawTics();
	
	restoreGLState();
}

void 
Axis::drawBase()
{

	glBegin( GL_LINES );
	glVertex3f( beg_.x, beg_.y, beg_.z); 
	glVertex3f( end_.x, end_.y, end_.z);
	glEnd();
//
//	GLUquadricObj  *cone;
//	cone = gluNewQuadric();
//  gluCylinder(cone, 0.005, 0.005, 1., 36, 36);
//  gluDeleteQuadric(cone);
}	

void 
Axis::drawTics()
{
	if (!drawTics_)

		return;

	autostart_ = start_;
	autostop_ = stop_;

	buildAutoScale(autostart_, autostop_); // changes tmaj_

	Triple normal = (end_ - beg_);
	normal.normalize();
	Triple beg = beg_ + (autostart_ - start_) * normal;
	Triple end = end_ - (stop_ - autostop_) * normal;

	Triple runningpoint = end - beg;

	int mj;
	
	for (mj = 0; mj <= tmaj_; ++mj) 
	{
		glLineWidth(majLineWidth_);
		double t = double(mj) / tmaj_;
		drawTic(beg + t * runningpoint, lmaj_);
		drawNumber(mj);
		if (t==1.0)
			break;
		glLineWidth(minLineWidth_);
		for (int mi=1; mi < tmin_; ++mi)
		{
			double tt = double(mi)  / (tmin_ * tmaj_);
			drawTic(beg + (t+tt)*runningpoint, lmin_);		
		}
	}

	if (tmin_ < 1)
		return;

	// remaining minor tics
	
	// beg_          beg
	//  |_____________|_____ _ _ _
	
	Triple step = (end-beg) / (tmaj_ * tmin_);
	int ii = 1;
	Triple residuum = beg_ - beg;
	runningpoint = ii * step;
	
	while (runningpoint.length() && (runningpoint.length() < residuum.length())) 
	{ 
		drawTic(beg - runningpoint, lmin_);								
		runningpoint = (++ii) * step;
	}
	
	//     end           end_
	// _ _ _|_____________|

	ii = 1;
	residuum = end_ - end;
	runningpoint = ii * step;
	
	while (runningpoint.length() && (runningpoint.length() < residuum.length())) 
	{ 
		drawTic(end + runningpoint, lmin_);								
		runningpoint = (++ii) * step;
	}	
}

void 
Axis::drawNumber(int mtic)
{
	if (!drawNumbers_ || (mtic < 0) || (mtic >= int(markerLabel_.size())))
		return;
	
	double t = double(mtic) / tmaj_;

	markerLabel_[mtic].setFont(numberfont_.family(), numberfont_.pointSize(), numberfont_.weight(), numberfont_.italic());
	markerLabel_[mtic].setColor(numbercolor_);

	double delta = fabs(autostop_-autostart_) / tmaj_; // prevent rounding errors near zero
	double anumber;
	if (autoScale())
	{
		anumber = (1-t) * autostart_ + t * autostop_;
		if (fabs(anumber) < delta / 10)
			anumber = 0;
		markerLabel_[mtic].setString(QString::number(anumber));		
	}
	else
		markerLabel_[mtic].setString(QString::number(start_ + t * (stop_ - start_)));
}

void 
Axis::postDrawNumber(Triple pos, int mtic)
{
	if (!drawNumbers_ || (mtic < 0) || (mtic >= int(markerLabel_.size())))
		return;
	markerLabel_[mtic].setPosition(pos, scaleNumberAnchor_);
	markerLabel_[mtic].draw();
}


void 
Axis::postDrawNumbers()
{
	Triple normal = (end_ - beg_);
	normal.normalize();
	Triple beg = beg_ + (autostart_ - start_) * normal;
	Triple end = end_ - (stop_ - autostop_) * normal;

	Triple runningpoint = end - beg;

	int mj;
	
	postDrawNumber(beg + 1.2 * lmaj_ * orientation_,0);

	for (mj = 1; mj <= tmaj_; ++mj) 
	{
		glLineWidth(majLineWidth_);
		double t = double(mj) / tmaj_;
		postDrawNumber(beg + t * runningpoint + 1.2 * lmaj_ * orientation_,mj);
	}
}


void 
Axis::drawTic(Triple nadir, double length)
{
	double ilength = (symtics_) ? -length : 0.0;

	glBegin( GL_LINES );
	glVertex3f( nadir.x  + ilength * orientation_.x,
				      nadir.y  + ilength * orientation_.y,
							nadir.z  + ilength * orientation_.z) ; 
	glVertex3f( nadir.x  + length * orientation_.x,
							nadir.y  + length * orientation_.y,
							nadir.z  + length * orientation_.z);
	glEnd();
}

void 
Axis::setNumberFont(QString const& family, int pointSize, int weight, bool italic)
{
	numberfont_ = QFont(family, pointSize, weight, italic );
}

void 
Axis::setNumberFont(QFont const& font)
{
	numberfont_ = font;
}

void 
Axis::setNumberColor(RGBA col)
{
	numbercolor_ = col;
}

void 
Axis::setLabelFont(QString const& family, int pointSize, int weight, bool italic)
{
	label_.setFont(family, pointSize, weight, italic);
}

void 
Axis::setLabelFont(QFont const& font)
{
	setLabelFont(font.family(), font.pointSize(), font.weight(), font.italic());
}

void
Axis::setLabelString(QString const& name)
{
	label_.setString(name);
}

void 
Axis::setLabelPosition(const Triple& pos,LabelPixmap::ANCHOR an)
{
	label_.setPosition(pos, an);
}

void 
Axis::setLabelColor(RGBA col)
{
	label_.setColor(col);
}

/*!
	changes tmaj_
*/
void 
Axis::buildAutoScale (double& a, double& b)
{
 	if (!autoscale_) 
      return;
  
  double delta = stop_ - start_;

  double y = round125(delta/tmaj_);
	a = y * ceil(start_/y);
	b = y * floor(stop_/y);

	setMajors(round((b-a) / y));
}

void 
Axis::postDraw()
{
	saveGLState();

//	glEnable(GL_LINE_SMOOTH);
	glLineWidth( lineWidth_ );
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4d(color.r,color.g,color.b,color.a);		

	postDrawNumbers();
	label_.draw();
	
	restoreGLState();
}
