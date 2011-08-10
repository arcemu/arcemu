#define _CRT_SECURE_NO_DEPRECATE

#include <fstream>
#include "dbcfile.h"

DBCFile::DBCFile(const std::string &filename):
    filename(filename),
    data(0)
{

}
bool DBCFile::open()
{
	std::ifstream f(filename.c_str());
    char header[4];
    unsigned int na,nb,es,ss;

    f.read(header,4); // Number of records

	if( f.fail() ){
		f.close();
		return false;
	}

	if(header[0]!='W' || header[1]!='D' || header[2]!='B' || header[3]!='C'){
		f.close();
        return false;
	}

    f.read( reinterpret_cast< char* >(&na),4);    // Number of records

	if( f.fail() ){
		f.close();
		return false;
	}
    
	f.read( reinterpret_cast< char* >(&nb),4);    // Number of fields

	if( f.fail() ){
		f.close();
		return false;
	}
    
	
	f.read( reinterpret_cast< char* >(&es),4);    // Size of a record

	if( f.fail() ){
		f.close();
		return false;
	}
    
	
	f.read( reinterpret_cast< char* >(&ss),4);   // String size

	if( f.fail() ){
		f.close();
		return false;
	}

    recordSize = es;
    recordCount = na;
    fieldCount = nb;
    stringSize = ss;
	if(fieldCount*4 != recordSize){
		f.close();
        return false;
	}

    data = new unsigned char[recordSize*recordCount+stringSize];
    stringTable = data + recordSize*recordCount;

    size_t data_size = recordSize*recordCount+stringSize;

    f.read( reinterpret_cast< char* >(data),data_size);

	if( f.fail() ){
		f.close();
		return false;
	}

    f.close();
    return true;
}
DBCFile::~DBCFile()
{
    delete [] data;
}

DBCFile::Record DBCFile::getRecord(size_t id)
{
    assert(data);
    return Record(*this, data + id*recordSize);
}

size_t DBCFile::getMaxId()
{
    assert(data);

    size_t maxId = 0;
    for(size_t i = 0; i < getRecordCount(); ++i)
    {
        if(maxId < getRecord(i).getUInt(0))
            maxId = getRecord(i).getUInt(0);
    }
    return maxId;
}

DBCFile::Iterator DBCFile::begin()
{
    assert(data);
    return Iterator(*this, data);
}
DBCFile::Iterator DBCFile::end()
{
    assert(data);
    return Iterator(*this, stringTable);
}
