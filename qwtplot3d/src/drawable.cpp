#include "drawable.h"

Drawable::~Drawable()
{
  detachAll();
}

void
Drawable::saveGLState()
{
	glGetBooleanv(GL_LINE_SMOOTH, &ls);
	glGetBooleanv(GL_POLYGON_SMOOTH, &pols);
	glGetFloatv(GL_LINE_WIDTH, &lw);
	glGetIntegerv(GL_BLEND_SRC, &blsrc);
	glGetIntegerv(GL_BLEND_DST, &bldst);
	glGetDoublev(GL_CURRENT_COLOR, col);
	glGetIntegerv(GL_LINE_STIPPLE_PATTERN, &pattern);
	glGetIntegerv(GL_LINE_STIPPLE_REPEAT, &factor);
	glGetBooleanv(GL_LINE_STIPPLE, &sallowed);
	glGetBooleanv(GL_TEXTURE_2D, &tex2d);
	glGetIntegerv(GL_POLYGON_MODE, polmode);
	glGetIntegerv(GL_MATRIX_MODE, &matrixmode);
}

void
Drawable::restoreGLState()
{
	Enable(GL_LINE_SMOOTH, ls);
	Enable(GL_POLYGON_SMOOTH, pols);
	
	glLineWidth(lw);
	glBlendFunc(blsrc, bldst);
	glColor4dv(col);

	glLineStipple(factor,pattern);
	Enable(GL_LINE_STIPPLE,sallowed);
	Enable(GL_TEXTURE_2D,tex2d);
	glPolygonMode(polmode[0], polmode[1]);
	glMatrixMode(matrixmode);
}

void 
Drawable::Enable(GLenum what, GLboolean val)
{
	if (val)
		glEnable(what);
  else
		glDisable(what);
}

void 
Drawable::attach(Drawable* dr)
{
	if ( dlist.end() == std::find( dlist.begin(), dlist.end(), dr ) )
		if (dr)
		{
			dlist.push_back(dr);
		}
}

void 
Drawable::detach(Drawable* dr)
{
	std::list<Drawable*>::iterator it = std::find(dlist.begin(), dlist.end(), dr);
	
	if ( it != dlist.end() )
	{
		dlist.erase(it);
	}
}
void 
Drawable::detachAll()
{
	dlist.clear();
}

void 
Drawable::draw()
{
	saveGLState();

	for (std::list<Drawable*>::iterator it = dlist.begin(); it!=dlist.end(); ++it)
	{
		(*it)->draw();
	}
	restoreGLState();
}

void 
Drawable::postDraw()
{
	saveGLState();

	for (std::list<Drawable*>::iterator it = dlist.begin(); it!=dlist.end(); ++it)
	{
		(*it)->postDraw();
	}
	
	restoreGLState();
}

void
Drawable::setColor(double r, double g, double b, double a)
{
	color = RGBA(r,g,b,a);
}	

void
Drawable::setColor(RGBA rgba)
{
	color = rgba;
}	
