#ifndef __DRAWABLE_H__
#define __DRAWABLE_H__


#include <qgl.h>
#include <list>
#include "types.h"

//! ABC for Drawables
class Drawable 
{

public:

	virtual ~Drawable() = 0;
	
	virtual void draw();
	virtual void postDraw();

	virtual void saveGLState();
	virtual void restoreGLState();

	void attach(Drawable*);
	void detach(Drawable*);
	void detachAll();
	
	virtual void setColor(double r, double g, double b, double a = 1);	
	virtual void setColor(RGBA rgba);	

protected:
	
	RGBA color;
	void Enable(GLenum what, GLboolean val);
	

private:

	GLboolean ls;
	GLboolean pols;
	GLint polmode[2];
	GLfloat lw;
	GLint blsrc, bldst;
	GLdouble col[4];
	GLint pattern, factor;
	GLboolean sallowed;
	GLboolean tex2d;
	GLint matrixmode;

	std::list<Drawable*> dlist;


};

#endif // __DRAWABLE_H__
