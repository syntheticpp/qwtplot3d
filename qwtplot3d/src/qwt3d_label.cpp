#include <qbitmap.h>
#include "qwt3d_gl2ps.h"
#include "qwt3d_label.h"

using namespace Qwt3D;

bool Label::printerFonts_ = false;

Label::Label()
{
	init();
}

Label::Label(const QString & family, int pointSize, int weight, bool italic)
{
	init(family, pointSize, weight, italic);
}


void 
Label::init(const QString & family, int pointSize, int weight, bool italic)
{
	init();
	font_ = QFont(family, pointSize, weight, italic );
}

void 
Label::init()
{
	beg_ = Triple(0.0, 0.0, 0.0);
	end_ = beg_;
	pos_ = beg_;
	setColor(0,0,0,1);
	pm_ = QPixmap(0, 0, -1);
	font_ = QFont();
	anchor_ = BottomLeft;
}

void 
Label::usePrinterFonts(bool val)
{
	printerFonts_ = val;
}

void 
Label::setFont(const QString & family, int pointSize, int weight, bool italic)
{
	font_ = QFont(family, pointSize, weight, italic );
	update();	
}

/**
 
example:

\verbatim

   Anchor TopCenter (*)  resp. BottomRight(X) 

   +----*----+
   |  Pixmap |
   +---------X

\endverbatim
*/
void 
Label::setPosition(Triple pos, ANCHOR a)
{
	anchor_ = a;
	pos_ = pos;
}

void
Label::setRelPosition(Tuple rpos, ANCHOR a)
{
	double ot = 0.99;

	getMatrices(modelMatrix, projMatrix, viewport);
	beg_ = relativePosition(Triple(rpos.x, rpos.y, ot));
	setPosition(beg_, a);	
	update();
}

void 
Label::update()
{
	QPainter p;
	QFontMetrics fm(font_);

  QFontInfo info(font_);

  QRect r = 	QRect(QPoint(0,0),fm.size(Qt::SingleLine, text_));//fm.boundingRect(text_)  misbehaviour under linux;
  
  r.moveBy(0, -r.top());
	
	pm_ = QPixmap(r.width(), r.bottom(), -1);

	if (pm_.isNull()) // else crash under linux
	{
		r = 	QRect(QPoint(0,0),fm.size(Qt::SingleLine, QString(" "))); // draw empty space else //todo
 		r.moveBy(0, -r.top());
		pm_ = QPixmap(r.width(), r.bottom(), -1);		
	}
	
	QBitmap bm(pm_.width(),pm_.height(),true);
	p.begin( &bm );
		p.setPen(Qt::color1);
		p.setFont(font_);
		p.drawText(0,r.height() - fm.descent() -1 , text_);
	p.end();

	pm_.setMask(bm);
	pm_.fill();
	p.begin( &pm_ );
		p.setFont( font_ );
		p.setPen( Qt::SolidLine );
		p.setPen( GL2Qt(color.r, color.g, color.b) );

		p.drawText(0,r.height() - fm.descent() -1 , text_);
	p.end();
	buf_ = pm_.convertToImage();
	tex_ = QGLWidget::convertToGLFormat( buf_ );	  // flipped 32bit RGBA ?		
}

void
Label::convert2screen()
{
	Triple start = World2ViewPort(pos_);

	switch (anchor_)
	{
		case BottomLeft :
			beg_ = pos_;
			break;
		case BottomRight:
			beg_ = ViewPort2World(start - Triple(width(), 0, 0));
			break;
		case BottomCenter:
			beg_ = ViewPort2World(start - Triple(width() / 2, 0, 0));
			break;
		case TopRight:
			beg_ = ViewPort2World(start - Triple(width(), height(), 0));
			break;
		case TopLeft:
			beg_ = ViewPort2World(start - Triple(0, height(), 0));
			break;
		case TopCenter:
			beg_ = ViewPort2World(start - Triple(width() / 2, height(), 0));
			break;
		case CenterLeft:
			beg_ = ViewPort2World(start - Triple(0, height() / 2, 0));
			break;
		case CenterRight:
			beg_ = ViewPort2World(start - Triple(width(), height() / 2, 0));
			break;
		case Center:
			beg_ = ViewPort2World(start - Triple(width() / 2, height() / 2, 0));
			break;
		default:
			break;
	}
	end_ = ViewPort2World(start + Triple(width(), height(), 0));	
}

void 
Label::draw()
{
	if (buf_.isNull())
		return;
	
	GLboolean b;
	GLint func;
	GLdouble v;
	glGetBooleanv(GL_ALPHA_TEST, &b);
	glGetIntegerv(GL_ALPHA_TEST_FUNC, &func);
	glGetDoublev(GL_ALPHA_TEST_REF, &v);
	
	glEnable (GL_ALPHA_TEST);
  glAlphaFunc (GL_NOTEQUAL, 0.0);
	
	convert2screen();
	glRasterPos3d(beg_.x, beg_.y, beg_.z);
 
	
	int w = tex_.width();
	int h = tex_.height();
 
	drawDevicePixels(w, h, GL_RGBA, GL_UNSIGNED_BYTE, tex_.bits(), printerFonts_ );

	if (printerFonts_)
	{
		Triple start = World2ViewPort(pos_);
		double gapx	 = ViewPort2World(start + Triple(4, 0, 0)).x - pos_.x;
		double gapy	 = ViewPort2World(start + Triple(0, 7, 0)).y - pos_.y;
		double gapz  = 0;
		
		Triple gap(gapx,gapy,gapz);

		drawDeviceText(text_.latin1(), "Courier", font_.pointSize(), pos_, color, anchor_, gap);
	}

	glAlphaFunc(func,v);
	Enable(GL_ALPHA_TEST, b);
}


double 
Label::width() const 
{ 
	return pm_.width(); 
}

double
Label::height() const 
{ 
	return pm_.height(); 
}	

void 
Label::setString(QString const& s)
{
  text_ = s;
}
