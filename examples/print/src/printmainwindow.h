#include "qwt3d_surfaceplot.h"
#include "qwt3d_function.h"
#include "qwt3d_plot3d.h"
#include "printMainWindowBase.h"


class printMainWindow : public printMainWindowBase
{
	Q_OBJECT

public:
	printMainWindow( QWidget* parent = 0, const char* name = 0, WFlags f = WType_TopLevel );
	~printMainWindow();
	Qwt3D::SurfacePlot* plot[2];
	Qwt3D::Function *rosenbrock, *hat;

public slots:
	void dumpImage();
	void setFileType(QString const& name);
	void setSortingType(QString const& name);
	void setPolygonOffset(int val);
	void setMeshLineWidth(int val);
	void setCoordLineWidth(int val);

private:
	int sortingtype_;
	QString filetype_;

};

