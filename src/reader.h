#ifndef __READER_H__
#define __READER_H__

#include <qstring.h>
#include "datasource.h"


class NativeReader : public DataSource
{
public:
	
	enum Type
	{
		XYZFILE,
		MESHFILE
	};
	
	explicit NativeReader(QString fname = "");

	virtual bool createData(Data& result);

	void setType(Type t) {rtype_ = t;}
	void setFileName(QString fname) {fileName_ = fname;}

	const char* magicstring;

	

private:
	bool read_mesh_data_(Data&, QString fname);
	bool read_xyz_data_(Data &, QString fname);

	Type rtype_;
	QString fileName_;
};




#endif