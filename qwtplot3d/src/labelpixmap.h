#ifndef __LABELPIXMAP_H__
#define __LABELPIXMAP_H__

#include <qpixmap.h>
#include <qimage.h>
#include <qfont.h>
#include <qpainter.h>
#include <qfontmetrics.h>

#include "drawable.h"

namespace Qwt3D
{

class LabelPixmap : public Drawable 
{
		
public:

	//! possible anchor points
	enum ANCHOR
	{
		BottomLeft,
		BottomRight,
		BottomCenter,
		TopLeft,
		TopRight,
		TopCenter,
		CenterLeft,
		CenterRight,
		Center
	};

  LabelPixmap();
 	//! construct pixmap and initialize with font 
	LabelPixmap(const QString & family, int pointSize, int weight = QFont::Normal, bool italic = false);
	
	//! set label font
	void setFont(const QString& family, int pointSize, int weight = QFont::Normal, bool italic = false);

	void setPosition(Qwt3D::Triple pos, ANCHOR a = BottomLeft); //!< set label position
	Qwt3D::Triple first() const { return beg_;}
	Qwt3D::Triple second() const { return end_;}
	ANCHOR anchor() const { return anchor_; } //!< define anchor point for position

	/*!
	For unicode labeling (<tt> QChar(0x3c0) </tt> etc.) please look at <a href="http://www.unicode.org/charts/">www.unicode.org</a>.
	*/
	void setString(QString const& s);	//!< set label string

	virtual void setColor(double r, double g, double b, double a = 1); //!< set label color	
	virtual void setColor(Qwt3D::RGBA rgba);	//!< overloaded member

	void draw();

	
private:

	Qwt3D::Triple beg_, end_;
	QPixmap pm_;
	QImage  buf_, tex_;
	QFont font_;
	QString text_;

	ANCHOR anchor_;
	
	void init();
  void init(const QString & family, int pointSize, int weight = QFont::Normal, bool italic = false);
	void update();

	double width() const;
	double height() const;

};

} // ns

#endif