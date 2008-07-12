/*
 * Moon++ Scripts for arcemu MMORPG Server
 * Copyright (C) 2005-2007 arcemu Team <http://www.arcemuemu.com/>
 * Copyright (C) 2007-2008 Moon++ Team <http://www.moonplusplus.info/>
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

#include "StdAfx.h"
#include "Setup.h"
#include "Base.h"

/************************************************************************/
/* Raid_TheEye.cpp Script												*/
/************************************************************************/

/****************/
/*    Trashes   */
/****************/

// APPRENTICE STAR SCRYER
#define CN_A_STAR_SCRYER	20043

#define ARCANE_VOLLEY		40424
#define A_STAR_ARCANE_EXPLOSION	27082

class AStarScryerAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(AStarScryerAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    AStarScryerAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(ARCANE_VOLLEY);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].cooldown = 15;
		spells[0].perctrigger = 50.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(A_STAR_ARCANE_EXPLOSION);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].cooldown = 5;
		spells[1].perctrigger = 50.0f;
		spells[1].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void OnTargetDied(Unit* mTarget)
    {
    }

    void OnCombatStop(Unit *mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		CastTime();
		RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
    }

	void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
						case TARGET_RANDOM_FRIEND:
						case TARGET_RANDOM_SINGLE:
						case TARGET_RANDOM_DESTINATION:
							CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<Unit*> TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					Unit* RandomTarget = NULL;
					RandomTarget = (Unit*)(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			Unit * RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}
	
protected:
	int nrspells;
};

// STAR SCRYER
#define CN_STAR_SCRYER 20034

#define STARFALL 37124

class StarScryerAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(StarScryerAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    StarScryerAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(STARFALL);
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = true;
		spells[0].cooldown = 12;
		spells[0].perctrigger = 50.0f;
		spells[0].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void OnTargetDied(Unit* mTarget)
    {
    }

    void OnCombatStop(Unit *mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		CastTime();
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
    }

	void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
						case TARGET_RANDOM_FRIEND:
						case TARGET_RANDOM_SINGLE:
						case TARGET_RANDOM_DESTINATION:
							CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<Unit*> TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					Unit* RandomTarget = NULL;
					RandomTarget = (Unit*)(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			Unit * RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}
	
protected:

	int nrspells;
};

// ASTROMANCER
#define CN_ASTROMANCER			20033

#define ASTROMANCER_FIREBALL_VOLLEY	 29922
#define ASTROMANCER_BLAST_WAVE 33933

class AstromancerAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(AstromancerAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    AstromancerAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(ASTROMANCER_FIREBALL_VOLLEY);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].cooldown = 15;
		spells[0].perctrigger = 50.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(ASTROMANCER_BLAST_WAVE);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].cooldown = 7;
		spells[1].perctrigger = 60.0f;
		spells[1].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void OnTargetDied(Unit* mTarget)
    {
    }

    void OnCombatStop(Unit *mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		CastTime();
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
    }

	void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
						case TARGET_RANDOM_FRIEND:
						case TARGET_RANDOM_SINGLE:
						case TARGET_RANDOM_DESTINATION:
							CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<Unit*> TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					Unit* RandomTarget = NULL;
					RandomTarget = (Unit*)(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			Unit * RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}
	
protected:
	int nrspells;
};

// ASTROMANCER LORD
#define CN_ASTROMANCER_LORD 20046

#define A_LORD_FIREBALL_VOLLEY 29922
#define BLAST_WEAVE	33933
#define DRAGONS_BREATH 33043

class AstromancerLordAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(AstromancerLordAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    AstromancerLordAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(A_LORD_FIREBALL_VOLLEY);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].cooldown = 15;
		spells[0].perctrigger = 50.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(BLAST_WEAVE);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].cooldown = 7;
		spells[1].perctrigger = 50.0f;
		spells[1].attackstoptimer = 1000;
			
		spells[2].info = dbcSpell.LookupEntry(DRAGONS_BREATH);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = true;
		spells[2].cooldown = 10;
		spells[2].perctrigger = 50.0f;
		spells[2].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void OnTargetDied(Unit* mTarget)
    {
    }

    void OnCombatStop(Unit *mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		CastTime();
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
    }

	void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
						case TARGET_RANDOM_FRIEND:
						case TARGET_RANDOM_SINGLE:
						case TARGET_RANDOM_DESTINATION:
							CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<Unit*> TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					Unit* RandomTarget = NULL;
					RandomTarget = (Unit*)(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			Unit * RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}
	
protected:
	int nrspells;
};

// BLOODWARDER VINDICATOR
#define CN_BLOOD_VINDICATOR	20032

#define HAMMER_OF_JUSTICE 32416
#define FLASH_HEAL 38588

class BloodVindicatorAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(BloodVindicatorAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    BloodVindicatorAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(HAMMER_OF_JUSTICE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = 10;
		spells[0].perctrigger = 50.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(FLASH_HEAL);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = false;
		spells[1].cooldown = 15;
		spells[1].perctrigger = 50.0f;
		spells[1].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void OnTargetDied(Unit* mTarget)
    {
    }

    void OnCombatStop(Unit *mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		CastTime();
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
    }

	void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
						case TARGET_RANDOM_FRIEND:
						case TARGET_RANDOM_SINGLE:
						case TARGET_RANDOM_DESTINATION:
							CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<Unit*> TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					Unit* RandomTarget = NULL;
					RandomTarget = (Unit*)(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			Unit * RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}
	
protected:
	int nrspells;
};

// BLOODWARDER LEGIONNARE
#define CN_BLOOD_LEGIONNARE 20031

#define LEGION_WHIRLWIND 36981
#define CLEAVE	26350

class BloodLegionnareAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(BloodLegionnareAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    BloodLegionnareAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(LEGION_WHIRLWIND);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = 15;
		spells[0].perctrigger = 50.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(CLEAVE);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].cooldown = 10;
		spells[1].perctrigger = 40.0f;
		spells[1].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void OnTargetDied(Unit* mTarget)
    {
    }

    void OnCombatStop(Unit *mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		CastTime();
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
    }

	void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
						case TARGET_RANDOM_FRIEND:
						case TARGET_RANDOM_SINGLE:
						case TARGET_RANDOM_DESTINATION:
							CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<Unit*> TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					Unit* RandomTarget = NULL;
					RandomTarget = (Unit*)(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			Unit * RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}
	
protected:
	int nrspells;
};

// BLOODWARDER MARSHAL
#define CN_BLOOD_MARSHAL 20035

#define WHIRLWIND		36981
#define UPPERCUT	26007

class BloodMarshalAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(BloodMarshalAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    BloodMarshalAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(WHIRLWIND);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = 15;
		spells[0].perctrigger = 50.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(UPPERCUT);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].cooldown = 20;
		spells[1].perctrigger = 50.0f;
		spells[1].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void OnTargetDied(Unit* mTarget)
    {
    }

    void OnCombatStop(Unit *mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		CastTime();
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
    }

	void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
						case TARGET_RANDOM_FRIEND:
						case TARGET_RANDOM_SINGLE:
						case TARGET_RANDOM_DESTINATION:
							CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<Unit*> TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					Unit* RandomTarget = NULL;
					RandomTarget = (Unit*)(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			Unit * RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}
	
protected:
	int nrspells;
};

// PHOENIX HAWK
#define CN_PHOENIX_HAWK 20039

#define MANA_BURN 25380
#define HAWK_CHARGE	16636

class PhoenixHawkAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(PhoenixHawkAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    PhoenixHawkAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(MANA_BURN);
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = true;
		spells[0].cooldown = 15;
		spells[0].perctrigger = 50.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(HAWK_CHARGE);
		spells[1].targettype = TARGET_RANDOM_SINGLE;
		spells[1].instant = true;
		spells[1].cooldown = 12;
		spells[1].perctrigger = 50.0f;
		spells[1].attackstoptimer = 1000;
		spells[1].mindist2cast = 15;
		spells[1].maxdist2cast = 45;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void OnTargetDied(Unit* mTarget)
    {
    }

    void OnCombatStop(Unit *mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		CastTime();
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
    }

	void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
						case TARGET_RANDOM_FRIEND:
						case TARGET_RANDOM_SINGLE:
						case TARGET_RANDOM_DESTINATION:
							CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<Unit*> TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					Unit* RandomTarget = NULL;
					RandomTarget = (Unit*)(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			Unit * RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}
	
protected:
	int nrspells;
};

// CRYSTALCORE SENTINEL
#define CN_CRYSTAL_SENTINEL 20041

#define TRAMPLE 40340
#define CHARGET_ARCANE_EXPLOSION 37106

class CrystalSentinelAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(CrystalSentinelAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    CrystalSentinelAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(TRAMPLE);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].cooldown = 15;
		spells[0].perctrigger = 50.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(CHARGET_ARCANE_EXPLOSION);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = false;
		spells[1].cooldown = 20;
		spells[1].perctrigger = 50.0f;
		spells[1].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void OnTargetDied(Unit* mTarget)
    {
    }

    void OnCombatStop(Unit *mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		CastTime();
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
    }

	void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
						case TARGET_RANDOM_FRIEND:
						case TARGET_RANDOM_SINGLE:
						case TARGET_RANDOM_DESTINATION:
							CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<Unit*> TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					Unit* RandomTarget = NULL;
					RandomTarget = (Unit*)(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			Unit * RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}
	
protected:
	int nrspells;
};

// CRYSTALCORE MECHANIC
#define CN_CRYSTAL_MECHANIC 20052

#define SAW_BLADE 37123

class CrystalMechanicAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(CrystalMechanicAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    CrystalMechanicAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(SAW_BLADE);
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = true;
		spells[0].cooldown = 6;
		spells[0].perctrigger = 100.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].mindist2cast = 0.0f;
		spells[0].maxdist2cast = 40.0f;
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void OnTargetDied(Unit* mTarget)
    {
    }

    void OnCombatStop(Unit *mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		CastTime();
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
    }

	void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
						case TARGET_RANDOM_FRIEND:
						case TARGET_RANDOM_SINGLE:
						case TARGET_RANDOM_DESTINATION:
							CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<Unit*> TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					Unit* RandomTarget = NULL;
					RandomTarget = (Unit*)(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			Unit * RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}
	
protected:
	int nrspells;
};


/****************/
/*    Bosses    */
/****************/

//------------------------------------
//	-= Void Reaver =-
//------------------------------------

#define CN_VOID_REAVER 19516
#define CN_ARCANEORBTARGET 19577

#define POUNDING 34164
#define ARCANE_ORB 34190
#define ARCANE_ORB_TRIGGER 34172
#define KNOCK_AWAY 25778 //21737
#define ENRAGE 27680 // Needs checking (as it can be wrong [or maybe IS wrong])

class VoidReaverAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(VoidReaverAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    VoidReaverAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

        spells[0].info = dbcSpell.LookupEntry(POUNDING);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].cooldown = 12;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(KNOCK_AWAY);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].cooldown = 20;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;
	} 

    void OnCombatStart(Unit* mTarget)
    {
		ResetCastTime();
		EnrageTimer = 600; //10 minutes
		ArcaneOrbTimer = 10; //first cast after 10 seconds
		Enraged = false;
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Alert, you are marked for extermination!");
		_unit->PlaySoundToSet(11213);
        RegisterAIUpdateEvent(1000);
    }

	void OnTargetDied(Unit* mTarget)
    {
		if (_unit->GetHealthPct() > 0)	// Hack to prevent double yelling (OnDied and OnTargetDied when creature is dying)
		{
			switch (rand()%3)
			{
			case 0: 
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Extermination, successful.");
				_unit->PlaySoundToSet(11215);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Imbecile life form, no longer functional.");
				_unit->PlaySoundToSet(11216);
				break;
			case 2:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Threat neutralized.");
				_unit->PlaySoundToSet(11217);
				break;
			}
		}
    }

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Systems... shutting... down...");
        _unit->PlaySoundToSet(11214);
       RemoveAIUpdateEvent();
    }

	void ResetCastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

    void AIUpdate()
    {
		//Enrage
		if(!Enraged)
		{
			EnrageTimer--;
			if(!EnrageTimer)
			{
				_unit->CastSpell(_unit, dbcSpell.LookupEntry(ENRAGE), true);
				Enraged = true;
			}
		}

		//Arcane Orb
		//6+k (on cloth) AoE with 6s silence, randomly targeted at the place where a non-melee player is standing (resistable, binary), 3 sec cooldown
		ArcaneOrbTimer--;
		if(!ArcaneOrbTimer)
		{
			Unit* RandomTarget = NULL;
			std::vector<Unit*> TargetTable;
			for(set<Player*>::iterator itr = _unit->GetInRangePlayerSetBegin(); itr != _unit->GetInRangePlayerSetEnd(); ++itr) 
			{ 
				if ((*itr)->GetTypeId() == TYPEID_PLAYER && (*itr)->GetInstanceID() == _unit->GetInstanceID() && (*itr)->GetDistance2dSq(_unit) > 225) //15 yards
				{
					RandomTarget = (Unit*)(*itr);

					if (RandomTarget && RandomTarget->isAlive())
						TargetTable.push_back(RandomTarget);
				}
			}

			//if there isn't any target, take the tank
			if(!TargetTable.size())
				TargetTable.push_back(_unit->GetAIInterface()->GetNextTarget());

			RandomTarget = *(TargetTable.begin()+rand()%TargetTable.size());
			if(RandomTarget)
			{
				//1) spawn a trigger
				_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ARCANEORBTARGET, RandomTarget->GetPositionX(), RandomTarget->GetPositionY(), RandomTarget->GetPositionZ(), 0, false, false, 0, 0);

				//2) send the missile
				_unit->CastSpellAoF(RandomTarget->GetPositionX(), RandomTarget->GetPositionY(), RandomTarget->GetPositionZ(), dbcSpell.LookupEntry(ARCANE_ORB_TRIGGER), true);
			}
			ArcaneOrbTimer = 3; //3secs
		}

		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
    }

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(spells[i].cooldown > 0)
					spells[i].casttime--;

				if (m_spellcheck[i])
				{
					if(spells[i].cooldown > 0)
						spells[i].casttime = spells[i].cooldown;

					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}

					//pounding yells
					//this is a dirty method but at the moment we don't support multiple spell yells
					if(i == 0)
					{
						switch(rand()%3)
						{
						case 0:
							_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Alternative measure commencing...");
							_unit->PlaySoundToSet(11218); 
							break;

						case 1:
							_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Calculating force parameters...");
							_unit->PlaySoundToSet(11219); 
							break;

						//case 2: doesn't yell
						}
					}

					m_spellcheck[i] = false;
					return;
				}

				if((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

protected:
	int nrspells;
	uint32 EnrageTimer;
	uint32 ArcaneOrbTimer;
	bool Enraged;
};

//Arcane Orb Target
class ArcaneOrbTargetAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(ArcaneOrbTargetAI);

    ArcaneOrbTargetAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		_unit->GetAIInterface()->m_canMove = false;

		//explode in some seconds
		//TODO: On official servers it explodes exactly when arcane orb trigger reaches it
		sEventMgr.AddEvent(((Unit*)_unit), &Unit::EventCastSpell, ((Unit*)_unit), dbcSpell.LookupEntry(ARCANE_ORB), EVENT_UNK, 3000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		_unit->Despawn(3200, 0);
	}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//High Astromancer Solarian AI Script
//
// Phase timers based on boss mods:
// - Split every 90sec (except first split happens 50sec after engage)
// - 3x4 Solarium Agents spawns 6sec after split
// - Solarian comes back with two Solarium Priest 22sec after split (end of phase 2)
// - Once phase 2 is finished, phase 1 starts again
// - At 20% health, Solarian enter phase 3 until she dies
//
#define CN_SOLARIAN								18805
#define CN_SOLARIUMAGENT						18925
#define CN_SOLARIUMPRIEST						18806
#define CN_SOLARIUM_SPOT_LIGHT					15631
#define SOLARIAN_WRATH_OF_THE_ASTROMANCER		42783	//Infuses an enemy with Arcane power, causing them to harm nearby allies for 5400 to 6600. Arcane damage after 6 sec.
#define SOLARIAN_WRATH_OF_THE_ASTROMANCER_BOMB	42787	//The actual spell that triggers the explosion with arcane damage and slow fall
#define SOLARIAN_ARCANE_MISSILES				33031	//Launches magical missiles at an enemy, inflicting Arcane damage each second for 3 sec. Trigger spell (3000 arcane damage)
#define SOLARIAN_BLINDING_LIGHT					33009	//Hits everyone in the raid for 2280 to 2520 arcane damage. 20sec cooldown.
#define SOLARIAN_SOLARIANS_TRANSFORM			39117	//Transforms into void walker.
#define SOLARIAN_VOID_BOLT						39329	//The Void Walker casts this every 10 seconds. It deals 4394 to 5106 shadow damage to the target with the highest aggro.
#define SOLARIAN_PSYCHIC_SCREAM					34322	//Fears up to 5 targets in melee range.
#define SOLARIUMPRIEST_GREATER_HEAL				38580	//Heals 23125 to 26875 any friendly target
#define SOLARIUMPRIEST_HOLY_SMITE				31740	//Deals 553 to 747 holy damage

bool Dummy_Solarian_WrathOfTheAstromancer(uint32 pEffectIndex, Spell* pSpell);
void SpellFunc_Solarian_Disappear(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType);
void SpellFunc_Solarian_Reappear(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType);

class HighAstromancerSolarianAI : public MoonScriptBossAI
{
    MOONSCRIPT_FACTORY_FUNCTION(HighAstromancerSolarianAI, MoonScriptBossAI);
	HighAstromancerSolarianAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
	{
		//Initialize timers
		mSplitTimer = mAgentsTimer = mSolarianTimer = INVALIDATE_TIMER;

		//Phase 1 spells
		AddPhaseSpell(1, AddSpell(SOLARIAN_ARCANE_MISSILES, Target_RandomUnit, 60, 3, 0, 0, 45));
		AddPhaseSpell(1, AddSpell(SOLARIAN_WRATH_OF_THE_ASTROMANCER, Target_RandomPlayerNotCurrent, 20, 0, 6, 0, 50000));
		AddPhaseSpell(1, AddSpell(SOLARIAN_BLINDING_LIGHT, Target_Self, 20, 0, 20, 0, 50));
		mDisappear = AddSpellFunc(&SpellFunc_Solarian_Disappear, Target_Self, 0, 22, 0);
		mDisappear->AddEmote("You are hopelessly outmatched!", Text_Yell, 11139);
		mDisappear->AddEmote("I will crush your delusions of grandeur!", Text_Yell, 11140);

		//Phase 2 spells
		mReappear = AddSpellFunc(&SpellFunc_Solarian_Reappear, Target_Self, 0, 0, 0);

		//Phase 3 spells
		AddPhaseSpell(3, AddSpell(SOLARIAN_VOID_BOLT, Target_Current, 100, 3, 10, 0, 100));
		AddPhaseSpell(3, AddSpell(SOLARIAN_PSYCHIC_SCREAM, Target_Self, 10, 0, 0));
		mVoidForm = AddSpell(SOLARIAN_SOLARIANS_TRANSFORM, Target_Self, 0, 0, 0);
		mVoidForm->AddEmote("Enough of this! Now I call upon the fury of the cosmos itself.");
		mVoidForm->AddEmote("I become ONE... with the VOID!");

		//Emotes
		AddEmote(Event_OnCombatStart, "Tal anu'men no sin'dorei!", Text_Yell, 11134);
		AddEmote(Event_OnDied, "The warmth of the sun... awaits.", Text_Yell, 11135);
		AddEmote(Event_OnTargetDied, "Your soul belongs to the Abyss!", Text_Yell, 11136);
		AddEmote(Event_OnTargetDied, "By the blood of the Highborne!", Text_Yell, 11137);
		AddEmote(Event_OnTargetDied, "For the Sunwell!", Text_Yell, 11138);
	}

	void OnCombatStart(Unit* pTarget)
	{
		mSplitTimer = AddTimer(50000);	//First split after 50sec
		ParentClass::OnCombatStart(pTarget);
	}
   
	void AIUpdate()
	{
		if( GetPhase() == 1 )
		{
			if( GetHealthPercent() <= 20 && !IsCasting() )
			{
				SetPhase(3, mVoidForm);
				CancelAllTimers();
			}
			else if( IsTimerFinished(mSplitTimer) && !IsCasting() )
			{
				SetPhase(2, mDisappear);
				ResetTimer(mSplitTimer, 90000);		//Next split in 90sec
				mAgentsTimer = AddTimer(6000);		//Agents spawns 6sec after the split
				mSolarianTimer = AddTimer(22000);	//Solarian with 2 priests spawns 22sec after split
			}
		}
		else if( GetPhase() == 2 )
		{
			if( IsTimerFinished(mSolarianTimer) && !IsCasting() )
			{
				SetPhase(1, mReappear);
				RemoveTimer(mSolarianTimer);
			}
			else if( IsTimerFinished(mAgentsTimer) && !IsCasting() )
			{
				for( int SpawnIter = 0; SpawnIter < 4; SpawnIter++ )
				{
					SpawnCreature(CN_SOLARIUMAGENT, mSpawnPositions[0][0], mSpawnPositions[0][1], 17, 0, true);
					SpawnCreature(CN_SOLARIUMAGENT, mSpawnPositions[1][0], mSpawnPositions[1][1], 17, 0, true);
					SpawnCreature(CN_SOLARIUMAGENT, mSpawnPositions[2][0], mSpawnPositions[2][1], 17, 0, true);
				}
				RemoveTimer(mAgentsTimer);
			}
		}
		ParentClass::AIUpdate();
	}

	SpellDesc*	mVoidForm;
	SpellDesc*	mDisappear;
	SpellDesc*	mReappear;
	int32		mSplitTimer, mAgentsTimer, mSolarianTimer;
	float		mSpawnPositions[3][2];
};

bool Dummy_Solarian_WrathOfTheAstromancer(uint32 pEffectIndex, Spell* pSpell)
{
	Unit* Caster = pSpell->u_caster;
	if( !Caster ) return true;

	Unit* Target = Caster->GetAIInterface()->GetNextTarget();
	if( !Target ) return true;

	SpellEntry* SpellInfo = dbcSpell.LookupEntry(SOLARIAN_WRATH_OF_THE_ASTROMANCER_BOMB);
	if( !SpellInfo ) return true;

	//Explode bomb after 6sec
	sEventMgr.AddEvent(Target, &Unit::EventCastSpell, Target, SpellInfo, EVENT_UNK, 6000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	return true;
}

void SpellFunc_Solarian_Disappear(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType)
{
	HighAstromancerSolarianAI* Solarian = ( pCreatureAI ) ? (HighAstromancerSolarianAI*)pCreatureAI : NULL;
	if( Solarian )
	{
		SpellFunc_Disappear(pThis, pCreatureAI, pTarget, pType);

		//Spawn spot lights, and despawn them after 26sec X(400,460) Y(-340,-400)
		Solarian->mSpawnPositions[0][0] = 400 + RandomFloat(60); Solarian->mSpawnPositions[0][1] = -400 + RandomFloat(60);
		Solarian->SpawnCreature(CN_SOLARIUM_SPOT_LIGHT, Solarian->mSpawnPositions[0][0], Solarian->mSpawnPositions[0][1], 17)->Despawn(26000);
		Solarian->mSpawnPositions[1][0] = 400 + RandomFloat(60); Solarian->mSpawnPositions[1][1] = -400 + RandomFloat(60);
		Solarian->SpawnCreature(CN_SOLARIUM_SPOT_LIGHT, Solarian->mSpawnPositions[1][0], Solarian->mSpawnPositions[1][1], 17)->Despawn(26000);
		Solarian->mSpawnPositions[2][0] = 400 + RandomFloat(60); Solarian->mSpawnPositions[2][1] = -400 + RandomFloat(60);
		Solarian->SpawnCreature(CN_SOLARIUM_SPOT_LIGHT, Solarian->mSpawnPositions[2][0], Solarian->mSpawnPositions[2][1], 17)->Despawn(26000);
	}
}

void SpellFunc_Solarian_Reappear(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType)
{
	HighAstromancerSolarianAI* Solarian = ( pCreatureAI ) ? (HighAstromancerSolarianAI*)pCreatureAI : NULL;
	if( Solarian )
	{
		//Spawn two priest friend to help Solarian
		Solarian->SpawnCreature(CN_SOLARIUMPRIEST, Solarian->mSpawnPositions[0][0], Solarian->mSpawnPositions[0][1], 17);
		Solarian->SpawnCreature(CN_SOLARIUMPRIEST, Solarian->mSpawnPositions[1][0], Solarian->mSpawnPositions[1][1], 17);
		//Solarian->MoveTo(Solarian->mSpawnPositions[2][0], Solarian->mSpawnPositions[2][1], 17);	//Doesn't work quite right yet

		SpellFunc_Reappear(pThis, pCreatureAI, pTarget, pType);
	}
}

class SolariumAgentAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(SolariumAgentAI, MoonScriptCreatureAI);
    SolariumAgentAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		SetDespawnWhenInactive(true);	//despawn creature if it gets out of combat or dead
		AggroNearestUnit();				//Aggro on spawn
	}
};

class SolariumPriestAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SolariumPriestAI, MoonScriptCreatureAI);
	SolariumPriestAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SOLARIUMPRIEST_GREATER_HEAL, Target_WoundedFriendly, 20, 2, 0, 0, 40);
		AddSpell(SOLARIUMPRIEST_HOLY_SMITE, Target_Current, 80, 2.5f, 0, 0, 40);
		SetDespawnWhenInactive(true);	//despawn creature if it gets out of combat or dead
		AggroNearestUnit();				//Aggro on spawn
	}
};

class SolariumSpotLight : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SolariumSpotLight, MoonScriptCreatureAI);
	SolariumSpotLight(Creature* pCreature) : MoonScriptCreatureAI(pCreature){ SetCanEnterCombat(false); }
};

// Al'ar AI
class AlarAuxClass: public Object
{
public:
AlarAuxClass(CreatureAIScript *);
~AlarAuxClass();
void Rebirth();

protected:
	CreatureAIScript *alar;
};

#define WALK 0
#define RUN 256
#define FLY 768

#define CN_ALAR 19514

// Phase1 spells
#define FLAME_BUFFET 34121	// casted only when there is no player in melee range!
#define FLAME_QUILLS 34316	// it flies to a high point in a center of room and use this on players located on platform	// also can be many others (Idk even if this spell works =S)
#define SUMMON_PHOENIX_ADDS 18814	// Summons 3 phoenixs

// Phase2 spells
#define FLAME_PATCH 35383	// 35383, 35380;
#define METEOR 35181		// but shouldn't be instant imho --Dive Bomb 35367
//#define EMBER_BLAST 34133	// used when one of adds has low health
#define MELT_ARMOR 35410 // maybe they are used, but not sure
//#define RANDOM_CHARGE

// Other spells
#define REBIRTH 34342

struct Coords
{
    float x;
    float y;
    float z;
    float o;
};

static Coords fly[] = 
{
	{  },
	{ 337.864868f,  65.702301f, 33.171944f, 1.329919f },	// fly 1 to ...
	{ 391.245148f,  35.472462f, 36.886353f, 0.822089f },
	{ 392.959869f, -34.031734f, 33.630096f, 3.342431f },
	{ 321.277008f, -70.101540f, 43.650482f, 3.844443f },
    { 256.202911f,  -1.549352f, 44.718426f, 0.069512f },	// ... fly 5
	{ 342.090088f,  55.661064f, 27.638260f, 0.739368f },	// attack/tank positions from phase 1 from here to ...
	{ 392.815369f,  31.636963f, 25.414761f, 0.551340f },
	{ 388.397308f, -38.834595f, 22.335297f, 5.702067f },
	{ 333.922229f, -60.645069f, 24.484278f, 1.454599f },	// ... to here
	{ 328.103455f,  -0.192393f, 50.216309f, 4.188326f },	// fire quills cast position	// EMOTE_STATE_WHIRLWIND = 382,
	{ 326.225647f,   2.381837f, -2.389485f, 4.877070f }	// center of the room which is used in phase 2
};

class AlarAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(AlarAI);
	SP_AI_Spell spells[7];
	bool m_spellcheck[7];

    AlarAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		/************************ Waypoint Place ************************/

		for (int i = 1; i < 12; i++)
		{
			_unit->GetAIInterface()->addWayPoint(CreateWaypoint(i, 0, RUN));	// FLY stucks my client
		}

		/************************** Spells ******************************/

		nrspells = 7;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

        spells[0].info = dbcSpell.LookupEntry(FLAME_BUFFET);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].cooldown = 4;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

        spells[1].info = dbcSpell.LookupEntry(FLAME_QUILLS);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].cooldown = 3;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(SUMMON_PHOENIX_ADDS);
		spells[2].targettype = TARGET_VARIOUS;
		spells[2].instant = true;
		spells[2].cooldown = 5;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;

		spells[3].info = dbcSpell.LookupEntry(FLAME_PATCH);
		spells[3].targettype = TARGET_VARIOUS;
		spells[3].instant = true;
		spells[3].cooldown = 10;
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 1000;

		spells[4].info = dbcSpell.LookupEntry(METEOR);
		spells[4].targettype = TARGET_ATTACKING;
		spells[4].instant = true;	// =(
		spells[4].cooldown = 30;
		spells[4].perctrigger = 0.0f;
		spells[4].attackstoptimer = 1000;

		spells[5].info = dbcSpell.LookupEntry(MELT_ARMOR);
		spells[5].targettype = TARGET_ATTACKING;
		spells[5].instant = true;
		spells[5].cooldown = 60;
		spells[5].perctrigger = 0.0f;
		spells[5].attackstoptimer = 1000;

		spells[6].info = dbcSpell.LookupEntry(REBIRTH);
		spells[6].targettype = TARGET_SELF;
		spells[6].instant = false;
		spells[6].cooldown = -1;
		spells[6].perctrigger = 0.0f;
		spells[6].attackstoptimer = 1000;

		/******************* Additional Settings *******************/

		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_FORWARDTHANSTOP);
		_unit->GetAIInterface()->m_moveFly = true;

		FlameQuills = false;
		Meteor = false;
		SetPhase(0);
		nDeath=0;
		timer=lasttime=0;
		_unit->GetAIInterface()->setOutOfCombatRange(200000);
        //RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		_unit->GetAIInterface()->m_moveFly = true;
		_unit->GetAIInterface()->StopMovement(0);
		_unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
		_unit->GetAIInterface()->setWaypointToMove(1);
		Flying=true;
		CastTime();
    }

	void OnDamageTaken(Unit* mAttacker, float fAmount) 
	{
		if (!_unit->event_HasEvents()) {
			RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
			ModifyAIUpdateEvent(1000);
			NextWP=6;
		}
	}

    void OnCombatStart(Unit* mTarget)
    {
		FlameQuills = false;
		Meteor = false;
		NextWP=6;

		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		ModifyAIUpdateEvent(1000);
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void SetPhase(uint32 ph)
	{
		Phase=ph;
		_unit->SetUInt32Value(UNIT_FIELD_BASE_MANA,Phase);
	}

    void OnCombatStop(Unit *mTarget)
    {
		CastTime();

		FlameQuills = false;
		Meteor = false;
		SetPhase(0);

		if (_unit->isAlive()) {
			if (_unit != NULL) _unit->SafeDelete();
			_unit->GetMapMgr()->GetInterface()->SpawnCreature(19514, fly[11].x, fly[11].y, fly[11].z, 0, false, false, 0, 0);
		}

        RemoveAIUpdateEvent();
    }

	void Rebirth()
	{
		//_unit->SetPosition( fly[11].x, fly[11].y, fly[11].z, fly[11].o);
		SetPhase(2);
		_unit->CastSpell(_unit, spells[6].info, spells[6].instant);
		_unit->SetUInt64Value(UNIT_FIELD_HEALTH,_unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH));
		_unit->setDeathState(ALIVE);
		_unit->RemoveFlag(UNIT_DYNAMIC_FLAGS,U_DYN_FLAG_TAGGED_BY_OTHER);
		_unit->SetFlag(UNIT_DYNAMIC_FLAGS, U_DYN_FLAG_LOOTABLE);
		_unit->Tagged = false;
		_unit->TaggerGuid = 0;
		_unit->GetAIInterface()->StopMovement(0); // after respawn monster can move

		_unit->WipeTargetList();
		_unit->WipeHateList();
		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
		_unit->GetAIInterface()->m_canMove = true;
		Flying=false;lasttime=timer;
	}

    void OnDied(Unit * mKiller)
    {
		nDeath++;
		if (nDeath==1) {
			_unit->SetFlag(UNIT_DYNAMIC_FLAGS,U_DYN_FLAG_TAGGED_BY_OTHER);
			_unit->RemoveFlag(UNIT_DYNAMIC_FLAGS, U_DYN_FLAG_LOOTABLE);
			_unit->Tagged = true;
			_unit->TaggerGuid = 0;
			AlarAuxClass *cAux=new AlarAuxClass(this);
			return;
		}
		SetPhase(0);nDeath=0;
		FlameQuills = false;
		Meteor = false;
		CastTime();
		RemoveAIUpdateEvent();
    }

	bool HostileInMeleeRange()
	{
		for(Object::InRangeSet::iterator i = _unit->GetInRangeSetBegin(); i != _unit->GetInRangeSetEnd(); ++i)
		{
			if(isHostile(_unit, (*i)) && _unit->GetDistance2dSq((*i)) < 225)
			{
				return true;
			}
		}
		return false;
	}

	void UPCastTime()
	{
		for(int i=0;i<nrspells;i++)
			if (spells[i].casttime>0) spells[i].casttime--;
	}

    void AIUpdate()
    {
		UPCastTime();
		if (!HostileInMeleeRange() && spells[0].casttime==0 && !FlameQuills && !Flying) {
			_unit->CastSpell(_unit, spells[0].info, spells[0].instant); 
			spells[0].casttime=spells[0].cooldown;
		}
		if (!Flying) timer++;

		switch (Phase)
		{
			case 0: return;
			case 1: 
				{
					PhaseOne();
				}break;
			case 2:
				{
					PhaseTwo();
				}break;
			default:
				{
					SetPhase(0);
				};
		}
    }

    void PhaseOne()
    {
		if (FlameQuills == true)
		{
			
			//_unit->CastSpell(_unit, dbcSpell.LookupEntry(34229), true);
			if (lasttime+11==timer)
			{
				_unit->CastSpellAoF(fly[1].x,fly[1].y,fly[1].z, spells[1].info, spells[1].instant); 
				_unit->CastSpellAoF(fly[2].x,fly[2].y,fly[2].z, spells[1].info, spells[1].instant); 
				_unit->CastSpellAoF(fly[3].x,fly[3].y,fly[3].z, spells[1].info, spells[1].instant); 
				_unit->CastSpellAoF(fly[4].x,fly[4].y,fly[4].z, spells[1].info, spells[1].instant); 
				_unit->GetAIInterface()->m_canMove = true;
				_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
				_unit->GetAIInterface()->setWaypointToMove(NextWP);
				FlameQuills=false;
				Flying=true;
				lasttime=timer;
			}
		}
		else
		if (lasttime+35==timer)
		{
			_unit->GetMapMgr()->GetInterface()->SpawnCreature(19551, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 0, false, false, 0, 0);
			//_unit->CastSpell(_unit, spells[2].info, spells[2].instant);
			lasttime=timer+rand()%10;
			_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->StopMovement(0);
			_unit->GetAIInterface()->m_canMove = true;

			_unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
			_unit->GetAIInterface()->setWaypointToMove(NextWP);
			_unit->GetAIInterface()->SetNextTarget((Unit*)NULL);
			Flying=true;
		}
    }

	void PhaseTwo()
	{
		CastSpell(5);
		CastSpell(4);
		if ((rand()%100)<2) {
			Unit *target=GetRandomTarget();
			Creature* patch;
			if (target!=NULL) {
				patch =_unit->GetMapMgr()->GetInterface()->SpawnCreature(20602, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 0, false, false, _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), 0);
				if (patch!=NULL) {
					patch->SetUInt32Value(UNIT_FIELD_DISPLAYID, 16946 );
					patch->SetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID, 16946 );
				}
			}
		}
		if (Meteor == true)
		{
		}
		if (lasttime+35==timer)
		{
			_unit->GetMapMgr()->GetInterface()->SpawnCreature(19551, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 0, false, false, 0, 0);
			_unit->GetMapMgr()->GetInterface()->SpawnCreature(19551, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 0, false, false, 0, 0);
			lasttime=timer;
		}
	}

	Unit *GetRandomTarget()
	{
		for(Object::InRangeSet::iterator i = _unit->GetInRangeSetBegin(); i != _unit->GetInRangeSetEnd(); ++i)
		{
			if(isHostile(_unit, (*i)) && (*i)->GetInstanceID() == _unit->GetInstanceID())
			{
				Unit *RandomTarget = static_cast< Unit* >(*i);

				if(RandomTarget->isAlive())
					return RandomTarget;
			}
		}
		return NULL;
	}

	void CastSpell(int id)
	{
		if (spells[id].casttime>0) return;
		if (_unit->GetCurrentSpell() != NULL) return;
		
		Unit *target = NULL;
		target = _unit->GetAIInterface()->GetNextTarget();
		switch(spells[id].targettype)
		{
			case TARGET_SELF:
			case TARGET_VARIOUS:
				_unit->CastSpell(_unit, spells[id].info, spells[id].instant); break;
			case TARGET_ATTACKING:
				_unit->CastSpell(target, spells[id].info, spells[id].instant); break;
			case TARGET_DESTINATION:
				_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[id].info, spells[id].instant); break;
			case TARGET_RANDOM_SINGLE: {
				target=GetRandomTarget();
				if (target!=NULL)
				_unit->CastSpell(target, spells[id].info, spells[id].instant);
			} break;
		}	
		spells[id].casttime=spells[id].cooldown;
	}

	void SetNextWP(uint32 wp)
	{
		_unit->GetAIInterface()->SetAIState(STATE_ATTACKING);//STATE_IDLE
		NextWP=wp;
		Flying=false;
		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
		_unit->GetAIInterface()->m_canMove = false;
		lasttime=timer;
	}

    void OnReachWP(uint32 iWaypointId, bool bForwards)
    {

		if (Phase == 0)
		{
			if (NextWP==6) Phase=1;
			else NextWP=iWaypointId%5+1;
			_unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
			_unit->GetAIInterface()->setWaypointToMove(NextWP);
		}

		switch(iWaypointId)
		{
			case 6:
			{
				SetNextWP(7);
			} break;

			case 7:
			{
				SetNextWP(8);
			} break;
			case 8:
			{
				SetNextWP(9);
			} break;
			case 9:
			{
				SetNextWP(10);
			} break;

			case 10:
		    {
				if (Phase == 1)
				{
					FlameQuills = true;
					QuillsCount = 0;
					Flying=false;
					_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
					_unit->GetAIInterface()->m_canMove = false;
					NextWP=6;
					lasttime=timer;
					//_unit->Emote(EMOTE_STATE_WHIRLWIND);
					_unit->CastSpell(_unit, dbcSpell.LookupEntry(34229), true);
				}

				if (Phase == 2)
				{
					Meteor = true;
				}

			}break;

			default:
				{
				}
		}
    }

    inline WayPoint* CreateWaypoint(int id, uint32 waittime, uint32 flags)
    {
        WayPoint * wp = _unit->CreateWaypointStruct();
        wp->id = id;
        wp->x = fly[id].x;
        wp->y = fly[id].y;
        wp->z = fly[id].z;
        wp->o = fly[id].o;
        wp->waittime = waittime;
        wp->flags = flags;
        wp->forwardemoteoneshot = 0;
        wp->forwardemoteid = 0;
        wp->backwardemoteoneshot = 0;
        wp->backwardemoteid = 0;
        wp->forwardskinid = 0;
        wp->backwardskinid = 0;
        return wp;
    }

protected:

	bool FlameQuills;
	uint32 QuillsCount;
	bool Meteor;
	uint32 NextWP;
    uint32 m_entry;
    uint32 FlyWay;
	uint32 Phase;
	uint32 nDeath;
	int nrspells;
	uint32 timer,lasttime;
	bool Flying;
};

#define CN_EMBEROFALAR	19551

class EmberAlarAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(EmberAlarAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    EmberAlarAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(34341);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].cooldown = 15;
		spells[0].perctrigger = 50.0f;
		spells[0].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void OnTargetDied(Unit* mTarget)
    {
    }

    void OnCombatStop(Unit *mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

	Unit *GetAlar()
	{
		for(Object::InRangeSet::iterator i = _unit->GetInRangeSetBegin(); i != _unit->GetInRangeSetEnd(); ++i)
			{
				if((*i)->GetTypeId() == TYPEID_UNIT && _unit->GetDistance2dSq((*i)) < 400000)
				{
					Creature *creature=static_cast<Creature *>((*i));
					if(creature->GetEntry() == 19514 && (*i)->GetInstanceID() == _unit->GetInstanceID())
					{
						return creature;
					}
				}
			}
		return NULL;
	}

    void OnDied(Unit * mKiller)
    {

		_unit->CastSpell(_unit, spells[0].info, spells[0].instant);
		CastTime();

		Unit* Alar = NULL;
		//Alar=_unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 19514);

		Alar=GetAlar();
		if (Alar==NULL) {
			return;
		}

		uint32 Phase=Alar->GetUInt32Value(UNIT_FIELD_BASE_MANA);
		if (Phase==2/*&& Alar->isAlive()*/) {
			uint32 maxhp=Alar->GetUInt32Value(UNIT_FIELD_MAXHEALTH);
			Alar->DealDamage(Alar,(maxhp*3)/100,0,0,0);
		}
    }

    void AIUpdate()
	{
    }
	
protected:
	int nrspells;
};


#define CN_PATCHALAR	20602

class PatchAlarAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(PatchAlarAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    PatchAlarAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(35380);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].cooldown = 15;
		spells[0].perctrigger = 50.0f;
		spells[0].attackstoptimer = 1000;

		//_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		//_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		//_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
		_unit->GetAIInterface()->m_canMove = false;
		_unit->Despawn(120000,0);
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		_unit->CastSpell(_unit, spells[0].info, true);
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void OnTargetDied(Unit* mTarget)
    {
    }

    void OnCombatStop(Unit *mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

	
    void OnDied(Unit * mKiller)
    {
		CastTime();
    }

    void AIUpdate()
	{
    }
	
protected:
	int nrspells;
};

	AlarAuxClass::AlarAuxClass (CreatureAIScript *al)
	{
		alar=al;
		sEventMgr.AddEvent(this, &AlarAuxClass::Rebirth, EVENT_CORPSE_DESPAWN, 3000, 1,0);
	}

	AlarAuxClass::~AlarAuxClass ()
	{
		sEventMgr.RemoveEvents(this);
	}

	void AlarAuxClass::Rebirth()
	{
		((AlarAI *)alar)->Rebirth();
		delete this;
	}


//-----------------------------------------------------------//
//---------------Kael'thas Encounter Script------------------//
//--------------Rewritten/optimized by M4ksiu----------------//
//-------------------Scripted by Plexor----------------------//
//-----------------------------------------------------------//

// Thaladred the Darkener AI(1st advisor)
#define CN_DARKENER 20064

#define PSYCHIC_BLOW 36966
#define SILENCE 29943

class DarkenerAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(DarkenerAI);
	SP_AI_Spell spells[2]; 
	bool m_spellcheck[2]; 
	
	DarkenerAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 2; 
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		
		spells[0].info = dbcSpell.LookupEntry(PSYCHIC_BLOW);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = 20;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000;
		//spells[0].speech = "Physic blow";
		
		spells[1].info = dbcSpell.LookupEntry(SILENCE); 
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].cooldown = 15;
		spells[1].perctrigger = 10.0f;
		spells[1].attackstoptimer = 1000;

		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);

		CurrentTarget = NULL;
	}

    void OnCombatStart(Unit* mTarget)
    {
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(true);

		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Prepare yourselves!");
		_unit->PlaySoundToSet(11203);

		CurrentTarget = mTarget;
		if (CurrentTarget)
		{
			_unit->GetAIInterface()->modThreatByPtr(CurrentTarget, 1000000);
			if (CurrentTarget->IsPlayer())
			{
				Player *pPlayer = (Player*)CurrentTarget;
				char msg[256];
				snprintf((char*)msg, 256, " sets his gaze on %s", pPlayer->GetName());
				_unit->SendChatMessageAlternateEntry(CN_DARKENER, CHAT_MSG_MONSTER_EMOTE, LANG_UNIVERSAL, msg);
			}
		}

		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = spells[i].cooldown;
    }

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);

		RemoveAIUpdateEvent();

		CurrentTarget = NULL;

		if (_unit->GetHealthPct() > 0)
		{
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
			_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		}
	}
	
	void OnDied(Unit * mKiller)
	{
		RemoveAIUpdateEvent();

		CurrentTarget = NULL;
		//_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "");
		_unit->PlaySoundToSet(11204);
	}

	void OnTargetDied(Unit *mTarget)
	{
		CurrentTarget = _unit->GetAIInterface()->GetMostHated();
		if (CurrentTarget)
		{
			_unit->GetAIInterface()->modThreatByPtr(CurrentTarget, 1000000);
			if (CurrentTarget->IsPlayer())
			{
				Player *pPlayer = (Player*)CurrentTarget;
				char msg[256];
				snprintf((char*)msg, 256, " sets his gaze on %s", pPlayer->GetName());
				_unit->SendChatMessageAlternateEntry(CN_DARKENER, CHAT_MSG_MONSTER_EMOTE, LANG_UNIVERSAL, msg);
			}
		}
	}
	
	void AIUpdate()
	{
		float val = RandomFloat(100.0f);
		SpellCast(val);
		
		float rand = RandomFloat(100.0f);
		if (rand < 20)
		{
			ResetTarget();
		}
	}
	Unit* FindTargetForSpell()
	{
		Unit* target = NULL;
		float distance = 1500.0f;

		Unit *pUnit;
		float dist;

		for (std::set<Object*>::iterator itr = _unit->GetInRangeOppFactsSetBegin(); itr != _unit->GetInRangeOppFactsSetEnd(); itr++)
		{
			if((*itr)->GetTypeId() != TYPEID_UNIT && (*itr)->GetTypeId() != TYPEID_PLAYER)
				continue;

			pUnit = static_cast<Unit*>((*itr));

			if(pUnit->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_FEIGN_DEATH))
				continue;

			if(pUnit->m_invisible)
				continue;
			
			if(!pUnit->isAlive() || _unit == pUnit)
				continue;

			dist = _unit->GetDistance2dSq(pUnit);

			if(dist > distance*distance)
				continue;

			target = pUnit;
			break;
		}

		return target;
	}
	void ResetTarget()
	{
		Unit* target = FindTargetForSpell();
		if (target)
		{
			CurrentTarget = NULL;
			if (target->IsPlayer())
			{
				_unit->GetAIInterface()->modThreatByPtr(target, 1000000);
				Player *pPlayer = (Player*)target;
				char msg[256];
				snprintf((char*)msg, 256, " sets his gaze on %s", pPlayer->GetName());
				_unit->SendChatMessageAlternateEntry(CN_DARKENER, CHAT_MSG_MONSTER_EMOTE, LANG_UNIVERSAL, msg);
				CurrentTarget = target;
			}
	}
	}
	
	
	void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				if (_unit->GetDistance2dSq(_unit->GetAIInterface()->GetNextTarget()) > 30.0f && i == 0)
					continue;

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

protected:	

	Unit *CurrentTarget;
	int nrspells;
};

// Lord Sanguinar AI (2nd advisor)
#define CN_SANGUINAR 20060

#define BELLOWING 36922

class SanguinarAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(SanguinarAI);
	SP_AI_Spell spells[1]; 
	
	SanguinarAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		spells[0].info = dbcSpell.LookupEntry(BELLOWING);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].cooldown = 30;

		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
	}

    void OnCombatStart(Unit* mTarget)
    {
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Blood for blood!");
		_unit->PlaySoundToSet(11152);

		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));

		spells[0].casttime = (uint32)time(NULL) + spells[0].cooldown;
    }

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);

		RemoveAIUpdateEvent();

		if (_unit->GetHealthPct() > 0)
		{
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
			_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		}
	}
	
	void OnDied(Unit * mKiller)
	{
		RemoveAIUpdateEvent();
		//_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "");
		_unit->PlaySoundToSet(11153);
	}
	
	void AIUpdate()
	{
		uint32 t = (uint32)time(NULL);
		if (t > spells[0].casttime)
		{
			_unit->CastSpell(_unit, spells[0].info, spells[0].instant);

			spells[0].casttime = t + spells[0].cooldown;
		}
	}

protected:	

	int nrspells;
};

// Grand Astromancer Capernian AI (3rd advisor)
#define CN_CAPERNIAN 20062

#define FIREBALL 36971
#define CONFLAGRATION 37018
#define ARCANE_BURST 36970

class CapernianAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(CapernianAI);
	SP_AI_Spell spells[3]; 
	bool m_spellcheck[3]; 
	
	CapernianAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 2; 
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(CONFLAGRATION);
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = true;
		spells[0].cooldown = 10;
		spells[0].perctrigger = 7.0f;
		spells[0].mindist2cast = 0.0f;
		spells[0].maxdist2cast = 30.0f;

		spells[1].info = dbcSpell.LookupEntry(ARCANE_BURST);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = false;
		spells[1].perctrigger = 10.0f;
		spells[1].cooldown = 15;

		spells[2].info = dbcSpell.LookupEntry(FIREBALL);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = false;
		spells[2].cooldown = 0;
		spells[2].perctrigger = 8.0f;
		spells[2].attackstoptimer = 2000;

		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
	}

    void OnCombatStart(Unit* mTarget)
    {
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(true);

		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The sin'dorei reign supreme!");
		_unit->PlaySoundToSet(11117);

		if (_unit->GetDistance2dSq(mTarget) <= 1225.0f)
		{
			_unit->GetAIInterface()->setCurrentAgent(AGENT_SPELL);
		}

		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));

		for (int i = 0; i < 3; i++)
			spells[i].casttime = 0;
    }

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);

		RemoveAIUpdateEvent();

		if (_unit->GetHealthPct() > 0)
		{
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
			_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		}
	}
	
	void OnDied(Unit * mKiller)
	{
		RemoveAIUpdateEvent();
	}
	
	void AIUpdate()
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		if (_unit->GetAIInterface()->GetNextTarget() && _unit->GetDistance2dSq(_unit->GetAIInterface()->GetNextTarget()) <= 1225.0f)
		{
			_unit->GetAIInterface()->setCurrentAgent(AGENT_SPELL);
			if (_unit->GetCurrentSpell() == NULL && RandomUInt(10) > 2)
			{
				_unit->setAttackTimer(spells[2].attackstoptimer, false);

				Unit *target = NULL;
				target = _unit->GetAIInterface()->GetNextTarget();

				_unit->CastSpell(target, spells[2].info, spells[2].instant);
				return;
			}
		}

		float val = RandomFloat(100.0f);
		SpellCast(val);
	}
	
	void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					if (!spells[i].instant)
						_unit->GetAIInterface()->StopMovement(1);

					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
						case TARGET_RANDOM_FRIEND:
						case TARGET_RANDOM_SINGLE:
						case TARGET_RANDOM_DESTINATION:
							CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<Unit*> TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					Unit* RandomTarget = NULL;
					RandomTarget = (Unit*)(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			Unit * RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}

protected:	

	int nrspells;
};

// Master Engineer Telonicus AI (4th advisor)
#define CN_TELONICUS 20063

#define BOMB 37036
#define REMOTE_TOY 37027	// doesn't seems to work like it should

class TelonicusAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(TelonicusAI);
	SP_AI_Spell spells[2]; 
	bool m_spellcheck[2]; 
	
	TelonicusAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 2; 
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		
		spells[0].info = dbcSpell.LookupEntry(BOMB);
		spells[0].targettype = TARGET_RANDOM_DESTINATION;
		spells[0].instant = false;
		spells[0].cooldown = 15;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 2000;
		spells[0].mindist2cast = 0.0f;
		spells[0].maxdist2cast = 30.0f;

		spells[1].info = dbcSpell.LookupEntry(REMOTE_TOY);
		spells[1].targettype = TARGET_RANDOM_SINGLE;
		spells[1].instant = true;
		spells[1].cooldown = 15;
		spells[1].perctrigger = 10.0f;
		spells[1].attackstoptimer = 1000;
		spells[1].mindist2cast = 0.0f;
		spells[1].maxdist2cast = 30.0f;

		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
	}

    void OnCombatStart(Unit* mTarget)
    {
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Anar'alah belore!");
		_unit->PlaySoundToSet(11157);

		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));

		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;
    }

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);

		RemoveAIUpdateEvent();

		if (_unit->GetHealthPct() > 0)
		{
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
			_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		}
	}
	
	void OnDied(Unit * mKiller)
	{
		RemoveAIUpdateEvent();
		//_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "");
		_unit->PlaySoundToSet(11158);
	}
	
	void AIUpdate()
	{
		float val = RandomFloat(100.0f);
		SpellCast(val);
	}
	
	void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
						case TARGET_RANDOM_FRIEND:
						case TARGET_RANDOM_SINGLE:
						case TARGET_RANDOM_DESTINATION:
							CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<Unit*> TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					Unit* RandomTarget = NULL;
					RandomTarget = (Unit*)(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			Unit * RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}

protected:	

	int nrspells;
};

//Phoenix Egg AI
#define CN_FLAMESTRIKE 21369

#define FLAMESTRIKE_TRIGGER 36730

class FlameStrikeAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(FlameStrikeAI);
	FlameStrikeAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		RegisterAIUpdateEvent(1000);

		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->m_canMove = false;

		RegisterAIUpdateEvent(1000);

		DespawnTimer = 15;
	}

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
	}
	
	void OnDied(Unit * mKiller)
	{
		RemoveAIUpdateEvent();

		_unit->Despawn(0,0);
	}
	
	void AIUpdate()
	{
		DespawnTimer--;
		if (DespawnTimer == 13)
			_unit->CastSpell(_unit, dbcSpell.LookupEntry(FLAMESTRIKE_TRIGGER), false);

		Unit *PhoenixEgg = NULL;
		PhoenixEgg = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 21364);
		if (PhoenixEgg == NULL || !PhoenixEgg->isAlive())
		{
			_unit->RemoveAura(36730);
			_unit->Despawn(0,0);
		}
	}
	
protected:	

	int DespawnTimer;
};

//Phoenix AI
#define CN_PHOENIX 21362

#define BURN 36721
//#define REBIRTH 35369 //Already used in Al'Ar AI

class PhoenixAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(PhoenixAI);
	SP_AI_Spell spells[2]; 
	bool m_spellcheck[2]; 
	
	PhoenixAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		
		spells[0].info = dbcSpell.LookupEntry(BURN);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = true;
		spells[0].cooldown = -1;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(REBIRTH);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = false;
		spells[1].perctrigger = 0.0f;

		//_unit->CastSpell(_unit, dbcSpell.LookupEntry(REBIRTH), false);

		Timer = 0;
	}

	void OnSpawn(Unit* mTarget)
    {
		RegisterAIUpdateEvent(1000);

		Unit *target = NULL;
		target = _unit->GetAIInterface()->GetNextTarget();
		if (target)
		{
			_unit->GetAIInterface()->AttackReaction(target, UNIT_FIELD_MINDAMAGE, 0);
		}
    }

    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(1000);

		Timer = 0;
    }

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}
	
	void OnDied(Unit * mKiller)
	{
		_unit->GetMapMgr()->GetInterface()->SpawnCreature(21364, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), _unit->GetOrientation(), false, false, 0, 0);
		_unit->Despawn(0,0);

		RemoveAIUpdateEvent();
	}
	
	void AIUpdate()
	{
		Timer = Timer++;
		
		if (_unit->GetUInt32Value(UNIT_FIELD_HEALTH) >= 2500)
		{
			if (Timer%3 == 0)
			{
				_unit->SetUInt32Value(UNIT_FIELD_HEALTH, (_unit->GetUInt32Value(UNIT_FIELD_HEALTH))-2500);
				_unit->CastSpell(_unit, dbcSpell.LookupEntry(BURN), false);
			}
		}

		else
		{
			_unit->GetMapMgr()->GetInterface()->SpawnCreature(21364, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), _unit->GetOrientation(), false, false, 0, 0);
			_unit->Despawn(0,0);
			
			RemoveAIUpdateEvent();
		}

		if (Timer == 20)
		{
			Unit *target = NULL;
			target = _unit->GetAIInterface()->GetNextTarget();
			if (target)
				_unit->GetAIInterface()->AttackReaction(target, UNIT_FIELD_MINDAMAGE, 0);
		}
	
		else
		{
			float val = RandomFloat(100.0f);
			SpellCast(val);
		}
	}
	
	void SpellCast(float val)
	{
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}

					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}

					m_spellcheck[i] = false;
					return;
				}

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

protected:	

	int nrspells;
	int Timer;
};

//Phoenix Egg AI
#define CN_PHOENIX_EGG 21364

class PhoenixEggAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(PhoenixEggAI);
	PhoenixEggAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		_unit->GetMapMgr()->GetInterface()->SpawnCreature(21369, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), _unit->GetOrientation(), false, false, 0, 0);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->m_canMove = false;

		RegisterAIUpdateEvent(15000);
	}

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
	}
	
	void OnDied(Unit * mKiller)
	{
		RemoveAIUpdateEvent();
	}
	
	void AIUpdate()
	{
		_unit->GetMapMgr()->GetInterface()->SpawnCreature(21362, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), _unit->GetOrientation(), false, false, 0, 0);
		_unit->Despawn(0,0);
	}
};

//-------------------Kael'thas Weapons------------------//

#define NETHERSTRAND_LONGBOW	21268
#define DEVASTATION				21269
#define COSMIC_INFUSER			21270
#define INFINITY_BLADE			21271
#define WARP_SLICER				21272
#define PHASESHIFT_BULWARK		21273
#define STAFF_OF_DISINTEGRATION	21274

class WeaponsAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(WeaponsAI);
	
	WeaponsAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		_unit->m_noRespawn = true;
	}
	
	void OnSpawn(Unit* mTarget)
	{
		Unit *target = FindTargetForSpell();
		if (target)
		{
			_unit->GetAIInterface()->AttackReaction(target, 1, 0);
		}
	}
	
	Unit* FindTargetForSpell()
	{
		Unit* target = NULL;
		float distance = 150.0f;

		Unit *pUnit;
		float dist;

		for (std::set<Object*>::iterator itr = _unit->GetInRangeOppFactsSetBegin(); itr != _unit->GetInRangeOppFactsSetEnd(); itr++)
		{
			if((*itr)->GetTypeId() != TYPEID_UNIT && (*itr)->GetTypeId() != TYPEID_PLAYER)
				continue;

			pUnit = static_cast<Unit*>((*itr));

			if(pUnit->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_FEIGN_DEATH))
				continue;

			if(pUnit->m_invisible)
				continue;
			
			if(!pUnit->isAlive() || _unit == pUnit)
				continue;

			dist = _unit->GetDistance2dSq(pUnit);

			if(dist > distance*distance)
				continue;

			target = pUnit;
			break;
		}

		return target;
	}
};

//------------------------TO DO-------------------------//
/*
 *	1. Weapon phase - don't have creatures in DB (too lazy to find them atm).
 *	2. Gravity lapse.
 *	3. Gravity lapse phase spells.
 *	4. Finish Kael'thas scene between his 2 phases - I am lacking of infos about spell effects.
 */

//Prince Kael'Thas
#define CN_KAELTHAS 19622

// Common spells
#define K_FIREBALL				36805	// prolly wrong id
#define ARCANE_DISTURPTION		36834
#define SHOCK_BARRIER			36815	// timed

// Phase 4 spells
#define FLAME_STRIKE			36731	// timed
#define PYROBLAST				38535	// timed
#define MIND_CONTROL			36797	// timed

// Phase 5 spells
#define GRAVITY_LAPSE			35966	// timed
#define NETHER_VAPOR			35859
#define NETHER_BEAM				35873	// timed along with lapse

//#define GRAVITY1 34480 //knockback + aura
//#define GRAVITY2 35941 // explosion effect

#define SUMMON_WEAPONS 36976 //casting effect

#define REMOVE_INFUSER 39498
#define REMOVE_DEVASTATION 39499
#define REMOVE_INFINITY 39500
#define REMOVE_LONGBOW 39501
#define REMOVE_BULWARK 39502
#define REMOVE_STAFF 39503
#define REMOVE_SLICER 39504

struct Pos
{
	float x;
	float y;
	float z;
};

struct Pos Triggers[]=
{
	{ 789.719543f, 24.627499f, 52.728550f },
	{ 791.931152f, -24.925735f, 52.728550f },
	{},
	{},
	{},
	{}
};

struct Pos WP[]=
{
	{  },
	{ 794.072998f,  0.214634f, 48.728500f },
	{ 794.052998f,  0.214634f, 75.728500f },
	{ 794.032998f,  0.214634f, 48.728500f }
};

float KaelthasWeapons[7][5] =
{
    {21270, 794.38f, 15, 48.72f, 2.9f },      //[Cosmic Infuser]
    {21269, 785.47f, 12.12f, 48.72f, 3.14f }, //[Devastation]
    {21271, 781.25f, 4.39f, 48.72f, 3.14f },  //[Infinity Blade]
    {21273, 777.38f, -0.81f, 48.72f, 3.06f }, //[Phaseshift Bulwark]
    {21274, 781.48f, -6.08f, 48.72f, 3.9f },  //[Staff of Disintegration]
    {21272, 785.42f, -13.59f, 48.72f, 3.4f }, //[Warp Slicer]
    {21268, 793.06f, -16.61f, 48.72f, 3.10f } //[Netherstrand Longbow]
};

class KaelThasAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(KaelThasAI);
	SP_AI_Spell spells[9]; 
	bool m_spellcheck[9]; 
	
	KaelThasAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(1, 0, RUN));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(2, 0, FLY));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(3, 0, FLY));
		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);

		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		// Common spells
		spells[0].info = dbcSpell.LookupEntry(K_FIREBALL); 
		spells[0].targettype = TARGET_ATTACKING; 
		spells[0].instant = false; 
		spells[0].perctrigger = 15.0f; 
		spells[0].attackstoptimer = 3000; 
		spells[0].cooldown = 10;

		spells[1].info = dbcSpell.LookupEntry(ARCANE_DISTURPTION);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].perctrigger = 8.0f;
		spells[1].attackstoptimer = 2000;
		spells[1].cooldown = 10;

		spells[2].info = dbcSpell.LookupEntry(SHOCK_BARRIER);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = true;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 2000;
		spells[2].cooldown = 60;

		// used after shock barrier
		spells[3].info = dbcSpell.LookupEntry(PYROBLAST); 
		spells[3].targettype = TARGET_RANDOM_SINGLE;
		spells[3].instant = false;
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 5500;
		spells[3].cooldown = 60;
		spells[3].mindist2cast = 0.0f;
		spells[3].maxdist2cast = 60.0f;

		// after it phoenix should be spawned
		spells[4].info = dbcSpell.LookupEntry(FLAME_STRIKE); 
		spells[4].targettype = TARGET_RANDOM_DESTINATION; 
		spells[4].instant = true; 
		spells[4].perctrigger = 0.0f; 
		spells[4].attackstoptimer = 1500; 
		spells[4].cooldown = 55;
		spells[4].mindist2cast = 0.0f;
		spells[4].maxdist2cast = 60.0f;

		spells[5].info = dbcSpell.LookupEntry(MIND_CONTROL);
		spells[5].targettype = TARGET_RANDOM_SINGLE;
		spells[5].instant = true;
		spells[5].perctrigger = 0.0f;
		spells[5].attackstoptimer = 1000;
		spells[5].cooldown = 30;
		spells[5].mindist2cast = 10.0f;
		spells[5].maxdist2cast = 60.0f;

		// 2 phase spells
		spells[6].info = dbcSpell.LookupEntry(NETHER_VAPOR);
		spells[6].targettype = TARGET_RANDOM_SINGLE;
		spells[6].instant = true;
		spells[6].perctrigger = 8.0f;
		spells[6].attackstoptimer = 1500;
		spells[6].cooldown = 10;
		spells[6].mindist2cast = 0.0f;
		spells[6].maxdist2cast = 64.0f;

		spells[7].info = dbcSpell.LookupEntry(GRAVITY_LAPSE);
		spells[7].targettype = TARGET_VARIOUS;
		spells[7].instant = true;
		spells[7].perctrigger = 0.0f;
		spells[7].attackstoptimer = 1000;
		spells[7].cooldown = 90;							// not sure if it should not be 60

		// uses it when Graviti Lapse is enabled
		spells[8].info = dbcSpell.LookupEntry(NETHER_BEAM);
		spells[8].targettype = TARGET_RANDOM_SINGLE;
		spells[8].instant = true;
		spells[8].perctrigger = 0.0f;
		spells[8].attackstoptimer = 1500;
		spells[8].cooldown = 2;								// no idea
		spells[8].mindist2cast = 0.0f;
		spells[8].maxdist2cast = 60.0f;

		_unit->SetFloatValue(OBJECT_FIELD_SCALE_X, 1);
		X = Y = Z = 0.0f;
		PhoenixTimer = 0;
		Phoenix = false;
		WeaponPhase = 0;
		AddsTimer = 0;
		AddActive = 1;
		AddPhase = 1;
		Speech = 0;
		Scene = 0;
	}

    void OnCombatStart(Unit* mTarget)
    {
		sanityCheck();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));

		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
		_unit->GetAIInterface()->m_canMove = false;

		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Energy. Power. My people are addicted to it. Their dependence made manifest after the Sunwell was destroyed. Welcome to the future...a pity you're too late to stop it. No one can stop me now. Selama ashal'anore.");
		_unit->PlaySoundToSet(11256);

		X = Y = Z = 0.0f;
		PhoenixTimer = 0;
		Phoenix = false;
		WeaponPhase = 0;
		AddsTimer = 0;
		AddActive = 0;
		AddPhase = 1;
		Speech = 0;
		Scene = 0;

		Unit *Darkener = NULL;
		Unit *Sanguinar = NULL;
		Unit *Capernian = NULL;
		Unit *Telonicus = NULL;

		Darkener = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(786.28f, 20.2825f, 48.7285f, 20064);
		Sanguinar = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(785.825f, -22.1231f, 48.7285f, 20060);
		Capernian = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(791.128f, -12.6735f, 48.7285f, 20062);
		Telonicus = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(791.906f, 11.9183f, 48.7285f, 20063);

		// Very strange way to do that
		if (Darkener == NULL || !Darkener->isAlive())
		{
			AddActive++;
			AddPhase++;
			if (Sanguinar == NULL || !Sanguinar->isAlive())
			{
				AddActive++;
				AddPhase++;
				if (Capernian == NULL || !Capernian->isAlive())
				{
					AddActive++;
					AddPhase++;
					if (Telonicus == NULL || !Telonicus->isAlive())
					{
						AddActive++;
						AddPhase++;
					}
				}
			}
		}

		uint32 t = (uint32)time(NULL);
		for (int i = 0; i < 2; i++)
			spells[i].casttime = 0;
		for (int i = 2; i < 9; i++)
			spells[i].casttime = t + spells[i].cooldown;
   }

	void OnTargetDied(Unit* mTarget)
	{
		if (_unit->GetHealthPct() > 0)
		{
			switch (RandomUInt(3))
			{
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You gambled...and lost.");
				_unit->PlaySoundToSet(11271);
				break;
			case 2:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "This was child's play.");	// not sure
				_unit->PlaySoundToSet(11272);
				break;
			default:
				{
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You will not prevail.");
					_unit->PlaySoundToSet(11270);
				}
			}
		}
	}

	/*void OnCombatStop(Unit *mTarget)
	{
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
		_unit->GetAIInterface()->m_canMove = true;

		_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);

		_unit->SetFloatValue(OBJECT_FIELD_SCALE_X, 3);
		X = Y = Z = 0.0f;
		PhoenixTimer = 0;
		Phoenix = false;
		WeaponPhase = 0;
		AddsTimer = 0;
		AddActive = 0;
		AddPhase = 1;
		Speech = 0;

		GameObject *GObj = NULL;
		GObj = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(743.057f, 4.63443f, 137.796f, 184069);
		if(GObj && GObj->GetUInt32Value(GAMEOBJECT_STATE) == 0)
		{
			GObj->SetUInt32Value(GAMEOBJECT_STATE, 1);
		}

		RemoveAIUpdateEvent();

		std::vector<Creature*> DespawnTable;
												
		for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
		{ 
			if ((*itr)->GetTypeId()== TYPEID_UNIT && (*itr)->GetInstanceID() == _unit->GetInstanceID() && (*itr) != _unit)
			{
				Creature *CreatureToAdd = NULL;
				CreatureToAdd = (Creature*)(*itr);

				if (CreatureToAdd && CreatureToAdd->GetSQL_id() == 0 && (CreatureToAdd->GetEntry() == CN_FLAMESTRIKE || 
					CreatureToAdd->GetEntry() == CN_PHOENIX || CreatureToAdd->GetEntry() == CN_PHOENIX_EGG || CreatureToAdd->GetEntry() == 21268
					|| CreatureToAdd->GetEntry() == 21269 || CreatureToAdd->GetEntry() == 21270 || CreatureToAdd->GetEntry() == 21271
					|| CreatureToAdd->GetEntry() == 21272 || CreatureToAdd->GetEntry() == 21273 || CreatureToAdd->GetEntry() == 21274))
				{
					DespawnTable.push_back(CreatureToAdd);
				} 
			} 
		}

		for (uint32 i = 0; i < DespawnTable.size(); i++)
		{
			Creature *CreatureToDespawn = DespawnTable[i];

			if (CreatureToDespawn)
				CreatureToDespawn->Despawn(0,0);
		}

		DespawnTable.clear();
	}*/
	void OnCombatStop(Unit *mTarget)
	{
		reset();
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}
	
	void sanityCheck()
	{
		Creature* Darkener = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(786.28f, 20.2825f, 48.7285f, 20064);
		Creature* Sanguinar = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(785.825f, -22.1231f, 48.7285f, 20060);
		Creature* Capernian = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(791.128f, -12.6735f, 48.7285f, 20062);
		Creature* Telonicus = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(791.906f, 11.9183f, 48.7285f, 20063);

		if(Darkener == NULL)
			spawnCreature(CN_DARKENER);
		if(Sanguinar == NULL)
			spawnCreature(CN_SANGUINAR);
		if(Capernian == NULL)
			spawnCreature(CN_CAPERNIAN);
		if(Telonicus == NULL)
			spawnCreature(CN_TELONICUS);
	}
	
	void reset()
	{
		GameObject *GObj = NULL;
		GObj = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(743.057f, 4.63443f, 137.796f, 184069);
		if(GObj && GObj->GetUInt32Value(GAMEOBJECT_STATE) == 0)
		{
			GObj->SetUInt32Value(GAMEOBJECT_STATE, 1);
		}
		
		Creature* Darkener = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(786.28f, 20.2825f, 48.7285f, 20064);
		Creature* Sanguinar = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(785.825f, -22.1231f, 48.7285f, 20060);
		Creature* Capernian = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(791.128f, -12.6735f, 48.7285f, 20062);
		Creature* Telonicus = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(791.906f, 11.9183f, 48.7285f, 20063);

		Creature* NerherstrandLongbow = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(786.28f, 20.2825f, 48.7285f, 21268);
		Creature* Devastation = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(786.28f, 20.2825f, 48.7285f, 21269);
		Creature* CosmicInfuser = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(786.28f, 20.2825f, 48.7285f, 21270);
		Creature* InfinityBlade = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(786.28f, 20.2825f, 48.7285f, 21271);
		Creature* WarpSlicer = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(786.28f, 20.2825f, 48.7285f, 21272);
		Creature* PhaseshiftBulwark = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(786.28f, 20.2825f, 48.7285f, 21273);
		Creature* StaffOfDisintegration = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(786.28f, 20.2825f, 48.7285f, 21274);
		
		if(Darkener != NULL)
			Darkener->SafeDelete();
		if(Sanguinar != NULL)
			Sanguinar->SafeDelete();
		if(Capernian != NULL)
			Capernian->SafeDelete();
		if(Telonicus != NULL)
			Telonicus->SafeDelete();
		if(_unit != NULL)
			_unit->SafeDelete();

		
		spawnCreature(CN_DARKENER);
		spawnCreature(CN_SANGUINAR);
		spawnCreature(CN_CAPERNIAN);
		spawnCreature(CN_TELONICUS);
		spawnCreature(CN_KAELTHAS);
		
		if(NerherstrandLongbow != NULL)
			NerherstrandLongbow->SafeDelete();
		if(Devastation != NULL)
			Devastation->SafeDelete();
		if(CosmicInfuser != NULL)
			CosmicInfuser->SafeDelete();
		if(InfinityBlade != NULL)
			InfinityBlade->SafeDelete();
		if(WarpSlicer != NULL)
			WarpSlicer->SafeDelete();
		if(PhaseshiftBulwark != NULL)
			PhaseshiftBulwark->SafeDelete();
		if(StaffOfDisintegration != NULL)
			StaffOfDisintegration->SafeDelete();
		if(InfinityBlade != NULL)
			InfinityBlade->SafeDelete();
	}

	std::string itos(uint32 i)
	{
		stringstream s;
		s << i;
		return s.str();
	}

	void spawnCreature(uint32 id)
	{
		std::string my_query = "SELECT position_x, position_y, position_z, orientation FROM creature_spawns WHERE entry = " + itos(id);
		
		QueryResult *spawnResult = WorldDatabase.Query(my_query.c_str());

		if(spawnResult != NULL)
		{
			Field *fields = spawnResult->Fetch();

			float x = fields[0].GetFloat();
			float y = fields[1].GetFloat();
			float z = fields[2].GetFloat();
			float o = fields[3].GetFloat();
			
			_unit->GetMapMgr()->GetInterface()->SpawnCreature(id, x, y, z, o, false, true, 0, 0);
			
			delete spawnResult;			
		}
	}
	
	void OnDied(Unit * mKiller)
	{
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "For...Quel...thalas!");
		_unit->PlaySoundToSet(11274);
		
		RemoveAIUpdateEvent();
	}
	
	void AIUpdate()
	{
		if(_unit->GetHealthPct() < 90 && AddPhase != 6)
		{
			sLog.outString("Health is under 90 % and addphase is %u despawning...", AddPhase);
			_unit->Despawn(1,10000);
		}
		if(Speech <= 20)
		{
			_unit->setAttackTimer(2000, false);
			Speech++;
			return;
		}

		if (AddPhase < 7)
		{
			_unit->setAttackTimer(2000, false);

			switch (AddPhase)
			{
			case 1:
				FirstAd();
				break;
			case 2:
				SecondAd();
				break;
			case 3:
				ThirdAd();
				break;
			case 4:
				FourthAd();
				break;
			case 5:	// to be added
				if (WeaponPhase == 0)
					WeaponRelease();

				else
				{
					uint32 t = (uint32)time(NULL);
					if (t > WeaponPhase)
					{
						WeaponPhase = 0;
						AddActive = 5;
						AddPhase = 6;
					}
				}
				break;
			case 6:
				if (AddActive < 6)
				{
					PhaseThree();

					uint32 t = (uint32)time(NULL);
					AddsTimer = t + 120;
				}

				else
				{
					uint32 t = (uint32)time(NULL);
					if (t > AddsTimer)
					{
						_unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
						_unit->GetAIInterface()->m_canMove = true;

						PhoenixTimer = (uint32)time(NULL) + 60;
						AddActive = 0;
						AddPhase = 7;

						uint32 t = (uint32)time(NULL);
						for (int i = 0; i < 2; i++)
							spells[i].casttime = 0;
						for (int i = 2; i < 9; i++)
							spells[i].casttime = t + spells[i].cooldown;
						spells[7].casttime = t + 60;

						Unit *Target = NULL;
						Target = FindTargetForSpell();
						if (Target)
							_unit->GetAIInterface()->AttackReaction(Target, 1, 0);
					}
				}
				break;
			default:
				{
				}
			}
		}

		else
		{
			if (AddPhase == 7)
			{
				if (_unit->GetHealthPct() <= 50)
				{
					if(_unit->GetCurrentSpell() != NULL)
						_unit->GetCurrentSpell()->cancel();

					_unit->GetAIInterface()->SetAllowedToEnterCombat(false);

					_unit->GetAIInterface()->StopMovement(0);
					_unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
					_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
					_unit->GetAIInterface()->setWaypointToMove(1);

					_unit->GetAIInterface()->SetNextTarget((Unit*)NULL);
				}

				if (_unit->GetAIInterface()->getMoveType() == MOVEMENTTYPE_WANTEDWP)
					return;

				uint32 t = (uint32)time(NULL);
				if (t > spells[2].casttime && _unit->GetCurrentSpell() == NULL)
				{
					_unit->setAttackTimer(spells[2].attackstoptimer, false);

					_unit->CastSpell(_unit, spells[2].info, spells[2].instant);

					spells[2].casttime = t + spells[2].cooldown;
				}

				if (t > spells[3].casttime && _unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
				{
					_unit->GetAIInterface()->StopMovement(1);
					_unit->setAttackTimer(spells[3].attackstoptimer, false);

					CastSpellOnRandomTarget(3, spells[3].mindist2cast, spells[3].maxdist2cast, 0, 100);

					spells[3].casttime = t + spells[3].cooldown;
					spells[2].casttime = t + spells[2].cooldown;
				}

				if (t > spells[5].casttime && _unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
				{
					_unit->setAttackTimer(spells[5].attackstoptimer, false);

					CastSpellOnRandomTarget(5, spells[5].mindist2cast, spells[5].maxdist2cast, 0, 100);

					spells[5].casttime = t + spells[5].cooldown;
				}

				if (t > PhoenixTimer - 5 && _unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget() && !Phoenix)
				{
					_unit->setAttackTimer(spells[4].attackstoptimer, false);

					CastSpellOnRandomTarget(4, spells[4].mindist2cast, spells[4].maxdist2cast, 0, 100);
					return;
				}

				if (t > PhoenixTimer && Phoenix)
				{
					SummonPhoenix();

					PhoenixTimer = t + 60;
					X = Y = Z = 0.0f;
					Phoenix = false;
				}

				float val = RandomFloat(100.0f);
				SpellCast(val);
			}

			else	// to rewrite to have cool animation and better gravity system :P
			{	// not even halfway done =/ + some facing bugs
				if (Scene > 0)
				{
					GameObject *GObj = NULL;
					GObj = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(743.057f, 4.63443f, 137.796f, 184069);

					switch(Scene)
					{
					case 2:
						_unit->GetAIInterface()->SetNextTarget((Unit*)NULL);
						break;
					case 4:
						_unit->CastSpell(_unit, 36520, true);	// probably it's wrong spell too
						break;
					case 5:
						if(_unit->GetCurrentSpell() != NULL)
						_unit->GetCurrentSpell()->cancel();

						_unit->GetAIInterface()->SetAllowedToEnterCombat(false);

						_unit->GetAIInterface()->StopMovement(0);
						_unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
						_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
						_unit->GetAIInterface()->setWaypointToMove(2);

						_unit->CastSpell(_unit, 36089, true);	// makes that jump effect very nice :)
						break;
					case 6:
						for (int i = 0; i < 2; i++)
						{
							Unit *Trigger = NULL;
							Trigger = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(Triggers[i].x, Triggers[i].y, Triggers[i].z, 30000);
							if (Trigger)
							{
								Trigger->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, _unit->GetGUID());
								Trigger->SetUInt32Value(UNIT_CHANNEL_SPELL, 36198);						// wrong spell for sure
							}
						}
						break;
					case 8:
						_unit->CastSpell(_unit, 36364, true);	// adds violet ball

						_unit->SetFloatValue(OBJECT_FIELD_SCALE_X, 1);
						break;
					case 10:
						for (int i = 2; i < 4; i++)
						{
							Unit *Trigger = NULL;
							Trigger = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(Triggers[i].x, Triggers[i].y, Triggers[i].z, 30000);
							if (Trigger)
							{
								Trigger->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, _unit->GetGUID());
								Trigger->SetUInt32Value(UNIT_CHANNEL_SPELL, 36198);						// wrong spell for sure
							}
						}
						break;
					case 11:
						_unit->RemoveAura(36364);
						_unit->CastSpell(_unit, 36371, true);
						break;
					case 13:
						for (int i = 4; i < 6; i++)
						{
							Unit *Trigger = NULL;
							Trigger = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(Triggers[i].x, Triggers[i].y, Triggers[i].z, 30000);
							if (Trigger)
							{
								Trigger->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, _unit->GetGUID());
								Trigger->SetUInt32Value(UNIT_CHANNEL_SPELL, 36198);						// wrong spell for sure
							}
						}
						break;
					case 15:
						if(GObj && GObj->GetUInt32Value(GAMEOBJECT_STATE) == 1)
						{
							GObj->SetUInt32Value(GAMEOBJECT_STATE, 0);
						}
						break;
					case 16:
						for (int i = 0; i < 6; i++)
						{
							Unit *Trigger = NULL;
							Trigger = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(Triggers[i].x, Triggers[i].y, Triggers[i].z, 30000);
							if (Trigger)
							{
								Trigger->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, 0);
								Trigger->SetUInt32Value(UNIT_CHANNEL_SPELL, 0);
							}
						}

						_unit->RemoveAura(36371);
						_unit->RemoveAura(36520);
						_unit->CastSpell(_unit, 36092, true);
						_unit->CastSpell(_unit, 36185, true);
						break;
					case 18:
						_unit->CastSpell(_unit, 36091, true);
						break;
					case 23:
						_unit->RemoveAura(36185);
						_unit->CastSpell(_unit, 36187, false);
						break;
					case 24:
						_unit->GetAIInterface()->SetAllowedToEnterCombat(false);

						_unit->GetAIInterface()->StopMovement(0);
						_unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
						_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
						_unit->GetAIInterface()->setWaypointToMove(3);
						break;
					case 25:
						_unit->RemoveAura(36091);
						break;
					case 35:
						if (_unit->GetAIInterface()->GetNextTarget() == NULL && _unit->GetAIInterface()->getMoveType() != MOVEMENTTYPE_WANTEDWP)
						{
							_unit->RemoveAura(36187);
							_unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
							_unit->GetAIInterface()->SetAllowedToEnterCombat(true);

							Unit *Target = NULL;
							Target = FindTargetForSpell();
							if (Target)
								_unit->GetAIInterface()->AttackReaction(Target, 1, 0);

							_unit->GetAIInterface()->m_moveRun = true;
						}
						break;
					default:
						{
						}
					}

					if (Scene >= 2 && _unit->GetOrientation() != 3.160250f)
						_unit->SetFacing(3.160250f);

					Scene++;
					if (Scene > 35)
						Scene = 0;
					else return;
				}

				_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);

				uint32 t = (uint32)time(NULL);
				if (_unit->GetAIInterface()->GetNextTarget() && _unit->GetCurrentSpell() == NULL && RandomUInt(12) == 1 && t > spells[6].casttime)
				{
					_unit->setAttackTimer(spells[6].attackstoptimer, false);

					CastSpellOnRandomTarget(6, spells[6].mindist2cast, spells[6].maxdist2cast, 0, 100);

					spells[6].casttime = t + spells[6].cooldown;
					return;
				}

				// doesn't work yet
				if (_unit->GetCurrentSpell() == NULL && t > spells[7].casttime)
				{
					_unit->setAttackTimer(spells[7].attackstoptimer, false);

					//_unit->CastSpell(_unit, spells[7].info, spells[7].instant);
					// hax!
					std::vector<Unit*> TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
														/* If anyone wants to use this function, then leave this note!										 */
					for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
					{ 
						if (isHostile(_unit, (*itr)) && (*itr) != _unit && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID())
						{
							Unit* Target = NULL;
							Target = (Unit*)(*itr);

							if (Target->isAlive() && _unit->GetDistance2dSq(Target) >= spells[7].mindist2cast*spells[7].mindist2cast && _unit->GetDistance2dSq(Target) <= spells[7].maxdist2cast*spells[7].maxdist2cast && _unit->GetAIInterface()->getThreatByPtr(Target) > 0 && isHostile(_unit, Target))
							{
								TargetTable.push_back(Target);
							} 
						} 
					}

					if (TargetTable.size())
					{
						for (size_t i = 0; i < TargetTable.size(); i++)
						{
							Unit* Target = TargetTable[i];
							if (Target)
							{
								Target->CastSpell(Target, 34480, spells[7].instant);
								if (Target->GetTypeId() == TYPEID_PLAYER)
								{
									Player *pPlayer = NULL;
									pPlayer = (Player*)Target;
									if (pPlayer)
									{
										WorldPacket fly(835, 13);
										pPlayer->m_setflycheat = true;
										fly << pPlayer->GetNewGUID();
										fly << uint32(2);
										pPlayer->SendMessageToSet(&fly, true);
									}
								}
							}
						}
					}

					TargetTable.clear();
						

					_unit->GetAIInterface()->setCurrentAgent(AGENT_SPELL);
					spells[7].casttime = t + spells[7].cooldown;
					spells[8].casttime = t;
					return;
				}

				if (t > spells[8].casttime && _unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
				{
					_unit->GetAIInterface()->setCurrentAgent(AGENT_SPELL);
					Unit *Target = FindTarget();
					if (!Target)
					{
						_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);

						// hax!
						std::vector<Unit*> TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
															/* If anyone wants to use this function, then leave this note!										 */
						for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
						{ 
							if (isHostile(_unit, (*itr)) && (*itr) != _unit && (*itr)->GetTypeId() == TYPEID_PLAYER && (*itr)->GetInstanceID() == _unit->GetInstanceID())
							{
								Unit* Target = NULL;
								Target = (Unit*)(*itr);

								if (Target->isAlive())
								{
									TargetTable.push_back(Target);
								} 
							} 
						}

						if (TargetTable.size())
						{
							for (size_t i = 0; i < TargetTable.size(); i++)
							{
								Unit* Target = TargetTable[i];
								if (Target && Target->GetTypeId() == TYPEID_PLAYER)
								{
									Player *pPlayer = NULL;
									pPlayer = (Player*)Target;
									if (pPlayer)
									{
										WorldPacket fly(836, 13);
										pPlayer->m_setflycheat = false;
										fly << pPlayer->GetNewGUID();
										fly << uint32(5);
										pPlayer->SendMessageToSet(&fly, true);
									}
								}
							}
						}

						TargetTable.clear();

						spells[8].casttime = t + 90;
						spells[7].casttime = t + 60;
						return;
					}

					else
					{
						_unit->setAttackTimer(spells[8].attackstoptimer, false);

						_unit->CastSpell(Target, spells[8].info, spells[8].instant);
					}

					spells[8].casttime = t + spells[8].cooldown;
				}

				float val = RandomFloat(100.0f);
				SpellCast(val);
			}
		}
	}
	
	void FirstAd()
	{
		Unit *Darkener = NULL;
		Darkener = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(786.28f, 20.2825f, 48.7285f, 20064);
		if (Darkener && Darkener->isAlive() && AddActive < 1)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Let us see how your nerves hold up against the Darkener, Thaladred."); 
			_unit->PlaySoundToSet(11259);

			Darkener->GetAIInterface()->SetAllowedToEnterCombat(true);
			Darkener->SetUInt64Value(UNIT_FIELD_FLAGS, 0);

			Unit *target = FindTargetForSpell();
			if (target)
			{
				Darkener->GetAIInterface()->AttackReaction(target, 1, 0);
			}

			AddActive = 1;
		}

		if (Darkener && !Darkener->isAlive())
		{
			AddPhase = 2;
			SecondAd();
		}
	}

	void SecondAd()
	{
		Unit *Sanguinar = NULL;
		Sanguinar = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(785.825f, -22.1231f, 48.7285f, 20060);
		if (Sanguinar && Sanguinar->isAlive() && AddActive < 2)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You have persevered against some of my best advisors. But none can withstand the might of the Bloodhammer. Behold, Lord Sanguinar."); 
			_unit->PlaySoundToSet(11260);

			Sanguinar->GetAIInterface()->SetAllowedToEnterCombat(true);
			Sanguinar->SetUInt64Value(UNIT_FIELD_FLAGS, 0);

			Unit *target = FindTargetForSpell();
			if (target)
			{
				Sanguinar->GetAIInterface()->AttackReaction(target, 1, 0);
			}

			AddActive = 2;
		}

		if (Sanguinar && !Sanguinar->isAlive())
		{
			AddPhase = 3;
			ThirdAd();
		}
	}

	void ThirdAd()
	{
		Unit *Capernian = NULL;
		Capernian = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(791.128f, -12.6735f, 48.7285f, 20062);
		if (Capernian && Capernian->isAlive() && AddActive < 3)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Capernian will see to it that your stay here is a short one."); 
			_unit->PlaySoundToSet(11257);

			Capernian->GetAIInterface()->SetAllowedToEnterCombat(true);
			Capernian->SetUInt64Value(UNIT_FIELD_FLAGS, 0);

			Unit *target = FindTargetForSpell();
			if (target)
			{
				Capernian->GetAIInterface()->AttackReaction(target, 1, 0);
			}

			AddActive = 3;
		}

		if (Capernian && !Capernian->isAlive())
		{
			AddPhase = 4;
			FourthAd();
		}
	}

	void FourthAd()
	{
		Unit *Telonicus = NULL;
		Telonicus = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(791.906f, 11.9183f, 48.7285f, 20063);
		if (Telonicus && Telonicus->isAlive() && AddActive < 4)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Well done. You have proven worthy to test your skills against my Master Engineer, Telonicus."); 
			_unit->PlaySoundToSet(11258);

			Telonicus->GetAIInterface()->SetAllowedToEnterCombat(true);
			Telonicus->SetUInt64Value(UNIT_FIELD_FLAGS, 0);

			Unit *target = FindTargetForSpell();
			if (target)
			{
				Telonicus->GetAIInterface()->AttackReaction(target, 1, 0);
			}

			AddActive = 4;
		}

		if (Telonicus && !Telonicus->isAlive())
		{
			AddActive = 0;
			AddPhase = 5;
		}
	}

	void WeaponRelease()
	{
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "As you see, I have many weapons in my arsenal...");
		_unit->PlaySoundToSet(11261);

        for (uint32 i = 0; i < 7; i++)
        {
			_unit->GetMapMgr()->GetInterface()->SpawnCreature((uint32)KaelthasWeapons[i][0], KaelthasWeapons[i][1],KaelthasWeapons[i][2],KaelthasWeapons[i][3], 0.0f, false, true, 0, 0);
        }

		WeaponPhase = (uint32)time(NULL) + 95;
	}

	void PhaseThree()
	{
		Creature *Darkener = NULL;
		Creature *Sanguinar = NULL;
		Creature *Capernian = NULL;
		Creature *Telonicus = NULL;

		Darkener = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(786.28f, 20.2825f, 48.7285f, 20064);
		Sanguinar = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(785.825f, -22.1231f, 48.7285f, 20060);
		Capernian = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(791.128f, -12.6735f, 48.7285f, 20062);
		Telonicus = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(791.906f, 11.9183f, 48.7285f, 20063);
		
		if(AddActive < 6)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Perhaps I underestimated you. It would be unfair to make you fight all four Advisors at once, but...fair treatment was never shown to my people. I'm just returning the favor.");
			_unit->PlaySoundToSet(11262);
			
			if (Darkener && !Darkener->isAlive())
			{
				//Darkener->Despawn(0,0);
				//Darkener = NULL;
				_unit->CastSpell(Darkener, dbcSpell.LookupEntry(36450), true); //Spell Visual on kaelthas
				Darkener->CastSpell(Darkener, dbcSpell.LookupEntry(36450), true);
				Darkener->setDeathState(ALIVE);
				Darkener->SetUInt32Value(UNIT_FIELD_HEALTH, _unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH));
				Darkener->GetAIInterface()->SetAllowedToEnterCombat(true);
				Darkener->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
				//Darkener->m_noRespawn = true;

				Unit *target = FindTargetForSpell();
				if (target)
				{
					Darkener->GetAIInterface()->AttackReaction(target, 1, 0);
				}				
			}

			/*if (!Darkener)
			{
				Darkener = _unit->GetMapMgr()->GetInterface()->SpawnCreature(20064, 786.28f, 20.2825f, 48.7285f, _unit->GetOrientation(), true, false, 0, 0);
				if (Darkener)
				{
					Darkener->GetAIInterface()->SetAllowedToEnterCombat(true);
					Darkener->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
					Darkener->m_noRespawn = true;

					Unit *target = FindTargetForSpell();
					if (target)
					{
						Darkener->GetAIInterface()->AttackReaction(target, 1, 0);
					}
				}
			}*/

			if (Sanguinar && !Sanguinar->isAlive())
			{
				//Sanguinar->Despawn(0,0);
				//Sanguinar = NULL;
				Sanguinar->CastSpell(Sanguinar, dbcSpell.LookupEntry(36450), true);
				Sanguinar->setDeathState(ALIVE);
				Sanguinar->SetUInt32Value(UNIT_FIELD_HEALTH, _unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH));
				Sanguinar->GetAIInterface()->SetAllowedToEnterCombat(true);
				Sanguinar->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
				//Sanguinar->m_noRespawn = true;

				Unit *target = FindTargetForSpell();
				if (target)
				{
					Sanguinar->GetAIInterface()->AttackReaction(target, 1, 0);
				}
			}

			/*if (!Sanguinar)
			{
				Sanguinar = _unit->GetMapMgr()->GetInterface()->SpawnCreature(20060, 785.825f, -22.1231f, 48.7285f, _unit->GetOrientation(), true, false, 0, 0);
				if (Sanguinar)
				{
					Sanguinar->GetAIInterface()->SetAllowedToEnterCombat(true);
					Sanguinar->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
					Sanguinar->m_noRespawn = true;

					Unit *target = FindTargetForSpell();
					if (target)
					{
						Sanguinar->GetAIInterface()->AttackReaction(target, 1, 0);
					}
				}
			}*/

			if (Capernian && !Capernian->isAlive())
			{
				//Capernian->Despawn(0,0);
				//Capernian = NULL;
				Capernian->CastSpell(Capernian, dbcSpell.LookupEntry(36450), true);
				Capernian->setDeathState(ALIVE);
				Capernian->SetUInt32Value(UNIT_FIELD_HEALTH, _unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH));
				Capernian->GetAIInterface()->SetAllowedToEnterCombat(true);
				Capernian->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
				//Capernian->m_noRespawn = true;

				Unit *target = FindTargetForSpell();
				if (target)
				{
					Capernian->GetAIInterface()->AttackReaction(target, 1, 0);
				}
			}

			/*if (!Capernian)
			{
				Capernian = _unit->GetMapMgr()->GetInterface()->SpawnCreature(20062, 791.128f, -12.6735f, 48.7285f, _unit->GetOrientation(), true, false, 0, 0);
				if (Capernian)
				{
					Capernian->GetAIInterface()->SetAllowedToEnterCombat(true);
					Capernian->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
					Capernian->m_noRespawn = true;

					Unit *target = FindTargetForSpell();
					if (target)
					{
						Capernian->GetAIInterface()->AttackReaction(target, 1, 0);
					}
				}
			}*/

			if (Telonicus && !Telonicus->isAlive())
			{
				//Telonicus->Despawn(0,0);
				//Telonicus = NULL;
				Telonicus->CastSpell(Telonicus, dbcSpell.LookupEntry(36450), true);
				Telonicus->setDeathState(ALIVE);
				Telonicus->SetUInt32Value(UNIT_FIELD_HEALTH, _unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH));
				Telonicus->GetAIInterface()->SetAllowedToEnterCombat(true);
				Telonicus->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
				//Telonicus->m_noRespawn = true;

				Unit *target = FindTargetForSpell();
				if (target)
				{
					Telonicus->GetAIInterface()->AttackReaction(target, 1, 0);
				}
			}

			/*if (!Telonicus)
			{
				Telonicus = _unit->GetMapMgr()->GetInterface()->SpawnCreature(20063, 791.906f, 11.9183f, 48.7285f, _unit->GetOrientation(), true, false, 0, 0);
				if (Telonicus)
				{
					Telonicus->GetAIInterface()->SetAllowedToEnterCombat(true);
					Telonicus->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
					Telonicus->m_noRespawn = true;

					Unit *target = FindTargetForSpell();
					if (target)
					{
						Telonicus->GetAIInterface()->AttackReaction(target, 1, 0);
					}
				}
			}*/

			AddActive = 6;
			return;
		}
	}

	void SummonPhoenix()
	{
		PhoenixTimer = (uint32)time(NULL) + 60;

		if(_unit->GetAIInterface()->GetNextTarget())
		{
			Unit *Phoenix = NULL;
			Phoenix = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_PHOENIX_EGG, X, Y, Z, _unit->GetOrientation(), false, false, 0, 0);
			if (Phoenix)
			{
				Unit *PhoenixTarget = NULL;
				PhoenixTarget = FindTargetForSpell();
				if (PhoenixTarget)
					Phoenix->GetAIInterface()->AttackReaction(PhoenixTarget, 1, 0);
			}

			int RandomSpeach=rand()%2;
			switch (RandomSpeach)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Anara'nel belore!");
				_unit->PlaySoundToSet(11267);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "By the power of the sun!");
				_unit->PlaySoundToSet(11266);
				break;
			}
		}
	}

	void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					if (!spells[i].instant)
						_unit->GetAIInterface()->StopMovement(1);

					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
						case TARGET_RANDOM_FRIEND:
						case TARGET_RANDOM_SINGLE:
						case TARGET_RANDOM_DESTINATION:
							CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast); break;
					}

					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}

					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<Unit*> TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					Unit* RandomTarget = NULL;
					RandomTarget = (Unit*)(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			Unit * RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();

			if (i == 4)
			{
				X = RTarget->GetPositionX();
				Y = RTarget->GetPositionY();
				Z = RTarget->GetPositionZ();
				Phoenix = true;
			}
		}
	}

	// A bit rewritten FindTarget function
	Unit* FindTargetForSpell()
	{
		Unit* target = NULL;
		float distance = 150.0f;

		Unit *pUnit;
		float dist;

		for (std::set<Object*>::iterator itr = _unit->GetInRangeOppFactsSetBegin(); itr != _unit->GetInRangeOppFactsSetEnd(); itr++)
		{
			if((*itr)->GetTypeId() != TYPEID_UNIT && (*itr)->GetTypeId() != TYPEID_PLAYER)
				continue;

			pUnit = static_cast<Unit*>((*itr));

			if(pUnit->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_FEIGN_DEATH))
				continue;

			if(pUnit->m_invisible)
				continue;
			
			if(!pUnit->isAlive() || _unit == pUnit)
				continue;

			dist = _unit->GetDistance2dSq(pUnit);

			if(dist > distance*distance)
				continue;

			target = pUnit;
			break;
		}

		return target;
	}

	Unit *FindTarget()
	{
		std::vector<Unit*> TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
											/* If anyone wants to use this function, then leave this note!										 */
		for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
		{ 
			if (isHostile(_unit, (*itr)) && (*itr) != _unit && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID())
			{
				Unit* RandomTarget = NULL;
				RandomTarget = (Unit*)(*itr);
				if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= spells[8].mindist2cast*spells[8].mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= spells[8].maxdist2cast*spells[8].maxdist2cast && _unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && RandomTarget->FindAura(34480))
				{
					TargetTable.push_back(RandomTarget);
				} 
			} 
		}

		if (!TargetTable.size())
			return NULL;

		size_t RandTarget = rand()%TargetTable.size();
		Unit * RTarget = TargetTable[RandTarget];

		if (!RTarget)
			return NULL;

		TargetTable.clear();

		return RTarget;
	}

	void OnReachWP(uint32 iWaypointId, bool bForwards)
	{
		if (iWaypointId == 1)
		{
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
			_unit->GetAIInterface()->m_moveFly = true;

			WorldPacket data(SMSG_MOVE_SET_HOVER, 13);
            data << _unit->GetNewGUID();
            data << uint32(0);
            _unit->SendMessageToSet(&data, false);

			AddPhase = 8;
			Scene = 1;
		}

		if (iWaypointId == 3)
		{
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
			_unit->GetAIInterface()->setWaypointToMove(0);
			_unit->GetAIInterface()->m_moveFly = false;

			WorldPacket data(SMSG_MOVE_UNSET_HOVER, 13);
			data << _unit->GetNewGUID();
			data << uint32(0);
			_unit->SendMessageToSet(&data, false);

			Scene = 33;
		}
	}

	inline WayPoint* CreateWaypoint(int id, uint32 waittime, uint32 flags)
    {
        WayPoint * wp = _unit->CreateWaypointStruct();
        wp->id = id;
		wp->x = WP[id].x;
		wp->y = WP[id].y;
		wp->z = WP[id].z;
		wp->o = 3.160250f;
        wp->waittime = waittime;
        wp->flags = flags;
        wp->forwardemoteoneshot = 0;
        wp->forwardemoteid = 0;
        wp->backwardemoteoneshot = 0;
        wp->backwardemoteid = 0;
        wp->forwardskinid = 0;
        wp->backwardskinid = 0;
        return wp;
    }

protected:	

	float X, Y, Z;
	uint32 PhoenixTimer;
	uint32 WeaponPhase;
	uint32 AddsTimer;
	int SummonTimer;
	int AddActive;
	int AddPhase;
	int nrspells;
	bool Phoenix;
	uint32 Scene;
	int Speech;
};


void SetupTheEye(ScriptMgr * mgr)
{
	//Trash mobs
	mgr->register_creature_script(CN_A_STAR_SCRYER, &AStarScryerAI::Create);
	mgr->register_creature_script(CN_ASTROMANCER, &AstromancerAI::Create);
	mgr->register_creature_script(CN_STAR_SCRYER, &StarScryerAI::Create);
	mgr->register_creature_script(CN_ASTROMANCER_LORD, &AstromancerLordAI::Create);
	mgr->register_creature_script(CN_BLOOD_VINDICATOR, &BloodVindicatorAI::Create);
	mgr->register_creature_script(CN_BLOOD_LEGIONNARE, &BloodLegionnareAI::Create);
	mgr->register_creature_script(CN_BLOOD_MARSHAL, &BloodMarshalAI::Create);
	mgr->register_creature_script(CN_PHOENIX_HAWK, &PhoenixHawkAI::Create);
	mgr->register_creature_script(CN_CRYSTAL_SENTINEL, &CrystalSentinelAI::Create);
	mgr->register_creature_script(CN_CRYSTAL_MECHANIC, &CrystalMechanicAI::Create);

	//Void Reaver event
	mgr->register_creature_script(CN_VOID_REAVER, &VoidReaverAI::Create);
	mgr->register_creature_script(CN_ARCANEORBTARGET, &ArcaneOrbTargetAI::Create);

	//High Astromancer Solarian
	mgr->register_creature_script(CN_SOLARIAN, &HighAstromancerSolarianAI::Create);
	mgr->register_creature_script(CN_SOLARIUMAGENT, &SolariumAgentAI::Create);
	mgr->register_creature_script(CN_SOLARIUMPRIEST, &SolariumPriestAI::Create);
	mgr->register_creature_script(CN_SOLARIUM_SPOT_LIGHT, &SolariumSpotLight::Create);
	mgr->register_dummy_spell(SOLARIAN_WRATH_OF_THE_ASTROMANCER, &Dummy_Solarian_WrathOfTheAstromancer);

	//Al'ar event
	mgr->register_creature_script(CN_ALAR, &AlarAI::Create);
	mgr->register_creature_script(CN_EMBEROFALAR, &EmberAlarAI::Create);
	mgr->register_creature_script(CN_PATCHALAR, &PatchAlarAI::Create);

	//Kael'Thas Encounter
	mgr->register_creature_script(CN_PHOENIX, &PhoenixAI::Create);
	mgr->register_creature_script(CN_PHOENIX_EGG, &PhoenixEggAI::Create);
	mgr->register_creature_script(CN_FLAMESTRIKE, &FlameStrikeAI::Create);
	mgr->register_creature_script(CN_DARKENER, &DarkenerAI::Create);
	mgr->register_creature_script(CN_SANGUINAR, &SanguinarAI::Create);
	mgr->register_creature_script(CN_CAPERNIAN, &CapernianAI::Create);
	mgr->register_creature_script(CN_TELONICUS, &TelonicusAI::Create);
	mgr->register_creature_script(CN_KAELTHAS, &KaelThasAI::Create);
	
	// Kael'thas Weapons
	mgr->register_creature_script(NETHERSTRAND_LONGBOW, &WeaponsAI::Create);
	mgr->register_creature_script(DEVASTATION, &WeaponsAI::Create);
	mgr->register_creature_script(COSMIC_INFUSER, &WeaponsAI::Create);
	mgr->register_creature_script(INFINITY_BLADE, &WeaponsAI::Create);
	mgr->register_creature_script(WARP_SLICER, &WeaponsAI::Create);
	mgr->register_creature_script(PHASESHIFT_BULWARK, &WeaponsAI::Create);
	mgr->register_creature_script(STAFF_OF_DISINTEGRATION, &WeaponsAI::Create);
}
