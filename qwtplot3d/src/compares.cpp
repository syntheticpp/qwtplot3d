#include "types.h"


double MAX(const std::vector<double>& data)
{
	return *std::max_element(data.begin(),data.end());
}

