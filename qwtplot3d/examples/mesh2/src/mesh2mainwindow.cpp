#include <qframe.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qaction.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
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
#include "../src/reader.h"


using namespace Qwt3d;


Mesh2MainWindow::~Mesh2MainWindow()      
{
	delete dataWidget;
}

Mesh2MainWindow::Mesh2MainWindow( QWidget* parent, const char* name, WFlags f )       
	: Mesh2MainWindowBase( parent, name, f )
{
		legend_ = false;
		redrawWait = 50;
		activeCoordSystem = None;
		setCaption("Mesh2");      

		connect( coord, SIGNAL( selected( QAction* ) ), this, SLOT( pickCoordSystem( QAction* ) ) );
		connect( plotstyle, SIGNAL( selected( QAction* ) ), this, SLOT( pickPlotStyle( QAction* ) ) );
		connect( axescolor, SIGNAL( activated() ), this, SLOT( pickAxesColor() ) );
		connect( backgroundcolor, SIGNAL( activated() ), this, SLOT( pickBgColor() ) );
		connect( floorstyle, SIGNAL( selected( QAction* ) ), this, SLOT( pickFloorStyle( QAction* ) ) );
		connect( meshcolor, SIGNAL( activated() ), this, SLOT( pickMeshColor() ) );
		connect( numbercolor, SIGNAL( activated() ), this, SLOT( pickNumberColor() ) );
		connect( labelcolor, SIGNAL( activated() ), this, SLOT( pickLabelColor() ) );
		connect( titlecolor, SIGNAL( activated() ), this, SLOT( pickTitleColor() ) );
		connect( resetcolor, SIGNAL( activated() ), this, SLOT( resetColors() ) );
 		connect( numberfont, SIGNAL( activated() ), this, SLOT( pickNumberFont() ) );
		connect( labelfont, SIGNAL( activated() ), this, SLOT( pickLabelFont() ) );
		connect( titlefont, SIGNAL( activated() ), this, SLOT( pickTitleFont() ) );
		connect( resetfont, SIGNAL( activated() ), this, SLOT( resetFonts() ) );
		connect( animation, SIGNAL( toggled(bool) ) , this, SLOT( toggleAnimation(bool) ) );
    connect( dump, SIGNAL( activated() ) , this, SLOT( dumpImage() ) );
		connect( openFile, SIGNAL( activated() ) , this, SLOT( open() ) );

	  timer = new QTimer( this );
		connect( timer, SIGNAL(timeout()), this, SLOT(rotate()) );

		resSlider->setRange(1,70);
		connect( resSlider, SIGNAL(valueChanged(int)), this, SLOT(setResolution(int)) );
		connect( dataWidget, SIGNAL(resolutionChanged(double)), resSlider, SLOT(setValue(double)) );
		resSlider->setValue(1);             
		
		offsSlider->setRange(0,10);
		connect( offsSlider, SIGNAL(valueChanged(int)), this, SLOT(setPolygonOffset(int)) );
		offsSlider->setValue(5);             

		connect(normButton, SIGNAL(clicked()), this, SLOT(setStandardView()));  
		
		QLabel* info = new QLabel("QwtPlot3D <by krischnamurti 2003>", statusBar());       
		info->setPaletteForegroundColor(Qt::darkBlue);
		statusBar()->addWidget(info, 0, false);
		filenameWidget = new QLabel("                                  ", statusBar());
		statusBar()->addWidget(filenameWidget,0, false);
		rotateLabel = new QLabel("", statusBar());
		statusBar()->addWidget(rotateLabel,0, false);
		shiftLabel = new QLabel("", statusBar());
		statusBar()->addWidget(shiftLabel,0, false);
		scaleLabel = new QLabel("", statusBar());
		statusBar()->addWidget(scaleLabel,0, false);
		zoomLabel = new QLabel("", statusBar());
		statusBar()->addWidget(zoomLabel,0, false);
		
		connect(dataWidget, SIGNAL(rotationChanged(double,double,double)),this,SLOT(showRotate(double,double,double)));
		connect(dataWidget, SIGNAL(vieportShiftChanged(double,double)),this,SLOT(showShift(double,double)));
		connect(dataWidget, SIGNAL(scaleChanged(double,double,double)),this,SLOT(showScale(double,double,double)));
		connect(dataWidget, SIGNAL(zoomChanged(double)),this,SLOT(showZoom(double)));

		connect(functionCB, SIGNAL(activated(const QString&)), this, SLOT(createFunction(const QString&)));
		connect(projection, SIGNAL( toggled(bool) ), this, SLOT( toggleProjectionMode(bool)));
		connect(colorlegend, SIGNAL( toggled(bool) ), this, SLOT( toggleColorLegend(bool)));
		connect(autoscale, SIGNAL( toggled(bool) ), this, SLOT( toggleAutoScale(bool)));
				
		setStandardView();

		dataWidget->coordinates()->setLineSmooth(true);

		// dataWidget->setCaptionPosition(0.7, 0.2);
		
		dataWidget->setMouseTracking(true);
}

void Mesh2MainWindow::open()
{
    QString s( QFileDialog::getOpenFileName( "../../data", "GridData Files (*.mes *.MES)", this ) );
 
		if ( s.isEmpty() || !dataWidget)
        return;

		filenameWidget->setText(QString("  ") + s + QString("  "));
		QFileInfo fi( s );
    QString ext = fi.extension( false );   // ext = "gz"
  
		NativeReader r(dataWidget,s);
		if ((ext == "MES") || (ext == "mes")) 
		{
			if (r.read())
			{
				dataWidget->setResolution(3);
			}
		}
 		
		createColorLegend(StandardColor(dataWidget->data()).colVector());

		for (unsigned i=0; i!=dataWidget->coordinates()->axes.size(); ++i)
		{
			dataWidget->coordinates()->axes[i].setMajors(4);
			dataWidget->coordinates()->axes[i].setMinors(5);
			dataWidget->coordinates()->axes[i].setLabelString(QString(""));
		}
		
		//setStandardView();
		pickCoordSystem(activeCoordSystem);
		dataWidget->showColorLegend(legend_);
}

void Mesh2MainWindow::pickCoordSystem( QAction* action)
{
	if (!action || !dataWidget)
		return;

	activeCoordSystem = action;
	
	dataWidget->setTitle("QwtPlot3D");

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
	}
	else if (action == None)
	{
		dataWidget->setTitle("");
		dataWidget->setCoordinateStyle(NOCOORD);
	}
	dataWidget->updateCoordinates();
}

void Mesh2MainWindow::pickPlotStyle( QAction* action )
{
	if (!action || !dataWidget)
		return;

	if (action == polygon)
	{
		dataWidget->setPlotStyle(FILLED);
	}
	else if (action == filledmesh)
	{
		dataWidget->setPlotStyle(FILLEDMESH);
	}
	else if (action == wireframe)
	{
		dataWidget->setPlotStyle(WIREFRAME);
	}
	else if (action == hiddenline)
	{
		dataWidget->setPlotStyle(HIDDENLINE);
	}
	else
	{
		dataWidget->setPlotStyle(NOPLOT);
	}
}

void
Mesh2MainWindow::pickFloorStyle( QAction* action )
{
	if (!action || !dataWidget)
		return;

	if (action == floordata)
	{
		dataWidget->setFloorStyle(FLOORDATA);
	}
	else if (action == flooriso)
	{
		dataWidget->setFloorStyle(FLOORISO);
	}
	else if (action == floormesh)
	{
		dataWidget->setFloorStyle(FLOORMESH);
	}
	else
	{
		dataWidget->setFloorStyle(NOFLOOR);
	}
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

	dataWidget->coordinates()->setAxesColor(axc);
	dataWidget->setBackgroundColor(bgc);
	dataWidget->setMeshColor(msc);
	dataWidget->updateData();
	dataWidget->coordinates()->setNumberColor(nuc);
	dataWidget->coordinates()->setLabelColor(lbc);
	dataWidget->updateCoordinates();
}


void Mesh2MainWindow::pickAxesColor()
{
  
	QColor c = QColorDialog::getColor( white, this );
  if ( !c.isValid() )
		return;
	RGBA rgb = Qt2GL(c);
	dataWidget->coordinates()->setAxesColor(rgb);
	dataWidget->updateCoordinates();
}

void Mesh2MainWindow::pickBgColor()
{
  
	QColor c = QColorDialog::getColor( white, this );
  if ( !c.isValid() )
		return;
	RGBA rgb = Qt2GL(c);
	dataWidget->setBackgroundColor(rgb);
	dataWidget->updateCoordinates();
}

void Mesh2MainWindow::pickMeshColor()
{
  
	QColor c = QColorDialog::getColor( white, this );
  if ( !c.isValid() )
		return;
	RGBA rgb = Qt2GL(c);
	dataWidget->setMeshColor(rgb);
	dataWidget->updateData();
	dataWidget->updateCoordinates();
}

void Mesh2MainWindow::pickNumberColor()
{
  
	QColor c = QColorDialog::getColor( white, this );
  if ( !c.isValid() )
		return;
	RGBA rgb = Qt2GL(c);
	dataWidget->coordinates()->setNumberColor(rgb);
	dataWidget->updateCoordinates();
}

void Mesh2MainWindow::pickLabelColor()
{
 	QColor c = QColorDialog::getColor( white, this );
  if ( !c.isValid() )
		return;
	RGBA rgb = Qt2GL(c);
	dataWidget->coordinates()->setLabelColor(rgb);
	dataWidget->updateCoordinates();
}
void Mesh2MainWindow::pickTitleColor()
{
 	QColor c = QColorDialog::getColor( white, this );
  if ( !c.isValid() )
		return;
	RGBA rgb = Qt2GL(c);
	dataWidget->setCaptionColor(rgb);
	dataWidget->updateGL();
}

void Mesh2MainWindow::pickNumberFont()
{
	bool ok;
	QFont font = QFontDialog::getFont(&ok, this );
	if ( !ok ) 
	{
		return;
	} 
	dataWidget->coordinates()->setNumberFont(font);
	dataWidget->updateCoordinates();
}
void Mesh2MainWindow::pickLabelFont()
{
	bool ok;
	QFont font = QFontDialog::getFont(&ok, this );
	if ( !ok ) 
	{
		return;
	} 
	dataWidget->coordinates()->setLabelFont(font);
	dataWidget->updateCoordinates();
}
void Mesh2MainWindow::pickTitleFont()
{
	bool ok;
	QFont font = QFontDialog::getFont(&ok, this );
	if ( !ok ) 
	{
		return;
	} 
	dataWidget->setCaptionFont(font.family(), font.pointSize(), font.weight(), font.italic());
}

void Mesh2MainWindow::resetFonts()
{
	dataWidget->coordinates()->setNumberFont(QFont("Courier", 12));
	dataWidget->coordinates()->setLabelFont(QFont("Courier", 14, QFont::Bold));
	dataWidget->updateCoordinates();
}

void Mesh2MainWindow::setStandardView()
{
	dataWidget->setRotation(30,0,15);
	dataWidget->setShift(0.1,0,0);
	dataWidget->setScale(1,1,1);
	dataWidget->setZoom(1);
}

void Mesh2MainWindow::dumpImage()
{
	static int counter = 0;
	if (!dataWidget)
		return;

	QString name = QString("dump_") + QString::number(counter++) + ".png";
	dataWidget->dump(name,"PNG");
}

/*!
  Turns animation on or off
*/

void Mesh2MainWindow::toggleAnimation(bool val)
{
	if ( val )
	{
		timer->start( redrawWait, false ); // Wait this many msecs before redraw
	}
	else
	{
		timer->stop();
	}
}

void Mesh2MainWindow::rotate()
{
	if (dataWidget)
	{
		dataWidget->setRotation(
			int(dataWidget->xRotation() + 1) % 360,
			int(dataWidget->yRotation() + 1) % 360,
			int(dataWidget->zRotation() + 1) % 360
			);
	}	
}

void Mesh2MainWindow::createFunction(QString const& name)
{
	if (name == QString("Rosenbrock")) 
	{
		Rosenbrock rosenbrock(dataWidget);
		
		rosenbrock.setMesh(70,71);
		rosenbrock.setDomain(-1.73,1.5,-1.5,1.5);
		//rosenbrock.setDomain(-5,5.5,-1,2);
		rosenbrock.setMinZ(-100);
		
		rosenbrock.create();
	}
	else if (name == QString("Hat")) 
	{
		Hat hat(dataWidget);
		
		hat.setMesh(51,72);
		hat.setDomain(-1.5,1.5,-1.5,1.5);
		
		//hat.setMaxZ(1.1);

		hat.create();		
	}
	else if (name == QString("Gauss")) 
	{
		Gauss gauss(dataWidget);
		
		gauss.setMesh(71,71);
		gauss.setDomain(-2.5,2.5,-2.5,2.5);
		
		//hat.setMaxZ(1.1);

		gauss.create();
	}

	createColorLegend(StandardColor(dataWidget->data()).colVector());

	for (unsigned i=0; i!=dataWidget->coordinates()->axes.size(); ++i)
	{
		dataWidget->coordinates()->axes[i].setMajors(7);
		dataWidget->coordinates()->axes[i].setMinors(5);
	}
	
	pickCoordSystem(activeCoordSystem);

	dataWidget->coordinates()->axes[X1].setLabelString(QChar (0x3b4) + QString("-axis"));
	dataWidget->coordinates()->axes[X2].setLabelString(QChar (0x3b4) + QString("-axis"));
	dataWidget->coordinates()->axes[X3].setLabelString(QChar (0x3b4) + QString("-axis"));
	dataWidget->coordinates()->axes[X4].setLabelString(QChar (0x3b4) + QString("-axis"));

	dataWidget->coordinates()->axes[Y1].setLabelString(QChar (0x3b6) + QString("-axis"));
	dataWidget->coordinates()->axes[Y2].setLabelString(QChar (0x3b6) + QString("-axis"));
	dataWidget->coordinates()->axes[Y3].setLabelString(QChar (0x3b6) + QString("-axis"));
	dataWidget->coordinates()->axes[Y4].setLabelString(QChar (0x3b6) + QString("-axis"));

	dataWidget->coordinates()->axes[Z1].setLabelString(QChar (0x3b8) + QString("-axis"));
	dataWidget->coordinates()->axes[Z2].setLabelString(QChar (0x3b8) + QString("-axis"));
	dataWidget->coordinates()->axes[Z3].setLabelString(QChar (0x3b8) + QString("-axis"));
	dataWidget->coordinates()->axes[Z4].setLabelString(QChar (0x3b8) + QString("-axis"));

	//setStandardView();
	dataWidget->showColorLegend(legend_);
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
	dataWidget->updateCoordinates();
}

void
Mesh2MainWindow::setResolution(int val)
{
	dataWidget->setResolution(val);
}

void
Mesh2MainWindow::setPolygonOffset(int val)
{
	dataWidget->setPolygonOffset(val / 10.0);
	dataWidget->updateData();
	dataWidget->updateGL();
}

void
Mesh2MainWindow::createColorLegend(ColorVector const& col)
{
	Triple a = dataWidget->hullFirst();
	Triple c = dataWidget->hullSecond();

	Triple b, d;

	double diff = c.x - a.x;

	a = Triple(a.x - diff / 5, c.y, a.z);
	b = Triple(a.x + diff / 7, c.y, a.z);
	c = Triple(a.x + diff / 7, c.y, c.z);
	d = Triple(a.x, a.y, c.z);
	
	dataWidget->createColorLegend(col, a, b, c, d);
}

void
Mesh2MainWindow::showRotate(double x, double y, double z)		
{
	rotateLabel->setText(" Angles ("  + QString::number(x) + " ," 
																	+ QString::number(y) + " ,"
																	+ QString::number(z) + ")");
}
void
Mesh2MainWindow::showShift(double x, double y)		
{
	shiftLabel->setText(" Shifts (" + QString::number(x) + " ," 
																	+ QString::number(y) + " )"
																	);
}
void
Mesh2MainWindow::showScale(double x, double y, double z)		
{
	scaleLabel->setText(" Scales (" + QString::number(x) + " ," 
																	+ QString::number(y) + " ,"
																	+ QString::number(z) + ")");
}
void
Mesh2MainWindow::showZoom(double z)		
{
	zoomLabel->setText(" Zoom "  + QString::number(z)); 
}

