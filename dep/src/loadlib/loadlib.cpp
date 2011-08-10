#define _CRT_SECURE_NO_DEPRECATE

#include "loadlib.h"
#include <fstream>
#include <cstdio>

class MPQFile;

FileLoader::FileLoader()
{
    data = 0;
    data_size = 0;
    version = 0;
}

FileLoader::~FileLoader()
{
    free();
}

bool FileLoader::loadFile(char *filename, bool log)
{
    free();
	std::ifstream mf(filename);
    if(mf.fail())
    {
        if (log)
            printf("No such file %s\n", filename);
		mf.close();
        return false;
    }

	mf.seekg(0, std::ios::end);
	data_size = mf.tellg();
	mf.seekg(0, std::ios::beg);

    data = new uint8 [data_size];
    if (data)
    {
        mf.read( reinterpret_cast< char* >( data ), data_size);
        mf.close();
        if (prepareLoadedData())
            return true;
    }
    printf("Error loading %s", filename);
    mf.close();
    free();
    return false;
}

bool FileLoader::prepareLoadedData()
{
    // Check version
    version = (file_MVER *) data;
	if( ( version->fcc_txt[ 0 ] != 'M' ) ||
		( version->fcc_txt[ 1 ] != 'V' ) ||
		( version->fcc_txt[ 2 ] != 'E' ) ||
		( version->fcc_txt[ 3 ] != 'R' ) )
		return false;

    if (version->ver != FILE_FORMAT_VERSION)
        return false;
    return true;
}

void FileLoader::free()
{
    if (data) delete[] data;
    data = 0;
    data_size = 0;
    version = 0;
}