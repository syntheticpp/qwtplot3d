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
#include "colormapreader.h"
#include "../../../include/qwt3d_io.h"

using namespace Qwt3D;
using namespace std;

Mesh2MainWindow::~Mesh2MainWindow()      
{
	delete dataWidget;
}

Mesh2MainWindow::Mesh2MainWindow( QWidget* parent, const char* name, WFlags f )       
	: Mesh2MainWindowBase( parent, name, f )
{
		col_ = 0;
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
		connect( datacolor, SIGNAL( activated() ), this, SLOT( pickDataColor() ) );
		connect( resetcolor, SIGNAL( activated() ), this, SLOT( resetColors() ) );
 		connect( numberfont, SIGNAL( activated() ), this, SLOT( pickNumberFont() ) );
		connect( labelfont, SIGNAL( activated() ), this, SLOT( pickLabelFont() ) );
		connect( titlefont, SIGNAL( activated() ), this, SLOT( pickTitleFont() ) );
		connect( resetfont, SIGNAL( activated() ), this, SLOT( resetFonts() ) );
		connect( animation, SIGNAL( toggled(bool) ) , this, SLOT( toggleAnimation(bool) ) );
    connect( dump, SIGNAL( activated() ) , this, SLOT( dumpImage() ) );
		connect( openFile, SIGNAL( activated() ) , this, SLOT( open() ) );
		connect( openMeshFile, SIGNAL( activated() ) , this, SLOT( openMesh() ) );

	  timer = new QTimer( this );
		connect( timer, SIGNAL(timeout()), this, SLOT(rotate()) );

		resSlider->setRange(1,70);
		connect( resSlider, SIGNAL(valueChanged(int)), dataWidget, SLOT(setResolution(int)) );
		connect( dataWidget, SIGNAL(resolutionChanged(int)), resSlider, SLOT(setValue(int)) );
		resSlider->setValue(1);             
		
		offsSlider->setRange(0,10);
		connect( offsSlider, SIGNAL(valueChanged(int)), this, SLOT(setPolygonOffset(int)) );
		offsSlider->setValue(5);             

		connect(normButton, SIGNAL(clicked()), this, SLOT(setStandardView()));  
		
		QLabel* info = new QLabel("qwtplot3d <by krischnamurti 2003>", statusBar());       
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
		connect(shader, SIGNAL( toggled(bool) ), this, SLOT( toggleShader(bool)));
		connect(mouseinput, SIGNAL( toggled(bool) ), dataWidget, SLOT( enableMouse(bool)));
		connect(normals, SIGNAL( toggled(bool) ), this, SLOT( showNormals(bool)));
		connect(normalsquality,  SIGNAL(valueChanged(int)), this, SLOT(setNormalQuality(int)) );
		connect(normalslength,  SIGNAL(valueChanged(int)), this, SLOT(setNormalLength(int)) );
				
		setStandardView();

		dataWidget->coordinates()->setLineSmooth(true);
		dataWidget->enableMouse(true);

		colormappv_ = new ColorMapPreview;
		datacolordlg_ = new QFileDialog( this );
		
		QDir dir("../../data/colormaps");
		if (dir.exists("../../data/colormaps"))
			datacolordlg_->setDir("../../data/colormaps");
		datacolordlg_->setFilter("Colormap files (*.map;*.MAP)");
		datacolordlg_->setContentsPreviewEnabled( TRUE );
		datacolordlg_->setContentsPreview( colormappv_, colormappv_ );
		datacolordlg_->setPreviewMode( QFileDialog::Contents );
  
		connect(datacolordlg_, SIGNAL(fileHighlighted(const QString&)), this, SLOT(adaptDataColors(const QString&)));
		connect(filetypeCB, SIGNAL(activated(const QString&)), this, SLOT(setFileType(const QString&)));

		filetype_ = "png";
		filetypeCB->setCurrentText("png");
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
 		
		for (unsigned i=0; i!=dataWidget->coordinates()->axes.size(); ++i)
		{
			dataWidget->coordinates()->axes[i].setMajors(4);
			dataWidget->coordinates()->axes[i].setMinors(5);
			dataWidget->coordinates()->axes[i].setLabelString(QString(""));
		}
		
		updateColorLegend(4,5);
		pickCoordSystem(activeCoordSystem);
		dataWidget->showColorLegend(legend_);
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
	else if (name == QString("Saddle")) 
	{
		Saddle saddle;
		
		saddle.setMesh(71,71);
		double dom = 2.5;
		saddle.setDomain(-dom, dom, -dom, dom);
		
		//hat.setMaxZ(1.1);

		saddle.assign(dataWidget);
		saddle.create();
	}
	else if (name == QString("Sombrero")) 
	{
		Mex mex;
		
		mex.setMesh(91,91);
		double dom = 15;
		mex.setDomain(-dom, dom, -dom, dom);
		
		//hat.setMaxZ(1.1);

		mex.create(dataWidget);
	}

	for (unsigned i=0; i!=dataWidget->coordinates()->axes.size(); ++i)
	{
		dataWidget->coordinates()->axes[i].setMajors(7);
		dataWidget->coordinates()->axes[i].setMinors(5);
	}

	updateColorLegend(7,5);


	pickCoordSystem(activeCoordSystem);

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
		dataWidget->setTitle("QwtPlot3D");
		dataWidget->setCoordinateStyle(NOCOORD);
	}
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
	dataWidget->updateData();
	dataWidget->updateGL();
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
	
	dataWidget->updateData();
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

	dataWidget->coordinates()->setAxesColor(axc);
	dataWidget->setBackgroundColor(bgc);
	dataWidget->setMeshColor(msc);
	dataWidget->updateData();
	dataWidget->coordinates()->setNumberColor(nuc);
	dataWidget->coordinates()->setLabelColor(lbc);

	col_ = new StandardColor(dataWidget);
	dataWidget->setDataColor(col_);
	dataWidget->updateData();	
	dataWidget->updateNormals();
	dataWidget->updateGL();
}


void Mesh2MainWindow::pickAxesColor()
{
  
	QColor c = QColorDialog::getColor( white, this );
  if ( !c.isValid() )
		return;
	RGBA rgb = Qt2GL(c);
	dataWidget->coordinates()->setAxesColor(rgb);
	dataWidget->updateGL();
}

void Mesh2MainWindow::pickBgColor()
{
  
	QColor c = QColorDialog::getColor( white, this );
  if ( !c.isValid() )
		return;
	RGBA rgb = Qt2GL(c);
	dataWidget->setBackgroundColor(rgb);
	dataWidget->updateGL();
}

void Mesh2MainWindow::pickMeshColor()
{
  
	QColor c = QColorDialog::getColor( white, this );
  if ( !c.isValid() )
		return;
	RGBA rgb = Qt2GL(c);
	dataWidget->setMeshColor(rgb);
	dataWidget->updateData();
	dataWidget->updateGL();
}

void Mesh2MainWindow::pickNumberColor()
{
  
	QColor c = QColorDialog::getColor( white, this );
  if ( !c.isValid() )
		return;
	RGBA rgb = Qt2GL(c);
	dataWidget->coordinates()->setNumberColor(rgb);
	dataWidget->updateGL();
}

void Mesh2MainWindow::pickLabelColor()
{
 	QColor c = QColorDialog::getColor( white, this );
  if ( !c.isValid() )
		return;
	RGBA rgb = Qt2GL(c);
	dataWidget->coordinates()->setLabelColor(rgb);
	dataWidget->updateGL();
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

void Mesh2MainWindow::pickDataColor()
{
	datacolordlg_->show();
}

void Mesh2MainWindow::adaptDataColors(const QString& fileName)
{
	ColorVector cv;
	
	if (!openColorMap(cv, fileName))
		return;
	
	col_ = new StandardColor(dataWidget);
	col_->setColorVector(cv);
	
	dataWidget->setDataColor(col_);
	dataWidget->updateData();
	dataWidget->updateNormals();
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
	dataWidget->updateGL();
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
	dataWidget->updateGL();
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
		
	if (filetype_ == QString("png"))
	{
		name = QString("dump_") + QString::number(counter++) + ".png";
		dataWidget->savePixmap(name,"PNG");
	}
	else if (filetype_ == QString("bmp"))
	{
		name = QString("dump_") + QString::number(counter++) + ".bmp";
		dataWidget->savePixmap(name,"BMP");
	}
	else if (filetype_ == QString("xpm"))
	{
		name = QString("dump_") + QString::number(counter++) + ".xpm";
		dataWidget->savePixmap(name,"XPM");
	}
	else if (filetype_ == QString("ppm"))
	{
		name = QString("dump_") + QString::number(counter++) + ".ppm";
		dataWidget->savePixmap(name,"PPM");
	}
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

void
Mesh2MainWindow::toggleProjectionMode(bool val)
{
	dataWidget->setOrtho(val);
}

void
Mesh2MainWindow::toggleColorLegend(bool val)
{
	legend_ = val;
	dataWidget->updateColorLegend();
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
}

void
Mesh2MainWindow::setPolygonOffset(int val)
{
	dataWidget->setPolygonOffset(val / 10.0);
	dataWidget->updateGL();
}

void
Mesh2MainWindow::createColorLegend(ColorVector const& col)
{
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
    QString data(QFileDialog::getOpenFileName( "../../data", "nodes (*.nod)", this ) );
    QString edges( QFileDialog::getOpenFileName( "../../data", "connectivities (*.cel)", this ) );
 
		if ( data.isEmpty() || edges.isEmpty() || !dataWidget)
        return;


		TripleField vdata;
		CellField vpoly;
		
		readNodes(vdata, data, NodeFilter());
		readConnections(vpoly, edges, CellFilter());
		
		dataWidget->createDataRepresentation(vdata, vpoly);

 		for (unsigned i=0; i!=dataWidget->coordinates()->axes.size(); ++i)
		{
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
	dataWidget->showNormals(val);
	dataWidget->updateNormals();
	dataWidget->updateGL();
}

void
Mesh2MainWindow::setNormalLength(int val)
{
	dataWidget->setNormalLength(val / 400.);
	dataWidget->updateNormals();
	dataWidget->updateGL();
}

void
Mesh2MainWindow::setNormalQuality(int val)
{
	dataWidget->setNormalQuality(val);
	dataWidget->updateNormals();
	dataWidget->updateGL();
}

bool
Mesh2MainWindow::openColorMap(ColorVector& cv, QString fname)
{	
	ifstream file(fname.latin1());

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
	dataWidget->legend()->setMajors(majors);
	dataWidget->legend()->setMinors(minors);
	double start, stop;
	dataWidget->coordinates()->axes[Z1].limits(start,stop);
	dataWidget->legend()->setLimits(start, stop);
}		

void Mesh2MainWindow::setFileType(QString const& name)
{
	filetype_ = name;	
}