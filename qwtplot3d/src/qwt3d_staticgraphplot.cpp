#if defined(_MSC_VER) /* MSVC Compiler */
/* 'identifier' : truncation from 'type1' to 'type2' */
#pragma warning ( disable : 4305 ) 
#endif


#include "qwt3d_staticgraphplot.h"

using namespace std;
using namespace Qwt3D;


// Data class (private)


StaticGraphPlot::StaticGraphData::StaticGraphData() 
{
  //datatype_p=Qwt3D::GRAPH;
  //setHull(ParallelEpiped());
}

StaticGraphPlot::StaticGraphData::~StaticGraphData()
{
  setHull(ParallelEpiped());
  nodes.clear();
  edges.clear();
}

bool StaticGraphPlot::StaticGraphData::empty() const
{
  return nodes.empty();
}

// Data class end


StaticGraphPlot::StaticGraphPlot( QWidget * parent, const QGLWidget * shareWidget)
    : GraphPlot( parent, shareWidget) 
{
  data_ = new StaticGraphData();
  actualData_p = data_;
}

StaticGraphPlot::~StaticGraphPlot()
{
  delete data_;
}

void StaticGraphPlot::createOpenGlData()
{
  if (plotStyle() == NOPLOT)
    return;

  //todo
  //if (plotStyle() == Qwt3D::USER)
  //{
  //  if (userplotstyle_p)
  //    drawEnrichment(*userplotstyle_p);
  //  return;
  //}

// 	setDeviceLineWidth(meshLineWidth());
//   GLStateBewarer sb(GL_POLYGON_OFFSET_FILL,true);
// 	setDevicePolygonOffset(polygonOffset(),1.0);
// 	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	int idx = 0;
// 	if (plotStyle() != WIREFRAME)
// 	{
// 		glPolygonMode(GL_FRONT_AND_BACK, GL_QUADS);
// 
// 		bool hl = (plotStyle() == HIDDENLINE);
// 		if (hl)
// 		{
// 			RGBA col = backgroundRGBAColor();
// 			glColor4d(col.r, col.g, col.b, col.a);
// 		}
     
/*
  RGBA col(0.8,0,0);    
  glColor4d(col.r, col.g, col.b, col.a);
  glBegin(GL_LINES);
  for (unsigned i=0; i!=data_->edges.size(); ++i)
  {
    Triple& beg = data_->nodes[data_->edges[i].first];
    Triple& end = data_->nodes[data_->edges[i].second];
    glVertex3d( beg.x, beg.y, beg.z );
    glVertex3d( end.x, end.y, end.z );
    //glNormal3d( data_->normals[idx].x, data_->normals[idx].y, data_->normals[idx].z );
  }
  glEnd();

  col = RGBA(0,0,0.5);    
  glColor4d(col.r, col.g, col.b, col.a);
  setDevicePointSize( 4 );
  glBegin(GL_POINTS);
  for (unsigned j=0; j!=data_->nodes.size(); ++j)
  {
    Triple& p = data_->nodes[j];
    glVertex3d( p.x, p.y, p.z );
  }
  glEnd();

*/

  glEnable(GL_POLYGON_SMOOTH);
  glEnable(GL_LINE_SMOOTH);
  Stick s((hull().maxVertex-hull().minVertex).length() / 150, 16);
  s.setColor(RGBA(0.5,0.5,0.5)) ;
  for (unsigned i=0; i!=data_->edges.size(); ++i)
  {
    Triple& beg = data_->nodes[data_->edges[i].first];
    Triple& end = data_->nodes[data_->edges[i].second];
    s.draw(beg,end);
  }

  Ball b((hull().maxVertex-hull().minVertex).length() / 50, 32);
  //static bool ff = true;
  //if (ff)
    b.setColor(RGBA(0.5,0,0));    
  //else
  //  b.setColor(RGBA(0,0.5,0));
  //ff = !ff;
  
  for (unsigned j=0; j!=data_->nodes.size(); ++j)
  {
    b.draw(data_->nodes[j]);
  }

  //	}
}

/*! 
	Convert user defined graph data to internal structure.
	See also Qwt3D::TripleField and Qwt3D::EdgeField
*/
bool StaticGraphPlot::appendDataSet(TripleField const& nodes, EdgeField const& edges)
{	
  delete data_;
  data_ = new StaticGraphData;
  actualData_p = data_;
		
	data_->nodes = nodes;
  data_->edges = edges;
	data_->setHull(Qwt3D::hull(nodes));
	updateData();
	createCoordinateSystem();

	return true;
}	
