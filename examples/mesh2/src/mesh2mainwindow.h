#include "Mesh2MainWindowBase.h"
#include "qwt_plot3d.h"


class QLabel;
class QTimer;
class QAction;

class Mesh2MainWindow : public Mesh2MainWindowBase
{
	Q_OBJECT
public:
	Mesh2MainWindow( QWidget* parent = 0, const char* name = 0, WFlags f = WType_TopLevel );
	~Mesh2MainWindow();
	
public slots:
	void open();

	void createFunction(QString const& name);

	void pickCoordSystem( QAction* );
	void pickPlotStyle( QAction* );
	void pickFloorStyle( QAction* );
	void pickAxesColor();
	void pickBgColor();
	void pickMeshColor();
	void pickNumberColor();
	void pickLabelColor();
	void resetColors();
	void pickNumberFont();
	void pickLabelFont();
	void resetFonts();
 	void setStandardView();
	void showPosition(double,double);
	void dumpImage();
	void toggleAnimation(bool);
	void toggleProjectionMode(bool);
	void toggleColorLegend(bool);
	void toggleAutoScale(bool val);
	void rotate();
	void xRotate(double);
	void yRotate(double);
	void zRotate(double);
	void xShift(double);
	void yShift(double);
	void zShift(double);
	void xScale(double);
	void yScale(double);
	void zScale(double);
	void setResolution(double);
	void setPolygonOffset(double val);
	void createColorLegend(ColorVector const&);

	void setPolygonsAlpha(double d);
	void setMeshAlpha(double d);

private:
	QLabel* positionWidget, *filenameWidget;
	
	QTimer* timer;
	int redrawWait;

	QAction* activeCoordSystem;

	bool legend_;


};

