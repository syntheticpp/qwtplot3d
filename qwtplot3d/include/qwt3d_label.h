#ifndef __LABELPIXMAP_H__
#define __LABELPIXMAP_H__

#include <qpixmap.h>
#include <qimage.h>
#include <qfont.h>
#include <qpainter.h>
#include <qfontmetrics.h>

#include "qwt3d_drawable.h"

namespace Qwt3D
{

//! A Qt string or an output device dependent string
class QWT3D_EXPORT Label : public Drawable 
{
public:
	Label();
 	//! Construct label and initialize with font 
	Label(const QString & family, int pointSize, int weight = QFont::Normal, bool italic = false);
	
	QFont font(){return font_;}
	//! Sets the labels font
	void setFont(const QFont&);

	//! Sets the labels font
	void setFont(QString const& family, int pointSize, int weight = QFont::Normal, bool italic = false);

	void adjust(int gap); //!< Fine tunes label;
	double gap() const {return gap_;} //!< Returns the gap caused by adjust();
	void setPosition(Qwt3D::Triple pos, ANCHOR a = BottomLeft); //!< Sets the labels position
	void setRelPosition(Tuple rpos, ANCHOR a); //!< Sets the labels position relative to screen
	Qwt3D::Triple first() const { return beg_;} //!< Receives bottom left label position
	Qwt3D::Triple second() const { return end_;} //!< Receives top right label position
	ANCHOR anchor() const { return anchor_; } //!< Defines an anchor point for the labels surrounding rectangle
	virtual void setColor(double r, double g, double b, double a = 1);	
	virtual void setColor(Qwt3D::RGBA rgba);	

	/*!
	\brief Sets the label's string
	For unicode labeling (<tt> QChar(0x3c0) </tt> etc.) please look at <a href="http://www.unicode.org/charts/">www.unicode.org</a>.
	*/
	void setString(QString const& s);
	//! the label's string
	const QString& string() const;
	void draw(double angle = 0.0); //!< Actual drawing

	/**
		\brief Decides about use of PDF standard fonts for PDF output 
		If true, Label can use one of the PDF standard fonts (unprecise positioning for now), 
		otherwise it dumps  pixmaps in the PDF stream (poor quality) 
	*/
	static void useDeviceFonts(bool val);
	double width() const;
	double height() const;
	double textHeight() const;

private:

	bool use_relpos_;
	Qwt3D::Triple relpos_;
	Qwt3D::Triple beg_, end_, pos_;
	QFont font_;
	QString text_;

	ANCHOR anchor_;

	void init();
	void init(const QString & family, int pointSize, int weight = QFont::Normal, bool italic = false);
	QImage createImage(double angle);//!< Creates an internal bitmap used only for axis labels
	const char * fontname(); //!< Try to guess an appropriate font name from the 14 standard Type 1 fonts available
	void convert2screen();

	int gap_;

	bool flagforupdate_;

	static bool devicefonts_;
	double width_, height_;
};

} // ns

#endif
