#ifndef __FEMREADER_H__
#define __FEMREADER_H__

#include <math.h>
#include <fstream.h>
#include "../../../src/types.h"

class NodeFilter
{
	public:
		explicit NodeFilter()
		{
			values = std::vector<double>(6);
		}
		
		Triple readLine(istream& str)
		{
			for (unsigned i = 0; i!=values.size(); ++i)
				str >> values[i];
//			return Triple(values[1], values[2], atan2(values[3],values[4]) / 1000);
//				return Triple(values[1], values[2], Triple(values[3],values[4],0).length() / 1000000000);
				return Triple(values[1], values[2], values[5] / 1000);
		}
	
	private:				
		std::vector<double> values;
};

class CellFilter
{
	public:
		
		Cell readLine(istream& str)
		{
			Cell cell(4);
			str >> cell[0]; // dummy (cell number) - overridden in next step
			for (unsigned i = 0; i<cell.size(); ++i)
			{
				str >> cell[i];
				cell[i] = cell[i] - 1;
			}
			return cell;
		}
};


template <typename FILTER>
bool readNodes(TripleVector& v, const char* fname, FILTER fil)
{
	ifstream file(fname);
	
	v.clear();

	Triple t;
	while ( file ) 
	{
		t = fil.readLine( file );			
		if (!file.good())
			break;
		v.push_back( t );
	}
	return true;
}

template <typename FILTER>
bool readConnections(Tesselation& v, const char* fname, FILTER fil)
{
	ifstream file(fname);

	v.clear();

	Cell cell;
	while ( file ) 
	{
		cell = fil.readLine( file );
		if (!file.good())
			break;
		v.push_back(cell);	
	}
	return true;
}


#endif // __FEMREADER_H__