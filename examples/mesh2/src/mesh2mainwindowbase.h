/****************************************************************************
** Form interface generated from reading ui file 'src\mesh2mainwindowbase.ui'
**
** Created: Di 29. Apr 00:50:51 2003
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
class QLabel;
class QPushButton;
class QSlider;
class QwtPlot3D;

class Mesh2MainWindowBase : public QMainWindow
{ 
    Q_OBJECT

public:
    Mesh2MainWindowBase( QWidget* parent = 0, const char* name = 0, WFlags fl = WType_TopLevel );
    ~Mesh2MainWindowBase();

    QPushButton* normButton;
    QLabel* TextLabel2;
    QSlider* offsSlider;
    QLabel* TextLabel1_2;
    QSlider* resSlider;
    QFrame* Frame3;
    QwtPlot3D* dataWidget;
    QCheckBox* projection;
    QCheckBox* colorlegend;
    QCheckBox* autoscale;
    QCheckBox* mouseinput;
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
    QAction* titlecolor;
    QAction* resetcolor;
    QActionGroup* font;
    QAction* numberfont;
    QAction* labelfont;
    QAction* titlefont;
    QAction* resetfont;
    QActionGroup* floorstyle;
    QAction* floordata;
    QAction* flooriso;
    QAction* floormesh;
    QAction* floornone;


protected:
    QGridLayout* Mesh2MainWindowBaseLayout;
    QHBoxLayout* Layout5;
    QVBoxLayout* Layout10;
    QVBoxLayout* Layout9;
    QGridLayout* Frame3Layout;
    QVBoxLayout* Layout7;
    QVBoxLayout* Layout6;
};

#endif // MESH2MAINWINDOWBASE_H
