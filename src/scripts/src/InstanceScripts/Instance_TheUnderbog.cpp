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
/* Instance_TheUnderbog.cpp Script		                                */
/************************************************************************/

// Bog Giant AI

#define CN_BOG_GIANT 17723

#define FUNGAL_DECAY_GIANT 32065
#define TRAMPLE 15550	// not sure to those spells
#define ENRAGE_GIANT 8599

class BOGGIANTAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(BOGGIANTAI);
		SP_AI_Spell spells[3];
		bool m_spellcheck[3];

		BOGGIANTAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 3;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(FUNGAL_DECAY_GIANT);
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = true;
			spells[0].cooldown = 25;
			spells[0].perctrigger = 0.0f;
			spells[0].attackstoptimer = 1000;

			spells[1].info = dbcSpell.LookupEntry(TRAMPLE);
			spells[1].targettype = TARGET_VARIOUS;
			spells[1].instant = true;
			spells[1].cooldown = 10;
			spells[1].perctrigger = 0.0f;
			spells[1].attackstoptimer = 1000;

			spells[2].info = dbcSpell.LookupEntry(ENRAGE_GIANT);
			spells[2].targettype = TARGET_SELF;
			spells[2].instant = true;
			spells[2].cooldown = 45;
			spells[2].perctrigger = 0.0f;
			spells[2].attackstoptimer = 1000;

		}

		void OnCombatStart(Unit* mTarget)
		{
			CastTime();
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void CastTime()
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = spells[i].cooldown;
		}

		void OnTargetDied(Unit* mTarget)
		{
		}

		void OnCombatStop(Unit* mTarget)
		{
			CastTime();
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
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

// Claw AI

#define CN_CLAW 17827

#define ENRAGE_CLAW 34971
#define ECHOING_ROAR 31429
#define MAUL 34298

class CLAWAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(CLAWAI);
		SP_AI_Spell spells[3];
		bool m_spellcheck[3];

		CLAWAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 3;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(ENRAGE_CLAW);
			spells[0].targettype = TARGET_VARIOUS;
			spells[0].instant = true;
			spells[0].cooldown = 45;
			spells[0].perctrigger = 0.0f;
			spells[0].attackstoptimer = 1000;

			spells[1].info = dbcSpell.LookupEntry(ECHOING_ROAR);
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = true;
			spells[1].cooldown = 35;
			spells[1].perctrigger = 0.0f;
			spells[1].attackstoptimer = 1000;

			spells[2].info = dbcSpell.LookupEntry(MAUL);
			spells[2].targettype = TARGET_ATTACKING;
			spells[2].instant = true;
			spells[2].cooldown = 10;
			spells[2].perctrigger = 0.0f;
			spells[2].attackstoptimer = 1000;

		}

		void OnCombatStart(Unit* mTarget)
		{
			CastTime();
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void CastTime()
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = spells[i].cooldown;
		}

		void OnTargetDied(Unit* mTarget)
		{
		}

		void OnCombatStop(Unit* mTarget)
		{
			CastTime();
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
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

// Underbat AI

#define CN_UNDERBAT 17724

#define KNOCKDOWN 20276 // can't find correct aoe knockdown for now =/ does it use it for sure?
#define TENTACLE_LASH 34171 // not sure to this

class UNDERBATAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(UNDERBATAI);
		SP_AI_Spell spells[2];
		bool m_spellcheck[2];

		UNDERBATAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 2;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(KNOCKDOWN);
			spells[0].targettype = TARGET_VARIOUS;
			spells[0].instant = true;
			spells[0].cooldown = 35;
			spells[0].perctrigger = 0.0f;
			spells[0].attackstoptimer = 1000;

			spells[1].info = dbcSpell.LookupEntry(TENTACLE_LASH);
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = true;
			spells[1].cooldown = 10;
			spells[1].perctrigger = 0.0f;
			spells[1].attackstoptimer = 1000;

		}

		void OnCombatStart(Unit* mTarget)
		{
			CastTime();
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void CastTime()
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = spells[i].cooldown;
		}

		void OnTargetDied(Unit* mTarget)
		{
		}

		void OnCombatStop(Unit* mTarget)
		{
			CastTime();
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
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

// Fen Ray AI

#define CN_FEN_RAY 17731

#define PSYCHIC_HORROR 34984

class FENRAYAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(FENRAYAI);
		SP_AI_Spell spell;
		bool m_spellcheck;

		FENRAYAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			m_spellcheck = false;
			spell.info = dbcSpell.LookupEntry(PSYCHIC_HORROR);
			spell.targettype = TARGET_ATTACKING; // Should be only used on target on the second place on aggro list
			spell.instant = false;
			spell.cooldown = 30;
			spell.perctrigger = 0.0f;
			spell.attackstoptimer = 1000;
		}

		void OnCombatStart(Unit* mTarget)
		{
			CastTime();
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void CastTime()
		{
			spell.casttime = spell.cooldown;
		}

		void OnTargetDied(Unit* mTarget)
		{
		}

		void OnCombatStop(Unit* mTarget)
		{
			CastTime();
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
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
				spell.casttime--;

				if(m_spellcheck)
				{
					spell.casttime = spell.cooldown;
					target = _unit->GetAIInterface()->getNextTarget();
					_unit->CastSpell(target, spell.info, spell.instant);

					if(spell.speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spell.speech.c_str());
						_unit->PlaySoundToSet(spell.soundid);
					}

					m_spellcheck = false;
					return;
				}

				if((val > comulativeperc && val <= (comulativeperc + spell.perctrigger)) || !spell.casttime)
				{
					_unit->setAttackTimer(spell.attackstoptimer, false);
					m_spellcheck = true;
				}
				comulativeperc += spell.perctrigger;
			}
		}

};


/*
	Lykul Stinger/Wasp - Bee-type mobs which inflict a poison which deals minor nature
	damage. However, targets which are inflicted with poison from both a Wasp and
	Stinger take sharply increased damage. Cannot be polymorphed, but can be frost
	nova'd. Can both be trapped by hunters, and Wasps should be killed first in a
	mixed mob group due to their relative weakness and poisons. The poison can be
	interrupted or spellreflected.
																						*/

// Lykul Stinger AI

#define CN_LYKUL_STINGER 19632

#define POISON 36694 // Maybe be: 36694 (armor - 5000 :O), 13526, 3396 (both very old spells), 24111
#define STINGER_FRENZY 34392	// not sure
// From description this poison is Corrosive Poison, but Idk for sure

class LYKULSTINGERAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(LYKULSTINGERAI);
		SP_AI_Spell spells[2];
		bool m_spellcheck[2];

		LYKULSTINGERAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 2;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(POISON);
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = true;
			spells[0].cooldown = 40;
			spells[0].perctrigger = 0.0f;
			spells[0].attackstoptimer = 1000;

			spells[1].info = dbcSpell.LookupEntry(STINGER_FRENZY);
			spells[1].targettype = TARGET_SELF;
			spells[1].instant = true;
			spells[1].cooldown = 15;
			spells[1].perctrigger = 0.0f;
			spells[1].attackstoptimer = 1000;

		}

		void OnCombatStart(Unit* mTarget)
		{
			CastTime();
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void CastTime()
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = spells[i].cooldown;
		}

		void OnTargetDied(Unit* mTarget)
		{
		}

		void OnCombatStop(Unit* mTarget)
		{
			CastTime();
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
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


// Lykul Wasp AI

#define CN_LYKUL_WASP 17732

#define POISON 36694 // Maybe be: 36694 (armor - 5000 :O), 13526, 3396 (both very old spells), 24111
#define POISON_SPIT 32330
#define ENRAGE_WASP 8599
// From description this poison is Corrosive Poison, but Idk for sure

class LYKULWASPAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(LYKULWASPAI);
		SP_AI_Spell spells[3];
		bool m_spellcheck[3];

		LYKULWASPAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 3;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(POISON);
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = true;
			spells[0].cooldown = 45;
			spells[0].perctrigger = 0.0f;
			spells[0].attackstoptimer = 1000;

			spells[1].info = dbcSpell.LookupEntry(POISON_SPIT);
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = false;
			spells[1].cooldown = 10;
			spells[1].perctrigger = 0.0f;
			spells[1].attackstoptimer = 1000;

			spells[2].info = dbcSpell.LookupEntry(ENRAGE_WASP);
			spells[2].targettype = TARGET_SELF;
			spells[2].instant = true;
			spells[2].cooldown = 70;
			spells[2].perctrigger = 0.0f;
			spells[2].attackstoptimer = 1000;

		}

		void OnCombatStart(Unit* mTarget)
		{
			CastTime();
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
			_unit->CastSpell(_unit, spells[2].info, spells[2].instant);
		}

		void CastTime()
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = spells[i].cooldown;
		}

		void OnTargetDied(Unit* mTarget)
		{
		}

		void OnCombatStop(Unit* mTarget)
		{
			CastTime();
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
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


// Wrathfin Warrior AI

#define CN_WRATHFIN_WARRIOR 17735

#define REND 36991 // It is only bleed like spell I remember well (if it isn't bleed, but other spell
// report it then. Can be: 36991, 37662, 29574, 29578, 36965 (big dmg)	// not sure if it's even casted =(
// Also can be Carnivorous Bite, but no idea :P
#define STRIKE 11976
#define SHIELD_BASH 11972	// ofc not sure to those
#define ENRAGE_WARTHFIN_WARRIOR 8599

class WRATHFINWARRIORAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(WRATHFINWARRIORAI);
		SP_AI_Spell spells[4];
		bool m_spellcheck[4];

		WRATHFINWARRIORAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 4;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(REND);
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = true;
			spells[0].cooldown = 25;
			spells[0].perctrigger = 0.0f;
			spells[0].attackstoptimer = 1000;

			spells[1].info = dbcSpell.LookupEntry(STRIKE);
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = true;
			spells[1].cooldown = 10;
			spells[1].perctrigger = 0.0f;
			spells[1].attackstoptimer = 1000;

			spells[2].info = dbcSpell.LookupEntry(SHIELD_BASH);
			spells[2].targettype = TARGET_ATTACKING;
			spells[2].instant = true;
			spells[2].cooldown = 15;
			spells[2].perctrigger = 0.0f;
			spells[2].attackstoptimer = 1000;

			spells[3].info = dbcSpell.LookupEntry(ENRAGE_WARTHFIN_WARRIOR);
			spells[3].targettype = TARGET_SELF;
			spells[3].instant = true;
			spells[3].cooldown = 70;
			spells[3].perctrigger = 0.0f;
			spells[3].attackstoptimer = 1000;

		}

		void OnCombatStart(Unit* mTarget)
		{
			CastTime();
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
			_unit->CastSpell(_unit, spells[3].info, spells[3].instant);
		}

		void CastTime()
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = spells[i].cooldown;
		}

		void OnTargetDied(Unit* mTarget)
		{
		}

		void OnCombatStop(Unit* mTarget)
		{
			CastTime();
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
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

// Wrathfin Sentry AI

#define CN_WRATHFIN_SENTRY 17727

#define STRIKE_WRATHFIN_SENTRY 11976
#define SHIELD_BASH_WRATHFIN_SENTRY 11972
//Invisibility and Stealth Detection 18950 ?

class WRATHFINSENTRYAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(WRATHFINSENTRYAI);
		SP_AI_Spell spells[2];
		bool m_spellcheck[2];

		WRATHFINSENTRYAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 2;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(STRIKE_WRATHFIN_SENTRY);
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = true;
			spells[0].cooldown = 10;
			spells[0].perctrigger = 0.0f;
			spells[0].attackstoptimer = 1000;

			spells[1].info = dbcSpell.LookupEntry(SHIELD_BASH_WRATHFIN_SENTRY);
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = true;
			spells[1].cooldown = 25;
			spells[1].perctrigger = 0.0f;
			spells[1].attackstoptimer = 1000;

		}

		void OnCombatStart(Unit* mTarget)
		{
			CastTime();
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void CastTime()
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = spells[i].cooldown;
		}

		void OnTargetDied(Unit* mTarget)
		{
		}

		void OnCombatStop(Unit* mTarget)
		{
			CastTime();
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
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

// Wrathfin Myrmidon AI

#define CN_WRATHFIN_MYRMIDON 17726

#define CORAL_CUT 31410

class WRATHFINMYRMIDONAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(WRATHFINMYRMIDONAI);
		SP_AI_Spell spell;
		bool m_spellcheck;

		WRATHFINMYRMIDONAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			m_spellcheck = false;
			spell.info = dbcSpell.LookupEntry(CORAL_CUT);
			spell.targettype = TARGET_ATTACKING;
			spell.instant = true;
			spell.cooldown = 10;
			spell.perctrigger = 0.0f;
			spell.attackstoptimer = 1000;
		}

		void OnCombatStart(Unit* mTarget)
		{
			CastTime();
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void CastTime()
		{
			spell.casttime = spell.cooldown;
		}

		void OnTargetDied(Unit* mTarget)
		{
		}

		void OnCombatStop(Unit* mTarget)
		{
			CastTime();
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
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
				spell.casttime--;

				if(m_spellcheck)
				{
					spell.casttime = spell.cooldown;
					target = _unit->GetAIInterface()->getNextTarget();
					_unit->CastSpell(target, spell.info, spell.instant);

					if(spell.speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spell.speech.c_str());
						_unit->PlaySoundToSet(spell.soundid);
					}

					m_spellcheck = false;
					return;
				}

				if((val > comulativeperc && val <= (comulativeperc + spell.perctrigger)) || !spell.casttime)
				{
					_unit->setAttackTimer(spell.attackstoptimer, false);
					m_spellcheck = true;
				}
				comulativeperc += spell.perctrigger;
			}
		}

};

// Underbog Lord AI

#define CN_UNDERBOG_LORD 17734

/*#define KNOCKBACK 28405	// this is wrong id for sure. On Wiki there was sth about kickback, but there is no such spell in DBC -.-'
							// or maybe it's Boglord Bash: 32077 ?*/
#define ENRAGE_LORD 8599	//33653 // This spell was added, but still I don't have infos abou it :| 
// Can be: 37023, 33653 and others...
#define KNOCK_AWAY 25778	// not sure to those
#define FUNGAL_DECAY 32065

class UNDERBOGLORDAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(UNDERBOGLORDAI);
		SP_AI_Spell spells[3];
		bool m_spellcheck[3];

		UNDERBOGLORDAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 3;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}
			/*
					spells[0].info = dbcSpell.LookupEntry(KNOCKBACK);
					spells[0].targettype = TARGET_ATTACKING; // Affects also caster :|
					spells[0].instant = true;
					spells[0].cooldown = 35;
					spells[0].perctrigger = 0.0f;
					spells[0].attackstoptimer = 1000;
			*/
			spells[0].info = dbcSpell.LookupEntry(KNOCK_AWAY);
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = true;
			spells[0].cooldown = 35;
			spells[0].perctrigger = 0.0f;
			spells[0].attackstoptimer = 1000;

			spells[1].info = dbcSpell.LookupEntry(ENRAGE_LORD);
			spells[1].targettype = TARGET_SELF;
			spells[1].instant = true;
			spells[1].cooldown = 40;
			spells[1].perctrigger = 0.0f;
			spells[1].attackstoptimer = 1000;
			spells[1].speech = "Bog Lord Grows in Size!";

			spells[2].info = dbcSpell.LookupEntry(FUNGAL_DECAY);
			spells[2].targettype = TARGET_ATTACKING;
			spells[2].instant = true;
			spells[2].cooldown = 25;
			spells[2].perctrigger = 0.0f;
			spells[2].attackstoptimer = 1000;

		}

		void OnCombatStart(Unit* mTarget)
		{
			CastTime();
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void CastTime()
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = spells[i].cooldown;
		}

		void OnTargetDied(Unit* mTarget)
		{
		}

		void OnCombatStop(Unit* mTarget)
		{
			CastTime();
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
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


/*	Murkblood Lost Ones - Humanoid mobs Spearmen (flings spears and cast Viper Sting),
	Tribesmen, Oracles (casts Fireball) and Healers (casts Holy Light, Heal and
	Prayer of Healing}. Come in groups together. Healers will cast Prayer of Healing
	if left alone which will completely heal all nearby Murkbloods and Wrathfins and so
	should be killed first or crowd-controlled until last. The heal is interruptible.
																						*/
// Murkblood Spearman AI
#define CN_MURKBLOOD_SPEARMAN 17729

//#define SPEAR_THROW 31758 // not sure if that spell is casted (maybe other, but similar), also can be: 31758, 40083, 32248
#define THROW 22887
#define VIPER_STRING 31407

class MURKBLOODSPEARMANAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(MURKBLOODSPEARMANAI);
		SP_AI_Spell spells[2];
		bool m_spellcheck[2];

		MURKBLOODSPEARMANAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 2;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}
			/*
					spells[0].info = dbcSpell.LookupEntry(SPEAR_THROW);
					spells[0].targettype = TARGET_ATTACKING;
					spells[0].instant = true;
					spells[0].cooldown = 10;
					spells[0].perctrigger = 0.0f;
					spells[0].attackstoptimer = 1000;
			*/
			spells[0].info = dbcSpell.LookupEntry(THROW);
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = true;
			spells[0].cooldown = 10;
			spells[0].perctrigger = 0.0f;
			spells[0].attackstoptimer = 1000;

			spells[1].info = dbcSpell.LookupEntry(VIPER_STRING);
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = true;
			spells[1].cooldown = 25;
			spells[1].perctrigger = 0.0f;
			spells[1].attackstoptimer = 1000;

		}

		void OnCombatStart(Unit* mTarget)
		{
			CastTime();
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void CastTime()
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = spells[i].cooldown;
		}

		void OnTargetDied(Unit* mTarget)
		{
		}

		void OnCombatStop(Unit* mTarget)
		{
			CastTime();
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
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


// Murkblood Oracle AI
#define CN_MURKBLOOD_ORACLE 17771

#define FIREBALL 14034 //32369 // can be: 32369, 37329, 32363, 31262, 40877, 38641, 37111
#define SHADOW_BOLT_ORACLE 12471
#define CORRUPTION 31405
#define SCORCH 15241
#define AMPLIFY_DAMAGE 12248
#define FROSTBOLT 15497
#define ELEMENTAL_ARMOR 34880

class MURKBLOODORACLEAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(MURKBLOODORACLEAI);
		SP_AI_Spell spells[7];
		bool m_spellcheck[7];

		MURKBLOODORACLEAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 7;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(FIREBALL);
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = false;
			spells[0].cooldown = 25;
			spells[0].perctrigger = 0.0f;
			spells[0].attackstoptimer = 1000;

			spells[1].info = dbcSpell.LookupEntry(SHADOW_BOLT_ORACLE);
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = false;
			spells[1].cooldown = 35;
			spells[1].perctrigger = 0.0f;
			spells[1].attackstoptimer = 1000;

			spells[2].info = dbcSpell.LookupEntry(CORRUPTION);
			spells[2].targettype = TARGET_ATTACKING;
			spells[2].instant = false;
			spells[2].cooldown = 45;
			spells[2].perctrigger = 0.0f;
			spells[2].attackstoptimer = 1000;

			spells[3].info = dbcSpell.LookupEntry(SCORCH);
			spells[3].targettype = TARGET_ATTACKING;
			spells[3].instant = false;
			spells[3].cooldown = 75;
			spells[3].perctrigger = 0.0f;
			spells[3].attackstoptimer = 1000;

			spells[4].info = dbcSpell.LookupEntry(AMPLIFY_DAMAGE);
			spells[4].targettype = TARGET_ATTACKING;
			spells[4].instant = false;
			spells[4].cooldown = 90;
			spells[4].perctrigger = 0.0f;
			spells[4].attackstoptimer = 1000;

			spells[5].info = dbcSpell.LookupEntry(FROSTBOLT);
			spells[5].targettype = TARGET_ATTACKING;
			spells[5].instant = false;
			spells[5].cooldown = 60;
			spells[5].perctrigger = 0.0f;
			spells[5].attackstoptimer = 1000;

			spells[6].info = dbcSpell.LookupEntry(ELEMENTAL_ARMOR);
			spells[6].targettype = TARGET_SELF;
			spells[6].instant = true;
			spells[6].cooldown = 150;
			spells[6].perctrigger = 0.0f;
			spells[6].attackstoptimer = 1000;

		}

		void OnCombatStart(Unit* mTarget)
		{
			CastTime();
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
			_unit->CastSpell(_unit, spells[6].info, spells[6].instant);
		}

		void CastTime()
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = spells[i].cooldown;
		}

		void OnTargetDied(Unit* mTarget)
		{
		}

		void OnCombatStop(Unit* mTarget)
		{
			CastTime();
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
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


// Murkblood Healer AI
#define CN_MURKBLOOD_HEALER 17730

//#define HEAL 32130 // also can be: 32130, 31730, 34945, 38209, 39378, 31739 NOT CASTED!?
#define HOLY_LIGHT 29427 //32769 // can be: 37979, 32769, 31713
#define RENEW 34423
#define PRAYER_OF_HEALING 15585 //30604 // can be: 35943, 30604 causes crashes =/

class MURKBLOODHEALERAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(MURKBLOODHEALERAI);
		SP_AI_Spell spells[3];
		bool m_spellcheck[3];

		MURKBLOODHEALERAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 3;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(HOLY_LIGHT);
			spells[0].targettype = TARGET_SELF;
			spells[0].instant = true;
			spells[0].cooldown = 20;
			spells[0].perctrigger = 0.0f;
			spells[0].attackstoptimer = 1000;

			spells[1].info = dbcSpell.LookupEntry(RENEW);
			spells[1].targettype = TARGET_SELF;	// should be casted on ally (not enemy :S)
			spells[1].instant = true;
			spells[1].cooldown = 35;
			spells[1].perctrigger = 0.0f;
			spells[1].attackstoptimer = 1000;

			spells[2].info = dbcSpell.LookupEntry(PRAYER_OF_HEALING);
			spells[2].targettype = TARGET_VARIOUS;
			spells[2].instant = false;
			spells[2].cooldown = -1;
			spells[2].perctrigger = 0.0f;
			spells[2].attackstoptimer = 1000;
			/*
					spells[1].info = dbcSpell.LookupEntry(HEAL);
					spells[1].targettype = TARGET_SELF;
					spells[1].instant = true;
					spells[1].cooldown = 25;
					spells[1].perctrigger = 0.0f;
					spells[1].attackstoptimer = 1000;
			*/
		}

		void OnCombatStart(Unit* mTarget)
		{
			CastTime();
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void CastTime()
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = spells[i].cooldown;
		}

		void OnTargetDied(Unit* mTarget)
		{
		}

		void OnCombatStop(Unit* mTarget)
		{
			CastTime();
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
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

// Murkblood Tribesman AI
#define CN_MURKBLOOD_TRIBESMAN 17728

#define STRIKE_TRIBESMAN 12057
#define ENRAGE_TRIBESMAN 8599

class MURKBLOODTRIBESMANAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(MURKBLOODTRIBESMANAI);
		SP_AI_Spell spells[2];
		bool m_spellcheck[2];

		MURKBLOODTRIBESMANAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 2;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(STRIKE_TRIBESMAN);
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = true;
			spells[0].cooldown = 10;
			spells[0].perctrigger = 0.0f;
			spells[0].attackstoptimer = 1000;

			spells[1].info = dbcSpell.LookupEntry(ENRAGE_TRIBESMAN);
			spells[1].targettype = TARGET_SELF;	// should be casted on ally (not enemy :S)
			spells[1].instant = true;
			spells[1].cooldown = 70;
			spells[1].perctrigger = 0.0f;
			spells[1].attackstoptimer = 1000;

		}

		void OnCombatStart(Unit* mTarget)
		{
			CastTime();
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
			_unit->CastSpell(_unit, spells[1].info, spells[1].instant);
		}

		void CastTime()
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = spells[i].cooldown;
		}

		void OnTargetDied(Unit* mTarget)
		{
		}

		void OnCombatStop(Unit* mTarget)
		{
			CastTime();
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
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

/*
	Underbog Shambler/Frenzy - Nature elementals which can afflict their
	target with a disease which inflicts nature damage the next 5 times that
	target takes damage. Also a Frenzy variety which enrage and deal increased
	damage. Can be banished and trapped. Shamblers heal themselves and others.
																				*/

// Underbog Shambler AI
#define CN_UNDERBOG_SHAMBLER 17871

//#define HEAL 32130 // 38209 | no idea if he is using heal (using healing spell yes (nearly sure), but if heal Idk)
//#define SPORE_EXPLOSION 37966 //38419 //32327
#define ITCHY_SPORES 32329
#define ALLERGIES 31427
#define FUNGAL_REGROWTH 34163

class UNDERBOGSHAMBLERAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(UNDERBOGSHAMBLERAI);
		SP_AI_Spell spells[3];
		bool m_spellcheck[3];

		UNDERBOGSHAMBLERAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 3;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(ITCHY_SPORES);
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = false;
			spells[0].cooldown = 15;
			spells[0].perctrigger = 0.0f;
			spells[0].attackstoptimer = 1000;

			spells[1].info = dbcSpell.LookupEntry(ALLERGIES);
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = false;
			spells[1].cooldown = 35;
			spells[1].perctrigger = 0.0f;
			spells[1].attackstoptimer = 1000;

			spells[2].info = dbcSpell.LookupEntry(FUNGAL_REGROWTH);
			spells[2].targettype = TARGET_SELF;	// should be ally
			spells[2].instant = false;
			spells[2].cooldown = 25;
			spells[2].perctrigger = 0.0f;
			spells[2].attackstoptimer = 1000;
			/*
					spells[1].info = dbcSpell.LookupEntry(HEAL);
					spells[1].targettype = TARGET_SELF;
					spells[1].instant = true;
					spells[1].cooldown = ;
					spells[1].perctrigger = 0.0f;
					spells[1].attackstoptimer = 1000;

					spells[2].info = dbcSpell.LookupEntry(SPORE_EXPLOSION);
					spells[2].targettype = TARGET_DESTINATION; // Idk why it casts that on himself =/
					spells[2].instant = true;
					spells[2].cooldown = ;
					spells[2].perctrigger = 0.0f;
					spells[2].attackstoptimer = 1000;
			*/
		}

		void OnCombatStart(Unit* mTarget)
		{
			CastTime();
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void CastTime()
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = spells[i].cooldown;
		}

		void OnTargetDied(Unit* mTarget)
		{
		}

		void OnCombatStop(Unit* mTarget)
		{
			CastTime();
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
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


// Underbog Frenzy AI
#define CN_UNDERBOG_FRENZY 20465

#define ENRAGE_FRENZY 34971 // no idea if this is good spell id / still don't know if it's good
// Invisibility and Stealth Detection 18950 ?
// Permanent Feign Death (Root) 31261 ?
class UNDERBOGFRENZYAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(UNDERBOGFRENZYAI);
		SP_AI_Spell spell;
		bool m_spellcheck;

		UNDERBOGFRENZYAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			m_spellcheck = false;
			spell.info = dbcSpell.LookupEntry(ENRAGE_FRENZY);
			spell.targettype = TARGET_SELF;
			spell.instant = true;
			spell.cooldown = 30;
			spell.perctrigger = 0.0f;
			spell.attackstoptimer = 1000;
		}

		void OnCombatStart(Unit* mTarget)
		{
			CastTime();
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void CastTime()
		{
			spell.casttime = spell.cooldown;
		}

		void OnTargetDied(Unit* mTarget)
		{
		}

		void OnCombatStop(Unit* mTarget)
		{
			CastTime();
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
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
				spell.casttime--;

				if(m_spellcheck)
				{
					spell.casttime = spell.cooldown;
					target = _unit->GetAIInterface()->getNextTarget();
					_unit->CastSpell(_unit, spell.info, spell.instant);

					if(spell.speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spell.speech.c_str());
						_unit->PlaySoundToSet(spell.soundid);
					}

					m_spellcheck = false;
					return;
				}

				if((val > comulativeperc && val <= (comulativeperc + spell.perctrigger)) || !spell.casttime)
				{
					_unit->setAttackTimer(spell.attackstoptimer, false);
					m_spellcheck = true;
				}
				comulativeperc += spell.perctrigger;
			}
		}

};

// Underbog Lurker AI
#define CN_UNDERBOG_LURKER 17725

#define WILD_GROWTH 34161

class UNDERBOGLURKERAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(UNDERBOGLURKERAI);
		SP_AI_Spell spell;
		bool m_spellcheck;

		UNDERBOGLURKERAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			m_spellcheck = false;
			spell.info = dbcSpell.LookupEntry(WILD_GROWTH);
			spell.targettype = TARGET_SELF;
			spell.instant = true;
			spell.cooldown = 30;
			spell.perctrigger = 0.0f;
			spell.attackstoptimer = 1000;
		}

		void OnCombatStart(Unit* mTarget)
		{
			CastTime();
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void CastTime()
		{
			spell.casttime = spell.cooldown;
		}

		void OnTargetDied(Unit* mTarget)
		{
		}

		void OnCombatStop(Unit* mTarget)
		{
			CastTime();
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
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
				spell.casttime--;

				if(m_spellcheck)
				{
					spell.casttime = spell.cooldown;
					target = _unit->GetAIInterface()->getNextTarget();
					_unit->CastSpell(_unit, spell.info, spell.instant);

					if(spell.speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spell.speech.c_str());
						_unit->PlaySoundToSet(spell.soundid);
					}

					m_spellcheck = false;
					return;
				}

				if((val > comulativeperc && val <= (comulativeperc + spell.perctrigger)) || !spell.casttime)
				{
					_unit->setAttackTimer(spell.attackstoptimer, false);
					m_spellcheck = true;
				}
				comulativeperc += spell.perctrigger;
			}
		}

};

/*****************************/
/*                           */
/*         Boss AIs          */
/*                           */
/*****************************/

// HungarfenAI

#define CN_HUNGARFEN 17770

#define UNDERBOG_MUSHROOM 31693 // still not idea *confused* //34588 // No idea if this is right spell, but should be correct (also aditional core support needed!)
#define FOUL_SPORES 31673 //DBC: 31673, 31697 // this one needs additional core support too
// Putrid Mushroom Primer 31693 ?
// Despawn Underbog Mushrooms 34874 ?

class HUNGARFENAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(HUNGARFENAI);
		SP_AI_Spell spells[2];
		bool m_spellcheck[2];

		HUNGARFENAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 1;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(UNDERBOG_MUSHROOM);	// need to have more fun with it?
			spells[0].targettype = TARGET_RANDOM_DESTINATION;
			spells[0].instant = true;
			spells[0].cooldown = 15;
			spells[0].perctrigger = 0.0f;//100.0f;
			spells[0].attackstoptimer = 1000;

			spells[1].info = dbcSpell.LookupEntry(FOUL_SPORES);
			spells[1].targettype = TARGET_VARIOUS;
			spells[1].instant = false;
			spells[1].cooldown = 0;
			spells[1].perctrigger = 0.0f;
			spells[1].attackstoptimer = 1000;

			FourSpores = false;
		}

		void OnCombatStart(Unit* mTarget)
		{
			spells[0].casttime = 0;

			FourSpores = false;

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
			FourSpores = false;

			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			if(_unit->GetHealthPct() <= 20 && !FourSpores)
			{
				_unit->GetAIInterface()->StopMovement(11000);
				_unit->setAttackTimer(1200, false);

				_unit->CastSpell(_unit, spells[1].info, spells[1].instant);

				FourSpores = true;
			}

			else if(!_unit->FindAura(FOUL_SPORES))
			{
				uint32 t = (uint32)time(NULL);
				if(t > spells[0].casttime)
				{
					// Not yet added
					//CastSpellOnRandomTarget(0, 0.0f, 40.0f, 0, 100);

					spells[0].casttime = t + spells[0].cooldown;
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
					if(isHostile(_unit, (*itr)) && (*itr) != _unit && (*itr)->IsUnit())
					{
						Unit* RandomTarget = NULL;
						RandomTarget = TO_UNIT(*itr);

						if(RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast * mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast * maxdist2cast)
						{
							TargetTable.push_back(RandomTarget);
						}
					}
				}

				if(!TargetTable.size())
					return;

				size_t RandTarget = rand() % TargetTable.size();

				Unit*  RTarget = TargetTable[RandTarget];

				if(!RTarget)
				{
					TargetTable.clear();
					return;
				}
				/* Spawning mushroom - TO DO */

				TargetTable.clear();
			}
		}

	protected:

		bool FourSpores;
		int nrspells;
};

// Ghaz'anAI

#define CN_GHAZAN 18105

#define ACID_SPIT		34290
#define TAIL_SWEEP		34267
#define ACID_BREATH		24839
#define ENRAGE			15716 // Not sure to id as always in Enrage case: 34409, 34970

class GhazanAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(GhazanAI);
		SP_AI_Spell spells[4];
		bool m_spellcheck[4];

		GhazanAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 3;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;

			}

			spells[0].info = dbcSpell.LookupEntry(ACID_SPIT);
			spells[0].targettype = TARGET_VARIOUS;
			spells[0].instant = true;
			spells[0].cooldown = 20;
			spells[0].perctrigger = 8.0f;
			spells[0].attackstoptimer = 1000;

			spells[1].info = dbcSpell.LookupEntry(TAIL_SWEEP);
			spells[1].targettype = TARGET_VARIOUS;
			spells[1].instant = true;
			spells[1].cooldown = 25;
			spells[1].perctrigger = 7.0f;
			spells[1].attackstoptimer = 1000;

			spells[2].info = dbcSpell.LookupEntry(ACID_BREATH);
			spells[2].targettype = TARGET_VARIOUS;
			spells[2].instant = true;
			spells[2].cooldown = 15;
			spells[2].perctrigger = 10.0f;
			spells[2].attackstoptimer = 1000;

			spells[3].info = dbcSpell.LookupEntry(ENRAGE);
			spells[3].targettype = TARGET_SELF;
			spells[3].instant = true;
			spells[3].cooldown = 0;
			spells[3].perctrigger = 0.0f;
			spells[3].attackstoptimer = 1000;

			Enraged = false;
		}

		void OnCombatStart(Unit* mTarget)
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = 0;

			Enraged = false;

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
			if(_unit->GetHealthPct() <= 20 && !Enraged && _unit->GetCurrentSpell() == NULL)
			{
				_unit->CastSpell(_unit, spells[3].info, spells[3].instant);

				Enraged = true;
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

		bool Enraged;
		int nrspells;
};

// ClawAI

#define CN_CLAW 17827

#define MAUL				34298
#define CL_ECHOING_ROAR		31429
#define FERAL_CHARGE		39435
#define CL_ENRAGE			34971

class ClawAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(ClawAI);
		SP_AI_Spell spells[4];
		bool m_spellcheck[4];

		ClawAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 4;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(MAUL);
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = true;
			spells[0].cooldown = 15;
			spells[0].perctrigger = 15.0f;
			spells[0].attackstoptimer = 1000;

			spells[1].info = dbcSpell.LookupEntry(CL_ECHOING_ROAR);
			spells[1].targettype = TARGET_VARIOUS;
			spells[1].instant = true;
			spells[1].cooldown = 30;
			spells[1].perctrigger = 8.0f;
			spells[1].attackstoptimer = 1000;

			spells[2].info = dbcSpell.LookupEntry(FERAL_CHARGE);
			spells[2].targettype = TARGET_RANDOM_SINGLE;
			spells[2].instant = true;
			spells[2].cooldown = 3;
			spells[2].perctrigger = 18.0f;
			spells[2].attackstoptimer = 2000;
			spells[2].mindist2cast = 0.0f;
			spells[2].maxdist2cast = 40.0f;

			spells[3].info = dbcSpell.LookupEntry(CL_ENRAGE);
			spells[3].targettype = TARGET_SELF;
			spells[3].instant = true;
			spells[3].cooldown = 30;
			spells[3].perctrigger = 15.0f;
			spells[3].attackstoptimer = 1000;
		}

		void OnCombatStart(Unit* mTarget)
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = 0;

			spells[3].casttime = (uint32)time(NULL) + RandomUInt(10);

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

						if(i == 1)
						{
							Unit* Swamplord = NULL;
							Swamplord = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 17826);
							if(Swamplord && Swamplord->isAlive())
							{
								_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Beast, obey me! Kill them at once!");
								_unit->PlaySoundToSet(10383);
							}
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

						if(RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast * mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast * maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || isHostile(_unit, RandomTarget)))
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

// Swamplord Musel'ekAI
#define CN_SWAMPLORD_MUSELEK 17826

#define THROW_FREEZING_TRAP		31946	// needs more core support
#define KNOCK_AWAY_MUSELEK		18813
#define AIMED_SHOT				31623
#define MULTI_SHOT				30990
#define SHOT					32103

class SwamplordMuselekAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(SwamplordMuselekAI);
		SP_AI_Spell spells[5];
		bool m_spellcheck[5];

		SwamplordMuselekAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 2;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(THROW_FREEZING_TRAP);
			spells[0].targettype = TARGET_RANDOM_SINGLE; // not sure to target type
			spells[0].instant = true;
			spells[0].cooldown = 30;
			spells[0].perctrigger = 8.0f;
			spells[0].attackstoptimer = 1000;
			spells[0].mindist2cast = 0.0f;
			spells[0].maxdist2cast = 40.0f;

			spells[1].info = dbcSpell.LookupEntry(KNOCK_AWAY_MUSELEK);
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = true;
			spells[1].cooldown = 20;
			spells[1].perctrigger = 12.0f;
			spells[1].attackstoptimer = 1000;

			spells[2].info = dbcSpell.LookupEntry(AIMED_SHOT);
			spells[2].targettype = TARGET_ATTACKING;
			spells[2].instant = false;
			spells[2].cooldown = 20;
			spells[2].perctrigger = 0.0f;
			spells[2].attackstoptimer = 6500;

			spells[3].info = dbcSpell.LookupEntry(MULTI_SHOT);
			spells[3].targettype = TARGET_ATTACKING; // changed from VARIOUS to prevent crashes
			spells[3].instant = true;
			spells[3].cooldown = 15;
			spells[3].perctrigger = 0.0f;
			spells[3].attackstoptimer = 1000;

			spells[4].info = dbcSpell.LookupEntry(SHOT);
			spells[4].targettype = TARGET_ATTACKING;
			spells[4].instant = true;
			spells[4].cooldown = 0;
			spells[4].perctrigger = 0.0f;
			spells[4].attackstoptimer = 1000;
		}

		void OnCombatStart(Unit* mTarget)
		{
			for(int i = 0; i < 5; i++)
				spells[i].casttime = 0;

			int RandomSpeach = rand() % 3;
			switch(RandomSpeach)
			{
				case 0:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "We fight to the death!");
					_unit->PlaySoundToSet(10384);
					break;
				case 1:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I will end this quickly!");
					_unit->PlaySoundToSet(10385);
					break;
				case 2:
					//_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "<missing_text>!");
					_unit->PlaySoundToSet(10386);
					break;
			}

			Unit* Bear = NULL;
			Bear = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 17827);
			if(Bear && Bear->isAlive())
			{
				Bear->GetAIInterface()->AttackReaction(mTarget, 1, 0);
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
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Krypta!");
						_unit->PlaySoundToSet(10387);
						break;
					case 1:
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "It is finished!");
						_unit->PlaySoundToSet(10388);
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
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Well... done..."); // not sure
			_unit->PlaySoundToSet(10389);

			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			if(_unit->GetAIInterface()->getNextTarget())
			{
				Unit* target = NULL;
				target = _unit->GetAIInterface()->getNextTarget();
				if(_unit->GetDistance2dSq(target) >= 100.0f && _unit->GetDistanceSq(target) <= 900.0f && RandomUInt(3) != 1)
				{
					_unit->GetAIInterface()->StopMovement(2000);
					if(_unit->GetCurrentSpell() == NULL)
					{
						uint32 t = (uint32)time(NULL);
						int RangedSpell = rand() % 100;
						if(RangedSpell >= 0 && RangedSpell <= 20 && t > spells[2].casttime)
						{
							_unit->CastSpell(target, spells[2].info, spells[2].instant);
							_unit->setAttackTimer(spells[2].attackstoptimer, false);

							spells[2].casttime = t + spells[2].cooldown;
						}

						if(RangedSpell > 20 && RangedSpell <= 40 && t > spells[3].casttime)
						{
							_unit->CastSpell(target, spells[3].info, spells[3].instant);
							_unit->setAttackTimer(spells[3].attackstoptimer, false);

							spells[3].casttime = t + spells[3].cooldown;
						}

						else
						{
							_unit->CastSpell(target, spells[4].info, spells[4].instant);
							_unit->setAttackTimer(spells[4].attackstoptimer, false);
						}
					}
				}

				else if(_unit->GetDistance2dSq(target) < 100.0f)
				{
					float val = RandomFloat(100.0f);
					SpellCast(val);
				}
			}

			else return;
		}

		void SpellCast(float val)
		{
			if(_unit->GetCurrentSpell() == NULL)
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
						if(i != 1 || (i == 1 && _unit->GetDistance2dSq(target) <= 100.0f))
						{
							if(!spells[i].instant)
								_unit->GetAIInterface()->StopMovement(1);

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


// The Black StalkerAI

#define CN_THE_BLACK_STALKER 17882

#define CHAIN_LIGHTNING			31717 //39066 // 28167, 39066
#define LEVITATE				31704 // Not sure to id
#define STATIC_CHARGE			31715
#define SUMMON_SPORE_STRIDER	38755 // spawning adds only on Heroic! lack of core support =/

class TheBlackStalkerAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(TheBlackStalkerAI);
		SP_AI_Spell spells[4];
		bool m_spellcheck[4];

		TheBlackStalkerAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 3;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(CHAIN_LIGHTNING);
			spells[0].targettype = TARGET_RANDOM_SINGLE;
			spells[0].instant = false;
			spells[0].cooldown = 15;
			spells[0].perctrigger = 12.0f;
			spells[0].attackstoptimer = 1000;
			spells[0].mindist2cast = 0.0f;
			spells[0].maxdist2cast = 40.0f;

			spells[1].info = dbcSpell.LookupEntry(LEVITATE);
			spells[1].targettype = TARGET_RANDOM_SINGLE;
			spells[1].instant = true;
			spells[1].cooldown = 30;
			spells[1].perctrigger = 8.0f;
			spells[1].attackstoptimer = 1000;
			spells[1].mindist2cast = 0.0f;
			spells[1].maxdist2cast = 40.0f;

			spells[2].info = dbcSpell.LookupEntry(STATIC_CHARGE);
			spells[2].targettype = TARGET_RANDOM_SINGLE;
			spells[2].instant = true;
			spells[2].cooldown = 25;
			spells[2].perctrigger = 8.0f;
			spells[2].attackstoptimer = 1000;
			spells[2].mindist2cast = 0.0f;
			spells[2].maxdist2cast = 40.0f;

			spells[3].info = dbcSpell.LookupEntry(SUMMON_SPORE_STRIDER);
			spells[3].targettype = TARGET_SELF;
			spells[3].instant = true;
			spells[3].cooldown = 10;
			spells[3].perctrigger = 0.0f;
			spells[3].attackstoptimer = 1000;
		}

		void OnCombatStart(Unit* mTarget)
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = 0;

			spells[3].casttime = (uint32)time(NULL) + spells[3].cooldown + rand() % 6;

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
			uint32 t = (uint32)time(NULL);
			if(t > spells[3].casttime && _unit->GetCurrentSpell() == NULL)
			{
				_unit->CastSpell(_unit, spells[3].info, spells[3].instant);

				spells[3].casttime = (uint32)time(NULL) + spells[3].cooldown + rand() % 6;
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

void SetupTheUnderbog(ScriptMgr* mgr)
{
	/*mgr->register_creature_script(CN_BOG_GIANT, &BOGGIANTAI::Create);
	mgr->register_creature_script(CN_CLAW, &CLAWAI::Create);
	mgr->register_creature_script(CN_FEN_RAY, &FENRAYAI::Create);
	mgr->register_creature_script(CN_LYKUL_STINGER, &LYKULSTINGERAI::Create);
	mgr->register_creature_script(CN_UNDERBAT, &UNDERBATAI::Create);
	mgr->register_creature_script(CN_FEN_RAY, &FENRAYAI::Create);
	mgr->register_creature_script(CN_LYKUL_STINGER, &LYKULSTINGERAI::Create);
	mgr->register_creature_script(CN_LYKUL_WASP, &LYKULWASPAI::Create);
	mgr->register_creature_script(CN_WRATHFIN_WARRIOR, &WRATHFINWARRIORAI::Create);
	mgr->register_creature_script(CN_WRATHFIN_MYRMIDON, &WRATHFINMYRMIDONAI::Create);
	mgr->register_creature_script(CN_WRATHFIN_SENTRY, &WRATHFINSENTRYAI::Create);
	mgr->register_creature_script(CN_MURKBLOOD_SPEARMAN, &MURKBLOODSPEARMANAI::Create);
	mgr->register_creature_script(CN_MURKBLOOD_ORACLE, &MURKBLOODORACLEAI::Create);
	mgr->register_creature_script(CN_MURKBLOOD_HEALER, &MURKBLOODHEALERAI::Create);
	mgr->register_creature_script(CN_MURKBLOOD_TRIBESMAN, &MURKBLOODTRIBESMANAI::Create);
	mgr->register_creature_script(CN_UNDERBOG_SHAMBLER, &UNDERBOGSHAMBLERAI::Create);
	mgr->register_creature_script(CN_UNDERBOG_FRENZY, &UNDERBOGFRENZYAI::Create);
	mgr->register_creature_script(CN_UNDERBOG_LORD, &UNDERBOGLORDAI::Create);
	mgr->register_creature_script(CN_UNDERBOG_LURKER, &UNDERBOGLURKERAI::Create);*/
	mgr->register_creature_script(CN_HUNGARFEN, &HUNGARFENAI::Create);
	mgr->register_creature_script(CN_GHAZAN, &GhazanAI::Create);
	mgr->register_creature_script(CN_CLAW, &ClawAI::Create);
	mgr->register_creature_script(CN_SWAMPLORD_MUSELEK, &SwamplordMuselekAI::Create);
	mgr->register_creature_script(CN_THE_BLACK_STALKER, &TheBlackStalkerAI::Create);
}

// Notes: Wasp/Stinger must be checked. Please check it (because for sure
// many spells/creatures with spells are missing and also you will find some dupes.
// No spells found for: Windcaller Claw, Spore Spider, Earthbinder Rayge
// Left Underbog Mushroom.