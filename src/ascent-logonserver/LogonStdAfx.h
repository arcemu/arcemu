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

#include "../ascent-shared/Log.h"
#include "../ascent-shared/Util.h"
#include "../ascent-shared/ByteBuffer.h"
#include "../ascent-shared/Config/ConfigEnv.h"
#include <zlib.h>

#include "../ascent-shared/Database/DatabaseEnv.h"
#include "../ascent-shared/Database/DBCStores.h"
#include "../ascent-shared/Database/dbcfile.h"

#include "../ascent-shared/Auth/BigNumber.h"
#include "../ascent-shared/Auth/Sha1.h"
#include "../ascent-shared/Auth/WowCrypt.h"
#include "../ascent-shared/CrashHandler.h"

#include "LogonOpcodes.h"
#include "../ascent-logonserver/Main.h"
#include "../ascent-world/NameTables.h"
#include "AccountCache.h"
#include "PeriodicFunctionCall_Thread.h"
#include "../ascent-logonserver/AutoPatcher.h"
#include "../ascent-logonserver/AuthSocket.h"
#include "../ascent-logonserver/AuthStructs.h"
#include "../ascent-logonserver/LogonOpcodes.h"
#include "../ascent-logonserver/LogonCommServer.h"
#include "../ascent-logonserver/LogonConsole.h"
#include "../ascent-shared/WorldPacket.h"

// database decl
extern Database* sLogonSQL;
