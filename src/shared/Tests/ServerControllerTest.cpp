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
#include "System/Platform.h"
#include "Threading/Mutex.h"
#include "Server/ServerController.h"

class ServerControllerTest
{
public:
	bool testDefaultValues();
	bool testScheduleShutdown();

	bool testScheduleRestart();
	bool testCancelShutdown();
	bool testSetTimer();

	bool run();
};

TESTCASE_REGISTRY_FOR( ServerControllerTest )
	TESTCASE( ServerControllerTest, testDefaultValues, "Default values are appropriate (no shutdown or restart scheduled)" )
	TESTCASE( ServerControllerTest, testScheduleShutdown, "Schedule shutdown works" )
	TESTCASE( ServerControllerTest, testScheduleRestart, "Schedule restart works" )
	TESTCASE( ServerControllerTest, testCancelShutdown, "Cancel shutdown works" )
	TESTCASE( ServerControllerTest, testSetTimer, "Set timer works" )
TESTCASE_REGISTRY_END()

bool ServerControllerTest::testDefaultValues()
{
	Arcemu::Shared::ServerController controller;
	Arcemu::Shared::ServerControlInfo info = controller.getServerControlInfo();

	TEST_EQ( false, info.shutdownEvent );
	TEST_EQ( false, info.restartEvent );
	TEST_EQ( 0, info.timer );

	TESTCASE_END();
}

bool ServerControllerTest::testScheduleShutdown()
{
	Arcemu::Shared::ServerController controller;
	controller.scheduleShutdown( 5000 );

	Arcemu::Shared::ServerControlInfo info = controller.getServerControlInfo();
	
	TEST_EQ( true, info.shutdownEvent );
	TEST_EQ( false, info.restartEvent );
	TEST_EQ( 5000, info.timer );
	TEST_EQ( true, controller.isBeingShutdown() );
	TEST_EQ( false, controller.isBeingRestarted() );

	TESTCASE_END();
}

bool ServerControllerTest::testScheduleRestart()
{
	Arcemu::Shared::ServerController controller;
	controller.scheduleRestart( 5000 );

	Arcemu::Shared::ServerControlInfo info = controller.getServerControlInfo();

	TEST_EQ( true, info.shutdownEvent );
	TEST_EQ( true, info.restartEvent );
	TEST_EQ( 5000, info.timer );
	TEST_EQ( true, controller.isBeingShutdown() );
	TEST_EQ( true, controller.isBeingRestarted() );

	TESTCASE_END();
}

bool ServerControllerTest::testCancelShutdown()
{
	Arcemu::Shared::ServerController controller;
	controller.scheduleShutdown( 5000 );
	controller.cancelShutdown();

	Arcemu::Shared::ServerControlInfo info = controller.getServerControlInfo();

	TEST_EQ( false, info.shutdownEvent );
	TEST_EQ( false, info.restartEvent );
	TEST_EQ( false, controller.isBeingShutdown() );
	TEST_EQ( false, controller.isBeingRestarted() );

	TESTCASE_END();
}

bool ServerControllerTest::testSetTimer()
{
	Arcemu::Shared::ServerController controller;
	controller.setTimer( 666 );
	
	Arcemu::Shared::ServerControlInfo info = controller.getServerControlInfo();
	TEST_EQ( false, info.shutdownEvent );
	TEST_EQ( false, info.restartEvent );
	TEST_EQ( false, controller.isBeingShutdown() );
	TEST_EQ( false, controller.isBeingRestarted() );
	TEST_EQ( 666, info.timer );

	TESTCASE_END();
}

bool ServerControllerTest::run()
{
	RUN_TESTS();
}

int main( int argc, char *argv[] )
{
	RUN_TESTCLASS( ServerControllerTest );
}
