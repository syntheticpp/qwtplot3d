/********************************************************************
    created:   2003/09/04
    filename:  qwt3d_gl2ps.h
	
    author:    Micha Bieber	
*********************************************************************/

//! \file Device independent functions to support gl2ps + gl simultaneously

#ifndef __qwt3d_gl2ps_2003_09_04_23_54__
#define __qwt3d_gl2ps_2003_09_04_23_54__

#include <qgl.h>

#ifdef QWT3D_GL2PS
	#include <gl2ps.h>
#endif

inline GLint setDeviceLineWidth(GLfloat val)
{
	glLineWidth(val);
#ifdef QWT3D_GL2PS
	return gl2psLineWidth(val);
#endif
  return 0;
}


#endif  // __qwt3d_gl2ps_2003_09_04_23_54__
