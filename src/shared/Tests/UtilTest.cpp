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

#include "../Util.h"
#include "../../testing/testmacros.h"
#include <iostream>

class UtilTest
{
public:
	bool testArcemuToLower();	
	bool testArcemuToLowerWithLowerCase();
	
	bool testArcemuToUpper();
	bool testArcemuToUpperWithUpperCase();
	
	bool testFloatRoundUp();
	bool testFloatRoundDown();
	bool testFloatRoundWhole();

	bool testDoubleRoundUp();
	bool testDoubleRoundDown();
	bool testDoubleRoundWhole();

	bool testLongDoubleRoundUp();
	bool testLongDoubleRoundDown();
	bool testLongDoubleRoundWhole();

	bool run();
};

TESTCASE_REGISTRY_FOR( UtilTest )
	TESTCASE( UtilTest, testArcemuToLower, "arcemu_TOLOWER converts uppercase to lowercase." )
	TESTCASE( UtilTest, testArcemuToLowerWithLowerCase, "arcemu_TOLOWER converts leaves lowercase." )

	TESTCASE( UtilTest, testArcemuToUpper, "arcemu_TOUPPER converts lowercase to uppercase." )
	TESTCASE( UtilTest, testArcemuToUpperWithUpperCase, "arcemu_TOUPPER leaves uppercase as is" )

	TESTCASE( UtilTest, testFloatRoundUp, "round( float ) rounds up when appropriate" )
	TESTCASE( UtilTest, testFloatRoundDown, "round( float ) rounds down when appropriate" )
	TESTCASE( UtilTest, testFloatRoundWhole, "round( float ) doesn't round a whole number" )

	TESTCASE( UtilTest, testDoubleRoundUp, "round( double ) rounds up when appropriate" )
	TESTCASE( UtilTest, testDoubleRoundDown, "round( double ) rounds down when appropriate" )
	TESTCASE( UtilTest, testDoubleRoundWhole, "round( double ) doesn't round a whole number" )

	TESTCASE( UtilTest, testLongDoubleRoundUp, "round( long double ) rounds up when appropriate" )
	TESTCASE( UtilTest, testLongDoubleRoundDown, "round( long double ) rounds down when appropriate" )
	TESTCASE( UtilTest, testLongDoubleRoundWhole, "round( long double ) doesn't round a whole number" )
TESTCASE_REGISTRY_END()

bool UtilTest::testArcemuToLower()
{
	std::string actual = "UPPERCASE STRING";
	std::string original = actual;
	std::string expected = "uppercase string";

	Arcemu::Shared::Util::arcemu_TOLOWER( actual );

	std::cout << "Original: " << original << std::endl;
	std::cout << "Expected: " << expected << std::endl;
	std::cout << "Actual: " << actual << std::endl;

	TEST_EQ( expected.compare( actual ), 0 );

	TESTCASE_END();
}

bool UtilTest::testArcemuToLowerWithLowerCase()
{
	std::string actual = "lowercase string";
	std::string original = actual;
	std::string expected = original;

	Arcemu::Shared::Util::arcemu_TOLOWER( actual );

	std::cout << "Original: " << original << std::endl;
	std::cout << "Expected: " << expected << std::endl;
	std::cout << "Actual: " << actual << std::endl;

	TEST_EQ( expected.compare( actual ), 0 );

	TESTCASE_END();
}

bool UtilTest::testArcemuToUpper()
{
	std::string actual = "lowercase string";
	std::string original = actual;
	std::string expected = "LOWERCASE STRING";

	Arcemu::Shared::Util::arcemu_TOUPPER( actual );

	std::cout << "Original: " << original << std::endl;
	std::cout << "Expected: " << expected << std::endl;
	std::cout << "Actual: " << actual << std::endl;

	TEST_EQ( expected.compare( actual ), 0 );

	TESTCASE_END();
}

bool UtilTest::testArcemuToUpperWithUpperCase()
{
	std::string actual = "UPPERCASE STRING";
	std::string original = actual;
	std::string expected = original;

	Arcemu::Shared::Util::arcemu_TOUPPER( actual );

	std::cout << "Original: " << original << std::endl;
	std::cout << "Expected: " << expected << std::endl;
	std::cout << "Actual: " << actual << std::endl;

	TEST_EQ( expected.compare( actual ), 0 );

	TESTCASE_END();
}

bool UtilTest::testFloatRoundUp()
{
	float input = 2.6f;
	float expected = 3.0f;
	float actual = Arcemu::Shared::Util::round( input );

	std::cout << "Input: " << input << std::endl;
	std::cout << "Expected: " << expected << std::endl;
	std::cout << "Actual: " << actual << std::endl;

	TEST_EQ( expected, actual );

	TESTCASE_END();
}

bool UtilTest::testFloatRoundDown()
{
	float input = 2.2f;
	float expected = 2.0f;
	float actual = Arcemu::Shared::Util::round( input );

	std::cout << "Input: " << input << std::endl;
	std::cout << "Expected: " << expected << std::endl;
	std::cout << "Actual: " << actual << std::endl;

	TEST_EQ( expected, actual );

	TESTCASE_END();
}

bool UtilTest::testFloatRoundWhole()
{
	float input = 2.0f;
	float expected = 2.0f;
	float actual = Arcemu::Shared::Util::round( input );

	std::cout << "Input: " << input << std::endl;
	std::cout << "Expected: " << expected << std::endl;
	std::cout << "Actual: " << actual << std::endl;

	TEST_EQ( expected, actual );

	TESTCASE_END();
}

bool UtilTest::testDoubleRoundUp()
{
	double input = 2.6;
	double expected = 3.0;
	double actual = Arcemu::Shared::Util::round( input );

	std::cout << "Input: " << input << std::endl;
	std::cout << "Expected: " << expected << std::endl;
	std::cout << "Actual: " << actual << std::endl;

	TEST_EQ( expected, actual );

	TESTCASE_END();
}

bool UtilTest::testDoubleRoundDown()
{
	double input = 2.2;
	double expected = 2.0;
	double actual = Arcemu::Shared::Util::round( input );

	std::cout << "Input: " << input << std::endl;
	std::cout << "Expected: " << expected << std::endl;
	std::cout << "Actual: " << actual << std::endl;

	TEST_EQ( expected, actual );

	TESTCASE_END();
}

bool UtilTest::testDoubleRoundWhole()
{
	double input = 2.0;
	double expected = 2.0;
	double actual = Arcemu::Shared::Util::round( input );

	std::cout << "Input: " << input << std::endl;
	std::cout << "Expected: " << expected << std::endl;
	std::cout << "Actual: " << actual << std::endl;

	TEST_EQ( expected, actual );

	TESTCASE_END();
}

bool UtilTest::testLongDoubleRoundUp()
{
	long double input = 2.6;
	long double expected = 3.0;
	long double actual = Arcemu::Shared::Util::round( input );

	std::cout << "Input: " << input << std::endl;
	std::cout << "Expected: " << expected << std::endl;
	std::cout << "Actual: " << actual << std::endl;

	TEST_EQ( expected, actual );

	TESTCASE_END();
}

bool UtilTest::testLongDoubleRoundDown()
{
	long double input = 2.2;
	long double expected = 2.0;
	long double actual = Arcemu::Shared::Util::round( input );

	std::cout << "Input: " << input << std::endl;
	std::cout << "Expected: " << expected << std::endl;
	std::cout << "Actual: " << actual << std::endl;

	TEST_EQ( expected, actual );

	TESTCASE_END();
}

bool UtilTest::testLongDoubleRoundWhole()
{
	long double input = 2.0;
	long double expected = 2.0;
	long double actual = Arcemu::Shared::Util::round( input );

	std::cout << "Input: " << input << std::endl;
	std::cout << "Expected: " << expected << std::endl;
	std::cout << "Actual: " << actual << std::endl;

	TEST_EQ( expected, actual );

	TESTCASE_END();
}

bool UtilTest::run()
{
	RUN_TESTS();
}

int main( int argc, char *argv[] )
{
	RUN_TESTCLASS( UtilTest );
}
