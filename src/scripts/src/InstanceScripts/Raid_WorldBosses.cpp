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
/* Raid_WorldBosses.cpp Script                                          */
/************************************************************************/

/********************************/
/*                                */
/*      EMERALD DREAM DRAGONS!    */
/*                                */
/********************************/


// Emeriss AI
// TODO:
// Check Putrid Mushroom
#define CN_EMERISS        14889

#define SLEEP                24777
#define NOXIOUS_BREATH            24818
#define TAIL_SWEEP            15847
#define MARK_OF_NATURE            25040   //If a player is killed by Emeriss, they will be afflicted by a 15 minute debuff called Mark of Nature. If resurrected during this time, they will be slept for 2 minutes rather than 4 seconds if they are hit with Sleep.
#define VOLATILE_INFECTION           24928
#define CORRUPTION_OF_EARTH              24910
#define PUTRID_MUSHROOM                24904 //31690 - Putrid mushroom //Summon Putrid Mushroom = 24904     //Despawn Putrid Mushroom = 24958
#define TELEPORT                15734

class EmerissAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(EmerissAI);
		SP_AI_Spell spells[7];
		bool m_spellcheck[7];

		EmerissAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 7;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(SLEEP);
			spells[0].targettype = TARGET_VARIOUS;
			spells[0].instant = true;
			spells[0].cooldown = -1;
			spells[0].perctrigger = 10.0f;
			spells[0].attackstoptimer = 1000;

			spells[1].info = dbcSpell.LookupEntry(NOXIOUS_BREATH);
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = false;
			spells[1].cooldown = -1;
			spells[1].perctrigger = 15.0f;
			spells[1].attackstoptimer = 2000;

			spells[2].info = dbcSpell.LookupEntry(TAIL_SWEEP);
			spells[2].targettype = TARGET_ATTACKING;        //if players are behind it's tail
			spells[2].instant = true;
			spells[2].cooldown = -1;
			spells[2].perctrigger = 7.0f;
			spells[2].attackstoptimer = 1000;

			spells[3].info = dbcSpell.LookupEntry(VOLATILE_INFECTION);
			spells[3].targettype = TARGET_VARIOUS;
			spells[3].instant = false;
			spells[3].cooldown = -1;
			spells[3].perctrigger = 10.0f;
			spells[3].attackstoptimer = 1000;

			spells[4].info = dbcSpell.LookupEntry(CORRUPTION_OF_EARTH);
			spells[4].targettype = TARGET_ATTACKING;
			spells[4].instant = true;
			spells[4].cooldown = -1;
			spells[4].perctrigger = 0.0f;
			spells[4].attackstoptimer = 1000;
			spells[4].speech = "Taste your world's corruption!";

			spells[5].info = dbcSpell.LookupEntry(PUTRID_MUSHROOM);
			spells[5].targettype = TARGET_DESTINATION;
			spells[5].instant = true;
			spells[5].cooldown = -1;
			spells[5].perctrigger = 0.0f;
			spells[5].attackstoptimer = 1000;

			spells[6].info = dbcSpell.LookupEntry(MARK_OF_NATURE);
			spells[6].targettype = TARGET_VARIOUS;
			spells[6].instant = true;
			spells[6].cooldown = -1;
			spells[6].perctrigger = 0.0f;
			spells[6].attackstoptimer = 1000;

		}

		void OnCombatStart(Unit* mTarget)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Hope is a DISEASE of the soul! This land shall wither and die!");
			RegisterAIUpdateEvent(1000); //Attack time is to slow on this boss
			CastTime();
		}

		void OnTargetDied(Unit* mTarget)
		{
			if(_unit->GetHealthPct() > 0)
			{
				_unit->CastSpell(mTarget, spells[6].info, spells[6].instant);
				_unit->CastSpellAoF(mTarget->GetPositionX(), mTarget->GetPositionY(), mTarget->GetPositionZ(), spells[5].info, spells[5].instant);
				//When a player dies a Putrid Mushroom spawns at their corpse. This deals 600 Nature damage per second to any surrounding player.
			}
		}

		void OnCombatStop(Unit* mTarget)
		{
			CastTime();
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
		}

		void CastTime()
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = spells[i].cooldown;
		}

		void OnDied(Unit* mKiller)
		{
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			// M4ksiu: Someone who wrote this hadn't thought about it much, so it should be rewritten
			Unit* Target = _unit->GetAIInterface()->getNextTarget();
			if(Target != NULL && !_unit->isInRange(Target, 20.0f))
				_unit->CastSpell(Target, TELEPORT, true);

			if(_unit->GetHealthPct() == 25 || _unit->GetHealthPct() == 50 || _unit->GetHealthPct() == 75)
			{
				_unit->CastSpell(_unit, spells[4].info, spells[4].instant);
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[4].speech.c_str());
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
					spells[i].casttime--;

					if(m_spellcheck[i])
					{
						spells[i].casttime = spells[i].cooldown;
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

						if(spells[i].speech != "")
						{
							_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
							_unit->PlaySoundToSet(spells[i].soundid);
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
};

// Taerar AI
#define CN_TAERAR        14890
#define CN_SHADESTAERAR 15302

#define SLEEP                24777
#define NOXIOUS_BREATH            24818
#define TAIL_SWEEP            15847
#define TELEPORT                15734
#define MARK_OF_NATURE            25040   //If a player is killed by Taerar, they will be afflicted by a 15 minute debuff called Mark of Nature. If resurrected during this time, they will be slept for 2 minutes rather than 4 seconds if they are hit with Sleep.
#define ARCANE_BLAST          24857
#define BELLOWING_ROAR        22686            //Mass fear
#define SUMMON_SHADE          24843

class TaerarAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(TaerarAI);
		SP_AI_Spell spells[7];
		bool m_spellcheck[7];

		TaerarAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 7;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(SLEEP);
			spells[0].targettype = TARGET_VARIOUS;
			spells[0].instant = true;
			spells[0].cooldown = -1;
			spells[0].perctrigger = 3.0f;
			spells[0].attackstoptimer = 1000;

			spells[1].info = dbcSpell.LookupEntry(NOXIOUS_BREATH);
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = false;
			spells[1].cooldown = -1;
			spells[1].perctrigger = 5.0f;
			spells[1].attackstoptimer = 2000;

			spells[2].info = dbcSpell.LookupEntry(TAIL_SWEEP);
			spells[2].targettype = TARGET_ATTACKING;        //if players are behind it's tail
			spells[2].instant = true;
			spells[2].cooldown = -1;
			spells[2].perctrigger = 3.0f;
			spells[2].attackstoptimer = 1000;

			spells[3].info = dbcSpell.LookupEntry(ARCANE_BLAST);
			spells[3].targettype = TARGET_VARIOUS;
			spells[3].instant = true;
			spells[3].cooldown = -1;
			spells[3].perctrigger = 3.0f;
			spells[3].attackstoptimer = 1000;

			spells[4].info = dbcSpell.LookupEntry(BELLOWING_ROAR);
			spells[4].targettype = TARGET_ATTACKING;
			spells[4].instant = true;
			spells[4].cooldown = -1;
			spells[4].perctrigger = 2.0f;
			spells[4].attackstoptimer = 1000;

			spells[5].info = dbcSpell.LookupEntry(SUMMON_SHADE);
			spells[5].targettype = TARGET_DESTINATION;
			spells[5].instant = true;
			spells[5].cooldown = -1;
			spells[5].perctrigger = 0.0f;
			spells[5].attackstoptimer = 1000;
			spells[5].speech = "Children of Madness - I release you upon this world!";

			spells[6].info = dbcSpell.LookupEntry(MARK_OF_NATURE);
			spells[6].targettype = TARGET_VARIOUS;
			spells[6].instant = true;
			spells[6].cooldown = -1;
			spells[6].perctrigger = 0.0f;
			spells[6].attackstoptimer = 1000;

		}

		void OnCombatStart(Unit* mTarget)
		{
			Shades = false;
			Shade_timer = 0;
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Peace is but a fleeting dream! Let the NIGHTMARE reign!");
			RegisterAIUpdateEvent(1000); //Attack time is to slow on this boss
			CastTime();
		}

		void OnTargetDied(Unit* mTarget)
		{
			if(_unit->GetHealthPct() > 0)
			{
				_unit->CastSpell(mTarget, spells[6].info, spells[6].instant);
			}
		}

		void OnCombatStop(Unit* mTarget)
		{
			Shades = false;
			Shade_timer = 0;
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
			CastTime();
		}

		void CastTime()
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = spells[i].cooldown;
		}

		void OnDied(Unit* mKiller)
		{
			RemoveAIUpdateEvent();
			Shades = false;
			Shade_timer = 0;
		}

		void SummonShades(Unit* mTarget)
		{
			Summoned = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_SHADESTAERAR, mTarget->GetPositionX(), mTarget->GetPositionY(), mTarget->GetPositionZ(), 0, true, false, _unit->GetFaction(), 50);
			Summoned->GetAIInterface()->setNextTarget(mTarget);
		}

		void AIUpdate()
		{
			// M4ksiu: Someone who wrote this hadn't thought about it much, so it should be rewritten
			Unit* Target = _unit->GetAIInterface()->getNextTarget();
			if(Target != NULL && !_unit->isInRange(Target, 20.0f))
				_unit->CastSpell(Target, TELEPORT, true);

			if(Shades && Shade_timer == 0)
			{
				//Become unbanished again
				_unit->SetFaction(14);
				//_unit->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
				_unit->GetAIInterface()->setNextTarget(_unit->GetAIInterface()->getNextTarget());
				Shades = false;
			}
			else if(Shades)
			{
				Shade_timer--;
				//Do nothing while banished
				return;
			}
			if(_unit->GetHealthPct() == 25 || _unit->GetHealthPct() == 50 || _unit->GetHealthPct() == 75)
			{
				//Inturrupt any spell casting
				_unit->InterruptSpell();
				//Root self
				_unit->CastSpell(_unit, 23973, true);
				_unit->SetFaction(35);
				//_unit->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

				//Summon shades
				SummonShades(_unit->GetAIInterface()->getNextTarget());
				SummonShades(_unit->GetAIInterface()->getNextTarget());
				SummonShades(_unit->GetAIInterface()->getNextTarget());
				Shades = true;
				Shade_timer = 60;
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[5].speech.c_str());
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
					spells[i].casttime--;

					if(m_spellcheck[i])
					{
						spells[i].casttime = spells[i].cooldown;
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

						if(spells[i].speech != "")
						{
							_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
							_unit->PlaySoundToSet(spells[i].soundid);
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

		Creature* Summoned;
		bool Shades;
		int Shade_timer;
		int nrspells;
};

// Shades of Taerar AI
#define CN_SHADEOFTAERAR 15302

#define POSION_CLOUD                24840
#define POSION_BREATH             20667

class ShadeofTaerarAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(ShadeofTaerarAI);
		SP_AI_Spell spells[2];
		bool m_spellcheck[2];

		ShadeofTaerarAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 2;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(POSION_CLOUD);
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = true;
			spells[0].cooldown = -1;
			spells[0].perctrigger = 18.0f;
			spells[0].attackstoptimer = 1000;

			spells[1].info = dbcSpell.LookupEntry(POSION_BREATH);
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = false;
			spells[1].cooldown = -1;
			spells[1].perctrigger = 16.0f;
			spells[1].attackstoptimer = 2000;

		}

		void OnCombatStart(Unit* mTarget)
		{
			CastTime();
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void OnTargetDied(Unit* mTarget)
		{
			//You died kek
		}

		void OnCombatStop(Unit* mTarget)
		{
			_unit->Despawn(15, 0);
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
			CastTime();
		}

		void CastTime()
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = spells[i].cooldown;
		}

		void OnDied(Unit* mKiller)
		{
			RemoveAIUpdateEvent();
			CastTime();
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
					spells[i].casttime--;

					if(m_spellcheck[i])
					{
						spells[i].casttime = spells[i].cooldown;
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

						if(spells[i].speech != "")
						{
							_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
							_unit->PlaySoundToSet(spells[i].soundid);
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
};



// Ysondre AI
#define CN_YSONDRE        14887
#define CN_YDRUIDS        15260

#define SLEEP                24777
#define NOXIOUS_BREATH            24818
#define TAIL_SWEEP            15847
#define TELEPORT                15734
#define MARK_OF_NATURE            25040   //If a player is killed by Ysondre, they will be afflicted by a 15 minute debuff called Mark of Nature. If resurrected during this time, they will be slept for 2 minutes rather than 4 seconds if they are hit with Sleep.
#define LIGHTNING_WAVE        24819
#define SUMMON_DRUIDS         24795 // Summon Demented Druid Spirit

class YsondreAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(YsondreAI);
		SP_AI_Spell spells[6];
		bool m_spellcheck[6];

		YsondreAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 6;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(SLEEP);
			spells[0].targettype = TARGET_VARIOUS;
			spells[0].instant = true;
			spells[0].cooldown = -1;
			spells[0].perctrigger = 3.0f;
			spells[0].attackstoptimer = 1000;

			spells[1].info = dbcSpell.LookupEntry(NOXIOUS_BREATH);
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = false;
			spells[1].cooldown = -1;
			spells[1].perctrigger = 5.0f;
			spells[1].attackstoptimer = 2000;

			spells[2].info = dbcSpell.LookupEntry(TAIL_SWEEP);
			spells[2].targettype = TARGET_ATTACKING;        //if players are behind it's tail
			spells[2].instant = true;
			spells[2].cooldown = -1;
			spells[2].perctrigger = 3.0f;
			spells[2].attackstoptimer = 1000;

			spells[3].info = dbcSpell.LookupEntry(LIGHTNING_WAVE);
			spells[3].targettype = TARGET_VARIOUS;
			spells[3].instant = true;
			spells[3].cooldown = -1;
			spells[3].perctrigger = 3.0f;
			spells[3].attackstoptimer = 1000;

			spells[4].info = dbcSpell.LookupEntry(SUMMON_DRUIDS);
			spells[4].targettype = TARGET_DESTINATION;
			spells[4].instant = true;
			spells[4].cooldown = -1;
			spells[4].perctrigger = 0.0f;
			spells[4].attackstoptimer = 1000;
			spells[4].speech = "Come forth, ye Dreamers - and claim your vengeance!";

			spells[5].info = dbcSpell.LookupEntry(MARK_OF_NATURE);
			spells[5].targettype = TARGET_VARIOUS;
			spells[5].instant = true;
			spells[5].cooldown = -1;
			spells[5].perctrigger = 0.0f;
			spells[5].attackstoptimer = 1000;

		}

		void OnCombatStart(Unit* mTarget)
		{
			CastTime();
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The strands of LIFE have been severed! The Dreamers must be avenged!");
			RegisterAIUpdateEvent(1000); //Attack time is to slow on this boss
		}

		void OnTargetDied(Unit* mTarget)
		{
			if(_unit->GetHealthPct() > 0)
			{
				_unit->CastSpell(mTarget, spells[5].info, spells[5].instant);
			}
		}

		void OnCombatStop(Unit* mTarget)
		{
			CastTime();
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
		}

		void CastTime()
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = spells[i].cooldown;
		}

		void OnDied(Unit* mKiller)
		{
			CastTime();
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			// M4ksiu: Someone who wrote this hadn't thought about it much, so it should be rewritten
			Unit* Target = _unit->GetAIInterface()->getNextTarget();
			if(Target != NULL && !_unit->isInRange(Target, 20.0f))
				_unit->CastSpell(Target, TELEPORT, true);

			if(_unit->GetHealthPct() == 25 || _unit->GetHealthPct() == 50 || _unit->GetHealthPct() == 75)
			{
				// Summon 6 druids
				_unit->CastSpellAoF(_unit->GetAIInterface()->getNextTarget()->GetPositionX(), _unit->GetAIInterface()->getNextTarget()->GetPositionY(), _unit->GetAIInterface()->getNextTarget()->GetPositionZ(), spells[4].info, spells[4].instant);
				_unit->CastSpellAoF(_unit->GetAIInterface()->getNextTarget()->GetPositionX(), _unit->GetAIInterface()->getNextTarget()->GetPositionY(), _unit->GetAIInterface()->getNextTarget()->GetPositionZ(), spells[4].info, spells[4].instant);
				_unit->CastSpellAoF(_unit->GetAIInterface()->getNextTarget()->GetPositionX(), _unit->GetAIInterface()->getNextTarget()->GetPositionY(), _unit->GetAIInterface()->getNextTarget()->GetPositionZ(), spells[4].info, spells[4].instant);
				_unit->CastSpellAoF(_unit->GetAIInterface()->getNextTarget()->GetPositionX(), _unit->GetAIInterface()->getNextTarget()->GetPositionY(), _unit->GetAIInterface()->getNextTarget()->GetPositionZ(), spells[4].info, spells[4].instant);
				_unit->CastSpellAoF(_unit->GetAIInterface()->getNextTarget()->GetPositionX(), _unit->GetAIInterface()->getNextTarget()->GetPositionY(), _unit->GetAIInterface()->getNextTarget()->GetPositionZ(), spells[4].info, spells[4].instant);
				_unit->CastSpellAoF(_unit->GetAIInterface()->getNextTarget()->GetPositionX(), _unit->GetAIInterface()->getNextTarget()->GetPositionY(), _unit->GetAIInterface()->getNextTarget()->GetPositionZ(), spells[4].info, spells[4].instant);

				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[4].speech.c_str());
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
					spells[i].casttime--;

					if(m_spellcheck[i])
					{
						spells[i].casttime = spells[i].cooldown;
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

						if(spells[i].speech != "")
						{
							_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
							_unit->PlaySoundToSet(spells[i].soundid);
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
};

// Demented Druid Spirit AI
#define CN_DEMENTEDDRUID    15260

#define MOONFIRE               27737

class DementedDruidSpiritAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(DementedDruidSpiritAI);
		SP_AI_Spell spells[1];
		bool m_spellcheck[1];

		DementedDruidSpiritAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 1;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(MOONFIRE);
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = true;
			spells[0].cooldown = -1;
			spells[0].perctrigger = 11.0f;
			spells[0].attackstoptimer = 1000;

		}

		void OnCombatStart(Unit* mTarget)
		{
			CastTime();
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void OnTargetDied(Unit* mTarget)
		{
			//You died kek
		}

		void OnCombatStop(Unit* mTarget)
		{
			CastTime();
			_unit->Despawn(15, 0);
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
		}

		void CastTime()
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = spells[i].cooldown;
		}

		void OnDied(Unit* mKiller)
		{
			CastTime();
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
					spells[i].casttime--;

					if(m_spellcheck[i])
					{
						spells[i].casttime = spells[i].cooldown;
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

						if(spells[i].speech != "")
						{
							_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
							_unit->PlaySoundToSet(spells[i].soundid);
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
};

// Lethon AI
#define CN_LETHON        14888

#define SLEEP                24777
#define NOXIOUS_BREATH            24818
#define TAIL_SWEEP            15847
#define TELEPORT                15734
#define MARK_OF_NATURE            25040   // If a player is killed by Lethon, they will be afflicted by a 15 minute debuff called Mark of Nature. If resurrected during this time, they will be slept for 2 minutes rather than 4 seconds if they are hit with Sleep.
#define SHADOW_WHIRL        24837
#define SUMMON_SHADES         24810

class LethonAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(LethonAI);
		SP_AI_Spell spells[6];
		bool m_spellcheck[6];

		LethonAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 6;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(SLEEP);
			spells[0].targettype = TARGET_VARIOUS;
			spells[0].instant = true;
			spells[0].cooldown = -1;
			spells[0].perctrigger = 3.0f;
			spells[0].attackstoptimer = 1000;

			spells[1].info = dbcSpell.LookupEntry(NOXIOUS_BREATH);
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = false;
			spells[1].cooldown = -1;
			spells[1].perctrigger = 5.0f;
			spells[1].attackstoptimer = 2000;

			spells[2].info = dbcSpell.LookupEntry(TAIL_SWEEP);
			spells[2].targettype = TARGET_ATTACKING;        //if players are behind it's tail
			spells[2].instant = true;
			spells[2].cooldown = -1;
			spells[2].perctrigger = 3.0f;
			spells[2].attackstoptimer = 1000;

			spells[3].info = dbcSpell.LookupEntry(SHADOW_WHIRL);
			spells[3].targettype = TARGET_ATTACKING;
			spells[3].instant = true;
			spells[3].cooldown = -1;
			spells[3].perctrigger = 2.0f;
			spells[3].attackstoptimer = 1000;

			spells[4].info = dbcSpell.LookupEntry(SUMMON_SHADES);
			spells[4].targettype = TARGET_SELF;
			spells[4].instant = true;
			spells[4].cooldown = -1;
			spells[4].perctrigger = 0.0f;
			spells[4].attackstoptimer = 1000;
			spells[4].speech = "Your wicked souls shall feed my power!";

			spells[5].info = dbcSpell.LookupEntry(MARK_OF_NATURE);
			spells[5].targettype = TARGET_VARIOUS;
			spells[5].instant = true;
			spells[5].cooldown = -1;
			spells[5].perctrigger = 0.0f;
			spells[5].attackstoptimer = 1000;
		}

		void OnCombatStart(Unit* mTarget)
		{
			Shade1 = false;
			Shade2 = false;
			Shade3 = false;
			CastTime();
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I can sense the SHADOW on your hearts. There can be no rest for the wicked!");
			RegisterAIUpdateEvent(1000); //Attack time is to slow on this boss
		}

		void OnTargetDied(Unit* mTarget)
		{
			if(_unit->GetHealthPct() > 0)
			{
				_unit->CastSpell(mTarget, spells[5].info, spells[5].instant); //Mark of nature
			}
		}

		void OnCombatStop(Unit* mTarget)
		{
			Shade1 = false;
			Shade2 = false;
			Shade3 = false;
			CastTime();
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
		}

		void CastTime()
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = spells[i].cooldown;
		}

		void OnDied(Unit* mKiller)
		{
			Shade1 = false;
			Shade2 = false;
			Shade3 = false;
			CastTime();
			RemoveAIUpdateEvent();
		}

		void AIUpdate()

		{
			std::list<Player*> mTargets;
			// M4ksiu: Someone who wrote this hadn't thought about it much, so it should be rewritten
			Unit* Target = _unit->GetAIInterface()->getNextTarget();
			if(Target != NULL && !_unit->isInRange(Target, 20.0f))
				_unit->CastSpell(Target, TELEPORT, true);

			if((_unit->GetHealthPct() == 25 && Shade3 == false) || (_unit->GetHealthPct() == 50 && Shade2 == false) || (_unit->GetHealthPct() == 75 && Shade1 == false))
			{
				//Made it like this because if lethon gets healed, he should spawn the adds again at the same pct. (Only spawn once at 75,50,25)
				switch(_unit->GetHealthPct())
				{
					case 25:
						Shade3 = true;
					case 50:
						Shade2 = true;
					case 75:
						Shade1 = true;
				}
				// Summon a spirit for each player
				std::list<Player*>::iterator itr = mTargets.begin();
				for(; itr != mTargets.end(); ++itr)
				{
					_unit->CastSpellAoF((*itr)->GetPositionX(), (*itr)->GetPositionY(), (*itr)->GetPositionZ(), spells[4].info, spells[4].instant);
				}
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[4].speech.c_str());
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
					spells[i].casttime--;

					if(m_spellcheck[i])
					{
						spells[i].casttime = spells[i].cooldown;
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

						if(spells[i].speech != "")
						{
							_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
							_unit->PlaySoundToSet(spells[i].soundid);
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

		bool Shade1; //75%
		bool Shade2; //50%
		bool Shade3; //25%
		int nrspells;
};


//Shades of Lethon

#define CN_LSHADE        15261

class ShadeofLethonAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(ShadeofLethonAI);

		ShadeofLethonAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{

		}

		void OnCombatStart(Unit* mTarget)
		{
			CheckDist();
			RegisterAIUpdateEvent(1000); //they cant attack anyway, update every sec instead
		}

		void OnTargetDied(Unit* mTarget)
		{
			//Will nevah happenz! haha
		}

		void OnCombatStop(Unit* mTarget)
		{
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
			_unit->Despawn(15, 0);
		}

		void OnDied(Unit* mKiller)
		{
			RemoveAIUpdateEvent();
		}

		void CheckDist()
		{
			float distance;
			std::list<Creature*> mTargets;
			std::list<Creature*>::iterator itr = mTargets.begin();
			for(; itr != mTargets.end(); ++itr)
			{
				if((*itr)->GetGUID() == CN_LETHON)
				{
					distance = (*itr)->GetDistanceSq((*itr)->GetPositionX(), (*itr)->GetPositionY(), (*itr)->GetPositionZ());
					if(distance < 5.0)
					{
						(*itr)->SetHealth(((*itr)->GetUInt32Value(UNIT_FIELD_MAXHEALTH) / 100)); //Heal him 1%
//                    if((*itr)->GetUInt32Value(UNIT_FIELD_HEALTH) > (*itr)->GetUInt32Value(UNIT_FIELD_MAXHEALTH))
//                        (*itr)->SetUInt32Value(UNIT_FIELD_HEALTH, (*itr)->GetUInt32Value(UNIT_FIELD_MAXHEALTH)); //Do i need to do this....?
						_unit->Despawn(1, 0);
					}
					else
						_unit->GetAIInterface()->_CalcDestinationAndMove((*itr), distance);
				}
				else
					OnCombatStop(_unit);
			}
		}

		void AIUpdate()
		{
			//Repeat this, if they move Lethon while the ghosts move, they need to update his position
			CheckDist();
		}

};



/***************************************/
/*                                       */
/*           Rest of World Bosses           */
/*                                       */
/***************************************/



// Highlord Kruul
#define CN_KRUUL    18338
#define CN_HOUNDS    19207

#define SHADOW_VOLLEY                21341
#define CLEAVE           20677
#define THUNDER_CLAP            23931
#define TWISTED_REFLECTION            21063
#define VOID_BOLT            21066
#define RAGE           21340
#define CAPTURE_SOUL               21053

class KruulAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(KruulAI);
		SP_AI_Spell spells[7];
		bool m_spellcheck[7];

		KruulAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 7;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(SHADOW_VOLLEY);
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = true;
			spells[0].cooldown = -1;
			spells[0].perctrigger = 5.0f;
			spells[0].attackstoptimer = 1000;

			spells[1].info = dbcSpell.LookupEntry(CLEAVE);
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = false;
			spells[1].cooldown = -1;
			spells[1].perctrigger = 12.0f;
			spells[1].attackstoptimer = 1000;

			spells[2].info = dbcSpell.LookupEntry(THUNDER_CLAP);
			spells[2].targettype = TARGET_ATTACKING;
			spells[2].instant = true;
			spells[2].cooldown = -1;
			spells[2].perctrigger = 10.0f;
			spells[2].attackstoptimer = 1000;

			spells[3].info = dbcSpell.LookupEntry(TWISTED_REFLECTION);
			spells[3].targettype = TARGET_ATTACKING;
			spells[3].instant = true;
			spells[3].cooldown = -1;
			spells[3].perctrigger = 7.0f;
			spells[3].attackstoptimer = 1000;

			spells[4].info = dbcSpell.LookupEntry(VOID_BOLT);
			spells[4].targettype = TARGET_ATTACKING;
			spells[4].instant = false;
			spells[4].cooldown = -1;
			spells[4].perctrigger = 5.0f;
			spells[4].attackstoptimer = 1000;

			spells[5].info = dbcSpell.LookupEntry(CAPTURE_SOUL);
			spells[5].targettype = TARGET_VARIOUS;
			spells[5].instant = true;
			spells[5].cooldown = -1;
			spells[5].perctrigger = 0.0f;
			spells[5].attackstoptimer = 1000;

			spells[6].info = dbcSpell.LookupEntry(RAGE);
			spells[6].targettype = TARGET_SELF;
			spells[6].instant = true;
			spells[6].cooldown = -1;
			spells[6].perctrigger = 0.0f;
			spells[6].attackstoptimer = 1000;

		}

		void OnCombatStart(Unit* mTarget)
		{
			hounds_timer = 45;
			enrage = 0;
			int RandomSpeach;
			RandomSpeach = rand() % 5;
			switch(RandomSpeach)
			{
				case 0:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Azeroth has cowered too long under our shadow! Now, feel the power of the Burning Crusade, and despair!");
					break;
				case 1:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Your fate is sealed, Azeroth! I will find the Aspect Shards, and then you will not stand against our might!");
					break;
				case 2:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Cower, little worms! Your heroes are nothing! Your saviors will be our first feast!");
					break;
				case 3:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Where? Where are the Shards! You cannot hide them from us!");
					break;
				case 4:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Your world will die, mortals! Your doom is now at hand!");
					break;
			}
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
			CastTime();
		}

		void OnTargetDied(Unit* mTarget)
		{
			if(_unit->GetHealthPct() > 0)
			{
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Your own strength feeds me, $N!");
				_unit->CastSpell(_unit, spells[5].info, spells[5].instant); // Either himself or target? :P
			}
		}

		void OnCombatStop(Unit* mTarget)
		{
			hounds_timer = 45;
			enrage = 0;
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
			CastTime();
		}

		void CastTime()
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = spells[i].cooldown;
		}

		void OnDied(Unit* mKiller)
		{
			hounds_timer = 45;
			enrage = 0;
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Ha! This place is not yet worthy of my infliction.");
			RemoveAIUpdateEvent();
			CastTime();
		}

		void SummonHounds(Unit* mTarget)
		{
			Rand = rand() % 15;
			switch(rand() % 2)
			{
				case 0:
					RandX = 0 - Rand;
					break;
				case 1:
					RandX = 0 + Rand;
					break;
			}

			Rand = rand() % 15;
			switch(rand() % 2)
			{
				case 0:
					RandY = 0 - Rand;
					break;
				case 1:
					RandY = 0 + Rand;
					break;
			}
			Rand = 0;
			Summoned = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_HOUNDS, (float)RandX, (float)RandY, 0, 0, true, false, _unit->GetFaction(), 50);
			Summoned->GetAIInterface()->setNextTarget(mTarget);
		}

		void AIUpdate()
		{
			if(hounds_timer == 0)
			{
				SummonHounds(_unit->GetAIInterface()->getNextTarget());
				SummonHounds(_unit->GetAIInterface()->getNextTarget());
				SummonHounds(_unit->GetAIInterface()->getNextTarget());
				hounds_timer = 45;
			}
			else
				hounds_timer--;

			if(enrage == 60)
				_unit->CastSpell(_unit, spells[6].info, spells[6].instant);
			else
			{
				enrage++;
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
					spells[i].casttime--;

					if(m_spellcheck[i])
					{
						spells[i].casttime = spells[i].cooldown;
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

						if(spells[i].speech != "")
						{
							_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
							_unit->PlaySoundToSet(spells[i].soundid);
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

		int Rand;
		int RandX;
		int RandY;
		int hounds_timer;
		int enrage;
		Creature* Summoned;
		int nrspells;
};

// Doom Lord Kazzak (Lord Kazzak does not exist anymore, he is promoted and replaced by Highlord Kruul instead)
// TODO:
// Death Messages. Both himself and player needs.
/*
Remaining:
11338,10,"A_GRULLAIR_Kazzak_Slay02" (443956),"GRULLAIR_Kazzak_Slay02.wav"
11340,10,"A_GRULLAIR_Kazzak_Death01" (444060),"GRULLAIR_Kazzak_Death01.wav"
*/

#define CN_KAZZAK    18728

#define SHADOW_VOLLEY                21341
#define CLEAVE           20677
#define THUNDER_CLAP            23931
#define TWISTED_REFLECTION            21063
#define VOID_BOLT            21066
#define RAGE           21340
#define CAPTURE_SOUL               21053
#define MARK_OF_KAZZAK            21056

class KazzakAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(KazzakAI);
		SP_AI_Spell spells[8];
		bool m_spellcheck[8];

		KazzakAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 8;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(SHADOW_VOLLEY);
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = true;
			spells[0].cooldown = -1;
			spells[0].perctrigger = 5.0f;
			spells[0].attackstoptimer = 1000;

			spells[1].info = dbcSpell.LookupEntry(CLEAVE);
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = false;
			spells[1].cooldown = -1;
			spells[1].perctrigger = 12.0f;
			spells[1].attackstoptimer = 1000;

			spells[2].info = dbcSpell.LookupEntry(THUNDER_CLAP);
			spells[2].targettype = TARGET_ATTACKING;
			spells[2].instant = true;
			spells[2].cooldown = -1;
			spells[2].perctrigger = 10.0f;
			spells[2].attackstoptimer = 1000;

			spells[3].info = dbcSpell.LookupEntry(TWISTED_REFLECTION);
			spells[3].targettype = TARGET_ATTACKING;
			spells[3].instant = true;
			spells[3].cooldown = -1;
			spells[3].perctrigger = 7.0f;
			spells[3].attackstoptimer = 1000;

			spells[4].info = dbcSpell.LookupEntry(VOID_BOLT);
			spells[4].targettype = TARGET_ATTACKING;
			spells[4].instant = false;
			spells[4].cooldown = -1;
			spells[4].perctrigger = 5.0f;
			spells[4].attackstoptimer = 1000;

			spells[5].info = dbcSpell.LookupEntry(MARK_OF_KAZZAK);
			spells[5].targettype = TARGET_VARIOUS;
			spells[5].instant = true;
			spells[5].cooldown = -1;
			spells[5].perctrigger = 5.0f;
			spells[5].attackstoptimer = 1000;

			spells[6].info = dbcSpell.LookupEntry(CAPTURE_SOUL);
			spells[6].targettype = TARGET_DESTINATION;
			spells[6].instant = true;
			spells[6].cooldown = -1;
			spells[6].perctrigger = 0.0f;
			spells[6].attackstoptimer = 1000;

			spells[7].info = dbcSpell.LookupEntry(RAGE);
			spells[7].targettype = TARGET_SELF;
			spells[7].instant = true;
			spells[7].cooldown = -1;
			spells[7].perctrigger = 0.0f;
			spells[7].attackstoptimer = 1000;

			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
			//Spawn intro.
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I remember well the sting of defeat at the conclusion of the Third War. I have waited far too long for my revenge. Now the shadow of the Legion falls over this world. It is only a matter of time until all of your failed creation... is undone.");
			_unit->PlaySoundToSet(11332);

		}

		void OnCombatStart(Unit* mTarget)
		{
			enrage = 0;
			int RandomSpeach;
			RandomSpeach = rand() % 2;
			switch(RandomSpeach)
			{
				case 0:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "All mortals will perish!");
					_unit->PlaySoundToSet(11333);
					break;
				case 1:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The Legion will conquer all!");
					_unit->PlaySoundToSet(11334);
					break;
			}
			CastTime();
		}

		void OnTargetDied(Unit* mTarget)
		{
			if(_unit->GetHealthPct() > 0)
			{
				int RandomSpeach;
				RandomSpeach = rand() % 2;
				switch(RandomSpeach)
				{
					case 0:
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Your own strength feeds me, $N!");
						_unit->PlaySoundToSet(11337);
						break;
					case 1:
						//    _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "");
						_unit->PlaySoundToSet(11338);
						break;
				}
				_unit->CastSpell(_unit, spells[6].info, spells[6].instant);
			}
		}

		void OnCombatStop(Unit* mTarget)
		{
			enrage = 0;
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The universe will be remade.");
			_unit->PlaySoundToSet(11339);
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			CastTime();
		}

		void CastTime()
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = spells[i].cooldown;
		}

		void OnDied(Unit* mKiller)
		{
			enrage = 0;
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "");
			_unit->PlaySoundToSet(11340);
			RemoveAIUpdateEvent();
			CastTime();
		}

		void RandomSpeech()
		{
			int RandomSpeach;
			RandomSpeach = rand() % 20; // 10% chance should do, he talks a lot tbh =P
			switch(RandomSpeach)
			{
				case 0:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Invaders, you dangle upon the precipice of oblivion! The Burning Legion comes and with it comes your end.");
					_unit->PlaySoundToSet(11335);
					break;
				case 1:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Kirel Narak!");
					_unit->PlaySoundToSet(11336);
					break;
			}
		}

		void AIUpdate()
		{
			if(_unit->CombatStatus.IsInCombat())
			{
				if(enrage == 180)
				{
					_unit->CastSpell(_unit, spells[7].info, spells[7].instant);
					enrage = 0;
				}
				else
				{
					enrage++;
					float val = RandomFloat(100.0f);
					SpellCast(val);
				}
			}
			else
			{
				RandomSpeech();        //awesome idea m4ksui
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
					spells[i].casttime--;

					if(m_spellcheck[i])
					{
						spells[i].casttime = spells[i].cooldown;
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

						if(spells[i].speech != "")
						{
							_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
							_unit->PlaySoundToSet(spells[i].soundid);
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

		int enrage;
		int nrspells;
};

// Azuregos

#define CN_AZUREGOS        6109

#define MARK_OF_FROST                23183
#define MANA_STORM            21097
#define REFLECT            30969               //Might not be the right, this one doesn't work on dots?
#define ACLEAVE           8255                //This is Strong Cleave, maybe it should be 27794, normal cleave 250+ damage
#define CONE_OF_COLD    30095
#define MASS_TELEPORT            16807

class AzuregosAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(AzuregosAI);
		SP_AI_Spell spells[5];
		bool m_spellcheck[5];

		AzuregosAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 5;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(MANA_STORM);
			spells[0].targettype = TARGET_VARIOUS;
			spells[0].instant = false;
			spells[0].cooldown = -1;
			spells[0].perctrigger = 7.0f;
			spells[0].attackstoptimer = 1000;

			spells[1].info = dbcSpell.LookupEntry(ACLEAVE);
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = false;
			spells[1].cooldown = -1;
			spells[1].perctrigger = 15.0f;
			spells[1].attackstoptimer = 1000;

			spells[2].info = dbcSpell.LookupEntry(CONE_OF_COLD);
			spells[2].targettype = TARGET_ATTACKING;
			spells[2].instant = false;
			spells[2].cooldown = -1;
			spells[2].perctrigger = 10.0f;
			spells[2].attackstoptimer = 1000;

			spells[3].info = dbcSpell.LookupEntry(REFLECT);
			spells[3].targettype = TARGET_SELF;
			spells[3].instant = true;
			spells[3].casttime = 45;
			spells[3].cooldown = 45;
			spells[3].perctrigger = 0.0f;
			spells[3].attackstoptimer = 1000;

			spells[4].info = dbcSpell.LookupEntry(MARK_OF_FROST);
			spells[4].targettype = TARGET_DESTINATION;
			spells[4].instant = true;
			spells[4].cooldown = -1;
			spells[4].perctrigger = 0.0f;
			spells[4].attackstoptimer = 1000;

		}

		void OnCombatStart(Unit* mTarget)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "This Place is under my Protection! The mysteries of the arcane shall remain untouched.");
			masstele = 60;
			RegisterAIUpdateEvent(1000);
			CastTime();
		}

		void OnTargetDied(Unit* mTarget)
		{
			if(_unit->GetHealthPct() > 0)
			{
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "So is the price of curiosity.");
				_unit->CastSpell(mTarget, spells[4].info, spells[4].instant);
			}
		}

		void OnCombatStop(Unit* mTarget)
		{
			masstele = 60;
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
			CastTime();
		}

		void OnDied(Unit* mKiller)
		{
			masstele = 60;
			RemoveAIUpdateEvent();
			CastTime();
		}

		void CastTime()
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = spells[i].cooldown;
		}


		void AIUpdate()
		{
			if(masstele == 0)
			{

				_unit->CastSpell(_unit->GetAIInterface()->getNextTarget(), MASS_TELEPORT, true);
				_unit->GetAIInterface()->WipeHateList();
				masstele = 60;
			}
			else
			{
				masstele--;
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
					spells[i].casttime--;

					if(m_spellcheck[i])
					{
						spells[i].casttime = spells[i].cooldown;
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

						if(spells[i].speech != "")
						{
							_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
							_unit->PlaySoundToSet(spells[i].soundid);
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

		int masstele;
		int nrspells;
};

// Doomwalker

#define CN_DOOMWALKER        17711

#define EARTHQUAKE                 32686
#define MARK_OF_DEATH           37128
#define CHAIN_LIGHTNING            33665
#define OVERRUN                    32636
#define ENRAGE                33653
#define AURA_OF_DEATH        37131
#define SUNDER_ARMOR          33661

class DoomwalkerAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(DoomwalkerAI);
		SP_AI_Spell spells[6];
		bool m_spellcheck[6];

		DoomwalkerAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 6;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(EARTHQUAKE);
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = true;
			spells[0].cooldown = 55;
			spells[0].casttime = 55;
			spells[0].perctrigger = 0.0f;
			spells[0].attackstoptimer = 1000;

			spells[1].info = dbcSpell.LookupEntry(MARK_OF_DEATH);
			spells[1].targettype = TARGET_DESTINATION;
			spells[1].instant = false;
			spells[1].cooldown = -1;
			spells[1].perctrigger = 0.0f;
			spells[1].attackstoptimer = 1000;

			spells[2].info = dbcSpell.LookupEntry(CHAIN_LIGHTNING);
			spells[2].targettype = TARGET_VARIOUS;
			spells[2].instant = false;
			spells[2].cooldown = -1;
			spells[2].perctrigger = 8.0f;
			spells[2].attackstoptimer = 1000;

			spells[3].info = dbcSpell.LookupEntry(OVERRUN);
			spells[3].targettype = TARGET_VARIOUS;
			spells[3].instant = true;
			spells[3].casttime = 45;
			spells[3].cooldown = 45;
			spells[3].perctrigger = 0.0f;
			spells[3].attackstoptimer = 1000;

			spells[4].info = dbcSpell.LookupEntry(ENRAGE);
			spells[4].targettype = TARGET_SELF;
			spells[4].instant = true;
			spells[4].perctrigger = 0.0f;
			spells[4].attackstoptimer = 1000;

			spells[5].info = dbcSpell.LookupEntry(SUNDER_ARMOR);
			spells[5].targettype = TARGET_ATTACKING;
			spells[5].instant = true;
			spells[5].cooldown = 15;
			spells[5].perctrigger = 0.0f;
			spells[5].attackstoptimer = 1000;

		}

		void OnCombatStart(Unit* mTarget)
		{
			enraged = false;
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Do not proceed. You will be eliminated.");
			_unit->PlaySoundToSet(11344);
			RegisterAIUpdateEvent(1000);
			CastTime();
		}

		void OnTargetDied(Unit* mTarget)
		{
			if(_unit->GetHealthPct() > 0)
			{
				int RandomSpeach;
				RandomSpeach = rand() % 3;
				switch(RandomSpeach)
				{
					case 0:
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Threat level zero.");
						_unit->PlaySoundToSet(11349);
						break;
					case 1:
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Directive accomplished.");
						_unit->PlaySoundToSet(11350);
						break;
					case 2:
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Target exterminated.");
						_unit->PlaySoundToSet(11351);
						break;
				}
				_unit->CastSpell(mTarget, spells[1].info, spells[1].instant);
			}
		}
		void OnCombatStop(Unit* mTarget)
		{
			_unit->RemoveAura(AURA_OF_DEATH);
			enraged = false;
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
			CastTime();
		}

		void OnDied(Unit* mKiller)
		{
			_unit->RemoveAura(AURA_OF_DEATH);
			enraged = false;
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "System failure in five... four...");
			_unit->PlaySoundToSet(11352);
			RemoveAIUpdateEvent();
			CastTime();
		}

		void CastTime()
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = spells[i].cooldown;
		}


		void AIUpdate()
		{
			if(_unit->GetHealthPct() == 20 && enraged == false)  //if he stays to long on 20% it could double activate without this check?
			{
				_unit->CastSpell(_unit, spells[4].info, spells[4].instant);
				enraged = true;
			}
			//_unit->CastSpell(_unit, AURA_OF_DEATH, true); //Repulse this every AIUpdate :) Spell is bugged atm, it also kills him methinks, not only those with Mark of Death
			float val = RandomFloat(100.0f);
			SpellCast(val);
		}

		void SpellCast(float val)
		{
			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
			{
				float comulativeperc = 0;
				Unit* target = NULL;
				int RandomSpeach;
				RandomSpeach = rand() % 2;
				for(int i = 0; i < nrspells; i++)
				{
					spells[i].casttime--;

					if(m_spellcheck[i])
					{
						spells[i].casttime = spells[i].cooldown;
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

						if(spells[i].speech != "")
						{
							_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
							_unit->PlaySoundToSet(spells[i].soundid);
						}
						if(m_spellcheck[0] == true)  //Earthquake
						{
							_unit->GetAIInterface()->WipeHateList();
							switch(RandomSpeach)
							{
								case 0:
									_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Tectonic disruption commencing.");
									_unit->PlaySoundToSet(11345);
									break;
								case 1:
									_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Magnitude set. Release. ");
									_unit->PlaySoundToSet(11346);
									break;
							}
						}
						if(m_spellcheck[3] == true)  //Overrun
						{
							_unit->GetAIInterface()->WipeHateList();
							switch(RandomSpeach)
							{
								case 0:
									_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Trajectory locked.");
									_unit->PlaySoundToSet(11347);
									break;
								case 1:
									_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Engage maximum speed.");
									_unit->PlaySoundToSet(11348);
									break;
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

		bool enraged; //Just to make sure
		int nrspells;
};

// Teremus The Devourer

#define CN_TEREMUS        7846

#define FLAME_BREATH                20712
#define SOUL_CONSUMPTION             12667
#define RESIST_ALL            18114

class TeremusAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(TeremusAI);
		SP_AI_Spell spells[3];
		bool m_spellcheck[3];

		TeremusAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 3;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(FLAME_BREATH);
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = true;
			spells[0].perctrigger = 10.0f;
			spells[0].attackstoptimer = 1000;

			spells[1].info = dbcSpell.LookupEntry(SOUL_CONSUMPTION);
			spells[1].targettype = TARGET_DESTINATION;
			spells[1].instant = false;
			spells[1].perctrigger = 9.0f;
			spells[1].attackstoptimer = 1000;

			spells[2].info = dbcSpell.LookupEntry(RESIST_ALL);
			spells[2].targettype = TARGET_VARIOUS;
			spells[2].instant = false;
			spells[2].perctrigger = 5.0f;
			spells[2].attackstoptimer = 1000;

		}

		void OnCombatStart(Unit* mTarget)
		{
			RegisterAIUpdateEvent(1000);
			CastTime();
		}

		void OnTargetDied(Unit* mTarget)
		{

		}
		void OnCombatStop(Unit* mTarget)
		{
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
			CastTime();
		}

		void OnDied(Unit* mKiller)
		{
			RemoveAIUpdateEvent();
			CastTime();
		}

		void CastTime()
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = spells[i].cooldown;
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
				int RandomSpeach;
				RandomSpeach = rand() % 2;
				for(int i = 0; i < nrspells; i++)
				{
					spells[i].casttime--;

					if(m_spellcheck[i])
					{
						spells[i].casttime = spells[i].cooldown;
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

						if(spells[i].speech != "")
						{
							_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
							_unit->PlaySoundToSet(spells[i].soundid);
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
};

void SetupWorldBosses(ScriptMgr* mgr)
{
	mgr->register_creature_script(CN_EMERISS, &EmerissAI::Create);
	mgr->register_creature_script(CN_TAERAR, &TaerarAI::Create);
	mgr->register_creature_script(CN_SHADEOFTAERAR, &ShadeofTaerarAI::Create);
	mgr->register_creature_script(CN_YSONDRE, &YsondreAI::Create);
	mgr->register_creature_script(CN_DEMENTEDDRUID, &DementedDruidSpiritAI::Create);
	mgr->register_creature_script(CN_LETHON, &LethonAI::Create);
	mgr->register_creature_script(CN_LSHADE, &ShadeofLethonAI::Create);
	mgr->register_creature_script(CN_KRUUL, &KruulAI::Create);
	mgr->register_creature_script(CN_KAZZAK, &KazzakAI::Create);
	mgr->register_creature_script(CN_AZUREGOS, &AzuregosAI::Create);
	mgr->register_creature_script(CN_DOOMWALKER, &DoomwalkerAI::Create);
	mgr->register_creature_script(CN_TEREMUS, &TeremusAI::Create);
}
