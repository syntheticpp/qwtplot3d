#ifndef __COORDSYS_H__
#define __COORDSYS_H__

#include "axis.h"
#include "plane.h"

namespace Qwt3D
{

//! A coordinate system with different styles (BOX, FRAME)
class CoordinateSystem : public Drawable
{

public:
	
	explicit CoordinateSystem(Qwt3D::Triple blb = Qwt3D::Triple(0,0,0), Qwt3D::Triple ftr = Qwt3D::Triple(0,0,0), Qwt3D::COORDSTYLE = Qwt3D::BOX);
  ~CoordinateSystem();	
	
	void init(Qwt3D::Triple beg = Qwt3D::Triple(0,0,0), Qwt3D::Triple end = Qwt3D::Triple(0,0,0));
	void setStyle(Qwt3D::COORDSTYLE s,	Qwt3D::AXIS frame_1 = Qwt3D::X1, 
																			Qwt3D::AXIS frame_2 = Qwt3D::Y1, 
																			Qwt3D::AXIS frame_3 = Qwt3D::Z1);
	Qwt3D::COORDSTYLE style() const { return style_;}
	void setPosition(Qwt3D::Triple first, Qwt3D::Triple second); //!< first == front_left_bottom, second == back_right_top
	
	void setAxesColor(Qwt3D::RGBA val);
	void setNumberFont(QString const& family, int pointSize, int weight = QFont::Normal, bool italic = false);
	void setNumberFont(QFont const& font);
	void setNumberColor(Qwt3D::RGBA val);
	
	void setLabelFont(QString const& family, int pointSize, int weight = QFont::Normal, bool italic = false);
	void setLabelFont(QFont const& font);
	void setLabelColor(Qwt3D::RGBA val);

	void setLineWidth(double val, double majfac = 1, double minfac = 0.6);
	void setTicLength(double major, double minor);
	void setAutoScale(bool val = true);

	Qwt3D::Triple first() const { return first_;}
	Qwt3D::Triple second() const { return second_;}

	void setAutoDecoration(bool val = true) {autodecoration_ = val;}
	bool autoDecoration() const { return autodecoration_;}

	void setLineSmooth(bool val = true) {smooth_ = val;} //!< draw smooth axes
	bool lineSmooth() const {return smooth_;}            //!< smooth axes ? 

	void draw();

	std::vector<Axis> axes;

private:

	void destroy();
	
	Qwt3D::Triple first_, second_;
	Qwt3D::COORDSTYLE style_;
	
	double ared_, agreen_, ablue_; 
	double lineWidth_, majLineWidth_, minLineWidth_;

	bool smooth_;

	void positionateLabel(Axis& ax, LabelPixmap::ANCHOR an);
	
	void chooseAxes();
	void autoDecorateExposedAxis(Axis& ax, bool left);

	bool autodecoration_;
};

} // ns

#endif