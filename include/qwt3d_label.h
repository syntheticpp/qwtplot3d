#ifndef __LABELPIXMAP_H__
#define __LABELPIXMAP_H__

#include <qpixmap.h>
#include <qimage.h>
#include <qfont.h>
#include <qpainter.h>
#include <qfontmetrics.h>

#include "qwt3d_global.h"
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
	
	//! Sets the labels font
	void setFont(QString const& family, int pointSize, int weight = QFont::Normal, bool italic = false);

	void setPosition(Qwt3D::Triple pos, ANCHOR a = BottomLeft); //!< Sets the labels position
	void setRelPosition(Tuple rpos, ANCHOR a); //!< Sets the labels position relative to screen
	Qwt3D::Triple first() const { return beg_;} //!< Receives bottom left label position
	Qwt3D::Triple second() const { return end_;} //!< Receives top right label position
	ANCHOR anchor() const { return anchor_; } //!< Defines an anchor point for the labels surrounding rectangle

	/*!
	For unicode labeling (<tt> QChar(0x3c0) </tt> etc.) please look at <a href="http://www.unicode.org/charts/">www.unicode.org</a>.
	*/
	void setString(QString const& s);	//!< Sets the labels string


	void update(); //!< Enforces an update of the internal pixmap
	void draw(); //!< Actual drawing

	/**
		\brief Decides about use of PDF standard fonts for PDF output 
		If true, Label can use one of the PDF standard fonts (unprecise positioning for now), 
		otherwise it dumps  pixmaps in the PDF stream (poor quality) 
	*/
	static void usePrinterFonts(bool val); 
	

private:

	Qwt3D::Triple beg_, end_, pos_;
	QPixmap pm_;
	QImage  buf_, tex_;
	QFont font_;
	QString text_;

	ANCHOR anchor_;
	
	void init();
  void init(const QString & family, int pointSize, int weight = QFont::Normal, bool italic = false);
	void convert2screen();

	double width() const;
	double height() const;

	static bool printerFonts_;

};

} // ns

#endif