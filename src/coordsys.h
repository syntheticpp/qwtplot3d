#ifndef __COORDSYS_H__
#define __COORDSYS_H__

#include <vector>
#include "axis.h"
#include "plane.h"

class CoordinateSystem : public Drawable
{

public:
	
	explicit CoordinateSystem(Triple blb = Triple(0,0,0), Triple ftr = Triple(0,0,0), Qwt3d::COORDSTYLE = Qwt3d::BOX);
  ~CoordinateSystem();	
	
	void init(Triple beg = Triple(0,0,0), Triple end = Triple(0,0,0));
	void setStyle(Qwt3d::COORDSTYLE s);
	Qwt3d::COORDSTYLE style() const { return style_;}
	void setPosition(Triple first, Triple second); // first == front_left_bottom, second == back_right_top
	
	void setAxesColor(RGBA val);
	void setNumberFont(QString const& family, int pointSize, int weight = QFont::Normal, bool italic = false);
	void setNumberFont(QFont const& font);
	void setNumberColor(RGBA val);
	
	void setLabelFont(QString const& family, int pointSize, int weight = QFont::Normal, bool italic = false);
	void setLabelFont(QFont const& font);
	void setLabelColor(RGBA val);

	void setLineWidth(double val, double majfac = 1, double minfac = 0.6);
	void setTicLength(double major, double minor);
	void setAutoScale(bool val = true);

	Triple first() const { return first_;}
	Triple second() const { return second_;}

	void draw();
	void postDraw();

	std::vector<Axis> axes;

private:

	void destroy();
	
	Triple first_, second_;
	Qwt3d::COORDSTYLE style_;
	
	double ared_, agreen_, ablue_; 
	double lineWidth_, majLineWidth_, minLineWidth_;

	void positionateLabel(Qwt3d::AXIS num, LabelPixmap::ANCHOR an, QString name = QString());
	
	static const double SQRT1_2;
};

#endif