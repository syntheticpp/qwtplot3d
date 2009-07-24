#include <qmetaobject.h>

#include <qframe.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qaction.h>
#include <qmenubar.h>

#if QT_VERSION < 0x040000
#include <qpopupmenu.h>
#endif

#include <qtoolbar.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qfiledialog.h>
#include <qstatusbar.h>
#include <qfileinfo.h>
#include <qslider.h>
#include <qtimer.h>
#include <qcombobox.h>
#include <qstring.h>
#include <qcheckbox.h>
#include <qcolordialog.h>
#include <qfontdialog.h>

#include "mesh2mainwindow.h"

#include "functions.h"
#include "colormapreader.h"
#include "lightingdlg.h"
#include "femreader.h"
#include <qwt3d_io.h>
#include <qwt3d_io_gl2ps.h>
#include <qwt3d_io_reader.h>

using namespace Qwt3D;
using namespace std;

bool Mesh2MainWindow::connectA (const QObject* sender, const char * slot)
{
#if QT_VERSION < 0x040000
    return connect( sender, SIGNAL( activated() ), this, slot );
#else
    return connect( sender, SIGNAL( triggered() ), this, slot );
#endif
}

bool Mesh2MainWindow::connectAG (const QObject* sender, const char * slot)
{
#if QT_VERSION < 0x040000
    return connect( sender, SIGNAL( selected( QAction* ) ), this, slot ) ;
#else
    return connect( sender, SIGNAL( triggered( QAction* ) ), this, slot ) ;
#endif
}

Mesh2MainWindow::~Mesh2MainWindow()
{
    delete dataWidget;
}

Mesh2MainWindow::Mesh2MainWindow( QWidget* parent )
	: DummyBase( parent )
{
#if QT_VERSION < 0x040000
    setCaption("Mesh2");
    QGridLayout *grid = new QGridLayout( frame, 0, 0 );
#else
    setupWorkaround(this);
    setupUi(this);
    QGridLayout *grid = new QGridLayout( frame );
#endif

    col_ = 0;
    legend_ = false;
    redrawWait = 50;
    activeCoordSystem = None;
    activeCoordSystem->setChecked(true);

    activePlotStyle = filledmesh;
    filledmesh->setChecked(true);

    activeFloorStyle = floornone;
    activeFloorStyle->setChecked(true);

    dataWidget = new Plot3D(frame);
    //dataWidget->setDoubleLegend();

    d_function = 0;
    d_parametric_surface = 0;
    functionRadioButton->hide();
    connect( functionRadioButton, SIGNAL( clicked ( bool ) ), this, SLOT( setActiveFunction( bool ) ) );
    parametricRadioButton->hide();
    connect( parametricRadioButton, SIGNAL( clicked ( bool ) ), this, SLOT( setActiveParametric( bool )));
    connect( smoothLineCheckBox, SIGNAL( clicked ( bool ) ), this, SLOT( setSmoothLine( bool )));

    grid->addWidget( dataWidget, 0, 0 );

    connectAG( coord, SLOT( pickCoordSystem( QAction* ) ) );
    connectAG( plotstyle, SLOT( pickPlotStyle( QAction* ) ) );
    connectA( axescolor, SLOT( pickAxesColor() ) );
    connectA( backgroundcolor, SLOT( pickBgColor() ) );
    connectAG( floorstyle, SLOT( pickFloorStyle( QAction* ) ) );
    connectA( meshcolor, SLOT( pickMeshColor() ) );
    connectA( numbercolor, SLOT( pickNumberColor() ) );
    connectA( labelcolor, SLOT( pickLabelColor() ) );
    connectA( titlecolor, SLOT( pickTitleColor() ) );
    connectA( datacolor, SLOT( pickDataColor() ) );
    connectA( mingridcolor, SLOT( pickMinGridColor() ) );
    connectA( majgridcolor, SLOT( pickMajGridColor() ) );

    connect( lighting, SIGNAL( clicked() ), this, SLOT( pickLighting() ) );
    connectA( resetcolor, SLOT( resetColors() ) );
    connectA( numberfont, SLOT( pickNumberFont() ) );
    connectA( labelfont, SLOT( pickLabelFont() ) );
    connectA( titlefont, SLOT( pickTitleFont() ) );
    connectA( resetfont, SLOT( resetFonts() ) );
    connect( animation, SIGNAL( toggled(bool) ) , this, SLOT( toggleAnimation(bool) ) );
    connectA( dump, SLOT( dumpImage() ) );
    connectA( openFile, SLOT( open() ) );
    //connect(openFile, SIGNAL(triggered()), this, SLOT(open()));
    connectA( openMeshFile, SLOT( openMesh() ) );


    // only EXCLUSIVE groups emit selected :-/
    connect( left, SIGNAL( toggled( bool ) ), this, SLOT( setLeftGrid( bool ) ) );
    connect( right, SIGNAL( toggled( bool ) ), this, SLOT( setRightGrid( bool ) ) );
    connect( ceil, SIGNAL( toggled( bool ) ), this, SLOT( setCeilGrid( bool ) ) );
    connect( floor, SIGNAL( toggled( bool ) ), this, SLOT( setFloorGrid( bool ) ) );
    connect( back, SIGNAL( toggled( bool ) ), this, SLOT( setBackGrid( bool ) ) );
    connect( front, SIGNAL( toggled( bool ) ), this, SLOT( setFrontGrid( bool ) ) );

    timer = new QTimer( this );
    connect( timer, SIGNAL(timeout()), this, SLOT(rotate()) );

    resSlider->setRange(1,70);
    connect( resSlider, SIGNAL(valueChanged(int)), this, SLOT(setResolution(int)) );
    resSlider->setValue(1);

    connect( offsSlider, SIGNAL(valueChanged(int)), this, SLOT(setPolygonOffset(int)) );

    connect(normButton, SIGNAL(clicked()), this, SLOT(setStandardView()));

    QString qwtstr(" qwtplot3d ");
    qwtstr += QString::number(QWT3D_MAJOR_VERSION) + ".";
    qwtstr += QString::number(QWT3D_MINOR_VERSION) + ".";
    qwtstr += QString::number(QWT3D_PATCH_VERSION) + " ";

    QLabel* info = new QLabel(qwtstr, statusBar());
    statusBar()->addWidget(info, 0);
    filenameWidget = new QLabel("                                  ", statusBar());
    statusBar()->addWidget(filenameWidget,0);
    dimWidget = new QLabel("", statusBar());
    statusBar()->addWidget(dimWidget,0);
    rotateLabel = new QLabel("", statusBar());
    statusBar()->addWidget(rotateLabel,0);
    shiftLabel = new QLabel("", statusBar());
    statusBar()->addWidget(shiftLabel,0);
    scaleLabel = new QLabel("", statusBar());
    statusBar()->addWidget(scaleLabel,0);
    zoomLabel = new QLabel("", statusBar());
    statusBar()->addWidget(zoomLabel,0);

    connect(dataWidget, SIGNAL(rotationChanged(double,double,double)),this,SLOT(showRotate(double,double,double)));
    connect(dataWidget, SIGNAL(vieportShiftChanged(double,double)),this,SLOT(showShift(double,double)));
    connect(dataWidget, SIGNAL(scaleChanged(double,double,double)),this,SLOT(showScale(double,double,double)));
    connect(dataWidget, SIGNAL(zoomChanged(double)),this,SLOT(showZoom(double)));

    connect(functionCB, SIGNAL(activated(const QString&)), this, SLOT(createFunction(const QString&)));
    connect(psurfaceCB, SIGNAL(activated(const QString&)), this, SLOT(createPSurface(const QString&)));

    connect(projection, SIGNAL( toggled(bool) ), this, SLOT( toggleProjectionMode(bool)));
    connect(colorlegend, SIGNAL( toggled(bool) ), this, SLOT( toggleColorLegend(bool)));
    connect(autoscale, SIGNAL( toggled(bool) ), this, SLOT( toggleAutoScale(bool)));
    connect(shader, SIGNAL( toggled(bool) ), this, SLOT( toggleShader(bool)));
    connect(mouseinput, SIGNAL( toggled(bool) ), dataWidget, SLOT( enableMouse(bool)));
    connect(lightingswitch, SIGNAL( toggled(bool) ), this, SLOT( enableLighting(bool)));
    connect(normals, SIGNAL( toggled(bool) ), this, SLOT( showNormals(bool)));
    connect(normalsquality,  SIGNAL(valueChanged(int)), this, SLOT(setNormalQuality(int)) );
    connect(normalslength,  SIGNAL(valueChanged(int)), this, SLOT(setNormalLength(int)) );

    connect(gridLineStyleBox,  SIGNAL(activated(int)), this, SLOT(updateGridLineStyle(int)) );

    setStandardView();

    dataWidget->coordinates()->setLineSmooth(true);
    dataWidget->coordinates()->setGridLinesColor(RGBA(0.35,0.35,0.35,1));
    dataWidget->enableMouse(true);
    dataWidget->setKeySpeed(15,20,20);

    lightingdlg_ = new LightingDlg( this );
    lightingdlg_->assign( dataWidget);

#if QT_VERSION < 0x040000 //todo - restore, when Qt4 re-implements preview functionality
    datacolordlg_ = new QFileDialog( this );
    QDir dir("./../../data/colormaps");
    if (dir.exists("./../../data/colormaps"))
        datacolordlg_->setDir("./../../data/colormaps");
    datacolordlg_->setFilter("Colormap files (*.map *.MAP)");
    colormappv_ = new ColorMapPreview;
    datacolordlg_->setContentsPreviewEnabled( TRUE );
    datacolordlg_->setContentsPreview( colormappv_, colormappv_ );
    datacolordlg_->setPreviewMode( QFileDialog::Contents );
    connect(datacolordlg_, SIGNAL(fileHighlighted(const QString&)), this, SLOT(adaptDataColors(const QString&)));
#else
	//connect(datacolordlg_, SIGNAL(filesSelected(const QStringList&)), this, SLOT(adaptDataColors4(const QStringList&)));
#endif
    connect(filetypeCB, SIGNAL(activated(const QString&)), this, SLOT(setFileType(const QString&)));

  filetypeCB->clear();

  QStringList list = IO::outputFormatList();
#if QT_VERSION < 0x040000
  filetypeCB->insertStringList(list);
#else
  filetypeCB->insertItems(0,list);
#endif

    filetype_ = filetypeCB->currentText();
    dataWidget->setTitleFont( "Arial", 14, QFont::Normal );

    grids->setEnabled(false);

    PixmapWriter* pmhandler = (PixmapWriter*)IO::outputHandler("JPEG");
    if (!pmhandler)
        pmhandler = (PixmapWriter*)IO::outputHandler("jpeg"); //Qt4 naming scheme change
    if (pmhandler)
        pmhandler->setQuality(70);

    VectorWriter* handler = (VectorWriter*)IO::outputHandler("PDF");
    handler->setTextMode(VectorWriter::TEX);
    handler = (VectorWriter*)IO::outputHandler("EPS");
    handler->setTextMode(VectorWriter::TEX);
    handler = (VectorWriter*)IO::outputHandler("EPS_GZ");
    if (handler) // with zlib support only
        handler->setTextMode(VectorWriter::TEX);
}

void Mesh2MainWindow::open()
{
    #if QT_VERSION < 0x040000
        QString s = QFileDialog::getOpenFileName( "../../data", "GridData Files (*.mes *.MES)", this );
    #else
        QString s = QFileDialog::getOpenFileName( this, "", "../../data", "GridData Files (*.mes *.MES)");
    #endif

    if ( s.isEmpty() || !dataWidget)
        return;

    QFileInfo fi( s );

    #if QT_VERSION < 0x040000
        QString ext = fi.extension();   // ext = "gz"
        QToolTip::add(filenameWidget, s);
    #else
        filenameWidget->setToolTip(s);
        QString ext = fi.suffix();
    #endif
    filenameWidget->setText(fi.fileName());
    qApp->processEvents(); // enforces repaint;

    Curve *curve = dataWidget->curve();
    if (!curve)
        return;

    if (IO::load(dataWidget, s, ext)){
        double a = curve->facets().first;
        double b = curve->facets().second;

        dimWidget->setText(QString("Cells ") + QString::number(a*b)
                + " (" + QString::number(a) + "x" + QString::number(b) +")" );

        dataWidget->setResolution(3);
    }

    for (unsigned i=0; i!=dataWidget->coordinates()->axes.size(); ++i) {
        dataWidget->coordinates()->axes[i].setMajors(4);
        dataWidget->coordinates()->axes[i].setMinors(5);
        dataWidget->coordinates()->axes[i].setLabelString("");
    }

    updateColorLegend(4,5);
    pickCoordSystem(activeCoordSystem);
    dataWidget->showColorLegend(legend_);
}

void Mesh2MainWindow::createFunction(QString const& name)
{
    if (d_function && name == tr( "---" )){
        dataWidget->removeCurve(d_function);
        dataWidget->updateData();
        dataWidget->updateGL();
        delete d_function;
        d_function = 0;
        functionRadioButton->hide();
        return;
    }

    if (!d_function){
        d_function = new Curve(dataWidget);
        d_function->setSmoothMesh(true);
        d_function->setDataProjection(false);
        d_function->setProjection(BASE);
        d_function->setProjection(FACE, false);
        d_function->setProjection(SIDE, false);
        d_function->setResolution(resSlider->value());
        dataWidget->addCurve(d_function);
    }

    functionRadioButton->show();
    functionRadioButton->setChecked(true);
    functionRadioButton->setText(name);

    dataWidget->makeCurrent();
    dataWidget->setCurve(d_function);
    d_function->legend()->setScale(LINEARSCALE);
    d_function->legend()->setTitleString(name);

    for (unsigned i=0; i!=dataWidget->coordinates()->axes.size(); ++i){
        dataWidget->coordinates()->axes[i].setMajors(7);
        dataWidget->coordinates()->axes[i].setMinors(5);
    }

    if (name == QString("Rosenbrock")) {
        Rosenbrock rosenbrock(*d_function);
        rosenbrock.setMesh(50,51);
        rosenbrock.setDomain(-1.73,1.55,-1.5,1.95);
        rosenbrock.setMinZ(-100);
        rosenbrock.create();

        dataWidget->coordinates()->axes[Z1].setScale(LOG10SCALE);
        dataWidget->coordinates()->axes[Z2].setScale(LOG10SCALE);
        dataWidget->coordinates()->axes[Z3].setScale(LOG10SCALE);
        dataWidget->coordinates()->axes[Z4].setScale(LOG10SCALE);
        d_function->legend()->setScale(LOG10SCALE);
    } else if (name == QString("Hat")) {
        Hat hat(*d_function);

        hat.setMesh(51,72);
        hat.setDomain(-1.5,1.5,-1.5,1.5);
        hat.create();
    } else if (name == QString("Ripple")) {
        Ripple ripple(*d_function);
        ripple.setMesh(120,120);
        ripple.create();
    } else if (name == QString("Saddle")) {
        Saddle saddle;

        saddle.setMesh(71,71);
        double dom = 2.5;
        saddle.setDomain(-dom, dom, -dom, dom);
        saddle.assign(*d_function);
        saddle.create();
    } else if (name == QString("Sombrero")) {
        Mex mex;

        mex.setMesh(91, 91);
        double dom = 15;
        mex.setDomain(-dom, dom, -dom, dom);
        mex.create(*d_function);
    }

    double a = d_function->facets().first;
    double b = d_function->facets().second;

    dimWidget->setText(QString("Cells ") + QString::number(a*b)
            + " (" + QString::number(a) + "x" + QString::number(b) +")" );

    updateColorLegend(7,5);

    dataWidget->coordinates()->axes[X1].setLabelString(QString("X1"));
    dataWidget->coordinates()->axes[X2].setLabelString(QString("X2"));
    dataWidget->coordinates()->axes[X3].setLabelString(QString("X3"));
    dataWidget->coordinates()->axes[X4].setLabelString(QString("X4"));

    dataWidget->coordinates()->axes[Y1].setLabelString(QString("Y1"));
    dataWidget->coordinates()->axes[Y2].setLabelString(QString("Y2"));
    dataWidget->coordinates()->axes[Y3].setLabelString(QString("Y3"));
    dataWidget->coordinates()->axes[Y4].setLabelString(QString("Y4"));

    dataWidget->coordinates()->axes[Z1].setLabelString(QString("Z1"));
    dataWidget->coordinates()->axes[Z2].setLabelString(QString("Z2"));
    dataWidget->coordinates()->axes[Z3].setLabelString(QString("Z3"));
    dataWidget->coordinates()->axes[Z4].setLabelString(QString("Z4"));

    Curve *curve = dataWidget->curve();
    if (curve){
        d_function->setFloorStyle(curve->floorStyle());
        d_function->setPlotStyle(curve->plotStyle());
    }

    pickCoordSystem(activeCoordSystem);
    pickFloorStyle(activeFloorStyle);
    pickPlotStyle(activePlotStyle);
}

void Mesh2MainWindow::createPSurface(QString const& name)
{
    if (d_parametric_surface && name == tr( "---" )){
        dataWidget->removeCurve(d_parametric_surface);
        dataWidget->updateData();
        dataWidget->updateGL();
        delete d_parametric_surface;
        d_parametric_surface = 0;
        parametricRadioButton->hide();
        return;
    }

    if (!d_parametric_surface){
        d_parametric_surface = new Curve(dataWidget);
        d_parametric_surface->setSmoothMesh(true);
        d_parametric_surface->setDataProjection(false);
        d_parametric_surface->setProjection(BASE);
        d_parametric_surface->setProjection(FACE, false);
        d_parametric_surface->setProjection(SIDE, false);
        d_parametric_surface->setResolution(resSlider->value());
        dataWidget->addCurve(d_parametric_surface);
    }

    parametricRadioButton->show();
    parametricRadioButton->setChecked(true);
    parametricRadioButton->setText(name);

    dataWidget->makeCurrent();
    dataWidget->setCurve(d_parametric_surface);
    if (name == QString("Torus"))
    {
        Torus sf(*d_parametric_surface);
        sf.create();
    }
    else if (name == QString("Seashell"))
    {
        Seashell ss(*d_parametric_surface);
        ss.create();
    }
    else if (name == QString("Boy"))
    {
        Boy boy(*d_parametric_surface);
        boy.create();
    }
    else if (name == QString("Dini"))
    {
        Dini dini(*d_parametric_surface);
        dini.create();
    }
    else if (name == QString("Cone"))
    {
        TripleField conepos;
        CellField conecell;
        createCone(conepos,conecell);
        d_parametric_surface->loadFromData(conepos, conecell);
    }
    else if (name == QString("Sphere"))
    {
        Sphere sphere(*d_parametric_surface);
        sphere.create();
    }

    for (unsigned i=0; i!=dataWidget->coordinates()->axes.size(); ++i)
    {
        dataWidget->coordinates()->axes[i].setMajors(7);
        dataWidget->coordinates()->axes[i].setMinors(5);
    }

    double a = d_parametric_surface->facets().first;
    double b = d_parametric_surface->facets().second;

    dimWidget->setText(QString("Cells ") + QString::number(a*b)
            + " (" + QString::number(a) + "x" + QString::number(b) +")" );

	d_parametric_surface->legend()->setTitleString(name);
    updateColorLegend(7,5);

    dataWidget->coordinates()->axes[X1].setLabelString(QString("X1"));
    dataWidget->coordinates()->axes[X2].setLabelString(QString("X2"));
    dataWidget->coordinates()->axes[X3].setLabelString(QString("X3"));
    dataWidget->coordinates()->axes[X4].setLabelString(QString("X4"));

    dataWidget->coordinates()->axes[Y1].setLabelString(QString("Y1"));
    dataWidget->coordinates()->axes[Y2].setLabelString(QString("Y2"));
    dataWidget->coordinates()->axes[Y3].setLabelString(QString("Y3"));
    dataWidget->coordinates()->axes[Y4].setLabelString(QString("Y4"));

    dataWidget->coordinates()->axes[Z1].setLabelString(QString("Z1"));
    dataWidget->coordinates()->axes[Z2].setLabelString(QString("Z2"));
    dataWidget->coordinates()->axes[Z3].setLabelString(QString("Z3"));
    dataWidget->coordinates()->axes[Z4].setLabelString(QString("Z4"));

    pickCoordSystem(activeCoordSystem);
    pickFloorStyle(activeFloorStyle);
    pickPlotStyle(activePlotStyle);
}


void Mesh2MainWindow::pickCoordSystem( QAction* action)
{
    if (!action || !dataWidget)
            return;

    activeCoordSystem = action;

    dataWidget->setTitle("QwtPlot3D (Use Ctrl-Alt-Shift-LeftBtn-Wheel or keyboard)");

    if (!dataWidget->hasData())
    {
            double l = 0.6;
            dataWidget->createCoordinateSystem(Triple(-l,-l,-l), Triple(l,l,l));
            for (unsigned i=0; i!=dataWidget->coordinates()->axes.size(); ++i)
            {
                    dataWidget->coordinates()->axes[i].setMajors(4);
                    dataWidget->coordinates()->axes[i].setMinors(5);
            }
    }

    if (action == Box || action == Frame)
    {
            if (action == Box)
                    dataWidget->setCoordinateStyle(BOX);
            if (action == Frame)
                    dataWidget->setCoordinateStyle(FRAME);
            grids->setEnabled(true);
            gridLineStyleBox->setEnabled(true);
    }
    else if (action == None)
    {
      dataWidget->setTitle("QwtPlot3D (Use Ctrl-Alt-Shift-LeftBtn-Wheel or keyboard)");
            dataWidget->setCoordinateStyle(NOCOORD);
            grids->setEnabled(false);
            gridLineStyleBox->setEnabled(false);
    }
}

void Mesh2MainWindow::pickPlotStyle( QAction* action )
{
    if (!action || !dataWidget)
            return;

    activePlotStyle = action;

    Curve *curve = dataWidget->curve();
    if (!curve)
        return;

    if (action == polygon)
        curve->setPlotStyle(FILLED);
    else if (action == filledmesh)
        curve->setPlotStyle(FILLEDMESH);
    else if (action == wireframe)
        curve->setPlotStyle(WIREFRAME);
    else if (action == hiddenline)
        curve->setPlotStyle(HIDDENLINE);
    else if (action == pointstyle)
        curve->setPlotStyle(Qwt3D::POINTS);
    else
        curve->setPlotStyle(NOPLOT);

    dataWidget->updateGL();
}

void
Mesh2MainWindow::pickFloorStyle( QAction* action )
{
    if (!action || !dataWidget)
        return;

    activeFloorStyle = action;

    FLOORSTYLE style = NOFLOOR;
    if (action == floordata)
        style = FLOORDATA;
    else if (action == flooriso)
        style = FLOORISO;

    Curve *curve = dataWidget->curve();
    if (!curve)
        return;

    curve->setFloorStyle(style);
    dataWidget->updateGL();
}

void Mesh2MainWindow::setLeftGrid(bool b)
{
  setGrid(Qwt3D::LEFT,b);
}
void Mesh2MainWindow::setRightGrid(bool b)
{
  setGrid(Qwt3D::RIGHT,b);
}
void Mesh2MainWindow::setCeilGrid(bool b)
{
  setGrid(Qwt3D::CEIL,b);
}
void Mesh2MainWindow::setFloorGrid(bool b)
{
  setGrid(Qwt3D::FLOOR,b);
}
void Mesh2MainWindow::setFrontGrid(bool b)
{
  setGrid(Qwt3D::FRONT,b);
}
void Mesh2MainWindow::setBackGrid(bool b)
{
  setGrid(Qwt3D::BACK,b);
}

void Mesh2MainWindow::setGrid(int s, bool b)
{
    if (!dataWidget)
        return;

    int sum = dataWidget->coordinates()->grids();

    if (b)
        sum |= s;
    else
        sum &= ~s;

    dataWidget->coordinates()->setGridLines(sum!=Qwt3D::NOSIDEGRID, sum!=Qwt3D::NOSIDEGRID, sum);
    dataWidget->updateGL();
}

void Mesh2MainWindow::resetColors()
{
    if (!dataWidget)
        return;

    const RGBA axc = RGBA(0,0,0,1);
    const RGBA bgc = RGBA(1.0,1.0,1.0,1.0);
    const RGBA msc = RGBA(0,0,0,1);
    const RGBA nuc = RGBA(0,0,0,1);
    const RGBA lbc = RGBA(0,0,0,1);
    const RGBA tc = RGBA(0,0,0,1);

    dataWidget->coordinates()->setAxesColor(axc);
    dataWidget->setBackgroundColor(bgc);
    dataWidget->setMeshColor(msc);
    dataWidget->updateData();
    dataWidget->coordinates()->setNumberColor(nuc);
    dataWidget->coordinates()->setLabelColor(lbc);
    setMajGridColor(Qt::black);
    setMinGridColor(Qt::black);
    dataWidget->setTitleColor(tc);

    Curve *curve = dataWidget->curve();
    if (curve){
        col_ = new StandardColor(curve);
        dataWidget->setDataColor(col_);
        dataWidget->updateData();
        curve->updateNormals();
    }
    dataWidget->updateGL();
}


void Mesh2MainWindow::pickAxesColor()
{
    QColor c = QColorDialog::getColor( Qt::white, this );
    if ( !c.isValid() )
        return;
    RGBA rgb = Qt2GL(c);
    dataWidget->coordinates()->setAxesColor(rgb);
    dataWidget->updateGL();
}

void Mesh2MainWindow::pickBgColor()
{
    QColor c = QColorDialog::getColor( Qt::white, this );
    if ( !c.isValid() )
        return;
    RGBA rgb = Qt2GL(c);
    dataWidget->setBackgroundColor(rgb);
    dataWidget->updateGL();
}

void Mesh2MainWindow::pickMeshColor()
{
    QColor c = QColorDialog::getColor( Qt::white, this );
    if ( !c.isValid() )
        return;
    RGBA rgb = Qt2GL(c);
    dataWidget->setMeshColor(rgb);
    dataWidget->updateData();
    dataWidget->updateGL();
}

void Mesh2MainWindow::pickNumberColor()
{
    QColor c = QColorDialog::getColor( Qt::white, this );
    if ( !c.isValid() )
        return;
    RGBA rgb = Qt2GL(c);
    dataWidget->coordinates()->setNumberColor(rgb);
    dataWidget->curve()->legend()->axis()->setNumberColor(rgb);
    dataWidget->updateGL();
}

void Mesh2MainWindow::pickLabelColor()
{
    QColor c = QColorDialog::getColor( Qt::white, this );
    if ( !c.isValid() )
        return;
    RGBA rgb = Qt2GL(c);
    dataWidget->coordinates()->setLabelColor(rgb);
    dataWidget->updateGL();
}
void Mesh2MainWindow::pickTitleColor()
{
    QColor c = QColorDialog::getColor( Qt::white, this );
    if ( !c.isValid() )
        return;
    RGBA rgb = Qt2GL(c);
    dataWidget->setTitleColor(rgb);
    dataWidget->updateGL();
}

void Mesh2MainWindow::pickMinGridColor()
{
    Qwt3D::GridLine minGrid = dataWidget->coordinates()->minorGridLines()[X1];

    RGBA rgba = minGrid.color_;
    QColor c = QColorDialog::getColor(GL2Qt(rgba.r, rgba.g, rgba.b), this );
    if ( !c.isValid() )
        return;

    setMinGridColor(c);
    dataWidget->updateGL();
}

void Mesh2MainWindow::setMinGridColor(const QColor& c)
{
    Qwt3D::GridLine minGrid = dataWidget->coordinates()->minorGridLines()[X1];
    minGrid.color_ = Qt2GL(c);
    minGrid.visible_ = true;

    static QVector<Qwt3D::AXIS> axes;
    axes << Qwt3D::X1 << Qwt3D::X2 << Qwt3D::X3 << Qwt3D::X4;
    axes << Qwt3D::Y1 << Qwt3D::Y2 << Qwt3D::Y3 << Qwt3D::Y4;
    axes << Qwt3D::Z1 << Qwt3D::Z2 << Qwt3D::Z3 << Qwt3D::Z4;

    foreach(Qwt3D::AXIS axis, axes)
        dataWidget->coordinates()->setMinorGridLines(axis, minGrid);
}

void Mesh2MainWindow::pickMajGridColor()
{
    Qwt3D::GridLine majGrid = dataWidget->coordinates()->majorGridLines()[X1];

    RGBA rgba = majGrid.color_;
    QColor c = QColorDialog::getColor(GL2Qt(rgba.r, rgba.g, rgba.b), this);
    if (!c.isValid())
        return;

    setMajGridColor(c);
    dataWidget->updateGL();
}

void Mesh2MainWindow::setMajGridColor(const QColor& c)
{
    Qwt3D::GridLine majGrid = dataWidget->coordinates()->majorGridLines()[X1];
    majGrid.color_ = Qt2GL(c);
    majGrid.visible_ = true;

    static QVector<Qwt3D::AXIS> axes;
    axes << Qwt3D::X1 << Qwt3D::X2 << Qwt3D::X3 << Qwt3D::X4;
    axes << Qwt3D::Y1 << Qwt3D::Y2 << Qwt3D::Y3 << Qwt3D::Y4;
    axes << Qwt3D::Z1 << Qwt3D::Z2 << Qwt3D::Z3 << Qwt3D::Z4;

    foreach(Qwt3D::AXIS axis, axes)
        dataWidget->coordinates()->setMajorGridLines(axis, majGrid);
}

void Mesh2MainWindow::pickLighting()
{
	lightingdlg_->show();
}

void Mesh2MainWindow::pickDataColor()
{
#if QT_VERSION < 0x040000
    datacolordlg_->show();
#else
    QString s =  QFileDialog::getOpenFileName( this, "", "./../../data/colormaps", "Colormap files (*.map *.MAP)");
    adaptDataColors(s);
#endif
}

void Mesh2MainWindow::adaptDataColors(const QString& fileName)
{
    ColorVector cv;

    if (!openColorMap(cv, fileName))
        return;

    Curve *curve = dataWidget->curve();
    if (!curve)
        return;

    col_ = new StandardColor(curve);
    col_->setColorVector(cv);

    curve->setDataColor(col_);
    curve->updateNormals();
    dataWidget->updateData();
    dataWidget->showColorLegend(legend_);
    dataWidget->updateGL();
}

void Mesh2MainWindow::pickNumberFont()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, this );
    if ( !ok )
        return;

    dataWidget->coordinates()->setNumberFont(font);
    dataWidget->curve()->legend()->axis()->setNumberFont(font);
    dataWidget->updateGL();
}
void Mesh2MainWindow::pickLabelFont()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, this );
    if ( !ok )
        return;
    dataWidget->coordinates()->setLabelFont(font);
    dataWidget->updateGL();
}
void Mesh2MainWindow::pickTitleFont()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, this );
    if ( !ok )
        return;
    dataWidget->setTitleFont(font.family(), font.pointSize(), font.weight(), font.italic());
}

void Mesh2MainWindow::resetFonts()
{
    dataWidget->coordinates()->setNumberFont(QFont("Courier", 12));
    dataWidget->coordinates()->setLabelFont(QFont("Courier", 14, QFont::Bold));
    dataWidget->setTitleFont( "Arial", 14, QFont::Normal );
    dataWidget->updateGL();
}

void Mesh2MainWindow::setStandardView()
{
    dataWidget->setRotation(30,0,15);
    dataWidget->setViewportShift(0.05,0);
    dataWidget->setScale(1,1,1);
    dataWidget->setZoom(0.95);
}

void Mesh2MainWindow::dumpImage()
{
    static int counter = 0;
    if (!dataWidget)
        return;
    QString name;

    name = QString("dump_") + QString::number(counter++) + ".";

    if (filetype_ == "PS_GZ")
        name += "ps.gz";
    else if (filetype_ == "EPS_GZ")
        name += "eps.gz";
    else
        name += filetype_;

    #if QT_VERSION < 0x040000
        IO::save(dataWidget, name.lower(), filetype_);
    #else
        VectorWriter* vw = (VectorWriter*)IO::outputHandler("PDF");
        if (vw)
            vw->setSortMode(VectorWriter::BSPSORT);
        IO::save(dataWidget, name.toLower(), filetype_);
    #endif
}

/*!
  Turns animation on or off
*/
void Mesh2MainWindow::toggleAnimation(bool val)
{
    if ( val )
        timer->start( redrawWait ); // Wait this many msecs before redraw
    else
        timer->stop();
}

void Mesh2MainWindow::rotate()
{
    if (!dataWidget)
        return;

    dataWidget->setRotation(int(dataWidget->xRotation() + 1) % 360,
            int(dataWidget->yRotation() + 1) % 360,
            int(dataWidget->zRotation() + 1) % 360);
}

void
Mesh2MainWindow::toggleProjectionMode(bool val)
{
	dataWidget->setOrtho(val);
}

void
Mesh2MainWindow::toggleColorLegend(bool val)
{
    legend_ = val;
    dataWidget->showColorLegend(val);
}

void
Mesh2MainWindow::toggleAutoScale(bool val)
{
    dataWidget->coordinates()->setAutoScale(val);
    dataWidget->updateGL();
}

void
Mesh2MainWindow::toggleShader(bool val)
{
    if (val)
        dataWidget->setShading(GOURAUD);
    else
        dataWidget->setShading(FLAT);
    dataWidget->updateGL();
}

void
Mesh2MainWindow::setPolygonOffset(int val)
{
    dataWidget->setPolygonOffset(val / 10.0);
    dataWidget->updateData();
    dataWidget->updateGL();
}

void
Mesh2MainWindow::showRotate(double x, double y, double z)
{
	rotateLabel->setText(" Angles ("  + QString::number(x,'g',3) + " ,"
																	+ QString::number(y,'g',3) + " ,"
																	+ QString::number(z,'g',3) + ")");
}
void
Mesh2MainWindow::showShift(double x, double y)
{
	shiftLabel->setText(" Shifts (" + QString::number(x,'g',3) + " ,"
																	+ QString::number(y,'g',3) + " )"
																	);
}
void
Mesh2MainWindow::showScale(double x, double y, double z)
{
	scaleLabel->setText(" Scales (" + QString::number(x,'g',3) + " ,"
																	+ QString::number(y,'g',3) + " ,"
																	+ QString::number(z,'g',3) + ")");
}
void
Mesh2MainWindow::showZoom(double z)
{
    zoomLabel->setText(" Zoom "  + QString::number(z,'g',3));
}

void Mesh2MainWindow::openMesh()
{
    #if QT_VERSION < 0x040000
    QString data(QFileDialog::getOpenFileName( "../../data", "nodes (*.nod)", this ) );
    QString edges( QFileDialog::getOpenFileName( "../../data", "connectivities (*.cel)", this ) );
    #else
    QString data( QFileDialog::getOpenFileName( this, "", "../../data", "nodes (*.nod)") );
    QString edges( QFileDialog::getOpenFileName( this, "", "../../data", "connectivities (*.cel)") );
    #endif

    if ( data.isEmpty() || edges.isEmpty() || !dataWidget)
        return;

    Curve *curve = dataWidget->curve();
    if (!curve)
        return;

    TripleField vdata;
    CellField vpoly;

    readNodes(vdata, QWT3DLOCAL8BIT(data), NodeFilter());
    readConnections(vpoly, QWT3DLOCAL8BIT(edges), CellFilter());

    curve->loadFromData(vdata, vpoly);
    dimWidget->setText(QString("Cells ") + QString::number(curve->facets().first));

    for (unsigned i=0; i!=dataWidget->coordinates()->axes.size(); ++i){
        dataWidget->coordinates()->axes[i].setMajors(4);
        dataWidget->coordinates()->axes[i].setMinors(5);
        dataWidget->coordinates()->axes[i].setLabelString(QString(""));
    }

    updateColorLegend(4,5);
    pickCoordSystem(activeCoordSystem);
}

void
Mesh2MainWindow::showNormals(bool val)
{
    Curve *curve = dataWidget->curve();
    if (!curve)
        return;

    curve->showNormals(val);
    curve->updateNormals();
    dataWidget->updateGL();
}

void
Mesh2MainWindow::setNormalLength(int val)
{
    Curve *curve = dataWidget->curve();
    if (!curve)
        return;

    curve->setNormalLength(val / 400.);
    curve->updateNormals();
    dataWidget->updateGL();
}

void
Mesh2MainWindow::setNormalQuality(int val)
{
    Curve *curve = dataWidget->curve();
    if (!curve)
        return;

    curve->setNormalQuality(val);
    curve->updateNormals();
    dataWidget->updateGL();
}

bool
Mesh2MainWindow::openColorMap(ColorVector& cv, QString fname)
{
  if (fname.isEmpty())
    return false;

  ifstream file(QWT3DLOCAL8BIT(fname));

	if (!file)
		return false;

	RGBA rgb;
	cv.clear();

	while ( file )
	{
		file >> rgb.r >> rgb.g >> rgb.b;
		file.ignore(1000,'\n');
		if (!file.good())
			break;
		else
		{
			rgb.a = 1;
			rgb.r /= 255;
			rgb.g /= 255;
			rgb.b /= 255;
			cv.push_back(rgb);
		}
	}

	return true;
}

void
Mesh2MainWindow::updateColorLegend(int majors, int minors)
{
    dataWidget->curve()->legend()->setMajors(majors);
    dataWidget->curve()->legend()->setMinors(minors);
    double start, stop;
    dataWidget->coordinates()->axes[Z1].limits(start,stop);
    dataWidget->curve()->legend()->setLimits(start, stop);

    QVector<Qwt3D::Curve*> curves = dataWidget->curveList();
    for(int i = 0; i < curves.size(); i++){
    	Qwt3D::Curve* curve = curves[i];
		curve->setColorLegend(i, dataWidget->isDoubleLegend(),
			dataWidget->isDoubleLegend() ? QSize(2,28)  : QSize(3,32),
			dataWidget->isDoubleLegend() ? QPoint(7,10) : QPoint(3,10));

		curve->showColorLegend(colorlegend->isChecked());
    }
}

void Mesh2MainWindow::setFileType(QString const& name)
{
    filetype_ = name;
}

void Mesh2MainWindow::enableLighting(bool val)
{
    dataWidget->enableLighting(val);
    dataWidget->illuminate(0);
    dataWidget->updateGL();
}

void Mesh2MainWindow::setActiveFunction( bool on)
{
    if (dataWidget && d_function && on)
        dataWidget->setCurve(d_function);
}

void Mesh2MainWindow::setActiveParametric( bool on)
{
    if (dataWidget && d_parametric_surface && on)
        dataWidget->setCurve(d_parametric_surface);
}

void Mesh2MainWindow::setSmoothLine(bool smooth)
{
    QVector<Qwt3D::Curve*> curves = dataWidget->curveList();
    foreach(Qwt3D::Curve* curve, curves)
        curve->setSmoothMesh(smooth);

    dataWidget->coordinates()->setLineSmooth(smooth);
    dataWidget->updateGL();
}

void Mesh2MainWindow::setResolution(int resolution)
{
    dataWidget->setResolution(resolution);
    dataWidget->updateData();
}

void Mesh2MainWindow::updateGridLineStyle(int style)
{
    Qwt3D::GridLine majGrid = dataWidget->coordinates()->majorGridLines()[X1];
    majGrid.style_ = (Qwt3D::LINESTYLE)style;
    majGrid.visible_ = true;

    Qwt3D::GridLine minGrid = dataWidget->coordinates()->minorGridLines()[X1];
    minGrid.style_ = (Qwt3D::LINESTYLE)style;
    minGrid.visible_ = true;

    static QVector<Qwt3D::AXIS> axes;
    axes << Qwt3D::X1 << Qwt3D::X2 << Qwt3D::X3 << Qwt3D::X4;
    axes << Qwt3D::Y1 << Qwt3D::Y2 << Qwt3D::Y3 << Qwt3D::Y4;
    axes << Qwt3D::Z1 << Qwt3D::Z2 << Qwt3D::Z3 << Qwt3D::Z4;

    foreach(Qwt3D::AXIS axis, axes){
        dataWidget->coordinates()->setMajorGridLines(axis, majGrid);
        dataWidget->coordinates()->setMinorGridLines(axis, minGrid);
    }

    dataWidget->updateGL();
}
