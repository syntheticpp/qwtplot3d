#include "qwt3d_surfaceplot.h"
#include "qwt3d_function.h"
#include "qwt3d_plot3d.h"
#include "enrichmentmainwindowbase.h"
#include "enrichments.h"


class enrichmentMainWindow : public enrichmentMainWindowBase
{
	Q_OBJECT

public:
	enrichmentMainWindow( QWidget* parent = 0, const char* name = 0, WFlags f = WType_TopLevel );
	~enrichmentMainWindow();
  void setColor();
  Bar *bar;

public slots:
  void setLevel(int);
  void setWidth(int);
  void barSlot();
 
private:
  double level_, width_;
  
};

