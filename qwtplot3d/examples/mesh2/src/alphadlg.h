/****************************************************************************
** Form interface generated from reading ui file 'src\alphadlg.ui'
**
** Created: Do 17. Apr 18:23:17 2003
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef ALPHADLGBASE_H
#define ALPHADLGBASE_H

#include <qvariant.h>
#include <qdialog.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QLabel;
class QPushButton;
class QwtSlider;

class alphadlgBase : public QDialog
{ 
    Q_OBJECT

public:
    alphadlgBase( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~alphadlgBase();

    QLabel* TextLabel1_2_2;
    QwtSlider* meshSld;
    QLabel* TextLabel1_2;
    QwtSlider* polygonsSld;
    QPushButton* buttonHide;
    QLabel* TextLabel1;


protected:
    QVBoxLayout* Layout2_2;
    QVBoxLayout* Layout2;
};

#endif // ALPHADLGBASE_H
