#include <time.h>

#include "qwt3d_plot.h"
#include "qwt3d_io_gl2ps.h"
#include "qwt3d_io_reader.h"
#if QT_VERSION < 0x040000
#else
  #include <QImageWriter>
#endif

using namespace Qwt3D;

static Qwt3D::IO qwt3diodummy; // Don't delete !


/*! 
  Registers a new IO::Function for data input.\n
  Every call overwrites a formerly registered handler for the same format string
  (case sensitive).
*/
bool IO::defineInputHandler(QString const& format, IO::Function func)
{
  return add_unique(rlist(), Entry(format, func));
}

/*! 
  Registers a new Functor for data input.\n
  Every call overwrites a formerly registered handler for the same format string
  (case sensitive).
*/
bool IO::defineInputHandler(QString const& format, IO::Functor const& func)
{
  return add_unique(rlist(), Entry(format, func));
}

/*! 
  Registers a new IO::Function for data output.  
  Every call overwrites a formerly registered handler for the same format string
  (case sensitive).
 */
bool IO::defineOutputHandler(QString const& format, IO::Function func)
{
  return add_unique(wlist(), Entry(format, func));
}

/*! 
  Registers a new Functor for data output.\n
  Every call overwrites a formerly registered handler for the same format string
  (case sensitive).
*/
bool IO::defineOutputHandler(QString const& format, IO::Functor const& func)
{
  return add_unique(wlist(), Entry(format, func));
}

/*!
  Applies a reading IO::Function or IO::Functor.
  \param plot Plot with the content that should be loaded
  \param fname File name 
  \param format Input format
  \return The return value from the called Function/Functor. 
  The function returns false, if no registered handler could be found.
*/
bool IO::load(Plot3D* plot, QString const& fname, QString const& format)
{
  IT it = IO::find(rlist(), format);

  if (it == rlist().end())
    return false;

  return (*it->iofunc)(plot, fname);
}

/*!
  Applies a writing IO::Function or IO::Functor.
  \param plot Plot with the content that should be saved
  \param fname File name
  \param format Output format
  \return The return value from the called Function/Functor.
  The function returns false, if no registered handler could be found.
*/
bool IO::save(Plot3D* plot, QString const& fname, QString const& format)
{
  IT it = IO::find(wlist(), format);

  if (it == wlist().end())
    return false;

  return (*it->iofunc)(plot, fname);
}

/*!
  Returns a list of currently registered input formats. 
*/
QStringList IO::inputFormatList()
{
  QStringList list;
  for ( IT it = rlist().begin(); it!=rlist().end(); ++it )
    list.append(it->fmt);
  
  return list;
}

/*!
  Returns a list of currently registered output formats. 
*/
QStringList IO::outputFormatList()
{
  QStringList list;  
  for ( IT it = wlist().begin(); it!=wlist().end(); ++it )
    list.append(it->fmt);
  
  return list;
}

/*! 
  Returns the input functor in charge for format and 0 if non-existent. 
*/
IO::Functor* IO::inputHandler(QString const& format)
{
  IO::IT it = IO::find(rlist(), format);
  
  if (it==rlist().end())
    return 0;

  return it->iofunc;
}

/*! 
  Returns the output functor in charge for format and 0 if non-existent. 
*/
IO::Functor* IO::outputHandler(QString const& format)
{
  IO::IT it = IO::find(wlist(), format);
  
  if (it==wlist().end())
    return 0;

  return it->iofunc;
}

bool PixmapWriter::operator()(Plot3D* plot, QString const& fname)
{
  QImage im = plot->grabFrameBuffer(true);
  
#if QT_VERSION < 0x040000
  QImageIO iio;
  iio.setImage(im);
#else
  QImageWriter iio;
#endif
  iio.setFormat(QWT3DLOCAL8BIT(fmt_));
  iio.setQuality(quality_);
  iio.setFileName(fname);
#if QT_VERSION < 0x040000
  return iio.write();
#else
  return iio.write(im);
#endif
}

//! Calls Qt's QImageIO::setQuality() function.
void PixmapWriter::setQuality(int val)
{
  quality_ = val;  
}

void IO::setupHandler()
{
#if QT_VERSION < 0x040000
  QStringList list = QImage::outputFormatList();
  QStringList::Iterator it = list.begin();
#else
  QList<QByteArray> list = QImageWriter::supportedImageFormats();
  QList<QByteArray>::Iterator it = list.begin();
#endif
  PixmapWriter qtw;
  while( it != list.end() ) 
  {
    qtw.fmt_ = *it;
    defineOutputHandler(*it, qtw);
    ++it;
  }
  VectorWriter vecfunc; 
  vecfunc.setCompressed(false);
  vecfunc.setFormat("EPS");
  defineOutputHandler("EPS", vecfunc);
  vecfunc.setFormat("PS");
  defineOutputHandler("PS", vecfunc);
  
#ifdef GL2PS_HAVE_ZLIB
  vecfunc.setCompressed(true);
  vecfunc.setFormat("EPS_GZ");
  defineOutputHandler("EPS_GZ", vecfunc);
  vecfunc.setFormat("PS_GZ");
  defineOutputHandler("PS_GZ", vecfunc);
#endif
  vecfunc.setFormat("PDF");
  defineOutputHandler("PDF", vecfunc);

  defineInputHandler("mes", NativeReader());
  defineInputHandler("MES", NativeReader());
}

/*!
	\deprecated  Use Plot3D::save or IO::save instead.
	
  Writes vector data supported by gl2ps. The corresponding format types are "EPS","PS"or "PDF".
  If zlib has been configured this will be extended by "EPS_GZ" and "PS_GZ". 
	\b Beware: BSPSORT turns out to behave very slowly and memory consuming, especially in cases where
	many polygons appear. It is still more exact than SIMPLESORT.
*/
bool Plot3D::saveVector(QString const& fileName, QString const& format, VectorWriter::TEXTMODE text, VectorWriter::SORTMODE sortmode)
{
  if (format == "EPS" || format == "EPS_GZ" || format == "PS" 
    || format == "PS_GZ" || format == "PDF")
  {  
    VectorWriter* gl2ps = (VectorWriter*)IO::outputHandler(format);
    if (gl2ps)
    {
      gl2ps->setSortMode(sortmode);
      gl2ps->setTextMode(text);
    }
    return IO::save(this, fileName, format);
  }
  return false;
}	
/*!
	\deprecated  Use Plot3D::save or IO::save instead.
  
  Saves the framebuffer to the file fileName using one of the image file formats supported by Qt.
*/
bool Plot3D::savePixmap(QString const& fileName, QString const& format)
{
  if (format == "EPS" || format == "EPS_GZ" || format == "PS" 
    || format == "PS_GZ" || format == "PDF")
    return false;
  
  return IO::save(this, fileName, format);
}

/*! 
  Saves content in one of the registered output formats. To modify the 
  behaviour for more complex output handling use IO::outputHandler.
*/
bool Plot3D::save(QString const& fileName, QString const& format)
{
  return IO::save(this, fileName, format);
}

