#ifndef qwt3d_data_h__2004_12_20_02_55_begin_guarded_code
#define qwt3d_data_h__2004_12_20_02_55_begin_guarded_code

#include "qwt3d_types.h"

namespace Qwt3D
{

#ifndef QWT3D_NOT_FOR_DOXYGEN

//! Mesh type
enum DATATYPE
{
  GRID,		//!< Rectangular grid
	POLYGON //!< Convex polygon
};	

typedef double *Vertex;
typedef std::vector<Vertex> DataColumn;
typedef std::vector<DataColumn> DataMatrix;


class Data
{
public:
  Qwt3D::DATATYPE datatype;
  Data() {datatype= Qwt3D::POLYGON;}
  virtual ~Data() {}
  virtual void clear() = 0; //!< destroy content
  virtual bool empty() const = 0; //!< no data
  void setHull(Qwt3D::ParallelEpiped const& h) {hull_p = h;}
  Qwt3D::ParallelEpiped const& hull() const {return hull_p;} 

protected:
  Qwt3D::ParallelEpiped hull_p;
};


//! Implements a matrix of z-Values with limit access functions 
class GridData : public Qwt3D::Data
{
public:
  GridData();
	GridData(unsigned int columns, unsigned int rows);//!< see setSize()
  ~GridData() { clear();}

	int columns() const;
	int rows() const;

	void clear(); //!< destroy content
	bool empty() const { return vertices.empty();}
	void setSize(unsigned int columns, unsigned int rows); //!< destroys content and set new size, elements are uninitialized
	
	DataMatrix vertices;		//!< mesh vertices
	DataMatrix normals;		//!< mesh normals
  void setPeriodic(bool u, bool v) {uperiodic_ = u; vperiodic_ = v;}
  bool uperiodic() const {return uperiodic_;} 
  bool vperiodic() const {return vperiodic_;} 
  Triple point(unsigned int col, unsigned int row) const
  {
    return Triple(vertices[col][row][0],vertices[col][row][1],vertices[col][row][2]);
  }

private:
  bool uperiodic_, vperiodic_;
};


//! Implements a graph-like cell structure with limit access functions 
class CellData : public Qwt3D::Data
{
public:
  CellData() {datatype=Qwt3D::POLYGON;}
  ~CellData() { clear();}

	void clear(); //!< destroy content
	bool empty() const { return cells.empty();}
	
	Triple const& operator()(unsigned cellnumber, unsigned vertexnumber);
	
	CellField cells;   //!< polygon/cell mesh 
	TripleField    nodes;
	TripleField    normals; //!< mesh normals
};

#endif // QWT3D_NOT_FOR_DOXYGEN 
  
} //ns

#endif /* include guard */
