/********************************************************************
    created:   2003/09/04
    filename:  qwt3d_gl2ps.h
	
    author:    Micha Bieber	
*********************************************************************/

//! \file Device independent functions to support gl2ps + gl simultaneously

#ifndef __qwt3d_gl2ps_2003_09_04_23_54__
#define __qwt3d_gl2ps_2003_09_04_23_54__

#include <qgl.h>
#include "qwt3d_types.h"
#include <gl2ps.h>

namespace Qwt3D
{

GLint setDeviceLineWidth(GLfloat val);
GLint drawDevicePixels(GLsizei width, GLsizei height,
                       GLenum format, GLenum type,
                       const void *pixels, bool printerfonts);
GLint drawDeviceText(const char* str, const char* fontname, int fontsize, Qwt3D::Triple pos, Qwt3D::ANCHOR align, Qwt3D::Triple gap);

} // ns

#endif  // __qwt3d_gl2ps_2003_09_04_23_54__
