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
	bool testCalcAngle0();
	bool testCalcAngle45();
	bool testCalcAngle90();
	bool testCalcAngle135();
	bool testCalcAngle180();
	bool testCalcAngle225();
	bool testCalcAngle270();
	bool testCalcAngle315();

	bool testDiffWhenALarger();
	bool testDiffWhenBLarger();
	bool testDiffWhenAEqualsB();

	bool testClampWhenSmaller();
	bool testClampWhenLarger();
	bool testClampWhenInRange();

	bool run();
};

TESTCASE_REGISTRY_FOR( MathTest )
	TESTCASE( MathTest, testCalcAngle0,       "Test that calcAngle returns 0.0 " )
	TESTCASE( MathTest, testCalcAngle45,      "Test that calcAngle returns 45.0 " )
	TESTCASE( MathTest, testCalcAngle90,      "Test that calcAngle returns 90.0 " )
	TESTCASE( MathTest, testCalcAngle135,     "Test that calcAngle returns 135.0 " )
	TESTCASE( MathTest, testCalcAngle180,     "Test that calcAngle returns 180.0 " )
	TESTCASE( MathTest, testCalcAngle225,     "Test that calcAngle returns 225.0 " )
	TESTCASE( MathTest, testCalcAngle270,     "Test that calcAngle returns 270.0 " )
	TESTCASE( MathTest, testCalcAngle315,     "Test that calcAngle returns 315.0 " )

	TESTCASE( MathTest, testDiffWhenALarger,  "Test that the diff method works when A is the larger number" )
	TESTCASE( MathTest, testDiffWhenBLarger,  "Test that the diff method works when B is the larger number" )
	TESTCASE( MathTest, testDiffWhenAEqualsB, "Test that the diff method works when A equals B" )

	TESTCASE( MathTest, testClampWhenSmaller, "Test that clamp returns min when value is smaller than min" )
	TESTCASE( MathTest, testClampWhenLarger,  "Test that clamp returns max when value is larger than max" )
	TESTCASE( MathTest, testClampWhenInRange, "Test that clamp returns the value when it is between min and max" )
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

bool MathTest::testClampWhenSmaller()
{
	TEST_EQ( 50, Math::clamp< uint32 >( 45, 50, 100 ) );

	TESTCASE_END();
}

bool MathTest::testClampWhenLarger()
{
	TEST_EQ( 100, Math::clamp< uint32 >( 150, 50, 100 ) );

	TESTCASE_END();
}

bool MathTest::testClampWhenInRange()
{
	TEST_EQ( 60, Math::clamp< uint32 >( 60, 50, 100 ) );

	TESTCASE_END();
}

bool MathTest::testCalcAngle0()
{
	TEST_EQ( 0.0f, Math::calcAngle( 10.0f, 10.0f, 10.0f, 10.0f ) );
	TEST_EQ( 0.0f, Math::calcAngle( 10.0f, 10.0f, 20.0f, 10.0f ) );	

	TESTCASE_END();
}

bool MathTest::testCalcAngle45()
{
	TEST_EQ( 45.0f, Math::calcAngle( 10.0f, 10.0f, 20.0f, 20.0f ) );

	TESTCASE_END();
}

bool MathTest::testCalcAngle90()
{
	TEST_EQ( 90.0f, Math::calcAngle( 10.0f, 10.0f, 10.0f, 20.0f ) );

	TESTCASE_END();
}

bool MathTest::testCalcAngle135()
{
	TEST_EQ( 135.0f, Math::calcAngle( 10.0f, 10.0f, 0.0f, 20.0f ) );

	TESTCASE_END();
}

bool MathTest::testCalcAngle180()
{
	TEST_EQ( 180.0f, Math::calcAngle( 10.0f, 10.0f, 0.0f, 10.0f ) );

	TESTCASE_END();
}

bool MathTest::testCalcAngle225()
{
	TEST_EQ( 225.0f, Math::calcAngle( 10.0f, 10.0f, 0.0f, 0.0f ) );

	TESTCASE_END();
}

bool MathTest::testCalcAngle270()
{
	TEST_EQ( 270.0f, Math::calcAngle( 10.0f, 10.0f, 10.0f, 0.0f ) );

	TESTCASE_END();
}

bool MathTest::testCalcAngle315()
{
	TEST_EQ( 315.0f, Math::calcAngle( 10.0f, 10.0f, 20.0f, 0.0f ) );

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
