#if defined(_MSC_VER) /* MSVC Compiler */
#pragma warning ( disable : 4305 )
#pragma warning ( disable : 4786 )
#endif

#include "qwt_plot3d.h"
#include "vectorfield.h"

using namespace std;
using namespace Qwt3D;

void 
Plot3D::updateCellData()
{
	int idx = 0;
	if (plotStyle() == FILLEDMESH || plotStyle() == WIREFRAME || plotStyle() == HIDDENLINE)
	{
		glColor4d(meshcolor_.r, meshcolor_.g, meshcolor_.b, meshcolor_.a);
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
	
	RGBA col;
	if (plotStyle() != WIREFRAME)
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glPolygonMode(GL_FRONT_AND_BACK, GL_QUADS);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(polygonOffset_,1.0);
		
		bool hl = (plotStyle() == HIDDENLINE);
		col = bgcolor_;

		glColor4d(meshcolor_.r, meshcolor_.g, meshcolor_.b, meshcolor_.a);
		{
			for (unsigned i=0; i!=actualCellData_->cells.size(); ++i)
			{
				if(!hl)
				{
					idx = actualCellData_->cells[i][0];
					col = (*dataColor_)(
						actualCellData_->nodes[idx].x, actualCellData_->nodes[idx].y, actualCellData_->nodes[idx].z);
				}
				glColor4d(col.r, col.g, col.b, col.a);
				glBegin(GL_POLYGON);
				for (unsigned j=0; j!=actualCellData_->cells[i].size(); ++j)
				{
					idx = actualCellData_->cells[i][j];
					glVertex3d( actualCellData_->nodes[idx].x, actualCellData_->nodes[idx].y, actualCellData_->nodes[idx].z );
					glNormal3d( actualCellData_->normals[idx].x, actualCellData_->normals[idx].y, actualCellData_->normals[idx].z );
				}
				glEnd();
			}
		}
		glDisable(GL_POLYGON_OFFSET_FILL);
	}
}

void 
Plot3D::CellData2Floor()
{	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPolygonMode(GL_FRONT_AND_BACK, GL_QUADS);
	
	double zshift = actualCellData_->minimum();
	int idx;

	RGBA col;
	{
		for (unsigned i = 0; i!=actualCellData_->cells.size(); ++i)
		{
			idx = actualCellData_->cells[i][0];
			col = (*dataColor_)(
				actualCellData_->nodes[idx].x, actualCellData_->nodes[idx].y, actualCellData_->nodes[idx].z);
			glColor4d(col.r, col.g, col.b, col.a);
			glBegin(GL_POLYGON);
			for (unsigned j=0; j!=actualCellData_->cells[i].size(); ++j)
			{
				idx = actualCellData_->cells[i][j];
				glVertex3d( actualCellData_->nodes[idx].x, actualCellData_->nodes[idx].y, zshift );
			}
			glEnd();
		}
	}
}


void 
Plot3D::Cell2Floor()
{
	if (actualCellData_->empty() || meshtype() == GRID)
		return;
	
	double zshift = actualCellData_->minimum();

	glColor4d(meshcolor_.r, meshcolor_.g, meshcolor_.b, meshcolor_.a);
	for (unsigned i=0; i!=actualCellData_->cells.size(); ++i)
	{
		glBegin(GL_LINE_LOOP);
		for (unsigned j=0; j!=actualCellData_->cells[i].size(); ++j)
		{
			int idx = actualCellData_->cells[i][j];
			glVertex3d( actualCellData_->nodes[idx].x, actualCellData_->nodes[idx].y, zshift );
		}
		glEnd();
	}
}

void 
Plot3D::CellIsolines2Floor()
{
	if (isolines_ <= 0 || actualCellData_->empty())
		return;

	double step = (actualCellData_->maximum() - actualCellData_->minimum()) / isolines_;		

	RGBA col;

	double zshift = actualCellData_->minimum();
		
	TripleVector nodes;
	TripleVector intersection;
	
	int hit = -1;
	double lambda = 0;
	
	for (int k = 0; k != isolines_; ++k) 
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
				col = (*dataColor_)(nodes[0].x,nodes[0].y,nodes[0].z);
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
Plot3D::updateCellNormals()
{
	if (!normals() || actualCellData_->empty())
		return;

	if (actualCellData_->nodes.size() != actualCellData_->normals.size())
		return;

	VectorField v(dataColor_);
	v.setQuality(normalQuality());
	v.bases = TripleVector(actualCellData_->normals.size());
	v.tops = TripleVector(v.bases.size());
	
	unsigned t = v.bases.size(); 

	Triple basev;
	Triple topv;	
	
	double diag = (actualCellData_->hull().maxVertex-actualCellData_->hull().minVertex).length() * normalLength();

	for (unsigned i = 0; i != v.bases.size(); ++i) 
	{
		basev = actualCellData_->nodes[i];
		topv = basev + actualCellData_->normals[i];
		
		Triple norm = (topv-basev);
		norm.normalize();
		norm	*= diag;

		v.bases[i] = basev;		
		v.tops[i] = basev + norm;	
	}
	v.drawArrows();
}