#include <qlayout.h>
#include <qslider.h>
#include <qcheckbox.h>
#include <qradiobutton.h>

#include "enrichmentmainwindow.h"
#include "enrichments.h"
#include <qwt3d_function.h>

using namespace Qwt3D;

class Hat : public Function
{
public:

Hat(Curve& pw)
:Function(pw)
{
}

double operator()(double x, double y)
{
    double ret = 1.0 / (x*x+y*y+0.5);
    return ret;
}
QString name() const {return "$\\frac{1}{x^2+y^2+\\frac{1}{2}}$";}
};

Hat* hat;

EnrichmentMainWindow::EnrichmentMainWindow( QWidget* parent )
	: DummyBase( parent )
{
#if QT_VERSION < 0x040000
    setCaption("enrichment");
    QGridLayout *grid = new QGridLayout( frame, 0, 0 );
#else
    setupUi(this);
    QGridLayout *grid = new QGridLayout( frame);
#endif

    plot = new Plot3D(frame);
    grid->addWidget(plot, 0, 0);

    curve = new Curve(plot);
    plot->addCurve(curve);
    hat = new Hat(*curve);
    hat->setMesh(23,21);
    hat->setDomain(-1.8,1.7,-1.6,1.7);
    hat->create();

    bar = Bar(0.007, 0.5);
    curve->setPlotStyle(bar);

    for (unsigned i=0; i!=plot->coordinates()->axes.size(); ++i){
        plot->coordinates()->axes[i].setMajors(5);
        plot->coordinates()->axes[i].setMinors(4);
    }

    plot->setTitle("Bar Style (Vertex Enrichment)");
    plot->setTitleFont("Arial", 12);
    plot->setZoom(0.8);
    plot->setRotation(30, 0, 15);
    plot->setCoordinateStyle(BOX);
    plot->setMeshLineWidth(1);
    plot->coordinates()->setGridLinesColor(RGBA(0,0,0.5));
    plot->coordinates()->setLineWidth(1);
    plot->coordinates()->setNumberFont("Courier",8);
    plot->coordinates()->adjustNumbers(5);
 
    setColor();
    plot->updateData();
    plot->updateGL();

    level_ = 0.5;
    levelSlider->setValue(50);
    connect(levelSlider, SIGNAL(valueChanged(int)), this, SLOT(setLevel(int)));

    width_ = 0.007;
    widthSlider->setValue(140);
    connect( widthSlider, SIGNAL(valueChanged(int)), this, SLOT(setWidth(int)));

    //connect( barBtn, SIGNAL(clicked()), this, SLOT(barSlot()) );
    //connect( sliceBtn, SIGNAL(clicked()), this, SLOT(sliceSlot()) );

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_LINE_SMOOTH);
}

EnrichmentMainWindow::~EnrichmentMainWindow()
{
    delete hat;
}

void EnrichmentMainWindow::setColor()
{	
    Qwt3D::ColorVector cv;

    RGBA rgb;
    int i = 252;
    int step = 4;

    while ( i>= 0){
        rgb.r = i/255.; rgb.g = (i-60>0) ? (i-60)/255.:0; rgb.b = 0;
        rgb.a = 0.6;
        cv.push_back(rgb);
        if (!--step){
            i -= 4;
            step = 4;
        }
    }

    StandardColor* col = new StandardColor(curve);
    col->setColorVector(cv);
    plot->setDataColor(col);
}

void EnrichmentMainWindow::setLevel(int i)
{
    level_ = 1 - i / 100.;
    bar.configure(width_, level_);
    curve->setPlotStyle(bar);
    plot->updateData();
    plot->updateGL();
}

void EnrichmentMainWindow::setWidth(int i)
{
    width_ = i / 20000.;
    bar.configure(width_, level_);
    curve->setPlotStyle(bar);
    plot->updateData();
    plot->updateGL();
}

void EnrichmentMainWindow::barSlot()
{  
    Bar b(width_,level_);
    curve->setPlotStyle(bar);
    //bar = (Bar*)plot->setPlotStyle(b);
    plot->updateData();
    plot->updateGL();
}
