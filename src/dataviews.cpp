#if defined(_MSC_VER) /* MSVC Compiler */
#pragma warning ( disable : 4305 )
#pragma warning ( disable : 4786 )
#endif

#include "qwt_plot3d.h"

using namespace Qwt3d;

/*!
  Generate an OpenGL display list for the object to be shown
*/
void 
QwtPlot3D::updateData()
{
	int i, j;
	RGBA col;
	int cstep = resolution_;
	int rstep = resolution_;

	updateFloorData();
	
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LINE_SMOOTH);

	SaveGlDeleteLists(objectList_[DataObject], 1); // nur Daten
	
	if (plotStyle() == NOPLOT)
		return;

	objectList_[DataObject] = glGenLists(1);
	glNewList(objectList_[DataObject], GL_COMPILE);

	if (plotStyle() == FILLEDMESH || plotStyle() == WIREFRAME || plotStyle() == HIDDENLINE)
	{
		glColor4d(meshcolor_.r, meshcolor_.g, meshcolor_.b, meshcolor_.a);
		
		for (i = 0; i < actualData_.columns() - cstep; i += cstep) 
		{
			for (j = 0; j < actualData_.rows() - rstep; j += rstep) 

			{
				glBegin(GL_LINE_LOOP);
					glVertex3dv(actualData_.vertices[i][j]);
					glVertex3dv(actualData_.vertices[i+cstep][j]);
					glVertex3dv(actualData_.vertices[i+cstep][j+rstep]);
					glVertex3dv(actualData_.vertices[i][j+rstep]);
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
			for (i = 0; i < actualData_.columns() - cstep; i += cstep) 
			{
				for (j = 0; j < actualData_.rows() - rstep; j += rstep) 
				{
					if(!hl)
					{
						col = (*dataColor_)(
							actualData_.vertices[i][j][0],
							actualData_.vertices[i][j][1],
							actualData_.vertices[i][j][2]);
					}
					glColor4d(col.r, col.g, col.b, col.a);
					
					glVertex3dv(actualData_.vertices[i][j]);
					glVertex3dv(actualData_.vertices[i+cstep][j]);
					glVertex3dv(actualData_.vertices[i+cstep][j+rstep]);
					glVertex3dv(actualData_.vertices[i][j+rstep]);

					glNormal3dv(actualData_.normals[i][j]);
					glNormal3dv(actualData_.normals[i+cstep][j]);
					glNormal3dv(actualData_.normals[i+cstep][j+rstep]);
					glNormal3dv(actualData_.normals[i][j+rstep]);
				}
			}
		glEnd();
		glDisable(GL_POLYGON_OFFSET_FILL);
	}

	glEndList();
}

void 
QwtPlot3D::updateFloorData()
{
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LINE_SMOOTH);

	SaveGlDeleteLists(objectList_[FloorObject], 1);
	
	if (floorStyle() == NOFLOOR)
		return;
		
	objectList_[FloorObject] = glGenLists(1);
	glNewList(objectList_[FloorObject], GL_COMPILE);

		if (floorstyle_ == FLOORDATA)
			calcFloorListAsData();
		
		else if (floorstyle_ == FLOORISO)
			calcFloorListAsIsolines();
	
	glEndList();
}


void 
QwtPlot3D::calcFloorListAsData()
{
	if (actualData_.empty())
		return;
	
	RGBA col;
	unsigned int cstep = resolution_;
	unsigned int rstep = resolution_;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPolygonMode(GL_FRONT_AND_BACK, GL_QUADS);
	
	glBegin(GL_QUADS);
		double zshift = actualData_.minimum();
		for (unsigned int i = 0; i < actualData_.columns() - cstep; i += cstep) 
		{
			for (unsigned int j = 0; j < actualData_.rows() - rstep; j += rstep) 
			{
				col = (*dataColor_)(
					actualData_.vertices[i][j][0],
					actualData_.vertices[i][j][1],
					actualData_.vertices[i][j][2]);

				glColor4d(col.r, col.g, col.b, col.a);
				
				glVertex3d(actualData_.vertices[i][j][0], actualData_.vertices[i][j][1], zshift);
				glVertex3d(actualData_.vertices[i+cstep][j][0],actualData_.vertices[i+cstep][j][1], zshift);
				glVertex3d(actualData_.vertices[i+cstep][j+rstep][0],actualData_.vertices[i+cstep][j+rstep][1], zshift);
				glVertex3d(actualData_.vertices[i][j+rstep][0],actualData_.vertices[i][j+rstep][1], zshift);
			}
		}
	glEnd();
}

void 
QwtPlot3D::calcFloorListAsIsolines()
{
	if (isolines_ <= 0 || actualData_.empty())
		return;

	double step = (actualData_.maximum() - actualData_.minimum()) / isolines_;		

	RGBA col;
	int cstep = resolution_;
	int rstep = resolution_;

//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	unsigned i,j;
	double zshift = actualData_.minimum();
	int cols = actualData_.columns();
	int rows = actualData_.rows();
	
	for (int k = isolines_; k > 0; --k) 
	{
		double hi = actualData_.minimum() + k * step;
		double lo = hi - step;
		

		for (i = 0; i <= cols-cstep; i += cstep) 
		{
			for (j = 0; j <= rows-rstep; j += rstep) 
			{
				col = (*dataColor_)(
					actualData_.vertices[i][j][0],
					actualData_.vertices[i][j][1],
					actualData_.vertices[i][j][2]);

				glColor4d(col.r, col.g, col.b, col.a);
				
				Triple thi = Triple(	actualData_.vertices[i][j][0],
															actualData_.vertices[i][j][1],
															actualData_.vertices[i][j][2]);
				if ( lo<thi.z && thi.z<hi)
				{
					Triple tlo[8];
					
					if (i<cols-cstep)
					{
						tlo[0] = Triple(actualData_.vertices[i+cstep][j][0],
							actualData_.vertices[i+cstep][j][1],
							actualData_.vertices[i+cstep][j][2]);
					}
					if (i<cols-cstep && j<rows-rstep)
					{
						tlo[1] = Triple(actualData_.vertices[i+cstep][j+rstep][0],
							actualData_.vertices[i+cstep][j+rstep][1],
							actualData_.vertices[i+cstep][j+rstep][2]);
					}
					if (j<rows-rstep)
					{
						tlo[2] = Triple(actualData_.vertices[i][j+rstep][0],
														actualData_.vertices[i][j+rstep][1],
														actualData_.vertices[i][j+rstep][2]);
					}
					if (i>0 && j<rows-rstep)
					{
						tlo[3] = Triple(actualData_.vertices[i-cstep][j+rstep][0],
														actualData_.vertices[i-cstep][j+rstep][1],
														actualData_.vertices[i-cstep][j+rstep][2]);
					}
					if (i>0)
					{
						tlo[4] = Triple(actualData_.vertices[i-cstep][j][0],
														actualData_.vertices[i-cstep][j][1],
														actualData_.vertices[i-cstep][j][2]);
					}
					if (i>0 && j>0)
					{
						tlo[5] = Triple(actualData_.vertices[i-cstep][j-rstep][0],
							actualData_.vertices[i-cstep][j-rstep][1],
							actualData_.vertices[i-cstep][j-rstep][2]);
					}
					if (j>0)
					{
						tlo[6] = Triple(actualData_.vertices[i][j-rstep][0],
							actualData_.vertices[i][j-rstep][1],
							actualData_.vertices[i][j-rstep][2]);
					}
					if (i<cols-cstep && j>0)
					{
						tlo[7] = Triple(actualData_.vertices[i+cstep][j-rstep][0],
														actualData_.vertices[i+cstep][j-rstep][1],
														actualData_.vertices[i+cstep][j-rstep][2]);
					}										
					
					unsigned k1,k2;
					
					if (i>0 && i<cols-cstep && j>0 && j<rows-rstep)
					{
						k1 = 0;
						k2 = 7;
					}
					else if (i==0 && j>0 && j<rows-rstep)
					{
						tlo[0] = Triple(actualData_.vertices[i][j-rstep][0],
							actualData_.vertices[i][j-rstep][1],
							actualData_.vertices[i][j-rstep][2]);
						tlo[1] = Triple(actualData_.vertices[i+cstep][j-rstep][0],
							actualData_.vertices[i+cstep][j-rstep][1],
							actualData_.vertices[i+cstep][j-rstep][2]);
						tlo[2] = Triple(actualData_.vertices[i+cstep][j][0],
							actualData_.vertices[i+cstep][j][1],
							actualData_.vertices[i+cstep][j][2]);
						tlo[3] = Triple(actualData_.vertices[i+cstep][j+rstep][0],
							actualData_.vertices[i+cstep][j+rstep][1],
							actualData_.vertices[i+cstep][j+rstep][2]);
						tlo[4] = Triple(actualData_.vertices[i][j+rstep][0],
							actualData_.vertices[i][j+rstep][1],
							actualData_.vertices[i][j+rstep][2]);
						k1 = 0;
						k2 = 3;
					}
					else if (i>0 && i<cols-cstep && j==rows-rstep)
					{
						k1 = 4;
						k2 = 7;
					}
					else if (i==cols-cstep && j>0 && j<rows-rstep)
					{
						k1 = 2;
						k2 = 5;
					}
					else if (i>0 && i<cols-cstep && j==0)
					{
						k1 = 0;
						k2 = 3;
					}
					else if (i==0 && j==0)
					{
						k1 = 0;
						k2 = 1;
					}
					else if (i==0 && j==rows-rstep)
					{
						k1 = 6;
						k2 = 7;
					}
					else if (i==cols-cstep && j==rows-rstep)
					{
						k1 = 4;
						k2 = 5;
					}
					else if (i==cols-cstep && j==0)
					{
						k1 = 2;
						k2 = 3;
					}
					else
					{
						k1 = 0;
						k2 = 7;
					}
					Triple tlo0 = tlo[0]; // remember for last step in while

					int k = k1;
					while (k1<=k && k<=k2)
					{
						tlo[0] = tlo[k];
						tlo[1] = (k<7) ? tlo[k+1] : tlo0;
						
						Triple rp1, rp2;
						if (tlo[0].z <= lo)
						{
							rp1 = tlo[0] + ((lo - tlo[0].z) / (thi.z - tlo[0].z)) * (thi-tlo[0]);
							if (tlo[1].z <= lo)
								rp2 = tlo[1] + ((lo - tlo[1].z) / (thi.z - tlo[1].z)) * (thi-tlo[1]);
							else
								rp2 = tlo[0] + ((lo - tlo[0].z) / (tlo[1].z - tlo[0].z)) * (tlo[1]-tlo[0]);
							glBegin(GL_LINES);
								glVertex3d(rp1.x,rp1.y,zshift);
								glVertex3d(rp2.x,rp2.y,zshift);
//								glVertex3d(rp1.x,rp1.y,rp1.z);
//								glVertex3d(rp2.x,rp2.y,rp2.z);
							glEnd();
						}
						else if (tlo[1].z <= lo)
						{
							rp1 = tlo[1] + ((lo - tlo[1].z) / (thi.z - tlo[1].z)) * (thi-tlo[1]);
							rp2 = tlo[1] + ((lo - tlo[1].z) / (tlo[0].z - tlo[1].z)) * (tlo[0]-tlo[1]);
							glBegin(GL_LINES);
								glVertex3d(rp1.x,rp1.y,zshift);
								glVertex3d(rp2.x,rp2.y,zshift);
//								glVertex3d(rp1.x,rp1.y,rp1.z);
//								glVertex3d(rp2.x,rp2.y,rp2.z);
							glEnd();
						}
						++k;
					}
				}
			}
		}
	}
}
