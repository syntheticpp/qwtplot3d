#ifndef __FUNCTIONGENERATOR_H__
#define __FUNCTIONGENERATOR_H__

#include <qstring.h>

#include "datasource.h"

class Function : public DataSource
{

public:
	
  Function();
	virtual ~Function();
	virtual double operator()(double x, double y) = 0;
	virtual QString name() const { return QString(""); }
	virtual QString formula() const { return QString(""); }
		
	virtual bool createData(Data& res);
};


#endif // __FUNCTIONGENERATOR_H__
