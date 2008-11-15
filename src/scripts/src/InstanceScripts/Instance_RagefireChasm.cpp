/*
 * ArcScript Scripts for Arcemu MMORPG Server
 * Copyright (C) 2008 Arcemu Team
 * Copyright (C) 2007 Moon++ <http://www.moonplusplus.com/>
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
/* Instance_RagefireChasm.cpp Script									*/
/************************************************************************/

#define CN_RAGEFIRE_SHAMAN			11319

#define SPELL_HEALING_WAVE			547 
#define SPELL_LIGHTNING_BOLT		45287

class RagefireShamanAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(RagefireShamanAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];
	
    RagefireShamanAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// -- Number of spells to add --
		nrspells = 2;

		// --- Initialization ---
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		// ----------------------
		
		// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not! 
        spells[0].info = dbcSpell.LookupEntry(SPELL_HEALING_WAVE);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = true;
		spells[0].perctrigger = 15.0f;
		spells[0].attackstoptimer = 1000; // 1sec

		spells[1].info = dbcSpell.LookupEntry(SPELL_LIGHTNING_BOLT);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = false;
		spells[1].perctrigger = 20.0f;
		spells[1].attackstoptimer = 1000; // 1sec
	}
    
    void OnCombatStart(Unit* mTarget)
    {
        RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
	}

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		float val = (float)RandomFloat(100.0f);
        SpellCast(val);
    }

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					if (!spells[i].instant)
						_unit->GetAIInterface()->StopMovement(1);

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

// Ragefire Trogg AI by Soulshifter

#define CN_RAGEFIRE_TROGG			11318

#define SPELL_STRIKE				11998 

class RagefireTroggAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(RagefireTroggAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];
	
    RagefireTroggAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// -- Number of spells to add --
		nrspells = 1;

		// --- Initialization ---
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		// ----------------------
		
		// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not! 
        spells[0].info = dbcSpell.LookupEntry(SPELL_STRIKE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 40.0f;
		spells[0].attackstoptimer = 1000; // 1sec
	}
    
    void OnCombatStart(Unit* mTarget)
    {
        RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
	}

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		float val = (float)RandomFloat(100.0f);
        SpellCast(val);
    }

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					if (!spells[i].instant)
						_unit->GetAIInterface()->StopMovement(1);

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

// Searing Blade Warlock AI by Soulshifter

#define CN_SEARING_BLADE_WARLOCK	11324
#define CN_VOIDWALKER_SPAWN			500000

#define SPELL_SHADOWBOLT          	705 

class SearingBladeWarlockAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(SearingBladeWarlockAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];
	bool Spawn_Voidwalker;

    SearingBladeWarlockAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// -- Number of spells to add --
		nrspells = 1;

		//This defines if the Warlock has summoned his Voidwalker to avoid multi spawning. Starts on false.
		
		Spawn_Voidwalker = false;
		
		// --- Initialization ---
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		// ----------------------
		
		// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not! 
        spells[0].info = dbcSpell.LookupEntry(SPELL_SHADOWBOLT);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = 20.0f;
		spells[0].attackstoptimer = 1000; // 1sec
	}
    
    void OnCombatStart(Unit* mTarget)
    {	// This way won't work, especially if Void Walker dies.
        if(!Spawn_Voidwalker)
		{
			_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_VOIDWALKER_SPAWN, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 0.0f, false, false, 0, 0);
			_unit->GetAIInterface()->AttackReaction(_unit->GetAIInterface()->GetNextTarget(), 1, 0);
		}

		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		Spawn_Voidwalker = true;
    }

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		float val = (float)RandomFloat(100.0f);
        SpellCast(val);
    }

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					if (!spells[i].instant)
						_unit->GetAIInterface()->StopMovement(1);

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

// Blade Cultist AI by Soulshifter

#define CN_BLADE_CULTIST			11322

#define SPELL_CURSE_AGONY			1014 

class BladeCultistAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(BladeCultistAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];
	
    BladeCultistAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// -- Number of spells to add --
		nrspells = 1;

		// --- Initialization ---
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		// ----------------------
		
		// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not! 
        spells[0].info = dbcSpell.LookupEntry(SPELL_CURSE_AGONY);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 30.0f;
		spells[0].attackstoptimer = 1000; // 1sec
	}
    
    void OnCombatStart(Unit* mTarget)
    {
        RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
	}

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		float val = (float)RandomFloat(100.0f);
        SpellCast(val);
    }

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					if (!spells[i].instant)
						_unit->GetAIInterface()->StopMovement(1);

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

// Molten Elemental AI by Soulshifter

#define CN_MOLTEN_ELEMENTAL			11321

#define SPELL_FIRE_SHIELD			134 

class MoltenElementalAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(MoltenElementalAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];
	
    MoltenElementalAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// -- Number of spells to add --
		nrspells = 1;

		// --- Initialization ---
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		// ----------------------
		
		// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not! 
        spells[0].info = dbcSpell.LookupEntry(SPELL_FIRE_SHIELD);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = true;
		spells[0].perctrigger = 40.0f;
		spells[0].attackstoptimer = 1000; // 1sec
    }
    
    void OnCombatStart(Unit* mTarget)
    {
        RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
	}

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		float val = (float)RandomFloat(100.0f);
        SpellCast(val);
    }

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					if (!spells[i].instant)
						_unit->GetAIInterface()->StopMovement(1);

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

// Earthborer AI by Soulshifter

#define CN_EARTHBORER				11320

#define SPELL_EARTHBORER_ACID		18070 

class EarthborerAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(EarthborerAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];
	
    EarthborerAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// -- Number of spells to add --
		nrspells = 1;

		// --- Initialization ---
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		// ----------------------
		
		// Create basic info for spells here, and play with it later , fill always the info, targettype and if is instant or not! 
        spells[0].info = dbcSpell.LookupEntry(SPELL_EARTHBORER_ACID);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = 15.0f;
		spells[0].attackstoptimer = 1000; // 1sec
	}
    
    void OnCombatStart(Unit* mTarget)
    {
        RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
	}

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		float val = (float)RandomFloat(100.0f);
        SpellCast(val);
    }

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					if (!spells[i].instant)
						_unit->GetAIInterface()->StopMovement(1);

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

//Jergosh The Invoker - Curse Of Weakness & Immolate

#define CN_JERGOSH			11518

#define JERGOSH_CURSE		8552
#define JERGOSH_IMMOLATE	707

class JergoshAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(JergoshAI);
	JergoshAI(Creature *pCreature) : CreatureAIScript(pCreature)
	{
		RegisterAIUpdateEvent(1000);
	}

	void AIUpdate()
	{
		float JerCast = (float)RandomFloat(100.0f);
		CastSpell(JerCast);
	}

	void CastSpell(float JerCast)
	{
		if (_unit->GetAIInterface()->GetNextTarget() != NULL && _unit->GetCurrentSpell() == NULL)
		{
			//15% chance to Immolate
			if (JerCast <= 15.0f)
			{
				_unit->GetAIInterface()->StopMovement(1);
				_unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), JERGOSH_IMMOLATE, false);
			}
			//10% chance to Curse Of Weakness
			else if (JerCast >= 90.0f)
			{
				_unit->GetAIInterface()->StopMovement(1);
				_unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), JERGOSH_CURSE, false);
			}
		}
	}

	void Destroy()
	{
		delete (JergoshAI*)this;
	}

	void OnDied()
	{
		RemoveAIUpdateEvent();
	}
};

//Oggleflint - Cleave

#define CN_OGGLEFLINT	11517

#define CLEAVE			15496

class OggleflintAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(OggleflintAI);
	OggleflintAI(Creature *pCreature) : CreatureAIScript(pCreature)
	{
		RegisterAIUpdateEvent(1000);
	}

	void AIUpdate()
	{
		float OggCast = (float)RandomFloat(100.0f);
		CastSpell(OggCast);
	}

	void CastSpell(float OggCast)
	{
		if (_unit->GetAIInterface()->GetNextTarget() != NULL && _unit->GetCurrentSpell() == NULL)
		{
			//10% chance to Cleave
			if (OggCast <= 10.0f)
			{
				_unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), CLEAVE, true);
			}
		}
	}

	void Destroy()
	{
		delete (OggleflintAI*)this;
	}

	void OnDied()
	{
		RemoveAIUpdateEvent();
	}
};

//Taragaman the Hungerer - Fire Nova & Uppercut

#define CN_TARAGAMAN	11520

#define TAR_NOVA		11969
#define TAR_UPPERCUT	10966

class TaragamanAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(TaragamanAI);
	TaragamanAI(Creature *pCreature) : CreatureAIScript(pCreature)
	{
		RegisterAIUpdateEvent(1000);
	}

	void AIUpdate()
	{
		float TarCast = (float)RandomFloat(100.0f);
		CastSpell(TarCast);
	}

	void CastSpell(float TarCast)
	{
		if (_unit->GetAIInterface()->GetNextTarget() != NULL && _unit->GetCurrentSpell() == NULL)
		{
			//10% chance to Fire Nova
			if (TarCast <= 10.0f)
			{
				_unit->GetAIInterface()->StopMovement(1);
				_unit->CastSpell(_unit, TAR_NOVA, false);
			}
			//10% chance to Uppercut
			else if (TarCast >= 90.0f)
			{
				_unit->GetAIInterface()->StopMovement(1);
				_unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), TAR_UPPERCUT, false);
			}
		}
	}

	void Destroy()
	{
		delete (TaragamanAI*)this;
	}

	void OnDied()
	{
		RemoveAIUpdateEvent();
	}
};

//Bazzalan - Poison and Sinister Strike

#define CN_BAZZALAN	11519

#define POISON		24583
#define SSTRIKE		1758

class BazzalanAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(BazzalanAI);
	BazzalanAI(Creature *pCreature) : CreatureAIScript(pCreature)
	{
		RegisterAIUpdateEvent(1000);
	}

	void AIUpdate()
	{
		float BazCast = (float)RandomFloat(100.0f);
		CastSpell(BazCast);
	}

	void CastSpell(float BazCast)
	{
		if (_unit->GetAIInterface()->GetNextTarget() != NULL && _unit->GetCurrentSpell() == NULL)
		{
			//5% chance to cast "Scorpid Poison 2"
			if (BazCast <= 5.0f)
			{
				_unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), POISON, true);
			}
			//5% chance to cast "Sinister Strike (Rank 3)"
			else if (BazCast >= 95.0f)
			{
				_unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), SSTRIKE, true);
			}
		}
	}

	void Destroy()
	{
		delete (BazzalanAI*)this;
	}

	void OnDied()
	{
		RemoveAIUpdateEvent();
	}
};

void SetupRagefireChasm(ScriptMgr * mgr)
{
	mgr->register_creature_script(CN_BAZZALAN, &BazzalanAI::Create);
	mgr->register_creature_script(CN_JERGOSH, &JergoshAI::Create);
	mgr->register_creature_script(CN_OGGLEFLINT, &OggleflintAI::Create);
	mgr->register_creature_script(CN_TARAGAMAN, &TaragamanAI::Create);
	mgr->register_creature_script(CN_SEARING_BLADE_WARLOCK, &SearingBladeWarlockAI::Create);
	mgr->register_creature_script(CN_EARTHBORER, &EarthborerAI::Create);
	mgr->register_creature_script(CN_MOLTEN_ELEMENTAL, &MoltenElementalAI::Create);
	mgr->register_creature_script(CN_RAGEFIRE_SHAMAN, &RagefireShamanAI::Create);
	mgr->register_creature_script(CN_RAGEFIRE_TROGG, &RagefireTroggAI::Create);
	mgr->register_creature_script(CN_BLADE_CULTIST, &BladeCultistAI::Create);
}