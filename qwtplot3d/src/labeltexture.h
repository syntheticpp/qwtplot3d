#ifndef __LABELTEXTURE_H__
#define __LABELTEXTURE_H__

#include <qpixmap.h>
#include <qimage.h>
#include <qfont.h>
#include <qpainter.h>
#include <qgl.h>
#include <qfontmetrics.h>

#include "types.h"
#include "drawable.h"

class LabelTexture : public Drawable 
{

public:

  LabelTexture();
  LabelTexture(const QString & family, int pointSize, int weight = QFont::Normal, bool italic = false);

	void init();
  void init(const QString & family, int pointSize, int weight = QFont::Normal, bool italic = false);
	void draw();
	bool load(QString const& fname);
	void setString(QString const& s);	
	void setPosition(Triple beg, Triple end);
	double width() const;
	double height() const;

	void drawPic();

private:

	void getEnclosingPixmap(int w, int h);
	void buildTextureObject();
	Triple beg_, end_;
	QPixmap pm_;
	QImage  buf_, tex_;
	QFont font_;
	GLuint texobj_;
	int origw_, origh_; //!< size before 2^n adapting - ROI of the texture
};


#endif