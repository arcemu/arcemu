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
/* Instance_SethekkHalls.cpp Script										*/
/************************************************************************/

// Avian Darkhawk AI

#define CN_AVIAN_DARKHAWK 20686

#define CHARGE 36509 // no idea if this is correct id

class AVIANDARKHAWKAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(AVIANDARKHAWKAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    AVIANDARKHAWKAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(CHARGE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 5.0f;
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

	Unit* target;
	int nrspells;
};

// Avian Ripper AI

#define CN_AVIAN_RIPPER 21891

#define FLESH_RIP 40199

class AVIANRIPPERAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(AVIANRIPPERAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    AVIANRIPPERAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(FLESH_RIP);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 15.0f;
		spells[0].attackstoptimer = 3000;

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

// Avian Warhawk AI

#define CN_AVIAN_WARHAWK 21904		// test it more@

#define CLEAVE 38474 // no idea if this is right
#define CHARGE_WARHAWK 40602 // same here
#define CARNIVOROUS_BITE 39382 // and here =)

class AVIANWARHAWKAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(AVIANWARHAWKAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    AVIANWARHAWKAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(CLEAVE);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].perctrigger = 12.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(CHARGE_WARHAWK);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].perctrigger = 5.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(CARNIVOROUS_BITE);
		spells[2].targettype = TARGET_ATTACKING; // check targeting!
		spells[2].instant = true;
		spells[2].perctrigger = 12.0f;
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

	Unit* target;
	int nrspells;
};

// Cobalt Serpent AI

#define CN_COBALT_SERPENT 19428

#define WING_BUFFET 41572
#define FROSTBOLT 40429 // no idea about if these are good ids :P
#define CHAIN_LIGHTNING_SERPENT 39945

class COBALTSERPENTAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(COBALTSERPENTAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    COBALTSERPENTAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(WING_BUFFET);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].perctrigger = 7.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(FROSTBOLT);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = false;
		spells[1].perctrigger = 15.0f;
		spells[1].attackstoptimer = 1000; 

		spells[2].info = dbcSpell.LookupEntry(CHAIN_LIGHTNING_SERPENT);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = false;
		spells[2].perctrigger = 9.0f;
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

// Time-Lost Controller AI

#define CN_TIME_LOST_CONTROLLER 20691

#define SHIRNK 36697 // 36697 or 35013
//#define CONTROL_TOTEM		// Can't find spell for that :O

class TIMELOSTCONTROLLERAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(TIMELOSTCONTROLLERAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    TIMELOSTCONTROLLERAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(SHIRNK);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000;
/*
		spells[1].info = dbcSpell.LookupEntry(CONTROL_TOTEM);
		spells[1].targettype = TARGET_;
		spells[1].instant = false;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000; 
*/
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

// Time-Lost Scryer AI

#define CN_TIME_LOST_SCRYER 20697

#define FLASH_HEAL 38588 // let's try this one
#define ARCANE_MISSILES 35034 // and those: 35033, 35034	// doesn't work somehow

class TIMELOSTSCRYERAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(TIMELOSTSCRYERAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    TIMELOSTSCRYERAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(FLASH_HEAL);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = true;
		spells[0].perctrigger = 5.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(ARCANE_MISSILES);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = false;
		spells[1].perctrigger = 12.0f;
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


// Time-Lost Shadowmage AI

#define CN_TIME_LOST_SHADOWMAGE 20698

#define CURSE_OF_THE_DARK_TALON 32682

class TIMELOSTSHADOWMAGEAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(TIMELOSTSHADOWMAGEAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    TIMELOSTSHADOWMAGEAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(CURSE_OF_THE_DARK_TALON);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
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

// Sethekk Guard AI

#define CN_SETHEKK_GUARD 18323

#define THUNDERCLAP 36214
#define SUNDER_ARMOR 30901 // 1000 arm per use (to 5 uses!) O_O

class SETHEKKGUARDAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(SETHEKKGUARDAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    SETHEKKGUARDAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(THUNDERCLAP);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].perctrigger = 12.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(SUNDER_ARMOR);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].perctrigger = 8.0f;
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

// Sethekk Initiate AI

#define CN_SETHEKK_INITIATE 18318

#define MAGIC_REFLECTION 20223 // 20223 or 20619

class SETHEKKINITIATEAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(SETHEKKINITIATEAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    SETHEKKINITIATEAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(MAGIC_REFLECTION);
		spells[0].targettype = TARGET_SELF;
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

// Sethekk Oracle AI

#define CN_SETHEKK_ORACLE 18328

#define FAERIE_FIRE 21670 // 20656 or 21670 or 32129 or other
#define ARCANE_LIGHTNING 38146 // 38146, 32690 or 38634

class SETHEKKORACLEAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(SETHEKKORACLEAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    SETHEKKORACLEAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(FAERIE_FIRE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 8.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(ARCANE_LIGHTNING);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = false;
		spells[1].perctrigger = 15.0f;
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

// Sethekk Prophet AI

#define CN_SETHEKK_PROPHET 18325

#define FEAR 32241 // Should it be aoe or normal? // damn it fears caster too
//#define   // Ghost spawning similar to those in Sunken Temple

class SETHEKKPROPHETAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(SETHEKKPROPHETAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    SETHEKKPROPHETAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(FEAR);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 8.0f;
		spells[0].attackstoptimer = 1000;
/*
		spells[1].info = dbcSpell.LookupEntry();
		spells[1].targettype = TARGET_;
		spells[1].instant = true;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;
*/
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

// Sethekk Ravenguard AI

#define CN_SETHEKK_RAVENGUARD 18322

#define BLOODTHIRST 31996 // check also spells like this: 31996 and this: 35948
#define HOWLING_SCREECH 32651

class SETHEKKRAVENGUARDAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(SETHEKKRAVENGUARDAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    SETHEKKRAVENGUARDAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(BLOODTHIRST);
		spells[0].targettype = TARGET_ATTACKING;	//?
		spells[0].instant = true;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(HOWLING_SCREECH);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].perctrigger = 8.0f;
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

// Sethekk Shaman AI

#define CN_SETHEKK_SHAMAN 18326

#define SUMMON_DARK_VORTEX 32663 //SUMMON_VOIDWALKER 30208 // Shouldn't be Dark Vortex (spell id: 32663) ?

class SETHEKKSHAMANAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(SETHEKKSHAMANAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    SETHEKKSHAMANAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(SUMMON_DARK_VORTEX);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = true;
		spells[0].perctrigger = 8.0f;
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

// Sethekk Talon Lord AI

#define CN_SETHEKK_TALON_LORD 18321

#define TALON_OF_JUSTICE 32654 // 32654 or 39229
#define AVENGERS_SHIELD 32774 // On WoWWiki is Shield of Revenge, but that should be it. Also spells that can be: 32774, 32674, 37554

class SETHEKKTALONLORDAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(SETHEKKTALONLORDAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    SETHEKKTALONLORDAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(TALON_OF_JUSTICE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(AVENGERS_SHIELD);
		spells[1].targettype = TARGET_ATTACKING;
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

	Unit* target;
	int nrspells;
};

////////////////////////////////////////////////////
// Lakka AI
#define CN_LAKKA		18956

static LocationExtra LakkaWaypoint[]= 
{
	{ },
	{ -157.200f, 159.922f, 0.010f, 0.104f, Flag_Walk },
	{ -128.318f, 172.483f, 0.009f, 0.222f, Flag_Walk },
	{ -73.749f, 173.171f, 0.009f, 6.234f, Flag_Walk },
};

class LakkaAI : public MoonScriptCreatureAI
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(LakkaAI);
	LakkaAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		SetMoveType(Move_DontMoveWP);

		//WPs
		for (int i = 1; i < 4; ++i)
		{
			AddWaypoint(CreateWaypoint(i, 0, LakkaWaypoint[i].addition, LakkaWaypoint[i]));
		}
	}
	
	void OnReachWP(uint32 iWaypointId, bool bForwards)
	{
		switch (iWaypointId)
		{
			case 1:
				{
					SetMoveType(Move_WantedWP);
					SetWaypointToMove(2);
					Player* pPlayer	= NULL;
					QuestLogEntry *pQuest	= NULL;
					for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
					{
						if((*itr)->IsPlayer())
						{
							pPlayer = TO_PLAYER((*itr));
							if (pPlayer != NULL)
							{
								pQuest = pPlayer->GetQuestLogForEntry(10097);
								if ( pQuest != NULL && pQuest->GetMobCount(1)<1 )
								{
									pQuest->SetMobCount(1, 1);
									pQuest->SendUpdateAddKill(1);
									pQuest->UpdatePlayerFields();
								}
							}
						}
					}
				}
				break;
			case 3:
				{
					Despawn(100, 0);
				}
				break;
			default:
				{				
					SetMoveType(Move_WantedWP);
					SetWaypointToMove(1);
				}
		}
	}
};

/*****************************/
/*                           */
/*         Boss AIs          */
/*                           */
/*****************************/

// Darkweaver SythAI

#define CN_DARKWEAVER_SYTH 18472

// Spells
#define FROST_SHOCK			37865
#define FLAME_SHOCK			34354
#define SHADOW_SHOCK		30138
#define ARCANE_SHOCK		37132
#define CHAIN_LIGHTNING		39945

// Summons
#define SUMMON_SYTH_FIRE_ELEMENTAL		33537
#define SUMMON_SYTH_FROST_ELEMENTAL		33539
#define SUMMON_SYTH_ARCANE_ELEMENTAL	33538
#define SUMMON_SYTH_SHADOW_ELEMENTAL	33540

class DARKWEAVERSYTHAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(DARKWEAVERSYTHAI);
	SP_AI_Spell spells[9];
	bool m_spellcheck[9];

    DARKWEAVERSYTHAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 5;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		} 
		// Not sure in any way about target types
        spells[0].info = dbcSpell.LookupEntry(FROST_SHOCK);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 8.0f;
		spells[0].attackstoptimer = 2000;
		spells[0].cooldown = 15;

        spells[1].info = dbcSpell.LookupEntry(FLAME_SHOCK);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].perctrigger = 8.0f;
		spells[1].attackstoptimer = 2000;
		spells[1].cooldown = 15;

        spells[2].info = dbcSpell.LookupEntry(SHADOW_SHOCK);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = true;
		spells[2].perctrigger = 8.0f;
		spells[2].attackstoptimer = 2000;
		spells[2].cooldown = 15;

        spells[3].info = dbcSpell.LookupEntry(ARCANE_SHOCK);
		spells[3].targettype = TARGET_ATTACKING;
		spells[3].instant = true;
		spells[3].perctrigger = 8.0f;
		spells[3].attackstoptimer = 2000;
		spells[3].cooldown = 15;

        spells[4].info = dbcSpell.LookupEntry(CHAIN_LIGHTNING);
		spells[4].targettype = TARGET_ATTACKING;
		spells[4].instant = true;
		spells[4].perctrigger = 10.0f;
		spells[4].attackstoptimer = 1000;
		spells[4].cooldown = 15;

        spells[5].info = dbcSpell.LookupEntry(SUMMON_SYTH_FIRE_ELEMENTAL);
		spells[5].targettype = TARGET_SELF;
		spells[5].instant = true;
		spells[5].perctrigger = 0.0f;
		spells[5].attackstoptimer = 1000;
		spells[5].cooldown = 10;

        spells[6].info = dbcSpell.LookupEntry(SUMMON_SYTH_FROST_ELEMENTAL);
		spells[6].targettype = TARGET_SELF;
		spells[6].instant = true;
		spells[6].perctrigger = 0.0f;
		spells[6].attackstoptimer = 1000;
		spells[6].cooldown = -1;

        spells[7].info = dbcSpell.LookupEntry(SUMMON_SYTH_ARCANE_ELEMENTAL);
		spells[7].targettype = TARGET_SELF;
		spells[7].instant = true;
		spells[7].perctrigger = 0.0f;
		spells[7].attackstoptimer = 1000;
		spells[7].cooldown = -1;

        spells[8].info = dbcSpell.LookupEntry(SUMMON_SYTH_SHADOW_ELEMENTAL);
		spells[8].targettype = TARGET_SELF;
		spells[8].instant = true;
		spells[8].perctrigger = 0.0f;
		spells[8].attackstoptimer = 1000;
		spells[8].cooldown = -1;

		Summons = 0;
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		for (int i = 0; i < 9; i++)
			spells[i].casttime = 0;

		Summons = 0;

		int RandomSpeach = rand()%3;
		switch (RandomSpeach)
		{
		case 0:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Time to... make my move!"); // needs corrections
			_unit->PlaySoundToSet(10503);
			break;
		case 1:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Nice pets, yes!"); // corrections needed!
			_unit->PlaySoundToSet(10504);
			break;
		case 2:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Nice pets have... weapons, not so... nice!");
			_unit->PlaySoundToSet(10505);
			break;
		}

		RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
    }

	void OnTargetDied(Unit* mTarget)
    {
		if (_unit->GetHealthPct() > 0)	// Hack to prevent double yelling (OnDied and OnTargetDied when creature is dying)
		{
			int RandomSpeach = rand()%2;
			switch (RandomSpeach)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Yes! Fleeting life is..."); // need to add it
				_unit->PlaySoundToSet(10506);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Be free!"); // corrections needed!!
				_unit->PlaySoundToSet(10507);
				break;
			}
		}
    }

    void OnCombatStop(Unit* mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);

        RemoveAIUpdateEvent();

		Summons = 0;
    }

    void OnDied(Unit* mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "No more life, no more pain!"); // It's talking so <censored>
		_unit->PlaySoundToSet(10508);

		GameObject* LakkasCage = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-160.813f, 157.043f, 0.194095f, 183051);
		Creature* mLakka = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-160.813f, 157.043f, 0.194095f, 18956);

		if (LakkasCage != NULL)
		{
			LakkasCage->SetState(0);
			LakkasCage->SetUInt32Value(GAMEOBJECT_FLAGS, LakkasCage->GetUInt32Value(GAMEOBJECT_FLAGS)-1);
		}

		if ( mLakka != NULL && mLakka->GetScript() )
		{
			MoonScriptCreatureAI *pLakkaAI = TO< MoonScriptCreatureAI* >(mLakka->GetScript());
			mLakka->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
			pLakkaAI->SetMoveType(Move_WantedWP);
			pLakkaAI->SetWaypointToMove(1);
			pLakkaAI->SetBehavior(Behavior_Default);
		}

		RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		uint32 t = (uint32)time(NULL);
		if (t > spells[4].casttime && ((_unit->GetHealthPct() <= 75 && Summons == 0) || (_unit->GetHealthPct() <= 50 && Summons == 1) || (_unit->GetHealthPct() <= 25 && Summons == 2)))
		{
			_unit->setAttackTimer(1500, false);
			_unit->GetAIInterface()->StopMovement(1000);	// really?

			SummonElementalWave();

			spells[4].casttime = t + spells[4].cooldown;
			Summons++;
			return;
		}

		else
		{
			float val = RandomFloat(100.0f);
			SpellCast(val);
		}
	}

	void SummonElementalWave()
	{
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I have pets... of my own"); // It's talking so <doublecensored> -.-'
		_unit->PlaySoundToSet(10502);

		_unit->CastSpell(_unit, spells[5].info, spells[5].instant);
		_unit->CastSpell(_unit, spells[6].info, spells[6].instant);
		_unit->CastSpell(_unit, spells[7].info, spells[7].instant);
		_unit->CastSpell(_unit, spells[8].info, spells[8].instant);		
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
					if (!spells[i].instant)
						_unit->GetAIInterface()->StopMovement(1);

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

	uint32 Summons;
	int nrspells;
};

// Talon King IkissAI

#define CN_TALON_KING_IKISS 18473

#define ARCANE_VOLLEY		36738	// 35059 ?
#define ARCANE_EXPLOSION	38197	// bit too high dmg, but should work nearly in the same way
#define BLINK				1953	// 38194; lacks of core support
#define POLYMORPH			12826	// 38245; worth to try also: 38245, 38896
//#define MANA_SHIELD			38151	// also: 35064, 38151

class TALONKINGIKISSAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(TALONKINGIKISSAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    TALONKINGIKISSAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(ARCANE_VOLLEY);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].perctrigger = 12.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = 10;

		spells[1].info = dbcSpell.LookupEntry(BLINK);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = true;
		spells[1].perctrigger = 7.0f;
		spells[1].attackstoptimer = 1000;
		spells[1].cooldown = 25;

		spells[2].info = dbcSpell.LookupEntry(POLYMORPH);
		spells[2].targettype = TARGET_RANDOM_SINGLE;
		spells[2].instant = false;
		spells[2].perctrigger = 9.0f;
		spells[2].attackstoptimer = 1000;
		spells[2].cooldown = 15;
		spells[2].mindist2cast = 0.0f;
		spells[2].maxdist2cast = 40.0f;

		spells[3].info = dbcSpell.LookupEntry(ARCANE_EXPLOSION);
		spells[3].targettype = TARGET_VARIOUS;
		spells[3].instant = false;
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 1000;
		spells[3].cooldown = -1;

		Blink = false;
	} 

    void OnCombatStart(Unit* mTarget)
    {
		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;
		uint32 t = (uint32)time(NULL);
		spells[1].casttime = t + RandomUInt(5) + 10;

		if (_unit->GetCurrentSpell() == NULL)
		{
			_unit->GetAIInterface()->StopMovement(1);
			_unit->setAttackTimer(3000, false);
			_unit->CastSpell(_unit, spells[0].info, spells[0].instant);

			spells[0].casttime = t + spells[0].cooldown;
		}

		Blink = false;

		int RandomSpeach = rand()%3;
		switch (RandomSpeach)
		{
		case 0:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You make war on Ikiss!"); // needs corrections
			_unit->PlaySoundToSet(10554);
			break;
		case 1:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Ikiss caught you pretty... sliced you, yes!"); // corrections needed!
			_unit->PlaySoundToSet(10555);
			break;
		case 2:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "No escape for... for you!");
			_unit->PlaySoundToSet(10556);
			break;
		}

        RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
    }

	void OnTargetDied(Unit* mTarget) // left to keep it easy to add needed data.
    {
		if (_unit->GetHealthPct() > 0)	// Hack to prevent double yelling (OnDied and OnTargetDied when creature is dying)
		{
			int RandomSpeach = rand()%2;
			switch (RandomSpeach)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You'll die! Stay away from trinkets!"); // needs corrections
				_unit->PlaySoundToSet(10558);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "<strange_noises>"); // corrections needed!
				_unit->PlaySoundToSet(10559);
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
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Ikiss will not... die!");
		_unit->PlaySoundToSet(10560);
		
		GameObject* IkissDoor = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(43.079f, 149.505f, 0.034f, 183398);
		if (IkissDoor != NULL)
			IkissDoor->SetState(0);

		RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		if (Blink)
		{	
			_unit->GetAIInterface()->StopMovement(2000);
			_unit->setAttackTimer(6500, false);

			if (_unit->GetCurrentSpell())
				_unit->GetCurrentSpell()->cancel();

			_unit->CastSpell(_unit, spells[3].info, spells[3].instant);

			Blink = false;
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
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					if (!spells[i].instant)
						_unit->GetAIInterface()->StopMovement(1);

					if (i == 1)
						BlinkCast();
					else
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
							case TARGET_RANDOM_FRIEND:
							case TARGET_RANDOM_SINGLE:
							case TARGET_RANDOM_DESTINATION:
								CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast); break;
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
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
        {
			std::vector<Unit*> TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && (*itr)->IsUnit()) // isAttackable(_unit, (*itr)) && 
				{
					Unit* RandomTarget = NULL;
					RandomTarget = TO_UNIT(*itr);

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

			Unit*  RTarget = TargetTable[RandTarget];

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

	void BlinkCast()
	{
		std::vector<Unit*> TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
											/* If anyone wants to use this function, then leave this note!										 */
		for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
		{ 
			if (isHostile(_unit, (*itr)) && (*itr) != _unit && (*itr)->IsUnit())
			{
				Unit* RandomTarget = NULL;
				RandomTarget = TO_UNIT(*itr);
				if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= 0.0f && _unit->GetDistance2dSq(RandomTarget) <= 900.0f && _unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0)
				{
					TargetTable.push_back(RandomTarget);
				} 
			} 
		}

		if (!TargetTable.size())
		{
			TargetTable.clear();
			return;
		}

		size_t RandTarget = rand()%TargetTable.size();
		Unit*  RTarget = TargetTable[RandTarget];

		if (!RTarget)
		{
			TargetTable.clear();
			return;
		}

		_unit->GetAIInterface()->setNextTarget(RTarget);
		_unit->CastSpell(_unit, spells[1].info, spells[1].instant);

		TargetTable.clear();

		_unit->setAttackTimer(2500, false);
		_unit->GetAIInterface()->StopMovement(2500);

		Blink = true;
	}

protected:

	bool Blink;
	int nrspells;
};

// AnzuAI

#define CN_ANZU 23035 // that should be real id, but it is not existing in my DB

#define SUMMON_RAVEN_GOD 40098	// event just to test it!

#define SPELL_BOMB				40303
#define CYCLONE_OF_FEATHERS		40321
#define PARALYZING_SCREECH		40184
#define BANISH					40370 // can be: 38791, 38009, 40370, 39674, 35182, 40825 // should banish for one minute

class ANZUAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(ANZUAI);
	SP_AI_Spell spells[5];
	bool m_spellcheck[5];

    ANZUAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
        
		spells[0].info = dbcSpell.LookupEntry(SPELL_BOMB);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(CYCLONE_OF_FEATHERS);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = false;
		spells[1].perctrigger = 10.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(PARALYZING_SCREECH);
		spells[2].targettype = TARGET_VARIOUS;
		spells[2].instant = false;
		spells[2].perctrigger = 10.0f;
		spells[2].attackstoptimer = 1000;

		spells[3].info = dbcSpell.LookupEntry(BANISH);
		spells[3].targettype = TARGET_SELF;
		spells[3].instant = true;
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 1000;

		spells[4].info = dbcSpell.LookupEntry(SUMMON_RAVEN_GOD);
		spells[4].targettype = TARGET_SELF;
		spells[4].instant = true;
		spells[4].perctrigger = 0.0f;
		spells[4].attackstoptimer = 1000;

		_unit->CastSpell(_unit, spells[4].info, spells[4].instant);

		Banished = false;
		Summon = 0;
	} 

    void OnCombatStart(Unit* mTarget)
    {
        RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));

		Banished = false;
		Summon = 0;
    }

    void OnCombatStop(Unit* mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		_unit->RemoveAura(BANISH);

        RemoveAIUpdateEvent();

		Banished = false;
		Summon = 0;
    }

	void OnDied(Unit* mKiller)
    {
		RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		if((_unit->GetHealthPct() <= 66 && Summon == 0) || (_unit->GetHealthPct() <= 33 && Summon == 1))
		{
			SummonPhase();
			Summon++;
		}

		if (Banished)
		{
			uint32 t = (uint32)time(NULL);
			if (t > spells[4].casttime)
			{
				_unit->RemoveAura(BANISH);

				Banished = false;
			}

			else
			{
				for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
				{ 
					if ((*itr) != _unit && (*itr)->IsCreature())
					{
						Creature* Check = NULL;
						Check = TO_CREATURE(*itr);

						if (Check->GetEntry() != 23132)
							continue;

						if (Check->isAlive())
							break;

						_unit->RemoveAura(BANISH);
					} 
				}
			}
			return;
		}

		else
		{
			float val = RandomFloat(100.0f);
			SpellCast(val);
		}
    }

	void SummonPhase()
	{
		_unit->CastSpell(_unit,spells[4].info, spells[4].instant);

		spells[4].casttime = (uint32)time(NULL) + 60;

		/*for (int i = 0; i < 15; i++)
		{
			Unit* Bird = NULL;
			Bird = _unit->GetMapMgr()->GetInterface()->SpawnCreature(23132, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), _unit->GetOrientation(), true, false, 0, 0);
			if (Bird)
			{
				Unit* target = NULL;
				target = FindTarget();
				if (target)
					_unit->GetAIInterface()->AttackReaction(target, 1, 0);
			}
		}*/

		Banished = true;
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

	// A bit rewritten FindTarget function
	Unit* FindTarget()
	{
		Unit* target = NULL;
		float distance = 80.0f;
		float z_diff;

		Unit* pUnit;
		float dist;

		for (set<Object*>::iterator itr = _unit->GetInRangeOppFactsSetBegin(); itr != _unit->GetInRangeOppFactsSetEnd(); itr++)
		{
			if(!(*itr)->IsUnit())
				continue;

			pUnit = TO_UNIT((*itr));

			if(pUnit->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_FEIGN_DEATH))
				continue;

			z_diff = fabs(_unit->GetPositionZ() - pUnit->GetPositionZ());
			if(z_diff > 5.0f)
				continue;
			
			if(!pUnit->isAlive() || _unit == pUnit)
				continue;

			dist = _unit->GetDistance2dSq(pUnit);

			if(dist > distance*distance)
				continue;

			distance = dist;
			target = pUnit;
		}

		return target;
	}

protected:

	bool Banished;
	uint32 Summon;
	int nrspells;
};

void SetupSethekkHalls(ScriptMgr * mgr)
{
	mgr->register_creature_script(CN_AVIAN_DARKHAWK, &AVIANDARKHAWKAI::Create);
	mgr->register_creature_script(CN_AVIAN_RIPPER, &AVIANRIPPERAI::Create);
	mgr->register_creature_script(CN_AVIAN_WARHAWK, &AVIANWARHAWKAI::Create);
	mgr->register_creature_script(CN_COBALT_SERPENT, &COBALTSERPENTAI::Create);
	mgr->register_creature_script(CN_TIME_LOST_CONTROLLER, &TIMELOSTCONTROLLERAI::Create);
	mgr->register_creature_script(CN_TIME_LOST_SCRYER, &TIMELOSTSCRYERAI::Create);
	mgr->register_creature_script(CN_TIME_LOST_SHADOWMAGE, &TIMELOSTSHADOWMAGEAI::Create);
	mgr->register_creature_script(CN_SETHEKK_GUARD, &SETHEKKGUARDAI::Create);
	mgr->register_creature_script(CN_SETHEKK_INITIATE, &SETHEKKINITIATEAI::Create);
	mgr->register_creature_script(CN_SETHEKK_ORACLE, &SETHEKKORACLEAI::Create);
	mgr->register_creature_script(CN_SETHEKK_PROPHET, &SETHEKKPROPHETAI::Create);
	mgr->register_creature_script(CN_SETHEKK_RAVENGUARD, &SETHEKKRAVENGUARDAI::Create);
	mgr->register_creature_script(CN_SETHEKK_SHAMAN, &SETHEKKSHAMANAI::Create);
	mgr->register_creature_script(CN_SETHEKK_TALON_LORD, &SETHEKKTALONLORDAI::Create);
    mgr->register_creature_script(CN_DARKWEAVER_SYTH, &DARKWEAVERSYTHAI::Create);
    mgr->register_creature_script(CN_TALON_KING_IKISS, &TALONKINGIKISSAI::Create);
    mgr->register_creature_script(CN_LAKKA, &LakkaAI::Create);
	//mgr->register_creature_script(CN_ANZU, &ANZUAI::Create);
}

// Can't check Anzu, as I don't have it in DB right now. Add some spells (2?) and
// change other if needed.
