/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008 <http://www.ArcEmu.org/>
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

#pragma once

#include <list>
#include <vector>
#include <map>
#include <sstream>
#include <string>
//#include <fstream>

#include "Common.h"
#include <Network/Network.h>

#include "../arcemu-shared/Log.h"
#include "../arcemu-shared/Util.h"
#include "../arcemu-shared/ByteBuffer.h"
#include "../arcemu-shared/Config/ConfigEnv.h"
#include <zlib.h>

#include "../arcemu-shared/Database/DatabaseEnv.h"
#include "../arcemu-shared/Database/DBCStores.h"
#include "../arcemu-shared/Database/dbcfile.h"

#include "../arcemu-shared/Auth/BigNumber.h"
#include "../arcemu-shared/Auth/Sha1.h"
#include "../arcemu-shared/Auth/WowCrypt.h"
#include "../arcemu-shared/CrashHandler.h"

#include "LogonOpcodes.h"
#include "../arcemu-logonserver/Main.h"
#include "../arcemu-world/NameTables.h"
#include "AccountCache.h"
#include "PeriodicFunctionCall_Thread.h"
#include "../arcemu-logonserver/AutoPatcher.h"
#include "../arcemu-logonserver/AuthSocket.h"
#include "../arcemu-logonserver/AuthStructs.h"
//#include "../arcemu-logonserver/LogonOpcodes.h"
#include "../arcemu-logonserver/LogonCommServer.h"
#include "../arcemu-logonserver/LogonConsole.h"
#include "../arcemu-shared/WorldPacket.h"

// database decl
extern Database* sLogonSQL;
