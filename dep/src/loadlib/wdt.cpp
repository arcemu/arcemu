#define _CRT_SECURE_NO_DEPRECATE

#include "wdt.h"

bool wdt_MWMO::prepareLoadedData()
{
    if( ( fcc_txt[ 3 ] != 'M' ) ||
		( fcc_txt[ 2 ] != 'W' ) ||
		( fcc_txt[ 1 ] != 'M' ) ||
		( fcc_txt[ 0 ] != 'O' ) )
		return false;

    return true;
}

bool wdt_MPHD::prepareLoadedData()
{
    if( ( fcc_txt[ 3 ] != 'M' ) ||
		( fcc_txt[ 2 ] != 'P' ) ||
		( fcc_txt[ 1 ] != 'H' ) ||
		( fcc_txt[ 0 ] != 'D' ) )
		return false;
	
	return true;
}

bool wdt_MAIN::prepareLoadedData()
{
    if( ( fcc_txt[ 3 ] != 'M' ) ||
		( fcc_txt[ 2 ] != 'A' ) ||
		( fcc_txt[ 1 ] != 'I' ) ||
		( fcc_txt[ 0 ] != 'N' ) )
		return false;
	
	return true;
}

WDT_file::WDT_file()
{
    mphd = 0;
    main = 0;
    wmo  = 0;
}

WDT_file::~WDT_file()
{
    free();
}

void WDT_file::free()
{
    mphd = 0;
    main = 0;
    wmo  = 0;
    FileLoader::free();
}

bool WDT_file::prepareLoadedData()
{
    // Check parent
    if (!FileLoader::prepareLoadedData())
        return false;

    mphd = (wdt_MPHD *)((uint8*)version+version->size+8);
    if (!mphd->prepareLoadedData())
        return false;
    main = (wdt_MAIN *)((uint8*)mphd + mphd->size+8);
    if (!main->prepareLoadedData())
        return false;
    wmo = (wdt_MWMO *)((uint8*)main+ main->size+8);
    if (!wmo->prepareLoadedData())
        return false;
    return true;
}