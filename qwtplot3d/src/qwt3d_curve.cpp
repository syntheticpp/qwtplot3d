#include "qwt3d_curve.h"

using namespace std;
using namespace Qwt3D;

/**
Initializes with dataNormals()==false, NOFLOOR, resolution() == 1
*/
Curve::Curve()
{
    plot_p = 0;
    plotstyle_ = FILLEDMESH;
    userplotstyle_p = 0;
    shading_ = GOURAUD;
    floorstyle_ = NOFLOOR;
    isolines_ = 10;

    datanormals_p = false;
    normalLength_p = 0.02;
    normalQuality_p = 3;

    resolution_p = 1;
    actualDataG_ = new GridData();
    actualDataC_ = new CellData();

    actualData_p = actualDataG_;

    floorstyle_ = NOFLOOR;
    displaylists_p.resize(DisplayListSize, 0);
    meshLineWidth_ = 1;
    datacolor_p = new StandardColor(this, 100);
    setPolygonOffset(0.5);
    setMeshColor(RGBA(0.0,0.0,0.0));
    setMeshLineWidth(1);

    update_displaylists_ = false;
}

Curve::~Curve()
{
    delete actualDataG_;
    delete actualDataC_;
    for (ELIT it = elist_p.begin(); it!=elist_p.end(); ++it)
        delete (*it);
    elist_p.clear();
}

void Curve::setPlot(Plot3D* p)
{
    plot_p = p;
}

void Curve::draw()
{
    // if the display list needs to be updated
    if ( update_displaylists_ ) {

        // create display list if necessary
        if ( 0 == displaylists_p[DataObject] ) {
            displaylists_p[DataObject] = glGenLists(1);
        }

        // glGenLists returns 0 on error. So, if displaylists_p[DataObject] is still 0,
        // then just call drawImplementation()
        if ( 0 == displaylists_p[DataObject] ) {
            drawImplementation();
            return;
        }

        glNewList(displaylists_p[DataObject], GL_COMPILE);
        drawImplementation();
        glEndList();

        update_displaylists_ = false;

        updateNormals();
    }

    // glGenLists returns 0 on error. So, if displaylists_p[DataObject] is still 0,
    // then just call drawImplementation()
    if ( 0 == displaylists_p[DataObject] ) {
        drawImplementation();
        return;
    }

    for ( unsigned int i=0; i < DisplayListSize; ++i ) {
        glCallList(displaylists_p[i]);
    }
}

void Curve::drawImplementation()
{
    createEnrichments();
    createData();
}

void Curve::setMeshColor(RGBA rgba)
{
    meshcolor_ = rgba;
    update_displaylists_ = true;
}

/*!
    assign a new coloring object for the data.
*/
void Curve::setDataColor( Color* col )
{
    Q_ASSERT(datacolor_p);

    if ( col == datacolor_p ) {
        return;
    }

    datacolor_p->destroy();
    datacolor_p = col;
    update_displaylists_ = true;
}


void Curve::showNormals(bool b)
{
    datanormals_p = b;
    update_displaylists_ = true;
}

/**
Values < 0 or > 1 are ignored
*/
void Curve::setNormalLength(double val)
{
    if (val<0 || val>1)
        return;
    normalLength_p = val;
    update_displaylists_ = true;
}

/**
Values < 3 are ignored 
*/
void Curve::setNormalQuality(int val) 
{
    if (val<3)
        return;
    normalQuality_p = val;
    update_displaylists_ = true;
}

/**
    Calculates the smallest x-y-z parallelepiped enclosing the data.
    It can be accessed by hull();
*/
void Curve::calculateHull()
{
    if (actualData_p->empty())
        return;
    setHull(actualData_p->hull());
}

/*!
  Sets data resolution (res == 1 original resolution) and updates widget
    If res < 1, the function does nothing
*/
void Curve::setResolution( int res )
{
    if (!actualData_p || actualData_p->datatype == Qwt3D::POLYGON)
        return;
  
    if ((resolution_p == res) || res < 1)
        return;
    
    resolution_p = res;
    calculateHull();
    update_displaylists_ = true;
    //emit resolutionChanged(res);
}

void Curve::updateData()
{
    update_displaylists_ = true;
    // TODO? derive this class from QObject and emit a signal?
    if ( plot_p ) {
        calculateHull();
        plot_p->updateData();
    }
}

void Curve::queueUpdate()
{
    update_displaylists_ = true;
    if ( plot_p ) {
        plot_p->update();
    }
}

void Curve::updateNormals()
{
    SaveGlDeleteLists(displaylists_p[NormalObject], 1); 
    
    if (plotStyle() == NOPLOT && !normals() || !actualData_p)
        return;

    displaylists_p[NormalObject] = glGenLists(1);
    glNewList(displaylists_p[NormalObject], GL_COMPILE);

    if (actualData_p->datatype == Qwt3D::POLYGON)
        createNormalsC();
    else if (actualData_p->datatype == Qwt3D::GRID)
        createNormalsG();
        
    glEndList();
}

void Curve::createData()
{
    if (!actualData_p)
        return;

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

    if (actualData_p->datatype == Qwt3D::POLYGON)
        createDataC();
    else if (actualData_p->datatype == Qwt3D::GRID)
        createDataG();
}


void Curve::createFloorData()
{
    if (!actualData_p)
    return;
  if (actualData_p->datatype == Qwt3D::POLYGON)
    createFloorDataC();
  else if (actualData_p->datatype == Qwt3D::GRID)
   createFloorDataG();  
}

/**
    The returned value is not affected by resolution(). The pair gives (columns,rows) for grid data
, (number of cells,1) for free formed data (datatype() == POLYGON) and (0,0) else
*/
pair<int,int> Curve::facets() const
{
    if (!hasData())
        return pair<int,int>(0,0);

  if (actualData_p->datatype == Qwt3D::POLYGON)
      return pair<int,int>(int(actualDataC_->cells.size()), 1);
  else if (actualData_p->datatype == Qwt3D::GRID)
      return pair<int,int>(actualDataG_->columns(), actualDataG_->rows());     
  else
    return pair<int,int>(0,0);    
}

void Curve::createPoints()
{
  Dot pt;
  createEnrichment(pt);
}

Enrichment* Curve::addEnrichment(Enrichment const& e)
{
    ELIT it = std::find( elist_p.begin(), elist_p.end(), &e );
    if ( elist_p.end() == it )
    {
        elist_p.push_back(e.clone());
        update_displaylists_ = true;
    }
    else
    {
        return *it;
    }
    return elist_p.back();
}

bool Curve::degrade(Enrichment* e)
{
    ELIT it = std::find(elist_p.begin(), elist_p.end(), e);
    
    if ( it != elist_p.end() )
    {
        delete (*it);
        elist_p.erase(it);
        update_displaylists_ = true;
        return true;
    }
    return false;
}

void Curve::createEnrichments()
{
  for (ELIT it = elist_p.begin(); it!=elist_p.end(); ++it)
  {
    this->createEnrichment(**it);
  } 
}

void Curve::createEnrichment(Enrichment& p)
{
    if (!actualData_p)
        return;
  
    //todo future work
    if (p.type() != Enrichment::VERTEXENRICHMENT)
        return;

    p.assign(*this);
    p.drawBegin();

    VertexEnrichment* ve = (VertexEnrichment*)&p; 
    if (actualData_p->datatype == Qwt3D::POLYGON)
    {    
        for (unsigned i = 0; i != actualDataC_->normals.size(); ++i) 
            ve->draw(actualDataC_->nodes[i]);
    }
    else if (actualData_p->datatype == Qwt3D::GRID)
    {
        int step = resolution();
        for (int i = 0; i <= actualDataG_->columns() - step; i += step) 
            for (int j = 0; j <= actualDataG_->rows() - step; j += step) 
                ve->draw(Triple(actualDataG_->vertices[i][j][0],
                                actualDataG_->vertices[i][j][1],
                                actualDataG_->vertices[i][j][2]));
    }
    p.drawEnd(); 
}


/*!
  Set plotstyle for the standard plotting types. An argument of value Qwt3D::USER
  is ignored.
*/
void Curve::setPlotStyle( PLOTSTYLE val )
{
   if (val == Qwt3D::USER)
      return;
   if ( val == plotstyle_ )
       return;
   delete userplotstyle_p;
   userplotstyle_p = 0;
   plotstyle_ = val;
   update_displaylists_ = true;
}

/*!
  Set plotstyle to Qwt3D::USER and an associated enrichment object.
*/
Qwt3D::Enrichment* Curve::setPlotStyle( Qwt3D::Enrichment const& obj )
{
    if (&obj == userplotstyle_p)
        return userplotstyle_p;

    delete userplotstyle_p;
    userplotstyle_p = obj.clone();
    plotstyle_ = Qwt3D::USER;
    update_displaylists_ = true;
    return userplotstyle_p;
}

/*!
  Set shading style
*/
void Curve::setShading( SHADINGSTYLE val )
{
    if (val == shading_)
        return;
    
    shading_ = val;

    update_displaylists_ = true;
}

/*!
  Set number of isolines. The lines are equidistant between minimal and maximal Z value
*/
void Curve::setIsolines(int steps)
{
    if (steps < 0 || steps == isolines_)
        return;

    isolines_ = steps;
    update_displaylists_ = true;
}

/*!
  Set Polygon offset. The function affects the OpenGL rendering process. 
    Try different values for surfaces with polygons only and with mesh and polygons
*/
void Curve::setPolygonOffset( double val )
{
    if ( val == polygonOffset_ )
        return;

    polygonOffset_ = val;
    update_displaylists_ = true;
}

void Curve::setMeshLineWidth( double val )
{
    Q_ASSERT(val>=0);

    if (val < 0 || val == meshLineWidth_ )
        return;
    
    meshLineWidth_ = val;
    update_displaylists_ = true;
}
