#include "qwt3d_plot3d.h"
#include "mesh2mainwindowbase.h"


class QLabel;
class QTimer;
class QAction;
class QFileDialog;
class ColorMapPreview;

class Mesh2MainWindow : public Mesh2MainWindowBase
{
	Q_OBJECT
public:
	Mesh2MainWindow( QWidget* parent = 0, const char* name = 0, WFlags f = WType_TopLevel );
	~Mesh2MainWindow();
	
public slots:
	void open();
	void openMesh();

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
	void pickDataColor();
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
	void toggleShader(bool val);
	void rotate();
	void setPolygonOffset(int);
	void createColorLegend(Qwt3D::ColorVector const&);

	void showRotate(double x, double y, double z);
	void showShift(double x, double y);
	void showScale(double x, double y, double z);
	void showZoom(double z);
	void showNormals(bool val);
	void setNormalQuality(int);
	void setNormalLength(int);
	bool openColorMap(Qwt3D::ColorVector& cv, QString fname);
	void adaptDataColors(const QString&);
	void updateColorLegend(int majors, int minors);

private:
	QLabel *filenameWidget,
		*rotateLabel, *shiftLabel, *scaleLabel, *zoomLabel;
	
	QTimer* timer;
	int redrawWait;

	QAction* activeCoordSystem;

	bool legend_;
	Qwt3D::StandardColor* col_;

	QFileDialog* datacolordlg_;
	ColorMapPreview* colormappv_; 
};

