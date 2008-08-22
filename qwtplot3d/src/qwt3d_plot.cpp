#if defined(_MSC_VER) /* MSVC Compiler */
#pragma warning ( disable : 4305 )
#pragma warning ( disable : 4786 )
#endif

#include <QtDebug>

#include "qwt3d_plot.h"
#include "qwt3d_enrichment.h"
#include "qwt3d_curve.h"

using namespace Qwt3D;
	
/*!
  This should be the first call in your derived classes constructor.
*/
#if QT_VERSION < 0x040000
Plot3D::Plot3D(QWidget* parent, const char* name)
    : QGLWidget(parent, name)
#else
Plot3D::Plot3D(QWidget* parent, const QGLWidget* shareWidget)
	: ExtGLWidget(parent, shareWidget) 
#endif
{
	renderpixmaprequest_ = false;
	displaylegend_		 = false;

	displaylists_p = std::vector<GLuint>(DisplayListSize);
	for (unsigned k=0; k!=displaylists_p.size(); ++k)
	{
		displaylists_p[k] = 0;
	}

	curve_p = 0;
	legend_.setLimits(0, 100);
	legend_.setMajors(10);
	legend_.setMinors(2);
	legend_.setOrientation(ColorLegend::BottomTop, ColorLegend::Left);

	setBackgroundColor(RGBA(1.0,1.0,1.0,1.0));
	update_coordinate_sys_ = true;
}

/*!
  Release allocated resources
*/
Plot3D::~Plot3D()
{
	int i;

	makeCurrent();
	SaveGlDeleteLists( displaylists_p[0], displaylists_p.size() );

    for (i = 0; i < curvelist_p.size(); ++i) {
        delete curvelist_p[i];
    }
    for (DrawableList::const_iterator itr = drawablelist_p.begin(); itr != drawablelist_p.end(); ++itr ) {
        delete (*itr);
    }
    for (i = 0; i < titlelist_p.size(); ++i) {
        delete titlelist_p[i];
    }
}

/*!
  Set up the OpenGL rendering state
*/
void Plot3D::initializeGL()
{
	ExtGLWidget::initializeGL();
	if (renderpixmaprequest_) {
		renderpixmaprequest_ = false;
	}
}

//! Reimplements QGLWidget::renderPixmap
QPixmap Plot3D::renderPixmap(int w/* =0 */, int h/* =0 */, bool useContext/* =false */)
{
	renderpixmaprequest_ = true;
	return QGLWidget::renderPixmap(w, h, useContext);
}

/*!
  Paint the widgets content.
*/
void Plot3D::paintGL()
{
	int i;

	glClearColor(bgcolor_.r, bgcolor_.g, bgcolor_.b, bgcolor_.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	applyLights();

	glRotatef( -90, 1.0, 0.0, 0.0 ); 
	glRotatef( 0.0, 0.0, 1.0, 0.0 ); 
	glRotatef( 0.0, 0.0, 0.0, 1.0 );

	if (displaylegend_)		legend_.draw();

	CurveList::const_iterator c = curvelist_p.begin();
	for (i = 0; i != titlelist_p.size(); ++i) {
		titlelist_p[i]->setRelPosition((*c)->titlerel_, (*c)->titleanchor_);
		titlelist_p[i]->draw();
		++c;
	}

	for (DrawableList::const_iterator itr = drawablelist_p.begin(); itr != drawablelist_p.end(); ++itr ) {
		(*itr)->draw();
	}

	ExtGLWidget::applyModelViewAndProjection(coordinates_p.first(), coordinates_p.second());

	if (lighting_enabled_)	glEnable(GL_NORMALIZE);

	for (unsigned int j = 0; j != displaylists_p.size(); ++j) {
		if (j != LegendObject)		glCallList( displaylists_p[j] );
	}

	// ripped from old updateData method
	if ( update_coordinate_sys_ ) {
		GLStateBewarer dt(GL_DEPTH_TEST, true);
		GLStateBewarer ls(GL_LINE_SMOOTH, true);
		createCoordinateSystem();
	}
	coordinates_p.draw();
	
	for (i = 0; i != curvelist_p.size(); ++i) {
		curvelist_p[i]->draw();
	}

	if (lighting_enabled_)	glDisable(GL_NORMALIZE);

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
  Add a Curve object to the plot.
  Takes ownership of the object and will delete it when finished.
 */
void Plot3D::addCurve(Curve* curve)
{
	if (!curve || curvelist_p.contains(curve))		return;

	if (curve->plot() != this)	curve->setPlot(this);
	curvelist_p.push_back(curve);
}

/*!
  Remove a Curve object from the plot.
  Erases the curve from the object list.
 */
bool Plot3D::removeCurve(Curve* curve)
{
	if (!curve || !curvelist_p.contains(curve))		return false;

	QVector<Curve*>::iterator c;
	for (c = curvelist_p.begin(); c != curvelist_p.end(); ++c) {
		if ((*c) == curve) {
			curvelist_p.erase(c);
			if (curve == curve_p)
				setCurve(curvelist_p.size() ? curvelist_p.last() : 0);
			return true;
		}
	}
	return false;
}

/*!
  Add a Drawable object to the plot.
  Takes ownership of the object and will delete it when finished.
 */
void Plot3D::addDrawable(Drawable* drawable)
{
	if (!drawable || drawablelist_p.contains(drawable))		return;

    drawablelist_p.push_back(drawable);
}

/*!
  Remove a Drawable object from the plot.
  Erases the drawable from the object list.
 */
bool Plot3D::removeDrawable(Drawable* drawable)
{
	if (!drawable || !drawablelist_p.contains(drawable))	return false;

	QVector<Drawable*>::iterator d;
	for (d = drawablelist_p.begin(); d != drawablelist_p.end(); ++d) {
		if ((*d) == drawable) {
			drawablelist_p.erase(d);
			return true;
		}
	}
	return false;
}

/*!
  Add a Title object to the plot.
  Takes ownership of the object and will delete it when finished.
 */
void Plot3D::addTitle(Label* label)
{
	if (!label || titlelist_p.contains(label))		return;
//	if (label->string().isEmpty() || ((label->string() == TITLE) && titlelist_p.size()))	return;

	if (title() != label)	setTitle(label);
	titlelist_p.push_back(label);
}

/*!
  Remove a Title object from the plot.
  Erases the curve from the object list.
 */
bool Plot3D::removeTitle(Label* title)
{
	if (!title || !titlelist_p.contains(title))		return false;

	QVector<Label*>::iterator t;
	for (t = titlelist_p.begin(); t != titlelist_p.end(); ++t) {
		if ((*t) == title) {
			titlelist_p.erase(t);
			if (title == title_p)
				setTitle(titlelist_p.size() ? titlelist_p.last() : 0);
			return true;
		}
	}
	return false;
}

void Plot3D::manageConnect(bool connect, Qwt3D::Curve* curve)
{
	qDebug() << "Plot3D:" << (connect ? "Connecting" : "Disconnecting") << "Curve(s)";

	curve ? (connect ? curve->connects() : curve->disconnects()) :
			childConnect(connect);
}

void Plot3D::childConnect(bool connect)
{
	qDebug() << "Plot3D:" << (connect ? "Connecting" : "Disconnecting") << "All Curves";

	foreach (Curve* curve, curvelist_p) {
		if (curve)	connect ? curve->connects() : curve->disconnects();
	}
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

/*!
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
        for ( int i = 0; i < curvelist_p.size(); ++i ) {
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
		for ( int i = 0; i < curvelist_p.size(); ++i ) {
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
  Set style of coordinate system
*/
void Plot3D::setCoordinateStyle(COORDSTYLE st)
{
	coordinates_p.setStyle(st);
	updateGL();
}

/*!
  Update OpenGL data representation
*/
void Plot3D::updateData()
{
	update_coordinate_sys_ = true;
	updateGL();
}
