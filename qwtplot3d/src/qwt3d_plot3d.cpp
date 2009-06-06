#if defined(_MSC_VER) /* MSVC Compiler */
#pragma warning ( disable : 4305 )
#pragma warning ( disable : 4786 )
#endif

#include "qwt3d_plot3d.h"
#include "qwt3d_enrichment.h"


using namespace Qwt3D;
	
/*!
  This should be the first call in your derived classes constructors.  
*/
Plot3D::Plot3D( QWidget * parent, const QGLWidget * shareWidget)
    : ExtGLWidget( parent, shareWidget) 
{  
  renderpixmaprequest_ = false;
	plotstyle_ = FILLEDMESH;
  userplotstyle_p = 0;
	shading_ = GOURAUD;
  isolinesZ_p.resize(10);
  delayisolinecalculation_p = true;
	displaylegend_ = false;
	smoothdatamesh_p = false;
  actualData_p = 0;

	setPolygonOffset(0.5);
	setMeshColor(RGBA(0.0,0.0,0.0));
	setMeshLineWidth(1);
	setBackgroundColor(RGBA(1.0,1.0,1.0,1.0));

	displaylists_p = std::vector<GLuint>(DisplayListSize);
	for (unsigned k=0; k!=displaylists_p.size(); ++k)
	{
		displaylists_p[k] = 0;
	}

	datacolor_p = new StandardColor(this, 100);
	title_.setFont("Courier", 16, QFont::Bold);
	title_.setString("");

	setTitlePosition(0.95);
	
	legend_.setLimits(0, 100);
	legend_.setMajors(10);
	legend_.setMinors(2);
	legend_.setOrientation(ColorLegend::BottomTop, ColorLegend::Left);
}

/*!
  Release allocated resources
*/

Plot3D::~Plot3D()
{
	makeCurrent();
  SaveGlDeleteLists( displaylists_p[0], displaylists_p.size() );
	datacolor_p->destroy();
  delete userplotstyle_p;
  for (ELIT it = elist_p.begin(); it!=elist_p.end(); ++it)
    delete (*it);

  elist_p.clear();
}

void Plot3D::initializeGL()
{
  ExtGLWidget::initializeGL();  
  if (renderpixmaprequest_)
  {
    updateData();
    renderpixmaprequest_ = false;
  }
}

//! Reimplements QGLWidget::renderPixmap
QPixmap Plot3D::renderPixmap(int w/* =0 */, int h/* =0 */, bool useContext/* =false */)
{
  renderpixmaprequest_ = true;
  return QGLWidget::renderPixmap(w,h,useContext);
}

/*!
  Paint the widgets content.
*/
void Plot3D::paintGL()
{
	glClearColor(bgcolor_.r, bgcolor_.g, bgcolor_.b, bgcolor_.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
  glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
  applyLights();

  glRotatef( -90, 1.0, 0.0, 0.0 ); 
  glRotatef( 0.0, 0.0, 1.0, 0.0 ); 
  glRotatef( 0.0, 0.0, 0.0, 1.0 );

	if (displaylegend_)
	{		
		legend_.draw();
	}
	title_.setRelPosition(titlerel_, titleanchor_);
	title_.draw();
	
	Triple beg = coordinates_p.first();
	Triple end = coordinates_p.second();
	
	Triple center = beg + (end-beg) / 2;
	double radius = (center-beg).length();
	
	glLoadIdentity();

  glRotatef( xRotation()-90, 1.0, 0.0, 0.0 ); 
  glRotatef( yRotation(), 0.0, 1.0, 0.0 ); 
  glRotatef( zRotation(), 0.0, 0.0, 1.0 );

	glScalef( zoom() * xScale(), zoom() * yScale(), zoom() * zScale() );
	
	glTranslatef(xShift()-center.x, yShift()-center.y, zShift()-center.z);
  
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();

	if (beg != end)
	{		
		if (ortho())
			glOrtho( -radius, +radius, -radius, +radius, 0, 40 * radius);
		else
			glFrustum( -radius, +radius, -radius, +radius, 5 * radius, 400 * radius );
	}
	else
	{
		if (ortho())
			glOrtho( -1.0, 1.0, -1.0, 1.0, 10.0, 100.0 );
		else
			glFrustum( -1.0, 1.0, -1.0, 1.0, 10.0, 100.0 );
	}

  glTranslatef( xViewportShift() * 2 * radius , yViewportShift() * 2 * radius , -7 * radius );
  
  if (lightingEnabled())
    glEnable(GL_NORMALIZE);

  for (unsigned i=0; i!= displaylists_p.size(); ++i)
	{
		if (i!=LegendObject)
			glCallList( displaylists_p[i] );
	}
  coordinates_p.draw();
	
  if (lightingEnabled())
    glDisable(GL_NORMALIZE);
  
  glMatrixMode( GL_MODELVIEW );
  glPopMatrix();
}


/*!
  Set up the OpenGL view port
*/
void Plot3D::resizeGL( int w, int h )
{
	glViewport( 0, 0, w, h );
	paintGL();
}

/*!
	Calculates the smallest x-y-z parallelepiped enclosing the data.
	It can be accessed by hull();
*/
void Plot3D::calculateHull()
{
	if (!actualData_p || actualData_p->empty())
		return;
	setHull(actualData_p->hull());
}

/*!
	Create a coordinate system with generating corners beg and end 
*/
void Plot3D::createCoordinateSystem( Triple beg, Triple end )
{
	if (beg != coordinates_p.first() || end != coordinates_p.second())
		coordinates_p.init(beg, end);
}

/*!
	Create a coordinate system from data
*/
void Plot3D::createCoordinateSystem()
{
	calculateHull();
  Triple beg = hull().minVertex;
  Triple end = hull().maxVertex;
  createCoordinateSystem(beg, end);
}

/*!
  Show a color legend
*/
void Plot3D::showColorLegend( bool show )
{
 	displaylegend_ = show;
	if (show)
    datacolor_p->createVector(legend_.colors);
	updateGL();
}

void Plot3D::setMeshColor(RGBA rgba)
{
	meshcolor_ = rgba;
}

void Plot3D::setBackgroundColor(RGBA rgba)
{
	bgcolor_ = rgba;
}


/*!
	assign a new coloring object for the data.
*/
void Plot3D::setDataColor( Color* col )
{
	Q_ASSERT(datacolor_p);

	datacolor_p->destroy();
	datacolor_p = col;
//  if ( displaylegend_ )
//    datacolor_p->createVector(legend_.colors);
}

/*!
  Set style of coordinate system
*/
void Plot3D::setCoordinateStyle(COORDSTYLE st)
{
	coordinates_p.setStyle(st);
	updateGL();
}

/*!
  Set plotstyle for the standard plotting types. An argument of value Qwt3D::USER
  is ignored.
*/
void Plot3D::setPlotStyle( PLOTSTYLE val )
{
  if (val == Qwt3D::USER)
    return;
  delete userplotstyle_p;
  userplotstyle_p = 0;
  plotstyle_ = val;
}

/*!
  Set plotstyle to Qwt3D::USER and an associated enrichment object.
*/
Qwt3D::Enrichment* Plot3D::setPlotStyle( Qwt3D::Enrichment const& obj )
{
  if (&obj == userplotstyle_p)
    return userplotstyle_p;
  
  delete userplotstyle_p;
  userplotstyle_p = obj.clone();
  plotstyle_ = Qwt3D::USER;
  return userplotstyle_p;
}

/*!
  Set shading style
*/
void Plot3D::setShading( SHADINGSTYLE val )
{
	if (val == shading_)
		return;
	
	shading_ = val;
	
	switch (shading_)
	{
		case FLAT:
			glShadeModel(GL_FLAT);
			break;
		case GOURAUD:
			glShadeModel(GL_SMOOTH);
			break;
		default:
			break;
	}
	updateGL();
}

/*!
  Set Polygon offset. The function affects the OpenGL rendering process. 
	Try different values for surfaces with polygons only and with mesh and polygons
*/
void Plot3D::setPolygonOffset( double val )
{
	polygonOffset_ = val;
}

void Plot3D::setMeshLineWidth( double val )
{
	Q_ASSERT(val>=0);

	if (val < 0)
		return;
	
	meshLineWidth_ = val;
}


/*!
Set relative caption position (0.5,0.5) means, the anchor point lies in the center of the screen
*/
void Plot3D::setTitlePosition(double rely, double relx, Qwt3D::ANCHOR anchor)
{
	titlerel_.y = (rely<0 || rely>1) ? 0.5 : rely;
	titlerel_.x = (relx<0 || relx>1) ? 0.5 : relx;

	titleanchor_ = anchor;
}

/*!
Set caption font
*/
void Plot3D::setTitleFont(const QString& family, int pointSize, int weight, bool italic)
{ 
	title_.setFont(family, pointSize, weight, italic);
}

Enrichment* Plot3D::addEnrichment(Enrichment const& e)
{
  if ( elist_p.end() == std::find( elist_p.begin(), elist_p.end(), &e ) )
    elist_p.push_back(e.clone());
  return elist_p.back();
}

bool Plot3D::degrade(Enrichment* e)
{
	ELIT it = std::find(elist_p.begin(), elist_p.end(), e);
	
	if ( it != elist_p.end() )
	{
		delete (*it);
    elist_p.erase(it);
    return true;
	}
  return false;
}

void Plot3D::drawEnrichments()
{
  for (ELIT it = elist_p.begin(); it!=elist_p.end(); ++it)
  {
    this->drawEnrichment(**it);
  } 
}

/*!
  Update OpenGL data representation
*/
void Plot3D::updateData()
{
	makeCurrent();
  GLStateBewarer dt(GL_DEPTH_TEST, true);
	GLStateBewarer ls(GL_LINE_SMOOTH, true);

	calculateHull();	

	SaveGlDeleteLists(displaylists_p[DataObject], 1); // nur Daten
	
	displaylists_p[DataObject] = glGenLists(1);
	glNewList(displaylists_p[DataObject], GL_COMPILE);
	
  this->drawEnrichments();
	this->createOpenGlData();
		
	glEndList();
}
