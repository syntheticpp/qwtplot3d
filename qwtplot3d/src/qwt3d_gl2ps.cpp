#include "qwt3d_gl2ps.h"

using namespace Qwt3D;

GLint 
Qwt3D::setDeviceLineWidth(GLfloat val)
{
	glLineWidth(val);
	return gl2psLineWidth(val);
}

GLint 
Qwt3D::drawDevicePixels(GLsizei width, GLsizei height,
                       GLenum format, GLenum type,
                       const void *pixels, bool printerfonts)
{
  glDrawPixels(width, height, format, type, pixels);

	if (printerfonts)
		return GL2PS_SUCCESS;

  if(format != GL_RGBA || type != GL_UNSIGNED_BYTE)
		return GL2PS_ERROR;
	
	GLfloat* convertedpixel = (GLfloat*)malloc(3 * width * height * sizeof(GLfloat));
	if (!convertedpixel)
		return GL2PS_ERROR;
	
	GLubyte* px = (GLubyte*)pixels; 
	for (int i=0; i!=3*width*height; i+=3)
	{
		int pxi = (4*i)/3;
		convertedpixel[i] = px[pxi] / float(255);
		convertedpixel[i+1] = px[pxi+1] / float(255);
		convertedpixel[i+2] = px[pxi+2] / float(255);
	}
	GLint ret = gl2psDrawPixels(width, height, 0, 0, GL_RGB, GL_FLOAT, convertedpixel);
	free(convertedpixel);
	return ret;
}

GLint 
Qwt3D::drawDeviceText(const char* str, const char* fontname, int fontsize, Triple pos, ANCHOR align, Triple gap)
{
	Triple adjpos = pos;

	GLdouble fcol[4];
	glGetDoublev(GL_CURRENT_COLOR, fcol);
	GLdouble bcol[4];
	glGetDoublev(GL_COLOR_CLEAR_VALUE, bcol);
	
//	glColor4d(color.r, color.g, color.b, color.a);
//		glClearColor(color.r, color.g, color.b, color.a);

	GLint ret = GL2PS_SUCCESS;

	GLint a = GL2PS_TEXT_BL;
	switch(align)
	{
		case Center:
			a = GL2PS_TEXT_C;
			break;
		case CenterLeft:
			a = GL2PS_TEXT_CL;
			adjpos.x += gap.x;
			break;
		case CenterRight:
			a = GL2PS_TEXT_CR;
			adjpos.x -= gap.x;
			break;
		case BottomCenter:
			a = GL2PS_TEXT_B;
			adjpos.y += gap.y;
			break;
		case BottomLeft:
			a = GL2PS_TEXT_BL;
			adjpos.x += gap.x;
			adjpos.y += gap.y;
			break;
		case BottomRight:
			a = GL2PS_TEXT_BR;
			adjpos.x -= gap.x;
			adjpos.y += gap.y;
			break;
		case TopCenter:
			a = GL2PS_TEXT_T;
			adjpos.y -= gap.y;
			break;
		case TopLeft:
			a = GL2PS_TEXT_TL;
			adjpos.x -= gap.x;
			adjpos.y -= gap.y;
			break;
		case TopRight:
			a = GL2PS_TEXT_TR;
			adjpos.x += gap.x;
			adjpos.y -= gap.y;
			break;
		default:
			break;
	}
	glRasterPos3d(adjpos.x, adjpos.y, adjpos.z);
	ret = gl2psText2(str, fontname, (int)fontsize, a);
	glColor4dv(fcol);
	glClearColor(bcol[0], bcol[1], bcol[2], bcol[3]);
  return ret;
}

void 
Qwt3D::setDevicePolygonOffset(GLfloat factor, GLfloat units)
{
	glPolygonOffset(factor, units);
	gl2psEnable(GL2PS_POLYGON_OFFSET_FILL);
}
