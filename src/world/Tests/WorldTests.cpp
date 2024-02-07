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

int MathTestRun();
int PvPCaptureRateBonusCalculatorTestRun();
int WorldUtilTestRun();

typedef int (*TestRunnerFunction)();

static TestRunnerFunction testRunnerFunctions[] =
{
	MathTestRun,
	PvPCaptureRateBonusCalculatorTestRun,
	WorldUtilTestRun
};

int main(int arc, char *argv[])
{
	int count = sizeof( testRunnerFunctions ) / sizeof( TestRunnerFunction );

	for( int i = 0; i < count; i++ )
	{
		int (*f)() = testRunnerFunctions[ i ];
		int value = f();
		if( value != 0 )
		{
			return value;
		}
	}

	return 0;
}
