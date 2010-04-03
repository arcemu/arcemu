#include "StdAfx.h"
#include <iostream>

uint32 Arcemu::Util::GUID_HIPART( uint64 GUID ){
    uint32 higuid = *( reinterpret_cast< const uint32* >( &GUID ) + 1 );

    return higuid;
}

uint32 Arcemu::Util::GUID_LOPART( uint64 GUID ){
    uint32 loguid = *( reinterpret_cast< const uint32* >( &GUID ) );

    return loguid;
}

void Arcemu::Util::ARCEMU_ASSERT(  bool condition){
	if( !condition ){
		std::cout << "Assertion failed. Please submit the callstack on forums(unless you are using a repack)."<< std::endl;

		// bogus null function call to make sure we stop and make a core dump / crash dump
		((void(*)())0)();
	}
}
