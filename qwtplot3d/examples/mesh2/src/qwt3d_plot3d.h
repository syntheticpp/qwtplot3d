#include "../../../include/qwt3d_surfaceplot.h"

class Qwt3D_Plot3D : public Qwt3D::SurfacePlot
{

public:
	Qwt3D_Plot3D( QWidget* parent = 0, const char* name = 0, Qwt3D::MESHTYPE  mt = Qwt3D::GRID )
	: SurfacePlot( parent, name, mt )
	{
	}
};
