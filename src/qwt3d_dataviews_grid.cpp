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
SurfacePlot::updateGridData()
{
	int i, j;
	RGBA col;
	const int cstep = resolution();
	const int rstep = resolution();
	
	GLStateBewarer sb(GL_POLYGON_OFFSET_FILL,true);
	setDevicePolygonOffset(polygonOffset(),1.0);

	//glEnable(GL_LINE_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (plotStyle() != WIREFRAME)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_QUADS);
		
		bool hl = (plotStyle() == HIDDENLINE);
		if (hl)
		{
			col = backgroundRGBAColor();
			glColor4d(col.r, col.g, col.b, col.a);
		}

		for (i = 0; i < actualGridData_->columns() - cstep; i += cstep) 
		{
		  glBegin(GL_TRIANGLE_STRIP);
				setColorFromGridVertex(i, 0, hl);
				glNormal3dv(actualGridData_->normals[i][0]);
				glVertex3dv(actualGridData_->vertices[i][0]);
					
				setColorFromGridVertex(i+cstep, 0, hl);
				glNormal3dv(actualGridData_->normals[i+cstep][0]);
				glVertex3dv(actualGridData_->vertices[i+cstep][0]);

				for (j = 0; j < actualGridData_->rows() - rstep; j += rstep) 
				{				
					setColorFromGridVertex(i,j+rstep, hl);
					glNormal3dv(actualGridData_->normals[i][j+rstep]);
					glVertex3dv(actualGridData_->vertices[i][j+rstep]);

					setColorFromGridVertex(i+cstep, j+rstep, hl);
					glNormal3dv(actualGridData_->normals[i+cstep][j+rstep]);
					glVertex3dv(actualGridData_->vertices[i+cstep][j+rstep]);
				}
			glEnd();
		}
	}

	if (plotStyle() == FILLEDMESH || plotStyle() == WIREFRAME || plotStyle() == HIDDENLINE)
	{
		glColor4d(meshColor().r, meshColor().g, meshColor().b, meshColor().a);		

		// utmost frame
		glBegin(GL_LINE_LOOP);
			for (i = 0; i < actualGridData_->columns() - cstep; i += cstep) 
				glVertex3dv(actualGridData_->vertices[i][0]);		
			for (j = 0; j < actualGridData_->rows() - rstep; j += rstep) 
				glVertex3dv(actualGridData_->vertices[i][j]);			
			for (; i > 0; i -= cstep) 
				glVertex3dv(actualGridData_->vertices[i][j]);			
			for (; j > 0; j -= rstep) 
				glVertex3dv(actualGridData_->vertices[0][j]);			
		glEnd();

		// weaving
		for (i = cstep; i < actualGridData_->columns() - cstep; i += cstep) 
		{		
			glBegin(GL_LINE_STRIP);
				for (j = 0; j < actualGridData_->rows(); j += rstep) 
					glVertex3dv(actualGridData_->vertices[i][j]);			
			glEnd();
		}
		for (j = rstep; j < actualGridData_->rows() - rstep; j += rstep) 
		{		
			glBegin(GL_LINE_STRIP);
				for (i = 0; i < actualGridData_->columns(); i += cstep) 
					glVertex3dv(actualGridData_->vertices[i][j]);			
			glEnd();
		}
	}
}

void
SurfacePlot::setColorFromGridVertex(int ix, int iy, bool skip)
{
		if (skip)
			return;

		RGBA col = (*dataColor)(
			actualGridData_->vertices[ix][iy][0],
			actualGridData_->vertices[ix][iy][1],
			actualGridData_->vertices[ix][iy][2]);
			
		glColor4d(col.r, col.g, col.b, col.a);
}

void 
SurfacePlot::GridData2Floor()
{
	if (actualGridData_->empty())
		return;
	
	RGBA col;
	unsigned int cstep = resolution();
	unsigned int rstep = resolution();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPolygonMode(GL_FRONT_AND_BACK, GL_QUADS);
	
	double zshift = actualGridData_->minimum();
	for (unsigned int i = 0; i < actualGridData_->columns() - cstep; i += cstep) 
	{
		glBegin(GL_TRIANGLE_STRIP);
  		setColorFromGridVertex(i, 0);
			glVertex3d(actualGridData_->vertices[i][0][0], actualGridData_->vertices[i][0][1], zshift);
			
			setColorFromGridVertex(i+cstep, 0);
			glVertex3d(actualGridData_->vertices[i+cstep][0][0],actualGridData_->vertices[i+cstep][0][1], zshift);
			for (unsigned int j = 0; j < actualGridData_->rows() - rstep; j += rstep) 
			{
				setColorFromGridVertex(i, j+rstep);
				glVertex3d(actualGridData_->vertices[i][j+rstep][0],actualGridData_->vertices[i][j+rstep][1], zshift);
				
				setColorFromGridVertex(i+cstep, j+rstep);
				glVertex3d(actualGridData_->vertices[i+cstep][j+rstep][0],actualGridData_->vertices[i+cstep][j+rstep][1], zshift);				
			}
		glEnd();
	}
}

void 
SurfacePlot::Grid2Floor()
{
	if (actualGridData_->empty())
		return;
	
	unsigned int cstep = resolution();
	unsigned int rstep = resolution();
	
	double zshift = actualGridData_->minimum();

	glColor4d(meshColor().r, meshColor().g, meshColor().b, meshColor().a);
	for (unsigned int i = 0; i < actualGridData_->columns() - cstep; i += cstep) 
	{
		for (unsigned int j = 0; j < actualGridData_->rows() - rstep; j += rstep) 
		{
			glBegin(GL_LINE_LOOP);
				glVertex3d(actualGridData_->vertices[i][j][0], actualGridData_->vertices[i][j][1], zshift);
				glVertex3d(actualGridData_->vertices[i+cstep][j][0],actualGridData_->vertices[i+cstep][j][1], zshift);
				glVertex3d(actualGridData_->vertices[i+cstep][j+rstep][0],actualGridData_->vertices[i+cstep][j+rstep][1], zshift);
				glVertex3d(actualGridData_->vertices[i][j+rstep][0],actualGridData_->vertices[i][j+rstep][1], zshift);
			glEnd();
		}
	}
}


void 
SurfacePlot::GridIsolines2Floor()
{
	if (isolines() <= 0 || actualGridData_->empty())
		return;

	double step = (actualGridData_->maximum() - actualGridData_->minimum()) / isolines();		

	RGBA col;
	int cstep = resolution();
	int rstep = resolution();

	double zshift = actualGridData_->minimum();
	
	int cols = actualGridData_->columns();
	int rows = actualGridData_->rows();
	
	Triple t[4];
	vector<Triple> intersection;
	
	int hit = -1;
	double lambda = 0;
	
	for (int k = 0; k != isolines(); ++k) 
	{
		double val = zshift + k * step;		
				
		for (int i = 0; i < cols-cstep; i += cstep) 
		{
			for (int j = 0; j < rows-rstep; j += rstep) 
			{
				t[0] =  Triple(	actualGridData_->vertices[i][j][0],
												actualGridData_->vertices[i][j][1],
												actualGridData_->vertices[i][j][2]);

				col = (*dataColor)(t[0].x,t[0].y,t[0].z);
  			glColor4d(col.r, col.g, col.b, col.a);
//  			glColor4d(0,0,0,1);
				
				t[1] =  Triple(	actualGridData_->vertices[i+cstep][j][0],
												actualGridData_->vertices[i+cstep][j][1],
												actualGridData_->vertices[i+cstep][j][2]);
				t[2] =  Triple(	actualGridData_->vertices[i+cstep][j+rstep][0],
												actualGridData_->vertices[i+cstep][j+rstep][1],
												actualGridData_->vertices[i+cstep][j+rstep][2]);
				t[3] =  Triple(	actualGridData_->vertices[i][j+rstep][0],
												actualGridData_->vertices[i][j+rstep][1],
												actualGridData_->vertices[i][j+rstep][2]);

				double diff = 0;
				for (int m = 0; m!=4; ++m)
				{
					int mm = (m+1)%4;
					if ((val>=t[m].z && val<=t[mm].z) || (val>=t[mm].z && val<=t[m].z))
					{
						diff = t[mm].z - t[m].z;
						
						if (isPracticallyZero(diff)) // degenerated
						{
							intersection.push_back(t[m]);
							intersection.push_back(t[mm]);
							continue;
						}
						
						lambda =  (val - t[m].z) / diff;
						intersection.push_back(Triple(t[m].x + lambda * (t[mm].x-t[m].x), t[m].y + lambda * (t[mm].y-t[m].y), val));
					}
				}
				
				if (!intersection.empty())
				{
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
}


void 
SurfacePlot::updateGridNormals()
{
	if (!normals() || actualGridData_->empty())
		return;

	VectorField v(dataColor);
	v.setQuality(normalQuality());
	v.elements = FreeVectorField(actualGridData_->columns()*actualGridData_->rows());

	Triple basev;
	Triple topv;	
	
	int cstep = resolution();
	int rstep = resolution();

	double diag = (actualGridData_->hull().maxVertex-actualGridData_->hull().minVertex).length() * normalLength();

	for (int i = 0; i <= actualGridData_->columns() - cstep; i += cstep) 
	{
		for (int j = 0; j <= actualGridData_->rows() - rstep; j += rstep) 
		{
			basev = Triple(actualGridData_->vertices[i][j][0],actualGridData_->vertices[i][j][1],actualGridData_->vertices[i][j][2]);
			topv = Triple(actualGridData_->vertices[i][j][0]+actualGridData_->normals[i][j][0],
							 actualGridData_->vertices[i][j][1]+actualGridData_->normals[i][j][1],
							 actualGridData_->vertices[i][j][2]+actualGridData_->normals[i][j][2]);	
			
			Triple norm = (topv-basev);
			norm.normalize();
			norm	*= diag;

			v.elements[i * actualGridData_->rows() +j].base 
				= basev;		
			v.elements[i * actualGridData_->rows() +j].top 
				= basev + norm;	
		}
	}
	v.drawArrows();
}