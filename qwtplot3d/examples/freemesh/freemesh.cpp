#include <qapplication.h>
#include <qwt3d_surfaceplot.h>
  
using namespace Qwt3D;

void createTetrahedron(Qwt3D::TripleField& pos, Qwt3D::CellField& poly)
{
  pos.clear(); poly.clear();
  Cell cell(3);
  
  pos.push_back(Triple(0,0,0));
  pos.push_back(Triple(1,0,0));
  pos.push_back(Triple(0.5,1,0));
  pos.push_back(Triple(0.5,0.7,1.5));

  cell[0] = 0; 
  cell[1] = 1; 
  cell[2] = 2; 
  poly.push_back(cell);
  
  cell[0] = 0; 
  cell[1] = 1; 
  cell[2] = 3; 
  poly.push_back(cell);

  cell[0] = 1; 
  cell[1] = 2; 
  cell[2] = 3; 
  poly.push_back(cell);

  cell[0] = 2; 
  cell[1] = 0; 
  cell[2] = 3; 
  poly.push_back(cell); 
}


int main(int argc, char **argv)
{
  QApplication a(argc, argv);

  SurfacePlot plot;
  TripleField pos;
  CellField poly;
  createCone(pos, poly);
  plot.loadFromData(pos, poly);
  plot.setRotation(30,0,15);
  for (unsigned i=0; i!=plot.coordinates()->axes.size(); ++i)
  {
    plot.coordinates()->axes[i].setMajors(4);
    plot.coordinates()->axes[i].setMinors(5);
  }

  a.setMainWidget(&plot);
  plot.resize(800,600);
  plot.show();
  return a.exec();
}
