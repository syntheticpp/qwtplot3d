#include "types.h"

namespace
{

struct Cmp4X
{
	bool operator() (const XYZ& first, const XYZ& second)
	{
		return (first.x<second.x);
	}
}; 
struct Cmp4Y
{
	bool operator() (const XYZ& first, const XYZ& second)
	{
		return (first.y<second.y);
	}
}; 
struct Cmp4Z
{
	bool operator() (const XYZ& first, const XYZ& second)
	{
		return (first.z<second.z);
	}
}; 
 

} // anon NS


XYZ MIN(const std::vector<XYZ>& data)
{
	XYZ ret;
	Cmp4X xmin;
	Cmp4Y ymin;
	Cmp4Z zmin;
	ret.x = (*std::min_element(data.begin(),data.end(),xmin)).x;
	ret.y = (*std::min_element(data.begin(),data.end(),ymin)).y;
	ret.z = (*std::min_element(data.begin(),data.end(),zmin)).z;
	return ret;
}

XYZ MAX(const std::vector<XYZ>& data)
{
	XYZ ret;
	Cmp4X xmin;
	Cmp4Y ymin;
	Cmp4Z zmin;
	ret.x = (*std::max_element(data.begin(),data.end(),xmin)).x;
	ret.y = (*std::max_element(data.begin(),data.end(),ymin)).y;
	ret.z = (*std::max_element(data.begin(),data.end(),zmin)).z;
	return ret;
}

double MAX(const std::vector<double>& data)
{
	return *std::max_element(data.begin(),data.end());
}

