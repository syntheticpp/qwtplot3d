/****************************************************************************
** Form interface generated from reading ui file 'src\mesh2mainwindowbase.ui'
**
** Created: Di 15. Apr 08:54:16 2003
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef MESH2MAINWINDOWBASE_H
#define MESH2MAINWINDOWBASE_H

#include <qvariant.h>
#include <qmainwindow.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QAction;
class QActionGroup;
class QToolBar;
class QPopupMenu;
class QCheckBox;
class QComboBox;
class QFrame;
class QLCDNumber;
class QLabel;
class QPushButton;
class QwtPlot3D;
class QwtSlider;
class QwtWheel;
class alphadlg;

class Mesh2MainWindowBase : public QMainWindow
{ 
    Q_OBJECT

public:
    Mesh2MainWindowBase( QWidget* parent = 0, const char* name = 0, WFlags fl = WType_TopLevel );
    ~Mesh2MainWindowBase();

    QFrame* Frame3;
    QwtPlot3D* dataWidget;
    QLabel* TextLabel1;
    QwtSlider* offsetSlider;
    QwtWheel* yS;
    QwtWheel* ySc;
    QwtWheel* yR;
    QLabel* TextLabel1_2;
    QLabel* TextLabel2;
    QCheckBox* projection;
    QCheckBox* colorlegend;
    QCheckBox* autoscale;
    QwtWheel* zR;
    QLabel* TextLabel1_2_2;
    QLabel* TextLabel2_4;
    QwtWheel* zSc;
    QwtWheel* zS;
    QPushButton* normButton;
    QLabel* betaLabel;
    QLCDNumber* LCDNumber2_3;
    QLabel* TextLabel1_4;
    QwtSlider* resSlider;
    QLabel* gammaLabel;
    QLCDNumber* LCDNumber2_2;
    QLabel* alphaLabel;
    QLCDNumber* LCDNumber2_4;
    QLabel* TextLabel1_3_6;
    QLCDNumber* LCDNumber2_6;
    QLabel* TextLabel2_3;
    QwtWheel* zoomWheel;
    QLCDNumber* LCDNumber1;
    QLabel* TextLabel1_3_5;
    QLCDNumber* LCDNumber2_5;
    QLabel* TextLabel2_2;
    QwtWheel* xR;
    QwtWheel* xS;
    QLabel* TextLabel2_2_2;
    QLabel* TextLabel2_2_3;
    QwtWheel* xSc;
    QLCDNumber* LCDNumber2;
    QLabel* TextLabel1_3;
    QComboBox* functionCB;
    QMenuBar *menubar;
    QPopupMenu *file;
    QPopupMenu *PopupMenu_3;
    QPopupMenu *PopupMenu;
    QToolBar *Toolbar;
    QAction* openFile;
    QAction* Exit;
    QActionGroup* coord;
    QAction* Box;
    QAction* Frame;
    QAction* None;
    QAction* animation;
    QAction* dump;
    QActionGroup* plotstyle;
    QAction* wireframe;
    QAction* hiddenline;
    QAction* polygon;
    QAction* filledmesh;
    QAction* nodata;
    QActionGroup* color;
    QAction* axescolor;
    QAction* backgroundcolor;
    QAction* meshcolor;
    QAction* numbercolor;
    QAction* labelcolor;
    QAction* resetcolor;
    QAction* alpha;
    QActionGroup* font;
    QAction* numberfont;
    QAction* labelfont;
    QAction* resetfont;
    QActionGroup* floorstyle;
    QAction* floordata;
    QAction* flooriso;
    QAction* floormesh;
    QAction* floornone;


public slots:
    virtual void showAlphaDlg();
    virtual void init();

protected:
    QGridLayout* Mesh2MainWindowBaseLayout;
    QGridLayout* Frame3Layout;
    QVBoxLayout* Layout19;
    QVBoxLayout* Layout17;
    QGridLayout* Layout11;
    QVBoxLayout* Layout21;
    QVBoxLayout* Layout14;
    QGridLayout* Layout13_2;
    QGridLayout* Layout22;
    QHBoxLayout* Layout5_3;
    QHBoxLayout* l5;
    QHBoxLayout* Layout5_4;
    QGridLayout* Layout24;
    QHBoxLayout* Layout5_6;
    QVBoxLayout* Layout16;
    QGridLayout* Layout17_2;
    QGridLayout* Layout12;
    QGridLayout* Layout11_2;

    alphadlg* alphaDlg;
};

#endif // MESH2MAINWINDOWBASE_H
