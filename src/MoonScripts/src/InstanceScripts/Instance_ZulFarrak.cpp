/*
 * Moon++ Scripts for Ascent MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
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

/************************************************************************/
/* Instance_ZulFarrak.cpp Script										*/
/************************************************************************/

//Theka the Martyr

// casts the spell Theka Transform 11089 at %30  hp
// casts the spell fevered plague around each 17 second
/*
Fevered Plague 8600 = Inflicts 250 Nature damage to an enemy, then an additional 11 damage every 5 sec. for 3 min.
Fevered Plague 16186 =  Inflicts 72 to 78 Nature damage to an enemy, then an additional 10 damage every 3 sec. for 30 sec. */

#define theka_transform 11089
#define fevered_plague 16186
//#define fevered_plague 8600  i dont know wich one it is he casts

class thekaAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(thekaAI);

    thekaAI(Creature* pCreature) : CreatureAIScript(pCreature) 
	{

        morph = dbcSpell.LookupEntry(theka_transform);

		plague = dbcSpell.LookupEntry(fevered_plague);
	}

	void OnCombatStart(Unit* mTarget) 
	{
		morphcheck = true;
		plaguecount = 0;
		RegisterAIUpdateEvent(1000);
	}

	void OnCombatStop(Unit* mTarget) 
	{
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		morphcheck = false;
		plaguecount = 0;
        RemoveAIUpdateEvent();
	}

	void OnDied(Unit *mKiller) 
	{
		morphcheck = false;
		plaguecount = 0;
        RemoveAIUpdateEvent();
	}

	void AIUpdate() 
	{
		plaguecount++;
		randomplague = 16 + RandomUInt(3);
		if(plaguecount >= randomplague && _unit->GetAIInterface()->GetNextTarget())
		{
			plaguecount = 0;
			Unit *target = NULL;
			target = _unit->GetAIInterface()->GetNextTarget();
			_unit->CastSpell(target, plague, true);
		}
		else if(_unit->GetHealthPct() <= 30 && morphcheck)
		{
			morphcheck = false;

			_unit->CastSpell(_unit, morph, false);
		}
	}

    void Destroy()
    {
        delete (thekaAI*)this;
    }

protected:
	int plaguecount, randomplague;
	bool morphcheck;
	SpellEntry * morph;
	SpellEntry * plague;
};


//----------------------------------------------------------------------------------------------------------

// Antu'sul

/* 
needs a aggro trigger outside cave

yells

Lunch has arrived, my beutiful childern. Tear them to pieces!   // on aggro
Rise and defend your master!  //  at 75% when his add spawn
The children of sul will protect their master. Rise once more Sul'lithuz! // at  25% when his adds start spawning again but he says it only once and adds keep on spawning

spells

Healing Ward 11889  random around 18 sec
Earthgrab Totem 8376 random around 18 sec

misc info

he summons 6 Sul'lithuz Broodling 8138 on aggro
he summons Servant of antu'sul 8156 75% with spell 11894
he summons Servant of antu'sul 8156 25% with spell 11894 each 15 second  

------------------------------------------------------------------------------------------------
*/
class antusulTriggerAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(antusulTriggerAI);

	antusulTriggerAI(Creature* pCreature) : CreatureAIScript(pCreature){}

	void OnCombatStart(Unit* mTarget) 
	{
		_unit->GetAIInterface()->m_canMove = false;
		_unit->GetAIInterface()->disable_melee = true;
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

		Unit *antusul = NULL;
		antusul = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(1815.030029f, 686.817017f, 14.519000f, 8127);
		if(antusul)
		{
			if(antusul->isAlive())
			{
				antusul->GetAIInterface()->AttackReaction(mTarget, 0, 0);
				antusul->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Lunch has arrived, my beutiful childern. Tear them to pieces!");
			}
		}
	}
};
/*--------------------------------------------------------------------------------------*/
/*antusul  healing ward and earthgrab ward commented out since they need time and work wich i dont have right now */
#define servants 11894
#define healingward 11889
#define earthgrabward 8376

class antusulAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(antusulAI);

    antusulAI(Creature* pCreature) : CreatureAIScript(pCreature) 
	{

        servant = dbcSpell.LookupEntry(servants);
		//healing_ward = dbcSpell.LookupEntry(healingward);
		//earthgrab_ward = dbcSpell.LookupEntry(earthgrabward);
	}

	void OnCombatStart(Unit* mTarget) 
	{
		add1 = add2 = add3 = add4 = add5 = add6 = trigger = NULL;
		spawns = firstspawn = secondspawn = true;
		spawns2 = attack = false;
		/*healingwardcount = earthgrabcount = hmax = emax =*/ secondspawncount = 0;
		RegisterAIUpdateEvent(1000);
	}

	void OnCombatStop(Unit* mTarget) 
	{
		spawns = spawns2 = attack = firstspawn = secondspawn = false;
		/*healingwardcount = earthgrabcount = hmax = emax =*/ secondspawncount = 0;
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
		resettrigger();
		deletespawns();
	}

	void OnDied(Unit *mKiller) 
	{
		spawns = spawns2 = attack = firstspawn = secondspawn = false;
		/*healingwardcount = earthgrabcount = hmax = emax =*/ secondspawncount = 0;
        RemoveAIUpdateEvent();
		trigger = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(1811.943726f, 714.839417f, 12.897189f, 133337);
		if (trigger)
			trigger->Despawn(100, 0);
	}

	void AIUpdate() 
	{
		//healingwardcount++;
		//earthgrabcount++;
		//hmax = 15 + RandomUInt(5);
		//emax = 15 + RandomUInt(5);
		if(_unit->GetHealthPct() <= 75 && firstspawn)
		{
			firstspawn = false;
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Rise and defend your master!");
			_unit->CastSpell(_unit, servant, true);
		}
		if(_unit->GetHealthPct() <= 25)
		{
			secondspawncount++;
			if(secondspawn)
			{
				secondspawn = false;
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The children of sul will protect their master. Rise once more Sul'lithuz!");
				_unit->CastSpell(_unit, servant, true);
			}
			if(secondspawncount >= 15)
			{
				secondspawncount = 0;
				_unit->CastSpell(_unit, servant, true);
			}

		}
		if(attack)
		{
			Unit * Target = NULL;
			Target = _unit->GetAIInterface()->GetNextTarget();
			if (_unit->GetAIInterface()->GetNextTarget())
			{
				if(add1 && Target)
				{
					add1->GetAIInterface()->AttackReaction(Target, 0, 0);
				}
				if(add2 && Target)
				{
					add2->GetAIInterface()->AttackReaction(Target, 0, 0);
				}
				if(add3 && Target)
				{
					add3->GetAIInterface()->AttackReaction(Target, 0, 0);
				}
				if(add4 && Target)
				{
					add4->GetAIInterface()->AttackReaction(Target, 0, 0);
				}
				if(add5 && Target)
				{
					add5->GetAIInterface()->AttackReaction(Target, 0, 0);
				}
				if(add6 && Target)
				{
					add6->GetAIInterface()->AttackReaction(Target, 0, 0);
				}
			}

			attack = false;
		}
		if(spawns2)
		{
			spawns2 = false;
			addsdefine();
			attack = true;
		}
		if(spawns)
		{
			spawns = false;
			spawnadds();
			spawns2 = true;
		}
		/*if(healingwardcount >= hmax)
		{
			//_unit->CastSpell(_unit, healing_ward, true);    wrong spell id cant find right one
			healingwardcount = 0;
		}
		if(earthgrabcount >= emax)
		{
			//_unit->CastSpell(_unit, earthgrab_ward, true);   the totem needs to be scripted with its own ai
			earthgrabcount = 0;
		}*/
	}

	void spawnadds()
	{
		_unit->GetMapMgr()->GetInterface()->SpawnCreature(8138, 1777.753540f, 741.063538f, 16.439308f, 6.197119f, true, false, 0, 0);
		_unit->GetMapMgr()->GetInterface()->SpawnCreature(8138, 1782.193481f, 751.190002f, 16.620836f, 5.174994f, true, false, 0, 0);
		_unit->GetMapMgr()->GetInterface()->SpawnCreature(8138, 1790.956299f, 754.666809f, 14.195786f, 5.174208f, true, false, 0, 0);
		_unit->GetMapMgr()->GetInterface()->SpawnCreature(8138, 1800.902710f, 755.723267f, 15.642491f, 4.545889f, true, false, 0, 0);
		_unit->GetMapMgr()->GetInterface()->SpawnCreature(8138, 1809.339722f, 749.212402f, 16.910545f, 4.109208f, true, false, 0, 0);
		_unit->GetMapMgr()->GetInterface()->SpawnCreature(8138, 1818.182129f, 744.702820f, 17.801855f, 3.899507f, true, false, 0, 0);
	}
	
	void addsdefine()
	{
		add1 = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(1777.753540f, 741.063538f, 16.439308f, 8138);
		add2 = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(1782.193481f, 751.190002f, 16.620836f, 8138);
		add3 = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(1790.956299f, 754.666809f, 14.195786f, 8138);
		add4 = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(1800.902710f, 755.723267f, 15.642491f, 8138);
		add5 = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(1809.339722f, 749.212402f, 16.910545f, 8138);
		add6 = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(1818.182129f, 744.702820f, 17.801855f, 8138);	
	}
	
	void resettrigger()
	{
		trigger = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(1811.943726f, 714.839417f, 12.897189f, 133337);
		if(trigger)
		{
			trigger->GetAIInterface()->m_canMove = true;
			trigger->GetAIInterface()->disable_melee = false;
			trigger->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
		}
	}

	void deletespawns()
	{
		if(add1)
		{
			add1->Despawn(1000, 0);
		}
		if(add2)
		{
			add2->Despawn(1000, 0);
		}
		if(add3)
		{
			add3->Despawn(1000, 0);
		}
		if(add4)
		{
			add4->Despawn(1000, 0);
		}
		if(add5)
		{
			add5->Despawn(1000, 0);
		}
		if(add6)
		{
			add6->Despawn(1000, 0);
		}
	}

    void Destroy()
    {
        delete (antusulAI*)this;
    }

protected:
	bool spawns, spawns2, attack, firstspawn, secondspawn;
	int /*healingwardcount, earthgrabcount, hmax, emax,*/ secondspawncount;

	Creature * add1;
	Creature * add2;
	Creature * add3;
	Creature * add4;
	Creature * add5;
	Creature * add6;
	Creature * trigger;

	SpellEntry * servant;
	SpellEntry * healing_ward;
	SpellEntry * earthgrab_ward;
};


void SetupZulFarrak(ScriptMgr * mgr)
{
	/*mgr->register_creature_script(8127, &antusulAI::Create);
	mgr->register_creature_script(7272, &thekaAI::Create);
	mgr->register_creature_script(133337, &antusulTriggerAI::Create);*/
}