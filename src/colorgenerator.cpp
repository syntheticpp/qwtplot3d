#include "types.h"
#include "colorgenerator.h"

Color::~Color()
{
}

StandardColor::StandardColor(Data const& data, int size)
			: data_(data)
{
	colors_ = ColorVector(size);
	RGBA elem;

	double dsize = size;

	for (int i=0; i!=colors_.size(); ++i)
	{
		elem.r = i / dsize;
		elem.g = i / dsize / 4;
		elem.b = 1 - i/dsize;
		elem.a = 1;
		colors_[i] = elem;
	}
}

void
StandardColor::setAlpha(double a)
{
	if (a<0 || a>1)
		return;
	
	RGBA elem;

	for (int i=0; i!=colors_.size(); ++i)
	{
		elem = colors_[i];
		elem.a = a;
		colors_[i] = elem;
	}	
}
		
RGBA 
StandardColor::operator()(double x, double y, double z)
{
	int index = (int)((colors_.size()-1) * (z - data_.minimum()) / (data_.maximum() - data_.minimum()));
	if (index < 0)
		index = 0;
	if (index > colors_.size() - 1)
		index = colors_.size() - 1;
	return colors_[index];

/*
	RGBA rgba;

	rgba.r = colors_[index].r;
	rgba.g = colors_[index].g;
	rgba.b = colors_[index].b;
	rgba.a = 1-((z - data_.minimum()) / (data_.maximum() - data_.minimum()));
	return rgba;
*/
}
	