/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2011 <http://www.ArcEmu.org/>
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


#include "../../StdAfx.h"

CompanionSummon::CompanionSummon(uint64 GUID) : Summon(GUID)
{
}

CompanionSummon::~CompanionSummon()
{
}

void CompanionSummon::Load(CreatureProto* proto, Unit* owner, LocationVector & position, uint32 spellid, int32 summonslot)
{
	Summon::Load(proto, owner, position, spellid, summonslot);

	SetFaction(35);
	setLevel(1);
	m_aiInterface->Init(this, AITYPE_PET, MOVEMENTTYPE_NONE, owner);
	m_aiInterface->SetUnitToFollow(owner);
	m_aiInterface->SetUnitToFollowAngle(-M_PI_FLOAT / 2);
	m_aiInterface->SetFollowDistance(3.0f);
	m_aiInterface->disable_melee = true;
	bInvincible = true;

	RemovePvPFlag();
	RemoveFFAPvPFlag();
}

void CompanionSummon::OnPushToWorld()
{
	Summon::OnPushToWorld();
}

void CompanionSummon::OnPreRemoveFromWorld()
{
	Summon::OnPreRemoveFromWorld();
}

