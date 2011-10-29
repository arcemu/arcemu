/*
 * ArcScript Scripts for Arcemu MMORPG Server
 * Copyright (C) 2008-2011 Arcemu Team
 * Copyright (C) 2007 Moon++ <http://www.moonplusplus.com/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Setup.h"

bool Refocus(uint32 i, Spell* pSpell)
{
	Player* playerTarget = pSpell->GetPlayerTarget();
	if(playerTarget == 0) return true;

	SpellSet::const_iterator itr = playerTarget->mSpells.begin();
	for(; itr != playerTarget->mSpells.end(); ++itr)
	{
		if((*itr) == 24531)       // skip calling spell.. otherwise spammies! :D
			continue;

		if((*itr) == 19434 || (*itr) == 20900 || (*itr) == 20901 || (*itr) == 20902 || (*itr) == 20903 || (*itr) == 20904 || (*itr) == 27632
		        || (*itr) == 2643 || (*itr) == 14288 || (*itr) == 14289 || (*itr) == 14290 || (*itr) == 25294 || (*itr) == 14443 || (*itr) == 18651 || (*itr) == 20735 || (*itr) == 21390
		        || (*itr) == 1510 || (*itr) == 14294 || (*itr) == 14295 || (*itr) == 1540 || (*itr) == 22908
		        || (*itr) == 3044 || (*itr) == 14281 || (*itr) == 14282 || (*itr) == 14283 || (*itr) == 14284 || (*itr) == 14285 || (*itr) == 14286 || (*itr) == 14287)
			playerTarget->ClearCooldownForSpell((*itr));
	}
	return true;
}

bool Readiness(uint32 i, Spell* pSpell)
{
	if(!pSpell->p_caster)
		return true;
	pSpell->p_caster->ClearCooldownsOnLine(50 , pSpell->GetProto()->Id);//Beast Mastery
	pSpell->p_caster->ClearCooldownsOnLine(163, pSpell->GetProto()->Id);//Marksmanship
	pSpell->p_caster->ClearCooldownsOnLine(51 , pSpell->GetProto()->Id);//Survival
	return true;
}

bool MastersCall(uint32 i, Spell* pSpell)
{
	Player* caster = pSpell->p_caster;

	if(caster == NULL)
		return true;

	Pet* Summon = caster->GetSummon();
	if(Summon == NULL || Summon->IsDead())
		return true;

	switch(i)
	{
		case 0:
			Summon->CastSpell(caster, pSpell->damage, true);

			return true;

		case 1:
			Summon->CastSpell(Summon, 62305, true);

			return true;
	}

	return true;
}

bool TheBeastWithin(uint32 i, Aura* a, bool apply)
{
	Unit* m_target = a->GetTarget();

	uint32 mechanics[15] = { MECHANIC_CHARMED, MECHANIC_DISORIENTED,	MECHANIC_DISTRACED, MECHANIC_FLEEING,
	                         MECHANIC_ROOTED, MECHANIC_ASLEEP, MECHANIC_ENSNARED, MECHANIC_STUNNED,
	                         MECHANIC_FROZEN, MECHANIC_INCAPACIPATED, MECHANIC_POLYMORPHED, MECHANIC_BANISHED,
	                         MECHANIC_SEDUCED, MECHANIC_HORRIFIED, MECHANIC_SAPPED
	                       };

	for(uint32 x = 0; x < 15; x++)
	{
		if(apply)
		{
			m_target->MechanicsDispels[ mechanics[x] ]++;
			m_target->RemoveAllAurasByMechanic(mechanics[x], (uint32)(-1), false);
		}
		else
			m_target->MechanicsDispels[ mechanics[x] ]--;
	}

	return true;
}


bool BestialWrath(uint32 i, Aura* a, bool apply)
{
	Unit* m_target = a->GetTarget();

	uint32 mechanics[15] = { MECHANIC_CHARMED, MECHANIC_DISORIENTED,	MECHANIC_DISTRACED, MECHANIC_FLEEING,
	                         MECHANIC_ROOTED, MECHANIC_ASLEEP, MECHANIC_ENSNARED, MECHANIC_STUNNED,
	                         MECHANIC_FROZEN, MECHANIC_INCAPACIPATED, MECHANIC_POLYMORPHED, MECHANIC_BANISHED,
	                         MECHANIC_SEDUCED, MECHANIC_HORRIFIED, MECHANIC_SAPPED
	                       };

	for(uint32 x = 0; x < 15; x++)
	{
		if(apply)
		{
			m_target->MechanicsDispels[ mechanics[x] ]++;
			m_target->RemoveAllAurasByMechanic(mechanics[x], (uint32)(-1), false);
		}
		else
			m_target->MechanicsDispels[ mechanics[x] ]--;
	}
	return true;
}

bool Misdirection(uint32 i, Aura* a, bool apply)
{
	Player* caster = a->GetPlayerCaster();

	if(caster == NULL)
		return true;

	if(!apply)
		sEventMgr.AddEvent(caster, &Player::SetMisdirectionTarget, (uint64)0, EVENT_UNK, 250, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

	return true;
}

bool ExplosiveShot(uint32 i, Aura* a, bool apply)
{
	if(!apply)
		return true;

	Unit* m_target = a->GetTarget();

	a->SetNegative();
	int32 dmg	= a->GetModAmount(i);
	dmg += float2int32(m_target->GetRangedAttackPower() * 0.16f);

	a->EventPeriodicDamage(dmg);

	return true;
}

class HasNameHash : public AuraCondition{
public:
	bool operator()( Aura *aura ){
		uint32 namehash = aura->GetSpellProto()->NameHash;
		
		if( std::find( hashes.begin(), hashes.end(), namehash ) != hashes.end() )
			return true;
		else
			return false;
	}

	void AddHashToCheck( uint32 hash ){
		hashes.push_back( hash );
	}

private:
	std::vector< uint32 > hashes;
};

class ChimeraShotAction : public AuraAction{
public:
	void operator()( Aura *a ){
		uint32 namehash = a->GetSpellProto()->NameHash;

		Unit *caster = a->GetUnitCaster();
		Unit *target = a->GetTarget();

		if( caster == NULL )
			return;

		a->Refresh();

		switch( namehash ){
			case SPELL_HASH_SCORPID_STING:
				caster->CastSpell( target, 53359, true );
				break;

			case SPELL_HASH_WYVERN_STING:
				caster->CastSpell( target, 53366, true );
				break;

			case SPELL_HASH_SERPENT_STING:
				caster->CastSpell( target, 53353, true );
				break;

			case SPELL_HASH_VIPER_STING:
				caster->CastSpell( target, 53358, true );
				break;
		}
	}
};

bool ChimeraShot( uint32 i, Spell *spell ){
	Unit *target = spell->GetUnitTarget();

	HasNameHash condition;
	ChimeraShotAction action;

	condition.AddHashToCheck( SPELL_HASH_SCORPID_STING );
	condition.AddHashToCheck( SPELL_HASH_WYVERN_STING );
	condition.AddHashToCheck( SPELL_HASH_SERPENT_STING );
	condition.AddHashToCheck( SPELL_HASH_VIPER_STING );

	target->AuraActionIf( &action, &condition );

	return true;
}

void SetupHunterSpells(ScriptMgr* mgr)
{
	mgr->register_dummy_spell( 53209, &ChimeraShot );
	mgr->register_dummy_spell(24531, &Refocus);
	mgr->register_dummy_spell(23989, &Readiness);
	mgr->register_dummy_spell(53271, &MastersCall);
	mgr->register_dummy_aura(19574, &BestialWrath);
	mgr->register_dummy_aura(34471, &TheBeastWithin);
	mgr->register_dummy_aura(34477, &Misdirection);

	uint32 explosiveshotids[] =
	{
		53301,
		60051,
		60052,
		60053,
		0
	};
	mgr->register_dummy_aura(explosiveshotids, &ExplosiveShot);
}
