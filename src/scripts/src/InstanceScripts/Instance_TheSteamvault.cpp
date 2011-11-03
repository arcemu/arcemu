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
/* Instance_TheSteamvault.cpp Script		                            */
/************************************************************************/

// Coilfang Engineer AI

#define CN_COILFANG_ENGINEER 17721

#define BOMB 22334 //40332 // AOE
#define NET 6533 //38338 // Should stay for 5 or 8 sec?

class COILFANGENGINEERAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(COILFANGENGINEERAI);
		SP_AI_Spell spells[2];
		bool m_spellcheck[2];

		COILFANGENGINEERAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 2;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(BOMB);
			spells[0].targettype = TARGET_DESTINATION;
			spells[0].instant = false;
			spells[0].cooldown = 10;
			spells[0].perctrigger = 0.0f;
			spells[0].attackstoptimer = 1000;

			spells[1].info = dbcSpell.LookupEntry(NET);
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


// Coilfang Oracle AI


#define CN_COILFANG_ORACLE 17803

#define FROST_SHOCK 22582 //34353
#define SONIC_BURST 8281 //39052 // Should make dmg? or maybe it shouldn't be added here?
#define HEAL 31730 //39378 // Hmm... no idea if this is correct id (and how much should heal)
// Also it should heal other naga like etc.
class COILFANGORACLEAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(COILFANGORACLEAI);
		SP_AI_Spell spells[3];
		bool m_spellcheck[3];

		COILFANGORACLEAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 3;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(FROST_SHOCK);
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = true;
			spells[0].cooldown = 15;
			spells[0].perctrigger = 0.0f;
			spells[0].attackstoptimer = 1000;

			spells[1].info = dbcSpell.LookupEntry(SONIC_BURST);
			spells[1].targettype = TARGET_VARIOUS;
			spells[1].instant = true;
			spells[1].cooldown = 25;
			spells[1].perctrigger = 0.0f;
			spells[1].attackstoptimer = 1000;

			spells[2].info = dbcSpell.LookupEntry(HEAL);
			spells[2].targettype = TARGET_SELF;
			spells[2].instant = false;
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

// Coilfang Warrior AI

#define CN_COILFANG_WARRIOR 17802  // at least, couldn't find any data to compare it with blizz

//#define MORTAL_STRIKE 29572	// should it really be here?
#define MORTAL_BLOW 35105
#define BATTLE_SHOUT 31403
// Defensive Stance 7164 ?
class COILFANGWARRIORAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(COILFANGWARRIORAI);
		SP_AI_Spell spells[2];
		bool m_spellcheck[2];

		COILFANGWARRIORAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 2;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}
			/*
					spells[0].info = dbcSpell.LookupEntry(MORTAL_STRIKE);
					spells[0].targettype = TARGET_ATTACKING;
					spells[0].instant = false;
					spells[0].cooldown = ;
					spells[0].perctrigger = 0.0f;
					spells[0].attackstoptimer = 1000;
			*/
			spells[0].info = dbcSpell.LookupEntry(MORTAL_BLOW);
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = true;
			spells[0].cooldown = 15;
			spells[0].perctrigger = 0.0f;
			spells[0].attackstoptimer = 1000;

			spells[1].info = dbcSpell.LookupEntry(BATTLE_SHOUT);	// should affect friends not enemies :S
			spells[1].targettype = TARGET_VARIOUS;
			spells[1].instant = true;
			spells[1].cooldown = 35;
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


// Coilfang Siren AI

#define CN_COILFANG_SIREN 17801

//#define MOONFIRE 20690 // not sure to id	// not used ?
#define AOE_FEAR 29321 //30584 // Maybe should be: 33547 or 29321
#define LIGHTNING_BOLT 15234 //36152 // 1 target
#define ARCANE_FLARE 35106

class COILFANGSIRENAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(COILFANGSIRENAI);
		SP_AI_Spell spells[3];
		bool m_spellcheck[3];

		COILFANGSIRENAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 3;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(AOE_FEAR);
			spells[0].targettype = TARGET_VARIOUS;
			spells[0].instant = true;
			spells[0].cooldown = 35;
			spells[0].perctrigger = 0.0f;
			spells[0].attackstoptimer = 1000;

			spells[1].info = dbcSpell.LookupEntry(LIGHTNING_BOLT);
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = false;
			spells[1].cooldown = 25;
			spells[1].perctrigger = 0.0f;
			spells[1].attackstoptimer = 1000;

			spells[2].info = dbcSpell.LookupEntry(ARCANE_FLARE);
			spells[2].targettype = TARGET_ATTACKING;
			spells[2].instant = true;
			spells[2].cooldown = 15;
			spells[2].perctrigger = 0.0f;
			spells[2].attackstoptimer = 1000;
			/*
					spells[2].info = dbcSpell.LookupEntry(MOONFIRE);
					spells[2].targettype = TARGET_ATTACKING;
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



// Bog OverlordAI

#define CN_BOG_OVERLORD 21694

#define FUNGAL_DECAY 32065
#define TRAMPLE 15550 //40340 // Should be used when enraged
#define ENRAGE_BOG_OVERLORD 8599 //40683 // No idea if this is good spell id (used ENRAGE_B [B = Bog], because of redefining)
#define DISEASE_CLOUD 37266 //DBC: 37266, 37267 || Heroic: 37863, 37864 ?
// Should be immunite on CC's and slowing down effects.
// Also should detect players in stealth mode. (Invisibility and Stealth Detection 18950 ?)

class BOGOVERLORDAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(BOGOVERLORDAI);
		SP_AI_Spell spells[4];
		bool m_spellcheck[4];

		BOGOVERLORDAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{

			nrspells = 4;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(FUNGAL_DECAY);
			spells[0].targettype = TARGET_ATTACKING;	// should be random (as in many other spells)
			spells[0].instant = true;
			spells[0].cooldown = 15;
			spells[0].perctrigger = 0.0f;
			spells[0].attackstoptimer = 1000;

			spells[1].info = dbcSpell.LookupEntry(TRAMPLE);
			spells[1].targettype = TARGET_VARIOUS;
			spells[1].instant = true;
			spells[1].cooldown = 10;
			spells[1].perctrigger = 0.0f;
			spells[1].attackstoptimer = 1000;

			spells[2].info = dbcSpell.LookupEntry(ENRAGE_BOG_OVERLORD);
			spells[2].targettype = TARGET_SELF;
			spells[2].instant = false;
			spells[2].cooldown = 55;
			spells[2].perctrigger = 0.0f;
			spells[2].attackstoptimer = 1000;

			spells[3].info = dbcSpell.LookupEntry(DISEASE_CLOUD);
			spells[3].targettype = TARGET_SELF;
			spells[3].instant = true;
			spells[3].cooldown = -1;	// not sure to this and...
			spells[3].perctrigger = 0.0f;
			spells[3].attackstoptimer = 1000;

			_unit->CastSpell(_unit, spells[3].info, spells[3].instant); // ...and this
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

// Coilfang Sorceress

#define CN_COILFANG_SORCERESS 17722

#define FROSTBOLT 12675 //39064
#define BLIZZARD 39416 //38646 // or maybe: 37263
#define FROST_NOVA 15063 //29849 // also can be: 30094 or 32365

class COILFANGSORCERESSAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(COILFANGSORCERESSAI);
		SP_AI_Spell spells[3];
		bool m_spellcheck[3];

		COILFANGSORCERESSAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 3;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(FROSTBOLT);
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = false;
			spells[0].cooldown = 10;
			spells[0].perctrigger = 0.0f;
			spells[0].attackstoptimer = 1000;

			spells[1].info = dbcSpell.LookupEntry(BLIZZARD);
			spells[1].targettype = TARGET_DESTINATION;
			spells[1].instant = false;
			spells[1].cooldown = 35;
			spells[1].perctrigger = 0.0f;
			spells[1].attackstoptimer = 1000;

			spells[2].info = dbcSpell.LookupEntry(FROST_NOVA);
			spells[2].targettype = TARGET_VARIOUS;
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

// Coilfang Leper

#define CN_COILFANG_LEPER 21338 // Couldn't find source to compare it's spells

#define SHADOW_BOLT 9613 //39025
#define FIRE_BLAST_LEPER 13339 // not sure to those
#define STRIKE 13446
#define FROST_NOVA_LEPER 11831
#define CLEAVE_LEPER 5532		// THOSE SPELLS MUST BE VERIFIED!
#define HEAL_LEPER 11642
#define SUNDER_ARMOR_LEPER 13444
#define SHOOT 15547

class COILFANGLEPERAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(COILFANGLEPERAI);
		SP_AI_Spell spells[8];
		bool m_spellcheck[8];

		COILFANGLEPERAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 8;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(SHADOW_BOLT);
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = false;
			spells[0].cooldown = 15;
			spells[0].perctrigger = 0.0f;
			spells[0].attackstoptimer = 1000;

			spells[1].info = dbcSpell.LookupEntry(FIRE_BLAST_LEPER);
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = true;
			spells[1].cooldown = 25;
			spells[1].perctrigger = 0.0f;
			spells[1].attackstoptimer = 1000;

			spells[2].info = dbcSpell.LookupEntry(STRIKE);
			spells[2].targettype = TARGET_ATTACKING;
			spells[2].instant = true;
			spells[2].cooldown = 35;
			spells[2].perctrigger = 0.0f;
			spells[2].attackstoptimer = 1000;

			spells[3].info = dbcSpell.LookupEntry(FROST_NOVA_LEPER);
			spells[3].targettype = TARGET_ATTACKING;
			spells[3].instant = true;
			spells[3].cooldown = 55;
			spells[3].perctrigger = 0.0f;
			spells[3].attackstoptimer = 1000;

			spells[4].info = dbcSpell.LookupEntry(CLEAVE_LEPER);
			spells[4].targettype = TARGET_VARIOUS;	// ?
			spells[4].instant = true;
			spells[4].cooldown = 45;
			spells[4].perctrigger = 0.0f;
			spells[4].attackstoptimer = 1000;

			spells[5].info = dbcSpell.LookupEntry(HEAL_LEPER);
			spells[5].targettype = TARGET_SELF;
			spells[5].instant = false;
			spells[5].cooldown = 85;
			spells[5].perctrigger = 0.0f;
			spells[5].attackstoptimer = 1000;

			spells[6].info = dbcSpell.LookupEntry(SUNDER_ARMOR_LEPER);
			spells[6].targettype = TARGET_ATTACKING;
			spells[6].instant = true;
			spells[6].cooldown = 95;
			spells[6].perctrigger = 0.0f;
			spells[6].attackstoptimer = 1000;

			spells[7].info = dbcSpell.LookupEntry(SHOOT);
			spells[7].targettype = TARGET_ATTACKING;
			spells[7].instant = true;
			spells[7].cooldown = -1;	// disabled for now
			spells[7].perctrigger = 0.0f;
			spells[7].attackstoptimer = 1000;

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

// Coilfang Slavemaster

#define CN_COILFANG_SLAVEMASTER 17805

#define GEYSER 10987 //40089 // It don't have to be good spell id [can be: 10987, 37478, 40089, 37427] - no idea why it doesn't knock you out
#define ENRAGE_SlAVEMASTER 8269
// Slaves should run after killing slavemaster

class COILFANGSLAVEMASTERAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(COILFANGSLAVEMASTERAI);
		SP_AI_Spell spells[2];
		bool m_spellcheck[2];

		COILFANGSLAVEMASTERAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 2;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(GEYSER);
			spells[0].targettype = TARGET_VARIOUS;
			spells[0].instant = true;
			spells[0].cooldown = 20;
			spells[0].perctrigger = 0.0f;
			spells[0].attackstoptimer = 1000;

			spells[1].info = dbcSpell.LookupEntry(ENRAGE_SlAVEMASTER);
			spells[1].targettype = TARGET_VARIOUS;
			spells[1].instant = true;
			spells[1].cooldown = 55;
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

// Coilfang water elemental

#define CN_COILFANG_WATER_ELEMENTAL 17917

//#define FROSTBOLT_VOLLEY 36741
#define WATER_BOLT_WOLLEY 34449

class COILFANGWATERELEMENTALAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(COILFANGWATERELEMENTALAI);
		SP_AI_Spell spells[1];
		bool m_spellcheck[1];

		COILFANGWATERELEMENTALAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 1;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}
			/*
			        spells[0].info = dbcSpell.LookupEntry(FROSTBOLT_VOLLEY);
					spells[0].targettype = TARGET_ATTACKING;
					spells[0].instant = true;		// Should be false, but doesn't work then
					spells[0].cooldown = ;
					spells[0].perctrigger = 0.0f;
					spells[0].attackstoptimer = 1000;
			*/
			spells[0].info = dbcSpell.LookupEntry(WATER_BOLT_WOLLEY);
			spells[0].targettype = TARGET_VARIOUS;
			spells[0].instant = false;
			spells[0].cooldown = 10;
			spells[0].perctrigger = 0.0f;
			spells[0].attackstoptimer = 1000;

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

// Coilfang MyrmidonAI

#define CN_COILFANG_MYRMIDON 17800

#define SWEEPING_STRIKES 18765 // DBC: 18765, 12723 //35429
#define CLEAVE 15622 //38260 // no idea about it's id
#define EXECUTE 7160 //38959 // should activate only on target with <= 25% hp // not sure about it

class COILFANGMYRMIDONAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(COILFANGMYRMIDONAI);
		SP_AI_Spell spells[3];
		bool m_spellcheck[3];

		COILFANGMYRMIDONAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 3;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(SWEEPING_STRIKES);
			spells[0].targettype = TARGET_SELF;
			spells[0].instant = true;
			spells[0].cooldown = 25;
			spells[0].perctrigger = 0.0f;
			spells[0].attackstoptimer = 1000;

			spells[1].info = dbcSpell.LookupEntry(CLEAVE);
			spells[1].targettype = TARGET_VARIOUS;	// not sure
			spells[1].instant = true;
			spells[1].cooldown = 15;
			spells[1].perctrigger = 0.0f;
			spells[1].attackstoptimer = 1000;

			spells[2].info = dbcSpell.LookupEntry(EXECUTE);
			spells[2].targettype = TARGET_ATTACKING;
			spells[2].instant = true;
			spells[2].cooldown = 35;
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

// Tidal SurgerAI

#define CN_TIDAL_SURGER 21695

//#define KNOCKBACK 30056
#define WATER_SPOUT 37250
#define FROST_NOVA_SURGER 15531
// Should has immunite to frost spells (as adds) + should has
// 3-4 minons (Idk if they should be spawned by script)

class TIDALSURGERAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(TIDALSURGERAI);
		SP_AI_Spell spells[2];
		bool m_spellcheck[2];

		TIDALSURGERAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 2;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}
			/*
					spells[0].info = dbcSpell.LookupEntry(KNOCKBACK);
					spells[0].targettype = TARGET_ATTACKING; // should be random
					spells[0].instant = true;
					spells[0].cooldown = ;
					spells[0].perctrigger = 0.0f;
					spells[0].attackstoptimer = 1000;
			*/
			spells[0].info = dbcSpell.LookupEntry(WATER_SPOUT);
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = true;
			spells[0].cooldown = 25;
			spells[0].perctrigger = 0.0f;
			spells[0].attackstoptimer = 1000;

			spells[1].info = dbcSpell.LookupEntry(FROST_NOVA_SURGER);
			spells[1].targettype = TARGET_VARIOUS;
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

// Steamrigger MechanicAI

#define CN_STEAMRIGGER_MECHANIC 17951

#define REPAIR 31532

class STEAMRIGGERMECHANICAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(STEAMRIGGERMECHANICAI);
		SP_AI_Spell spells[1];
		bool m_spellcheck[1];

		STEAMRIGGERMECHANICAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 1;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(REPAIR);
			spells[0].targettype = TARGET_SELF;	//Should be mechanical ally =/
			spells[0].instant = false;
			spells[0].cooldown = 10;
			spells[0].perctrigger = 0.0f;
			spells[0].attackstoptimer = 1000;

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

// Steam SurgerAI

#define CN_STEAM_SURGER 21696

#define WATER_BOLT 37252

class STEAMSURGERAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(STEAMSURGERAI);
		SP_AI_Spell spells[1];
		bool m_spellcheck[1];

		STEAMSURGERAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 1;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(WATER_BOLT);
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = true;
			spells[0].cooldown = 15;
			spells[0].perctrigger = 0.0f;
			spells[0].attackstoptimer = 1000;

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

/*****************************/
/*                           */
/*         Boss AIs          */
/*                           */
/*****************************/

// Hydromancer ThespiaAI

#define CN_HYDROMANCER_THESPIA 17797

#define ENVELOPING_WINDS 31718
#define LIGHTNING_CLOUD 25033
#define LUNG_BURST 31481

class HydromancerThespiaAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(HydromancerThespiaAI);
		SP_AI_Spell spells[3];
		bool m_spellcheck[3];

		HydromancerThespiaAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 3;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;

			}

			spells[0].info = dbcSpell.LookupEntry(ENVELOPING_WINDS);
			spells[0].targettype = TARGET_RANDOM_SINGLE;
			spells[0].instant = true;
			spells[0].cooldown = 10;
			spells[0].perctrigger = 9.0f;
			spells[0].attackstoptimer = 1000;
			spells[0].mindist2cast = 0.0f;
			spells[0].maxdist2cast = 35.0f;

			spells[1].info = dbcSpell.LookupEntry(LIGHTNING_CLOUD);
			spells[1].targettype = TARGET_RANDOM_DESTINATION;
			spells[1].instant = true;
			spells[1].cooldown = 25;
			spells[1].perctrigger = 8.0f;
			spells[1].attackstoptimer = 1000;
			spells[1].mindist2cast = 0.0f;
			spells[1].maxdist2cast = 30.0f;

			spells[2].info = dbcSpell.LookupEntry(LUNG_BURST);
			spells[2].targettype = TARGET_RANDOM_SINGLE;
			spells[2].instant = true;
			spells[2].cooldown = 15;
			spells[2].perctrigger = 9.0f;
			spells[2].attackstoptimer = 1000;
			spells[2].mindist2cast = 0.0f;
			spells[2].maxdist2cast = 40.0f;
		}

		void OnCombatStart(Unit* mTarget)
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = 0;

			int RandomSpeach = rand() % 3;
			switch(RandomSpeach)
			{
				case 0:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The depths will consume you!");
					_unit->PlaySoundToSet(10361);
					break;
				case 1:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Meet your doom, surface dwellers!");
					_unit->PlaySoundToSet(10362);
					break;
				case 2:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You will drown in blood!");
					_unit->PlaySoundToSet(10363);
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
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "To the depths of oblivion with you!");
						_unit->PlaySoundToSet(10364);
						break;
					case 1:
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "For my Lady and Master.");
						_unit->PlaySoundToSet(10365);
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
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Our matron will be ... The end of you");
			_unit->PlaySoundToSet(10366);

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

//---- Steamrigger encounter ----//

static Location SpawnCoords[] =
{
	{ -300.037842f, -115.296227f, -7.865229f, 4.197916f },
	{ -330.083008f, -121.505997f, -7.985120f, 5.061450f },
	{ -346.530273f, -147.167892f, -6.703687f, 0.010135f }
};

// Steamrigger MechanicAI
// Should they really fight?
#define CN_STEAMRIGGER_MECHANIC		17951

#define REPAIR	31532

class SteamriggerMechanicAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(SteamriggerMechanicAI);

		SteamriggerMechanicAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
			_unit->m_noRespawn = true;
		}

		void OnCombatStart(Unit* mTarget)
		{
			_unit->GetAIInterface()->ResetUnitToFollow();
			_unit->GetAIInterface()->SetUnitToFollowAngle(0.0f);

			if(_unit->GetCurrentSpell() != NULL)
				_unit->GetCurrentSpell()->cancel();
		}

		void OnCombatStop(Unit* mTarget)
		{
			_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		}

		void OnTargetDied(Unit* mTarget)
		{
			_unit->GetAIInterface()->RemoveThreatByPtr(mTarget);
		}

		void OnDied(Unit* mKiller)
		{
			_unit->GetAIInterface()->ResetUnitToFollow();
			_unit->GetAIInterface()->SetUnitToFollowAngle(0.0f);

			RemoveAIUpdateEvent();
		}

		void OnDamageTaken(Unit* mAttacker, uint32 fAmount)
		{
			_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
		}

};


// Mekgineer Steamrigger

#define CN_MEKGINEER_STEAMRIGGER	17796

#define SUPER_SHRINK_RAY	31485
#define SAW_BLADE			31486
#define ELECTRIFIED_NET		35107
#define ENRAGE				41447  // No idea if this is good id

// Must spawn 3 Steamrigger Mechanics when his health is on 75%, 50% and 25%
class MekgineerSteamriggerAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(MekgineerSteamriggerAI);
		SP_AI_Spell spells[4];
		bool m_spellcheck[4];
		std::vector <Unit*> Gnomes;

		MekgineerSteamriggerAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 3;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(SUPER_SHRINK_RAY);
			spells[0].targettype = TARGET_RANDOM_SINGLE;
			spells[0].instant = true;
			spells[0].cooldown = 20;
			spells[0].perctrigger = 9.0f;
			spells[0].attackstoptimer = 1000;
			spells[0].mindist2cast = 0.0f;
			spells[0].maxdist2cast = 40.0f;

			spells[1].info = dbcSpell.LookupEntry(SAW_BLADE);
			spells[1].targettype = TARGET_RANDOM_SINGLE; // when killed with VARIOUS (because with that caster attacks also himself) server crashes
			spells[1].instant = true;
			spells[1].cooldown = 15;
			spells[1].perctrigger = 10.0f;
			spells[1].attackstoptimer = 1000;
			spells[1].mindist2cast = 0.0f;
			spells[1].maxdist2cast = 40.0f;

			spells[2].info = dbcSpell.LookupEntry(ELECTRIFIED_NET);
			spells[2].targettype = TARGET_RANDOM_SINGLE;
			spells[2].instant = true;
			spells[2].cooldown = 15;
			spells[2].perctrigger = 8.0f;
			spells[2].attackstoptimer = 1000;
			spells[2].mindist2cast = 0.0f;
			spells[2].maxdist2cast = 40.0f;

			spells[3].info = dbcSpell.LookupEntry(ENRAGE);
			spells[3].targettype = TARGET_SELF;
			spells[3].instant = true;
			spells[3].cooldown = 300;
			spells[3].perctrigger = 0.0f;
			spells[3].attackstoptimer = 1000;

			GnomeCounter = 0;
		}

		void OnCombatStart(Unit* mTarget)
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = 0;
			spells[3].casttime = (uint32)time(NULL) + spells[3].cooldown;

			GnomeCounter = 0;

			Gnomes.clear();

			int RandomSpeach = rand() % 4;
			switch(RandomSpeach)
			{
				case 0:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Your in for a World-O-Hurt!");
					_unit->PlaySoundToSet(10368);
					break;
				case 1:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Eat hot metal! Scumbag!");
					_unit->PlaySoundToSet(10369);
					break;
				case 2:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I'll come over there!");
					_unit->PlaySoundToSet(10370);
					break;
				case 3:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I'm bringin' the pain!");
					_unit->PlaySoundToSet(10371);
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
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You just got served, Punk!");
						_unit->PlaySoundToSet(10372);
						break;
					case 1:
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I own you!");
						_unit->PlaySoundToSet(10373);
						break;
				}
			}
		}

		void OnCombatStop(Unit* mTarget)
		{
			for(size_t i = 0; i < Gnomes.size(); i++)
			{
				Creature* Gnome = NULL;
				Gnome = TO_CREATURE(Gnomes[i]);
				if(!Gnome)
					continue;

				if(!Gnome->IsInWorld() || !Gnome->isAlive())
					continue;

				Gnome->Despawn(0, 0);
			}

			Gnomes.clear();

			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);

			RemoveAIUpdateEvent();
		}

		void OnDied(Unit* mKiller)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Mommy");
			_unit->PlaySoundToSet(10375);

			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			if(Gnomes.size())
			{
				Unit* Gnome = NULL;
				for(std::vector<Unit*>::iterator itr = Gnomes.begin(); itr < Gnomes.end(); ++itr)
				{
					Gnome = *itr;
					if(!Gnome->isAlive() || !Gnome->IsInWorld())
					{
						Gnomes.erase(itr);
						continue;
					}

					if(Gnome->GetAIInterface()->getNextTarget())
						continue;

					if(Gnome->GetAIInterface()->getUnitToFollow() == NULL)
					{
						Gnome->GetAIInterface()->SetUnitToFollow(_unit);
						Gnome->GetAIInterface()->SetFollowDistance(15.0f);
					}

					if(_unit->GetDistance2dSq(Gnome) > 250.0f)
					{
						if(Gnome->GetCurrentSpell() != NULL)
							Gnome->GetCurrentSpell()->cancel();

						continue;
					}

					if(Gnome->GetCurrentSpell() == NULL)
					{
						Gnome->GetAIInterface()->StopMovement(1);
						Gnome->CastSpell(_unit, REPAIR, false);	// core problem? casted on self (and effect is applied on caster instead of _unit)
					}
				}
			}

			uint32 t = (uint32)time(NULL);
			if(t > spells[3].casttime)
			{
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Have fun dyin', cupcake!");
				_unit->PlaySoundToSet(10374);

				_unit->CastSpell(_unit, spells[3].info, spells[3].instant);

				spells[3].casttime = t + spells[3].cooldown;
			}

			if((_unit->GetHealthPct() <= 75 && GnomeCounter == 0) || (_unit->GetHealthPct() <= 50 && GnomeCounter == 1) || (_unit->GetHealthPct() <= 25 && GnomeCounter == 2))
			{
				Unit* Gnome = NULL;
				for(int i = 0; i < 3; i++)
				{
					Gnome = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_STEAMRIGGER_MECHANIC, SpawnCoords[i].x, SpawnCoords[i].y, SpawnCoords[i].z, SpawnCoords[i].o, true, false, _unit->GetFaction(), 50);
					if(Gnome)
					{
						Gnome->GetAIInterface()->SetUnitToFollow(_unit);
						Gnome->GetAIInterface()->SetFollowDistance(15.0f);
						Gnomes.push_back(Gnome);
					}
				}

				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Tune 'em up good boys!");
				_unit->PlaySoundToSet(10367);

				GnomeCounter++;
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

		uint8 GnomeCounter;
		int nrspells;
};

//---- Warlord Kalitresh Encounter ----//

static Location Distiller[] =
{
	{  },
	{ -113.183952f, -488.599335f, 8.196310f, 6.134734f },
	{  -76.880989f, -489.164673f, 8.236189f, 2.103285f },
	{  -74.309753f, -520.418884f, 8.255078f, 4.612630f },
	{ -116.220764f, -520.139771f, 8.198921f, 5.127069f }
};

static Location DistillerMoveTo[] =
{
	{  },
	{ -108.092949f, -491.747803f, 8.198845f,  0.621336f },
	{  -81.165871f, -492.459869f, 8.255936f,  6.531955f },
	{  -79.170982f, -518.544800f, 8.241381f, -2.281880f },
	{ -112.033188f, -517.945190f, 8.205022f, -0.949258f }
};

// Naga DistillerAI

#define CN_NAGA_DISTILLER	17954

#define REPAIR	31532

class NagaDistillerAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(NagaDistillerAI);

		NagaDistillerAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
			_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
			_unit->GetAIInterface()->disable_melee = true;
			_unit->GetAIInterface()->m_canMove = false;
		}

		void OnDied(Unit* mKiller)
		{
			_unit->SetChannelSpellTargetGUID(0);
			_unit->SetChannelSpellId(0);
		}
};

// Warlord Kalitresh AI

#define CN_WARLORD_KALITRESH 17798

#define IMPALE				34451
#define HEAD_CRACK			16172
#define SPELL_REFLECTION	31534
#define WARLORDS_RAGE		36453 // DBC: 37081, 36453	// still he must be forced by Driller to cast it

class WarlordKalitreshAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(WarlordKalitreshAI);
		SP_AI_Spell spells[4];
		bool m_spellcheck[4];

		WarlordKalitreshAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 3;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(IMPALE);
			spells[0].targettype = TARGET_RANDOM_SINGLE;
			spells[0].instant = true;
			spells[0].cooldown = 10;
			spells[0].perctrigger = 8.0f;
			spells[0].attackstoptimer = 1000;
			spells[0].mindist2cast = 0.0f;
			spells[0].maxdist2cast = 40.0f;

			spells[1].info = dbcSpell.LookupEntry(HEAD_CRACK);
			spells[1].targettype = TARGET_RANDOM_SINGLE;
			spells[1].instant = true;
			spells[1].cooldown = 10;
			spells[1].perctrigger = 8.0f;
			spells[1].attackstoptimer = 1000;
			spells[1].mindist2cast = 0.0f;
			spells[1].maxdist2cast = 40.0f;

			spells[2].info = dbcSpell.LookupEntry(SPELL_REFLECTION);
			spells[2].targettype = TARGET_SELF;
			spells[2].instant = true;
			spells[2].cooldown = 25;
			spells[2].perctrigger = 8.0f;
			spells[2].attackstoptimer = 1000;

			spells[3].info = dbcSpell.LookupEntry(WARLORDS_RAGE);
			spells[3].targettype = TARGET_SELF;
			spells[3].instant = true;
			spells[3].cooldown = 90;
			spells[3].perctrigger = 0.0f;
			spells[3].attackstoptimer = 1000;
			spells[3].soundid = 10391;
			spells[3].speech = "This is not nearly over!";

			DistillerNumber = 0;
			RagePhaseTimer = 0;
			EnrageTimer = 0;
			RagePhase = 0;
		}

		void OnCombatStart(Unit* mTarget)
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = 0;

			RagePhaseTimer = (uint32)time(NULL) + RandomUInt(15) + 10;
			DistillerNumber = 0;
			EnrageTimer = 0;
			RagePhase = 0;

			GameObject* Gate = NULL;
			Gate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-95.774361f, -439.608612f, 3.382976f, 183049);
			if(Gate)
				Gate->SetState(1);

			int RandomSpeach = rand() % 3;
			switch(RandomSpeach)
			{
				case 0:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Your head will roll!");
					_unit->PlaySoundToSet(10392);
					break;
				case 1:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I despise all of your kind!");
					_unit->PlaySoundToSet(10393);
					break;
				case 2:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Ba'anthalso'dorei!");
					_unit->PlaySoundToSet(10394);
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
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Scram, surface filth!");
						_unit->PlaySoundToSet(10395);
						break;
					case 1:
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Ah ha ha ha ha ha ha!");
						_unit->PlaySoundToSet(10366);
						break;
				}
			}
		}

		void OnCombatStop(Unit* mTarget)
		{
			GameObject* Gate = NULL;
			Gate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-95.774361f, -439.608612f, 3.382976f, 183049);
			if(Gate)
				Gate->SetState(0);

			_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
			_unit->GetAIInterface()->m_canMove = true;
			_unit->GetAIInterface()->ResetUnitToFollow();
			_unit->GetAIInterface()->SetFollowDistance(0.0f);
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);

			if(_unit->FindAura(37076))
				_unit->RemoveAura(37076);
			if(_unit->FindAura(36453))
				_unit->RemoveAura(36453);

			Unit* pDistiller = NULL;
			pDistiller = GetClosestDistiller();
			if(pDistiller)
			{
				pDistiller->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
				pDistiller->SetChannelSpellTargetGUID(0);
				pDistiller->SetChannelSpellId(0);
				pDistiller->GetAIInterface()->WipeTargetList();
				pDistiller->GetAIInterface()->WipeHateList();
			}

			RemoveAIUpdateEvent();
		}

		void OnDied(Unit* mKiller)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "For her Excellency... for... Vashj!");
			_unit->PlaySoundToSet(10397);

			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			uint32 t = (uint32)time(NULL);
			if(t > RagePhaseTimer)
			{
				if(EnrageTimer != 0)
					EnrageTimer++;

				Unit* pDistiller = NULL;
				pDistiller = GetClosestDistiller();
				if(!pDistiller || (pDistiller->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9) && RagePhase != 0))
				{
					_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
					_unit->GetAIInterface()->m_canMove = true;
					_unit->GetAIInterface()->ResetUnitToFollow();
					_unit->GetAIInterface()->SetFollowDistance(0.0f);

					RagePhaseTimer = t + RandomUInt(15) + 20;
					EnrageTimer = 0;
					RagePhase = 0;

					if(_unit->FindAura(31543))
						_unit->RemoveAura(31543);
					if(_unit->FindAura(37076))
						_unit->RemoveAura(37076);
				}

				else
				{
					if(EnrageTimer == 0 && RagePhase == 0)
					{
						_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
						_unit->GetAIInterface()->SetUnitToFollow(pDistiller);
						_unit->GetAIInterface()->SetFollowDistance(10.0f);

						_unit->GetAIInterface()->StopMovement(0);
						_unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
						_unit->GetAIInterface()->MoveTo(DistillerMoveTo[DistillerNumber].x, DistillerMoveTo[DistillerNumber].y, DistillerMoveTo[DistillerNumber].z, DistillerMoveTo[DistillerNumber].o);

						if(_unit->GetDistance2dSq(pDistiller) <= 100.0f)
						{
							pDistiller->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
							pDistiller->SetChannelSpellTargetGUID(_unit->GetGUID());
							pDistiller->SetChannelSpellId(31543);

							_unit->GetAIInterface()->StopMovement(0);
							_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
							_unit->GetAIInterface()->m_canMove = false;
							_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "This is not nearly over...");
							_unit->PlaySoundToSet(10394);

							if(!_unit->FindAura(36453))
								_unit->CastSpell(_unit, 31543, true);

							EnrageTimer = t + 3;
							RagePhase = 1;
						}
					}

					else if(t > EnrageTimer && RagePhase == 1)
					{
						EnrageTimer = t + 6;	// 9
						RagePhase = 2;
					}

					else if(t > EnrageTimer && RagePhase == 2)
					{
						pDistiller->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
						pDistiller->SetChannelSpellTargetGUID(0);
						pDistiller->SetChannelSpellId(0);
						pDistiller->GetAIInterface()->WipeTargetList();
						pDistiller->GetAIInterface()->WipeHateList();

						_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
						_unit->GetAIInterface()->m_canMove = true;
						_unit->GetAIInterface()->ResetUnitToFollow();
						_unit->GetAIInterface()->SetFollowDistance(0.0f);
						_unit->CastSpell(_unit, 36453, true);

						RagePhaseTimer = t + RandomUInt(15) + 20;
						DistillerNumber = 0;
						EnrageTimer = 0;
						RagePhase = 0;
					}
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

		Unit* GetClosestDistiller()
		{
			float distance = 50.0f;
			Unit* pDistiller = NULL;
			Unit* Unit2Check = NULL;

			for(int i = 1; i < 5; i++)
			{
				Unit2Check = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(Distiller[i].x, Distiller[i].y, Distiller[i].z, 17954);
				if(!Unit2Check)
					continue;

				if(!Unit2Check->isAlive())
					continue;

				float Dist2Unit = _unit->GetDistance2dSq(Unit2Check);
				if(Dist2Unit > distance * distance)
					continue;

				pDistiller = Unit2Check;
				distance = sqrt(Dist2Unit);
				DistillerNumber = i;
			}

			return pDistiller;
		}

	protected:

		uint32 DistillerNumber;
		uint32 RagePhaseTimer;
		uint32 EnrageTimer;
		uint32 RagePhase;
		int nrspells;
};

void SetupTheSteamvault(ScriptMgr* mgr)
{
	/*mgr->register_creature_script(CN_COILFANG_ORACLE, &COILFANGORACLEAI::Create);
	mgr->register_creature_script(CN_COILFANG_ENGINEER, &COILFANGENGINEERAI::Create);
	mgr->register_creature_script(CN_COILFANG_WARRIOR, &COILFANGWARRIORAI::Create);
	mgr->register_creature_script(CN_COILFANG_SIREN, &COILFANGSIRENAI::Create);
	mgr->register_creature_script(CN_COILFANG_SORCERESS, &COILFANGSORCERESSAI::Create);
	mgr->register_creature_script(CN_COILFANG_LEPER, &COILFANGLEPERAI::Create);
	mgr->register_creature_script(CN_COILFANG_SLAVEMASTER, &COILFANGSLAVEMASTERAI::Create);
	mgr->register_creature_script(CN_COILFANG_MYRMIDON, &COILFANGMYRMIDONAI::Create);
	mgr->register_creature_script(CN_COILFANG_WATER_ELEMENTAL, &COILFANGWATERELEMENTALAI::Create);
	mgr->register_creature_script(CN_BOG_OVERLORD, &BOGOVERLORDAI::Create);
	mgr->register_creature_script(CN_TIDAL_SURGER, &TIDALSURGERAI::Create);
	mgr->register_creature_script(CN_STEAM_SURGER, &STEAMSURGERAI::Create);
	mgr->register_creature_script(CN_STEAMRIGGER_MECHANIC, &STEAMRIGGERMECHANICAI::Create);*/
	mgr->register_creature_script(CN_HYDROMANCER_THESPIA, &HydromancerThespiaAI::Create);
	mgr->register_creature_script(CN_STEAMRIGGER_MECHANIC, &SteamriggerMechanicAI::Create);
	mgr->register_creature_script(CN_MEKGINEER_STEAMRIGGER, &MekgineerSteamriggerAI::Create);
	mgr->register_creature_script(CN_NAGA_DISTILLER, &NagaDistillerAI::Create);
	mgr->register_creature_script(CN_WARLORD_KALITRESH, &WarlordKalitreshAI::Create);
}

/* TO DO: Check all spells/creatures and evenatually balance them (if needed!)
   also add spawns correctly (no core support for now and hacky Onyxia way does
   not work (in Onyxia also). Change target type when there will be more core
   support. Also very imporant thing is to force spawns to heal random or target
   you choosed. When that will work, I will add AI for mechanics who should
   heal one of instance bosses.*/
// Don't have infos about: Second Fragment Guardian (22891) | Dreghood Slave (17799 -
// should they really have enrage ? [8269]), Driller should use Warlord's Rage spell
// (31543) to force Warlord to enrage, but I need more infos about targeting target
// you want to.
