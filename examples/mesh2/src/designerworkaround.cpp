#include "designerworkaround.h"

namespace{
  QString tr(const char* val)
  {
    return QObject::tr(val);
  }
}


void DesignerWorkaround::setupWorkaround(QMainWindow* mw)
{
    // actions
  openFile = new QAction(QIcon(":/images/fileopen.png"), tr("&Open File"), mw);
  openFile->setShortcut( QKeySequence(tr("CTRL+O")));
  openMeshFile = new QAction(QIcon(":/images/filecell.png"), tr("Open FEM File"), mw);
  
  Exit = new QAction(tr("&Exit"), mw);
  Exit->setShortcut( QKeySequence(tr("CTRL+Q")));

  animation = new QAction(QIcon(":/images/movie.png"), "", mw);
  dump = new QAction(QIcon(":/images/savecontent.png"), "", mw);

  coord = new QActionGroup(mw);
    Box = new QAction(QIcon(":/images/box.png"), "", mw);
    Frame = new QAction(QIcon(":/images/frame.png"), "", mw);
    None = new QAction(QIcon(":/images/grid.png"), "", mw);
    coord->addAction(Box);
    coord->addAction(Frame);
    coord->addAction(None);

  grids = new QActionGroup(mw);
    front = new QAction(QIcon(":/images/gridfr.png"), "", mw);
    back = new QAction(QIcon(":/images/gridb.png"), "", mw);
    right = new QAction(QIcon(":/images/gridr.png"), "", mw);
    left = new QAction(QIcon(":/images/gridl.png"), "", mw);
    ceil = new QAction(QIcon(":/images/gridc.png"), "", mw);
    floor = new QAction(QIcon(":/images/gridf.png"), "", mw);
    grids->addAction(front);
    grids->addAction(back);
    grids->addAction(right);
    grids->addAction(left);
    grids->addAction(ceil);
    grids->addAction(floor);

  plotstyle = new QActionGroup(mw);
    pointstyle = new QAction(QIcon(":/images/scattered.png"), "", mw);
    wireframe = new QAction(QIcon(":/images/wireframe.png"), "", mw);
    hiddenline = new QAction(QIcon(":/images/hiddenline.png"), "", mw);
    polygon = new QAction(QIcon(":/images/polygon.png"), "", mw);
    filledmesh = new QAction(QIcon(":/images/filledmesh.png"), "", mw);
    nodata = new QAction(QIcon(":/images/nodata.png"), "", mw);
    plotstyle->addAction(pointstyle);
    plotstyle->addAction(wireframe);
    plotstyle->addAction(hiddenline);
    plotstyle->addAction(polygon);
    plotstyle->addAction(filledmesh);
    plotstyle->addAction(nodata);

  floorstyle = new QActionGroup(mw);
    floordata = new QAction(QIcon(":/images/floordata.png"), "", mw);
    flooriso = new QAction(QIcon(":/images/flooriso.png"), "", mw);
    floornone = new QAction(QIcon(":/images/floorempty.png"), "", mw);
    floorstyle->addAction(floordata);
    floorstyle->addAction(flooriso);
    floorstyle->addAction(floornone);

  normals = new QAction(QIcon(":/images/normals.png"), "", mw);

  color = new QActionGroup(mw);
    axescolor = new QAction(tr("&Axes"), mw);
    backgroundcolor = new QAction(tr("&Background"), mw);
    meshcolor = new QAction(tr("&Mesh"), mw);
    numbercolor = new QAction(tr("&Numbers"), mw);
    labelcolor = new QAction(tr("&Label"), mw);
    titlecolor = new QAction(tr("Caption"), mw);
    datacolor = new QAction(tr("Data color"), mw);
    resetcolor = new QAction(tr("&Reset"), mw);
    color->addAction(axescolor);
    color->addAction(backgroundcolor);
    color->addAction(meshcolor);
    color->addAction(numbercolor);
    color->addAction(labelcolor);
    color->addAction(titlecolor);
    color->addAction(datacolor);
    color->addAction(resetcolor);

  font = new QActionGroup(mw);
    numberfont = new QAction(tr("&Scale numbering"), mw);
    labelfont = new QAction(tr("&Axes label"), mw);
    titlefont = new QAction(tr("&Caption"), mw);
    resetfont = new QAction(tr("&Reset"), mw);
    font->addAction(numberfont);
    font->addAction(labelfont);
    font->addAction(titlefont);
    font->addAction(resetfont);
  


//    // toolbars
  mainToolbar = new QToolBar( QString(""), mw ); 
  mainToolbar->setMovable( false );
  
  mainToolbar->addAction(openFile);
  mainToolbar->addAction(openMeshFile);
  mainToolbar->addAction(dump);

  filetypeCB = new QComboBox;
  functionCB = new QComboBox;
  psurfaceCB = new QComboBox;

  mainToolbar->addWidget(filetypeCB);
  mainToolbar->addWidget(functionCB);
  mainToolbar->addWidget(psurfaceCB);

  mainToolbar->addAction(animation);

  csToolbar = new QToolBar( QString(""), mw ); 
  csToolbar->setMovable( false );

  csToolbar->addActions(coord->actions());
  csToolbar->addActions(grids->actions());
  csToolbar->addActions(plotstyle->actions());
  csToolbar->addActions(floorstyle->actions());
  csToolbar->addAction(normals);
  
  // menubar
  QMenuBar* menuBar = mw->menuBar();
    QMenu* fileMenu = new QMenu(mw);
//      filemenu->addAction(openFile);
////        filemenu->addAction(openMeshFile);
////        filemenu->addAction(animation);
////        filemenu->addAction(Exit);
//    QMenu* colormenu = menuBar->addMenu(tr("&Color"));    
//      colormenu->addActions(color->actions());
//    QMenu* fontmenu = menuBar->addMenu(tr("&Fonts"));    
//      colormenu->addActions(font->actions());
  
  mw->addToolBar(mainToolbar);
  mw->addToolBar(csToolbar);
  

  functionCB->clear();
  functionCB->addItem( tr( "---" ) );
  functionCB->addItem( tr( "Hat" ) );
  functionCB->addItem( tr( "Rosenbrock" ) );
  functionCB->addItem( tr( "Saddle" ) );
  functionCB->addItem( tr( "Sombrero" ) );
  functionCB->addItem( tr( "Ripple" ) );
  functionCB->setToolTip(tr( "Display function" ) );
  psurfaceCB->clear();
  psurfaceCB->addItem( tr( "---" ) );
  psurfaceCB->addItem( tr( "Torus" ) );
  psurfaceCB->addItem( tr( "Seashell" ) );
  psurfaceCB->addItem( tr( "Boy" ) );
  psurfaceCB->addItem( tr( "Dini" ) );
  psurfaceCB->addItem( tr( "Cone" ) );
  psurfaceCB->setToolTip(tr( "Display parametric surface. Turn Lighting on for better quality."));
    
  QObject::connect(Exit, SIGNAL(triggered()), mw, SLOT(close()));
}
