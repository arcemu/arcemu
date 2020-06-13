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
/* Raid_ZulGurub.cpp Script												*/
/************************************************************************/

// High Priestess Jeklik AI
#define CN_JEKLIK 14517

#define TRANSFORM_BAT 23966 // TODO: MISSING CREATURE_NAME TO TRANSFORM
#define CRUSHING_BLOW 24257
#define CRUSHING_AOE_SILENCE 24687

#define MIND_FLAY 23953
#define SUMMON_BATS 23974 // TODO EFFECT :P
#define SHADOW_WORD_PAIN 24212
#define GREAT_HEAL 29564

// agro/transform sound -> 8417

class JeklikAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(JeklikAI);
		SP_AI_Spell spells[6];
		bool m_spellcheck[6];

		JeklikAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			// -- Number of spells to add --
			nrspells = 6;

			// --- Initialization ---
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}
			// ----------------------

			// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not!
			spells[0].info = dbcSpell.LookupEntry(TRANSFORM_BAT);
			spells[0].targettype = TARGET_SELF;
			spells[0].instant = true;
			spells[0].perctrigger = 0.0f;
			spells[0].attackstoptimer = 1000; // 1sec
			m_spellcheck[0] = true;

			spells[1].info = dbcSpell.LookupEntry(CRUSHING_BLOW);
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = false;
			spells[1].perctrigger = 10.0f;
			spells[1].attackstoptimer = 2000; // 1sec

			spells[2].info = dbcSpell.LookupEntry(CRUSHING_AOE_SILENCE);
			spells[2].targettype = TARGET_VARIOUS;
			spells[2].instant = false;
			spells[2].perctrigger = 10.0f;
			spells[2].attackstoptimer = 2000; // 1sec

			// 2 phase spells
			spells[3].info = dbcSpell.LookupEntry(MIND_FLAY);
			spells[3].targettype = TARGET_ATTACKING;
			spells[3].instant = false;
			spells[3].perctrigger = 10.0f;
			spells[3].attackstoptimer = 6000; // 1sec

			spells[4].info = dbcSpell.LookupEntry(SHADOW_WORD_PAIN);
			spells[4].targettype = TARGET_ATTACKING;
			spells[4].instant = false;
			spells[4].perctrigger = 10.0f;
			spells[4].attackstoptimer = 2000; // 1sec

			spells[5].info = dbcSpell.LookupEntry(GREAT_HEAL);
			spells[5].targettype = TARGET_SELF;
			spells[5].instant = false;
			spells[5].perctrigger = 10.0f;
			spells[5].attackstoptimer = 10000; // 1sec

		}

		void OnCombatStart(Unit* mTarget)
		{
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
			// bat transform
			_unit->CastSpell(_unit, spells[0].info, spells[0].instant);
			_unit->PlaySoundToSet(8417);
		}

		void OnCombatStop(Unit* mTarget)
		{
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
			_unit->RemoveAura(TRANSFORM_BAT);
		}

		void OnDied(Unit* mKiller)
		{
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			if(_unit->GetHealthPct() <= 50 && m_spellcheck[0])
			{
				m_spellcheck[0] = false;
				_unit->RemoveAura(TRANSFORM_BAT);
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

					// do another set of spells on transform
					if(_unit->GetHealthPct() <= 50 && i < 3) continue;

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

/*
Jeklik begins the encounter in bat form. In this form she has an AoE silence that also damages.
She randomly charges people in the group, and summons 6-8 bloodseeker bats once per minute.
When she drops below 50% HP, she reverts to priest form. Here she casts Shadow Word: Pain, Mind Flay, Chain Mind Flay and Greater Heal.
She also summons bomb bats which drop fire bombs on the ground which AOE DoT those inside.
¨*/

// High Priestess Venoxis AI

#define CN_VENOXIS 14507 // TODO: MISSING CREATURE_NAME TO TRANSFORM

#define HOLY_NOVA 23858 // various targets
#define HOLY_FIRE 23860 // various targets

#define TRANSFORM_SNAKE 23849
#define SPIT_POISON  24688 // various targets

class VenoxisAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(VenoxisAI);
		SP_AI_Spell spells[4];
		bool m_spellcheck[4];

		VenoxisAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			// -- Number of spells to add --
			nrspells = 4;

			// --- Initialization ---
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}
			// ----------------------

			// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not!

			spells[0].info = dbcSpell.LookupEntry(TRANSFORM_SNAKE);
			spells[0].targettype = TARGET_SELF;
			spells[0].instant = false;
			spells[0].perctrigger = 0.0f;
			spells[0].attackstoptimer = 1000; // 1sec
			m_spellcheck[0] = true;

			spells[1].info = dbcSpell.LookupEntry(HOLY_NOVA);
			spells[1].targettype = TARGET_VARIOUS;
			spells[1].instant = false;
			spells[1].perctrigger = 10.0f;
			spells[1].attackstoptimer = 2000; // 2sec

			spells[2].info = dbcSpell.LookupEntry(HOLY_FIRE);
			spells[2].targettype = TARGET_VARIOUS;
			spells[2].instant = false;
			spells[2].perctrigger = 10.0f;
			spells[2].attackstoptimer = 2000; // 2sec

			spells[3].info = dbcSpell.LookupEntry(SPIT_POISON);
			spells[3].targettype = TARGET_VARIOUS;
			spells[3].instant = false;
			spells[3].perctrigger = 10.0f;
			spells[3].attackstoptimer = 2000; // 2sec

			// ----------------------------------------------------------

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
			_unit->RemoveAura(TRANSFORM_SNAKE);
		}

		void OnDied(Unit* mKiller)
		{
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			if(_unit->GetHealthPct() <= 50 && m_spellcheck[0])
			{
				//cast snake transform
				_unit->CastSpell(_unit, spells[0].info, spells[0].instant);
				m_spellcheck[0] = false;
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

					// do another set of spells on transform
					if(_unit->GetHealthPct() <= 50 && i < 3) continue;

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

/*
Venoxis comes with 4 snake adds. He stays in priest form initially, where he casts Holy Nova, Holy Fire, and Renew.
He can also cast Holy Wrath, a spell which jumps from person to person with damage increasing exponentially as it hits people (9000dmg per hit isn't uncommon).
He later shifts to snake form, where his melee damage goes up dramatically and he releases a poison cloud AoE (500dmg/tick).
*/


#define CN_MARLI

#define SUMMON_SPIDERS 24081 // TODO: SUMMON WILDS
#define SPIDER_TRANSFORM 24084

/*
Mar'li has two main forms, like the other bosses in Zul'Gurub.
She starts off in her troll form where she can spawn adds and cast 30 yard AoE poison.
These spider adds quickly gain strength and size if not killed quickly.
When she transforms into her spider form she will web everyone standing near her in place and charge.
As soon as she webs everyone she will attack the person with the highest aggro that has not been webbed (usually a healer if they are out of range.)
*/

void SetupZulGurub(ScriptMgr* mgr)
{
	mgr->register_creature_script(CN_JEKLIK, &JeklikAI::Create);
	mgr->register_creature_script(CN_VENOXIS, &VenoxisAI::Create);
}