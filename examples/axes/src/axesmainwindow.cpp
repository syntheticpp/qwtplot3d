#include <qsplitter.h>
#include <qlayout.h>
#include <qcombobox.h>
#include <qaction.h>
#include <qslider.h>
#include <qcheckbox.h>
#include <qpopupmenu.h>
#include <qdatetime.h>

#include "axesmainwindow.h"
#include "../../../include/qwt3d_function.h"

using namespace Qwt3D;

/*****************************
*
*  3 Examples for user defined 
*  tic labels
*
******************************/

class Letter : public Axis::Item
{
public:
  explicit Letter(bool uppercase = true) : uc_(uppercase) {}
  Axis::Item* clone() const {return new Letter(*this);}
  QString operator()(double val, unsigned int idx)
  {
    if (idx<tics() && idx < 26)
      return (uc_) ? QString(QChar('A'+idx)) : QString(QChar('a'+idx));
    return QString("-");
  }
private:
  bool uc_;
};

class Imaginary : public Axis::Item
{
public:
  Axis::Item* clone() const {return new Imaginary;}
  QString operator()(double val, unsigned int idx)
  {
    if (idx<tics())
    {
      if (val)
        return QString::number(val) + "*i";
      return QString::number(val);
    }
    return QString("");
  }
};

class TimeItems : public Axis::Item
{
public:
  Axis::Item* clone() const {return new TimeItems;}
  QString operator()(double val, unsigned int idx)
  {
    if (idx<tics())
    {
      QTime t = QTime::currentTime();
      int h = t.hour();
      int m = t.minute();
      if (m+idx > 59)
      {
        if (h==23)
          h=0;
        else
          h+=1;
        m %= 60; 
      }
      else
        m += idx;

      return QTime(h,m).toString("hh:mm")+"h";
    }
    return QString("");
  }
};


// Example function
class Rosenbrock : public Function
{
public:

	Rosenbrock(SurfacePlot& pw)
	:Function(pw)
	{
	}

	double operator()(double x, double y)
	{
		return log((1-x)*(1-x) + 100 * (y - x*x)*(y - x*x)) / 4;
	}
};


// Main widget

AxesMainWindow::AxesMainWindow( QWidget* parent, const char* name, WFlags f )
	: AxesMainWindowBase( parent, name, f )
{
	setCaption("axes");
 
  QGridLayout *grid = new QGridLayout( frame, 0, 0 );
  plot = new SurfacePlot(frame);
  grid->addWidget( plot, 0, 0 );

  plot->setZoom(0.8);
	plot->setRotation(30,0,15);

	plot->coordinates()->setGridLines(true,true);
	plot->setCoordinateStyle(BOX);
	//plot->setCoordinateStyle(NOCOORD);
	//plot->setPlotStyle(FILLED);


	rosenbrock = new Rosenbrock(*plot);
	
	rosenbrock->setMesh(31,33);
	rosenbrock->setDomain(-1.73,1.8,-1.9,1.8);
	rosenbrock->setMinZ(-100);
	
	rosenbrock->create();

	for (unsigned i=0; i!=plot->coordinates()->axes.size(); ++i)
  {
    plot->coordinates()->axes[i].setMajors(5);
    plot->coordinates()->axes[i].setMinors(4);
  }

	//plot->setTitle("Rosenbrock");
	plot->setMeshLineWidth(1);
	plot->coordinates()->setGridLinesColor(RGBA(0,0,0.5));
	plot->coordinates()->setLineWidth(1);
	plot->coordinates()->setNumberColor(RGBA(0,0.5,0));
	plot->coordinates()->setNumberFont("Courier",10);
	plot->setTitleFont("Courier",11);
	plot->coordinates()->setLabelFont("Courier",12, QFont::Bold);
	plot->coordinates()->axes[X1].setLabelColor(RGBA(0,0,0.4));
	plot->coordinates()->axes[X1].setLabelString("X1");
	plot->coordinates()->axes[Y1].setLabelColor(RGBA(0,0,0.4));
	plot->coordinates()->axes[Y1].setLabelString("Y1");
	plot->coordinates()->axes[Z1].setLabelColor(RGBA(0,0,0.4));
	plot->coordinates()->axes[Z1].setLabelString("Z1");
	plot->coordinates()->axes[X2].setLabelColor(RGBA(0,0,0.4));
	plot->coordinates()->axes[X2].setLabelString("X2");
	plot->coordinates()->axes[Y2].setLabelColor(RGBA(0,0,0.4));
	plot->coordinates()->axes[Y2].setLabelString("Y2");
	plot->coordinates()->axes[Z2].setLabelColor(RGBA(0,0,0.4));
	plot->coordinates()->axes[Z2].setLabelString("Z2");
	plot->coordinates()->axes[X3].setLabelColor(RGBA(0,0,0.4));
	plot->coordinates()->axes[X3].setLabelString("X3");
	plot->coordinates()->axes[Y3].setLabelColor(RGBA(0,0,0.4));
	plot->coordinates()->axes[Y3].setLabelString("Y3");
	plot->coordinates()->axes[Z3].setLabelColor(RGBA(0,0,0.4));
	plot->coordinates()->axes[Z3].setLabelString("Z3");
	plot->coordinates()->axes[X4].setLabelColor(RGBA(0,0,0.4));
	plot->coordinates()->axes[X4].setLabelString("X4");
	plot->coordinates()->axes[Y4].setLabelColor(RGBA(0,0,0.4));
	plot->coordinates()->axes[Y4].setLabelString("Y4");
	plot->coordinates()->axes[Z4].setLabelColor(RGBA(0,0,0.4));
	plot->coordinates()->axes[Z4].setLabelString("Z4");
  
  //plot->coordinates()->setGridLines(true,false,Qwt3D::FLOOR);
  
  plot->coordinates()->setLineSmooth(true);
  smoothBox->setDown(true);
  Items->insertItem( "&Standard",  this, SLOT(standardItems()), ALT+Key_S );
  Items->insertItem( "&Complex", this, SLOT(complexItems()),    ALT+Key_C );
  Items->insertItem( "&Letter", this, SLOT(letterItems()),    ALT+Key_L );
  Items->insertItem( "&Time", this, SLOT(timeItems()),    ALT+Key_T );
        
  plot->makeCurrent();
	plot->updateData();
  plot->updateGL();

	connect(smoothBox, SIGNAL(toggled(bool)), this, SLOT(setSmootLines(bool)) );	
	connect(numbergapslider, SIGNAL(valueChanged(int)), this, SLOT(setNumberGap(int)) );
	connect(labelgapslider, SIGNAL(valueChanged(int)), this, SLOT(setLabelGap(int)) );
	connect(ticLengthSlider, SIGNAL(valueChanged(int)), this, SLOT(setTicLength(int)) );
	connect(ticNumberSlider, SIGNAL(valueChanged(int)), this, SLOT(setTicNumber(int)) );

  tics = plot->coordinates()->axes[X1].majors();

  resetTics();
}

AxesMainWindow::~AxesMainWindow()
{
	delete rosenbrock;
}

void AxesMainWindow::setNumberGap(int gap)
{
	plot->coordinates()->adjustNumbers(gap);
  plot->makeCurrent();
  plot->updateGL();
}

void AxesMainWindow::setLabelGap(int gap)
{
	plot->coordinates()->adjustLabels(gap);
  plot->makeCurrent();
  plot->updateGL();
}

void AxesMainWindow::setSmoothLines(bool val)
{
  plot->coordinates()->setLineSmooth(val);
  plot->updateGL();
}

void AxesMainWindow::setTicLength(int val)
{
  double majl =  (plot->coordinates()->second()-plot->coordinates()->first()).length() / 1000.;
	majl = majl * val;
	plot->coordinates()->setTicLength(majl,0.6*majl);
  plot->updateGL();
}

void AxesMainWindow::setTicNumber(int degree)
{
  plot->coordinates()->axes[X1].setMajors(tics + degree);
  plot->updateGL();
}


void AxesMainWindow::standardItems()
{  
  resetTics();
  plot->updateGL();
}

void AxesMainWindow::letterItems()
{  
  ticNumberSlider->setEnabled(true);
  plot->coordinates()->axes[X1].setAutoScale(false);
  plot->coordinates()->setStandardNumbering();
  plot->coordinates()->axes[X1].setMap(new Letter);
  plot->coordinates()->axes[X2].setMap(new Letter);
  plot->coordinates()->axes[X3].setMap(new Letter);
  plot->coordinates()->axes[X4].setMap(new Letter);
  plot->coordinates()->axes[Y1].setMap(new Letter(false));
  plot->coordinates()->axes[Y2].setMap(new Letter(false));
  plot->coordinates()->axes[Y3].setMap(new Letter(false));
  plot->coordinates()->axes[Y4].setMap(new Letter(false));
  plot->updateGL();
}

void AxesMainWindow::complexItems()
{  
  resetTics();
  plot->coordinates()->axes[Y1].setMap(new Imaginary);
  plot->coordinates()->axes[Y2].setMap(new Imaginary);
  plot->coordinates()->axes[Y3].setMap(new Imaginary);
  plot->coordinates()->axes[Y4].setMap(new Imaginary);
  plot->updateGL();
}

void AxesMainWindow::timeItems()
{  
  resetTics();
  plot->coordinates()->axes[Z1].setMap(new TimeItems);
  plot->coordinates()->axes[Z2].setMap(new TimeItems);
  plot->coordinates()->axes[Z3].setMap(new TimeItems);
  plot->coordinates()->axes[Z4].setMap(new TimeItems);
  plot->updateGL();
}

void AxesMainWindow::resetTics()
{
  ticNumberSlider->setEnabled(false);
  plot->coordinates()->axes[X1].setMajors(5);
  plot->coordinates()->setAutoScale(true);
  plot->coordinates()->setStandardNumbering();
}