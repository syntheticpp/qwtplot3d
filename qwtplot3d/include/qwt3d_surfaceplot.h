#ifndef __surfaceplot_2003_06_09_12_08__
#define __surfaceplot_2003_06_09_12_08__

#include "qwt3d_global.h"
#include "qwt3d_plot.h"

namespace Qwt3D
{
//! Class representing Surfaces like z=f(x,y)
/**
	A SurfacePlot is a plotting widget for three-dimensional data {x,y,f(x,y)}.
	f can be a mathematical function or also a data-driven mapping 
*/
class QWT3D_EXPORT SurfacePlot : public Qwt3D::Plot3D
{
    Q_OBJECT

public:

		SurfacePlot( QWidget* parent = 0, const char* name = 0, Qwt3D::MESHTYPE  = GRID );
    virtual ~SurfacePlot();

		virtual bool hasData() const { return !actualGridData_->empty() || !actualCellData_->empty() ; } //!< \return Valid data available (true) or not (false)
		virtual	void calculateHull();

		bool createDataRepresentation(double** data, unsigned int columns, unsigned int rows
																			,double minx, double maxx, double miny, double maxy);
		
		bool createDataRepresentation(Qwt3D::TripleField const& data, Qwt3D::CellField const& poly, Qwt3D::MESHTYPE mtype = POLYGON);

		void updateNormals();
		void showNormals(bool); //!< draw normals to every vertex
		bool normals() const { return datanormals_;} //!< \return true, if normal drawing is on
		
		void setNormalLength(double val); //!< set length of normals in percent per hull diagonale
		double normalLength() const { return normalLength_; }//!< \return relative length of normals
		void setNormalQuality(int val); //!< increase plotting quality of normal arrows
		double normalQuality() const { return normalQuality_; }//!< \return plotting quality of normal arrows
		int	resolution() const {return resolution_;} //!< \return data resolution (1 means all data)

signals:
		
		void resolutionChanged(int);

public slots:

		void	setResolution( int );

protected:

		//! No copies
		SurfacePlot(SurfacePlot const&);
		SurfacePlot& operator=(SurfacePlot const&);

		virtual void createData();
		virtual void createFloorData();

private:
		void updateGridData();
	  void updateCellData();
		
		void updateGridNormals();
		void GridData2Floor();
		void GridIsolines2Floor();
		void Grid2Floor();
		void updateCellNormals();
		void CellData2Floor();
		void CellIsolines2Floor();
		void Cell2Floor();

		void setColorFromGridVertex(int ix, int iy, bool skip = false);
		void setColorFromCellVertex(int node, bool skip = false);
	
		Qwt3D::MESHTYPE meshtype() const { return meshtype_; }	

		Qwt3D::GridData* actualGridData_;
		Qwt3D::CellData* actualCellData_;
		Qwt3D::MESHTYPE meshtype_;

		bool datanormals_;
		double normalLength_;
		int normalQuality_;

		int resolution_;
};

} // ns

#endif  // __surfaceplot_2003_06_09_12_08__
