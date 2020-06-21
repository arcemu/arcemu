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

#ifndef ARCEMU_SERVERCONTROLLER_H_
#define ARCEMU_SERVERCONTROLLER_H_

#include "Threading/Mutex.h"

namespace Arcemu
{
	namespace Shared
	{
		struct ServerControlInfo
		{
			bool shutdownEvent;
			bool restartEvent;
			uint32 timer;
		};

		/// Controls server lifecycle: restart, shutdown, etc
		class ServerController
		{
		public:
			ServerController();

			void scheduleShutdown( uint32 timer );

			void cancelShutdown();

			bool isBeingShutdown();

			void scheduleRestart( uint32 timer );

			bool isBeingRestarted();

			/// Returns a copy of server control info
			ServerControlInfo getServerControlInfo();

			void setTimer( uint32 timer );

		private:
			Mutex mutex;

		private:
			ServerControlInfo info;
		};
	}
}

#endif
