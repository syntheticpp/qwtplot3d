#if defined(_MSC_VER) /* MSVC Compiler */
#pragma warning ( disable : 4305 )
#pragma warning ( disable : 4786 )
#endif

#include "qwt3d_surfaceplot.h"
#include "qwt3d_vectorfield.h"
#include "qwt3d_gl2ps.h"

using namespace std;
using namespace Qwt3D;

void 
SurfacePlot::updateCellData()
{		
	GLStateBewarer sb(GL_POLYGON_OFFSET_FILL,true);
	int idx = 0;
	if (plotStyle() != WIREFRAME)
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glPolygonMode(GL_FRONT_AND_BACK, GL_QUADS);
		setDevicePolygonOffset(polygonOffset(),1.0);

		bool hl = (plotStyle() == HIDDENLINE);
		if (hl)
		{
			RGBA col = backgroundRGBAColor();
			glColor4d(col.r, col.g, col.b, col.a);
		}
		
		for (unsigned i=0; i!=actualCellData_->cells.size(); ++i)
		{
			glBegin(GL_POLYGON);
			for (unsigned j=0; j!=actualCellData_->cells[i].size(); ++j)
			{
				idx = actualCellData_->cells[i][j];
				setColorFromCellVertex(idx, hl);
				glVertex3d( actualCellData_->nodes[idx].x, actualCellData_->nodes[idx].y, actualCellData_->nodes[idx].z );
				glNormal3d( actualCellData_->normals[idx].x, actualCellData_->normals[idx].y, actualCellData_->normals[idx].z );
			}
			glEnd();
		}
	}

	if (plotStyle() == FILLEDMESH || plotStyle() == WIREFRAME || plotStyle() == HIDDENLINE)
	{
		glColor4d(meshColor().r, meshColor().g, meshColor().b, meshColor().a);
		{
			for (unsigned i=0; i!=actualCellData_->cells.size(); ++i)
			{
				glBegin(GL_LINE_LOOP);
				for (unsigned j=0; j!=actualCellData_->cells[i].size(); ++j)
				{
					idx = actualCellData_->cells[i][j];
					glVertex3d( actualCellData_->nodes[idx].x, actualCellData_->nodes[idx].y, actualCellData_->nodes[idx].z );
				}
				glEnd();
			}
		}
	}

}

// ci = cell index
// cv = vertex index in cell ci
void
SurfacePlot::setColorFromCellVertex(int node, bool skip)
{
	if (skip)
		return;

	RGBA col = (*dataColor)(
		actualCellData_->nodes[node].x, actualCellData_->nodes[node].y, actualCellData_->nodes[node].z);
		
	glColor4d(col.r, col.g, col.b, col.a);
}

void 
SurfacePlot::CellData2Floor()
{	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPolygonMode(GL_FRONT_AND_BACK, GL_QUADS);
	
	double zshift = actualCellData_->minimum();
	int idx;

	for (unsigned i = 0; i!=actualCellData_->cells.size(); ++i)
	{
		glBegin(GL_POLYGON);
		for (unsigned j=0; j!=actualCellData_->cells[i].size(); ++j)
		{
			idx = actualCellData_->cells[i][j];
			setColorFromCellVertex(idx);
			glVertex3d( actualCellData_->nodes[idx].x, actualCellData_->nodes[idx].y, zshift );
		}
		glEnd();
	}
}

void 
SurfacePlot::CellIsolines2Floor()
{
	if (isolines() <= 0 || actualCellData_->empty())
		return;

	double step = (actualCellData_->maximum() - actualCellData_->minimum()) / isolines();		

	RGBA col;

	double zshift = actualCellData_->minimum();
		
	TripleField nodes;
	TripleField intersection;
	
	int hit = -1;
	double lambda = 0;
	
	for (int k = 0; k != isolines(); ++k) 
	{
		double val = zshift + k * step;		
				
		for (unsigned i=0; i!=actualCellData_->cells.size(); ++i)
		{
			nodes.clear();
			unsigned cellnodes = actualCellData_->cells[i].size();
			for (unsigned j=0; j!=cellnodes; ++j)
			{
				nodes.push_back(actualCellData_->nodes[actualCellData_->cells[i][j]]);
			}
			
			double diff = 0;
			for (int m = 0; m!=cellnodes; ++m)
			{
				int mm = (m+1)%cellnodes;
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
					intersection.push_back(Triple(nodes[m].x + lambda * (nodes[mm].x-nodes[m].x), nodes[m].y + lambda * (nodes[mm].y-nodes[m].y), val));
				}
			}

			if (!intersection.empty())
			{
				col = (*dataColor)(nodes[0].x,nodes[0].y,nodes[0].z);
  			glColor4d(col.r, col.g, col.b, col.a);
				if (intersection.size()>2)
				{
					glBegin(GL_LINE_STRIP);
					for (unsigned dd = 0; dd!=intersection.size(); ++dd)
					{
						glVertex3d(intersection[dd].x, intersection[dd].y, zshift);
					}
					glEnd();
					glBegin(GL_POINTS);
						glVertex3d(intersection[0].x,intersection[0].y,zshift);
					glEnd();
				}
				else if (intersection.size() == 2)
				{
					glBegin(GL_LINES);
						glVertex3d(intersection[0].x,intersection[0].y,zshift);
						glVertex3d(intersection[1].x,intersection[1].y,zshift);
						
						// small pixel gap problem (see OpenGL spec.)
						glVertex3d(intersection[1].x,intersection[1].y,zshift);
						glVertex3d(intersection[0].x,intersection[0].y,zshift);
					glEnd();
				}
				
				intersection.clear();
			}
		}
	}
}

void 
SurfacePlot::updateCellNormals()
{
	if (!normals() || actualCellData_->empty())
		return;

	if (actualCellData_->nodes.size() != actualCellData_->normals.size())
		return;

	VectorField v(dataColor);
	v.setQuality(normalQuality());
	v.elements = FreeVectorField(actualCellData_->normals.size());
	
	unsigned t = v.elements.size(); 

	Triple basev;
	Triple topv;	
	
	double diag = (actualCellData_->hull().maxVertex-actualCellData_->hull().minVertex).length() * normalLength();

	for (unsigned i = 0; i != v.elements.size(); ++i) 
	{
		basev = actualCellData_->nodes[i];
		topv = basev + actualCellData_->normals[i];
		
		Triple norm = (topv-basev);
		norm.normalize();
		norm	*= diag;

		v.elements[i].base = basev;		
		v.elements[i].top = basev + norm;	
	}
	v.drawArrows();
}