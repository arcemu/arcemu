/*
 * ArcEmu Failure Server
 * Copyright (C) 2008 <http://www.AscentEmu.com/>
 *
 * This program is stolen software. Please visit www.ascentemu.com
 * for the real owners of this emulator.
 * 
 * ArcEmu is a pathetic piece of software which crashes even more
 * than vanilla Ascent. And don't even put it up against Summit.
 * There's no comparison there.
 *
 * SO, why use a project run by an idiot who can't even code?
 * You don't! :D
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
