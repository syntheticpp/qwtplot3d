#include <fstream>

#include <qmetaobject.h>
#include <qframe.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qaction.h>
#include <qmenubar.h>
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

#include <qwt3d_io.h>
#include <qwt3d_io_reader.h>

#include "mainwindow.h"

#include "functions.h"
//#include "colormapreader.h"
//#include "configframe.h"

using namespace Qwt3D;
using namespace std;


MainWindow::~MainWindow()      
{
	delete dataWidget;
}

MainWindow::MainWindow( QWidget* parent )       
	: QMainWindow( parent )
{
	setupUi(this);

 // QActionGroup* plotstyle = new QActionGroup(this);
 // plotstyle->addAction(pointstyle);
 // plotstyle->addAction(wireframe);
 // plotstyle->addAction(hiddenline);
 // plotstyle->addAction(polygon);
 // plotstyle->addAction(filledmesh);
 // plotstyle->addAction(nodata);

 // QActionGroup* floorstyle = new QActionGroup(this);
 // floorstyle->addAction(floordata);
 // floorstyle->addAction(flooriso);
 // floorstyle->addAction(floornone);

 // QActionGroup* color = new QActionGroup(this);
 // color->addAction(axescolor);
 // color->addAction(backgroundcolor);
 // color->addAction(meshcolor);
 // color->addAction(numbercolor);
 // color->addAction(labelcolor);
 // color->addAction(titlecolor);
 // color->addAction(datacolor);
 // color->addAction(resetcolor);


 // QObject::connect(Exit, SIGNAL(triggered()), SLOT(close()));

 QGridLayout *grid = new QGridLayout( frame );

 // col_ = 0;
	//legend_ = false;

 dataWidget = new GridPlot(frame);
 grid->addWidget( dataWidget, 0, 0 );


	//connect( coord, SIGNAL( triggered( QAction* ) ), this, SLOT( pickCoordSystem( QAction* ) ) );
	//connect( plotstyle, SIGNAL( triggered( QAction* ) ), this, SLOT( pickPlotStyle( QAction* ) ) );
	//connect( axescolor, SIGNAL( triggered() ), this, SLOT( pickAxesColor() ) );
	//connect( backgroundcolor, SIGNAL( triggered() ), this, SLOT( pickBgColor() ) );
	//connect( floorstyle, SIGNAL( triggered( QAction* ) ), this, SLOT( pickFloorStyle( QAction* ) ) );
	//connect( meshcolor, SIGNAL( triggered() ), this, SLOT( pickMeshColor() ) );
	//connect( numbercolor, SIGNAL( triggered() ), this, SLOT( pickNumberColor() ) );
	//connect( labelcolor, SIGNAL( triggered() ), this, SLOT( pickLabelColor() ) );
	//connect( titlecolor, SIGNAL( triggered() ), this, SLOT( pickTitleColor() ) );
	//connect( datacolor, SIGNAL( triggered() ), this, SLOT( pickDataColor() ) );
	//connect( resetcolor, SIGNAL( triggered() ), this, SLOT( resetColors() ) );
	//connect( openFile, SIGNAL( triggered() ) , this, SLOT( open() ) );
	//

 pmanager["hat"] = PlotletItem(cbHat, 0);
 pmanager["hatn"] = PlotletItem(cbHatn, -1);
 pmanager["boy"] = PlotletItem(cbBoy, -1);
 pmanager["ripple"] = PlotletItem(cbRipple, -1);

 PM_IT i = pmanager.begin();
 while (i != pmanager.end()) 
 {
   connect( i->cbox, SIGNAL( toggled( bool ) ), this, SLOT( togglePlotlet( bool ) ) );
   ++i;
 }

	//timer = new QTimer( this );
	//connect( timer, SIGNAL(timeout()), this, SLOT(rotate()) );

	//resSlider->setRange(1,70);
	//connect( resSlider, SIGNAL(valueChanged(int)), dataWidget, SLOT(setResolution(int)) );
	//connect( dataWidget, SIGNAL(resolutionChanged(int)), resSlider, SLOT(setValue(int)) );
	//resSlider->setValue(1);             
	//
	//connect( offsSlider, SIGNAL(valueChanged(int)), this, SLOT(setPolygonOffset(int)) );

	//connect(normButton, SIGNAL(clicked()), this, SLOT(setStandardView()));  
	//
 // QString qwtstr(" qwtplot3d ");
 // qwtstr += QString::number(QWT3D_MAJOR_VERSION) + ".";
 // qwtstr += QString::number(QWT3D_MINOR_VERSION) + ".";
 // qwtstr += QString::number(QWT3D_PATCH_VERSION) + " ";

	//QLabel* info = new QLabel(qwtstr, statusBar());       
 // statusBar()->addWidget(info, 0);
	//filenameWidget = new QLabel("                                  ", statusBar());
	//statusBar()->addWidget(filenameWidget,0);
	//dimWidget = new QLabel("", statusBar());
	//statusBar()->addWidget(dimWidget,0);
	//rotateLabel = new QLabel("", statusBar());
	//statusBar()->addWidget(rotateLabel,0);
	//shiftLabel = new QLabel("", statusBar());
	//statusBar()->addWidget(shiftLabel,0);
	//scaleLabel = new QLabel("", statusBar());
	//statusBar()->addWidget(scaleLabel,0);
	//zoomLabel = new QLabel("", statusBar());
	//statusBar()->addWidget(zoomLabel,0);
	//
	//connect(dataWidget, SIGNAL(rotationChanged(double,double,double)),this,SLOT(showRotate(double,double,double)));
	//connect(dataWidget, SIGNAL(vieportShiftChanged(double,double)),this,SLOT(showShift(double,double)));
	//connect(dataWidget, SIGNAL(scaleChanged(double,double,double)),this,SLOT(showScale(double,double,double)));
	//connect(dataWidget, SIGNAL(zoomChanged(double)),this,SLOT(showZoom(double)));

	//connect(functionCB, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(createFunction(const QString&)));
	//connect(psurfaceCB, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(createPSurface(const QString&)));
	//connect(projection, SIGNAL( toggled(bool) ), this, SLOT( toggleProjectionMode(bool)));
	//connect(colorlegend, SIGNAL( toggled(bool) ), this, SLOT( toggleColorLegend(bool)));
	//connect(autoscale, SIGNAL( toggled(bool) ), this, SLOT( toggleAutoScale(bool)));
	//connect(shader, SIGNAL( toggled(bool) ), this, SLOT( toggleShader(bool)));
	//connect(mouseinput, SIGNAL( toggled(bool) ), dataWidget, SLOT( enableMouse(bool)));
	//connect(lightingswitch, SIGNAL( toggled(bool) ), this, SLOT( enableLighting(bool)));
	//connect(normals, SIGNAL( toggled(bool) ), this, SLOT( showNormals(bool)));
	//connect(normalsquality,  SIGNAL(valueChanged(int)), this, SLOT(setNormalQuality(int)) );
	//connect(normalslength,  SIGNAL(valueChanged(int)), this, SLOT(setNormalLength(int)) );
	//		
	setStandardView();

	dataWidget->coordinates()->setLineSmooth(true);
  //dataWidget->coordinates()->setGridLinesColor(RGBA(0.35,0.35,0.35,1));
	dataWidget->enableMouse(true);
  dataWidget->setKeySpeed(15,20,20);

 // lightingdlg_ = new ConfigFrame( this );
 // lightingdlg_->assign( dataWidget);  

  dataWidget->setTitleFont( "Arial", 14, QFont::Normal );
  //dataWidget->legend()->setScale(LINEARSCALE);
  for (unsigned i=0; i!=dataWidget->coordinates()->axes.size(); ++i)
  {
    dataWidget->coordinates()->axes[i].setMajors(7);
    dataWidget->coordinates()->axes[i].setMinors(5);
  }
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

  createFunction("hat", false);
  dataWidget->enableLighting(true);
  dataWidget->illuminate(0);
}

void MainWindow::open()
{
}

int MainWindow::createFunction(QString const& name, bool append /*= true*/)
{
	dataWidget->makeCurrent();
  if (name == QString("hat")) 
  {
    Hat hat(*dataWidget, true);

    hat.setMesh(51,72);
    hat.setDomain(-1.5,1.5,-1.5,1.5);
    hat.create(append);	
  }
  else if (name == QString("hatn")) 
  {
    Hat hat(*dataWidget, false);

    hat.setMesh(51,72);
    hat.setDomain(-1.5,1.5,-1.5,1.5);
    hat.create(append);	
  }
	else if (name == QString("ripple")) 
	{
		Ripple ripple(*dataWidget);
    ripple.setMesh(120,120);
		ripple.create(append);	
	}
  else if (name == QString("boy")) 
  {
    Boy boy(*dataWidget);
    boy.create(append);
  }

  //	updateColorLegend(7,5);

  if (append)
    return (int)(dataWidget->plotlets() - 1);
  
  return 0;
}



void MainWindow::pickCoordSystem( QAction* action)
{
	if (/*!action ||*/ !dataWidget)
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

	//if (action == Box || action == Frame)
	//{
	//	if (action == Box)
		dataWidget->setCoordinateStyle(BOX);
	//	if (action == Frame)
	//		dataWidget->setCoordinateStyle(FRAME);
		grids->setEnabled(true);
	//}
	//else if (action == None)
	//{
	//  dataWidget->setTitle("QwtPlot3D (Use Ctrl-Alt-Shift-LeftBtn-Wheel or keyboard)");
	//	dataWidget->setCoordinateStyle(NOCOORD);
	//	grids->setEnabled(false);
	//}
}

void MainWindow::pickPlotStyle( QAction* action )
{
	//if (!action || !dataWidget)
	//	return;

	//if (action == polygon)
	//{
	//	dataWidget->setPlotStyle(FILLED);
	//}
	//else if (action == filledmesh)
	//{
	//	dataWidget->setPlotStyle(FILLEDMESH);
	//}
	//else if (action == wireframe)
	//{
	//	dataWidget->setPlotStyle(WIREFRAME);
	//}
	//else if (action == hiddenline)
	//{
	//	dataWidget->setPlotStyle(HIDDENLINE);
	//}
	//else if (action == pointstyle)
	//{
 //   dataWidget->setPlotStyle(Qwt3D::POINTS);
	//}
	//else
	//{
	//	dataWidget->setPlotStyle(NOPLOT);
	//}
	//dataWidget->updateData();
	//dataWidget->updateGL();
}

void MainWindow::pickFloorStyle( QAction* action )
{
	//if (!action || !dataWidget)
	//	return;

	//if (action == floordata)
	//{
	//	dataWidget->setFloorStyle(FLOORDATA);
	//}
	//else if (action == flooriso)
	//{
	//	dataWidget->setFloorStyle(FLOORISO);
	//}
	//else
	//{
	//	dataWidget->setFloorStyle(NOFLOOR);
	//}
	//
	//dataWidget->updateData();
	//dataWidget->updateGL();
}	

void MainWindow::resetColors()
{
	//if (!dataWidget)
	//	return;

	//const RGBA axc = RGBA(0,0,0,1);
	//const RGBA bgc = RGBA(1.0,1.0,1.0,1.0);
	//const RGBA msc = RGBA(0,0,0,1);
	//const RGBA nuc = RGBA(0,0,0,1);
	//const RGBA lbc = RGBA(0,0,0,1);
	//const RGBA tc = RGBA(0,0,0,1);

	//dataWidget->coordinates()->setAxesColor(axc);
	//dataWidget->setBackgroundColor(bgc);
	//dataWidget->setMeshColor(msc);
	//dataWidget->updateData();
	//dataWidget->coordinates()->setNumberColor(nuc);
	//dataWidget->coordinates()->setLabelColor(lbc);
 // dataWidget->setTitleColor(tc);

	//dataWidget->setDataColor(StandardColor());
	//dataWidget->updateData();	
	//dataWidget->updateNormals();
	//dataWidget->updateGL();
}


void MainWindow::pickMeshColor()
{
 // 
	//QColor c = QColorDialog::getColor( Qt::white, this );
 // if ( !c.isValid() )
	//	return;
	//RGBA rgb = Qt2GL(c);
	//dataWidget->setMeshColor(rgb);
	//dataWidget->updateData();
	//dataWidget->updateGL();
}




void MainWindow::pickDataColor()
{
  //QString s =  QFileDialog::getOpenFileName( this, "", "./../../data/colormaps", "Colormap files (*.map *.MAP)");
  //adaptDataColors(s);
}

void MainWindow::adaptDataColors(const QString& fileName)
{
 // ColorVector cv;
	//
	//if (!openColorMap(cv, fileName))
	//	return;
	//
	//StandardColor col_;
	//col_.setColorVector(cv);
	//
	//dataWidget->setDataColor(col_);
	//dataWidget->updateData();
	//dataWidget->updateNormals();
	//dataWidget->showColorLegend(legend_);
 // dataWidget->updateGL();
}

void MainWindow::setStandardView()
{
	dataWidget->setRotation(30,0,15);
	dataWidget->setViewportShift(0.05,0);
	dataWidget->setScale(1,1,1);
	dataWidget->setZoom(0.95);
}

void MainWindow::rotate()
{
	//if (!dataWidget)
	//	return;

	//dataWidget->setRotation(
	//	int(dataWidget->xRotation() + 1) % 360,
	//	int(dataWidget->yRotation() + 1) % 360,
	//	int(dataWidget->zRotation() + 1) % 360
	//	);
}

void MainWindow::toggleProjectionMode(bool val)
{
	//dataWidget->setOrtho(val);
}

void MainWindow::toggleColorLegend(bool val)
{
	//legend_ = val;
	//dataWidget->showColorLegend(val);
}

void MainWindow::toggleAutoScale(bool val)
{
	//dataWidget->coordinates()->setAutoScale(val);
	//dataWidget->updateGL();
}

void MainWindow::toggleShader(bool val)
{
	//if (val)
	//	dataWidget->setShading(GOURAUD);
	//else
	//	dataWidget->setShading(FLAT);
}

void MainWindow::showNormals(bool val)
{
	//dataWidget->showNormals(val);
	//dataWidget->updateNormals();
	//dataWidget->updateGL();
}

void MainWindow::setNormalLength(int val)
{
	//dataWidget->setNormalLength(val / 400.);
	//dataWidget->updateNormals();
	//dataWidget->updateGL();
}

void MainWindow::setNormalQuality(int val)
{
	//dataWidget->setNormalQuality(val);
	//dataWidget->updateNormals();
	//dataWidget->updateGL();
}

bool MainWindow::openColorMap(ColorVector& cv, QString fname)
{	
 // if (fname.isEmpty())
 //   return false;
 // 
 // ifstream file(QWT3DLOCAL8BIT(fname));

	//if (!file)
	//	return false;
	//
	//RGBA rgb;
	//cv.clear();
	//
	//while ( file ) 
	//{		
	//	file >> rgb.r >> rgb.g >> rgb.b;
	//	file.ignore(1000,'\n');
	//	if (!file.good())
	//		break;
	//	else
	//	{
	//		rgb.a = 1;
	//		rgb.r /= 255;
	//		rgb.g /= 255;
	//		rgb.b /= 255;
	//		cv.push_back(rgb);	
	//	}
	//}

	return true;
}

void MainWindow::updateColorLegend(int majors, int minors)
{
	//dataWidget->legend()->setMajors(majors);
	//dataWidget->legend()->setMinors(minors);
	//double start, stop;
	//dataWidget->coordinates()->axes[Z1].limits(start,stop);
	//dataWidget->legend()->setLimits(start, stop);
}		

void MainWindow::togglePlotlet(bool val)
{
  if (!val) // removing plotlets
  {
    PM_IT i = pmanager.begin();
    if (2 == dataWidget->plotlets()) // we are going to remove the penultimate plotlet...
    {
      // ... so make sure, the last one will be secured
      while (i != pmanager.end()) 
      {
        if (i->cbox->isChecked() && i->cbox != sender())
        {
          i->cbox->setEnabled(false);
          break;
        }       
        ++i;
      }
    }

    int deleted = -1;
    i = pmanager.begin();
    while (i != pmanager.end()) 
    {
      if (sender() == i->cbox)
      {
        dataWidget->removePlotlet(i->pos);
        deleted = i->pos;
        i->pos = -1;
        break;
      }
      ++i;
    }
    
    // adapt plotlet vector idx observer
    if (deleted>=0)
    {
      i = pmanager.begin();
      while (i != pmanager.end()) 
      {
        if (i->pos >= deleted)
        {
          --i->pos;
        }      
        ++i;
      }
    }
  }
  else // adding plotlets
  {
    PM_IT i = pmanager.begin();
    while (i != pmanager.end()) 
    {
      // re-enable possible single plotlet checkbox
      if (!i->cbox->isEnabled())
        i->cbox->setEnabled(true);
      if (sender() == i->cbox)
      {
        i->pos = createFunction(i.key());
      }
      ++i;
    }
  }
  dataWidget->updateData();
  dataWidget->updateGL();
}
