#include "qwt3d_gl2ps.h"
#include "qwt3d_openglhelper.h"

using namespace Qwt3D;


GLint Qwt3D::setDeviceLineWidth(GLfloat val)
{
	if (val<0) 
		val=0;

	GLint ret = gl2psLineWidth(val);

	GLfloat lw[2];
	glGetFloatv(GL_LINE_WIDTH_RANGE, lw);
	
	if (val < lw[0])
		val = lw[0];
	else if (val > lw[1])
		val = lw[1];

	glLineWidth(val);
	return ret;
}

GLint Qwt3D::setDevicePointSize(GLfloat val)
{
	if (val<0) 
		val=0;

	GLint ret = gl2psPointSize(val);

	GLfloat lw[2];
	glGetFloatv(GL_POINT_SIZE_RANGE, lw);
	
	if (val < lw[0])
		val = lw[0];
	else if (val > lw[1])
		val = lw[1];

	glPointSize(val);
	return ret;
}

GLint Qwt3D::drawDevicePixels(GLsizei width, GLsizei height,
                       GLenum format, GLenum type,
                       const void *pixels)
{
  glDrawPixels(width, height, format, type, pixels);

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

GLint Qwt3D::drawDeviceText(const char* str, const char* fontname, int fontsize, Triple pos, RGBA rgba, ANCHOR align, double gap)
{
	double vp[3];

	World2ViewPort(vp[0], vp[1], vp[2], pos.x, pos.y, pos.z);
	Triple start(vp[0],vp[1],vp[2]);

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
			start += Triple(gap,0,0);
			break;
		case CenterRight:
			a = GL2PS_TEXT_CR;
			start += Triple(-gap,0,0);
			break;
		case BottomCenter:
			a = GL2PS_TEXT_B;
			start += Triple(0,gap,0);
			break;
		case BottomLeft:
			a = GL2PS_TEXT_BL;
			start += Triple(gap,gap,0);
			break;
		case BottomRight:
			a = GL2PS_TEXT_BR;
			start += Triple(-gap,gap,0);
			break;
		case TopCenter:
			a = GL2PS_TEXT_T;
			start += Triple(0,-gap,0);
			break;
		case TopLeft:
			a = GL2PS_TEXT_TL;
			start += Triple(gap,-gap,0);
			break;
		case TopRight:
			a = GL2PS_TEXT_TR;
			start += Triple(-gap,-gap,0);
			break;
		default:
			break;
	}
	
	ViewPort2World(vp[0], vp[1], vp[2], start.x, start.y, start.z);
	Triple adjpos(vp[0],vp[1],vp[2]);
	
	GL2PSrgba rgba2;
		
	rgba2[0] = rgba.r;
	rgba2[1] = rgba.g;
	rgba2[2] = rgba.b;
	rgba2[3] = rgba.a;

	glRasterPos3d(adjpos.x, adjpos.y, adjpos.z);
	ret = gl2psTextOpt(str, fontname, (int)fontsize, a, rgba2);
	glColor4dv(fcol);
	glClearColor(bcol[0], bcol[1], bcol[2], bcol[3]);
  return ret;
}

void Qwt3D::setDevicePolygonOffset(GLfloat factor, GLfloat units)
{
	glPolygonOffset(factor, units);
	gl2psEnable(GL2PS_POLYGON_OFFSET_FILL);
}
