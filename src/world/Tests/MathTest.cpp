/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2024 <http://www.ArcEmu.org/>
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

#include "../../shared/System/Platform.h"
#include "../Math/Math.h"

#include "../../testing/testmacros.h"

class MathTest
{
public:
	bool testDiffWhenALarger();
	bool testDiffWhenBLarger();
	bool testDiffWhenAEqualsB();
	bool run();
};

TESTCASE_REGISTRY_FOR( MathTest )
	TESTCASE( MathTest, testDiffWhenALarger,  "Test that the diff method works when A is the larger number" )
	TESTCASE( MathTest, testDiffWhenBLarger,  "Test that the diff method works when B is the larger number" )
	TESTCASE( MathTest, testDiffWhenAEqualsB, "Test that the diff method works when A equals B" )
TESTCASE_REGISTRY_END()

bool MathTest::testDiffWhenALarger()
{
	TEST_EQ( 1, Math::diff< unsigned int >( 2, 1 ) );

	TESTCASE_END();
}

bool MathTest::testDiffWhenBLarger()
{
	TEST_EQ( 1, Math::diff< unsigned int >( 1, 2 ) );

	TESTCASE_END();
}

bool MathTest::testDiffWhenAEqualsB()
{
	TEST_EQ( 0, Math::diff< unsigned int >( 1, 1 ) );

	TESTCASE_END();
}

bool MathTest::run()
{
	RUN_TESTS();
}

int MathTestRun()
{
	RUN_TESTCLASS( MathTest );
}
