#include <qsplitter.h>
#include <qlayout.h>
#include <qcombobox.h>
#include <qaction.h>

#include "printmainwindow.h"
#include "../../../include/qwt3d_function.h"

using namespace Qwt3D;


class Rosenbrock : public Function
{
public:

	Rosenbrock(SurfacePlot* pw)
	:Function(pw)
	{
	}

	double operator()(double x, double y)
	{
		return log((1-x)*(1-x) + 100 * (y - x*x)*(y - x*x)) / 8;
	}
	QString name() const {return "Almost {\\it Rosenbrock}\\,:\\quad$\\frac{\\ln((1-x)^2 + 100(y-x^2)^2)}{8}$";}
};

class Hat : public Function
{
public:

	Hat(SurfacePlot* pw)
	:Function(pw)
	{
	//	setMaxZ(0.8);     
	}
	
	double operator()(double x, double y)
	{
		return 1.0 / (x*x+y*y+0.5);
	}
	QString name() const {return "$\\frac{1}{x^2+y^2+\\frac{1}{2}}$";}
};

printMainWindow::printMainWindow( QWidget* parent, const char* name, WFlags f )
	: printMainWindowBase( parent, name, f )
{
	filetype_ = "tex (both)";
	filetypeCB->setCurrentText("tex (both)");

	connect(filetypeCB, SIGNAL(activated(const QString&)), this, SLOT(setFileType(const QString&)));
  connect( dump, SIGNAL( activated() ) , this, SLOT( dumpImage() ) );
	
	setCaption("print");
	QSplitter* spl = new QSplitter(QSplitter::Horizontal, frame);
  QGridLayout* layout = new QGridLayout( frame, 1, 1, 2, 0); 
	layout->addWidget( spl, 0, 0 );
 
	plot[0] = new SurfacePlot(spl);
	//plot[0]->setFloorStyle(FLOORISO);
	plot[0]->setZoom(0.8);
	plot[0]->setCoordinateStyle(BOX);
	plot[0]->setBackgroundColor(RGBA(1,1, 157./255));
	plot[0]->setRotation(30,0,15);


	plot[1] = new SurfacePlot(spl);
	plot[1]->setZoom(0.8);
	plot[1]->setCoordinateStyle(FRAME);
	plot[1]->setPlotStyle(WIREFRAME);
	plot[1]->setBackgroundColor(RGBA(1,1, 157./255));
	plot[1]->setFloorStyle(FLOORDATA);
	plot[1]->setRotation(30,0,15);


	rosenbrock = new Rosenbrock(plot[0]);
	
	rosenbrock->setMesh(21,23);
	rosenbrock->setDomain(-1.73,1.8,-1.9,1.8);
	rosenbrock->setMinZ(-100);
	
	rosenbrock->create();

	hat = new Hat(plot[1]);
	
	hat->setMesh(30,27);
	hat->setDomain(-1.8,1.7,-1.6,1.7);
	
	//hat.setMaxZ(1.1);

	hat->create();	

	for (unsigned i=0; i!=plot[0]->coordinates()->axes.size(); ++i)
  {
    plot[0]->coordinates()->axes[i].setMajors(5);
    plot[0]->coordinates()->axes[i].setMinors(4);
    plot[1]->coordinates()->axes[i].setMajors(5);
    plot[1]->coordinates()->axes[i].setMinors(4);
  }

	plot[0]->setTitle("Rosenbrock");
	plot[1]->setTitle("Hat");

  plot[0]->makeCurrent();
	plot[0]->updateData();
  plot[0]->updateGL();
	plot[1]->makeCurrent();
  plot[1]->updateData();
  plot[1]->updateGL();


	LabelPixmap::usePrinterFonts(true);
}

printMainWindow::~printMainWindow()
{
	delete rosenbrock;
	delete hat;
}

void printMainWindow::dumpImage()
{
	if (!plot[0] || !plot[1])
		return;
	QString name;
		
  plot[0]->setTitle(rosenbrock->name());
  plot[1]->setTitle(hat->name());

	if (filetype_ == QString("eps"))
	{
		plot[0]->saveVector( "dump0a.eps", "EPS");
		plot[1]->saveVector( "dump0b.eps", "EPS");
	}
	else if (filetype_ == QString("ps"))
	{
		plot[0]->saveVector( "dump0a.ps", "PS");
		plot[1]->saveVector( "dump0b.ps", "PS");
	}
	else if (filetype_ == QString("pdf"))
	{
		plot[0]->makeCurrent();
		plot[0]->saveVector( "dump0a.pdf", "PDF");
		plot[1]->makeCurrent();
		plot[1]->saveVector( "dump0b.pdf", "PDF");
	}
	else if (filetype_ == QString("latex"))
	{
		plot[0]->saveVector( "dump1a.tex", "TEX");
		plot[1]->saveVector( "dump1b.tex", "TEX");
		plot[0]->saveVector( "dump1a.eps", "EPS",true);
		plot[1]->saveVector( "dump1b.eps", "EPS",true);
	}
	else if (filetype_ == QString("pdftex"))
	{
		plot[0]->saveVector( "dump2a.tex", "TEX");
		plot[1]->saveVector( "dump2b.tex", "TEX");
		plot[0]->saveVector( "dump2a.pdf", "PDF",true);
		plot[1]->saveVector( "dump2b.pdf", "PDF",true);
	}
	else if (filetype_ == QString("tex (both)"))
	{
		plot[0]->saveVector( "dump3a.tex", "TEX");
		plot[1]->saveVector( "dump3b.tex", "TEX");
		plot[0]->saveVector( "dump3a.pdf", "PDF",true);
		plot[1]->saveVector( "dump3b.pdf", "PDF",true);
		plot[0]->saveVector( "dump3a.eps", "EPS",true);
		plot[1]->saveVector( "dump3b.eps", "EPS",true);
	}

	plot[0]->setTitle("Rosenbrock");
 	plot[1]->setTitle("Hat");

}

void printMainWindow::setFileType(QString const& name)
{
	filetype_ = name;	
}