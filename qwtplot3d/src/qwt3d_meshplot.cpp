#if defined(_MSC_VER) /* MSVC Compiler */
#pragma warning ( disable : 4305 )
#pragma warning ( disable : 4786 )
#endif

#include "qwt3d_meshplot.h"
#include "qwt3d_enrichment_std.h"

using namespace std;
using namespace Qwt3D;

Triple const& MeshPlot::MeshData::operator()(unsigned cellnumber, unsigned vertexnumber)
{
	return nodes[cells[cellnumber][vertexnumber]];
}

MeshPlot::MeshData::~MeshData()
{
	setHull(ParallelEpiped());
  cells.clear();
	nodes.clear();
	normals.clear();
}


/**
Initializes with dataNormals()==false, NOFLOOR, resolution() == 1
*/
MeshPlot::MeshPlot( QWidget * parent, const QGLWidget * shareWidget)
    : SurfacePlot( parent, shareWidget) 
{
  data_ = new MeshData();

  actualData_p = data_;
}

MeshPlot::~MeshPlot()
{
  delete data_;
}

// ci = cell index
// cv = vertex index in cell ci
void MeshPlot::setColorFromVertex(int node, bool skip)
{
  if (skip)
    return;

  RGBA col = (*datacolor_p)(
    data_->nodes[node].x, data_->nodes[node].y, data_->nodes[node].z);

  glColor4d(col.r, col.g, col.b, col.a);
}


void MeshPlot::data2Floor()
{	
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  double zshift = data_->hull().minVertex.z;
  int idx;

  for (unsigned i = 0; i!=data_->cells.size(); ++i)
  {
    glBegin(GL_POLYGON);
    for (unsigned j=0; j!=data_->cells[i].size(); ++j)
    {
      idx = data_->cells[i][j];
      setColorFromVertex(idx);
      glVertex3d( data_->nodes[idx].x, data_->nodes[idx].y, zshift );
    }
    glEnd();
  }
}

void MeshPlot::isolines2Floor()
{
  if (isolines() <= 0 || actualData_p->empty())
    return;

  double step = (actualData_p->hull().maxVertex.z - actualData_p->hull().minVertex.z) / isolines();		
  double zshift = actualData_p->hull().minVertex.z;

  TripleField nodes;
  TripleField intersection;

  double lambda = 0;

  GLStateBewarer sb2(GL_LINE_SMOOTH, false);

  for (int k = 0; k != isolines(); ++k) 
  {
    double val = zshift + k * step;		

    for (unsigned i=0; i!=data_->cells.size(); ++i)
    {
      nodes.clear();
      unsigned cellnodes = data_->cells[i].size();
      for (unsigned j=0; j!=cellnodes; ++j)
      {
        nodes.push_back(data_->nodes[data_->cells[i][j]]);
      }

      double diff = 0;
      for (unsigned m = 0; m!=cellnodes; ++m)
      {
        unsigned mm = (m+1)%cellnodes;
        if ((val>=nodes[m].z && val<=nodes[mm].z) || (val>=nodes[mm].z && val<=nodes[m].z))
        {
          diff = nodes[mm].z - nodes[m].z;

          if (isPracticallyZero(diff)) // degenerated
          {
            intersection.push_back(nodes[m]);
            intersection.push_back(nodes[mm]);
            continue;
          }

          lambda =  (val - nodes[m].z) / diff;
          //intersection.push_back(Triple(nodes[m].x + lambda * (nodes[mm].x-nodes[m].x), nodes[m].y + lambda * (nodes[mm].y-nodes[m].y), val));
          intersection.push_back(Triple(nodes[m].x + lambda * (nodes[mm].x-nodes[m].x), nodes[m].y + lambda * (nodes[mm].y-nodes[m].y), zshift));
        }
      }
      drawIntersection(intersection, (*datacolor_p)(nodes[0].x,nodes[0].y,nodes[0].z));
      intersection.clear();
    }
  }
}

void MeshPlot::createNormals()
{
  if (!normals() || actualData_p->empty())
    return;

  if (data_->nodes.size() != data_->normals.size())
    return;
  Arrow arrow;
  arrow.setQuality(normalQuality());

  Triple basev, topv, norm;	

  double diag = (actualData_p->hull().maxVertex-actualData_p->hull().minVertex).length() * normalLength();

  RGBA col;
  arrow.assign(*this);
  arrow.drawBegin();
  for (unsigned i = 0; i != data_->normals.size(); ++i) 
  {
    basev = data_->nodes[i];
    topv = basev + data_->normals[i];

    norm = topv-basev;
    norm.normalize();
    norm	*= diag;

    arrow.setTop(basev+norm);
    arrow.setColor((*datacolor_p)(basev.x,basev.y,basev.z));
    arrow.draw(basev);
  }
  arrow.drawEnd();
}

/*! 
Convert user (non-rectangular) mesh based data to internal structure.
See also Qwt3D::TripleField and Qwt3D::CellField
*/
bool MeshPlot::appendDataSet(TripleField const& data, CellField const& poly)
{	
  delete actualData_p;
  data_ = new MeshData;
  actualData_p = data_;

  data_->nodes = data;
  data_->cells = poly;
  data_->normals = TripleField(data_->nodes.size());

  unsigned i;

  //  normals for the moment
  Triple n, u, v;
  for ( i = 0; i < poly.size(); ++i) 
  {
    if (poly[i].size() < 3)
      n = Triple(0,0,0);
    else
    {
      for (unsigned j = 0; j < poly[i].size(); ++j) 
      {
        unsigned jj = (j+1) % poly[i].size(); 
        unsigned pjj = (j) ? j-1 : poly[i].size()-1;
        u = data_->nodes[poly[i][jj]]-data_->nodes[poly[i][j]];		
        v = data_->nodes[poly[i][pjj]]-data_->nodes[poly[i][j]];
        n = normalizedcross(u,v);
        data_->normals[poly[i][j]] += n;
      }
    }
  }
  for ( i = 0; i != data_->normals.size(); ++i) 
  {
    data_->normals[i].normalize();
  }  

  data_->setHull(Qwt3D::hull(data));

  updateData();
  updateNormals();
  createCoordinateSystem();

  return true;
}	

void MeshPlot::createOpenGlData()
{
  if (!actualData_p)
    return;
    
  createFloorOpenGlData();

  if (plotStyle() == NOPLOT)
    return;

  if (plotStyle() == Qwt3D::POINTS)
  {
    Dot dot;
    drawEnrichment(dot);
    return;
  }
  else if (plotStyle() == Qwt3D::USER)
  {
    if (userplotstyle_p)
      drawEnrichment(*userplotstyle_p);
    return;
  }

  setDeviceLineWidth(meshLineWidth());
  GLStateBewarer sb(GL_POLYGON_OFFSET_FILL,true);
  setDevicePolygonOffset(polygonOffset(),1.0);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  int idx = 0;
  if (plotStyle() != WIREFRAME)
  {
    glPolygonMode(GL_FRONT_AND_BACK, GL_QUADS);

    bool hl = (plotStyle() == HIDDENLINE);
    if (hl)
    {
      RGBA col = backgroundRGBAColor();
      glColor4d(col.r, col.g, col.b, col.a);
    }

    for (unsigned i=0; i!=data_->cells.size(); ++i)
    {
      glBegin(GL_POLYGON);
      for (unsigned j=0; j!=data_->cells[i].size(); ++j)
      {
        idx = data_->cells[i][j];
        setColorFromVertex(idx, hl);
        glVertex3d( data_->nodes[idx].x, data_->nodes[idx].y, data_->nodes[idx].z );
        glNormal3d( data_->normals[idx].x, data_->normals[idx].y, data_->normals[idx].z );
      }
      glEnd();
    }
  }

  if (plotStyle() == FILLEDMESH || plotStyle() == WIREFRAME || plotStyle() == HIDDENLINE)
  {
    glColor4d(meshColor().r, meshColor().g, meshColor().b, meshColor().a);
    {
      for (unsigned i=0; i!=data_->cells.size(); ++i)
      {
        glBegin(GL_LINE_LOOP);
        for (unsigned j=0; j!=data_->cells[i].size(); ++j)
        {
          idx = data_->cells[i][j];
          glVertex3d( data_->nodes[idx].x, data_->nodes[idx].y, data_->nodes[idx].z );
        }
        glEnd();
      }
    }
  }
}

void MeshPlot::drawEnrichment(Enrichment& p)
{
  if (!actualData_p)
    return;  

  switch(p.type()) {
  case Enrichment::USERENRICHMENT:
    {
      p.assign(*this);
      UserEnrichment* ue = (UserEnrichment*)&p; 
      ue->drawBegin();
      ue->draw();
      ue->drawEnd();
    }
    break;
  case Enrichment::VERTEXENRICHMENT:
    {
      p.assign(*this);
      VertexEnrichment* ve = (VertexEnrichment*)&p; 
      ve->drawBegin();
      for (unsigned i = 0; i != data_->normals.size(); ++i) 
        ve->draw(data_->nodes[i]);
      ve->drawEnd(); 
    }
    break;
  case Enrichment::EDGEENRICHMENT:
    break;
  default:
    break; //todo
  }
}

/**
\return Number of cells
*/
int MeshPlot::facets() const
{
  if (!hasData())
    return 0;

  return int(data_->cells.size());
}
