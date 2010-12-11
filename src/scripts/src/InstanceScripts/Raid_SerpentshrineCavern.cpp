/*
 * ArcScripts for ArcEmu MMORPG Server
 * Copyright (C) 2009 ArcEmu Team <http://www.arcemu.org/>
 * Copyright (C) 2008-2009 Sun++ Team <http://www.sunscripting.com/>
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

#include "Setup.h"

/************************************************************************/
/* Raid_SerpentshrineCavern.cpp Script									*/
/************************************************************************/

#ifdef WIN32
#pragma warning(disable:4146)      //warning C4146: unary minus operator applied to unsigned type, result still unsigned
#endif

#define SP_SCALDING_WATER 37284 //This "dot" is supposed to tick whenever you get in the water, it doesnt tick anymore after u kill lurker.

//------------------------------------
//	-= Hydross the Unstable =-
//------------------------------------
#define CN_HYDROSS_THE_UNSTABLE 21216
#define CN_TAINTED_SPAWN_OF_HYDROSS 22036
#define CN_PURE_SPAWN_OF_HYDROSS 22035

#define WATER_TOMB 38235
#define VILE_SLUDGE 38246
#define MARK_OF_HYDROSS1    38215
#define MARK_OF_HYDROSS2    38216
#define MARK_OF_HYDROSS3    38217
#define MARK_OF_HYDROSS4    38218
#define MARK_OF_HYDROSS5    38231
#define MARK_OF_HYDROSS6    40584

#define MARK_OF_CORRUPTION1 38219
#define MARK_OF_CORRUPTION2 38220
#define MARK_OF_CORRUPTION3 38221
#define MARK_OF_CORRUPTION4 38222
#define MARK_OF_CORRUPTION5 38230
#define MARK_OF_CORRUPTION6 40583

#define HYDROSS_ENRAGE 13048
#define PURIFY_ELEMENTAL 36461

class HydrossTheUnstableAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(HydrossTheUnstableAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

	HydrossTheUnstableAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 2;

		spells[0].info = dbcSpell.LookupEntry(WATER_TOMB); 
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = true;
		spells[0].cooldown = 7;
		spells[0].attackstoptimer = 1000;
		m_spellcheck[0] = false;

		spells[1].info = dbcSpell.LookupEntry(VILE_SLUDGE); 
		spells[1].targettype = TARGET_RANDOM_SINGLE;
		spells[1].instant = true;
		spells[1].cooldown = 15;
		spells[1].attackstoptimer = 1000;
		m_spellcheck[1] = false;

		//frost attack type	
		_unit->GetProto()->AttackType = 4;
		//frost immunity
		_unit->SchoolImmunityList[SCHOOL_FROST] = 1;
	}

	void ResetCastTime()
	{
		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void OnCombatStart(Unit* mTarget)
	{
		ResetCastTime();
		MarkCount = 0;
		form = false;
		MarkTimer = 10;
		minspell = 0;
		maxspell = 0;
		Enraged = false;
		EnrageTimer = 600;

		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I cannot allow you to interfere!");
		_unit->PlaySoundToSet(11289);

		RegisterAIUpdateEvent(1000);
	}

	void OnCombatStop(Unit* mTarget)
	{
		_unit->SetDisplayId(20162);
		_unit->SchoolImmunityList[SCHOOL_FROST] = 1;
		_unit->SchoolImmunityList[SCHOOL_NATURE] = 0;

		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnTargetDied(Unit* mTarget)
	{
		if(_unit->GetHealthPct() > 0)	
		{
			if(!form)
			{
				switch(rand()%2)
				{
				case 0:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "They have forced me to this...");
					_unit->PlaySoundToSet(11291);
					break;

				case 1:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I have no choice.");
					_unit->PlaySoundToSet(11292);
					break;
				}
			}
			else
			{
				switch(rand()%2)
				{
				case 0:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL,  "I will purge you from this place.");
					_unit->PlaySoundToSet(11298);
					break;

				case 1:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You are no better than they!");
					_unit->PlaySoundToSet(11299);
					break;
				}
			}
		}
	}

	void OnDied(Unit* mKiller)
	{
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You are the disease, not I.");
		_unit->PlaySoundToSet(11300);

		RemoveAIUpdateEvent();
	}

	void AIUpdate()
	{
		if(!form) //water form
		{
			//Mark of Hydross
			if(MarkCount < 6)
			{
				MarkTimer--;
				if(!MarkTimer)
				{
//					uint32 spellid = 0; UNUSED???
					switch(MarkCount)
					{
					case 0:
						_unit->CastSpell(_unit, dbcSpell.LookupEntry(MARK_OF_HYDROSS1), true);
						break;

					case 1:
						_unit->CastSpell(_unit, dbcSpell.LookupEntry(MARK_OF_HYDROSS2), true);
						break;

					case 2:
						_unit->CastSpell(_unit, dbcSpell.LookupEntry(MARK_OF_HYDROSS3), true);
						break;

					case 3:
						_unit->CastSpell(_unit, dbcSpell.LookupEntry(MARK_OF_HYDROSS4), true);
						break;

					case 4:
						_unit->CastSpell(_unit, dbcSpell.LookupEntry(MARK_OF_HYDROSS5), true);
						break;

					case 5:
						_unit->CastSpell(_unit, dbcSpell.LookupEntry(MARK_OF_HYDROSS6), true);
						break;
					}

					MarkCount++;
					MarkTimer = 20;
				}
			}

			//Switch check
			float distx = _unit->GetSpawnX() - _unit->GetPositionX();
			float disty = _unit->GetSpawnY() - _unit->GetPositionY();
			float dist = sqrt( (distx*distx) + (disty*disty) );
			if(dist > 25)
			{
				//switch to poison form
				MarkTimer = 10;
				MarkCount = 0;
				minspell = 1;
				maxspell = 1;
				form = true;
				_unit->SetDisplayId(5498);
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Aaghh, the poison...");
				_unit->PlaySoundToSet(11297);
				_unit->GetProto()->AttackType = 3;
				_unit->SchoolImmunityList[SCHOOL_FROST] = 0;
				_unit->SchoolImmunityList[SCHOOL_NATURE] = 1;

				//Summon 4 elementals
				Creature*  summon;
				float posx = _unit->GetPositionX();
				float posy = _unit->GetPositionY();
				float posz = _unit->GetPositionZ();
				float orientation = _unit->GetOrientation();
				summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_TAINTED_SPAWN_OF_HYDROSS, 
					posx+6.93f, posy-11.25f, posz, orientation,
					true, false, 0, 0);
				if(summon) summon->SchoolImmunityList[SCHOOL_NATURE] = 1;
				summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_TAINTED_SPAWN_OF_HYDROSS, 
					posx-6.93f, posy+11.25f, posz, orientation,
					true, false, 0, 0);
				if(summon) summon->SchoolImmunityList[SCHOOL_NATURE] = 1;
				summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_TAINTED_SPAWN_OF_HYDROSS, 
					posx-12.57f, posy-4.72f, posz, orientation,
					true, false, 0, 0);
				if(summon) summon->SchoolImmunityList[SCHOOL_NATURE] = 1;
				summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_TAINTED_SPAWN_OF_HYDROSS, 
					posx+12.57f, posy+4.72f, posz, orientation,
					true, false, 0, 0);
				if(summon) summon->SchoolImmunityList[SCHOOL_NATURE] = 1;
			}
		}
		else //poison form
		{
			//Mark of Corruption
			if(MarkCount < 6)
			{
				MarkTimer--;
				if(!MarkTimer)
				{
//					uint32 spellid = 0; UNUSED???
					switch(MarkCount)
					{
					case 0:
						_unit->CastSpell(_unit, dbcSpell.LookupEntry(MARK_OF_CORRUPTION1), true);
						break;

					case 1:
						_unit->CastSpell(_unit, dbcSpell.LookupEntry(MARK_OF_CORRUPTION2), true);
						break;

					case 2:
						_unit->CastSpell(_unit, dbcSpell.LookupEntry(MARK_OF_CORRUPTION3), true);
						break;

					case 3:
						_unit->CastSpell(_unit, dbcSpell.LookupEntry(MARK_OF_CORRUPTION4), true);
						break;

					case 4:
						_unit->CastSpell(_unit, dbcSpell.LookupEntry(MARK_OF_CORRUPTION5), true);
						break;

					case 5:
						_unit->CastSpell(_unit, dbcSpell.LookupEntry(MARK_OF_CORRUPTION6), true);
						break;
					}

					MarkCount++;
					MarkTimer = 20;
				}
			}

			//Switch check
			float distx = _unit->GetSpawnX() - _unit->GetPositionX();
			float disty = _unit->GetSpawnY() - _unit->GetPositionY();
			float dist = sqrt( (distx*distx) + (disty*disty) );
			if(dist < 25)
			{
				//switch to water form
				MarkTimer = 10;
				MarkCount = 0;
				minspell = 0;
				maxspell = 0;
				form = false;
				_unit->SetDisplayId(20162);
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Better, much better.");
				_unit->PlaySoundToSet(11290);
				_unit->GetProto()->AttackType = 4;
				_unit->SchoolImmunityList[SCHOOL_FROST] = 1;
				_unit->SchoolImmunityList[SCHOOL_NATURE] = 0;

				//Summon 4 elementals
				Creature*  summon;
				float posx = _unit->GetPositionX();
				float posy = _unit->GetPositionY();
				float posz = _unit->GetPositionZ();
				float orientation = _unit->GetOrientation();
				summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_PURE_SPAWN_OF_HYDROSS, 
					posx+6.93f, posy-11.25f, posz, orientation,
					true, false, 0, 0);
				if(summon) summon->SchoolImmunityList[SCHOOL_FROST] = 1;
				summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_PURE_SPAWN_OF_HYDROSS, 
					posx-6.93f, posy+11.25f, posz, orientation,
					true, false, 0, 0);
				if(summon) summon->SchoolImmunityList[SCHOOL_FROST] = 1;
				summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_PURE_SPAWN_OF_HYDROSS, 
					posx-12.57f, posy-4.72f, posz, orientation,
					true, false, 0, 0);
				if(summon) summon->SchoolImmunityList[SCHOOL_FROST] = 1;
				summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_PURE_SPAWN_OF_HYDROSS, 
					posx+12.57f, posy+4.72f, posz, orientation,
					true, false, 0, 0);
				if(summon) summon->SchoolImmunityList[SCHOOL_FROST] = 1;
			}
		}

		//Enrage
		if(!Enraged)
		{
			EnrageTimer--;
			if(!EnrageTimer)
			{
				_unit->CastSpell(_unit, dbcSpell.LookupEntry(HYDROSS_ENRAGE), true);
				Enraged = true;
			}
		}

		float val = RandomFloat(100.0f);
		SpellCast(val);
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
		{
			float comulativeperc = 0;
			Unit* target = NULL;
			for(int i = minspell; i <= maxspell; i++)
			{		
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->getNextTarget();
					switch(spells[i].targettype)
					{
					case TARGET_SELF:
					case TARGET_VARIOUS:
						_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
					case TARGET_ATTACKING:
						_unit->CastSpell(target, spells[i].info, spells[i].instant);
						break;
					case TARGET_DESTINATION:
						_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					case TARGET_RANDOM_SINGLE:
					case TARGET_RANDOM_DESTINATION:
						CastSpellOnRandomTarget(i, 0, 0); break;
					}
					spells[i].casttime = spells[i].cooldown;
					m_spellcheck[i] = false;
					return;
				}

				if(spells[i].casttime > 0)
					spells[i].casttime--;
					
				if(!spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;

		Unit* RandomTarget = NULL;
		std::vector<Unit*> TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
		for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
		{ 
			if (isHostile(_unit, (*itr)) && (*itr)->IsUnit())
			{
				RandomTarget = TO_UNIT(*itr);

				if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast)
					TargetTable.push_back(RandomTarget);
			} 
		}

		if (!TargetTable.size())
			return;

		Unit*  RTarget = *(TargetTable.begin()+rand()%TargetTable.size());

		if (!RTarget)
			return;

		switch (spells[i].targettype)
		{
		case TARGET_RANDOM_SINGLE:
			_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
		case TARGET_RANDOM_DESTINATION:
			_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
		}

		TargetTable.clear();
	}

private:
	int nrspells;
	int minspell;
	int maxspell;
	bool form; //false = water | true = poison
	SpellDesc *spell_water_tomb;
	uint32 MarkTimer;
	uint32 MarkCount;
	bool Enraged;
	uint32 EnrageTimer;
};

//------------------------------------
//	-= The Lurker Below=-
//------------------------------------
#define CN_THE_LURKER_BELOW 21217
#define CN_CN_COILFANG_AMBUSHER 21865
#define CN_CN_COILFANG_GUARDIAN 21873

#define WHIRL 37363
#define GEYSER 37478
#define SPOUT 37433
#define SUBMERGE 37433 // Didn't find the spell id
//#define WATER_BOLT 37138  

class LurkerAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(LurkerAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

	LurkerAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		// -- Number of spells to add --
		nrspells = 4;

		// --- Initialization ---
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		// ----------------------
		
		// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not! 

		spells[0].info = dbcSpell.LookupEntry(WHIRL);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = 30;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000; // 1sec
		m_spellcheck[0] = true;

		spells[1].info = dbcSpell.LookupEntry(GEYSER);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].cooldown = 10;
		spells[1].perctrigger = 10.0f;
		spells[1].attackstoptimer = 2000; // 2sec

		spells[2].info = dbcSpell.LookupEntry(SPOUT);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = false;
		spells[2].cooldown = 60;
		spells[2].perctrigger = 5.0f;
		spells[2].attackstoptimer = 2000; // 2sec

		spells[3].info = dbcSpell.LookupEntry(SUBMERGE);
		spells[3].targettype = TARGET_SELF;
		spells[3].instant = true;
		spells[3].cooldown = 120;
		spells[3].perctrigger = 10.0f;
		spells[3].attackstoptimer = 2000; // 2sec
	}

	void OnCombatStart(Unit* mTarget)
	{
		RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
	}

	void OnCombatStop(Unit* mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
	   RemoveAIUpdateEvent();
	}

	void OnDied(Unit* mKiller)
	{
		RemoveAIUpdateEvent();
	}

	void OnTargetDied(Unit* mTarget)
	{

	}

	void AIUpdate()
	{
		float val = RandomFloat(100.0f);
		SpellCast(val);
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
		{
			float comulativeperc = 0;
			Unit* target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->getNextTarget();
					switch(spells[i].targettype)
					{
					case TARGET_SELF:
					case TARGET_VARIOUS:
						_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
					case TARGET_ATTACKING:
						_unit->CastSpell(target, spells[i].info, spells[i].instant);
						break;
					case TARGET_DESTINATION:
						_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
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
};

//------------------------------------
//	-= Leotheras the Blind =-
//------------------------------------

/* ToDo:
 - Some phase timers
 - Insidious whispers
*/

#define CN_LEOTHERAS_THE_BLIND 21215
#define CN_INNER_DEMON 21857 
#define CN_GREYHEART_SPELLBINDER 21806
#define CN_SHADOW_OF_LEOTHERAS 21875

#define MIND_BLAST 37531
#define WHIRLWINDLEO 40653
#define INSIDIOUS_WHISPER 37676
#define CHAOS_BLAST_ANIMATION 37674
#define CHAOS_BLAST_EFFECT 37675
#define LEO_ENRAGE 41447 //13048
#define LEO_BANISH 37546

uint32 LeotherasEventGreyheartToKill[1000000];

class LeotherasAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(LeotherasAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

	LeotherasAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		//Insidious Whisper (inner demons) 
		//"We all have our demons..." 
		////Debuff that summons an Inner Demon from up to five raid members. Each Inner Demon can be attacked only by the person it spawned from. If you do not kill your Inner Demon before Leotheras gets back into humanoid form you will become Mind Controlled for 10 minutes and can't get out of it unless killed. Inner Demons take increased damage from arcane, nature, and holy spells.
		spells[0].info = dbcSpell.LookupEntry(INSIDIOUS_WHISPER);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].perctrigger = 2.0f;
		spells[0].attackstoptimer = 2000;
		m_spellcheck[0] = false;

		info_chaos_blast = dbcSpell.LookupEntry(CHAOS_BLAST_ANIMATION);
		info_whirlwind = dbcSpell.LookupEntry(WHIRLWINDLEO);

		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);

		LeotherasEventGreyheartToKill[_unit->GetInstanceID()] = 0;
		FirstCheck();
	}

	void FirstCheck()
	{
		//count greyheart spellbinders
		Creature* creature = NULL;
		for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
		{
			if((*itr)->IsCreature())
			{
				creature = TO_CREATURE((*itr));

				if(creature->GetCreatureInfo()->Id == CN_GREYHEART_SPELLBINDER && creature->isAlive())
					LeotherasEventGreyheartToKill[_unit->GetInstanceID()]++;
			}
		}

		//no greyheart spellbinder found, release him
		if(!LeotherasEventGreyheartToKill[_unit->GetInstanceID()])
		{
			//remove banish & blocks
			_unit->RemoveAllAuras();
			_unit->SetUInt32Value(UNIT_FIELD_FLAGS, 0);
			_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
			_unit->GetAIInterface()->m_canMove = true;
			_unit->SetStandState(STANDSTATE_STAND);
		}
	}

	void OnCombatStart(Unit* mTarget)
	{
		if(LeotherasEventGreyheartToKill[_unit->GetInstanceID()] != 0)
			return;

		Phase = 0; //nightelf form
		SwitchTimer = 40+rand()%5; //wowwiki says 45, bosskillers says 40
		WhirlwindTimer = 15;
		mInWhirlwind = false;
		FinalPhaseSubphase = 0;
		FinalPhaseTimer = 0;
		IsMorphing = false;
		Enraged = false;
		EnrageTimer = 599; //10 minutes

		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Finally my banishment ends!");
		_unit->PlaySoundToSet(11312);

		RegisterAIUpdateEvent(1000);
	}

	void OnCombatStop(Unit* mTarget)
	{
		//despawn shadow of leotheras
		Creature* shadow = NULL;
		shadow = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), CN_SHADOW_OF_LEOTHERAS);
		if(shadow)
		{
			shadow->Despawn(0, 0);
		}

		SwitchToHumanForm();
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
	  	_unit->GetAIInterface()->SetAIState(STATE_IDLE);
	 	RemoveAIUpdateEvent();
	}

	void OnTargetDied(Unit* mTarget)
	{
		if (_unit->GetHealthPct() > 0)
		{
			if(Phase) //blood elf form
			{
				switch (rand()%3)
				{
				case 0:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Kill! KILL!");
					_unit->PlaySoundToSet(11314);
					break;

				case 1:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "That's right! Yes!");
					_unit->PlaySoundToSet(11315);
					break;

				case 2:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Who's the master now?");
					_unit->PlaySoundToSet(11316);
					break;
				}
			}
			else //demon form
			{
				switch (rand()%3)
				{
				case 0:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I have no equal.");
					_unit->PlaySoundToSet(11306);
					break;
				case 1:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Perish, mortal.");
					_unit->PlaySoundToSet(11307);
					break;
				case 2:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Yes, YES! Ahahah!");
					_unit->PlaySoundToSet(11308);
					break;
				}
			}
		}
	}

	void OnDied(Unit* mKiller)
	{
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You cannot kill me! Fools, I'll be back! I'll... aarghh...");
		_unit->PlaySoundToSet(11317);
		RemoveAIUpdateEvent();
	}

	void SwitchToHumanForm()
	{
		_unit->SetDisplayId(20514);
		_unit->SetEquippedItem(MELEE,(_unit->m_spawn != NULL)? _unit->m_spawn->Item1SlotDisplay : 0);
		_unit->SetEquippedItem(OFFHAND,(_unit->m_spawn != NULL)?  _unit->m_spawn->Item2SlotDisplay : 0);
	}

	void SwitchToDemonForm()
	{
		_unit->SetDisplayId(20125);
		_unit->SetEquippedItem(MELEE,0);
		_unit->SetEquippedItem(OFFHAND,0);
	}

	void AIUpdate()
	{
		if(Phase == 0 || Phase == 3) //nightelf phase
		{
			if(!IsMorphing)
			{
				//whirlwind
				WhirlwindTimer--;
				if(!WhirlwindTimer)
				{
					if(!mInWhirlwind)
					{
						_unit->CastSpell(_unit, info_whirlwind, true);
						_unit->setAttackTimer(15000, false);
						_unit->GetAIInterface()->ClearHateList(); //reset aggro
						WhirlwindTimer = 15;
						mInWhirlwind = true;
					}
					else
					{
						_unit->GetAIInterface()->ClearHateList(); //reset aggro
						WhirlwindTimer = 15;
						mInWhirlwind = false;
					}
				}

				float val = RandomFloat(100.0f);
				SpellCast(val);
			}

			if(Phase == 0)
			{
				//switch to demon form
				SwitchTimer--;
				if(!SwitchTimer)
				{
					//switch to AGENT_SPELL
					_unit->GetAIInterface()->setCurrentAgent(AGENT_SPELL);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Be gone trifling elf. I'm in control now");
					_unit->PlaySoundToSet(11304);
					SwitchToDemonForm();
					Phase = 1;
					SwitchTimer = 60; //60 seconds
				}

				//15% Leotheras/Demon split
				//wait until he returns nightelf (blizzlike)
				if(_unit->GetHealthPct() <= 15 && !mInWhirlwind)
					Phase = 3;
			}
			else
			{
				Creature* shadow = NULL;
				switch(FinalPhaseSubphase)
				{
				case 0:
					IsMorphing = true;
					_unit->setAttackTimer(15000, false);
					_unit->SetStandState(STANDSTATE_KNEEL);
					_unit->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
					_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
					_unit->GetAIInterface()->m_canMove = false;
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "No... no! What have you done? I am the master! Do you hear me? I am... aaggh! Can't... contain him.");
					_unit->PlaySoundToSet(11313);
					FinalPhaseTimer = 10;
					FinalPhaseSubphase++;
					break;

				case 1:
					FinalPhaseTimer--;
					if(!FinalPhaseTimer)
					{
						_unit->SetStandState(STANDSTATE_STAND);
						shadow = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(),CN_SHADOW_OF_LEOTHERAS);
						
						if (shadow == NULL)
							shadow = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_SHADOW_OF_LEOTHERAS, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), _unit->GetOrientation(), true, false, 0, 0);
						
						FinalPhaseTimer = 5;
						FinalPhaseSubphase++;
					}
					break;

				case 2:
					FinalPhaseTimer--;
					if(!FinalPhaseTimer)
					{
						_unit->SetUInt32Value(UNIT_FIELD_FLAGS, 0);
						_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
						_unit->GetAIInterface()->m_canMove = true;
						IsMorphing = false;
						FinalPhaseSubphase++;
					}
					break;
						
				default:
					break;
				}
			}

			//Enrage
			if(!Enraged)
			{
				EnrageTimer--;
				if(!EnrageTimer)
				{
					_unit->CastSpell(_unit, dbcSpell.LookupEntry(LEO_ENRAGE), true);
					Enraged = true;
				}
			}
		}
		else if(Phase == 1) //demon form
		{
			//Chaos Blast
			if(_unit->GetAIInterface()->getNextTarget())
			{
				if(_unit->GetCurrentSpell() == NULL)
				{
					if(rand()%2)
					{
						_unit->CastSpell(_unit->GetAIInterface()->getNextTarget(), info_chaos_blast, false);
					}
				}

				//move if needed
				if(_unit->GetAIInterface()->getNextTarget()->GetDistance2dSq(_unit) >= 400) //20 yards
				{
					_unit->GetAIInterface()->m_moveRun = true;
					_unit->GetAIInterface()->_CalcDestinationAndMove(_unit->GetAIInterface()->getNextTarget(), 5.0f);
				}
			}

			//switch
			SwitchTimer--;
			if(!SwitchTimer)
			{
				_unit->GetAIInterface()->setCurrentAgent(AGENT_MELEE);
				SwitchToHumanForm();
				Phase = 0;
				WhirlwindTimer = 10+rand()%5;
				SwitchTimer = 40+rand()%5; //wowwiki says 45, bosskillers says 40
				_unit->GetAIInterface()->ClearHateList(); //reset aggro
			}
		}
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
		{
			float comulativeperc = 0;
			Unit* target = NULL;
			for(int i = 0; i < 1; i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->getNextTarget();
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

				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
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
	uint32 SwitchTimer;
	uint32 WhirlwindTimer;
	uint32 EnrageTimer;
	bool Enraged;
	bool mInWhirlwind;
	bool IsMorphing;
	uint32 Phase;
	SpellEntry *info_whirlwind;
	SpellEntry *info_chaos_blast;
	uint32 FinalPhaseSubphase;
	uint32 FinalPhaseTimer;
};

class GreyheartSpellbinderAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(GreyheartSpellbinderAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

	GreyheartSpellbinderAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 1;

		spells[0].info = dbcSpell.LookupEntry(MIND_BLAST);
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = true;
		spells[0].perctrigger = 50.0f;
		spells[0].attackstoptimer = 2000;
		m_spellcheck[0] = false;

		Unit* Leotheras = NULL;
		Leotheras = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(376.543f, -438.631f, 29.7083f, CN_LEOTHERAS_THE_BLIND);
		if(Leotheras)
		{
			_unit->SetChannelSpellTargetGUID(Leotheras->GetGUID());
			_unit->SetChannelSpellId(30166);//wrong
		}
	}

	void OnCombatStart(Unit* mTarget)
	{			
		_unit->SetChannelSpellTargetGUID(0);
		_unit->SetChannelSpellId(0);

		RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
	}

    void OnCombatStop(Unit* mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);

        RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		float val = RandomFloat(100.0f);
        SpellCast(val);
    }

	void OnDied(Unit* mKiller)
	{
		LeotherasEventGreyheartToKill[_unit->GetInstanceID()]--;

		//start the event
		if(LeotherasEventGreyheartToKill[_unit->GetInstanceID()] == 0)
		{
			Unit* Leotheras = NULL;
			Leotheras = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(376.543f, -438.631f, 29.7083f, CN_LEOTHERAS_THE_BLIND);
			if(Leotheras)
			{
				//remove banish & blocks
				Leotheras->RemoveAllAuras();
				Leotheras->SetUInt32Value(UNIT_FIELD_FLAGS, 0);
				Leotheras->GetAIInterface()->SetAllowedToEnterCombat(true);
				Leotheras->GetAIInterface()->m_canMove = true;
				Leotheras->SetStandState(STANDSTATE_STAND);

				//attack nearest player
				Player *NearestPlayer = NULL;
				float NearestDist = 0;
				for(set< Object* >::iterator itr = _unit->GetInRangePlayerSetBegin(); itr != _unit->GetInRangePlayerSetEnd(); ++itr) 
				{
					if(isHostile(_unit, (*itr)) && ((*itr)->GetDistance2dSq(_unit) < NearestDist || !NearestDist))
					{
						NearestDist = (*itr)->GetDistance2dSq(_unit);
						NearestPlayer = TO< Player* >(*itr);
					}
				}

				if(NearestPlayer)
					Leotheras->GetAIInterface()->AttackReaction(NearestPlayer, 1, 0);
			}
		}
	}

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
        {
			float comulativeperc = 0;
		    Unit* target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->getNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
						case TARGET_RANDOM_SINGLE:
						case TARGET_RANDOM_DESTINATION:
							CastSpellOnRandomTarget(i, 0, 0); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;

		Unit* RandomTarget = NULL;
		std::vector<Unit*> TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
		for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
		{ 
			if (isHostile(_unit, (*itr)) && (*itr)->IsUnit() && isAttackable(_unit, (*itr)))
			{
				RandomTarget = TO_UNIT(*itr);

				if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast)
					TargetTable.push_back(RandomTarget);
			} 
		}

		if (!TargetTable.size())
			return;

		Unit*  RTarget = *(TargetTable.begin()+rand()%TargetTable.size());

		if (!RTarget)
			return;

		switch (spells[i].targettype)
		{
		case TARGET_RANDOM_SINGLE:
			_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
		case TARGET_RANDOM_DESTINATION:
			_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
		}

		TargetTable.clear();
	}

private:
	int nrspells;
};

class ShadowofLeotherasAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(ShadowofLeotherasAI);

	ShadowofLeotherasAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		info_chaos_blast = dbcSpell.LookupEntry(CHAOS_BLAST_ANIMATION);

		_unit->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);

		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "At last I am liberated. It has been too long since I have tasted true freedom!");
		_unit->PlaySoundToSet(11309);

		sEventMgr.AddEvent(TO_OBJECT(_unit), &Object::EventSetUInt32Value, (uint32)UNIT_FIELD_FLAGS, (uint32)0, EVENT_CREATURE_UPDATE, 7500, 0, 1);
	}

	void OnCombatStart(Unit* mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_SPELL);
		RegisterAIUpdateEvent(1000);
	}

	void OnCombatStop(Unit* mTarget)
	{
		RemoveAIUpdateEvent();
	}

	void AIUpdate()
	{
		//Chaos Blast
		if(_unit->GetAIInterface()->getNextTarget())
		{
			if(_unit->GetCurrentSpell() == NULL)
			{
				if(rand()%2)
				{
					_unit->CastSpell(_unit->GetAIInterface()->getNextTarget(), info_chaos_blast, false);
				}
			}

			//move if needed
			if(_unit->GetAIInterface()->getNextTarget()->GetDistance2dSq(_unit) >= 400) //20 yards
			{
				_unit->GetAIInterface()->m_moveRun = true;
				_unit->GetAIInterface()->_CalcDestinationAndMove(_unit->GetAIInterface()->getNextTarget(), 5.0f);
			}
		}
	}

protected:
	SpellEntry* info_chaos_blast;
};



//------------------------------------
//	-= Fathom-Lord Karathress =-
//------------------------------------
#define CN_FATHOM_LORD_KARATHRESS 21214
#define CN_FATHOM_GUARD_SHARKKIS 21966
#define CN_FATHOM_GUARD_TIDALVESS 21965
#define CN_FATHOM_LURKER 22119
#define CN_FATHOM_SPOREBAT 22120
#define CN_FATHOM_GUARD_CARIBDIS 21964
#define CN_SEER_OLUM 22820

#define CATACLYSMIC_BOLT 38441
#define BLESSING_OF_THE_TIDES 38449
#define KARATHRESS_ENRAGE 39249

class KarathressAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(KarathressAI);

	KarathressAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		info_cataclysmic_bolt = dbcSpell.LookupEntry(CATACLYSMIC_BOLT);
		AdvisorsLeft = 3;
		BlessingOfTidesCounter = 0;

	}
/* we don't need to use that, as long we handle advisors counter with events
	void CheckAdvisors()
	{
		Unit* advisor = NULL;

		//Sharkkis
		advisor = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), CN_FATHOM_GUARD_SHARKKIS);
		if(advisor && advisor->isAlive()) AdvisorsLeft--;

		//Tidalvess
		advisor = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), CN_FATHOM_GUARD_TIDALVESS);
		if(advisor && advisor->isAlive()) AdvisorsLeft--;

		//Caribdis
		advisor = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), CN_FATHOM_GUARD_CARIBDIS);
		if(advisor && advisor->isAlive()) AdvisorsLeft--;
	}*/

	void OnCombatStart(Unit* mTarget)
	{
		CataclysmicBoltTimer = 10;
		EnrageTimer = 600;
		Enraged = false;
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Guards, attention! We have visitors ...");
		_unit->PlaySoundToSet(11277);
		RegisterAIUpdateEvent(1000);
	}

	void OnCombatStop(Unit* mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
	   RemoveAIUpdateEvent();
	}

	void OnDied(Unit* mKiller)
	{
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Her ... excellency ... awaits!");
		_unit->PlaySoundToSet(11285);
		RemoveAIUpdateEvent();

		//spawn seer olum and despawn him in 3 minutes
		Creature* olum = NULL;
		olum = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_SEER_OLUM, 451.099f, -544.984f, -7.36327f, 0.174533f, true, false, 0, 0);
		if(olum)
			olum->Despawn(180000, 0);
	}

	void OnTargetDied(Unit* mTarget)
	{
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I am rid of you.");
		_unit->PlaySoundToSet(11284);
	}

	void AIUpdate()
	{
		//Cataclysmic Bolt
		CataclysmicBoltTimer--;
		if(CataclysmicBoltTimer==0)
		{
		// trying to be blizzlike: cast this bolt random on casters only
		CataclysmicBoltTimer = 10;
		Unit* RandomTarget = NULL;
		std::vector<Unit*> TargetTable;
		for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
		{ 
			if (isHostile(_unit, (*itr)) && (*itr)->IsUnit())
			{
				RandomTarget = TO_UNIT(*itr);

				if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) <= 80.0f && _unit->GetPowerType() == POWER_TYPE_MANA)
					TargetTable.push_back(RandomTarget);
			} 
		}

		if (!TargetTable.size())
			return;

		RandomTarget = *(TargetTable.begin()+rand()%TargetTable.size());

		if (!RandomTarget)
			return;
		//let's force this effect
		info_cataclysmic_bolt->EffectBasePoints[0] = RandomTarget->GetUInt32Value(UNIT_FIELD_MAXHEALTH)/2;
		_unit->CastSpell(RandomTarget, info_cataclysmic_bolt, true);
		TargetTable.clear();
		}

		//Blessing of the Tides
		if(_unit->GetHealthPct() <= 70 && AdvisorsLeft > 0)
		{
			if (BlessingOfTidesCounter < AdvisorsLeft)
			{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Your overconfidence will be your undoing! Guards, lend me your strength!");
			_unit->PlaySoundToSet(11278);
			_unit->CastSpell(_unit, dbcSpell.LookupEntry(BLESSING_OF_THE_TIDES), true);
			BlessingOfTidesCounter++;
			}
		}

		//Enrage
		if(!Enraged)
		{
			EnrageTimer--;
			if(!EnrageTimer)
			{
				_unit->CastSpell(_unit, dbcSpell.LookupEntry(KARATHRESS_ENRAGE), true);
				Enraged = true;
			}
		}
	}

	uint32 AdvisorsLeft;

private:
	SpellEntry *info_cataclysmic_bolt;
	uint32 CataclysmicBoltTimer;
	uint32 EnrageTimer;
	uint32 BlessingOfTidesCounter;
	bool Enraged;
};

//Fathom-Guard Sharkkis (Hunter)
#define MULTI_SHOT 29576
#define LEECHING_THROW 29436
#define THE_BEAST_WITHIN 38373

class FathomGuardSharkissAI : public MoonScriptCreatureAI
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(FathomGuardSharkissAI);
	FathomGuardSharkissAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(MULTI_SHOT, Target_Current, 10.0f, 0, 0);
		AddSpell(LEECHING_THROW, Target_Current, 10.0f, 0, 0);
		AddSpell(THE_BEAST_WITHIN, Target_Current, 10.0f, 0, 40);
	}

   void OnCombatStart(Unit* pTarget)
   {
	   CurrentPet = NULL;
	   SummonPetTimer = 5;

	   MoonScriptCreatureAI::OnCombatStart(pTarget);
   }

	void OnDied(Unit* pKiller)
	{
		Creature* FLK = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), CN_FATHOM_LORD_KARATHRESS);
		if(FLK)
		{
			FLK->CastSpell(FLK, dbcSpell.LookupEntry(38455), true); //Power of Sharkkis
			FLK->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I am more powerful than ever!");
			FLK->PlaySoundToSet(11280);
			if(TO< KarathressAI* >(FLK->GetScript())->AdvisorsLeft > 0)
				TO< KarathressAI* >(FLK->GetScript())->AdvisorsLeft--;
			FLK->RemoveAura (BLESSING_OF_THE_TIDES);
		}

		MoonScriptCreatureAI::OnDied(pKiller);
	}

	void AIUpdate()
	{
		//Summon Pet
		if(!CurrentPet || !CurrentPet->isAlive())
		{
			SummonPetTimer--;
			if(!SummonPetTimer)
			{
				switch(rand()%2)
				{
				case 0:
					CurrentPet = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_FATHOM_LURKER, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 0, true, false, 0, 0);
					break;

				case 1:
					CurrentPet = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_FATHOM_SPOREBAT, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 0, true, false, 0, 0);
					break;
				}

				SummonPetTimer = 5;
			}
		}

		MoonScriptCreatureAI::AIUpdate();
	}

private:
	uint32 SummonPetTimer;
	Creature* CurrentPet;
};

//Fathom-Guard Tidalvess (Shaman)
#define SPITFIRE_TOTEM 38236
#define POISON_CLEANSING_TOTEM 38306
#define EARTHBIND_TOTEM 38304
#define FROST_SHOCK 38234
#define WINDFURY 38229

class FathomGuardTidalvessAI : public MoonScriptCreatureAI
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(FathomGuardTidalvessAI);
	FathomGuardTidalvessAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		//totems
		AddSpell(SPITFIRE_TOTEM, Target_Self, 10.0f, 0, 0);
		AddSpell(POISON_CLEANSING_TOTEM, Target_Self, 10.0f, 0, 0);
		AddSpell(EARTHBIND_TOTEM, Target_Self, 10.0f, 0, 0);

		AddSpell(FROST_SHOCK, Target_Current, 10.0f, 0, 0);
		AddSpell(WINDFURY, Target_Current, 10.0f, 0, 0);
	}

	void OnDied(Unit*  pKiller)
	{
		Creature* FLK = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), CN_FATHOM_LORD_KARATHRESS);
		if(FLK)
		{
			FLK->CastSpell(FLK, dbcSpell.LookupEntry(38452), true); //Power of Tidalvess
			FLK->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Go on, kill them! I'll be the better for it!");
			FLK->PlaySoundToSet(11279);
			if(TO< KarathressAI* >(FLK->GetScript())->AdvisorsLeft > 0)
				TO< KarathressAI* >(FLK->GetScript())->AdvisorsLeft--;
			FLK->RemoveAura (BLESSING_OF_THE_TIDES);
		}
		
		MoonScriptCreatureAI::OnDied(pKiller);
	}
};

//Fathom-Guard Caribdis (Priest)
#define HEAL 33144
#define TIDAL_SURGE 38358
#define SUMMON_CYCLONE 38337

class FathomGuardCaribdisAI : public MoonScriptCreatureAI
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(FathomGuardCaribdisAI);
	FathomGuardCaribdisAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(TIDAL_SURGE, Target_Self, 20.0f, 0, 10);
		AddSpell(SUMMON_CYCLONE, Target_Self, 2.0f, 0, 0);
	}

	void OnCombatStart(Unit* pTarget)
	{
		HealingWaveTimer = 15;

		MoonScriptCreatureAI::OnCombatStart(pTarget);
	}

	void AIUpdate()
	{
		/*HealingWaveTimer--;
		if(!HealingWaveTimer)
		{
			vector<Unit*> TargetTable;
			for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{
				if((*itr)->GetTypeId() == TYPEID_UNIT && isFriendly(_unit, (*itr)))
					TargetTable.push_back(TO_UNIT(*itr));
			}

			HealingWaveTimer = 15;
		}*/

		MoonScriptCreatureAI::AIUpdate();
	}

	void OnDied(Unit*  pKiller)
	{
		Creature* FLK = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), CN_FATHOM_LORD_KARATHRESS);
		if(FLK)
		{
			FLK->CastSpell(FLK, dbcSpell.LookupEntry(38451), true); //Power of Caribdis
			FLK->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "More knowledge, more power!");
			FLK->PlaySoundToSet(11281);
			if(TO< KarathressAI* >(FLK->GetScript())->AdvisorsLeft > 0)
				TO< KarathressAI* >(FLK->GetScript())->AdvisorsLeft--;
			FLK->RemoveAura (BLESSING_OF_THE_TIDES);
		}
		MoonScriptCreatureAI::OnDied(pKiller);
	}

private:
	uint32 HealingWaveTimer;
};

//------------------------------------
//	-= Morogrim Tidewalker =-
//------------------------------------
#define CN_MOROGRIM_TIDEWALKER 21213
#define CN_TIDEWALKER_LURKER 21920 //Murlocks that he spawns after earthquake

#define TIDAL_WAVE 37730
#define WATERY_GRAVE 38049
#define EARTHQUAKE 37764
#define SUMMON_WATER_GLOBULE 37854

class MorogrimAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(MorogrimAI);
	SP_AI_Spell spells[4];
	
	MorogrimAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		spells[0].info = dbcSpell.LookupEntry(TIDAL_WAVE);
		spells[0].instant = true;

		spells[1].info = dbcSpell.LookupEntry(EARTHQUAKE);
		spells[1].instant = true;
		spells[1].cooldown = 40;

		spells[2].info = dbcSpell.LookupEntry(WATERY_GRAVE);
		spells[2].instant = true;
		spells[2].cooldown = 30;

		spells[3].info = dbcSpell.LookupEntry(SUMMON_WATER_GLOBULE);
		spells[3].instant = true;
		spells[3].cooldown = 30;
	}

	void OnCombatStart(Unit* mTarget)
	{
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Flood of the deep, take you!");
		_unit->PlaySoundToSet(11321);

		RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));

		uint32 t = (uint32)time(NULL);
		for (int i = 1; i < 4; i++)
			spells[i].casttime = t + spells[i].cooldown;
	}

	void OnCombatStop(Unit* mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);

		RemoveAIUpdateEvent();
	}

	void OnDied(Unit* mKiller)
	{
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Great... currents of... Ageon.");
		_unit->PlaySoundToSet(11329);

		RemoveAIUpdateEvent();
	}

	void OnTargetDied(Unit* mTarget)
	{
		if (_unit->GetHealthPct() > 0)	
		{
			int RandomSpeach = rand()%3;
			switch (RandomSpeach)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Only the strong survive.");
				_unit->PlaySoundToSet(11328);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Struggling only makes it worse.");
				_unit->PlaySoundToSet(11327);
				break;
			case 2:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "It is done!");
				_unit->PlaySoundToSet(11326);
				break;
			}
		}
	}

	void AIUpdate()
	{
		if (_unit->GetAIInterface()->getNextTarget() != NULL && _unit->GetCurrentSpell() == NULL)
		{
			Unit* target = NULL;
			uint32 t = (uint32)time(NULL);
			target = _unit->GetAIInterface()->getNextTarget();
			if (t > spells[2].casttime)
			{
				_unit->SendChatMessageAlternateEntry(CN_MOROGRIM_TIDEWALKER, CHAT_MSG_MONSTER_EMOTE, LANG_UNIVERSAL, " sends his enemies to their watery graves!");
				_unit->CastSpell(target, spells[2].info, spells[2].instant);

				spells[2].casttime = t + spells[2].cooldown;
				return;
			}
			if (t > spells[1].casttime)
			{
				_unit->SendChatMessageAlternateEntry(17165, CHAT_MSG_MONSTER_EMOTE, LANG_UNIVERSAL, "The violent earthquake has alerted nearby Murlocs!");

				int RandomSpeach = rand()%2;
				switch (RandomSpeach)
				{
					case 0:
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Destroy them my subjects!");
						_unit->PlaySoundToSet(11323);
						break;
					case 1:
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "By the Tides, kill them at once!");
						_unit->PlaySoundToSet(11322);
						break;
				}
				_unit->CastSpell(_unit, spells[1].info, spells[1].instant);

				for (int i = 0; i < 6; i++)
					_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_TIDEWALKER_LURKER, 370.82f, -723.93f, -13.9f, 0, true, false, 0, 0);

				for (int i = 0; i < 6; i++)
					_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_TIDEWALKER_LURKER, 527.90f, -721.88f, -7.14f, 0, true, false, 0, 0);

				spells[1].casttime = t + spells[1].cooldown;
				return;
			}
			if (_unit->GetHealthPct() < 25)
			{
				if (t > spells[3].casttime)
				{
					_unit->SendChatMessageAlternateEntry(CN_MOROGRIM_TIDEWALKER, CHAT_MSG_MONSTER_EMOTE, LANG_UNIVERSAL, " summons Watery Globules!");

					int RandomSpeach = rand()%2;
					switch (RandomSpeach)
					{
						case 0:
							_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Soon it will be finished.");
							_unit->PlaySoundToSet(11325);
							break;
						case 1:
							_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "There is nowhere to hide!");
							_unit->PlaySoundToSet(11324);
							break;
					}
					_unit->CastSpell(target, spells[3].info, spells[3].instant);
					
					spells[3].casttime = t + spells[3].cooldown;
					return;
				}
			}
			else if (_unit->GetHealthPct() >= 25)
			{
				spells[3].casttime = t + spells[3].cooldown;
			}
			
			float random = RandomFloat(100.0f);
			if (random < 10.0f)
			{
				_unit->CastSpell(target, spells[0].info, spells[0].instant);
			}
		}
	}

};

//CN_TIDEWALKER_LURKER
class TidewalkerLurkerAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(TidewalkerLurkerAI);

	TidewalkerLurkerAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		Unit* target = FindTargetForSpell();
		if (target)
		{
			_unit->GetAIInterface()->AttackReaction(target, 1, 0);
		}
	}

	void OnCombatStop(Unit* mTarget)
	{
		_unit->Despawn(1,0);
	}

	void OnDied(Unit* mKiller)
	{
		_unit->Despawn(1,0);
	}
	
	Unit* FindTargetForSpell()
	{
		Unit* target = NULL;
		float distance = 150.0f;

		Unit* pUnit;
		float dist;

		for (set<Object*>::iterator itr = _unit->GetInRangeOppFactsSetBegin(); itr != _unit->GetInRangeOppFactsSetEnd(); itr++)
		{
			if(!(*itr)->IsUnit())
				continue;

			pUnit = TO_UNIT((*itr));

			if(pUnit->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_FEIGN_DEATH))
				continue;

//			if(pUnit->m_auracount[SPELL_AURA_MOD_INVISIBILITY])
//				continue;
			
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

//------------------------------------
//	-= Lady Vashj =-
//------------------------------------

/* ToDo:
 - Toxic Sporebats
 - Coilfang Strider spawn points
 - Some vashj spells and cooldowns are wrong
 - Right Shield generators coords
*/

#define CN_LADY_VASHJ 21212
#define CN_ENCHANTED_ELEMENTAL 21958
#define CN_COILFANG_STRIDER 22056
#define CN_TAINTED_ELEMENTAL 22009
#define CN_COILFANG_ELITE 22055
#define CN_TOXIC_SPORE_BAT 22140
#define CN_SHIELD_GENERATOR_CHANNEL 19870

#define SHOOT 37770
#define ENTANGLE 38316
#define SPAWN_ELEMENTAL 37053 
#define MULTI_SHOT 29576
#define STATIC_CHARGE 38280
#define FORKED_LIGHTNING 40088
#define VASHJ_SHIELD 38112
#define POISON_SPIT 40078
#define TOXIC_SPORES 38575
#define SHOCK_BLAST 38509

#define WALK 0
#define RUN 256
#define FLY 768

static LocationExtra ElementalSpawnPoints[] = 
{
    {8.3f, -835.3f, 21.9f, 5},
    {53.4f, -835.3f, 21.9f, 4.5f},
    {96.0f, -861.9f, 21.8f, 4},
    {96.0f, -986.4f, 21.4f, 2.5f},
    {54.4f, -1010.6f, 22.0f, 1.8f},
    {9.8f, -1012.0f, 21.7f, 1.4f},
    {-35.0f, -987.6f, 21.5f, 0.8f},
    {-58.9f, -901.6f, 21.5f, 6.0f}
};

static LocationExtra ElementalSpawnPoints2[] =
{
	{16.305f, -867.82f, 41.09f, 0},
	{43.853f, -868.338f, 41.097f, 0},
	{71.55f, -885.12f, 40.87f, 0},
	{70.96f, -962.56f, 41.09f, 0},
	{45.227f, -977.987f, 41.09f, 0},
	{17.35f, -979.27f, 41.01f, 0},
	{-9.89f, -963.63f, 41.09f, 0},
	{-25.37f, -910.266f, 41.09f, 0}
};

static LocationExtra CoilfangEliteSpawnPoints[] =
{
	{14.837f, -949.106f, 41.53f, 0},
	{14.857f, -897.68f, 41.536f, 0},
	{29.79f, -923.35f, 42.9f, 0},
	{44.269f, -948.832f, 41.54f, 0}
};

static float ShieldGeneratorCoords[4][3] =
{
	{49.256f, -902.354f, 42.9769f},
	{9.78695f, -902.882f, 42.9f},
	{10.4122f, -944.613f, 42.8262f},
	{49.687f, -944.406f, 42.7324f}
};

static Location fly[] = 
{
	{ 29.769f, -866.190f, 43 },
	{ 1.135f, -874.345f, 43 },
	{ -19.719f, -894.950f, 43 },
	{ -27.4222f, -923.572f, 43 },
	{ -19.739f, -951.907f, 43 },
	{ 1.059f, -973.314f, 43 },
	{ 30.071f, -980.424f, 43 },
	{ 58.665f, -973.410f, 43 },
	{ 79.353f, -952.011f, 43 },
	{ 87.552f, -923.175f, 43 },
	{ 79.068f, -894.570f, 43 },
	{ 58.503f, -873.295f, 43 }
};

class EnchantedElementalAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(EnchantedElementalAI);
		
	EnchantedElementalAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);

		Unit* Vashj = NULL;
		Vashj = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(29.798161f, -923.358276f, 42.900517f, CN_LADY_VASHJ);
		if(Vashj)
		{
			_unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
			_unit->GetAIInterface()->setWaypointToMove(1);
		}
	}

	void OnReachWP(uint32 iWaypointId, bool bForwards)
	{
		switch(iWaypointId)
		{
		case 1:
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
			_unit->GetAIInterface()->setWaypointToMove(2);
			break;

		case 2:
			Unit* Vashj = NULL;
			Vashj = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(29.798161f, -923.358276f, 42.900517f, CN_LADY_VASHJ);
			if(Vashj)
			{
				//Increase Lady Vashj attack by 5%
				Vashj->ModFloatValue(UNIT_FIELD_MINDAMAGE, (Vashj->GetMinDamage()/100)*5);
				Vashj->ModFloatValue(UNIT_FIELD_MAXDAMAGE, (Vashj->GetMaxDamage()/100)*5);
			}

			//despawn
			_unit->Despawn(1, 0);
			break;
		}
	}

};

class VashjAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(VashjAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];
		
	VashjAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 3;

		spells[0].info = dbcSpell.LookupEntry(SHOCK_BLAST);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = 5;
		spells[0].perctrigger = 5.0f;
		spells[0].attackstoptimer = 2000;
		m_spellcheck[0] = false;

		spells[1].info = dbcSpell.LookupEntry(STATIC_CHARGE);
		spells[1].targettype = TARGET_RANDOM_SINGLE;
		spells[1].instant = true;
		spells[1].cooldown = 5;
		spells[1].perctrigger = 10.0f;
		spells[1].attackstoptimer = 1000;
		m_spellcheck[1] = false;

		spells[2].info = dbcSpell.LookupEntry(ENTANGLE);
		spells[2].targettype = TARGET_RANDOM_DESTINATION;
		spells[2].instant = true;
		spells[2].cooldown = 15;
		spells[2].perctrigger = 10.0f;
		spells[2].attackstoptimer = 1000;
		m_spellcheck[2] = false;

		spells[3].info = dbcSpell.LookupEntry(FORKED_LIGHTNING);
		spells[3].targettype = TARGET_RANDOM_SINGLE;
		spells[3].perctrigger = 0.0f;
		spells[3].instant = false;

		info_multishot = dbcSpell.LookupEntry(MULTI_SHOT);
		info_shot = dbcSpell.LookupEntry(SHOOT);

		WayPoint * wp = _unit->CreateWaypointStruct();
		wp->id = 1;
		wp->x = 29.798161f;
		wp->y = -923.358276f;
		wp->z = 42.900517f;
		wp->o = 0.0f;
		wp->waittime = 0;
		wp->flags = RUN;
		wp->forwardemoteoneshot = 0;
		wp->forwardemoteid = 0;
		wp->backwardemoteoneshot = 0;
		wp->backwardemoteid = 0;
		wp->forwardskinid = 0;
		wp->backwardskinid = 0;

		_unit->GetAIInterface()->addWayPoint(wp);
		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);

		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
	}

	void ResetCastTime()
	{
		for(int i = 0; i < nrspells; i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void OnCombatStart(Unit* mTarget)
	{
		ResetCastTime();
		Phase = 1;
		EnchantedElementalTimer = 10;
		CoilfangStriderTimer = 60;
		CoilfangEliteTimer = 40;
		TaintedElementalTimer = 50;
		SporebatTimer = 0;
		ForkedLightningTimer = 5;

		switch(rand()%4)
		{
		case 0:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I'll split you from stem to stern!");
			_unit->PlaySoundToSet(11532);
			break;

		case 1:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Victory to Lord Illidan!");
			_unit->PlaySoundToSet(11533);
			break;

		case 2:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I spit on you, surface filth!");
			_unit->PlaySoundToSet(11534);
			break;

		case 3:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Death to the outsiders!");
			_unit->PlaySoundToSet(11535);
			break;
		}

		RegisterAIUpdateEvent(1000);
	}
		
	void OnCombatStop(Unit* mTarget)
	{
		//despawn enchanted elemental, tainted elemental, coilfang elite, coilfang strider
		Creature* creature = NULL;
		for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
		{
			if((*itr)->IsCreature())
			{
				creature = TO_CREATURE((*itr));
	
				if((creature->GetCreatureInfo()->Id == CN_ENCHANTED_ELEMENTAL || 
					creature->GetCreatureInfo()->Id == CN_TAINTED_ELEMENTAL ||
					creature->GetCreatureInfo()->Id == CN_COILFANG_STRIDER ||
					creature->GetCreatureInfo()->Id == CN_COILFANG_ELITE ||
                    creature->GetCreatureInfo()->Id == CN_SHIELD_GENERATOR_CHANNEL)
					&& creature->isAlive())
					creature->Despawn(500, 0);
			}
		}

		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
		_unit->RemoveAura(VASHJ_SHIELD);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
		_unit->GetAIInterface()->m_canMove = true;
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}
		
	void OnDied(Unit* mKiller)
	{
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Lord Illidan, I... I am... sorry.");
		_unit->PlaySoundToSet(11544);
		RemoveAIUpdateEvent();
	}
		
	void OnTargetDied(Unit* mTarget)
	{
		switch(rand()%2)
		{
		case 0:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Your time ends now!");
			_unit->PlaySoundToSet(11541);
			break;

		case 1:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You have failed!");
			_unit->PlaySoundToSet(11542);
			break;
		}
	}
		
	void AIUpdate()
	{
		switch(Phase)
		{
		case 1:
		case 3:
			PhaseOneAndThree();
			break;
		case 2:
			PhaseTwo();
			break;
		}
	}
		
	void PhaseOneAndThree()
	{
		if(Phase == 3)
		{
			//WRONG
			//sporebats are spawned faster and faster
			/*SporebatTimer++;
			if(SporebatTimer > 10)
			{
				Creature* cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_TOXIC_SPORE_BAT,  29.769f, -866.190f, 43, 0, true, false, 0, 0);
				cre->GetAIInterface()->setOutOfCombatRange(1);		
				SporebatTimer = 0;
			}*/
		}
		else
		{
			if(_unit->GetHealthPct() <= 70)
			{
				_unit->RemoveAllAuras();
				_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
				_unit->GetAIInterface()->StopMovement(0);
				_unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
				_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
				_unit->GetAIInterface()->setWaypointToMove(1);
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The time is now! Leave none standing!");
				_unit->PlaySoundToSet(11539);
				_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
				_unit->CastSpell(_unit, dbcSpell.LookupEntry(VASHJ_SHIELD), true);
				_unit->GetAIInterface()->setOutOfCombatRange(3000);
				Phase = 2;
			}
		}

		//if nobody is in range, shot or multishot
		bool InRange = false;
		for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
		{
			if(isHostile(_unit, (*itr)) && _unit->GetDistance2dSq((*itr)) < 100) //10 yards
			{
				InRange = true;
				break;
			}
		}
		if(!InRange)
		{
			Shoot(_unit->GetAIInterface()->getNextTarget());
		}

		float val = RandomFloat(100.0f);
		SpellCast(val);
	}

	void PhaseTwo()
	{
		//WORKAROUND
		_unit->setAttackTimer(2000, false);

		//Forked Lightning
		ForkedLightningTimer--;
		if(!ForkedLightningTimer)
		{
			CastSpellOnRandomTarget(3, 0, 0);
			ForkedLightningTimer = 2+rand()%6;
		}

		//spawn creatures
		EnchantedElementalTimer--;
		if(!EnchantedElementalTimer)
		{
			uint32 pos = rand()%8;
			Creature* elemental = NULL;
			elemental = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ENCHANTED_ELEMENTAL, ElementalSpawnPoints[pos].x, ElementalSpawnPoints[pos].y, ElementalSpawnPoints[pos].z, ElementalSpawnPoints[pos].o, true, false, 0, 0);
			if(elemental)
			{
				WayPoint * wp = _unit->CreateWaypointStruct();
				wp->id = 1;
				wp->x = ElementalSpawnPoints2[pos].x;
				wp->y = ElementalSpawnPoints2[pos].y;
				wp->z = ElementalSpawnPoints2[pos].z;
				wp->o = ElementalSpawnPoints2[pos].o;
				wp->waittime = 0;
				wp->flags = WALK;
				wp->forwardemoteoneshot = 0;
				wp->forwardemoteid = 0;
				wp->backwardemoteoneshot = 0;
				wp->backwardemoteid = 0;
				wp->forwardskinid = 0;
				wp->backwardskinid = 0;
				elemental->GetAIInterface()->addWayPoint(wp);

				wp = _unit->CreateWaypointStruct();
				wp->id = 2;
				wp->x = 29.798161f;
				wp->y = -923.358276f;
				wp->z = 42.900517f;
				wp->o = 0.0f;
				wp->o = 0;
				wp->waittime = 0;
				wp->flags = WALK;
				wp->forwardemoteoneshot = 0;
				wp->forwardemoteid = 0;
				wp->backwardemoteoneshot = 0;
				wp->backwardemoteid = 0;
				wp->forwardskinid = 0;
				wp->backwardskinid = 0;
				elemental->GetAIInterface()->addWayPoint(wp);
			}
			EnchantedElementalTimer = 10+rand()%5;
		}
		CoilfangStriderTimer--;
		if(!CoilfangStriderTimer)
		{
			Creature* summoned = NULL;
			summoned = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_COILFANG_STRIDER, -29.761278f, -980.252930f, 41.097122f, 0.0f, true, false, 0, 0);
			if(summoned)
			{
				//attack nearest target
				Unit* nearest = NULL;
				float nearestdist = 0;
				for(set<Object*>::iterator itr = summoned->GetInRangeSetBegin(); itr != summoned->GetInRangeSetEnd(); ++itr) 
				{
					if((*itr)->IsUnit() && isHostile(summoned, (*itr)) && (summoned->GetDistance2dSq((*itr)) < nearestdist || !nearestdist) )
					{
						nearestdist = summoned->GetDistance2dSq((*itr));
						nearest = TO_UNIT((*itr));
					}
				}
				if(nearest)
					summoned->GetAIInterface()->AttackReaction(nearest, 1, 0);
			}
			CoilfangStriderTimer = 60;
		}
		CoilfangEliteTimer--;
		if(!CoilfangEliteTimer)
		{
			uint32 pos = rand()%4;
			Creature* summoned = NULL;
			summoned = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_COILFANG_ELITE, CoilfangEliteSpawnPoints[pos].x, CoilfangEliteSpawnPoints[pos].y, CoilfangEliteSpawnPoints[pos].z, CoilfangEliteSpawnPoints[pos].o, true, false, 0, 0);
			if(summoned)
			{
				//attack nearest target
				Unit* nearest = NULL;
				float nearestdist = 0;
				for(set<Object*>::iterator itr = summoned->GetInRangeSetBegin(); itr != summoned->GetInRangeSetEnd(); ++itr) 
				{
					if((*itr)->IsUnit() && isHostile(summoned, (*itr)) && (summoned->GetDistance2dSq((*itr)) < nearestdist || !nearestdist) )
					{
						nearestdist = summoned->GetDistance2dSq((*itr));
						nearest = TO_UNIT((*itr));
					}
				}
				if(nearest)
					summoned->GetAIInterface()->AttackReaction(nearest, 1, 0);
			}
			CoilfangEliteTimer = 45;
		}
		TaintedElementalTimer--;
		if(!TaintedElementalTimer)
		{
			uint32 pos = rand()%8;
			_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_TAINTED_ELEMENTAL, ElementalSpawnPoints[pos].x, ElementalSpawnPoints[pos].y, ElementalSpawnPoints[pos].z, ElementalSpawnPoints[pos].o, true, false, 0, 0);
			TaintedElementalTimer = 120;
		}

		if(_unit->GetHealthPct() <= 50)
		{
			//despawn enchanted elementals
			Creature* creature = NULL;
			for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{
				if((*itr)->IsCreature())
				{
					creature = TO_CREATURE((*itr));

					if(creature->GetCreatureInfo()->Id == CN_ENCHANTED_ELEMENTAL && creature->isAlive())
						creature->Despawn(0, 0);
				}
			}

			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
			_unit->RemoveAura(VASHJ_SHIELD);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You may want to take cover.");
			_unit->PlaySoundToSet(11540);
			_unit->GetAIInterface()->m_canMove = true;
			Phase = 3;
		}
	}

	void Shoot(Unit* target)
	{
        switch(rand()%2)
        {
        case 0:
            //shoot
			_unit->CastSpell(target, info_shot, true);
            break;

        case 1:
            //multishot
            _unit->CastSpell(target, info_multishot, true);
            break;
        }

		switch(rand()%6)
		{
		case 0:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Straight to the heart!");
			_unit->PlaySoundToSet(11536);
			break;

		case 1:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Seek your mark!");
			_unit->PlaySoundToSet(11537);
			break;
		}
	}

	void OnReachWP(uint32 iWaypointId, bool bForwards)
	{
		switch(iWaypointId)
		{
		case 1:
			_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
			_unit->GetAIInterface()->m_canMove = false;

			//setup shield
			Creature* channel = NULL;
			for(uint8 i = 0; i < 4; i++)
			{
				channel = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_SHIELD_GENERATOR_CHANNEL, ShieldGeneratorCoords[i][0],  ShieldGeneratorCoords[i][1],  ShieldGeneratorCoords[i][2], 0, true, false, 0, 0);
				if(channel)
				{
					channel->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
					channel->GetAIInterface()->m_canMove = false;
					channel->SetChannelSpellTargetGUID(_unit->GetGUID());
					channel->SetChannelSpellId(VASHJ_SHIELD);
				}
			}
			break;
		}
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
		{
			float comulativeperc = 0;
			Unit* target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
					
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->getNextTarget();
					switch(spells[i].targettype)
					{
					case TARGET_SELF:
					case TARGET_VARIOUS:
						_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
					case TARGET_ATTACKING:
						_unit->CastSpell(target, spells[i].info, spells[i].instant);
						break;
					case TARGET_DESTINATION:
						_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					case TARGET_RANDOM_SINGLE:
					case TARGET_RANDOM_DESTINATION:
						CastSpellOnRandomTarget(i, 0, 0); break;
					}
					spells[i].casttime = spells[i].cooldown;
					m_spellcheck[i] = false;
					return;
				}

				if(spells[i].casttime > 0)
					spells[i].casttime--;
					
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && !spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;

		Unit* RandomTarget = NULL;
		std::vector<Unit*> TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
		for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
		{ 
			if (isHostile(_unit, (*itr)) && (*itr)->IsUnit())
			{
				RandomTarget = TO_UNIT(*itr);

				if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast)
					TargetTable.push_back(RandomTarget);
			} 
		}

		if (!TargetTable.size())
			return;

		Unit*  RTarget = *(TargetTable.begin()+rand()%TargetTable.size());

		if (!RTarget)
			return;

		switch (spells[i].targettype)
		{
		case TARGET_RANDOM_SINGLE:
			_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
		case TARGET_RANDOM_DESTINATION:
			_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
		}

		TargetTable.clear();
	}

	uint32 TaintedElementalTimer;
	uint32 Phase;
		
protected:
	int nrspells;
	uint32 EnchantedElementalTimer;
	uint32 CoilfangStriderTimer;
	uint32 CoilfangEliteTimer;
	uint32 SporebatTimer;
	uint32 ForkedLightningTimer;
	SpellEntry *info_multishot;
	SpellEntry *info_shot;
};

class TaintedElementalAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(TaintedElementalAI);
		
	TaintedElementalAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		spell_poison_spit = new AI_Spell();
		spell_poison_spit->agent = AGENT_SPELL;
		spell_poison_spit->procChance = 0;
		spell_poison_spit->spell = dbcSpell.LookupEntry(POISON_SPIT);
		spell_poison_spit->spellType = STYPE_DAMAGE;
		spell_poison_spit->spelltargetType = TTYPE_SINGLETARGET;
		spell_poison_spit->cooldown = 2000;
		spell_poison_spit->cooldowntime = 0;
		spell_poison_spit->procCount = 0;
		spell_poison_spit->procCounter = 0;
		spell_poison_spit->minrange = 0;
		spell_poison_spit->maxrange = 30;

		_unit->GetAIInterface()->m_canMove = false;
	}

	void OnCombatStart(Unit* mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_SPELL);
		RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
	}

	void OnCombatStop(Unit* mTarget)
	{
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit* mKiller)
	{
		Creature* Vashj = NULL;
		Vashj = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), CN_LADY_VASHJ);
		if(Vashj)
		{
			if(TO< VashjAI* >(Vashj->GetScript())->TaintedElementalTimer > 50)
				TO< VashjAI* >(Vashj->GetScript())->TaintedElementalTimer = 50;
		}
	}

	void AIUpdate()
	{
		//TODO: Despawn after 15 secs
		if(_unit->GetCurrentSpell() == NULL)
			_unit->GetAIInterface()->SetNextSpell(spell_poison_spit);
	}

	void Destroy()
	{
		// Could someone confirm it's nowhere saved, so we won't have any crashes related
		// to using deleted pointers ?
		if ( spell_poison_spit != NULL )
		{
			delete spell_poison_spit;
			spell_poison_spit = NULL;
		};

		delete this;
	};

private:
	AI_Spell *spell_poison_spit;
};

class TaintedCoreGO : public GameObjectAIScript
{
public:
	TaintedCoreGO(GameObject* pGameObject) : GameObjectAIScript(pGameObject)
	{
	}

	void OnActivate(Player* pPlayer)
	{
		Creature* Vashj = NULL;
		Vashj = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(29.798161f, -923.358276f, 42.900517f, CN_LADY_VASHJ);
		if( Vashj != NULL && TO< VashjAI* >(Vashj->GetScript())->Phase == 2 )
		{
			Vashj->ModHealth(-((Vashj->GetUInt32Value(UNIT_FIELD_MAXHEALTH)/100)*5) );
			Creature* channel = NULL;
			channel = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), CN_SHIELD_GENERATOR_CHANNEL);
			if(channel != NULL && channel->IsInWorld() )
				channel->Despawn(0, 0);
		}
	}

	static GameObjectAIScript *Create(GameObject* pGameObject) { return new TaintedCoreGO(pGameObject); }
};

class ToxicSporeBatAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(ToxicSporeBatAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];
		
	ToxicSporeBatAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		/************************ Waypoint Place ************************/
		
		m_entry = pCreature->GetEntry();
		
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(1, 0, FLY));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(2, 0, FLY));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(3, 0, FLY));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(4, 0, FLY));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(5, 0, FLY));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(6, 0, FLY));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(7, 0, FLY));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(8, 0, FLY));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(9, 0, FLY));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(10, 0, FLY));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(11, 0, FLY));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(12, 0, FLY));
		
		/************************** Spells ****************************/
		
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		
		spells[0].info = dbcSpell.LookupEntry(TOXIC_SPORES);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].cooldown = -1;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;
		
		/******************* Additional Settings *******************/
		
		Phase = 0;
		FlameQuills = false;
		Meteor = false;
		PositionChange=rand()%8+15;
		PhoenixSummon=rand()%6+17;
		_unit->GetAIInterface()->m_moveFly = true;
		_unit->GetAIInterface()->StopMovement(0);
		_unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
		_unit->GetAIInterface()->setWaypointToMove(1);
		RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
	}
		
	void OnCombatStart(Unit* mTarget)
	{
		//_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Phase 1 Test!");
		_unit->PlaySoundToSet(11243);
		_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
		CastTime();
		Phase = 1;
		FlameQuills = false;
		Meteor = false;
		PositionChange=rand()%8+15;	// 30-45sec /*** if attack time 1000 (%15+31) ***/
		PhoenixSummon=rand()%6+17;	// 34-44sec /*** if attack time 1000 (%11+34) ***/
		FlyWay=rand()%2;
		switch (FlyWay)
		{
		case 0:	// Clock like
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
			_unit->GetAIInterface()->setWaypointToMove(6);
			break;
				
		case 1:	// hmm... other?
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
			_unit->GetAIInterface()->setWaypointToMove(9);
			break;
		}
		//RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));	// Taken from here to add more abilities to code
	}
		
	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}
		
	void OnCombatStop(Unit* mTarget)
	{
		Phase = 0;
		FlameQuills = false;
		Meteor = false;
		PhoenixSummon=rand()%6+17;
		PositionChange=rand()%8+15;
		CastTime();
		_unit->GetAIInterface()->StopMovement(0);
		_unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
		_unit->GetAIInterface()->setWaypointToMove(1);
		//_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		//_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		//RemoveAIUpdateEvent();
	}
		
	void OnDied(Unit* mKiller)
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
					_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
					_unit->GetAIInterface()->setWaypointToMove(6);
					break;
					
				case 1:	// hmm... other?
					_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
					_unit->GetAIInterface()->setWaypointToMove(9);
					break;
				}
			}
			_unit->CastSpell(_unit, spells[0].info, spells[0].instant);
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
		//float val = RandomFloat(100.0f);
		//SpellCast(val);
	}
		
	void PhaseOne()
	{
		PositionChange--;
		PhoenixSummon--;
		
		if (_unit->GetHealthPct() == 0)
		{
			Phase = 2;
			_unit->CastSpell(_unit, spells[0].info, spells[0].instant);
		}
		
		if (!PhoenixSummon--)
		{
			_unit->CastSpell(_unit, spells[0].info, spells[0].instant);
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
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
		{
			float comulativeperc = 0;
			Unit* target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{				
				spells[i].casttime = spells[i].cooldown;
				target = _unit->GetAIInterface()->getNextTarget();
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
		case 1:	// First fly point
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
			_unit->GetAIInterface()->setWaypointToMove(2);
			break;
				
		case 2:
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
			_unit->GetAIInterface()->setWaypointToMove(3);
			break;
				
		case 3:
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
			_unit->GetAIInterface()->setWaypointToMove(4);
			break;
				
		case 4:
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
			_unit->GetAIInterface()->setWaypointToMove(5);
			break;
				
		case 5:
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
			_unit->GetAIInterface()->setWaypointToMove(1);	// Last fly point (flyback to point 1 - reset)
			break;
				
		case 6:
		{
			_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
			_unit->GetAIInterface()->m_canMove = false;
			switch (FlyWay)
			{
			case 0:
				NextWP = 7;
				break;
					
			case 1:	
				NextWP = 9;
				break;
			}
		}
			break;
				
		case 7:
		{
			_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
			_unit->GetAIInterface()->m_canMove = false;
			switch (FlyWay)
			{
			case 0:
				NextWP = 8;
				break;
					
			case 1:	
				NextWP = 6;
				break;
			}
		}
			break;
				
		case 8:
		{
			_unit->GetAIInterface()->m_canMove = false;
			_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
			switch (FlyWay)
			{
			case 0:
				NextWP = 9;
				break;
					
			case 1:
				NextWP = 7;
				break;
			}
		}
			break;
				
		case 9:
		{
			_unit->GetAIInterface()->m_canMove = false;
			_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
			switch (FlyWay)
			{
			case 0:
				NextWP = 6;
				break;

			case 1:	
				NextWP = 8;
				break;
			}
		}
			break;
				
		case 10:
		{
			_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
			if (Phase == 1)
			{
				FlameQuills = true;
				QuillsCount = 0;
			}
				
			if (Phase == 2)
			{
				Meteor = true;
			}
				
		}
			break;
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


//------------------------------------
//	-= Trash Mobs =-
//------------------------------------
	
//Coilfang Ambusher
#define CN_COILFANG_AMBUSHER 21865
#define CA_MULTI_SHOT 27021

class CoilfangAmbusherAI : public MoonScriptCreatureAI
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(CoilfangAmbusherAI);
	CoilfangAmbusherAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(CA_MULTI_SHOT, Target_Self, 10.0f, 0, 0);
	}
};

//Coilfang Fathom-Witch
#define CN_COILFANG_FATHOM_WITCH 21299
#define SHADOW_BOLT 27209 
#define WHIRLWIND_KNOCKBACK 34109 

class CoilfangFathomWitchAI : public MoonScriptCreatureAI
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(CoilfangFathomWitchAI);
	CoilfangFathomWitchAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SHADOW_BOLT, Target_Current, 2.0f, 0, 0);
		AddSpell(WHIRLWIND_KNOCKBACK, Target_Self, 2.0f, 0, 0);
	}
};

//Coilfang Guardian
#define CN_COILFANG_GUARDIAN 21873
#define CLEAVE 38260

class CoilfangGuardianAI : public MoonScriptCreatureAI
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(CoilfangGuardianAI);
	CoilfangGuardianAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(CLEAVE, Target_Destination, 3.0f, 0, 0);
	}
};

//Coilfang Priestess
#define CN_COILFANG_PRIESTESS 21220
#define HOLY_NOVA 38589 
#define SMITE 25364 
#define SPIRIT_OF_REDEMPTION 35618

class CoilfangPriestessAI : public MoonScriptCreatureAI
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(CoilfangPriestessAI);
	CoilfangPriestessAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(HOLY_NOVA, Target_Self, 2.0f, 0, 0);
		AddSpell(SMITE, Target_Current, 1.0f, 2, 0);
		AddSpell(SPIRIT_OF_REDEMPTION, Target_Self, 2.0f, 0, 0);
	}
};

//Underbog Colossus 
#define CN_UNDERBOG_COLOSSUS 21251
#define ACID_GEYSER 37959 
#define ATROPIC_BLOW 39015 
#define SPORE_QUAKE 38976
#define TOXIC_POOL 38718
#define FRENZY 37605
#define RAMPANT_INFECTION 39042
#define PARASITE 39044

class UnderbogColossusAI : public MoonScriptCreatureAI
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(UnderbogColossusAI);
	UnderbogColossusAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		//these mobs pick from a random set of abilities
		switch(rand()%3)
		{
		case 0:
			AddSpell(RAMPANT_INFECTION, Target_Self, 5.0f, 0, 0);
			AddSpell(SPORE_QUAKE, Target_Self, 2.0f, 0, 0);
			break;

		case 1:
			AddSpell(ACID_GEYSER, Target_Destination, 10.0f, 0, 0);
			AddSpell(PARASITE, Target_Current, 2.0f, 0, 0);
			break;

		case 2:
			AddSpell(FRENZY, Target_Self, 10.0f, 0, 0);
			break;
		}
	}

	void OnDied(Unit* pKiller)
	{
		//There will also be a choice of abilities he might use as he dies:
		switch(rand()%3)
		{
		case 0:
			//cast toxic pool
			_unit->CastSpell(_unit, dbcSpell.LookupEntry(TOXIC_POOL), true);
			break;

		case 1:
			//spawn two colossus lurkers
			_unit->GetMapMgr()->GetInterface()->SpawnCreature(22347, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 0, true, false, 0, 0);
			_unit->GetMapMgr()->GetInterface()->SpawnCreature(22347, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 0, true, false, 0, 0);
			break;

		default:
			//Nothing!
			break;

			//Many small adds, TODO
			//Refreshing mist, TODO
		}

		MoonScriptCreatureAI::OnDied(pKiller);
	}
};

//Tidewalker Warrior
#define CN_TIDEWALKER_WARRIOR 21225
#define TW_CLEAVE 38260 
#define TW_BLOODTHIRST 30335 //INSTANT
#define TW_FRENZY 37605

class TidewalkerWarriorAI : public MoonScriptCreatureAI
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(TidewalkerWarriorAI);
	TidewalkerWarriorAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(TW_CLEAVE, Target_Destination, 1.0f, 0, 0);
		AddSpell(TW_BLOODTHIRST, Target_Current, 1.0f, -1, 0); //-1 means instant
		AddSpell(TW_FRENZY, Target_Self, 2.0f, 0, 0);
	}
};

//Coilfang Serpentguard
#define CN_COILFANG_SERPENTGUARD 21298
#define CSERP_CLEAVE 38260 
#define CSERP_REFLECTION 36096 
#define CSERP_DEVOTION 38603 

class CoilfangSerpentguardAI : public MoonScriptCreatureAI
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(CoilfangSerpentguardAI);
	CoilfangSerpentguardAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(CSERP_CLEAVE, Target_Destination, 1.0f, 0, 0);
		AddSpell(CSERP_REFLECTION, Target_Self, 0.5f, 0, 0);
		AddSpell(CSERP_DEVOTION, Target_Self, 1.0f, 0, 0);
	}
};

//Coilfang Shatterer
#define CN_COILFANG_SHATTERER 21301
#define CSHATT_ARMOR 38591 

class CoilfangShattererAI : public MoonScriptCreatureAI
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(CoilfangShattererAI);
	CoilfangShattererAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(CSHATT_ARMOR, Target_Current, 2.0f, 0, 0);
	}
};

//Coilfang Strider
#define CN_COILFANG_STRIDER 22056
#define CSTRID_SCREAM 10890 

class CoilfangStriderAI : public MoonScriptCreatureAI
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(CoilfangStriderAI);
	CoilfangStriderAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(CSTRID_SCREAM, Target_Current, 2.0f, 0, 0);
	}
};

// Serpentshrine Cavern Instance Script
class SerpentshrineCavern : public MoonInstanceScript
{
public:
	// Console & Bridge parts
	uint32 mBridgePart[3];

	MOONSCRIPT_INSTANCE_FACTORY_FUNCTION( SerpentshrineCavern, MoonInstanceScript );
	SerpentshrineCavern(MapMgr* pMapMgr) : MoonInstanceScript(pMapMgr)
	{
		for( int i = 0; i < 3; ++i )
			mBridgePart[i] = 0;
	}

	void OnGameObjectPushToWorld( GameObject* pGameObject )
	{
		switch( pGameObject->GetEntry() )
		{
		case 184203: mBridgePart[0] = pGameObject->GetLowGUID(); break;
		case 184204: mBridgePart[1] = pGameObject->GetLowGUID(); break;
		case 184205: mBridgePart[2] = pGameObject->GetLowGUID(); break;
		}
	};

    void OnGameObjectActivate(GameObject* pGameObject, Player* pPlayer )
    {
		if(pGameObject->GetInfo()->ID != 184568)
			return;

		GameObject* pBridgePart = NULL;

		for( int i = 0; i < 3; ++i )
		{
			pBridgePart = GetGameObjectByGuid( mBridgePart[i] );
			if( pBridgePart != NULL )
				pBridgePart->SetState(0);
		};


		pGameObject->SetState(0);
    }

 };

void SetupSerpentshrineCavern(ScriptMgr * mgr)
{
	mgr->register_creature_script(CN_HYDROSS_THE_UNSTABLE, &HydrossTheUnstableAI::Create);
	mgr->register_creature_script(CN_THE_LURKER_BELOW, &LurkerAI::Create);

	//Leotheras the Blind event
	mgr->register_creature_script(CN_LEOTHERAS_THE_BLIND, &LeotherasAI::Create);
	mgr->register_creature_script(CN_GREYHEART_SPELLBINDER, &GreyheartSpellbinderAI::Create);
	mgr->register_creature_script(CN_SHADOW_OF_LEOTHERAS, &ShadowofLeotherasAI::Create);
	

	//Morogrim Tidewalker event
	mgr->register_creature_script(CN_MOROGRIM_TIDEWALKER, &MorogrimAI::Create);
	mgr->register_creature_script(CN_TIDEWALKER_LURKER, &TidewalkerLurkerAI::Create);
	
	//Fathom-Lord Karathress event
	mgr->register_creature_script(CN_FATHOM_LORD_KARATHRESS, &KarathressAI::Create);
	mgr->register_creature_script(CN_FATHOM_GUARD_CARIBDIS, &FathomGuardCaribdisAI::Create);
	mgr->register_creature_script(CN_FATHOM_GUARD_TIDALVESS, &FathomGuardTidalvessAI::Create);
	mgr->register_creature_script(CN_FATHOM_GUARD_SHARKKIS, &FathomGuardSharkissAI::Create);

	//Lady Vashj event
	mgr->register_creature_script(CN_LADY_VASHJ, &VashjAI::Create);
	//mgr->register_creature_script(CN_TOXIC_SPORE_BAT, &ToxicSporeBatAI::Create);
	mgr->register_creature_script(CN_COILFANG_STRIDER, &CoilfangStriderAI::Create);
	mgr->register_creature_script(CN_ENCHANTED_ELEMENTAL, &EnchantedElementalAI::Create);
	mgr->register_creature_script(CN_TAINTED_ELEMENTAL, &TaintedElementalAI::Create);

	// Shield Generator
	mgr->register_gameobject_script(185051, &TaintedCoreGO::Create);

	// Serpentsrine Cavern instance script
	mgr->register_instance_script(548, &SerpentshrineCavern::Create);

	//Trash mobs
	mgr->register_creature_script(CN_COILFANG_AMBUSHER, &CoilfangAmbusherAI::Create);
	mgr->register_creature_script(CN_COILFANG_FATHOM_WITCH, &CoilfangFathomWitchAI::Create);
	mgr->register_creature_script(CN_COILFANG_GUARDIAN, &CoilfangGuardianAI::Create);
	mgr->register_creature_script(CN_COILFANG_PRIESTESS, &CoilfangPriestessAI::Create);
	mgr->register_creature_script(CN_UNDERBOG_COLOSSUS, &UnderbogColossusAI::Create);
	mgr->register_creature_script(CN_TIDEWALKER_WARRIOR, &TidewalkerWarriorAI::Create);
	mgr->register_creature_script(CN_COILFANG_SERPENTGUARD, &CoilfangSerpentguardAI::Create);
	mgr->register_creature_script(CN_COILFANG_SHATTERER, &CoilfangShattererAI::Create);
}
