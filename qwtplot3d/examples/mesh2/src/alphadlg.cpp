/****************************************************************************
** Form implementation generated from reading ui file 'src\alphadlg.ui'
**
** Created: Di 8. Apr 01:23:18 2003
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "alphadlg.h"

#include <qvariant.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qwt_slider.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/* 
 *  Constructs a alphadlgBase which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
alphadlgBase::alphadlgBase( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "alphadlgBase" );
    resize( 500, 280 ); 
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, sizePolicy().hasHeightForWidth() ) );
    setMinimumSize( QSize( 500, 280 ) );
    setMaximumSize( QSize( 500, 280 ) );
    setCaption( trUtf8( "Alpha Values" ) );
    setSizeGripEnabled( TRUE );

    QWidget* privateLayoutWidget = new QWidget( this, "Layout2_2" );
    privateLayoutWidget->setGeometry( QRect( 95, 5, 60, 170 ) ); 
    Layout2_2 = new QVBoxLayout( privateLayoutWidget, 0, 6, "Layout2_2"); 

    TextLabel1_2_2 = new QLabel( privateLayoutWidget, "TextLabel1_2_2" );
    TextLabel1_2_2->setText( trUtf8( "Mesh" ) );
    TextLabel1_2_2->setAlignment( int( QLabel::AlignCenter ) );
    Layout2_2->addWidget( TextLabel1_2_2 );

    meshSld = new QwtSlider( privateLayoutWidget, "meshSld" );
    meshSld->setOrientation( QwtSlider::Vertical );
    meshSld->setThumbLength( 16 );
    meshSld->setThumbWidth( 23 );
    meshSld->setBgStyle( QwtSlider::BgSlot );
    meshSld->setScalePos( QwtSlider::Left );
    Layout2_2->addWidget( meshSld );

    QWidget* privateLayoutWidget_2 = new QWidget( this, "Layout2" );
    privateLayoutWidget_2->setGeometry( QRect( 20, 5, 60, 170 ) ); 
    Layout2 = new QVBoxLayout( privateLayoutWidget_2, 0, 6, "Layout2"); 

    TextLabel1_2 = new QLabel( privateLayoutWidget_2, "TextLabel1_2" );
    TextLabel1_2->setText( trUtf8( "Polygons" ) );
    TextLabel1_2->setAlignment( int( QLabel::AlignCenter ) );
    Layout2->addWidget( TextLabel1_2 );

    polygonsSld = new QwtSlider( privateLayoutWidget_2, "polygonsSld" );
    polygonsSld->setOrientation( QwtSlider::Vertical );
    polygonsSld->setThumbLength( 16 );
    polygonsSld->setThumbWidth( 23 );
    polygonsSld->setBgStyle( QwtSlider::BgSlot );
    polygonsSld->setScalePos( QwtSlider::Left );
    Layout2->addWidget( polygonsSld );

    buttonHide = new QPushButton( this, "buttonHide" );
    buttonHide->setGeometry( QRect( 310, 230, 82, 25 ) ); 
    buttonHide->setCaption( trUtf8( "" ) );
    buttonHide->setText( trUtf8( "&Hide" ) );
    buttonHide->setAccel( 0 );
    buttonHide->setAutoDefault( TRUE );
    buttonHide->setDefault( TRUE );

    TextLabel1 = new QLabel( this, "TextLabel1" );
    TextLabel1->setGeometry( QRect( 175, 80, 295, 25 ) ); 
    QFont TextLabel1_font(  TextLabel1->font() );
    TextLabel1_font.setFamily( "Arial" );
    TextLabel1_font.setPointSize( 12 );
    TextLabel1_font.setBold( TRUE );
    TextLabel1->setFont( TextLabel1_font ); 
    TextLabel1->setText( trUtf8( "Feature under construction !" ) );
    TextLabel1->setScaledContents( FALSE );

    // signals and slots connections
    connect( buttonHide, SIGNAL( clicked() ), this, SLOT( accept() ) );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
alphadlgBase::~alphadlgBase()
{
    // no need to delete child widgets, Qt does it all for us
}

