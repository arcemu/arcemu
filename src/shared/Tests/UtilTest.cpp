/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2012 <http://www.ArcEmu.org/>
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

#include <iostream>

class UtilTest
{
public:
	bool testArcemuToLower();	
	bool testArcemuToLowerWithLowerCase();
	bool testArcemuToUpper();
	bool testArcemuToUpperWithUpperCase();
	bool run();
};

typedef bool (UtilTest::*UtilTestMethod)();

static UtilTestMethod testMethods[] = 
{
	&UtilTest::testArcemuToLower,
	&UtilTest::testArcemuToLowerWithLowerCase,
	&UtilTest::testArcemuToUpper,
	&UtilTest::testArcemuToUpperWithUpperCase
};

bool UtilTest::testArcemuToLower()
{
	std::cout << "Test that arcemu_TOLOWER converts uppercase to lowercase." << std::endl;

	std::string actual = "UPPERCASE STRING";
	std::string original = actual;
	std::string expected = "uppercase string";

	Arcemu::Shared::Util::arcemu_TOLOWER( actual );

	std::cout << "Original: " << original << std::endl;
	std::cout << "Expected: " << expected << std::endl;
	std::cout << "Actual: " << actual << std::endl;
	std::cout << std::endl;

	if( expected.compare( actual ) == 0 )
		return true;
	else
		return false;
}

bool UtilTest::testArcemuToLowerWithLowerCase()
{
	std::cout << "Test that arcemu_TOLOWER converts leaves lowercase." << std::endl;

	std::string actual = "lowercase string";
	std::string original = actual;
	std::string expected = original;

	Arcemu::Shared::Util::arcemu_TOLOWER( actual );

	std::cout << "Original: " << original << std::endl;
	std::cout << "Expected: " << expected << std::endl;
	std::cout << "Actual: " << actual << std::endl;
	std::cout << std::endl;

	if( expected.compare( actual ) == 0 )
		return true;
	else
		return false;
}

bool UtilTest::testArcemuToUpper()
{
	std::cout << "Test that arcemu_TOUPPER converts lowercase to uppercase." << std::endl;

	std::string actual = "lowercase string";
	std::string original = actual;
	std::string expected = "LOWERCASE STRING";

	Arcemu::Shared::Util::arcemu_TOUPPER( actual );

	std::cout << "Original: " << original << std::endl;
	std::cout << "Expected: " << expected << std::endl;
	std::cout << "Actual: " << actual << std::endl;
	std::cout << std::endl;

	if( expected.compare( actual ) == 0 )
		return true;
	else
		return false;
}

bool UtilTest::testArcemuToUpperWithUpperCase()
{
	std::cout << "Test that arcemu_TOUPPER leaves uppercase as is" << std::endl;

	std::string actual = "UPPERCASE STRING";
	std::string original = actual;
	std::string expected = original;

	Arcemu::Shared::Util::arcemu_TOUPPER( actual );

	std::cout << "Original: " << original << std::endl;
	std::cout << "Expected: " << expected << std::endl;
	std::cout << "Actual: " << actual << std::endl;
	std::cout << std::endl;

	if( expected.compare( actual ) == 0 )
		return true;
	else
		return false;
}

bool UtilTest::run()
{
	int count = sizeof( testMethods ) / sizeof( UtilTestMethod );

	for( int i = 0; i < count; i++ )
	{
		if( ! (this->*(testMethods[ i ]))() )
			return false;
	}

	return true;
}

int main( int argc, char *argv[] )
{
	std::cout << "Running UtilTest..." << std::endl;
	std::cout << std::endl;

	UtilTest test;
	if( test.run() )
		return 0;
	else
		return 1;
}
