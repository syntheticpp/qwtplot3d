#include <qapplication.h>
#include <qtimer.h>

#include "colormodels.h"


//--------------------------------------------------------------------
//              colormodels.cpp
//
//      Demonstrates use of data related colors
//--------------------------------------------------------------------

class MyColor : public Color
{
private:
	
	double minval_, maxval_;
	double cnt_;



public:
	
	explicit MyColor(int cnt)
	{
		cnt_ = cnt / 10.;
		
		if (cnt>10)
			cnt_ = 1;
		else if (cnt<0)
			cnt_ = 0;
		
		RGBA elem;

		minval_ = 0; maxval_ = 1;
	}	
	
	void setMinMax(double minval, double maxval)
	{
		minval_ = minval;
		maxval_ = maxval;

		if (maxval_ == minval_)
			maxval_ += 1;
	}
	
	RGBA operator()(double x, double y, double z)
	{
		return RGBA(minval_ + cnt_*(maxval_-minval_), minval_ + (1-cnt_)*(maxval_-minval_), x*y - (minval_) / (maxval_-minval_));
	}

};	


Plot::Plot(QWidget* pw)
:QwtPlot3D(pw)
{
	Saddle saddle(this);
	
	setDataColor(new MyColor(0));
	saddle.create();

	setRotation(30,0,15);
	setScale(1,1,1);
	setShift(0.1,0,0);
	setZoom(0.8);
	coordinates()->setNumberFont("Courier",8);

	for (unsigned i=0; i!=coordinates()->axes.size(); ++i)
	{
		coordinates()->axes[i].setMajors(7);
		coordinates()->axes[i].setMinors(4);
	}

	
	updateData();
	updateCoordinates();

  QTimer* timer = new QTimer( this );
	connect( timer, SIGNAL(timeout()), this, SLOT(changeColor()) );

	timer->start(400);

}

using namespace Qwt3d;

void Plot::changeColor()
{
	static int cnt = 0;
	static int dir = 1;

	MyColor* col = new MyColor(cnt);
	
	cnt += dir;
	if (!(cnt % 10))
		dir = -dir;

	calculateHull();
	col->setMinMax(hullFirst().z, hullSecond().z);

	setDataColor(col);
	updateData();
	setTitle(QString::number(cnt));
	updateGL();
}


int main(int argc, char **argv)
{
		QApplication a(argc, argv);

		Plot* plot = new Plot;
		plot->setBackgroundColor(RGBA(1,220./255, 180./255));
		plot->setTitle("Colors");
		plot->setCoordinateStyle(BOX);

		a.setMainWidget(plot);
		plot->resize(800,600);
    plot->show();
    return a.exec(); 
}
