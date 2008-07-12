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
/* Raid_TheEye.cpp Script												*/
/************************************************************************/

// Al'ar AI

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
#define METEOR 35181		// but shouldn't be instant imho
#define EMBER_BLAST 34133	// used when one of adds has low health
/*#define MELT_ARMOR		// maybe they are used, but not sure
#define RANDOM_CHARGE*/

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
	{ 328.103455f,  -0.192393f, 52.216309f, 4.188326f },	// fire quills cast position	// EMOTE_STATE_WHIRLWIND = 382,
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
		spells[0].cooldown = -1;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

        spells[1].info = dbcSpell.LookupEntry(FLAME_QUILLS);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].cooldown = -1;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(SUMMON_PHOENIX_ADDS);
		spells[2].targettype = TARGET_VARIOUS;
		spells[2].instant = true;
		spells[2].cooldown = -1;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;

		spells[3].info = dbcSpell.LookupEntry(FLAME_PATCH);
		spells[3].targettype = TARGET_VARIOUS;
		spells[3].instant = true;
		spells[3].cooldown = -1;
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 1000;

		spells[4].info = dbcSpell.LookupEntry(METEOR);
		spells[4].targettype = TARGET_ATTACKING;
		spells[4].instant = true;	// =(
		spells[4].cooldown = -1;
		spells[4].perctrigger = 0.0f;
		spells[4].attackstoptimer = 1000;

		spells[5].info = dbcSpell.LookupEntry(EMBER_BLAST);
		spells[5].targettype = TARGET_VARIOUS;
		spells[5].instant = true;
		spells[5].cooldown = -1;
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

		PositionChange=rand()%8+15;
		PhoenixSummon=rand()%6+17;
		FlameQuills = false;
		Meteor = false;
		Phase = 0;
        //RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);	// reseting movetype and adding it once again to let it move from 1-5 again
		_unit->GetAIInterface()->setWaypointToMove(0);

		_unit->GetAIInterface()->StopMovement(0);
		_unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
		CastTime();

		FlyWay = rand()%2;
		switch (FlyWay)
		{
		case 0:	// Clock like
			{
				_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
				_unit->GetAIInterface()->setWaypointToMove(6);
			}break;

		case 1:	// hmm... other?
			{

				_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
				_unit->GetAIInterface()->setWaypointToMove(9);
			}break;
		}
		
		PositionChange=rand()%8+15;	// 30-45sec /*** if attack time 1000 (%15+31) ***/
		PhoenixSummon=rand()%6+17;	// 34-44sec /*** if attack time 1000 (%11+34) ***/
		FlameQuills = false;
		Meteor = false;
		Phase = 1;

		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

    void OnCombatStop(Unit *mTarget)
    {
		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);	// reseting movetype and adding it once again to let it move from 1-5 again
		_unit->GetAIInterface()->setWaypointToMove(0);

		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_FORWARDTHANSTOP);
		_unit->GetAIInterface()->m_moveFly = true;
		CastTime();

		PositionChange=rand()%8+15;
		PhoenixSummon=rand()%6+17;
		FlameQuills = false;
		Meteor = false;
		Phase = 0;

        //_unit->GetAIInterface()->StopMovement(0);
        //_unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
		//_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
		//_unit->GetAIInterface()->setWaypointToMove(1);
        //_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        //_unit->GetAIInterface()->SetAIState(STATE_IDLE);
        //RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		Phase = 0;
		FlameQuills = false;
		Meteor = false;
		PositionChange=rand()%8+15;
		PhoenixSummon=rand()%6+17;
		CastTime();
       //RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		if (FlameQuills == true)
		{
			QuillsCount++;
			if (QuillsCount == 9)
			{
				FlameQuills = false;
				switch (FlyWay)
				{
				case 0:	// Clock like
					{
						_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
						_unit->GetAIInterface()->setWaypointToMove(6);
					}break;
		
				case 1:	// hmm... other?
					{
						_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
						_unit->GetAIInterface()->setWaypointToMove(9);
					}break;
				}
			}
			_unit->CastSpell(_unit, spells[1].info, spells[1].instant);
		}

		if (Meteor == true)
		{
		}
		
		else 
		{
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
					Phase = 0;
				};
			};
		}
		//float val = (float)RandomFloat(100.0f);
		//SpellCast(val);
    }

    void PhaseOne()
    {
		PositionChange--;
		PhoenixSummon--;

		if (_unit->GetHealthPct() == 0)
		{
			Phase = 2;
			_unit->CastSpell(_unit, spells[6].info, spells[6].instant);
		}

		if (!PhoenixSummon--)
		{
			_unit->CastSpell(_unit, spells[2].info, spells[2].instant);
			PhoenixSummon=rand()%6+17;
		}

		if (!PositionChange)
		{
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
			_unit->GetAIInterface()->setWaypointToMove(NextWP);
			PositionChange=rand()%8+17;	// added 4 sec fit time + time needed to move to next pos.
		}

		else
		{
			uint32 val = RandomUInt(100);

			if (val > 0 && val < 5)	// Flame Quills wp here!
			{
				_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
				_unit->GetAIInterface()->setWaypointToMove(10);
			}
		}
    }

	void PhaseTwo()
	{

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

    void OnReachWP(uint32 iWaypointId, bool bForwards)
    {
		if (Phase == 1)
		{
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
			_unit->GetAIInterface()->setWaypointToMove(6);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Phase 1 Test!");
			_unit->PlaySoundToSet(11243);
		}

		switch(iWaypointId)
		{
			case 5:
			    {
					_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);	// reseting movetype and adding it once again to let it move from 1-5 again
					_unit->GetAIInterface()->setWaypointToMove(0);
			        _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_FORWARDTHANSTOP);
			    }break;

			case 6:
			    {
					//_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
					//_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
					_unit->GetAIInterface()->m_canMove = false;
					switch (FlyWay)
					{
					case 0:
						{
							NextWP = 7;
						}break;

					case 1:
						{	
							NextWP = 9;
						}break;
					}
				}break;
				
			case 7:
			    {
					//_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
					//_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
					_unit->GetAIInterface()->m_canMove = false;
					switch (FlyWay)
					{
					case 0:
						{
							NextWP = 8;
						}break;

					case 1:
						{	
							NextWP = 6;
						}break;
					}
			    }break;
		
			case 8:
		    {
				_unit->GetAIInterface()->m_canMove = false;
				//_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
                //_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
				switch (FlyWay)
				{
				case 0:
					{
						NextWP = 9;
					}break;
	
				case 1:
					{	
						NextWP = 7;
					}break;
				}
		    }break;

			case 9:
		    {
				_unit->GetAIInterface()->m_canMove = false;
				//_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
                //_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
				switch (FlyWay)
				{
				case 0:
					{
						NextWP = 6;
					}break;

				case 1:
					{	
						NextWP = 8;
					}break;
				}
			}break;

			case 10:
		    {
				//_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
                //_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
				if (Phase == 1)
				{
					FlameQuills = true;
					QuillsCount = 0;
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
	int PositionChange;
	int PhoenixSummon;
	uint32 NextWP;
    uint32 m_entry;
    uint32 FlyWay;
	uint32 Phase;
	int nrspells;
};


void SetupTheEye(ScriptMgr * mgr)
{
	mgr->register_creature_script(CN_ALAR, &AlarAI::Create);
}
