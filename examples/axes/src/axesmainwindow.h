#include "qwt3d_surfaceplot.h"
#include "qwt3d_function.h"
#include "qwt3d_plot.h"
#include "axesmainwindowbase.h"


class AxesMainWindow : public AxesMainWindowBase
{
	Q_OBJECT

public:
	AxesMainWindow( QWidget* parent = 0, const char* name = 0, WFlags f = WType_TopLevel );
	~AxesMainWindow();
	Qwt3D::SurfacePlot* plot;
	Qwt3D::Function *rosenbrock;
  void resetTics();

public slots:
	void setNumberGap(int gap);
	void setLabelGap(int gap);

  void setSmoothLines(bool);
  void setTicLength(int val);
  void setTicNumber(int degree);

  void standardItems();
  void complexItems();
  void letterItems();
  void timeItems();
  void customScale();

private:

  int tics;
};
