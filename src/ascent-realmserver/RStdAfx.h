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

#include "../ascent-shared/Common.h"
#include "../ascent-shared/MersenneTwister.h"
#include "../ascent-shared/WorldPacket.h"
#include "../ascent-shared/Log.h"
#include "../ascent-shared/NGLog.h"
#include "../ascent-shared/ByteBuffer.h"
#include "../ascent-shared/StackBuffer.h"
#include "../ascent-shared/Config/ConfigEnv.h"
#include "../ascent-shared/crc32.h"
#include "../ascent-shared/LocationVector.h"

#include <zlib.h>

#include "../ascent-shared/Database/DatabaseEnv.h"
#include "../ascent-shared/Database/DBCStores.h"
#include "../ascent-shared/Database/dbcfile.h"
#include "../ascent-shared/FastQueue.h"

#include <Network/Network.h>

#include "../ascent-shared/Auth/MD5.h"
#include "../ascent-shared/Auth/BigNumber.h"
#include "../ascent-shared/Auth/Sha1.h"
#include "../ascent-shared/Auth/WowCrypt.h"
#include "../ascent-shared/CrashHandler.h"

#include "../ascent-world/Opcodes.h"
#include "../ascent-shared/CallBack.h"

#include "Master.h"
#include "WorkerOpcodes.h"
#include "WorkerServerSocket.h"
#include "Structures.h"
#include "WorldSocket.h"
#include "Session.h"
#include "WorkerServer.h"
#include "ClusterManager.h"
#include "ClientManager.h"
#include "LogonCommClient.h"
#include "LogonCommHandler.h"
#include "../ascent-shared/Storage.h"
#include "ObjectStorage.h"
