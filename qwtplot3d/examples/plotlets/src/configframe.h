#pragma once
#ifndef configdialog_h__2010_02_18_14_05_33_begin_guarded_code
#define configdialog_h__2010_02_18_14_05_33_begin_guarded_code

#include <qapplication.h>

#include "ui_configframebase.h"


class ConfigFrame : public QFrame, protected Ui::ConfigFrameBase
{
	Q_OBJECT
    
public:
  ConfigFrame(QWidget *parent=0);

public slots:
	void setEmission(int);
};

#endif /* include guarded */
