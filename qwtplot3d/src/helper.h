#ifndef __HELPER_H__
#define __HELPER_H__

#include <math.h>
#include <float.h>
#include <vector>
#include <algorithm>

namespace
{
	inline double Min_(double a, double b)
	{
		return (a<b) ? a : b;
	}
}

inline double MAX(const std::vector<double>& data)
{
	return *std::max_element(data.begin(),data.end());
}

inline bool isPracticallyZero(double a, double b = 0)
{
  if (!b)
		return (fabs (a) <=  DBL_MIN);	

	return (fabs (a - b) <= Min_(fabs(a), fabs(b))*DBL_EPSILON);	
}
 
inline int round(double d)
{
	return (d>0) ? int(d+0.5) : int(d-0.5);
}

/*!
  \brief Find the smallest value out of {1,2,5}*10^n with an integer number n
  which is greater than or equal to x

  \param x Input value
*/
inline double ceil125( double x)
{
    if (x == 0.0) 
        return 0.0;

    double sign = (x > 0) ? 1.0 : -1.0;
    double lx = log10(fabs(x));
    double p10 = floor(lx);

    double fr = pow(10.0, lx - p10);
    if (fr <=1.0)
       fr = 1.0;
    else if (fr <= 2.0)
       fr = 2.0;
    else if (fr <= 5.0)
       fr = 5.0;
    else
       fr = 10.0;

    return sign * fr * pow(10.0, p10);
}

/*!
  \brief Find the largest value out of {1,2,5}*10^n with an integer number n
  which is smaller than or equal to x
  
  \param x Input value
*/
inline double floor125( double x)
{
    if (x == 0.0) 
        return 0.0;
    
    double sign = (x > 0) ? 1.0 : -1.0;
    double lx = log10(fabs(x));
    double p10 = floor(lx);

    double fr = pow(10.0, lx - p10);
    if (fr >= 10.0)
       fr = 10.0;
    else if (fr >= 5.0)
       fr = 5.0;
    else if (fr >= 2.0)
       fr = 2.0;
    else
       fr = 1.0;

    return sign * fr * pow(10.0, p10);
} 

inline double round125( double x)
{
	double f = floor125(x);
	double c = ceil125(x);
	
	if ( x-f < c-x )
		return f;
	
	return c;
}

#endif
