#ifndef __AXIS_H__
#define __AXIS_H__

#include "qwt3d_global.h"
#include "qwt3d_labelpixmap.h"
#include "qwt3d_autoscaler.h"

namespace Qwt3D
{

class LabelTexture;

//! Autoscalable axis with caption
class QWT3D_EXPORT Axis : public Drawable
{

public:

	explicit Axis();
	Axis(Qwt3D::Triple beg, Qwt3D::Triple end);
	~Axis();
	
	virtual void draw();

	void setPosition(const Qwt3D::Triple& beg, const Qwt3D::Triple& end);
	void position(Qwt3D::Triple& beg, Qwt3D::Triple& end) const {beg = beg_; end = end_;}
	Qwt3D::Triple begin() const { return beg_; }
	Qwt3D::Triple end() const { return end_; }

	void setTicLength(double majorl, double minorl);
	void ticLength(double& majorl, double& minorl) const {majorl = lmaj_; minorl = lmin_;}
	void setTicOrientation(double tx, double ty, double tz);
	void setTicOrientation(const Qwt3D::Triple& val);
	Qwt3D::Triple ticOrientation() const { return orientation_;}
	void setSymmetricTics( bool b) { symtics_ = b;}
	
	void setLabelFont(QString const& family, int pointSize, int weight = QFont::Normal, bool italic = false);
	void setLabelFont(QFont const& font);
  void setLabelString(QString const& name);
	void setLabelPosition(const Qwt3D::Triple& pos, Qwt3D::ANCHOR);
	void setLabelColor(Qwt3D::RGBA col);
	void setLabel(bool d) {drawLabel_ = d;}

	void setScale(bool d) {drawTics_ = d;}
	void setNumbers(bool d) {drawNumbers_ = d;}
	bool numbers() const {return drawNumbers_;}
	void setNumberColor(Qwt3D::RGBA col);
	Qwt3D::RGBA numberColor() const {return numbercolor_;}
	void setNumberFont(QString const& family, int pointSize, int weight = QFont::Normal, bool italic = false);
	void setNumberFont(QFont const&);
	void setNumberAnchor(Qwt3D::ANCHOR a) { scaleNumberAnchor_ = a;}

	void setAutoScale(bool val = true) {autoscale_ = val;};
	bool autoScale() const { return autoscale_;}

	void setMajors(int val);
	void setMinors(int val);
	int majors() const { return majorintervals_; }
	int minors() const { return minorintervals_; }

	void setLineWidth(double val, double majfac = 1, double minfac = 0.5);

	void setLimits(double start, double stop) {start_=start; stop_=stop;}
	void limits(double& start, double& stop) const {start = start_; stop = stop_;}

	
private:

	void init();
	void drawBase();
	void drawTics();
	void drawNumber(Qwt3D::Triple Pos, int mtic);
	void drawTic(Qwt3D::Triple nadir, double length);
	
	
	Qwt3D::ANCHOR scaleNumberAnchor_;
	LabelPixmap label_;
	std::vector<LabelPixmap> markerLabel_;

	Qwt3D::Triple beg_, end_;

	Qwt3D::Triple ncube_beg_, ncube_end_; //!< enclosing parallelepiped for axis numbering

	double start_, stop_, autostart_, autostop_;
	double lmaj_, lmin_;
	Qwt3D::Triple orientation_;

	int majorintervals_, minorintervals_;

	double lineWidth_, majLineWidth_, minLineWidth_;
	bool symtics_;
	bool drawNumbers_, drawTics_, drawLabel_;
	bool autoscale_;
	QFont numberfont_;
	Qwt3D::RGBA  numbercolor_;
	bool simpleLabels_;

	void makeSimpleLabels(bool val) { simpleLabels_ = val;}
	bool simpleLables() const { return simpleLabels_;}

	Qwt3D::AutoScaler as_;
};

} // ns 

#endif