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

#ifndef TOTEMSUMMON_HPP_
#define TOTEMSUMMON_HPP_

///////////////////////////////////////////////////////////
//class TotemSummon
//  Class that implements Totems
//  Totems are stationary, and don't attack with melee,
//  however they can cast spells
//
//
///////////////////////////////////////////////////////////
class TotemSummon : public Summon
{
	public:
		TotemSummon(uint64 GUID);


		~TotemSummon();


		void Load(CreatureProto* proto, Unit* owner, LocationVector & position, uint32 spellid, int32 summonslot);


		void OnPushToWorld();


		void OnPreRemoveFromWorld();


		bool IsTotem() { return true; }


		Group* GetGroup();


		//////////////////////////////////////////////////////
		//void SetupSpells()
		//  Sets up the spells the totem will cast
		//  This code was almost directly copied from SpellEffects.cpp,
		//  it requires further refactoring!
		//  For example totems should cast like other units..
		//
		//Parameter(s)
		//  None
		//
		//Return Value
		//  None
		//
		//
		//////////////////////////////////////////////////////
		void SetupSpells();


		void Die(Unit* pAttacker, uint32 damage, uint32 spellid);


	private:
};

#endif
