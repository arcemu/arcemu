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
/* Instance_AuchenaiCrypts.cpp Script		                            */
/************************************************************************/

/*****************************/
/*                           */
/*         Boss AIs          */
/*                           */
/*****************************/

// Shirrak the Dead WatcherAI

#define CN_SHIRRAK_THE_DEAD_WATCHER 18371

#define INHIBIT_MAGIC		32264
#define CARNIVOROUS_BITE	41092 // Also can be: 36383 or 41092
#define FOCUS_FIRE			32310 // not fully functional for now =/ Let's try: 32310, 32301 or 32300	- needs further researches
#define ATTRACT_MAGIC		32265 // doesn't work anyway
// Hmmm... next boss without sounds?

class SHIRRAKTHEDEADWATCHERAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(SHIRRAKTHEDEADWATCHERAI);
		SP_AI_Spell spells[4];
		bool m_spellcheck[4];

		SHIRRAKTHEDEADWATCHERAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 4;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(INHIBIT_MAGIC);
			spells[0].targettype = TARGET_SELF;
			spells[0].instant = true;
			spells[0].perctrigger = 7.0f;
			spells[0].attackstoptimer = 1000;
			spells[0].cooldown = 10;

			spells[1].info = dbcSpell.LookupEntry(CARNIVOROUS_BITE);
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = true;
			spells[1].perctrigger = 15.0f;
			spells[1].attackstoptimer = 1000;
			spells[1].cooldown = 10;

			spells[2].info = dbcSpell.LookupEntry(FOCUS_FIRE);
			spells[2].targettype = TARGET_RANDOM_DESTINATION;   // changed from attack as it wasn't working anyway
			spells[2].instant = false;
			spells[2].perctrigger = 8.0f;
			spells[2].attackstoptimer = 1000;
			spells[2].cooldown = 15;
			spells[2].mindist2cast = 0.0f;
			spells[2].maxdist2cast = 40.0f;

			spells[3].info = dbcSpell.LookupEntry(ATTRACT_MAGIC);
			spells[3].targettype = TARGET_VARIOUS;
			spells[3].instant = true;
			spells[3].perctrigger = 10.0f;
			spells[3].attackstoptimer = 1000;
			spells[3].cooldown = 15;
		}

		void OnCombatStart(Unit* mTarget)
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = 0;

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
				for(int i = 0; i < nrspells; i++)
				{
					if(!spells[i].perctrigger) continue;

					if(m_spellcheck[i])
					{
						if(!spells[i].instant)
							_unit->GetAIInterface()->StopMovement(1);

						target = _unit->GetAIInterface()->getNextTarget();
						switch(spells[i].targettype)
						{
							case TARGET_SELF:
							case TARGET_VARIOUS:
								_unit->CastSpell(_unit, spells[i].info, spells[i].instant);
								break;
							case TARGET_ATTACKING:
								_unit->CastSpell(target, spells[i].info, spells[i].instant);
								break;
							case TARGET_DESTINATION:
								_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), spells[i].info, spells[i].instant);
								break;
							case TARGET_RANDOM_FRIEND:
							case TARGET_RANDOM_SINGLE:
							case TARGET_RANDOM_DESTINATION:
								CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast);
								break;
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
			if(!maxdist2cast) maxdist2cast = 100.0f;
			if(!maxhp2cast) maxhp2cast = 100;

			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
			{
				std::vector<Unit*> TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
				/* If anyone wants to use this function, then leave this note!										 */
				for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr)
				{
					if(((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && (*itr)->IsUnit())  // isAttackable(_unit, (*itr)) &&
					{
						Unit* RandomTarget = NULL;
						RandomTarget = TO_UNIT(*itr);

						if(RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast * mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast * maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
						{
							TargetTable.push_back(RandomTarget);
						}
					}
				}

				if(_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
					TargetTable.push_back(_unit);

				if(!TargetTable.size())
					return;

				size_t RandTarget = rand() % TargetTable.size();

				Unit*  RTarget = TargetTable[RandTarget];

				if(!RTarget)
					return;

				switch(spells[i].targettype)
				{
					case TARGET_RANDOM_FRIEND:
					case TARGET_RANDOM_SINGLE:
						_unit->CastSpell(RTarget, spells[i].info, spells[i].instant);
						break;
					case TARGET_RANDOM_DESTINATION:
						_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant);
						break;
				}

				TargetTable.clear();
			}
		}

	protected:

		int nrspells;
};


// Avatar of the MartyredAI

#define CN_AVATAR_OF_THE_MARTYRED 18478

#define SUNDER_ARMOR	16145
#define MORTAL_STRIKE	15708	// not sure to spells ofc :)
#define PHASE_IN		33422

class AvatarOfTheMartyredAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(AvatarOfTheMartyredAI);
		SP_AI_Spell spells[3];
		bool m_spellcheck[3];

		AvatarOfTheMartyredAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 2;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(SUNDER_ARMOR);
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = true;
			spells[0].perctrigger = 15.0f;
			spells[0].attackstoptimer = 1000;
			spells[0].cooldown = 10;

			spells[1].info = dbcSpell.LookupEntry(MORTAL_STRIKE);
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = true;
			spells[1].perctrigger = 10.0f;
			spells[1].attackstoptimer = 1000;
			spells[1].cooldown = 10;

			spells[2].info = dbcSpell.LookupEntry(PHASE_IN);
			spells[2].targettype = TARGET_SELF;
			spells[2].instant = true;
			spells[2].perctrigger = 0.0f;
			spells[2].attackstoptimer = 1000;
			spells[2].cooldown = -1;

			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));

			_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
			_unit->m_noRespawn = true;

			Appear = true;
		}

		void OnCombatStart(Unit* mTarget)
		{
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void OnCombatStop(Unit* mTarget)
		{
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			//RemoveAIUpdateEvent();
		}

		void OnDied(Unit* mKiller)
		{
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			if(Appear)
			{
				_unit->CastSpell(_unit, spells[2].info, spells[2].instant);
				_unit->GetAIInterface()->SetAllowedToEnterCombat(true);

				Appear = false;
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
				for(int i = 0; i < nrspells; i++)
				{
					if(!spells[i].perctrigger) continue;

					if(m_spellcheck[i])
					{
						target = _unit->GetAIInterface()->getNextTarget();
						switch(spells[i].targettype)
						{
							case TARGET_SELF:
							case TARGET_VARIOUS:
								_unit->CastSpell(_unit, spells[i].info, spells[i].instant);
								break;
							case TARGET_ATTACKING:
								_unit->CastSpell(target, spells[i].info, spells[i].instant);
								break;
							case TARGET_DESTINATION:
								_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), spells[i].info, spells[i].instant);
								break;
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

	protected:

		bool Appear;
		int nrspells;
};


// Exarch MaladaarAI

#define CN_EXARCH_MALADAAR 18373

#define SOUL_SCREAM			32421
#define RIBBON_OF_SOULS		32422
#define STOLEN_SOUL			32346
#define SUMMON_AVATAR		32424
//#define SOUL_CLEAVE		32346

class EXARCHMALADAARAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(EXARCHMALADAARAI);
		SP_AI_Spell spells[4];
		bool m_spellcheck[4];

		EXARCHMALADAARAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 3;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(SOUL_SCREAM);
			spells[0].targettype = TARGET_VARIOUS;
			spells[0].instant = true;
			spells[0].perctrigger = 10.0f;
			spells[0].attackstoptimer = 1000;
			spells[0].cooldown = 15;
			spells[0].soundid = 10510;
			spells[0].speech = "Let your mind be clouded.";	// dunno for sure if it should be here, but still gives better effect of fight :)

			spells[1].info = dbcSpell.LookupEntry(RIBBON_OF_SOULS);
			spells[1].targettype = TARGET_RANDOM_SINGLE;
			spells[1].instant = false;
			spells[1].perctrigger = 15.0f;
			spells[1].attackstoptimer = 2000;
			spells[1].cooldown = 15;
			spells[1].mindist2cast = 0.0f;
			spells[1].maxdist2cast = 40.0f;
			spells[1].soundid = 10511;
			spells[1].speech = "Stare into the darkness of your soul!"; // not sure if it's really "stand"

			spells[2].info = dbcSpell.LookupEntry(STOLEN_SOUL);
			spells[2].targettype = TARGET_RANDOM_SINGLE;
			spells[2].instant = false;
			spells[2].perctrigger = 7.0f;
			spells[2].attackstoptimer = 1000;
			spells[2].cooldown = 15;
			spells[2].mindist2cast = 0.0f;
			spells[2].maxdist2cast = 40.0f;

			spells[3].info = dbcSpell.LookupEntry(SUMMON_AVATAR);
			spells[3].targettype = TARGET_SELF;
			spells[3].instant = false;
			spells[3].perctrigger = 0.0f;
			spells[3].attackstoptimer = 1000;
			spells[3].cooldown = -1;

			Avatar = false;
		}

		void OnCombatStart(Unit* mTarget)
		{
			for(int i = 0; i < 4; i++)
				spells[i].casttime = 0;

			Avatar = false;

			int RandomSpeach = rand() % 3;
			switch(RandomSpeach)
			{
				case 0:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You will pay with your life!");
					_unit->PlaySoundToSet(10513);
					break;
				case 1:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "There is no turning back now!");
					_unit->PlaySoundToSet(10514);
					break;
				case 2:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Serve your penitence!");
					_unit->PlaySoundToSet(10515);
					break;
			}

			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void OnTargetDied(Unit* mTarget)
		{
			if(_unit->GetHealthPct() > 0)	// Hack to prevent double yelling (OnDied and OnTargetDied when creature is dying)
			{
				int RandomSpeach = rand() % 2;
				switch(RandomSpeach)
				{
					case 0:
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "These wall will be your DOOM!");
						_unit->PlaySoundToSet(10516);
						break;
					case 1:
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Haha, now you'll stay for eternity! Mwahahah!"); // spelling check needed !
						_unit->PlaySoundToSet(10517);
						break;
				}
			}

		}

		void OnCombatStop(Unit* mTarget)
		{
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);

			RemoveAIUpdateEvent();

			Avatar = false;
		}

		void OnDied(Unit* mKiller)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "This is... Where... I belong...");
			_unit->PlaySoundToSet(10518);

			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			if(_unit->GetHealthPct() <= 25 && !Avatar && !_unit->IsStunned())
			{
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Rise my fallen brothers! Take form and fight!");
				_unit->PlaySoundToSet(10512);

				_unit->setAttackTimer(3500, false);
				_unit->GetAIInterface()->StopMovement(2000);

				_unit->CastSpell(_unit, spells[3].info, spells[3].instant);
				Avatar = true;
			}

			else
			{
				float val = RandomFloat(100.0f);
				SpellCast(val);
			}
		}

		void SpellCast(float val)
		{
			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
			{
				float comulativeperc = 0;
				Unit* target = NULL;
				for(int i = 0; i < nrspells; i++)
				{
					if(!spells[i].perctrigger) continue;

					if(m_spellcheck[i])
					{
						if(!spells[i].instant)
							_unit->GetAIInterface()->StopMovement(1);

						target = _unit->GetAIInterface()->getNextTarget();
						switch(spells[i].targettype)
						{
							case TARGET_SELF:
							case TARGET_VARIOUS:
								_unit->CastSpell(_unit, spells[i].info, spells[i].instant);
								break;
							case TARGET_ATTACKING:
								_unit->CastSpell(target, spells[i].info, spells[i].instant);
								break;
							case TARGET_DESTINATION:
								_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), spells[i].info, spells[i].instant);
								break;
							case TARGET_RANDOM_FRIEND:
							case TARGET_RANDOM_SINGLE:
							case TARGET_RANDOM_DESTINATION:
								CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast);
								break;
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
			if(!maxdist2cast) maxdist2cast = 100.0f;
			if(!maxhp2cast) maxhp2cast = 100;

			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
			{
				std::vector<Unit*> TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
				/* If anyone wants to use this function, then leave this note!										 */
				for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr)
				{
					if(((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && (*itr)->IsUnit())  // isAttackable(_unit, (*itr)) &&
					{
						Unit* RandomTarget = NULL;
						RandomTarget = TO_UNIT(*itr);

						if(RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast * mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast * maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
						{
							TargetTable.push_back(RandomTarget);
						}
					}
				}

				if(_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
					TargetTable.push_back(_unit);

				if(!TargetTable.size())
					return;

				size_t RandTarget = rand() % TargetTable.size();

				Unit*  RTarget = TargetTable[RandTarget];

				if(!RTarget)
					return;

				switch(spells[i].targettype)
				{
					case TARGET_RANDOM_FRIEND:
					case TARGET_RANDOM_SINGLE:
						_unit->CastSpell(RTarget, spells[i].info, spells[i].instant);
						break;
					case TARGET_RANDOM_DESTINATION:
						_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant);
						break;
				}

				TargetTable.clear();
			}
		}

	protected:

		bool Avatar;
		int nrspells;
};

void SetupAuchenaiCrypts(ScriptMgr* mgr)
{
	mgr->register_creature_script(CN_SHIRRAK_THE_DEAD_WATCHER, &SHIRRAKTHEDEADWATCHERAI::Create);
	mgr->register_creature_script(CN_AVATAR_OF_THE_MARTYRED, &AvatarOfTheMartyredAI::Create);
	mgr->register_creature_script(CN_EXARCH_MALADAAR, &EXARCHMALADAARAI::Create);
}