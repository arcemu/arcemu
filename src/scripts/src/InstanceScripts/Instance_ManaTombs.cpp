/*
 * ArcScripts for ArcEmu MMORPG Server
 * Copyright (C) 2008-2012 ArcEmu Team <http://arcemu.org/>
 * Copyright (C) 2008-2009 Sun++ Team <http://www.sunscripting.com/>
 * Copyright (C) 2005-2007 Ascent Team
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
/* Instance_ManaTombs.cpp Script		                                */
/************************************************************************/

// Ethereal Darkcaster AI

#define CN_ETHEREAL_DARKCASTER 18331

#define MANA_BURN 29310 // can be wrong
#define SHADOW_WORD_PAIN 41355

class ETHEREALDARKCASTERAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(ETHEREALDARKCASTERAI);
		SP_AI_Spell spells[2];
		bool m_spellcheck[2];

		ETHEREALDARKCASTERAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 2;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(MANA_BURN);
			spells[0].targettype = TARGET_DESTINATION;
			spells[0].instant = true;
			spells[0].perctrigger = 10.0f;
			spells[0].attackstoptimer = 1000;

			spells[1].info = dbcSpell.LookupEntry(SHADOW_WORD_PAIN);
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = true;
			spells[1].perctrigger = 10.0f;
			spells[1].attackstoptimer = 1000;

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

// Ethereal Priest AI

#define CN_ETHEREAL_PRIEST 18317

#define HEAL 39378
#define POWER_WORD_SHIELD 29408 // no idea if this is correct one [also can be: 41373 or other]

class ETHEREALPRIESTAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(ETHEREALPRIESTAI);
		SP_AI_Spell spells[2];
		bool m_spellcheck[2];

		ETHEREALPRIESTAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 2;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(HEAL);
			spells[0].targettype = TARGET_SELF; // until function to target wounded friendly unit will be done
			spells[0].instant = false;
			spells[0].perctrigger = 7.0f;
			spells[0].attackstoptimer = 1000;

			spells[1].info = dbcSpell.LookupEntry(POWER_WORD_SHIELD);
			spells[1].targettype = TARGET_SELF;
			spells[1].instant = true;
			spells[1].perctrigger = 7.0f;
			spells[1].attackstoptimer = 1000;

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

// Ethereal Theurgist AI

#define CN_ETHEREAL_THEURGIST 18315

#define POLYMORPH 36840 // so many poly to choose from
#define BLAST_WAVE 30092 // not sure, also can be: 39001 (maybe too powerful?), 36278, 30600, 25049 (eventually)

class ETHEREALTHEURGISTAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(ETHEREALTHEURGISTAI);
		SP_AI_Spell spells[2];
		bool m_spellcheck[2];

		ETHEREALTHEURGISTAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 2;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(POLYMORPH);
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = false;
			spells[0].perctrigger = 7.0f;
			spells[0].attackstoptimer = 1000;

			spells[1].info = dbcSpell.LookupEntry(BLAST_WAVE);
			spells[1].targettype = TARGET_VARIOUS;
			spells[1].instant = true;
			spells[1].perctrigger = 10.0f;
			spells[1].attackstoptimer = 1000;

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


// Ethereal Sorcerer AI

#define CN_ETHEREAL_SORCERER 18313

#define ARCANE_MISSILES 29956  //29955 can be also: 39414 (too much dmg?), 29956 (just arcane missiles without add. effects) // doesnt'w work for now

class ETHEREALSORCERERAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(ETHEREALSORCERERAI);
		SP_AI_Spell spells[1];
		bool m_spellcheck[1];

		ETHEREALSORCERERAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 1;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(ARCANE_MISSILES);
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = true;
			spells[0].perctrigger = 10.0f;
			spells[0].attackstoptimer = 1000;

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

// Nexus Stalker AI

#define CN_NEXUS_STALKER 18314

#define GOUGE 29425 // not sure, maybe should be : 36862, 
#define POISON 34969 // no idea if this is correct one
#define STEALTH 32615 // Can be also: 31621, 31526, 30991 and others // maybe I should set it's proc chance to 0.0 and add spell casting OnCombatStart...

class NEXUSSTALKERAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(NEXUSSTALKERAI);
		SP_AI_Spell spells[3];
		bool m_spellcheck[3];

		NEXUSSTALKERAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 3;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(GOUGE);
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = true;
			spells[0].perctrigger = 10.0f;
			spells[0].attackstoptimer = 1000;

			spells[1].info = dbcSpell.LookupEntry(POISON);
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = true;
			spells[1].perctrigger = 7.0f;
			spells[1].attackstoptimer = 1000;

			spells[2].info = dbcSpell.LookupEntry(STEALTH);
			spells[2].targettype = TARGET_SELF;
			spells[2].instant = true;
			spells[2].perctrigger = 5.0f;
			spells[2].attackstoptimer = 1000;

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

// Nexus Terror AI

#define CN_NEXUS_TERROR 19307

#define PSYCHIC_SCREAM 34322

class NEXUSTERRORAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(NEXUSTERRORAI);
		SP_AI_Spell spells[1];
		bool m_spellcheck[1];

		NEXUSTERRORAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 1;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(PSYCHIC_SCREAM);
			spells[0].targettype = TARGET_VARIOUS;
			spells[0].instant = true;
			spells[0].perctrigger = 3.0f;
			spells[0].attackstoptimer = 1000;

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


// Mana Leech AI

#define CN_MANA_LEECH 19306

#define ARCANE_EXPLOSION 39348 // not sure about id

class MANALEECHAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(MANALEECHAI);
		SP_AI_Spell spells[1];
		bool m_spellcheck[1];

		MANALEECHAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 1;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(ARCANE_EXPLOSION);
			spells[0].targettype = TARGET_VARIOUS;
			spells[0].instant = true;
			spells[0].perctrigger = 0.0f;
			spells[0].attackstoptimer = 1000;

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
			_unit->CastSpell(_unit, spells[0].info, spells[0].instant);
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

// Ethereal Spellbinder AI

#define CN_ETHEREAL_SPELLBINDER 18312  // couldn't find accurate source of it's spells

#define CORRUPTION 30938
#define IMMOLATE 38806 // Not sure to any id/spell, because of lack of information source
#define UNSTABLE_AFFLICTION 35183 // if it casts this spell it can also be: 34439, 30938
#define SUMMON_ETHEREAL_WRAITH 32316 // added, but still more core support needed
// On WoWWiki is description that Spellbinder summons are Mana Wraiths, but there is no
// spell connected with such creature.

class ETHEREALSPELLBINDERAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(ETHEREALSPELLBINDERAI);
		SP_AI_Spell spells[4];
		bool m_spellcheck[4];

		ETHEREALSPELLBINDERAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 4;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(CORRUPTION);
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = true;
			spells[0].perctrigger = 7.0f;
			spells[0].attackstoptimer = 1000;

			spells[1].info = dbcSpell.LookupEntry(IMMOLATE);
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = true;
			spells[1].perctrigger = 7.0f;
			spells[1].attackstoptimer = 1000;

			spells[2].info = dbcSpell.LookupEntry(UNSTABLE_AFFLICTION);
			spells[2].targettype = TARGET_ATTACKING;
			spells[2].instant = true;
			spells[2].perctrigger = 7.0f;
			spells[2].attackstoptimer = 1000;

			spells[2].info = dbcSpell.LookupEntry(SUMMON_ETHEREAL_WRAITH);
			spells[2].targettype = TARGET_SELF;  // ?
			spells[2].instant = true;
			spells[2].perctrigger = 5.0f;
			spells[2].attackstoptimer = 1000;
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


// Ethereal Wraith AI

#define CN_ETHEREAL_WRAITH 18394

#define SHADOW_BOLT_VOLLEY 36736 // no idea if this is correct spell, but description seems to match with this what should be used
// maybe normal shadow bolt like those: 29927, 30055 should be used

class ETHEREALWRAITHAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(ETHEREALWRAITHAI);
		SP_AI_Spell spells[1];
		bool m_spellcheck[1];

		ETHEREALWRAITHAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 1;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(SHADOW_BOLT_VOLLEY);
			spells[0].targettype = TARGET_VARIOUS; // Haven't tested on groups, but should work correctly.
			spells[0].instant = false;
			spells[0].perctrigger = 15.0f;
			spells[0].attackstoptimer = 1000;

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


/*****************************/
/*                           */
/*         Boss AIs          */
/*                           */
/*****************************/

// Needs to add Yor in a future

// PandemoniusAI

#define CN_PANDEMONIUS 18341

#define VOID_BLAST 32325 //38760
#define DARK_SHELL 32358 //38759

class PANDEMONIUSAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(PANDEMONIUSAI);
		SP_AI_Spell spells[2];
		bool m_spellcheck[2];

		PANDEMONIUSAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 1;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;

			}

			spells[0].info = dbcSpell.LookupEntry(DARK_SHELL);
			spells[0].targettype = TARGET_SELF;
			spells[0].instant = false;
			spells[0].perctrigger = 20.0f;
			spells[0].attackstoptimer = 2000;
			spells[0].cooldown = 20;

			spells[1].info = dbcSpell.LookupEntry(VOID_BLAST);
			spells[1].targettype = TARGET_RANDOM_SINGLE;
			spells[1].instant = false;
			spells[1].perctrigger = 0.0f;
			spells[1].attackstoptimer = 1500;
			spells[1].cooldown = 5;
			spells[1].mindist2cast = 0.0f;
			spells[1].maxdist2cast = 40.0f;
		}

		void OnCombatStart(Unit* mTarget)
		{
			for(int i = 0; i < 2; i++)
				spells[i].casttime = 0;

			int RandomSpeach = rand() % 3;
			switch(RandomSpeach)
			{
				case 0:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "All will feed all your soul!");
					_unit->PlaySoundToSet(10561);
					break;
				case 1:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You fool life!");
					_unit->PlaySoundToSet(10562);
					break;
				case 2:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Do not resist!");
					_unit->PlaySoundToSet(10563);
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
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Yes, I am powered!"); // powered, emopowered?
						_unit->PlaySoundToSet(10564);
						break;
					case 1:
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "More, I must have more!");
						_unit->PlaySoundToSet(10565);
						break;
				}
			}
		}

		void OnCombatStop(Unit* mTarget)
		{
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);

			RemoveAIUpdateEvent();
		}

		void OnDied(Unit* mKiller)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Do the void... once... more.");
			_unit->PlaySoundToSet(10566);

			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			uint32 t = (uint32)time(NULL);
			if(t > spells[1].casttime && _unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
			{
				CastSpellOnRandomTarget(1, spells[1].mindist2cast, spells[1].maxdist2cast, 0, 100);

				spells[1].casttime = t + spells[1].cooldown;
				return;
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

// TavarokAI

#define CN_TAVAROK 18343

#define EARTHQUAKE		33919 // affects also caster if it is close enough (and does it really works?)
#define CRYSTAL_PRISON	32361 // deals 10 dmg instead of 10% player hp - core problem
#define ARCING_SMASH	39144 // 8374; probably wrong id (maybe: 40457 ?)
// Strange... I couldn't find any sounds for this boss in DBC and in extracted
// from client sounds O_O

class TAVAROKAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(TAVAROKAI);
		SP_AI_Spell spells[3];
		bool m_spellcheck[3];

		TAVAROKAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 3;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(EARTHQUAKE);
			spells[0].targettype = TARGET_VARIOUS;
			spells[0].instant = false;
			spells[0].perctrigger = 8.0f;
			spells[0].attackstoptimer = 2000;
			spells[0].cooldown = 20;

			spells[1].info = dbcSpell.LookupEntry(CRYSTAL_PRISON);
			spells[1].targettype = TARGET_RANDOM_SINGLE;
			spells[1].instant = true;
			spells[1].perctrigger = 8.0f;
			spells[1].attackstoptimer = 2000;
			spells[1].cooldown = 20;
			spells[1].mindist2cast = 0.0f;
			spells[1].maxdist2cast = 40.0f;

			spells[2].info = dbcSpell.LookupEntry(ARCING_SMASH);
			spells[2].targettype = TARGET_VARIOUS;  // Should affect only party/raid member in front of caster (I think it works, but needs tests anyway)
			spells[2].instant = true;
			spells[2].perctrigger = 12.0f;
			spells[2].attackstoptimer = 1000;
			spells[2].cooldown = 10;
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


// Nexus-Prince ShaffarAI

#define CN_NEXUS_PRINCE_SHAFFAR 18344

#define FIREBALL				32363
#define FROSTBOLT				32364 // Also can be: 40430 or 32370
#define FROST_NOVA				32365 // Also worth to try: 30094
#define BLINK					34605 // 36109 - forward 20yard, 36718 - 5yard, behind target, 29883 - random target // still doesn't tp boss
#define SUMMON_ETEREAL_BECON	32371 // not sure about spawning way
// TO DO: Work on beacons and find out if my current way of spawning them is correct

class NEXUSPRINCESHAFFARAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(NEXUSPRINCESHAFFARAI);
		SP_AI_Spell spells[5];
		bool m_spellcheck[5];

		NEXUSPRINCESHAFFARAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 4;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(FIREBALL);
			spells[0].targettype = TARGET_RANDOM_SINGLE;
			spells[0].instant = false;
			spells[0].perctrigger = 35.0f;
			spells[0].attackstoptimer = 2000;
			spells[0].cooldown = 5;
			spells[0].mindist2cast = 0.0f;
			spells[0].maxdist2cast = 40.0f;

			spells[1].info = dbcSpell.LookupEntry(FROSTBOLT);
			spells[1].targettype = TARGET_RANDOM_SINGLE;
			spells[1].instant = false;
			spells[1].perctrigger = 35.0f;
			spells[1].attackstoptimer = 2000;
			spells[1].cooldown = 5;
			spells[1].mindist2cast = 0.0f;
			spells[1].maxdist2cast = 40.0f;

			spells[2].info = dbcSpell.LookupEntry(FROST_NOVA);
			spells[2].targettype = TARGET_VARIOUS;
			spells[2].instant = true;
			spells[2].perctrigger = 15.0f;
			spells[2].attackstoptimer = 1000;
			spells[2].cooldown = 15;

			spells[3].info = dbcSpell.LookupEntry(BLINK);
			spells[3].targettype = TARGET_SELF;
			spells[3].instant = true;
			spells[3].perctrigger = 5.0f;
			spells[3].attackstoptimer = 1000;
			spells[3].cooldown = 20;

			spells[4].info = dbcSpell.LookupEntry(SUMMON_ETEREAL_BECON);
			spells[4].targettype = TARGET_SELF;
			spells[4].instant = true;
			spells[4].perctrigger = 0.0f;
			spells[4].attackstoptimer = 1000;
			spells[4].cooldown = 10;
		}

		void OnCombatStart(Unit* mTarget)
		{
			for(int i = 0; i < 3; i++)
				spells[i].casttime = 0;

			uint32 t = (uint32)time(NULL);
			spells[3].casttime = t + RandomUInt(10);
			spells[4].casttime = t + spells[4].cooldown;

			int RandomSpeach = rand() % 3;
			switch(RandomSpeach)
			{
				case 0:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Ah, we have not been properly introduced!");
					_unit->PlaySoundToSet(10541);
					break;
				case 1:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "An epic battle! How exciting.");
					_unit->PlaySoundToSet(10542);
					break;
				case 2:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I have longed for a good adventure!");
					_unit->PlaySoundToSet(10543);
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
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "And now, we part company."); // part company?
						_unit->PlaySoundToSet(10544);
						break;
					case 1:
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "It has been... entertaining.");
						_unit->PlaySoundToSet(10545);
						break;
				}
			}
		}

		void OnCombatStop(Unit* mTarget)
		{
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);

			RemoveAIUpdateEvent();
		}

		void OnDied(Unit* mKiller)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I must bid you... Farewell."); // I don't think it's correct.
			_unit->PlaySoundToSet(10546);

			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			// not sure if it should be like that
			uint32 t = (uint32)time(NULL);
			if(t > spells[4].casttime && _unit->GetCurrentSpell() == NULL)
			{
				_unit->CastSpell(_unit, spells[4].info, spells[4].instant);

				spells[4].casttime = t + 10;
				return;
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
						if(!spells[i].instant)
							_unit->GetAIInterface()->StopMovement(1);


						if(i == 3)
						{
							uint32 t = (uint32)time(NULL);
							if(t > spells[2].casttime && RandomUInt(2) == 1)
							{
								_unit->CastSpell(_unit, spells[2].info, spells[2].instant);

								spells[2].casttime = t + spells[2].cooldown;
							}
						}

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

// YorAI

#define CN_YOR 22930

#define DOUBLE_BREATH	38369 // couldn't have found anything more powerful with that name
#define STOMP			34716 // not sure even more about this one

class YorAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(YorAI);
		SP_AI_Spell spells[2];
		bool m_spellcheck[2];

		YorAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 2;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(DOUBLE_BREATH);
			spells[0].targettype = TARGET_VARIOUS;
			spells[0].instant = true;
			spells[0].perctrigger = 20.0f;
			spells[0].attackstoptimer = 2000;
			spells[0].cooldown = 15;

			spells[1].info = dbcSpell.LookupEntry(STOMP);
			spells[1].targettype = TARGET_VARIOUS;
			spells[1].instant = true;
			spells[1].perctrigger = 7.0f;
			spells[1].attackstoptimer = 2000;
			spells[1].cooldown = 25;
		}

		void OnCombatStart(Unit* mTarget)
		{
			uint32 t = (uint32)time(NULL);
			spells[0].casttime = 0;
			spells[1].casttime = t + RandomUInt(10);

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

		int nrspells;
};

void SetupManaTombs(ScriptMgr* mgr)
{
	mgr->register_creature_script(CN_ETHEREAL_DARKCASTER, &ETHEREALDARKCASTERAI::Create);
	mgr->register_creature_script(CN_ETHEREAL_PRIEST, &ETHEREALPRIESTAI::Create);
	mgr->register_creature_script(CN_ETHEREAL_SPELLBINDER, &ETHEREALSPELLBINDERAI::Create);
	mgr->register_creature_script(CN_ETHEREAL_THEURGIST, &ETHEREALTHEURGISTAI::Create);
	mgr->register_creature_script(CN_ETHEREAL_SORCERER, &ETHEREALSORCERERAI::Create);
	mgr->register_creature_script(CN_ETHEREAL_WRAITH, &ETHEREALWRAITHAI::Create);
	mgr->register_creature_script(CN_NEXUS_STALKER, &NEXUSSTALKERAI::Create);
	mgr->register_creature_script(CN_NEXUS_TERROR, &NEXUSTERRORAI::Create);
	mgr->register_creature_script(CN_MANA_LEECH, &MANALEECHAI::Create);
	mgr->register_creature_script(CN_PANDEMONIUS, &PANDEMONIUSAI::Create);
	mgr->register_creature_script(CN_TAVAROK, &TAVAROKAI::Create);
	mgr->register_creature_script(CN_NEXUS_PRINCE_SHAFFAR, &NEXUSPRINCESHAFFARAI::Create);
	mgr->register_creature_script(CN_YOR, &YorAI::Create);
}
