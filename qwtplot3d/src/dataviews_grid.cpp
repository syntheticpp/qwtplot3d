#if defined(_MSC_VER) /* MSVC Compiler */
#pragma warning ( disable : 4305 )
#pragma warning ( disable : 4786 )
#endif

#include "qwt_plot3d.h"

using namespace std;
using namespace Qwt3D;

void 
QwtPlot3D::updateGridData()
{
	int i, j;
	RGBA col;
	int cstep = resolution_;
	int rstep = resolution_;
	if (plotStyle() == FILLEDMESH || plotStyle() == WIREFRAME || plotStyle() == HIDDENLINE)
	{
		glColor4d(meshcolor_.r, meshcolor_.g, meshcolor_.b, meshcolor_.a);

		for (i = 0; i < actualGridData_->columns() - cstep; i += cstep) 
		{
			for (j = 0; j < actualGridData_->rows() - rstep; j += rstep) 
			{
				glBegin(GL_LINE_LOOP);
					glVertex3dv(actualGridData_->vertices[i][j]);
					glVertex3dv(actualGridData_->vertices[i+cstep][j]);
					glVertex3dv(actualGridData_->vertices[i+cstep][j+rstep]);
					glVertex3dv(actualGridData_->vertices[i][j+rstep]);
				glEnd();
			}
		}
	}
	
	if (plotStyle() != WIREFRAME)
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glPolygonMode(GL_FRONT_AND_BACK, GL_QUADS);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(polygonOffset_,1.0);
		
		bool hl = (plotStyle() == HIDDENLINE);
		col = bgcolor_;

		glBegin(GL_QUADS);
			for (i = 0; i < actualGridData_->columns() - cstep; i += cstep) 
			{
				for (j = 0; j < actualGridData_->rows() - rstep; j += rstep) 
				{
					if(!hl)
					{
						col = (*dataColor_)(
							actualGridData_->vertices[i][j][0],
							actualGridData_->vertices[i][j][1],
							actualGridData_->vertices[i][j][2]);
					}
					glColor4d(col.r, col.g, col.b, col.a);
					
					glVertex3dv(actualGridData_->vertices[i][j]);
					glVertex3dv(actualGridData_->vertices[i+cstep][j]);
					glVertex3dv(actualGridData_->vertices[i+cstep][j+rstep]);
					glVertex3dv(actualGridData_->vertices[i][j+rstep]);

					glNormal3dv(actualGridData_->normals[i][j]);
					glNormal3dv(actualGridData_->normals[i+cstep][j]);
					glNormal3dv(actualGridData_->normals[i+cstep][j+rstep]);
					glNormal3dv(actualGridData_->normals[i][j+rstep]);
				}
			}
		glEnd();
		glDisable(GL_POLYGON_OFFSET_FILL);
	}
}

void 
QwtPlot3D::GridData2Floor()
{
	if (actualGridData_->empty())
		return;
	
	RGBA col;
	unsigned int cstep = resolution_;
	unsigned int rstep = resolution_;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPolygonMode(GL_FRONT_AND_BACK, GL_QUADS);
	
	glBegin(GL_QUADS);
		double zshift = actualGridData_->minimum();
		for (unsigned int i = 0; i < actualGridData_->columns() - cstep; i += cstep) 
		{
			for (unsigned int j = 0; j < actualGridData_->rows() - rstep; j += rstep) 
			{
				col = (*dataColor_)(
					actualGridData_->vertices[i][j][0],
					actualGridData_->vertices[i][j][1],
					actualGridData_->vertices[i][j][2]);

				glColor4d(col.r, col.g, col.b, col.a);
				
				glVertex3d(actualGridData_->vertices[i][j][0], actualGridData_->vertices[i][j][1], zshift);
				glVertex3d(actualGridData_->vertices[i+cstep][j][0],actualGridData_->vertices[i+cstep][j][1], zshift);
				glVertex3d(actualGridData_->vertices[i+cstep][j+rstep][0],actualGridData_->vertices[i+cstep][j+rstep][1], zshift);
				glVertex3d(actualGridData_->vertices[i][j+rstep][0],actualGridData_->vertices[i][j+rstep][1], zshift);
			}
		}
	glEnd();
}

void 
QwtPlot3D::Grid2Floor()
{
	if (actualGridData_->empty())
		return;
	
	RGBA col;
	unsigned int cstep = resolution_;
	unsigned int rstep = resolution_;
	
	double zshift = actualGridData_->minimum();

	glColor4d(meshcolor_.r, meshcolor_.g, meshcolor_.b, meshcolor_.a);
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
QwtPlot3D::GridIsolines2Floor()
{
	if (isolines_ <= 0 || actualGridData_->empty())
		return;

	double step = (actualGridData_->maximum() - actualGridData_->minimum()) / isolines_;		

	RGBA col;
	int cstep = resolution_;
	int rstep = resolution_;

	double zshift = actualGridData_->minimum();
	
	int cols = actualGridData_->columns();
	int rows = actualGridData_->rows();
	
	Triple t[4];
	vector<Triple> intersection;
	
	int hit = -1;
	double lambda = 0;
	
	for (int k = 0; k != isolines_; ++k) 
	{
		double val = zshift + k * step;		
				
		for (int i = 0; i < cols-cstep; i += cstep) 
		{
			for (int j = 0; j < rows-rstep; j += rstep) 
			{
				t[0] =  Triple(	actualGridData_->vertices[i][j][0],
												actualGridData_->vertices[i][j][1],
												actualGridData_->vertices[i][j][2]);

				col = (*dataColor_)(t[0].x,t[0].y,t[0].z);
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
