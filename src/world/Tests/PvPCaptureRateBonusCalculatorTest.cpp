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

#include "../../world/Game/Util/PvPCaptureRateBonusCalculator.h"

#include "../../testing/testmacros.h"

class PvPCaptureRateBonusCalculatorTest
{
public:
	bool testNoAdvantageReturnsZero();
	bool testOneAdvantageNoBonus();
	bool testMaxAdvantageMaxBonus();
	bool testOverMaxAdvantageMaxBonus();
	bool testHalfBonus();
	bool run();
};

TESTCASE_REGISTRY_FOR( PvPCaptureRateBonusCalculatorTest )
	TESTCASE( PvPCaptureRateBonusCalculatorTest, testNoAdvantageReturnsZero,     "Test that no advantage returns a factor of 0.0f" )
	TESTCASE( PvPCaptureRateBonusCalculatorTest, testOneAdvantageNoBonus,        "Test that an advantage of 1 yields no bonus ( factor 1.0 )" )
	TESTCASE( PvPCaptureRateBonusCalculatorTest, testMaxAdvantageMaxBonus,       "Test that max advantage returns max bonus ( factor of 2.0 )" )
	TESTCASE( PvPCaptureRateBonusCalculatorTest, testOverMaxAdvantageMaxBonus,   "Test that advantage over the max advantage returns max bonus ( factor of 2.0 )" )
	TESTCASE( PvPCaptureRateBonusCalculatorTest, testHalfBonus,                  "Test that ~half of the max advantage returns half bonus  ( factor of 1.5 )" )
TESTCASE_REGISTRY_END()

bool PvPCaptureRateBonusCalculatorTest::testNoAdvantageReturnsZero()
{
	PvPCaptureRateBonusFactorCalculator bonusCalculator( 5 );

	TEST_EQ( 0.0f, bonusCalculator.calculateBonusFactor( 0 ) );

	TESTCASE_END();
}

bool PvPCaptureRateBonusCalculatorTest::testOneAdvantageNoBonus()
{
	PvPCaptureRateBonusFactorCalculator bonusCalculator( 5 );

	TEST_EQ( 1.0f, bonusCalculator.calculateBonusFactor( 1 ) );

	TESTCASE_END();
}

bool PvPCaptureRateBonusCalculatorTest::testMaxAdvantageMaxBonus()
{
	PvPCaptureRateBonusFactorCalculator bonusCalculator( 5 );

	TEST_EQ( 2.0f, bonusCalculator.calculateBonusFactor( 5 ) );

	TESTCASE_END();
}

bool PvPCaptureRateBonusCalculatorTest::testOverMaxAdvantageMaxBonus()
{
	PvPCaptureRateBonusFactorCalculator bonusCalculator( 5 );

	TEST_EQ( 2.0f, bonusCalculator.calculateBonusFactor( 15 ) );

	TESTCASE_END();
}

bool PvPCaptureRateBonusCalculatorTest::testHalfBonus()
{
	PvPCaptureRateBonusFactorCalculator bonusCalculator( 5 );

	TEST_EQ( 1.5f, bonusCalculator.calculateBonusFactor( 3 ) );

	TESTCASE_END();
}

bool PvPCaptureRateBonusCalculatorTest::run()
{
	RUN_TESTS();
}

int PvPCaptureRateBonusCalculatorTestRun()
{
	RUN_TESTCLASS( PvPCaptureRateBonusCalculatorTest )
}
