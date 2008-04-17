  //-----------------------------------------------------------------
  //              simpleplot.cpp
  //
  //      A simple example which shows how to use SurfacePlot
  //-----------------------------------------------------------------

  #include <math.h>
  #include <qapplication.h>
  #include <qwt3d_plot.h>
  #include <qwt3d_curve.h>
  #include <qwt3d_color.h>
  #include <qwt3d_function.h>
  

  using namespace Qwt3D;

  class Rosenbrock : public Function
  {
  public:

    Rosenbrock(Curve& pw)
    :Function(pw)
    {
    }

    double operator()(double x, double y)
    {
      return log((1-x)*(1-x) + 100 * (y - x*x)*(y - x*x)) / 8;
    }
  };

  class RosenbrockNegative : public Function
  {
  public:

    RosenbrockNegative(Curve& pw)
    :Function(pw)
    {
    }

    double operator()(double x, double y)
    {
      return -1.0 * (log((1-x)*(1-x) + 100 * (y - x*x)*(y - x*x)) / 8);
    }
  };


  class Plot : public Plot3D
  {
  public:
      Plot();
   private:
      Curve* curve_p;
      Curve* curve_p2;
  };


  Plot::Plot()
  {
    setTitle("A Simple SurfacePlot Demonstration");
    
    curve_p = new Curve;
    curve_p->setPlot(this);
    addCurve(curve_p);
    Rosenbrock rosenbrock(*curve_p);

    rosenbrock.setMesh(41,31);
    rosenbrock.setDomain(-1.73,1.5,-1.5,1.5);
    rosenbrock.setMinZ(-10);

    rosenbrock.create();

    curve_p2 = new Curve;
    curve_p2->setPlot(this);
    addCurve(curve_p2);
    RosenbrockNegative rosenbrockNegative(*curve_p2);

    rosenbrockNegative.setMesh(41,31);
    rosenbrockNegative.setDomain(-1.73,1.5,-1.5,1.5);
    rosenbrockNegative.setMinZ(-10);

    rosenbrockNegative.create();

    setRotation(30,0,15);
    setScale(1,1,1);
    setShift(0.15,0,0);
    setZoom(0.9);

    for (unsigned i=0; i!=coordinates()->axes.size(); ++i)
    {
      coordinates()->axes[i].setMajors(7);
      coordinates()->axes[i].setMinors(4);
    }


    coordinates()->axes[X1].setLabelString("x-axis");
    coordinates()->axes[Y1].setLabelString("y-axis");
    //coordinates()->axes[Z1].setLabelString(QChar(0x38f)); // Omega - see http://www.unicode.org/charts/


    setCoordinateStyle(BOX);

    updateData();
    updateGL();
  }

  int main(int argc, char **argv)
  {
      QApplication a(argc, argv);
      Plot plot;
#if QT_VERSION < 0x040000
      a.setMainWidget(&plot);
#endif
      plot.resize(800,600);
      plot.show();
      return a.exec();
  }
