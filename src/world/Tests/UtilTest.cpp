/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2020 <http://www.ArcEmu.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "../../testing/testmacros.h"

#include "../../shared/Network/AuthCodes.h"
#include "../../shared/System/Platform.h"

#include "Game/Entities/ObjectConstants.h"
#include "../WUtil.h"

#include <iostream>

class WorldUtilTest
{
public:
	bool testGUID_LOPART();
	bool testGUID_HIPART();
	bool testMAKE_ITEM_GUID();

	bool run();
};

TESTCASE_REGISTRY_FOR( WorldUtilTest )
	TESTCASE( WorldUtilTest, testGUID_LOPART, "GUID_LOPART works" )
	TESTCASE( WorldUtilTest, testGUID_HIPART, "GUID_HIPART works" )
	TESTCASE( WorldUtilTest, testMAKE_ITEM_GUID, "MAKE_ITEM_GUID works" )
TESTCASE_REGISTRY_END()

bool WorldUtilTest::testGUID_LOPART()
{
	uint64 guid = 0;

	TEST_EQ( 0, Arcemu::Util::GUID_LOPART( guid ) );

	uint32 *p = reinterpret_cast< uint32* >( &guid );
	p[ 0 ] = 123;

	TEST_EQ( 123, Arcemu::Util::GUID_LOPART( guid ) );

	TESTCASE_END();
}

bool WorldUtilTest::testGUID_HIPART()
{
	uint64 guid = 0;

	TEST_EQ( 0, Arcemu::Util::GUID_HIPART( guid ) );

	uint32 *p = reinterpret_cast< uint32* >( &guid );
	p[ 1 ] = 456;

	TEST_EQ( 456, Arcemu::Util::GUID_HIPART( guid ) );

	TESTCASE_END();
}

bool WorldUtilTest::testMAKE_ITEM_GUID()
{
	uint64 guid = Arcemu::Util::MAKE_ITEM_GUID( 123 );

	TEST_EQ( 123, Arcemu::Util::GUID_LOPART( guid ) );
	TEST_EQ( HIGHGUID_TYPE_ITEM, Arcemu::Util::GUID_HIPART( guid ) );

	TESTCASE_END();
}

bool WorldUtilTest::run()
{
	RUN_TESTS();
}

int main(int arc, char *argv[])
{
	RUN_TESTCLASS( WorldUtilTest );
}
