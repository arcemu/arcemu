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
 
#ifndef TESTMACROS_H
#define TESTMACROS_H

#include <iostream>

#define TESTCASE_REGISTRY_FOR( TESTCLASS ) \
	typedef bool (TESTCLASS::*TestMethod)(); \
	\
	struct TestEntry{ \
	TestMethod method; \
	const char *description; }; \
	\
	static TestEntry testEntries[] = {

#define TESTCASE( CLASS, METHOD, DESCRIPTION ) { &CLASS::METHOD, DESCRIPTION },

#define TESTCASE_REGISTRY_END() };

#define TEST_EQ( EXPECTED, ACTUAL ); if( EXPECTED != ACTUAL ){ \
	std::cout << "Equality test failed: TEST_EQ( " << #EXPECTED << ", " << #ACTUAL << ")" << std::endl; \
	std::cout << "With expansion: TEST_EQ( " << EXPECTED << ", " << ACTUAL << " )" << std::endl; \
	return false; }

#define TESTCASE_END(); return true;

#define RUN_TESTS(); \
	int count = sizeof( testEntries ) / sizeof( TestEntry ); \
	for( int i = 0; i < count; i++ ){ \
	const char *description = testEntries[ i ].description; \
	TestMethod testMethod = testEntries[ i ].method; \
	std::cout << "Testcase description: " << description << std::endl; \
	if( ! (this->*(testMethod))() ){ std::cout << "FAILED" << std::endl; return false; } \
	std::cout << "PASSED" << std::endl; \
	std::cout << std::endl; \
	} \
	return true;

#define RUN_TESTCLASS( CLASS ) std::cout << "Running " << #CLASS << "..." << std::endl; \
	std::cout << std::endl; \
	CLASS test; \
	if( test.run() ) return 0; else return 1;

#endif
