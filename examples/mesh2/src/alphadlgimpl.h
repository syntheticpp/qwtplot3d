#ifndef ALPHADLG_H
#define ALPHADLG_H
#include "alphadlg.h"

class alphadlg : public alphadlgBase
{ 
    Q_OBJECT

public:
    alphadlg( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~alphadlg();

};

#endif // ALPHADLG_H
