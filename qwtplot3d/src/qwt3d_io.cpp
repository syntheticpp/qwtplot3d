#include <time.h>

#include "qwt3d_plot.h"
#include "qwt3d_gl2ps.h"
#include "qwt3d_io.h"

using namespace Qwt3D;


/*!
	Writes vector data supported by gl2ps. The corresponding format types are "EPS","PS","PDF" or "TEX" .
	The last parameter is one of gl2ps' sorting types: GL2PS_NO_SORT, GL2PS_SIMPLE_SORT or GL2PS_BSP_SORT.
  Default is GL2PS_SIMPLE_SORT.\n 
	\b Beware: GL2PS_BSP_SORT turns out to behave very slowly and memory consuming, especially in cases where
	many polygons appear. It is still more exact than GL2PS_SIMPLE_SORT.\n
  Another word of caution: If you use compressed output, the created postscript file is actually a gzipped 
  EPS resp. PS file ("xyz.eps.gz, yyz.ps.gz"). PDF on the other hand defines compression in his specification 
  and therefore the option produces valid PDF.  
*/
bool Plot3D::saveVector(QString fileName, QString format, bool notext, int sorttype)
{
	makeCurrent();
	Label::useDeviceFonts(true);
	
	GLint gl2ps_format;
	if (format == QString("EPS"))
	{
		gl2ps_format = GL2PS_EPS;
	}
	else if (format == QString("PS"))
	{
		gl2ps_format = GL2PS_PS;
	}
	else if (format == QString("TEX"))
	{
		gl2ps_format = GL2PS_TEX;
	}
	else if (format == QString("PDF"))
	{
		gl2ps_format = GL2PS_PDF;
	}
	else
	{
		Label::useDeviceFonts(false);
		return false;
	}

	FILE *fp = fopen(fileName.latin1(), "wb");	
	if (!fp)
		return false;

	GLint bufsize = 0, state = GL2PS_OVERFLOW;
	GLint viewport[4];

	glGetIntegerv(GL_VIEWPORT, viewport);

	GLint options = GL2PS_SIMPLE_LINE_OFFSET | GL2PS_SILENT | GL2PS_DRAW_BACKGROUND |
										 GL2PS_OCCLUSION_CULL | GL2PS_BEST_ROOT | GL2PS_COMPRESS;

	if (viewport[2] - viewport[0] > viewport[3] - viewport[0])
		options |= GL2PS_LANDSCAPE;

	if (notext)
		options |= GL2PS_NO_PIXMAP | GL2PS_NO_TEXT;

	if (sorttype < 0)
		sorttype = GL2PS_SIMPLE_SORT;


	QString version = QString::number(QWT3D_MAJOR_VERSION) + "."
		+ QString::number(QWT3D_MINOR_VERSION) + "."
		+ QString::number(QWT3D_PATCH_VERSION); 
	    
	QString producer = QString("QwtPlot3D ") + version + 
		" (beta) , (C) 2002";
  
  // calculate actual year
  time_t now;
  struct tm *newtime;
  time(&now);
  newtime = gmtime(&now);
	if (newtime && newtime->tm_year + 1900 > 2002)
	  producer += "-" + QString::number(newtime->tm_year+1900); 
  
  producer += " Micha Bieber <krischnamurti@users.sourceforge.net>";

	while( state == GL2PS_OVERFLOW )
	{ 
		bufsize += 2*1024*1024;
		gl2psBeginPage ( "---", producer, viewport,
										 gl2ps_format, sorttype,
										 options, GL_RGBA, 0, NULL, 0, 0, 0, bufsize,
										 fp, fileName.latin1() );
		
		updateData();
		updateGL(); 
		state = gl2psEndPage();
	}
	fclose(fp);

	Label::useDeviceFonts(false);
	return true;
}	
/*!
	Saves the framebuffer to the file fileName using one of the image file formats supported by Qt
*/
bool Plot3D::savePixmap(QString fileName, QString format)
{
	QImage im = grabFrameBuffer(true);
	return im.save(fileName,format);
}
