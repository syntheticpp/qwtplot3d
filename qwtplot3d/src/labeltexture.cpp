#include <qbitmap.h>
#include "labeltexture.h"

LabelTexture::LabelTexture()
{
	init();
}

LabelTexture::LabelTexture(const QString & family, int pointSize, int weight, bool italic)
{
	init(family, pointSize, weight, italic);
}


void 
LabelTexture::init(const QString & family, int pointSize, int weight, bool italic)
{
	init();
	font_ = QFont(family, pointSize, weight, italic );
}

void 
LabelTexture::init()
{
  origw_ = origh_ = 0;
	texobj_ = 0;
	beg_ = Triple(0.0, 0.0, 0.0);
  end_ = Triple(0.0, 0.0, 0.0);
	setColor(0,0,0);
	pm_ = QPixmap(0, 0, -1);
	font_ = QFont();
}

void 
LabelTexture::setPosition(Triple beg, Triple end)
{
	beg_ = beg;
	end_ = end;
}

void 
LabelTexture::setString(QString const& s)
{
	QPainter p;
	QFontMetrics fm(font_);

  QRect r = 	QRect(QPoint(0,0),fm.size(Qt::SingleLine, s));//fm.boundingRect(text_)  misbehaviour under linux;
	r.moveBy(0, -r.top()); // if italic
	
	getEnclosingPixmap(r.width(), r.bottom());
		
	QBitmap bm(pm_.width(),pm_.height(),true);
	p.begin( &bm );
		p.setPen(Qt::color1);
		p.setFont(font_);
		p.drawText(0,r.height() - fm.descent() -1 , s);
	p.end();

	pm_.setMask(bm);
	pm_.fill();
	p.begin( &pm_ );
		p.setFont( font_ );
		p.setPen( Qt::SolidLine );
		p.setPen( GL2Qt(color.r,color.g,color.b) );
		
		p.drawText(0,r.height() - fm.descent() -1 , s);
	p.end();			

	buf_ = pm_.convertToImage();
	buildTextureObject();
}


void 
LabelTexture::draw()
{
	if (buf_.isNull())
		return;

	saveGLState();

	glEnable( GL_TEXTURE_2D );
  				     
	glColor4d(color.r,color.g,color.b,color.a);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;
		

	double x = double(origw_) / tex_.width();
	double y = double(origh_) / tex_.height();

	glBegin( GL_QUADS);
		glTexCoord2d( 0, 1-y); glVertex3f( end_.x, end_.y, beg_.z);
		glTexCoord2d( x, 1-y); glVertex3f( end_.x, end_.y, end_.z);
		glTexCoord2d( x, 1); glVertex3f( beg_.x, beg_.y, end_.z);
		glTexCoord2d( 0, 1); glVertex3f( beg_.x, beg_.y, beg_.z);
	glEnd();
	
	drawPic();

	restoreGLState();
}

bool
LabelTexture::load(QString const& fname)
{
	bool ret = pm_.load(fname);
	getEnclosingPixmap(pm_.width(), pm_.height());
	buf_ = pm_.convertToImage();
	buildTextureObject();
	return ret;
}

void
LabelTexture::buildTextureObject()
{
 	saveGLState();

	static bool first = true;

  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	tex_ = QGLWidget::convertToGLFormat( buf_ );	  // flipped 32bit RGBA	

	if (first)
	{
		glGenTextures(1, &texobj_);
		glBindTexture(GL_TEXTURE_2D, texobj_);
	}
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, tex_.width(), tex_.height(), 0,
		GL_RGBA, GL_UNSIGNED_BYTE, tex_.bits() );

	first = false;
	
	restoreGLState();
}

void
LabelTexture::drawPic()
{
//	tex_ = QGLWidget::convertToGLFormat( buf_ );	  // flipped 32bit RGBA	
	
	bool err;
	Triple win = World2ViewPort(end_, &err);

	glRasterPos3d(beg_.x, beg_.y-2.6, 0.0);

	int w = tex_.width();
	int h = tex_.height();
	glDrawPixels(w, h, GL_RGBA, GL_UNSIGNED_BYTE, tex_.bits() );
}


void 
LabelTexture::getEnclosingPixmap(int w, int h)
{
	origw_ = w;
	origh_ = h;
	
	int s = (w < h) ? h : w;
	
	int power = 0;
	int ns = pow(2,power);
	while ( ns < s )
		ns = pow(2,++power);
	
	QPixmap ret = QPixmap(ns, ns, -1);
	
	GLdouble v[4];
	glGetDoublev(GL_COLOR_CLEAR_VALUE, v);
		QColor col = GL2Qt(v[0],v[1],v[2]);
	ret.fill(col);
	
	bitBlt(&ret, 0, 0, &pm_, 0, 0, pm_.width(), pm_.height(), Qt::CopyROP);

	pm_ = ret;
}

double 
LabelTexture::width() const   
{ 
	if (pm_.width()) 
		return double(origw_) / pm_.width(); 
	return 0;
}

double
LabelTexture::height() const 
{ 
	if (pm_.height()) 
		return double(origh_) / pm_.height(); 
	return 0;
}
