#include "configframe.h"

ConfigFrame::ConfigFrame(QWidget *parent)
:QFrame(parent)
{
  setupUi(this);
	//connect( emissSL, SIGNAL(valueChanged(int)), this, SLOT(setEmission(int)) );
}

void ConfigFrame::setEmission(int val)
{
//  if (!dataPlot)
//    return;
//  dataPlot->setMaterialComponent(GL_EMISSION, val / 100.);
//  dataPlot->updateGL();
}


