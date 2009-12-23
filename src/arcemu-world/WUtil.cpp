#include "StdAfx.h"

uint32 Arcemu::Util::GUID_HIPART( uint64 GUID ){
    uint32 higuid = *( reinterpret_cast< const uint32* >( &GUID ) + 1 );

    return higuid;
}

uint32 Arcemu::Util::GUID_LOPART( uint64 GUID ){
    uint32 loguid = *( reinterpret_cast< const uint32* >( &GUID ) );

    return loguid;
}
