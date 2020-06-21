/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
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

#include "System/Platform.h"
#include "ServerController.h"
#include "Threading/Guard.h"

namespace Arcemu
{
	namespace Shared
	{
		ServerController::ServerController()
		{
			info.restartEvent = false;
			info.shutdownEvent = false;
			info.timer = 0;
		}

		void ServerController::scheduleShutdown( uint32 timer )
		{
			Guard guard( mutex );

			info.shutdownEvent = true;
			info.restartEvent = false;
			info.timer = timer;
		}

		void ServerController::cancelShutdown()
		{
			Guard guard( mutex );

			info.shutdownEvent = false;
			info.restartEvent = false;
			info.timer = 5000;
		}

		bool ServerController::isBeingShutdown()
		{
			Guard guard( mutex );

			return info.shutdownEvent;
		}

		void ServerController::scheduleRestart( uint32 timer )
		{
			Guard guard( mutex );

			info.shutdownEvent = true;
			info.restartEvent = true;
			info.timer = timer;
		}

		bool ServerController::isBeingRestarted()
		{
			Guard guard( mutex );

			return info.restartEvent;
		}

		ServerControlInfo ServerController::getServerControlInfo()
		{
			Guard guard( mutex );

			return info;
		}

		void ServerController::setTimer( uint32 timer )
		{
			Guard guard( mutex );

			info.timer = timer;
		}
	}
}
