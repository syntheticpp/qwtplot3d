#include "qbitmap.h"
#include "labelpixmap.h"

LabelPixmap::LabelPixmap()
{
	init();
}

LabelPixmap::LabelPixmap(const QString & family, int pointSize, int weight, bool italic)
{
	init(family, pointSize, weight, italic);
}


void 
LabelPixmap::init(const QString & family, int pointSize, int weight, bool italic)
{
	init();
	font_ = QFont(family, pointSize, weight, italic );
}

void 
LabelPixmap::init()
{
	beg_ = Triple(0.0, 0.0, 0.0);
	end_ = beg_;
	setColor(0,0,0);
	pm_ = QPixmap(0, 0, -1);
	font_ = QFont();
	anchor_ = BottomLeft;
}

void 
LabelPixmap::setFont(const QString & family, int pointSize, int weight, bool italic)
{
	font_ = QFont(family, pointSize, weight, italic );
	update();	
}

void 
LabelPixmap::setPosition(Triple pos, ANCHOR a)
{
	anchor_ = a;
	Triple start = World2ViewPort(pos);

	switch (anchor_)
	{
		case BottomLeft :
			beg_ = pos;
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
LabelPixmap::update()
{
	QPainter p;
	QFontMetrics fm(font_);

  QFontInfo info(font_);

  QRect r = 	QRect(QPoint(0,0),fm.size(Qt::SingleLine, text_));//fm.boundingRect(text_)  misbehaviour under linux;
  
  r.moveBy(0, -r.top());
	
	pm_ = QPixmap(r.width(), r.bottom(), -1);

//	if (pm_.isNull())
//		return;

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
LabelPixmap::draw()
{
//	if (buf_.isNull())
//		return;


	GLboolean b;
	GLint func;
	GLdouble v;
	glGetBooleanv(GL_ALPHA_TEST, &b);
	glGetIntegerv(GL_ALPHA_TEST_FUNC, &func);
	glGetDoublev(GL_ALPHA_TEST_REF, &v);
	
	glEnable (GL_ALPHA_TEST);
  glAlphaFunc (GL_NOTEQUAL, 0.0);

	glRasterPos3d(beg_.x, beg_.y, beg_.z);

	int w = tex_.width();
	int h = tex_.height();

  glDrawPixels(w, h, GL_RGBA, GL_UNSIGNED_BYTE, tex_.bits() );
	
	glAlphaFunc(func,v);
	Enable(GL_ALPHA_TEST, b);
}


double 
LabelPixmap::width() const 
{ 
	return pm_.width(); 
}

double
LabelPixmap::height() const 
{ 
	return pm_.height(); 
}

void
LabelPixmap::setColor(double r, double g, double b, double a)
{
	Drawable::setColor(r,g,b,a);
	update();
}	

void
LabelPixmap::setColor(RGBA rgba)
{
	Drawable::setColor(rgba);
	update();
}	

void 
LabelPixmap::setString(QString const& s)
{
  text_ = s;
	update();
}
