#ifndef __qwt3d_autoscaler_2003_08_18_12_05__
#define __qwt3d_autoscaler_2003_08_18_12_05__

#include "qwt3d_global.h"

namespace Qwt3D
{

//! Automatic beautifying of linear scales
class QWT3D_EXPORT AutoScaler
{
public:

	AutoScaler();
	AutoScaler(double start, double stop, int ivals);

	void init(double start, double stop, int ivals);
	int execute(double& a, double& b);
	int intervals() const {return intervals_;} //!< Current number of intervals (will mostly change after the first execute() call) 


private:
	
	double start_, stop_;
	int intervals_; 

	double floor125( int& exponent, double x);
	double anchorvalue(double start, double mantisse, int exponent);
	int segments(int& l_intervals, int& r_intervals, double start, double stop, double anchor, double mantissa, int exponent);
};

} // ns


#endif
