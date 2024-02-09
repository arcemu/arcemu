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
	bool testToRadians();
	bool testToAngle();

	bool testCalcAngle();

	bool testDiffWhenALarger();
	bool testDiffWhenBLarger();
	bool testDiffWhenAEqualsB();

	bool testClampWhenSmaller();
	bool testClampWhenLarger();
	bool testClampWhenInRange();

	bool run();
};

TESTCASE_REGISTRY_FOR( MathTest )
	TESTCASE( MathTest, testToRadians,        "Test that toRadians works" )
	TESTCASE( MathTest, testToAngle,          "Test that toAngle works" )

	TESTCASE( MathTest, testCalcAngle,        "Test that calcAngle works" )

	TESTCASE( MathTest, testDiffWhenALarger,  "Test that the diff method works when A is the larger number" )
	TESTCASE( MathTest, testDiffWhenBLarger,  "Test that the diff method works when B is the larger number" )
	TESTCASE( MathTest, testDiffWhenAEqualsB, "Test that the diff method works when A equals B" )

	TESTCASE( MathTest, testClampWhenSmaller, "Test that clamp returns min when value is smaller than min" )
	TESTCASE( MathTest, testClampWhenLarger,  "Test that clamp returns max when value is larger than max" )
	TESTCASE( MathTest, testClampWhenInRange, "Test that clamp returns the value when it is between min and max" )
TESTCASE_REGISTRY_END()

bool MathTest::testToRadians()
{
	static float testData[ 8 ][ 2 ] =
	{
		{ 0.0f, 0.0f },
		{ 45.0f, M_PI_FLOAT / 4 },
		{ 90.0f, M_PI_FLOAT / 2 },
		{ 135.0f, 3.0f/4.0f * M_PI_FLOAT },
		{ 180.0f, M_PI_FLOAT },
		{ 225.0f, 5.0f/4.0f * M_PI_FLOAT },
		{ 270.0f, 3 * M_PI_FLOAT / 2 },
		{ 315.0f, 7.0f/4.0f * M_PI_FLOAT }
	};

	for( int i = 0; i < 8; i++ )
	{
		TEST_TRUE( ( Math::diff< float >( testData[ i ][ 1 ], Math::toRadians( testData[ i ][ 0 ] ) ) <= TOLERANCE_FLOAT ) );
	}

	TESTCASE_END();
}

bool MathTest::testToAngle()
{
	static float testData[ 8 ][ 2 ] =
	{
		{ 0.0f, 0.0f },
		{ 45.0f, M_PI_FLOAT / 4 },
		{ 90.0f, M_PI_FLOAT / 2 },
		{ 135.0f, 3.0f/4.0f * M_PI_FLOAT },
		{ 180.0f, M_PI_FLOAT },
		{ 225.0f, 5.0f/4.0f * M_PI_FLOAT },
		{ 270.0f, 3 * M_PI_FLOAT / 2 },
		{ 315.0f, 7.0f/4.0f * M_PI_FLOAT }
	};

	for( int i = 0; i < 8; i++ )
	{
		TEST_TRUE( ( Math::diff< float >( testData[ i ][ 0 ], Math::toAngle( testData[ i ][ 1 ] ) ) <= TOLERANCE_FLOAT ) );
	}

	TESTCASE_END();
}

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

bool MathTest::testCalcAngle()
{
	static float testData[ 9 ][ 5 ] = 
	{
		{ 10.0f, 10.0f, 10.0f, 10.0f, 0.0f  },
		{ 10.0f, 10.0f, 20.0f, 10.0f, 0.0f  },
		{ 10.0f, 10.0f, 20.0f, 20.0f, 45.0f },
		{ 10.0f, 10.0f, 10.0f, 20.0f, 90.0f },
		{ 10.0f, 10.0f, 0.0f, 20.0f, 135.0f },
		{ 10.0f, 10.0f, 0.0f, 10.0f, 180.0f },
		{ 10.0f, 10.0f, 0.0f, 0.0f,  225.0f },
		{ 10.0f, 10.0f, 10.0f, 0.0f, 270.0f },
		{ 10.0f, 10.0f, 20.0f, 0.0f, 315.0f }
	};

	for( int i = 0; i < 9; i++ )
	{
		TEST_EQ( testData[ i ][ 4 ], Math::calcAngle( testData[ i ][ 0 ], testData[ i ][ 1 ], testData[ i ][ 2 ], testData[ i ][ 3 ] ) );
	}

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
