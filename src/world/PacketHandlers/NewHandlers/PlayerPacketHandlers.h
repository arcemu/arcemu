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

#ifndef PLAYERPACKETHANDLERS_H_
#define PLAYERPACKETHANDLERS_H_

#include "PacketHandlerMacros.h"

DEFINE_PACKET_HANDLER_CLASS( UnlearnSkillPacketHandler )
DEFINE_PACKET_HANDLER_CLASS( LearnMultipleTalentsPacketHandler )
DEFINE_PACKET_HANDLER_CLASS( LearnTalentPacketHandler )
DEFINE_PACKET_HANDLER_CLASS( UnlearnTalentsPacketHandler )
DEFINE_PACKET_HANDLER_CLASS( DissmissCritterPacketHandler )
DEFINE_PACKET_HANDLER_CLASS( QueryQuestPOIPacketHandler )
DEFINE_PACKET_HANDLER_CLASS( MirrorImagePacketHandler )
DEFINE_PACKET_HANDLER_CLASS( UseEquipmentSetPacketHandler )
DEFINE_PACKET_HANDLER_CLASS( SaveEquipmentSetPacketHandler )
DEFINE_PACKET_HANDLER_CLASS( DeleteEquipmentSetPacketHandler )

#endif
