#include "functions.h"

Rosenbrock::Rosenbrock()
:Function()
{
	setMinZ(-100);
}

Hat::Hat()
:Function()
{
//	setMaxZ(0.8);     
}

void CreateFunction(Data& res, QString const& name)    
{
	if (name == QString("Rosenbrock")) 
	{
		Rosenbrock rosenbrock;
		
		rosenbrock.setMesh(71,71);
		rosenbrock.setDomain(-1.73,1.5,-1.5,1.5);
		//rosenbrock.setDomain(-5,5.5,-1,2);
		rosenbrock.setMinZ(-10);
		
		rosenbrock.createData(res);
	}
	else if (name == QString("Hat")) 
	{
		Hat hat;
		
		hat.setMesh(61,61);
		hat.setDomain(-1.5,1.5,-1.5,1.5);
		
		hat.createData(res);		
	}
}
