#include "alphadlgimpl.h"

/* 
 *  Constructs a alphadlg which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
alphadlg::alphadlg( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : alphadlgBase( parent, name, modal, fl )
{
}

/*  
 *  Destroys the object and frees any allocated resources
 */
alphadlg::~alphadlg()
{
    // no need to delete child widgets, Qt does it all for us
}

