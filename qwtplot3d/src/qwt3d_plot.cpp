#if defined(_MSC_VER) /* MSVC Compiler */
#pragma warning ( disable : 4305 )
#pragma warning ( disable : 4786 )
#endif

#include "qwt3d_plot.h"
#include "qwt3d_enrichment.h"
#include "qwt3d_curve.h"

using namespace Qwt3D;
	
/*!
  This should be the first call in your derived classes constructors.  
*/
#if QT_VERSION < 0x040000
Plot3D::Plot3D( QWidget* parent, const char* name )
    : QGLWidget( parent, name )
#else
Plot3D::Plot3D( QWidget * parent, const QGLWidget * shareWidget)
    : QGLWidget( parent, shareWidget) 
#endif
{  
  initializedGL_ = false;
  renderpixmaprequest_ = false;
  xRot_ = yRot_ = zRot_ = 0.0;		// default object rotation
  
	xShift_ = yShift_ = zShift_ = xVPShift_ = yVPShift_ = 0.0;
	xScale_ = yScale_ = zScale_ = 1.0;
	zoom_ = 1;
	ortho_ = true;
	displaylegend_ = false;

	lastMouseMovePosition_ = QPoint(0,0);
	mpressed_ = false;
	mouse_input_enabled_ = true;

	displaylists_p = std::vector<GLuint>(DisplayListSize);
	for (unsigned k=0; k!=displaylists_p.size(); ++k)
	{
		displaylists_p[k] = 0;
	}

	title_.setFont("Courier", 16, QFont::Bold);
	title_.setString("");

	setTitlePosition(0.95);
	
  kbd_input_enabled_ = true;

#if QT_VERSION < 0x040000
  setFocusPolicy(QWidget::StrongFocus);
  assignMouse(Qt::LeftButton, 
							Qt::LeftButton | Qt::ShiftButton,
							Qt::LeftButton, 
							Qt::LeftButton | Qt::AltButton, 
							Qt::LeftButton | Qt::AltButton, 
							Qt::LeftButton | Qt::AltButton | Qt::ShiftButton,
							Qt::LeftButton | Qt::AltButton | Qt::ControlButton,
							Qt::LeftButton | Qt::ControlButton, 
							Qt::LeftButton | Qt::ControlButton);


  assignKeyboard(Qt::Key_Down, Qt::Key_Up,
    Qt::ShiftButton + Qt::Key_Right, Qt::ShiftButton + Qt::Key_Left,
    Qt::Key_Right, Qt::Key_Left,
    Qt::AltButton + Qt::Key_Right, Qt::AltButton + Qt::Key_Left,
    Qt::AltButton + Qt::Key_Down, Qt::AltButton + Qt::Key_Up,
    Qt::AltButton + Qt::ShiftButton + Qt::Key_Down, Qt::AltButton + Qt::ShiftButton + Qt::Key_Up,
    Qt::AltButton + Qt::ControlButton + Qt::Key_Down, Qt::AltButton + Qt::ControlButton + Qt::Key_Up,
    Qt::ControlButton + Qt::Key_Right, Qt::ControlButton + Qt::Key_Left,
    Qt::ControlButton + Qt::Key_Down, Qt::ControlButton + Qt::Key_Up
   );
#else
  setFocusPolicy(Qt::StrongFocus);
  assignMouse(Qt::LeftButton, 
							MouseState(Qt::LeftButton, Qt::ShiftModifier),
							Qt::LeftButton, 
							MouseState(Qt::LeftButton, Qt::AltModifier), 
							MouseState(Qt::LeftButton, Qt::AltModifier), 
							MouseState(Qt::LeftButton, Qt::AltModifier | Qt::ShiftModifier),
							MouseState(Qt::LeftButton, Qt::AltModifier | Qt::ControlModifier),
							MouseState(Qt::LeftButton, Qt::ControlModifier), 
							MouseState(Qt::LeftButton, Qt::ControlModifier)
              );


  assignKeyboard(Qt::Key_Down, Qt::Key_Up,
    KeyboardState(Qt::Key_Right, Qt::ShiftModifier), KeyboardState(Qt::Key_Left, Qt::ShiftModifier),
    Qt::Key_Right, Qt::Key_Left,
    KeyboardState(Qt::Key_Right, Qt::AltModifier), KeyboardState(Qt::Key_Left, Qt::AltModifier),
    KeyboardState(Qt::Key_Down, Qt::AltModifier), KeyboardState(Qt::Key_Up, Qt::AltModifier),
    KeyboardState(Qt::Key_Down, Qt::AltModifier|Qt::ShiftModifier), KeyboardState(Qt::Key_Up, Qt::AltModifier|Qt::ShiftModifier),
    KeyboardState(Qt::Key_Down, Qt::AltModifier|Qt::ControlModifier), KeyboardState(Qt::Key_Up, Qt::AltModifier|Qt::ControlModifier),
    KeyboardState(Qt::Key_Right, Qt::ControlModifier), KeyboardState(Qt::Key_Left, Qt::ControlModifier),
    KeyboardState(Qt::Key_Down, Qt::ControlModifier), KeyboardState(Qt::Key_Up, Qt::ControlModifier)
   );
#endif
  setKeySpeed(3,5,5);

	legend_.setLimits(0, 100);
	legend_.setMajors(10);
	legend_.setMinors(2);
	legend_.setOrientation(ColorLegend::BottomTop, ColorLegend::Left);

  lighting_enabled_ = false;
  disableLighting();
  lights_ = std::vector<Light>(8);
  setBackgroundColor(RGBA(1.0,1.0,1.0,1.0));
  update_coordinate_sys_ = true;
}

/*!
  Release allocated resources
*/

Plot3D::~Plot3D()
{
	makeCurrent();
	SaveGlDeleteLists( displaylists_p[0], displaylists_p.size() );

    for ( unsigned int i=0; i < curvelist_p.size(); ++i ) {
        delete curvelist_p[i];
    }
    for (DrawableList::const_iterator itr = drawablelist_p.begin(); itr != drawablelist_p.end(); ++itr ) {
        delete (*itr);
    }
}


/*!
  Set up the OpenGL rendering state
*/
void Plot3D::initializeGL()
{
  glEnable( GL_BLEND );
  glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	
  // Set up the lights

  disableLighting();
	
  GLfloat whiteAmb[4] = {1.0, 1.0, 1.0, 1.0};
    
  setLightShift(0, 0, 3000);
  glEnable(GL_COLOR_MATERIAL);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteAmb);

  setMaterialComponent(GL_DIFFUSE, 1.0);
  setMaterialComponent(GL_SPECULAR, 0.3);
  setMaterialComponent(GL_SHININESS, 5.0);
  setLightComponent(GL_DIFFUSE, 1.0);
  setLightComponent(GL_SPECULAR, 1.0);

  initializedGL_ = true;	
  if (renderpixmaprequest_)
  {
    renderpixmaprequest_ = false;
  }
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
	
        for (DrawableList::const_iterator itr = drawablelist_p.begin(); itr != drawablelist_p.end(); ++itr ) {
            (*itr)->draw();
        }

        applyModelViewAndProjection();

	
  
  if (lighting_enabled_)
    glEnable(GL_NORMALIZE);

    for (unsigned i=0; i!= displaylists_p.size(); ++i)
    {
        if (i!=LegendObject)
            glCallList( displaylists_p[i] );
    }

    // ripped from old updateData method
    if ( update_coordinate_sys_ ) {
        GLStateBewarer dt(GL_DEPTH_TEST, true);
        GLStateBewarer ls(GL_LINE_SMOOTH, true);
        createCoordinateSystem();
    }
    coordinates_p.draw();

    for ( unsigned int i=0; i < curvelist_p.size(); ++i ) {
        curvelist_p[i]->draw();
    }
	
  if (lighting_enabled_)
    glDisable(GL_NORMALIZE);
  
  glMatrixMode( GL_MODELVIEW );
  glPopMatrix();
}

void Plot3D::applyModelViewAndProjection()
{
	Triple beg = coordinates_p.first();
	Triple end = coordinates_p.second();
	
	Triple center = beg + (end-beg) / 2;
	double radius = (center-beg).length();
	
	glLoadIdentity();

  glRotatef( xRot_-90, 1.0, 0.0, 0.0 ); 
  glRotatef( yRot_, 0.0, 1.0, 0.0 ); 
  glRotatef( zRot_, 0.0, 0.0, 1.0 );

	glScalef( zoom_ * xScale_, zoom_ * yScale_, zoom_ * zScale_ );
	
	glTranslatef(xShift_-center.x, yShift_-center.y, zShift_-center.z);
  
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();

	if (beg != end)
	{		
		if (ortho_)
    {	
      glOrtho( -radius, +radius, -radius, +radius, 0, 40 * radius);
    }
    else
    {	
      glFrustum( -radius, +radius, -radius, +radius, 5 * radius, 400 * radius );
    }
  }
	else
	{
		if (ortho_)
			glOrtho( -1.0, 1.0, -1.0, 1.0, 10.0, 100.0 );
		else
			glFrustum( -1.0, 1.0, -1.0, 1.0, 10.0, 100.0 );
	}

  glTranslatef( xVPShift_ * 2 * radius , yVPShift_ * 2 * radius , -7 * radius );
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
  Reimplemented from QGLWidget
*/
QPixmap Plot3D::renderPixmap(int w/* =0 */, int h/* =0 */, bool useContext/* =false */)
{
  renderpixmaprequest_ = true;
  return QGLWidget::renderPixmap(w,h,useContext);
}

/*!
  Add a curve to the plot.
  Takes ownership of the object and will delete it when finished.
 */
void Plot3D::addCurve(Qwt3D::Curve* c)
{
    c->setPlot(this);
    curvelist_p.push_back(c);
}

/*!
  Add a Drawable object to the plot.
  Takes ownership of the object and will delete it when finished.
 */
void Plot3D::addDrawable(Qwt3D::Drawable* c)
{
    drawablelist_p.push_back(c);
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
    Qwt3D::ParallelEpiped the_hull = hull();
    Triple beg = the_hull.minVertex; // Irix 6.5 compiler bug
    Triple end = the_hull.maxVertex;
  createCoordinateSystem(beg, end);
}

void Plot3D::setPlotStyle( Qwt3D::PLOTSTYLE val)
{
    for ( unsigned int i=0; i < curvelist_p.size(); ++i ) {
        curvelist_p[i]->setPlotStyle(val);
}
}

Qwt3D::Enrichment* Plot3D::setPlotStyle( Qwt3D::Enrichment const& val)
{
    //TODO: Broken
    Qwt3D::Enrichment* en = 0;
    for ( unsigned int i=0; i < curvelist_p.size(); ++i ) {
        en = curvelist_p[i]->setPlotStyle(val);
    }
    return en;
}

void Plot3D::setDataColor( Color* col )
{
    for ( unsigned int i=0; i < curvelist_p.size(); ++i ) {
        curvelist_p[i]->setDataColor(col);
}
}
void Plot3D::setFloorStyle( Qwt3D::FLOORSTYLE val )
{
    for ( unsigned int i=0; i < curvelist_p.size(); ++i ) {
        curvelist_p[i]->setFloorStyle(val);
}
}
void Plot3D::setResolution(int val)
{
    for ( unsigned int i=0; i < curvelist_p.size(); ++i ) {
        curvelist_p[i]->setResolution(val);
    }
}

/**
  Update the hull
*/
void Plot3D::calculateHull()
{
    Qwt3D::ParallelEpiped the_hull;
  
    // account for the curves (if any)
    if ( !curvelist_p.empty() ) {
        the_hull.minVertex.x = DBL_MAX;
        the_hull.minVertex.y = DBL_MAX;
        the_hull.minVertex.z = DBL_MAX;
        the_hull.maxVertex.x = -DBL_MAX;
        the_hull.maxVertex.y = -DBL_MAX;
        the_hull.maxVertex.z = -DBL_MAX;
        for ( unsigned int i=0; i < curvelist_p.size(); ++i ) {
            Qwt3D::ParallelEpiped sub_hull = curvelist_p[i]->hull();

            if ( sub_hull.minVertex == sub_hull.maxVertex ) {
                continue;
            }

            if ( sub_hull.minVertex.x < the_hull.minVertex.x ) { the_hull.minVertex.x  = sub_hull.minVertex.x; }
            if ( sub_hull.minVertex.y < the_hull.minVertex.y ) { the_hull.minVertex.y  = sub_hull.minVertex.y; }
            if ( sub_hull.minVertex.z < the_hull.minVertex.z ) { the_hull.minVertex.z  = sub_hull.minVertex.z; }
            if ( sub_hull.maxVertex.x > the_hull.maxVertex.x ) { the_hull.maxVertex.x  = sub_hull.maxVertex.x; }
            if ( sub_hull.maxVertex.y > the_hull.maxVertex.y ) { the_hull.maxVertex.y  = sub_hull.maxVertex.y; }
            if ( sub_hull.maxVertex.z > the_hull.maxVertex.z ) { the_hull.maxVertex.z  = sub_hull.maxVertex.z; }
        }
    }
    setHull(the_hull);
//
//    Triple beg = the_hull.minVertex; // Irix 6.5 compiler bug
//    Triple end = the_hull.maxVertex;
//    std::cout << "Hull is from "
//              << "(" << beg.x << ", " << beg.y << ", " << beg.z << ")"
//              << " to "
//              << "(" << end.x << ", " << end.y << ", " << end.z << ")"
//              << std::endl;
}

/*!
  Show a color legend
*/
void Plot3D::showColorLegend( bool show )
	{
    displaylegend_ = show;
    // TODO: not sure how to work the legend!
    if (show) {
        for ( unsigned int i=0; i < curvelist_p.size(); ++i ) {
            Color* color = const_cast<Color*>(curvelist_p[i]->dataColor());
            if ( color ) {
                color->createVector(legend_.colors);
			break;
	}
        }
    }
	updateGL();
}

/*!
  Set up ortogonal or perspective mode and updates widget
*/
void Plot3D::setOrtho( bool val )
{
	if (val == ortho_)
		return;
	ortho_ = val;
	updateGL();

	emit projectionChanged(val);
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

/*!
  Update OpenGL data representation
*/
void Plot3D::updateData()
{
    update_coordinate_sys_ = true;
    update();
}
	
void Plot3D::setBackgroundColor(RGBA rgba)
{
	bgcolor_ = rgba;
}
	
		
