#include <qbitmap.h>
#include "qwt3d_gl2ps.h"
#include "qwt3d_label.h"

using namespace Qwt3D;

bool Label::devicefonts_ = false;

Label::Label()
{
	init();
}

Label::Label(const QString & family, int pointSize, int weight, bool italic)
{
	init(family, pointSize, weight, italic);
}


void Label::init(const QString & family, int pointSize, int weight, bool italic)
{
	init();
	font_ = QFont(family, pointSize, weight, italic );
}

void Label::init()
{
	beg_ = Triple(0.0, 0.0, 0.0);
	end_ = beg_;
	pos_ = beg_;
	setColor(0,0,0);
	pm_ = QPixmap(0, 0, -1);
	font_ = QFont();
	anchor_ = BottomLeft;
	gap_ = 0;
	flagforupdate_ = true;
}

void Label::useDeviceFonts(bool val)
{
	devicefonts_ = val;
}

void Label::setFont(const QString & family, int pointSize, int weight, bool italic)
{
	font_ = QFont(family, pointSize, weight, italic );
	flagforupdate_ = true;
}

void Label::setString(QString const& s)
{
  text_ = s;
	flagforupdate_ = true;
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
void Label::setPosition(Triple pos, ANCHOR a)
{
	anchor_ = a;
	pos_ = pos;
}

void Label::setRelPosition(Tuple rpos, ANCHOR a)
{
	double ot = 0.99;

	getMatrices(modelMatrix, projMatrix, viewport);
	beg_ = relativePosition(Triple(rpos.x, rpos.y, ot));
	setPosition(beg_, a);	
}

void Label::update()
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

/**
Adds an additional shift to the anchor point. This happens in a more or less intelligent manner
depending on the nature of the anchor:
\verbatim
anchor type         shift

left aligned         -->
right aligned        <--
top aligned          top-down            
bottom aligned       bottom-up
\endverbatim
The unit is user space dependend (one pixel on screen - play around to get satisfying results)
*/
void Label::adjust(int gap)
{
	gap_ = gap;
}

void Label::convert2screen()
{
	Triple start = World2ViewPort(pos_);
	
	switch (anchor_)
	{
		case BottomLeft :
			beg_ = pos_;
			break;
		case BottomRight:
			beg_ = ViewPort2World(start - Triple(width() + gap_, 0, 0));
			break;
		case BottomCenter:
			beg_ = ViewPort2World(start - Triple(width() / 2, -gap_, 0));
			break;
		case TopRight:
			beg_ = ViewPort2World(start - Triple(width() + gap_, height(), 0));
			break;
		case TopLeft:
			beg_ = ViewPort2World(start - Triple(-gap_, height(), 0));
			break;
		case TopCenter:
			beg_ = ViewPort2World(start - Triple(width() / 2, height() + gap_, 0));
			break;
		case CenterLeft:
			beg_ = ViewPort2World(start - Triple(-gap_, height() / 2, 0));
			break;
		case CenterRight:
			beg_ = ViewPort2World(start - Triple(width() + gap_, height() / 2, 0));
			break;
		case Center:
			beg_ = ViewPort2World(start - Triple(width() / 2, height() / 2, 0));
			break;
		default:
			break;
	}
	start = World2ViewPort(beg_);
	end_ = ViewPort2World(start + Triple(width(), height(), 0));	
}

void Label::draw()
{
	if (flagforupdate_)
	{
		update();
		flagforupdate_ = false;
	}

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
 
	if (devicefonts_)
	{
//		drawDevicePixels(w, h, GL_RGBA, GL_UNSIGNED_BYTE, tex_.bits());
		drawDeviceText(text_.latin1(), "Courier", font_.pointSize(), pos_, color, anchor_, gap_);
	}
	else
	{
		glDrawPixels(w, h, GL_RGBA, GL_UNSIGNED_BYTE, tex_.bits());	
	}


	glAlphaFunc(func,v);
	Enable(GL_ALPHA_TEST, b);
}


double Label::width() const 
{ 
	return pm_.width(); 
}

double Label::height() const 
{ 
	return pm_.height(); 
}	