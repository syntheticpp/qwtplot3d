#include "mesh2mainwindowbase.h"
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
	void pickTitleColor();
	void resetColors();
	void pickNumberFont();
	void pickLabelFont();
	void pickTitleFont();
	void resetFonts();
 	void setStandardView();
	void dumpImage();
	void toggleAnimation(bool);
	void toggleProjectionMode(bool);
	void toggleColorLegend(bool);
	void toggleAutoScale(bool val);
	void rotate();
	void setPolygonOffset(int);
	void createColorLegend(ColorVector const&);

	void showRotate(double x, double y, double z);
	void showShift(double x, double y);
	void showScale(double x, double y, double z);
	void showZoom(double z);

private:
	QLabel *filenameWidget,
		*rotateLabel, *shiftLabel, *scaleLabel, *zoomLabel;
	
	QTimer* timer;
	int redrawWait;

	QAction* activeCoordSystem;

	bool legend_;


};

