/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2012 <http://www.ArcEmu.org/>
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

#ifndef __SPELL_PROC_H
#define __SPELL_PROC_H

#include "StdAfx.h"

class SpellProc;
class Unit;

#define SPELL_PROC_FACTORY_FUNCTION(T) \
  public: \
	static SpellProc* Create() { return new T(); }

typedef SpellProc* (*spell_proc_factory_function)();

typedef HM_NAMESPACE::hash_map<uint32, spell_proc_factory_function>  SpellProcMap;

class SpellProc
{
	public:
		~SpellProc()
		{
		}

		// Returns true if this spell can proc, false otherwise
		virtual bool CanProc(Unit* victim, SpellEntry* CastingSpell)
		{
			return true;
		}

		// Called when procFlags is to be compared.
		// Return true on success, false otherwise
		virtual bool CheckProcFlags(uint32 flag)
		{
			if(mProcFlags & flag)
				return true;
			else
				return false;
		}

		// Check if this object is identified by method arguments, so it can be deleted
		virtual bool CanDelete(uint32 spellId, uint64 casterGuid = 0, uint64 misc = 0)
		{
			if(mSpell->Id == spellId && (casterGuid == 0 || mCaster == casterGuid) && !mDeleted)
				return true;

			return false;
		}

		// Called when is proccing from casting spell. It checks proc class mask with spell group type
		// Return true allow proc, false otherwise
		virtual bool CheckClassMask(Unit* victim, SpellEntry* CastingSpell)
		{
			if((mProcClassMask[0] == 0 && mProcClassMask[1] == 0 && mProcClassMask[2] == 0) ||
			        mProcClassMask[0] & CastingSpell->SpellGroupType[0] ||
			        mProcClassMask[1] & CastingSpell->SpellGroupType[1] ||
			        mProcClassMask[2] & CastingSpell->SpellGroupType[2])
				return true;
			else
				return false;
		}

		// Called after proc chance is rolled
		// Return false so Unit::HandleProc execute subsequent statements
		// Return true if this handle everything, so Unit::HandleProc skips to next iteration
		virtual bool DoEffect(Unit* victim, SpellEntry* CastingSpell, uint32 flag, uint32 dmg, uint32 abs, int* dmg_overwrite, uint32 weapon_damage_type)
		{
			return false;
		}

		// Called just after this object is created. Usefull for initialize object members
		virtual void Init(Object* obj)
		{
		}

		virtual uint32 CalcProcChance(Unit* victim, SpellEntry* CastingSpell);

		// Called when trying to proc on a triggered spell
		// Return true allow proc, false otherwise
		virtual bool CanProcOnTriggered(Unit* victim, SpellEntry* CastingSpell);

		// Cast proc spell
		virtual void CastSpell(Unit* victim, SpellEntry* CastingSpell, int* dmg_overwrite);

		// Spell to proc
		SpellEntry* mSpell;

		// Spell that created this proc
		SpellEntry* mOrigSpell;

		// Unit 'owner' of this proc
		Unit*  mTarget;

		// GUID of the caster of this proc
		uint64 mCaster;

		uint32 mProcChance;
		uint32 mProcFlags;
		uint32 mProcCharges;

		// Time of last time of proc
		uint32 mLastTrigger;

		// Mask used to compare with casting spell group_type
		uint32 mProcClassMask[3];

		// Mask used on spell effect
		uint32 mGroupRelation[3];

		// Indicate that this object is deleted, and should be remove on next iteration
		bool mDeleted;
};

class SpellProcMgr: public Singleton < SpellProcMgr >
{
	public:
		SpellProcMgr()
		{
			Setup();
		}

		~SpellProcMgr()
		{
		}

		SpellProc* NewSpellProc(Unit* target, uint32 spell_id, uint32 orig_spell_id, uint64 caster, uint32 procChance, uint32 procFlags, uint32 procCharges, uint32* groupRelation, uint32* procClassMask, Object* obj);

		SpellProc* NewSpellProc(Unit* target, SpellEntry* spell, SpellEntry* orig_spell, uint64 caster, uint32 procChance, uint32 procFlags, uint32 procCharges, uint32* groupRelation, uint32* procClassMask, Object* obj);

	private:

		SpellProcMap mSpellProc;
		SpellProcMap mSpellProcNameHash;

		void AddById(uint32 spellId, spell_proc_factory_function spell_proc)
		{
			mSpellProc.insert(make_pair(spellId, spell_proc));
		}

		void AddByNameHash(uint32 name_hash, spell_proc_factory_function spell_proc)
		{
			mSpellProcNameHash.insert(make_pair(name_hash, spell_proc));
		}

		void Setup();

		void SetupItems();
		void SetupDeathKnight();
		void SetupDruid();
		void SetupHunter();
		void SetupMage();
		void SetupPaladin();
		void SetupPriest();
		void SetupRogue();
		void SetupShamman();
		void SetupWarlock();
		void SetupWarrior();
};

#define sSpellProcMgr SpellProcMgr::getSingleton()

#endif
