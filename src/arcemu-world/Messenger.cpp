/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2019 <http://www.ArcEmu.org/>
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

#include "StdAfx.h"
#include "Messenger.h"

void Messenger::SendSpellLog(Object *Caster, Object *Target, uint32 Ability, uint8 SpellLogType)
{
	if(Caster == NULL || Target == NULL || Ability == 0)
		return;

	WorldPacket data(SMSG_SPELLLOGMISS, 26);

	data << uint32(Ability);					// spellid
	data << Caster->GetGUID();					// caster / player
	data << uint8(1);							// unknown but I think they are const
	data << uint32(1);						// unknown but I think they are const
	data << Target->GetGUID();					// target
	data << uint8(SpellLogType);				// spelllogtype

	Caster->SendMessageToSet(&data, true);
}

void Messenger::SendSpellNonMeleeDamageLog(Object* Caster, Object* Target, uint32 SpellID, uint32 Damage, uint8 School, uint32 AbsorbedDamage, uint32 ResistedDamage, bool PhysicalDamage, uint32 BlockedDamage, bool CriticalHit, bool bToset)
{
	if(!Caster || !Target || !SpellID)
		return;

	uint32 Overkill = 0;

	if(Damage > Target->GetUInt32Value(UNIT_FIELD_HEALTH))
		Overkill = Damage - Target->GetUInt32Value(UNIT_FIELD_HEALTH);

	WorldPacket data(SMSG_SPELLNONMELEEDAMAGELOG, 48);

	data << Target->GetNewGUID();
	data << Caster->GetNewGUID();
	data << uint32(SpellID);                      // SpellID / AbilityID
	data << uint32(Damage);                       // All Damage
	data << uint32(Overkill);					// Overkill
	data << uint8(g_spellSchoolConversionTable[School]);     // School
	data << uint32(AbsorbedDamage);               // Absorbed Damage
	data << uint32(ResistedDamage);               // Resisted Damage
	data << uint8(PhysicalDamage);        // Physical Damage (true/false)
	data << uint8(0);                     // unknown or it binds with Physical Damage
	data << uint32(BlockedDamage);		       // Physical Damage (true/false)

	// unknown const
	if(CriticalHit)
		data << uint8(7);
	else
		data << uint8(5);

	data << uint32(0);

	Caster->SendMessageToSet(&data, bToset);
}

void Messenger::SendAttackerStateUpdate( Object* Caster, Object* Target, dealdamage* Dmg, uint32 Damage, uint32 Abs, uint32 BlockedDamage, uint32 HitStatus, uint32 VState )
{
	if(!Caster || !Target || !Dmg)
		return;

	WorldPacket data(SMSG_ATTACKERSTATEUPDATE, 70);

	uint32 Overkill = 0;

	if(Damage > Target->GetUInt32Value(UNIT_FIELD_MAXHEALTH))
		Overkill = Damage - Target->GetUInt32Value(UNIT_FIELD_HEALTH);

	data << uint32(HitStatus);
	data << Caster->GetNewGUID();
	data << Target->GetNewGUID();

	data << uint32(Damage);						// Realdamage
	data << uint32(Overkill);					// Overkill
	data << uint8(1);					// Damage type counter / swing type

	data << uint32(g_spellSchoolConversionTable[Dmg->school_type]);				    // Damage school
	data << float(Dmg->full_damage);	// Damage float
	data << uint32(Dmg->full_damage);	// Damage amount

	if(HitStatus & HITSTATUS_ABSORBED)
	{
		data << uint32(Abs);				// Damage absorbed
	}

	if(HitStatus & HITSTATUS_RESIST)
	{
		data << uint32(Dmg->resisted_damage);	// Damage resisted
	}

	data << uint8(VState);
	data << uint32(0);				// can be 0,1000 or -1
	data << uint32(0);

	if(HitStatus & HITSTATUS_BLOCK)
	{
		data << uint32(BlockedDamage);		// Damage amount blocked
	}


	if(HitStatus & HITSTATUS_UNK2)
	{
		data << uint32(0);				// unknown
	}

	if(HitStatus & HITSTATUS_UNK)
	{
		data << uint32(0);
		data << float(0);
		data << float(0);
		data << float(0);
		data << float(0);
		data << float(0);
		data << float(0);
		data << float(0);
		data << float(0);

		data << float(0);   // Found in loop
		data << float(0);	// Found in loop
		data << uint32(0);
	}

	Caster->SendMessageToSet(&data, Caster->IsPlayer());
}
