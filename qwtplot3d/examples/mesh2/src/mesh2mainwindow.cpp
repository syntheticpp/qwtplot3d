#include <qframe.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qwt_knob.h>
#include <qwt_wheel.h>
#include <qwt_slider.h>
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
#include "alphadlgimpl.h"

#include "functions.h"

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
		connect( resetcolor, SIGNAL( activated() ), this, SLOT( resetColors() ) );
 		connect( numberfont, SIGNAL( activated() ), this, SLOT( pickNumberFont() ) );
		connect( labelfont, SIGNAL( activated() ), this, SLOT( pickLabelFont() ) );
		connect( resetfont, SIGNAL( activated() ), this, SLOT( resetFonts() ) );
		connect( animation, SIGNAL( toggled(bool) ) , this, SLOT( toggleAnimation(bool) ) );
    connect( dump, SIGNAL( activated() ) , this, SLOT( dumpImage() ) );
		connect( openFile, SIGNAL( activated() ) , this, SLOT( open() ) );

	  timer = new QTimer( this );
		connect( timer, SIGNAL(timeout()), this, SLOT(rotate()) );

		// Create the three sliders; one for each rotation axis
    xR->setRange(0,360,1);
    connect( xR, SIGNAL(valueChanged(double)), this, SLOT(xRotate(double)) );
    
		yR->setRange(0,360,1);
    connect( yR, SIGNAL(valueChanged(double)), this, SLOT(yRotate(double)) );
    
		zR->setRange(0,360,1);
    connect( zR, SIGNAL(valueChanged(double)), this, SLOT(zRotate(double)) );

    xS->setRange(-1.5,1.5,0.05);
    connect( xS, SIGNAL(valueChanged(double)), this, SLOT(xShift(double)) );

    yS->setRange(-1.5,1.5,0.05);
    connect( yS, SIGNAL(valueChanged(double)), this, SLOT(yShift(double)) );
    
		zS->setRange(-1.5,1.5,0.05);
    connect( zS, SIGNAL(valueChanged(double)), this, SLOT(zShift(double)) );
    
		xSc->setRange(0.0,100,0.02);
    connect( xSc, SIGNAL(valueChanged(double)), this, SLOT(xScale(double)) );

    ySc->setRange(0.0,100,0.02);
    connect( ySc, SIGNAL(valueChanged(double)), this, SLOT(yScale(double)) );
    
		zSc->setRange(0.0,100,0.02);
    connect( zSc, SIGNAL(valueChanged(double)), this, SLOT(zScale(double)) );
 
		zoomWheel->setRange(0,100,0.01,10);
		connect( zoomWheel, SIGNAL(valueChanged(double)), dataWidget, SLOT(setZoom(double)) );

		resSlider->setRange(1,70);
		resSlider->setStep(1);
		connect( resSlider, SIGNAL(valueChanged(double)), this, SLOT(setResolution(double)) );
		connect( dataWidget, SIGNAL(resolutionChanged(double)), resSlider, SLOT(setValue(double)) );
		resSlider->setValue(1);             
		
		offsetSlider->setRange(0,1);
		offsetSlider->setStep(0.1);
		connect( offsetSlider, SIGNAL(valueChanged(double)), this, SLOT(setPolygonOffset(double)) );
		offsetSlider->setValue(0.5);             

		connect(normButton, SIGNAL(clicked()), this, SLOT(setStandardView()));  
		
		alphaLabel->setText(QChar (0x3b1));
		betaLabel->setText(QChar (0x3b2));
		gammaLabel->setText(QChar (0x3b3));

		QLabel* info = new QLabel("QwtPlot3d <by krischnamurti 2003>", statusBar());       
		info->setPaletteForegroundColor(Qt::darkBlue);
		statusBar()->addWidget(info, 0, false);
		filenameWidget = new QLabel("                                  ", statusBar());
		statusBar()->addWidget(filenameWidget,0, false);
		positionWidget = new QLabel("                         ", statusBar());
		statusBar()->addWidget(positionWidget,0, false);
		
		
		connect(dataWidget, SIGNAL(screenpositionChanged(double,double)), this, SLOT(showPosition(double,double))); 

		connect(functionCB, SIGNAL(activated(const QString&)), this, SLOT(createFunction(const QString&)));
		connect(projection, SIGNAL( toggled(bool) ), this, SLOT( toggleProjectionMode(bool)));
		connect(colorlegend, SIGNAL( toggled(bool) ), this, SLOT( toggleColorLegend(bool)));
		connect(autoscale, SIGNAL( toggled(bool) ), this, SLOT( toggleAutoScale(bool)));


		
		alphaDlg->polygonsSld->setRange(0,1);
		alphaDlg->polygonsSld->setStep(0.1);
		connect( alphaDlg->polygonsSld, SIGNAL(valueChanged(double)), this, SLOT(setPolygonsAlpha(double)) );
		alphaDlg->polygonsSld->setValue(1);             
					
		alphaDlg->meshSld->setRange(0,1);
		alphaDlg->meshSld->setStep(0.1);
		connect( alphaDlg->meshSld, SIGNAL(valueChanged(double)), this, SLOT(setMeshAlpha(double)) );
		alphaDlg->meshSld->setValue(1);             
				
		setStandardView();
		
		// dataWidget->setMouseTracking(true);
}

void Mesh2MainWindow::open()
{
    QString s( QFileDialog::getOpenFileName( "../../data", "Data Files (*.mes *.XYZ)", this ) );
 
		if ( s.isEmpty() || !dataWidget)
        return;

		filenameWidget->setText(QString("  ") + s + QString("  "));
		QFileInfo fi( s );
    QString ext = fi.extension( false );   // ext = "gz"
  
		NativeReader r(s);
		Data data;
		if ((ext == "XYZ") || (ext == "xyz"))
		{
			r.setType(NativeReader::XYZFILE);
			if (r.createData(data))
			{
				dataWidget->assignData(data);
				dataWidget->setResolution(5);
			}
		}
		else if ((ext == "MES") || (ext == "mes")) 
		{
			r.setType(NativeReader::MESHFILE);
			if (r.createData(data))
			{
				dataWidget->assignData(data);
				dataWidget->setResolution(1);
			}
		}
 		
		createColorLegend(StandardColor(data).colVector());

		for (unsigned i=0; i!=dataWidget->coord.axes.size(); ++i)
		{
			dataWidget->coord.axes[i].setMajors(4);
			dataWidget->coord.axes[i].setMinors(5);
		}
		
		setStandardView();
		pickCoordSystem(activeCoordSystem);
		dataWidget->showColorLegend(legend_);
}

void Mesh2MainWindow::pickCoordSystem( QAction* action)
{
	if (!action || !dataWidget)
		return;

	activeCoordSystem = action;
	
	if (!dataWidget->hasData())
	{
		double l = 0.6;
		dataWidget->createCoordinateSystem(Triple(-l,-l,-l), Triple(l,l,l));
		for (unsigned i=0; i!=dataWidget->coord.axes.size(); ++i)
		{
			dataWidget->coord.axes[i].setMajors(4);
			dataWidget->coord.axes[i].setMinors(5);
		}
	}			

	if (action == Box || action == Frame)
	{
		Triple first = dataWidget->coord.first();
		Triple second = dataWidget->coord.second();

		dataWidget->coord.axes[X1].setLimits(first.x, second.x);
		dataWidget->coord.axes[Y1].setLimits(first.y, second.y);
		dataWidget->coord.axes[Z1].setLimits(first.z, second.z);

		dataWidget->coord.axes[X1].setScale(true);
		dataWidget->coord.axes[Y1].setScale(true);
		dataWidget->coord.axes[Z1].setScale(true);

		dataWidget->coord.axes[X1].setNumbers(true);
		dataWidget->coord.axes[Y1].setNumbers(true);
		dataWidget->coord.axes[Z1].setNumbers(true);
		
		dataWidget->coord.axes[X1].setNumberAnchor(LabelPixmap::TopCenter);
		dataWidget->coord.axes[Y1].setNumberAnchor(LabelPixmap::CenterRight);
		dataWidget->coord.axes[Z1].setNumberAnchor(LabelPixmap::CenterRight);

		if (action == Box)
			dataWidget->setCoordinateStyle(BOX);
		if (action == Frame)
			dataWidget->setCoordinateStyle(FRAME);
	}
	else if (action == None)
	{
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
		dataWidget->setPlotStyle(NOPLOTDATA);
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

	dataWidget->coord.setAxesColor(axc);
	dataWidget->setBackgroundColor(bgc);
	dataWidget->setMeshColor(msc);
	dataWidget->updateData();
	dataWidget->coord.setNumberColor(nuc);
	dataWidget->coord.setLabelColor(lbc);
	dataWidget->updateCoordinates();
}


void Mesh2MainWindow::pickAxesColor()
{
  
	QColor c = QColorDialog::getColor( white, this );
  if ( !c.isValid() )
		return;
	RGBA rgb = Qt2GL(c);
	dataWidget->coord.setAxesColor(rgb);
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
	dataWidget->coord.setNumberColor(rgb);
	dataWidget->updateCoordinates();
}

void Mesh2MainWindow::pickLabelColor()
{
 	QColor c = QColorDialog::getColor( white, this );
  if ( !c.isValid() )
		return;
	RGBA rgb = Qt2GL(c);
	dataWidget->coord.setLabelColor(rgb);
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
	dataWidget->coord.setLabelFont(font);
	dataWidget->updateCoordinates();
}

void Mesh2MainWindow::pickNumberFont()
{
	bool ok;
	QFont font = QFontDialog::getFont(&ok, this );
	if ( !ok ) 
	{
		return;
	} 
	dataWidget->coord.setNumberFont(font);
	dataWidget->updateCoordinates();
}

void Mesh2MainWindow::resetFonts()
{
	dataWidget->coord.setNumberFont(QFont("Helvetica", 10));
	dataWidget->coord.setLabelFont(QFont("Helvetica", 14, QFont::Bold));
	dataWidget->updateCoordinates();
}

void Mesh2MainWindow::setStandardView()
{
	zoomWheel->setValue(1);
	xRotate(30); xR->setValue(30); 
	yRotate(0); yR->setValue(0); 
	zRotate(15); zR->setValue(15);
	xShift(0.1); xS->setValue(0.1);
	yShift(0); yS->setValue(0);
	zShift(0); zS->setValue(0);
	xScale(1); xSc->setValue(1);
	yScale(1); ySc->setValue(1);
	zScale(1); zSc->setValue(1);
}

void Mesh2MainWindow::showPosition(double x, double y)
{
	QString a = QString::number(x) + "," + QString::number(y);
	positionWidget->setText(QString("  Pos: ") + a + QString("  "));
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
		disconnect(xR);
		disconnect(yR);
		disconnect(zR);
		timer->start( redrawWait, false ); // Wait this many msecs before redraw
	}
	else
	{
		timer->stop();
		connect(xR, SIGNAL(valueChanged(double)), this, SLOT(xRotate(double)));
		connect(yR, SIGNAL(valueChanged(double)), this, SLOT(yRotate(double)));
		connect(zR, SIGNAL(valueChanged(double)), this, SLOT(zRotate(double)));
		xR->setValue(dataWidget->xRotation());
		yR->setValue(dataWidget->yRotation());
		zR->setValue(dataWidget->zRotation());
	}
//	normButton->setEnabled(!val);
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

void Mesh2MainWindow::xRotate(double val)
{
	if (!dataWidget)
	{
		return;
	}
	dataWidget->setRotation(
			val,
			dataWidget->yRotation(),
			dataWidget->zRotation()
			);
}

void Mesh2MainWindow::yRotate(double val)
{
	if (!dataWidget)
	{
		return;
	}
	dataWidget->setRotation(
			dataWidget->xRotation(),
			val,
			dataWidget->zRotation()
			);
}

void Mesh2MainWindow::zRotate(double val)
{
	if (!dataWidget)
	{
		return;
	}
	dataWidget->setRotation(
			dataWidget->xRotation(),
			dataWidget->yRotation(),
			val			
			);
}

void Mesh2MainWindow::xShift(double val)
{
	
	if (!dataWidget)
	{
		return;
	}
	
	dataWidget->calculateHull();
	Triple a = dataWidget->hullFirst();
	Triple b = dataWidget->hullSecond();
	double dist = fabs((b-a).x);
	val*=dist;

	dataWidget->setShift(
			val,
			dataWidget->yShift(),
			dataWidget->zShift()			
			);
}

void Mesh2MainWindow::yShift(double val)
{
	if (!dataWidget)
	{
		return;
	}

	dataWidget->calculateHull();
	Triple a = dataWidget->hullFirst();
	Triple b = dataWidget->hullSecond();
	double dist = fabs((b-a).y);
	val*=dist;

	dataWidget->setShift(
			dataWidget->xShift(),
			val,
			dataWidget->zShift()			
			);
}

void Mesh2MainWindow::zShift(double val)
{
	if (!dataWidget)
	{
		return;
	}

	dataWidget->calculateHull();
	Triple a = dataWidget->hullFirst();
	Triple b = dataWidget->hullSecond();
	double dist = fabs((b-a).z);
	val*=dist;

	dataWidget->setShift(
			dataWidget->xShift(),			
			dataWidget->yShift(),
			val
			);
}

void Mesh2MainWindow::xScale(double val)
{
	if (!dataWidget)
	{
		return;
	}
	dataWidget->setScale(
			val,
			dataWidget->yScale(),
			dataWidget->zScale()			
			);
}

void Mesh2MainWindow::yScale(double val)
{
	if (!dataWidget)
	{
		return;
	}
	dataWidget->setScale(
			dataWidget->xScale(),
			val,
			dataWidget->zScale()			
			);
}

void Mesh2MainWindow::zScale(double val)
{
	if (!dataWidget)
	{
		return;
	}
	dataWidget->setScale(
			dataWidget->xScale(),			
			dataWidget->yScale(),
			val
			);
}


void Mesh2MainWindow::createFunction(QString const& name)
{
	Data res;
	CreateFunction(res, name);
	
	dataWidget->assignData(res);
	createColorLegend(StandardColor(res).colVector());
	setStandardView();

	for (unsigned i=0; i!=dataWidget->coord.axes.size(); ++i)
	{
		dataWidget->coord.axes[i].setMajors(7);
		dataWidget->coord.axes[i].setMinors(5);
	}
	
	dataWidget->coord.axes[X1].setLabelString(QChar (0x3b4) + QString("-axis"));
	dataWidget->coord.axes[Y1].setLabelString(QChar (0x3b6) + QString("-axis"));
	dataWidget->coord.axes[Z1].setLabelString(QChar (0x3b8) + QString("-axis"));

	pickCoordSystem(activeCoordSystem);
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
	dataWidget->coord.setAutoScale(val);
	dataWidget->updateCoordinates();
}

void
Mesh2MainWindow::setResolution(double val)
{
	dataWidget->setResolution((int)val);
}

void
Mesh2MainWindow::setPolygonOffset(double val)
{
	dataWidget->setPolygonOffset(val);
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
Mesh2MainWindow::setPolygonsAlpha(double d)
{
	dataWidget->modifyStandardColorAlpha(d);
	dataWidget->updateData();
	dataWidget->updateGL();
}

void
Mesh2MainWindow::setMeshAlpha(double d)
{
	RGBA rgba = dataWidget->meshColor();
	rgba.a = d;
	dataWidget->setMeshColor(rgba);
	dataWidget->updateData();
	dataWidget->updateGL();
}
