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
/* Generic AI Script		                                            */
/************************************************************************/

// Family based AI's
class SpiderFamily : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(SpiderFamily);
	SP_AI_Spell spells[10];
	bool m_spellcheck[10];

    SpiderFamily(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 10;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		     spells[0].info = dbcSpell.LookupEntry(17253);
		     spells[0].reqlvl = 1;
		     spells[0].targettype = TARGET_ATTACKING;
		     spells[0].instant = false;
		     spells[0].perctrigger = (float)RandomFloat(5.0f);
		     spells[0].attackstoptimer = 1000;
		     m_spellcheck[0] = true;


		     spells[1].info = dbcSpell.LookupEntry(17255);
		     spells[1].reqlvl = 8;
		     spells[1].targettype = TARGET_ATTACKING;
		     spells[1].instant = false;
		     spells[1].perctrigger = (float)RandomFloat(5.0f);
		     spells[1].attackstoptimer = 1000;
		     m_spellcheck[1] = true;


		     spells[2].info = dbcSpell.LookupEntry(17256);
		     spells[2].reqlvl = 16;
		     spells[2].targettype = TARGET_ATTACKING;
		     spells[2].instant = false;
		     spells[2].perctrigger = (float)RandomFloat(5.0f);
		     spells[2].attackstoptimer = 1000;
		     m_spellcheck[2] = true;


		     spells[3].info = dbcSpell.LookupEntry(17257);
		     spells[3].reqlvl = 24;
		     spells[3].targettype = TARGET_ATTACKING;
		     spells[3].instant = false;
		     spells[3].perctrigger = (float)RandomFloat(5.0f);
		     spells[3].attackstoptimer = 1000;
		     m_spellcheck[3] = true;


		     spells[4].info = dbcSpell.LookupEntry(17258);
		     spells[4].reqlvl = 32;
		     spells[4].targettype = TARGET_ATTACKING;
		     spells[4].instant = false;
		     spells[4].perctrigger = (float)RandomFloat(5.0f);
		     spells[4].attackstoptimer = 1000;
		     m_spellcheck[4] = true;


		     spells[5].info = dbcSpell.LookupEntry(17259);
		     spells[5].reqlvl = 40;
		     spells[5].targettype = TARGET_ATTACKING;
		     spells[5].instant = false;
		     spells[5].perctrigger = (float)RandomFloat(5.0f);
		     spells[5].attackstoptimer = 1000;
		     m_spellcheck[5] = true;


		     spells[6].info = dbcSpell.LookupEntry(17260);
		     spells[6].reqlvl = 48;
		     spells[6].targettype = TARGET_ATTACKING;
		     spells[6].instant = false;
		     spells[6].perctrigger = (float)RandomFloat(5.0f);
		     spells[6].attackstoptimer = 1000;
		     m_spellcheck[6] = true;


		     spells[7].info = dbcSpell.LookupEntry(17261);
		     spells[7].reqlvl = 56;
		     spells[7].targettype = TARGET_ATTACKING;
		     spells[7].instant = false;
		     spells[7].perctrigger = (float)RandomFloat(5.0f);
		     spells[7].attackstoptimer = 1000;
		     m_spellcheck[7] = true;


		     spells[8].info = dbcSpell.LookupEntry(27050);
		     spells[8].reqlvl = 64;
		     spells[8].targettype = TARGET_ATTACKING;
		     spells[8].instant = false;
		     spells[8].perctrigger = (float)RandomFloat(5.0f);
		     spells[8].attackstoptimer = 1000;
		     m_spellcheck[8] = true;

		     spells[9].info = dbcSpell.LookupEntry(15471);
		     spells[9].reqlvl = 1;
		     spells[9].targettype = TARGET_ATTACKING;
		     spells[9].instant = false;
		     spells[9].perctrigger = (float)RandomFloat(5.0f);
		     spells[9].attackstoptimer = 1000;
		     m_spellcheck[9] = true;

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
	      			if((spells[i].reqlvl<=_unit->GetUInt32Value(UNIT_FIELD_LEVEL)) && (spells[i].reqlvl+10>=_unit->GetUInt32Value(UNIT_FIELD_LEVEL)))
					{
			      	if(m_spellcheck[i])
		      		{
				        target = _unit->GetAIInterface()->GetNextTarget();
			       	  switch(spells[i].targettype)
			    		  {
			    		    case TARGET_SELF:
				         			_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
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

#define Frost_Nova 10230 
#define Shadow_Shock 16583
#define Consuming_Shadows 24614

class VoidwalkerFamily : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(VoidwalkerFamily);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    VoidwalkerFamily(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// -- Number of spells to add --
		nrspells = 3;

		// --- Initialization ---
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		// ----------------------
		
        spells[0].info = dbcSpell.LookupEntry(Frost_Nova);
		spells[0].reqlvl = 25;
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].perctrigger = (float)RandomFloat(5.0f);
		spells[0].attackstoptimer = 1000; // 1sec
		m_spellcheck[0] = true;

		spells[1].info = dbcSpell.LookupEntry(Shadow_Shock);
		spells[1].reqlvl = 20;
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = false;
		spells[1].perctrigger = (float)RandomFloat(5.0f);
		spells[1].attackstoptimer = 1000; // 1sec
		m_spellcheck[1] = true;

		spells[2].info = dbcSpell.LookupEntry(Consuming_Shadows);
		spells[2].reqlvl = 20;
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = false;
		spells[2].perctrigger = (float)RandomFloat(5.0f);
		spells[2].attackstoptimer = 1000; // 1sec
		m_spellcheck[2] = true;
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

				// do another set of spells on transform
	      			if((spells[i].reqlvl<=_unit->GetUInt32Value(UNIT_FIELD_LEVEL)) && (spells[i].reqlvl+10>=_unit->GetUInt32Value(UNIT_FIELD_LEVEL)))
					{
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
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

class BirdFamily : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(BirdFamily);
	SP_AI_Spell spells[28];
	bool m_spellcheck[28];

    BirdFamily(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 28;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}


		     spells[0].info = dbcSpell.LookupEntry(17253);
		     spells[0].reqlvl = 1;
		     spells[0].targettype = TARGET_ATTACKING;
		     spells[0].instant = false;
		     spells[0].perctrigger = (float)RandomFloat(5.0f);
		     spells[0].attackstoptimer = 1000;
		     m_spellcheck[0] = true;


		     spells[1].info = dbcSpell.LookupEntry(17255);
		     spells[1].reqlvl = 8;
		     spells[1].targettype = TARGET_ATTACKING;
		     spells[1].instant = false;
		     spells[1].perctrigger = (float)RandomFloat(5.0f);
		     spells[1].attackstoptimer = 1000;
		     m_spellcheck[1] = true;


		     spells[2].info = dbcSpell.LookupEntry(17256);
		     spells[2].reqlvl = 16;
		     spells[2].targettype = TARGET_ATTACKING;
		     spells[2].instant = false;
		     spells[2].perctrigger = (float)RandomFloat(5.0f);
		     spells[2].attackstoptimer = 1000;
		     m_spellcheck[2] = true;


		     spells[3].info = dbcSpell.LookupEntry(17257);
		     spells[3].reqlvl = 24;
		     spells[3].targettype = TARGET_ATTACKING;
		     spells[3].instant = false;
		     spells[3].perctrigger = (float)RandomFloat(5.0f);
		     spells[3].attackstoptimer = 1000;
		     m_spellcheck[3] = true;


		     spells[4].info = dbcSpell.LookupEntry(17258);
		     spells[4].reqlvl = 32;
		     spells[4].targettype = TARGET_ATTACKING;
		     spells[4].instant = false;
		     spells[4].perctrigger = (float)RandomFloat(5.0f);
		     spells[4].attackstoptimer = 1000;
		     m_spellcheck[4] = true;


		     spells[5].info = dbcSpell.LookupEntry(17259);
		     spells[5].reqlvl = 40;
		     spells[5].targettype = TARGET_ATTACKING;
		     spells[5].instant = false;
		     spells[5].perctrigger = (float)RandomFloat(5.0f);
		     spells[5].attackstoptimer = 1000;
		     m_spellcheck[5] = true;


		     spells[6].info = dbcSpell.LookupEntry(17260);
		     spells[6].reqlvl = 48;
		     spells[6].targettype = TARGET_ATTACKING;
		     spells[6].instant = false;
		     spells[6].perctrigger = (float)RandomFloat(5.0f);
		     spells[6].attackstoptimer = 1000;
		     m_spellcheck[6] = true;


		     spells[7].info = dbcSpell.LookupEntry(17261);
		     spells[7].reqlvl = 56;
		     spells[7].targettype = TARGET_ATTACKING;
		     spells[7].instant = false;
		     spells[7].perctrigger = (float)RandomFloat(5.0f);
		     spells[7].attackstoptimer = 1000;
		     m_spellcheck[7] = true;


		     spells[8].info = dbcSpell.LookupEntry(27050);
		     spells[8].reqlvl = 64;
		     spells[8].targettype = TARGET_ATTACKING;
		     spells[8].instant = false;
		     spells[8].perctrigger = (float)RandomFloat(5.0f);
		     spells[8].attackstoptimer = 1000;
		     m_spellcheck[8] = true;

			 spells[9].info = dbcSpell.LookupEntry(27050);
		     spells[9].reqlvl = 64;
		     spells[9].targettype = TARGET_ATTACKING;
		     spells[9].instant = false;
		     spells[9].perctrigger = 0;
		     spells[9].attackstoptimer = 1000;
		     m_spellcheck[9] = true;

		     spells[10].info = dbcSpell.LookupEntry(16827);
		     spells[10].reqlvl = 1;
		     spells[10].targettype = TARGET_ATTACKING;
		     spells[10].instant = false;
		     spells[10].perctrigger = (float)RandomFloat(5.0f);
		     spells[10].attackstoptimer = 1000;
		     m_spellcheck[10] = true;


		     spells[11].info = dbcSpell.LookupEntry(16828);
		     spells[11].reqlvl = 8;
		     spells[11].targettype = TARGET_ATTACKING;
		     spells[11].instant = false;
		     spells[11].perctrigger = (float)RandomFloat(5.0f);
		     spells[11].attackstoptimer = 1000;
		     m_spellcheck[11] = true;


		     spells[12].info = dbcSpell.LookupEntry(16829);
		     spells[12].reqlvl = 16;
		     spells[12].targettype = TARGET_ATTACKING;
		     spells[12].instant = false;
		     spells[12].perctrigger = (float)RandomFloat(5.0f);
		     spells[12].attackstoptimer = 1000;
		     m_spellcheck[12] = true;


		     spells[13].info = dbcSpell.LookupEntry(16830);
		     spells[13].reqlvl = 24;
		     spells[13].targettype = TARGET_ATTACKING;
		     spells[13].instant = false;
		     spells[13].perctrigger = (float)RandomFloat(5.0f);
		     spells[13].attackstoptimer = 1000;
		     m_spellcheck[13] = true;


		     spells[14].info = dbcSpell.LookupEntry(16831);
		     spells[14].reqlvl = 32;
		     spells[14].targettype = TARGET_ATTACKING;
		     spells[14].instant = false;
		     spells[14].perctrigger = (float)RandomFloat(5.0f);
		     spells[14].attackstoptimer = 1000;
		     m_spellcheck[14] = true;


		     spells[15].info = dbcSpell.LookupEntry(16832);
		     spells[15].reqlvl = 40;
		     spells[15].targettype = TARGET_ATTACKING;
		     spells[15].instant = false;
		     spells[15].perctrigger = (float)RandomFloat(5.0f);
		     spells[15].attackstoptimer = 1000;
		     m_spellcheck[15] = true;


		     spells[16].info = dbcSpell.LookupEntry(3010);
		     spells[16].reqlvl = 48;
		     spells[16].targettype = TARGET_ATTACKING;
		     spells[16].instant = false;
		     spells[16].perctrigger = (float)RandomFloat(5.0f);
		     spells[16].attackstoptimer = 1000;
		     m_spellcheck[16] = true;


		     spells[17].info = dbcSpell.LookupEntry(3009);
		     spells[17].reqlvl = 56;
		     spells[17].targettype = TARGET_ATTACKING;
		     spells[17].instant = false;
		     spells[17].perctrigger = (float)RandomFloat(5.0f);
		     spells[17].attackstoptimer = 1000;
		     m_spellcheck[17] = true;


		     spells[18].info = dbcSpell.LookupEntry(27049);
		     spells[18].reqlvl = 64;
		     spells[18].targettype = TARGET_ATTACKING;
		     spells[18].instant = false;
		     spells[18].perctrigger = (float)RandomFloat(5.0f);
		     spells[18].attackstoptimer = 1000;
		     m_spellcheck[18] = true;


		     spells[19].info = dbcSpell.LookupEntry(23145);
		     spells[19].reqlvl = 30;
		     spells[19].targettype = TARGET_SELF;
		     spells[19].instant = false;
		     spells[19].perctrigger = (float)RandomFloat(5.0f);
		     spells[19].attackstoptimer = 1000;
		     m_spellcheck[19] = true;


		     spells[20].info = dbcSpell.LookupEntry(23147);
		     spells[20].reqlvl = 40;
		     spells[20].targettype = TARGET_SELF;
		     spells[20].instant = false;
		     spells[20].perctrigger = (float)RandomFloat(5.0f);
		     spells[20].attackstoptimer = 1000;
		     m_spellcheck[20] = true;


		     spells[21].info = dbcSpell.LookupEntry(23148);
		     spells[21].reqlvl = 50;
		     spells[21].targettype = TARGET_SELF;
		     spells[21].instant = false;
		     spells[21].perctrigger = (float)RandomFloat(5.0f);
		     spells[21].attackstoptimer = 1000;
		     m_spellcheck[21] = true;


		     spells[22].info = dbcSpell.LookupEntry(24423);
		     spells[22].reqlvl = 8;
		     spells[22].targettype = TARGET_ATTACKING;
		     spells[22].instant = false;
		     spells[22].perctrigger = (float)RandomFloat(5.0f);
		     spells[22].attackstoptimer = 1000;
		     m_spellcheck[22] = true;


		     spells[23].info = dbcSpell.LookupEntry(24577);
		     spells[23].reqlvl = 24;
		     spells[23].targettype = TARGET_ATTACKING;
		     spells[23].instant = false;
		     spells[23].perctrigger = (float)RandomFloat(5.0f);
		     spells[23].attackstoptimer = 1000;
		     m_spellcheck[23] = true;


		     spells[24].info = dbcSpell.LookupEntry(24578);
		     spells[24].reqlvl = 48;
		     spells[24].targettype = TARGET_ATTACKING;
		     spells[24].instant = false;
		     spells[24].perctrigger = (float)RandomFloat(5.0f);
		     spells[24].attackstoptimer = 1000;
		     m_spellcheck[24] = true;


		     spells[25].info = dbcSpell.LookupEntry(24579);
		     spells[25].reqlvl = 55;
		     spells[25].targettype = TARGET_ATTACKING;
		     spells[25].instant = false;
		     spells[25].perctrigger = (float)RandomFloat(5.0f);
		     spells[25].attackstoptimer = 1000;
		     m_spellcheck[25] = true;


		     spells[26].info = dbcSpell.LookupEntry(24579);
		     spells[26].reqlvl = 64;
		     spells[26].targettype = TARGET_ATTACKING;
		     spells[26].instant = false;
		     spells[26].perctrigger = (float)RandomFloat(5.0f);
		     spells[26].attackstoptimer = 1000;
		     m_spellcheck[26] = true;

		     spells[27].info = dbcSpell.LookupEntry(23147);
		     spells[27].reqlvl = 1;
		     spells[27].targettype = TARGET_ATTACKING;
		     spells[27].instant = false;
		     spells[27].perctrigger = (float)RandomFloat(5.0f);
		     spells[27].attackstoptimer = 1000;
		     m_spellcheck[27] = true;

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
	      			if((spells[i].reqlvl<=_unit->GetUInt32Value(UNIT_FIELD_LEVEL)) && (spells[i].reqlvl+10>=_unit->GetUInt32Value(UNIT_FIELD_LEVEL)))
					{
			      	if(m_spellcheck[i])
		      		{
				        target = _unit->GetAIInterface()->GetNextTarget();
			       	  switch(spells[i].targettype)
			    		  {
			    		    case TARGET_SELF:
				         			_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
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

class ScorpionFamily : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(ScorpionFamily);
	SP_AI_Spell spells[10];
	bool m_spellcheck[10];

    ScorpionFamily(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// -- Number of spells to add --
		nrspells = 10;

		// --- Initialization ---
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		// ----------------------
		
		     spells[0].info = dbcSpell.LookupEntry(27049);
		     spells[0].reqlvl = 64;
		     spells[0].targettype = TARGET_ATTACKING;
		     spells[0].instant = false;
		     spells[0].perctrigger = (float)RandomFloat(5.0f);
		     spells[0].attackstoptimer = 1000;
		     m_spellcheck[0] = true;

		     spells[1].info = dbcSpell.LookupEntry(16827);
		     spells[1].reqlvl = 1;
		     spells[1].targettype = TARGET_ATTACKING;
		     spells[1].instant = false;
		     spells[1].perctrigger = (float)RandomFloat(5.0f);
		     spells[1].attackstoptimer = 1000;
		     m_spellcheck[1] = true;


		     spells[2].info = dbcSpell.LookupEntry(16828);
		     spells[2].reqlvl = 8;
		     spells[2].targettype = TARGET_ATTACKING;
		     spells[2].instant = false;
		     spells[2].perctrigger = (float)RandomFloat(5.0f);
		     spells[2].attackstoptimer = 1000;
		     m_spellcheck[2] = true;


		     spells[3].info = dbcSpell.LookupEntry(16829);
		     spells[3].reqlvl = 16;
		     spells[3].targettype = TARGET_ATTACKING;
		     spells[3].instant = false;
		     spells[3].perctrigger = (float)RandomFloat(5.0f);
		     spells[3].attackstoptimer = 1000;
		     m_spellcheck[3] = true;


		     spells[4].info = dbcSpell.LookupEntry(16830);
		     spells[4].reqlvl = 24;
		     spells[4].targettype = TARGET_ATTACKING;
		     spells[4].instant = false;
		     spells[4].perctrigger = (float)RandomFloat(5.0f);
		     spells[4].attackstoptimer = 1000;
		     m_spellcheck[4] = true;


		     spells[5].info = dbcSpell.LookupEntry(16831);
		     spells[5].reqlvl = 32;
		     spells[5].targettype = TARGET_ATTACKING;
		     spells[5].instant = false;
		     spells[5].perctrigger = (float)RandomFloat(5.0f);
		     spells[5].attackstoptimer = 1000;
		     m_spellcheck[5] = true;


		     spells[6].info = dbcSpell.LookupEntry(16832);
		     spells[6].reqlvl = 40;
		     spells[6].targettype = TARGET_ATTACKING;
		     spells[6].instant = false;
		     spells[6].perctrigger = (float)RandomFloat(5.0f);
		     spells[6].attackstoptimer = 1000;
		     m_spellcheck[6] = true;


		     spells[7].info = dbcSpell.LookupEntry(3010);
		     spells[7].reqlvl = 48;
		     spells[7].targettype = TARGET_ATTACKING;
		     spells[7].instant = false;
		     spells[7].perctrigger = (float)RandomFloat(5.0f);
		     spells[7].attackstoptimer = 1000;
		     m_spellcheck[7] = true;


		     spells[8].info = dbcSpell.LookupEntry(3009);
		     spells[8].reqlvl = 56;
		     spells[8].targettype = TARGET_ATTACKING;
		     spells[8].instant = false;
		     spells[8].perctrigger = (float)RandomFloat(5.0f);
		     spells[8].attackstoptimer = 1000;
		     m_spellcheck[8] = true;

		     spells[9].info = dbcSpell.LookupEntry(3609);
		     spells[9].reqlvl = 1;
		     spells[9].targettype = TARGET_ATTACKING;
		     spells[9].instant = false;
		     spells[9].perctrigger = (float)RandomFloat(5.0f);
		     spells[9].attackstoptimer = 1000;
		     m_spellcheck[9] = true;

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

				// do another set of spells on transform
	      			if((spells[i].reqlvl<=_unit->GetUInt32Value(UNIT_FIELD_LEVEL)) && (spells[i].reqlvl+10>=_unit->GetUInt32Value(UNIT_FIELD_LEVEL)))
					{
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
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

/* #define Rabies 3150 // If u have that buff and u kill wolf ur instanly dead */

class WolfFamily : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(WolfFamily);
	SP_AI_Spell spells[16];
	bool m_spellcheck[16];

    WolfFamily(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// -- Number of spells to add --
		nrspells = 16;

		// --- Initialization ---
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		// ----------------------
		
		     spells[0].info = dbcSpell.LookupEntry(17253);
		     spells[0].reqlvl = 1;
		     spells[0].targettype = TARGET_ATTACKING;
		     spells[0].instant = false;
		     spells[0].perctrigger = (float)RandomFloat(5.0f);
		     spells[0].attackstoptimer = 1000;
		     m_spellcheck[0] = true;


		     spells[1].info = dbcSpell.LookupEntry(17255);
		     spells[1].reqlvl = 8;
		     spells[1].targettype = TARGET_ATTACKING;
		     spells[1].instant = false;
		     spells[1].perctrigger = (float)RandomFloat(5.0f);
		     spells[1].attackstoptimer = 1000;
		     m_spellcheck[1] = true;


		     spells[2].info = dbcSpell.LookupEntry(17256);
		     spells[2].reqlvl = 16;
		     spells[2].targettype = TARGET_ATTACKING;
		     spells[2].instant = false;
		     spells[2].perctrigger = (float)RandomFloat(5.0f);
		     spells[2].attackstoptimer = 1000;
		     m_spellcheck[2] = true;


		     spells[3].info = dbcSpell.LookupEntry(17257);
		     spells[3].reqlvl = 24;
		     spells[3].targettype = TARGET_ATTACKING;
		     spells[3].instant = false;
		     spells[3].perctrigger = (float)RandomFloat(5.0f);
		     spells[3].attackstoptimer = 1000;
		     m_spellcheck[3] = true;


		     spells[4].info = dbcSpell.LookupEntry(17258);
		     spells[4].reqlvl = 32;
		     spells[4].targettype = TARGET_ATTACKING;
		     spells[4].instant = false;
		     spells[4].perctrigger = (float)RandomFloat(5.0f);
		     spells[4].attackstoptimer = 1000;
		     m_spellcheck[4] = true;


		     spells[5].info = dbcSpell.LookupEntry(17259);
		     spells[5].reqlvl = 40;
		     spells[5].targettype = TARGET_ATTACKING;
		     spells[5].instant = false;
		     spells[5].perctrigger = (float)RandomFloat(5.0f);
		     spells[5].attackstoptimer = 1000;
		     m_spellcheck[5] = true;


		     spells[6].info = dbcSpell.LookupEntry(17260);
		     spells[6].reqlvl = 48;
		     spells[6].targettype = TARGET_ATTACKING;
		     spells[6].instant = false;
		     spells[6].perctrigger = (float)RandomFloat(5.0f);
		     spells[6].attackstoptimer = 1000;
		     m_spellcheck[6] = true;


		     spells[7].info = dbcSpell.LookupEntry(17261);
		     spells[7].reqlvl = 56;
		     spells[7].targettype = TARGET_ATTACKING;
		     spells[7].instant = false;
		     spells[7].perctrigger = (float)RandomFloat(5.0f);
		     spells[7].attackstoptimer = 1000;
		     m_spellcheck[7] = true;


		     spells[8].info = dbcSpell.LookupEntry(27050);
		     spells[8].reqlvl = 64;
		     spells[8].targettype = TARGET_ATTACKING;
		     spells[8].instant = false;
		     spells[8].perctrigger = (float)RandomFloat(5.0f);
		     spells[8].attackstoptimer = 1000;
		     m_spellcheck[8] = true;

		     spells[9].info = dbcSpell.LookupEntry(23099);
		     spells[9].reqlvl = 30;
		     spells[9].targettype = TARGET_SELF;
		     spells[9].instant = false;
		     spells[9].perctrigger = (float)RandomFloat(5.0f);
		     spells[9].attackstoptimer = 1000;
		     m_spellcheck[9] = true;


		     spells[10].info = dbcSpell.LookupEntry(23109);
		     spells[10].reqlvl = 40;
		     spells[10].targettype = TARGET_SELF;
		     spells[10].instant = false;
		     spells[10].perctrigger = (float)RandomFloat(5.0f);
		     spells[10].attackstoptimer = 1000;
		     m_spellcheck[10] = true;


		     spells[11].info = dbcSpell.LookupEntry(23110);
		     spells[11].reqlvl = 50;
		     spells[11].targettype = TARGET_SELF;
		     spells[11].instant = false;
		     spells[11].perctrigger = (float)RandomFloat(5.0f);
		     spells[11].attackstoptimer = 1000;
		     m_spellcheck[11] = true;


		     spells[12].info = dbcSpell.LookupEntry(24604);
		     spells[12].reqlvl = 10;
		     spells[12].targettype = TARGET_SELF;
		     spells[12].instant = false;
		     spells[12].perctrigger = (float)RandomFloat(5.0f);
		     spells[12].attackstoptimer = 1000;
		     m_spellcheck[12] = true;


		     spells[13].info = dbcSpell.LookupEntry(24605);
		     spells[13].reqlvl = 24;
		     spells[13].targettype = TARGET_SELF;
		     spells[13].instant = false;
		     spells[13].perctrigger = (float)RandomFloat(5.0f);
		     spells[13].attackstoptimer = 1000;
		     m_spellcheck[13] = true;


		     spells[14].info = dbcSpell.LookupEntry(24603);
		     spells[14].reqlvl = 40;
		     spells[14].targettype = TARGET_SELF;
		     spells[14].instant = false;
		     spells[14].perctrigger = (float)RandomFloat(5.0f);
		     spells[14].attackstoptimer = 1000;
		     m_spellcheck[14] = true;


		     spells[15].info = dbcSpell.LookupEntry(24597);
		     spells[15].reqlvl = 56;
		     spells[15].targettype = TARGET_SELF;
		     spells[15].instant = false;
		     spells[15].perctrigger = (float)RandomFloat(5.0f);
		     spells[15].attackstoptimer = 1000;
		     m_spellcheck[15] = true;

		/* 
		spells[2].info = dbcSpell.LookupEntry(Rabies);
		spells[2].reqlvl = 0;
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = false;
		spells[2].perctrigger = (float)RandomFloat(5.0f);
		spells[2].attackstoptimer = 1000; // 1sec
		m_spellcheck[2] = true;
		*/
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

				// do another set of spells on transform
	      			if((spells[i].reqlvl<=_unit->GetUInt32Value(UNIT_FIELD_LEVEL)) && (spells[i].reqlvl+10>=_unit->GetUInt32Value(UNIT_FIELD_LEVEL)))
					{
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
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

class GorillaFamily : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(GorillaFamily);
	SP_AI_Spell spells[13];
	bool m_spellcheck[13];

    GorillaFamily(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// -- Number of spells to add --
		nrspells = 13;

		// --- Initialization ---
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		// ----------------------
		
		     spells[0].info = dbcSpell.LookupEntry(17253);
		     spells[0].reqlvl = 1;
		     spells[0].targettype = TARGET_ATTACKING;
		     spells[0].instant = false;
		     spells[0].perctrigger = (float)RandomFloat(5.0f);
		     spells[0].attackstoptimer = 1000;
		     m_spellcheck[0] = true;


		     spells[1].info = dbcSpell.LookupEntry(17255);
		     spells[1].reqlvl = 8;
		     spells[1].targettype = TARGET_ATTACKING;
		     spells[1].instant = false;
		     spells[1].perctrigger = (float)RandomFloat(5.0f);
		     spells[1].attackstoptimer = 1000;
		     m_spellcheck[1] = true;


		     spells[2].info = dbcSpell.LookupEntry(17256);
		     spells[2].reqlvl = 16;
		     spells[2].targettype = TARGET_ATTACKING;
		     spells[2].instant = false;
		     spells[2].perctrigger = (float)RandomFloat(5.0f);
		     spells[2].attackstoptimer = 1000;
		     m_spellcheck[2] = true;


		     spells[3].info = dbcSpell.LookupEntry(17257);
		     spells[3].reqlvl = 24;
		     spells[3].targettype = TARGET_ATTACKING;
		     spells[3].instant = false;
		     spells[3].perctrigger = (float)RandomFloat(5.0f);
		     spells[3].attackstoptimer = 1000;
		     m_spellcheck[3] = true;


		     spells[4].info = dbcSpell.LookupEntry(17258);
		     spells[4].reqlvl = 32;
		     spells[4].targettype = TARGET_ATTACKING;
		     spells[4].instant = false;
		     spells[4].perctrigger = (float)RandomFloat(5.0f);
		     spells[4].attackstoptimer = 1000;
		     m_spellcheck[4] = true;


		     spells[5].info = dbcSpell.LookupEntry(17259);
		     spells[5].reqlvl = 40;
		     spells[5].targettype = TARGET_ATTACKING;
		     spells[5].instant = false;
		     spells[5].perctrigger = (float)RandomFloat(5.0f);
		     spells[5].attackstoptimer = 1000;
		     m_spellcheck[5] = true;


		     spells[6].info = dbcSpell.LookupEntry(17260);
		     spells[6].reqlvl = 48;
		     spells[6].targettype = TARGET_ATTACKING;
		     spells[6].instant = false;
		     spells[6].perctrigger = (float)RandomFloat(5.0f);
		     spells[6].attackstoptimer = 1000;
		     m_spellcheck[6] = true;


		     spells[7].info = dbcSpell.LookupEntry(17261);
		     spells[7].reqlvl = 56;
		     spells[7].targettype = TARGET_ATTACKING;
		     spells[7].instant = false;
		     spells[7].perctrigger = (float)RandomFloat(5.0f);
		     spells[7].attackstoptimer = 1000;
		     m_spellcheck[7] = true;


		     spells[8].info = dbcSpell.LookupEntry(27050);
		     spells[8].reqlvl = 64;
		     spells[8].targettype = TARGET_ATTACKING;
		     spells[8].instant = false;
		     spells[8].perctrigger = (float)RandomFloat(5.0f);
		     spells[8].attackstoptimer = 1000;
		     m_spellcheck[8] = true;

		     spells[9].info = dbcSpell.LookupEntry(26090);
		     spells[9].reqlvl = 30;
		     spells[9].targettype = TARGET_DESTINATION;
		     spells[9].instant = false;
		     spells[9].perctrigger = (float)RandomFloat(5.0f);
		     spells[9].attackstoptimer = 1000;
		     m_spellcheck[9] = true;


		     spells[10].info = dbcSpell.LookupEntry(26187);
		     spells[10].reqlvl = 30;
		     spells[10].targettype = TARGET_DESTINATION;
		     spells[10].instant = false;
		     spells[10].perctrigger = (float)RandomFloat(5.0f);
		     spells[10].attackstoptimer = 1000;
		     m_spellcheck[10] = true;


		     spells[11].info = dbcSpell.LookupEntry(26188);
		     spells[11].reqlvl = 30;
		     spells[11].targettype = TARGET_DESTINATION;
		     spells[11].instant = false;
		     spells[11].perctrigger = (float)RandomFloat(5.0f);
		     spells[11].attackstoptimer = 1000;
		     m_spellcheck[11] = true;


		     spells[12].info = dbcSpell.LookupEntry(27063);
		     spells[12].reqlvl = 30;
		     spells[12].targettype = TARGET_DESTINATION;
		     spells[12].instant = false;
		     spells[12].perctrigger = (float)RandomFloat(5.0f);
		     spells[12].attackstoptimer = 1000;
		     m_spellcheck[12] = true;

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

				// do another set of spells on transform
	      			if((spells[i].reqlvl<=_unit->GetUInt32Value(UNIT_FIELD_LEVEL)) && (spells[i].reqlvl+10>=_unit->GetUInt32Value(UNIT_FIELD_LEVEL)))
					{
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
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

// Only if type!=8
class CrabFamily : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(CrabFamily);
	SP_AI_Spell spells[9];
	bool m_spellcheck[9];

    CrabFamily(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// -- Number of spells to add --
		nrspells = 9;

		// --- Initialization ---
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		// ----------------------
		
		     spells[0].info = dbcSpell.LookupEntry(27049);
		     spells[0].reqlvl = 64;
		     spells[0].targettype = TARGET_ATTACKING;
		     spells[0].instant = false;
		     spells[0].perctrigger = (float)RandomFloat(5.0f);
		     spells[0].attackstoptimer = 1000;
		     m_spellcheck[0] = true;

		     spells[1].info = dbcSpell.LookupEntry(16827);
		     spells[1].reqlvl = 1;
		     spells[1].targettype = TARGET_ATTACKING;
		     spells[1].instant = false;
		     spells[1].perctrigger = (float)RandomFloat(5.0f);
		     spells[1].attackstoptimer = 1000;
		     m_spellcheck[1] = true;


		     spells[2].info = dbcSpell.LookupEntry(16828);
		     spells[2].reqlvl = 8;
		     spells[2].targettype = TARGET_ATTACKING;
		     spells[2].instant = false;
		     spells[2].perctrigger = (float)RandomFloat(5.0f);
		     spells[2].attackstoptimer = 1000;
		     m_spellcheck[2] = true;


		     spells[3].info = dbcSpell.LookupEntry(16829);
		     spells[3].reqlvl = 16;
		     spells[3].targettype = TARGET_ATTACKING;
		     spells[3].instant = false;
		     spells[3].perctrigger = (float)RandomFloat(5.0f);
		     spells[3].attackstoptimer = 1000;
		     m_spellcheck[3] = true;


		     spells[4].info = dbcSpell.LookupEntry(16830);
		     spells[4].reqlvl = 24;
		     spells[4].targettype = TARGET_ATTACKING;
		     spells[4].instant = false;
		     spells[4].perctrigger = (float)RandomFloat(5.0f);
		     spells[4].attackstoptimer = 1000;
		     m_spellcheck[4] = true;


		     spells[5].info = dbcSpell.LookupEntry(16831);
		     spells[5].reqlvl = 32;
		     spells[5].targettype = TARGET_ATTACKING;
		     spells[5].instant = false;
		     spells[5].perctrigger = (float)RandomFloat(5.0f);
		     spells[5].attackstoptimer = 1000;
		     m_spellcheck[5] = true;


		     spells[6].info = dbcSpell.LookupEntry(16832);
		     spells[6].reqlvl = 40;
		     spells[6].targettype = TARGET_ATTACKING;
		     spells[6].instant = false;
		     spells[6].perctrigger = (float)RandomFloat(5.0f);
		     spells[6].attackstoptimer = 1000;
		     m_spellcheck[6] = true;


		     spells[7].info = dbcSpell.LookupEntry(3010);
		     spells[7].reqlvl = 48;
		     spells[7].targettype = TARGET_ATTACKING;
		     spells[7].instant = false;
		     spells[7].perctrigger = (float)RandomFloat(5.0f);
		     spells[7].attackstoptimer = 1000;
		     m_spellcheck[7] = true;


		     spells[8].info = dbcSpell.LookupEntry(3009);
		     spells[8].reqlvl = 56;
		     spells[8].targettype = TARGET_ATTACKING;
		     spells[8].instant = false;
		     spells[8].perctrigger = (float)RandomFloat(5.0f);
		     spells[8].attackstoptimer = 1000;
		     m_spellcheck[8] = true;
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

				// do another set of spells on transform
	      			if((spells[i].reqlvl<=_unit->GetUInt32Value(UNIT_FIELD_LEVEL)) && (spells[i].reqlvl+10>=_unit->GetUInt32Value(UNIT_FIELD_LEVEL)))
					{
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
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

class BearFamily : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(BearFamily);
	SP_AI_Spell spells[18];
	bool m_spellcheck[18];

    BearFamily(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// -- Number of spells to add --
		nrspells = 18;

		// --- Initialization ---
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		// ----------------------
		
		     spells[0].info = dbcSpell.LookupEntry(27049);
		     spells[0].reqlvl = 64;
		     spells[0].targettype = TARGET_ATTACKING;
		     spells[0].instant = false;
		     spells[0].perctrigger = (float)RandomFloat(5.0f);
		     spells[0].attackstoptimer = 1000;
		     m_spellcheck[0] = true;

		     spells[1].info = dbcSpell.LookupEntry(16827);
		     spells[1].reqlvl = 1;
		     spells[1].targettype = TARGET_ATTACKING;
		     spells[1].instant = false;
		     spells[1].perctrigger = (float)RandomFloat(5.0f);
		     spells[1].attackstoptimer = 1000;
		     m_spellcheck[1] = true;


		     spells[2].info = dbcSpell.LookupEntry(16828);
		     spells[2].reqlvl = 8;
		     spells[2].targettype = TARGET_ATTACKING;
		     spells[2].instant = false;
		     spells[2].perctrigger = (float)RandomFloat(5.0f);
		     spells[2].attackstoptimer = 1000;
		     m_spellcheck[2] = true;


		     spells[3].info = dbcSpell.LookupEntry(16829);
		     spells[3].reqlvl = 16;
		     spells[3].targettype = TARGET_ATTACKING;
		     spells[3].instant = false;
		     spells[3].perctrigger = (float)RandomFloat(5.0f);
		     spells[3].attackstoptimer = 1000;
		     m_spellcheck[3] = true;


		     spells[4].info = dbcSpell.LookupEntry(16830);
		     spells[4].reqlvl = 24;
		     spells[4].targettype = TARGET_ATTACKING;
		     spells[4].instant = false;
		     spells[4].perctrigger = (float)RandomFloat(5.0f);
		     spells[4].attackstoptimer = 1000;
		     m_spellcheck[4] = true;


		     spells[5].info = dbcSpell.LookupEntry(16831);
		     spells[5].reqlvl = 32;
		     spells[5].targettype = TARGET_ATTACKING;
		     spells[5].instant = false;
		     spells[5].perctrigger = (float)RandomFloat(5.0f);
		     spells[5].attackstoptimer = 1000;
		     m_spellcheck[5] = true;


		     spells[6].info = dbcSpell.LookupEntry(16832);
		     spells[6].reqlvl = 40;
		     spells[6].targettype = TARGET_ATTACKING;
		     spells[6].instant = false;
		     spells[6].perctrigger = (float)RandomFloat(5.0f);
		     spells[6].attackstoptimer = 1000;
		     m_spellcheck[6] = true;


		     spells[7].info = dbcSpell.LookupEntry(3010);
		     spells[7].reqlvl = 48;
		     spells[7].targettype = TARGET_ATTACKING;
		     spells[7].instant = false;
		     spells[7].perctrigger = (float)RandomFloat(5.0f);
		     spells[7].attackstoptimer = 1000;
		     m_spellcheck[7] = true;


		     spells[8].info = dbcSpell.LookupEntry(3009);
		     spells[8].reqlvl = 56;
		     spells[8].targettype = TARGET_ATTACKING;
		     spells[8].instant = false;
		     spells[8].perctrigger = (float)RandomFloat(5.0f);
		     spells[8].attackstoptimer = 1000;
		     m_spellcheck[8] = true;

		     spells[9].info = dbcSpell.LookupEntry(17253);
		     spells[9].reqlvl = 1;
		     spells[9].targettype = TARGET_ATTACKING;
		     spells[9].instant = false;
		     spells[9].perctrigger = (float)RandomFloat(5.0f);
		     spells[9].attackstoptimer = 1000;
		     m_spellcheck[9] = true;


		     spells[10].info = dbcSpell.LookupEntry(17255);
		     spells[10].reqlvl = 8;
		     spells[10].targettype = TARGET_ATTACKING;
		     spells[10].instant = false;
		     spells[10].perctrigger = (float)RandomFloat(5.0f);
		     spells[10].attackstoptimer = 1000;
		     m_spellcheck[10] = true;


		     spells[11].info = dbcSpell.LookupEntry(17256);
		     spells[11].reqlvl = 16;
		     spells[11].targettype = TARGET_ATTACKING;
		     spells[11].instant = false;
		     spells[11].perctrigger = (float)RandomFloat(5.0f);
		     spells[11].attackstoptimer = 1000;
		     m_spellcheck[11] = true;


		     spells[12].info = dbcSpell.LookupEntry(17257);
		     spells[12].reqlvl = 24;
		     spells[12].targettype = TARGET_ATTACKING;
		     spells[12].instant = false;
		     spells[12].perctrigger = (float)RandomFloat(5.0f);
		     spells[12].attackstoptimer = 1000;
		     m_spellcheck[12] = true;


		     spells[13].info = dbcSpell.LookupEntry(17258);
		     spells[13].reqlvl = 32;
		     spells[13].targettype = TARGET_ATTACKING;
		     spells[13].instant = false;
		     spells[13].perctrigger = (float)RandomFloat(5.0f);
		     spells[13].attackstoptimer = 1000;
		     m_spellcheck[13] = true;


		     spells[14].info = dbcSpell.LookupEntry(17259);
		     spells[14].reqlvl = 40;
		     spells[14].targettype = TARGET_ATTACKING;
		     spells[14].instant = false;
		     spells[14].perctrigger = (float)RandomFloat(5.0f);
		     spells[14].attackstoptimer = 1000;
		     m_spellcheck[14] = true;


		     spells[15].info = dbcSpell.LookupEntry(17260);
		     spells[15].reqlvl = 48;
		     spells[15].targettype = TARGET_ATTACKING;
		     spells[15].instant = false;
		     spells[15].perctrigger = (float)RandomFloat(5.0f);
		     spells[15].attackstoptimer = 1000;
		     m_spellcheck[15] = true;


		     spells[16].info = dbcSpell.LookupEntry(17261);
		     spells[16].reqlvl = 56;
		     spells[16].targettype = TARGET_ATTACKING;
		     spells[16].instant = false;
		     spells[16].perctrigger = (float)RandomFloat(5.0f);
		     spells[16].attackstoptimer = 1000;
		     m_spellcheck[16] = true;


		     spells[17].info = dbcSpell.LookupEntry(27050);
		     spells[17].reqlvl = 64;
		     spells[17].targettype = TARGET_ATTACKING;
		     spells[17].instant = false;
		     spells[17].perctrigger = (float)RandomFloat(5.0f);
		     spells[17].attackstoptimer = 1000;
		     m_spellcheck[17] = true;

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

				// do another set of spells on transform
	      			if((spells[i].reqlvl<=_unit->GetUInt32Value(UNIT_FIELD_LEVEL)) && (spells[i].reqlvl+10>=_unit->GetUInt32Value(UNIT_FIELD_LEVEL)))
					{
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
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

class BoarFamily : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(BoarFamily);
	SP_AI_Spell spells[27];
	bool m_spellcheck[27];

    BoarFamily(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// -- Number of spells to add --
		nrspells = 27;

		// --- Initialization ---
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		// ----------------------
		
		     spells[0].info = dbcSpell.LookupEntry(17253);
		     spells[0].reqlvl = 1;
		     spells[0].targettype = TARGET_ATTACKING;
		     spells[0].instant = false;
		     spells[0].perctrigger = (float)RandomFloat(5.0f);
		     spells[0].attackstoptimer = 1000;
		     m_spellcheck[0] = true;


		     spells[1].info = dbcSpell.LookupEntry(17255);
		     spells[1].reqlvl = 8;
		     spells[1].targettype = TARGET_ATTACKING;
		     spells[1].instant = false;
		     spells[1].perctrigger = (float)RandomFloat(5.0f);
		     spells[1].attackstoptimer = 1000;
		     m_spellcheck[1] = true;


		     spells[2].info = dbcSpell.LookupEntry(17256);
		     spells[2].reqlvl = 16;
		     spells[2].targettype = TARGET_ATTACKING;
		     spells[2].instant = false;
		     spells[2].perctrigger = (float)RandomFloat(5.0f);
		     spells[2].attackstoptimer = 1000;
		     m_spellcheck[2] = true;


		     spells[3].info = dbcSpell.LookupEntry(17257);
		     spells[3].reqlvl = 24;
		     spells[3].targettype = TARGET_ATTACKING;
		     spells[3].instant = false;
		     spells[3].perctrigger = (float)RandomFloat(5.0f);
		     spells[3].attackstoptimer = 1000;
		     m_spellcheck[3] = true;


		     spells[4].info = dbcSpell.LookupEntry(17258);
		     spells[4].reqlvl = 32;
		     spells[4].targettype = TARGET_ATTACKING;
		     spells[4].instant = false;
		     spells[4].perctrigger = (float)RandomFloat(5.0f);
		     spells[4].attackstoptimer = 1000;
		     m_spellcheck[4] = true;


		     spells[5].info = dbcSpell.LookupEntry(17259);
		     spells[5].reqlvl = 40;
		     spells[5].targettype = TARGET_ATTACKING;
		     spells[5].instant = false;
		     spells[5].perctrigger = (float)RandomFloat(5.0f);
		     spells[5].attackstoptimer = 1000;
		     m_spellcheck[5] = true;


		     spells[6].info = dbcSpell.LookupEntry(17260);
		     spells[6].reqlvl = 48;
		     spells[6].targettype = TARGET_ATTACKING;
		     spells[6].instant = false;
		     spells[6].perctrigger = (float)RandomFloat(5.0f);
		     spells[6].attackstoptimer = 1000;
		     m_spellcheck[6] = true;


		     spells[7].info = dbcSpell.LookupEntry(17261);
		     spells[7].reqlvl = 56;
		     spells[7].targettype = TARGET_ATTACKING;
		     spells[7].instant = false;
		     spells[7].perctrigger = (float)RandomFloat(5.0f);
		     spells[7].attackstoptimer = 1000;
		     m_spellcheck[7] = true;


		     spells[8].info = dbcSpell.LookupEntry(27050);
		     spells[8].reqlvl = 64;
		     spells[8].targettype = TARGET_ATTACKING;
		     spells[8].instant = false;
		     spells[8].perctrigger = (float)RandomFloat(5.0f);
		     spells[8].attackstoptimer = 1000;
		     m_spellcheck[8] = true;

 		     spells[9].info = dbcSpell.LookupEntry(23099);
		     spells[9].reqlvl = 30;
		     spells[9].targettype = TARGET_SELF;
		     spells[9].instant = false;
		     spells[9].perctrigger = (float)RandomFloat(5.0f);
		     spells[9].attackstoptimer = 1000;
		     m_spellcheck[9] = true;


		     spells[10].info = dbcSpell.LookupEntry(23109);
		     spells[10].reqlvl = 40;
		     spells[10].targettype = TARGET_SELF;
		     spells[10].instant = false;
		     spells[10].perctrigger = (float)RandomFloat(5.0f);
		     spells[10].attackstoptimer = 1000;
		     m_spellcheck[10] = true;


		     spells[11].info = dbcSpell.LookupEntry(23110);
		     spells[11].reqlvl = 50;
		     spells[11].targettype = TARGET_SELF;
		     spells[11].instant = false;
		     spells[11].perctrigger = (float)RandomFloat(5.0f);
		     spells[11].attackstoptimer = 1000;
		     m_spellcheck[11] = true;

		     spells[12].info = dbcSpell.LookupEntry(7371);
		     spells[12].reqlvl = 1;
		     spells[12].targettype = TARGET_ATTACKING;
		     spells[12].instant = false;
		     spells[12].perctrigger = (float)RandomFloat(5.0f);
		     spells[12].attackstoptimer = 1000;
		     m_spellcheck[12] = true;


		     spells[13].info = dbcSpell.LookupEntry(26177);
		     spells[13].reqlvl = 12;
		     spells[13].targettype = TARGET_ATTACKING;
		     spells[13].instant = false;
		     spells[13].perctrigger = (float)RandomFloat(5.0f);
		     spells[13].attackstoptimer = 1000;
		     m_spellcheck[13] = true;


		     spells[14].info = dbcSpell.LookupEntry(26178);
		     spells[14].reqlvl = 24;
		     spells[14].targettype = TARGET_ATTACKING;
		     spells[14].instant = false;
		     spells[14].perctrigger = (float)RandomFloat(5.0f);
		     spells[14].attackstoptimer = 1000;
		     m_spellcheck[14] = true;


		     spells[15].info = dbcSpell.LookupEntry(26179);
		     spells[15].reqlvl = 36;
		     spells[15].targettype = TARGET_ATTACKING;
		     spells[15].instant = false;
		     spells[15].perctrigger = (float)RandomFloat(5.0f);
		     spells[15].attackstoptimer = 1000;
		     m_spellcheck[15] = true;


		     spells[16].info = dbcSpell.LookupEntry(26201);
		     spells[16].reqlvl = 48;
		     spells[16].targettype = TARGET_ATTACKING;
		     spells[16].instant = false;
		     spells[16].perctrigger = (float)RandomFloat(5.0f);
		     spells[16].attackstoptimer = 1000;
		     m_spellcheck[16] = true;


		     spells[17].info = dbcSpell.LookupEntry(27685);
		     spells[17].reqlvl = 60;
		     spells[17].targettype = TARGET_ATTACKING;
		     spells[17].instant = false;
		     spells[17].perctrigger = (float)RandomFloat(5.0f);
		     spells[17].attackstoptimer = 1000;
		     m_spellcheck[17] = true;

 
		     spells[18].info = dbcSpell.LookupEntry(35290);
		     spells[18].reqlvl = 1;
		     spells[18].targettype = TARGET_ATTACKING;
		     spells[18].instant = false;
		     spells[18].perctrigger = (float)RandomFloat(5.0f);
		     spells[18].attackstoptimer = 1000;
		     m_spellcheck[18] = true;


		     spells[19].info = dbcSpell.LookupEntry(35291);
		     spells[19].reqlvl = 8;
		     spells[19].targettype = TARGET_ATTACKING;
		     spells[19].instant = false;
		     spells[19].perctrigger = (float)RandomFloat(5.0f);
		     spells[19].attackstoptimer = 1000;
		     m_spellcheck[19] = true;


		     spells[20].info = dbcSpell.LookupEntry(35292);
		     spells[20].reqlvl = 16;
		     spells[20].targettype = TARGET_ATTACKING;
		     spells[20].instant = false;
		     spells[20].perctrigger = (float)RandomFloat(5.0f);
		     spells[20].attackstoptimer = 1000;
		     m_spellcheck[20] = true;


		     spells[21].info = dbcSpell.LookupEntry(35293);
		     spells[21].reqlvl = 24;
		     spells[21].targettype = TARGET_ATTACKING;
		     spells[21].instant = false;
		     spells[21].perctrigger = (float)RandomFloat(5.0f);
		     spells[21].attackstoptimer = 1000;
		     m_spellcheck[21] = true;


		     spells[22].info = dbcSpell.LookupEntry(35294);
		     spells[22].reqlvl = 32;
		     spells[22].targettype = TARGET_ATTACKING;
		     spells[22].instant = false;
		     spells[22].perctrigger = (float)RandomFloat(5.0f);
		     spells[22].attackstoptimer = 1000;
		     m_spellcheck[22] = true;


		     spells[23].info = dbcSpell.LookupEntry(35295);
		     spells[23].reqlvl = 40;
		     spells[23].targettype = TARGET_ATTACKING;
		     spells[23].instant = false;
		     spells[23].perctrigger = (float)RandomFloat(5.0f);
		     spells[23].attackstoptimer = 1000;
		     m_spellcheck[23] = true;


		     spells[24].info = dbcSpell.LookupEntry(35296);
		     spells[24].reqlvl = 48;
		     spells[24].targettype = TARGET_ATTACKING;
		     spells[24].instant = false;
		     spells[24].perctrigger = (float)RandomFloat(5.0f);
		     spells[24].attackstoptimer = 1000;
		     m_spellcheck[24] = true;


		     spells[25].info = dbcSpell.LookupEntry(35297);
		     spells[25].reqlvl = 56;
		     spells[25].targettype = TARGET_ATTACKING;
		     spells[25].instant = false;
		     spells[25].perctrigger = (float)RandomFloat(5.0f);
		     spells[25].attackstoptimer = 1000;
		     m_spellcheck[25] = true;


		     spells[26].info = dbcSpell.LookupEntry(35298);
		     spells[26].reqlvl = 63;
		     spells[26].targettype = TARGET_ATTACKING;
		     spells[26].instant = false;
		     spells[26].perctrigger = (float)RandomFloat(5.0f);
		     spells[26].attackstoptimer = 1000;
		     m_spellcheck[26] = true;

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

				// do another set of spells on transform
	      			if((spells[i].reqlvl<=_unit->GetUInt32Value(UNIT_FIELD_LEVEL)) && (spells[i].reqlvl+10>=_unit->GetUInt32Value(UNIT_FIELD_LEVEL)))
					{
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
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
// End of family based ai's
// Type based AI's
class DragonType : public CreatureAIScript
	// 8873 9573 16390 16396 20712 (self)
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(DragonType);
	SP_AI_Spell spells[5];
	bool m_spellcheck[5];

    DragonType(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// -- Number of spells to add --
		nrspells = 5;

		// --- Initialization ---
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		// ----------------------
		
        spells[0].info = dbcSpell.LookupEntry(8873);
		spells[0].reqlvl = 1;
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = (float)RandomFloat(5.0f);
		spells[0].attackstoptimer = 1000; // 1sec
		m_spellcheck[0] = true;

		spells[1].info = dbcSpell.LookupEntry(9573);
		spells[1].reqlvl = 10;
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = false;
		spells[1].perctrigger = (float)RandomFloat(5.0f);
		spells[1].attackstoptimer = 1000; // 1sec
		m_spellcheck[1] = true;

		spells[2].info = dbcSpell.LookupEntry(16390);
		spells[1].reqlvl = 20;
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = false;
		spells[2].perctrigger = (float)RandomFloat(5.0f);
		spells[2].attackstoptimer = 1000; // 1sec
		m_spellcheck[2] = true;

		spells[3].info = dbcSpell.LookupEntry(16396);
		spells[3].reqlvl = 20;
		spells[3].targettype = TARGET_ATTACKING;
		spells[3].instant = false;
		spells[3].perctrigger = (float)RandomFloat(5.0f);
		spells[3].attackstoptimer = 1000; // 1sec
		m_spellcheck[3] = true;

		spells[4].info = dbcSpell.LookupEntry(20712);
		spells[4].reqlvl = 60;
		spells[4].targettype = TARGET_SELF;
		spells[4].instant = false;
		spells[4].perctrigger = (float)RandomFloat(5.0f);
		spells[4].attackstoptimer = 1000; // 1sec
		m_spellcheck[4] = true;

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

				// do another set of spells on transform
	      			if((spells[i].reqlvl<=_unit->GetUInt32Value(UNIT_FIELD_LEVEL)) && (spells[i].reqlvl+10>=_unit->GetUInt32Value(UNIT_FIELD_LEVEL)))
					{
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
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

class MechanicalType : public CreatureAIScript
	// 9483 91 12021 11837 8277 403 474 (Self)
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(MechanicalType);
	SP_AI_Spell spells[7];
	bool m_spellcheck[7];

    MechanicalType(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// -- Number of spells to add --
		nrspells = 7;

		// --- Initialization ---
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		// ----------------------
		
        spells[0].info = dbcSpell.LookupEntry(9483);
		spells[0].reqlvl = 20;
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = (float)RandomFloat(5.0f);
		spells[0].attackstoptimer = 1000; // 1sec
		m_spellcheck[0] = true;

		spells[1].info = dbcSpell.LookupEntry(91);
		spells[1].reqlvl = 5;
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = false;
		spells[1].perctrigger = (float)RandomFloat(5.0f);
		spells[1].attackstoptimer = 1000; // 1sec
		m_spellcheck[1] = true;

		spells[2].info = dbcSpell.LookupEntry(12021);
		spells[2].reqlvl = 1;
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = false;
		spells[2].perctrigger = (float)RandomFloat(5.0f);
		spells[2].attackstoptimer = 1000; // 1sec
		m_spellcheck[2] = true;

		spells[3].info = dbcSpell.LookupEntry(11837);
		spells[3].reqlvl = 1;
		spells[3].targettype = TARGET_ATTACKING;
		spells[3].instant = false;
		spells[3].perctrigger = (float)RandomFloat(5.0f);
		spells[3].attackstoptimer = 1000; // 1sec
		m_spellcheck[3] = true;

		spells[4].info = dbcSpell.LookupEntry(8277);
		spells[4].reqlvl = 5;
		spells[4].targettype = TARGET_ATTACKING;
		spells[4].instant = false;
		spells[4].perctrigger = (float)RandomFloat(5.0f);
		spells[4].attackstoptimer = 1000; // 1sec
		m_spellcheck[4] = true;

		spells[5].info = dbcSpell.LookupEntry(403);
		spells[5].reqlvl = 1;
		spells[5].targettype = TARGET_ATTACKING;
		spells[5].instant = false;
		spells[5].perctrigger = (float)RandomFloat(5.0f);
		spells[5].attackstoptimer = 1000; // 1sec
		m_spellcheck[5] = true;

		spells[6].info = dbcSpell.LookupEntry(474);
		spells[6].reqlvl = 8;
		spells[6].targettype = TARGET_SELF;
		spells[6].instant = false;
		spells[6].perctrigger = (float)RandomFloat(5.0f);
		spells[6].attackstoptimer = 1000; // 1sec
		m_spellcheck[6] = true;

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

	      			if((spells[i].reqlvl<=_unit->GetUInt32Value(UNIT_FIELD_LEVEL)) && (spells[i].reqlvl+10>=_unit->GetUInt32Value(UNIT_FIELD_LEVEL)))
					{
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
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


class UnknownType /* Type=10 */ : public CreatureAIScript
	// 9515 (<-self) 11975 <-self) 12001 12038 (<-self) 17009
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(UnknownType);
	SP_AI_Spell spells[5];
	bool m_spellcheck[5];

    UnknownType(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// -- Number of spells to add --
		nrspells = 5;

		// --- Initialization ---
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		// ----------------------
		
        spells[0].info = dbcSpell.LookupEntry(9515);
		spells[0].reqlvl = 30;
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = (float)RandomFloat(2.0f);
		spells[0].attackstoptimer = 1000; // 1sec
		m_spellcheck[0] = true;

		spells[1].info = dbcSpell.LookupEntry(11975);
		spells[1].reqlvl = 20;
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = false;
		spells[1].perctrigger = (float)RandomFloat(5.0f);
		spells[1].attackstoptimer = 1000; // 1sec
		m_spellcheck[1] = true;

		spells[2].info = dbcSpell.LookupEntry(12001);
		spells[2].reqlvl = 17;
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = false;
		spells[2].perctrigger = (float)RandomFloat(5.0f);
		spells[2].attackstoptimer = 1000; // 1sec
		m_spellcheck[2] = true;

		spells[3].info = dbcSpell.LookupEntry(12038); // this shity spell doesnt exist?:PPP
		spells[3].reqlvl = 0;
		spells[3].targettype = TARGET_ATTACKING;
		spells[3].instant = false;
		spells[3].perctrigger = (float)RandomFloat(5.0f);
		spells[3].attackstoptimer = 1000; // 1sec
		m_spellcheck[3] = true;

		spells[4].info = dbcSpell.LookupEntry(17009); //VOODOOOOOO :)
		spells[4].reqlvl = 0; // Feel the 1337 power
		spells[4].targettype = TARGET_ATTACKING;
		spells[4].instant = false;
		spells[4].perctrigger = (float)RandomFloat(5.0f);
		spells[4].attackstoptimer = 1000; // 1sec
		m_spellcheck[4] = true;

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

	      			if((spells[i].reqlvl<=_unit->GetUInt32Value(UNIT_FIELD_LEVEL)) && (spells[i].reqlvl+10>=_unit->GetUInt32Value(UNIT_FIELD_LEVEL)))
					{
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
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
/* Humanoids AI */
class HumanoidHunter : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(HumanoidHunter);
	SP_AI_Spell spells[44];
 bool m_spellcheck[44];

    HumanoidHunter(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 44;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		     spells[0].info = dbcSpell.LookupEntry(1978);
		     spells[0].reqlvl = 4;
		     spells[0].targettype = TARGET_ATTACKING;
		     spells[0].instant = false;
		     spells[0].perctrigger = (float)RandomFloat(5.0f);
		     spells[0].attackstoptimer = 1000;
		     m_spellcheck[0] = true;


		     spells[1].info = dbcSpell.LookupEntry(13549);
		     spells[1].reqlvl = 10;
		     spells[1].targettype = TARGET_ATTACKING;
		     spells[1].instant = false;
		     spells[1].perctrigger = (float)RandomFloat(5.0f);
		     spells[1].attackstoptimer = 1000;
		     m_spellcheck[1] = true;


		     spells[2].info = dbcSpell.LookupEntry(13550);
		     spells[2].reqlvl = 18;
		     spells[2].targettype = TARGET_ATTACKING;
		     spells[2].instant = false;
		     spells[2].perctrigger = (float)RandomFloat(5.0f);
		     spells[2].attackstoptimer = 1000;
		     m_spellcheck[2] = true;


		     spells[3].info = dbcSpell.LookupEntry(3551);
		     spells[3].reqlvl = 20;
		     spells[3].targettype = TARGET_ATTACKING;
		     spells[3].instant = false;
		     spells[3].perctrigger = (float)RandomFloat(5.0f);
		     spells[3].attackstoptimer = 1000;
		     m_spellcheck[3] = true;


		     spells[4].info = dbcSpell.LookupEntry(13552);
		     spells[4].reqlvl = 34;
		     spells[4].targettype = TARGET_ATTACKING;
		     spells[4].instant = false;
		     spells[4].perctrigger = (float)RandomFloat(5.0f);
		     spells[4].attackstoptimer = 1000;
		     m_spellcheck[4] = true;


		     spells[5].info = dbcSpell.LookupEntry(13553);
		     spells[5].reqlvl = 42;
		     spells[5].targettype = TARGET_ATTACKING;
		     spells[5].instant = false;
		     spells[5].perctrigger = (float)RandomFloat(5.0f);
		     spells[5].attackstoptimer = 1000;
		     m_spellcheck[5] = true;


		     spells[6].info = dbcSpell.LookupEntry(13555);
		     spells[6].reqlvl = 58;
		     spells[6].targettype = TARGET_ATTACKING;
		     spells[6].instant = false;
		     spells[6].perctrigger = (float)RandomFloat(5.0f);
		     spells[6].attackstoptimer = 1000;
		     m_spellcheck[6] = true;


		     spells[7].info = dbcSpell.LookupEntry(3044);
		     spells[7].reqlvl = 6;
		     spells[7].targettype = TARGET_ATTACKING;
		     spells[7].instant = false;
		     spells[7].perctrigger = (float)RandomFloat(5.0f);
		     spells[7].attackstoptimer = 1000;
		     m_spellcheck[7] = true;


		     spells[8].info = dbcSpell.LookupEntry(14281);
		     spells[8].reqlvl = 12;
		     spells[8].targettype = TARGET_ATTACKING;
		     spells[8].instant = false;
		     spells[8].perctrigger = (float)RandomFloat(5.0f);
		     spells[8].attackstoptimer = 1000;
		     m_spellcheck[8] = true;


		     spells[9].info = dbcSpell.LookupEntry(14282);
		     spells[9].reqlvl = 20;
		     spells[9].targettype = TARGET_ATTACKING;
		     spells[9].instant = false;
		     spells[9].perctrigger = (float)RandomFloat(5.0f);
		     spells[9].attackstoptimer = 1000;
		     m_spellcheck[9] = true;


		     spells[10].info = dbcSpell.LookupEntry(14283);
		     spells[10].reqlvl = 28;
		     spells[10].targettype = TARGET_ATTACKING;
		     spells[10].instant = false;
		     spells[10].perctrigger = (float)RandomFloat(5.0f);
		     spells[10].attackstoptimer = 1000;
		     m_spellcheck[10] = true;


		     spells[11].info = dbcSpell.LookupEntry(14284);
		     spells[11].reqlvl = 36;
		     spells[11].targettype = TARGET_ATTACKING;
		     spells[11].instant = false;
		     spells[11].perctrigger = (float)RandomFloat(5.0f);
		     spells[11].attackstoptimer = 1000;
		     m_spellcheck[11] = true;


		     spells[12].info = dbcSpell.LookupEntry(13554);
		     spells[12].reqlvl = 50;
		     spells[12].targettype = TARGET_ATTACKING;
		     spells[12].instant = false;
		     spells[12].perctrigger = (float)RandomFloat(5.0f);
		     spells[12].attackstoptimer = 1000;
		     m_spellcheck[12] = true;


		     spells[13].info = dbcSpell.LookupEntry(14287);
		     spells[13].reqlvl = 60;
		     spells[13].targettype = TARGET_ATTACKING;
		     spells[13].instant = false;
		     spells[13].perctrigger = (float)RandomFloat(5.0f);
		     spells[13].attackstoptimer = 1000;
		     m_spellcheck[13] = true;


		     spells[14].info = dbcSpell.LookupEntry(20736);
		     spells[14].reqlvl = 12;
		     spells[14].targettype = TARGET_ATTACKING;
		     spells[14].instant = false;
		     spells[14].perctrigger = (float)RandomFloat(5.0f);
		     spells[14].attackstoptimer = 1000;
		     m_spellcheck[14] = true;


		     spells[15].info = dbcSpell.LookupEntry(14274);
		     spells[15].reqlvl = 20;
		     spells[15].targettype = TARGET_ATTACKING;
		     spells[15].instant = false;
		     spells[15].perctrigger = (float)RandomFloat(5.0f);
		     spells[15].attackstoptimer = 1000;
		     m_spellcheck[15] = true;


		     spells[16].info = dbcSpell.LookupEntry(15629);
		     spells[16].reqlvl = 30;
		     spells[16].targettype = TARGET_ATTACKING;
		     spells[16].instant = false;
		     spells[16].perctrigger = (float)RandomFloat(5.0f);
		     spells[16].attackstoptimer = 1000;
		     m_spellcheck[16] = true;


		     spells[17].info = dbcSpell.LookupEntry(15630);
		     spells[17].reqlvl = 40;
		     spells[17].targettype = TARGET_ATTACKING;
		     spells[17].instant = false;
		     spells[17].perctrigger = (float)RandomFloat(5.0f);
		     spells[17].attackstoptimer = 1000;
		     m_spellcheck[17] = true;


		     spells[18].info = dbcSpell.LookupEntry(14285);
		     spells[18].reqlvl = 44;
		     spells[18].targettype = TARGET_ATTACKING;
		     spells[18].instant = false;
		     spells[18].perctrigger = (float)RandomFloat(5.0f);
		     spells[18].attackstoptimer = 1000;
		     m_spellcheck[18] = true;


		     spells[19].info = dbcSpell.LookupEntry(15632);
		     spells[19].reqlvl = 60;
		     spells[19].targettype = TARGET_ATTACKING;
		     spells[19].instant = false;
		     spells[19].perctrigger = (float)RandomFloat(5.0f);
		     spells[19].attackstoptimer = 1000;
		     m_spellcheck[19] = true;


		     spells[20].info = dbcSpell.LookupEntry(5116);
		     spells[20].reqlvl = 8;
		     spells[20].targettype = TARGET_ATTACKING;
		     spells[20].instant = false;
		     spells[20].perctrigger = (float)RandomFloat(5.0f);
		     spells[20].attackstoptimer = 1000;
		     m_spellcheck[20] = true;


		     spells[21].info = dbcSpell.LookupEntry(14286);
		     spells[21].reqlvl = 52;
		     spells[21].targettype = TARGET_ATTACKING;
		     spells[21].instant = false;
		     spells[21].perctrigger = (float)RandomFloat(5.0f);
		     spells[21].attackstoptimer = 1000;
		     m_spellcheck[21] = true;


		     spells[22].info = dbcSpell.LookupEntry(19434);
		     spells[22].reqlvl = 20;
		     spells[22].targettype = TARGET_ATTACKING;
		     spells[22].instant = false;
		     spells[22].perctrigger = (float)RandomFloat(5.0f);
		     spells[22].attackstoptimer = 1000;
		     m_spellcheck[22] = true;


		     spells[23].info = dbcSpell.LookupEntry(20900);
		     spells[23].reqlvl = 28;
		     spells[23].targettype = TARGET_ATTACKING;
		     spells[23].instant = false;
		     spells[23].perctrigger = (float)RandomFloat(5.0f);
		     spells[23].attackstoptimer = 1000;
		     m_spellcheck[23] = true;


		     spells[24].info = dbcSpell.LookupEntry(20901);
		     spells[24].reqlvl = 36;
		     spells[24].targettype = TARGET_ATTACKING;
		     spells[24].instant = false;
		     spells[24].perctrigger = (float)RandomFloat(5.0f);
		     spells[24].attackstoptimer = 1000;
		     m_spellcheck[24] = true;


		     spells[25].info = dbcSpell.LookupEntry(15631);
		     spells[25].reqlvl = 50;
		     spells[25].targettype = TARGET_ATTACKING;
		     spells[25].instant = false;
		     spells[25].perctrigger = (float)RandomFloat(5.0f);
		     spells[25].attackstoptimer = 1000;
		     m_spellcheck[25] = true;


		     spells[26].info = dbcSpell.LookupEntry(20903);
		     spells[26].reqlvl = 52;
		     spells[26].targettype = TARGET_ATTACKING;
		     spells[26].instant = false;
		     spells[26].perctrigger = (float)RandomFloat(5.0f);
		     spells[26].attackstoptimer = 1000;
		     m_spellcheck[26] = true;


		     spells[27].info = dbcSpell.LookupEntry(2643);
		     spells[27].reqlvl = 18;
		     spells[27].targettype = TARGET_ATTACKING;
		     spells[27].instant = false;
		     spells[27].perctrigger = (float)RandomFloat(5.0f);
		     spells[27].attackstoptimer = 1000;
		     m_spellcheck[27] = true;


		     spells[28].info = dbcSpell.LookupEntry(14288);
		     spells[28].reqlvl = 30;
		     spells[28].targettype = TARGET_ATTACKING;
		     spells[28].instant = false;
		     spells[28].perctrigger = (float)RandomFloat(5.0f);
		     spells[28].attackstoptimer = 1000;
		     m_spellcheck[28] = true;


		     spells[29].info = dbcSpell.LookupEntry(5116);
		     spells[29].reqlvl = 8;
		     spells[29].targettype = TARGET_ATTACKING;
		     spells[29].instant = false;
		     spells[29].perctrigger = (float)RandomFloat(5.0f);
		     spells[29].attackstoptimer = 1000;
		     m_spellcheck[29] = true;


		     spells[30].info = dbcSpell.LookupEntry(20904);
		     spells[30].reqlvl = 60;
		     spells[30].targettype = TARGET_ATTACKING;
		     spells[30].instant = false;
		     spells[30].perctrigger = (float)RandomFloat(5.0f);
		     spells[30].attackstoptimer = 1000;
		     m_spellcheck[30] = true;


		     spells[31].info = dbcSpell.LookupEntry(3043);
		     spells[31].reqlvl = 22;
		     spells[31].targettype = TARGET_ATTACKING;
		     spells[31].instant = false;
		     spells[31].perctrigger = (float)RandomFloat(5.0f);
		     spells[31].attackstoptimer = 1000;
		     m_spellcheck[31] = true;


		     spells[32].info = dbcSpell.LookupEntry(14275);
		     spells[32].reqlvl = 32;
		     spells[32].targettype = TARGET_ATTACKING;
		     spells[32].instant = false;
		     spells[32].perctrigger = (float)RandomFloat(5.0f);
		     spells[32].attackstoptimer = 1000;
		     m_spellcheck[32] = true;


		     spells[33].info = dbcSpell.LookupEntry(20902);
		     spells[33].reqlvl = 44;
		     spells[33].targettype = TARGET_ATTACKING;
		     spells[33].instant = false;
		     spells[33].perctrigger = (float)RandomFloat(5.0f);
		     spells[33].attackstoptimer = 1000;
		     m_spellcheck[33] = true;


		     spells[34].info = dbcSpell.LookupEntry(14290);
		     spells[34].reqlvl = 54;
		     spells[34].targettype = TARGET_ATTACKING;
		     spells[34].instant = false;
		     spells[34].perctrigger = (float)RandomFloat(5.0f);
		     spells[34].attackstoptimer = 1000;
		     m_spellcheck[34] = true;


		     spells[35].info = dbcSpell.LookupEntry(3034);
		     spells[35].reqlvl = 36;
		     spells[35].targettype = TARGET_ATTACKING;
		     spells[35].instant = false;
		     spells[35].perctrigger = (float)RandomFloat(5.0f);
		     spells[35].attackstoptimer = 1000;
		     m_spellcheck[35] = true;


		     spells[36].info = dbcSpell.LookupEntry(14289);
		     spells[36].reqlvl = 42;
		     spells[36].targettype = TARGET_ATTACKING;
		     spells[36].instant = false;
		     spells[36].perctrigger = (float)RandomFloat(5.0f);
		     spells[36].attackstoptimer = 1000;
		     m_spellcheck[36] = true;


		     spells[37].info = dbcSpell.LookupEntry(14277);
		     spells[37].reqlvl = 52;
		     spells[37].targettype = TARGET_ATTACKING;
		     spells[37].instant = false;
		     spells[37].perctrigger = (float)RandomFloat(5.0f);
		     spells[37].attackstoptimer = 1000;
		     m_spellcheck[37] = true;


		     spells[38].info = dbcSpell.LookupEntry(1510);
		     spells[38].reqlvl = 40;
		     spells[38].targettype = TARGET_ATTACKING;
		     spells[38].instant = false;
		     spells[38].perctrigger = (float)RandomFloat(5.0f);
		     spells[38].attackstoptimer = 1000;
		     m_spellcheck[38] = true;


		     spells[39].info = dbcSpell.LookupEntry(14276);
		     spells[39].reqlvl = 42;
		     spells[39].targettype = TARGET_ATTACKING;
		     spells[39].instant = false;
		     spells[39].perctrigger = (float)RandomFloat(5.0f);
		     spells[39].attackstoptimer = 1000;
		     m_spellcheck[39] = true;


		     spells[40].info = dbcSpell.LookupEntry(14280);
		     spells[40].reqlvl = 56;
		     spells[40].targettype = TARGET_ATTACKING;
		     spells[40].instant = false;
		     spells[40].perctrigger = (float)RandomFloat(5.0f);
		     spells[40].attackstoptimer = 1000;
		     m_spellcheck[40] = true;


		     spells[41].info = dbcSpell.LookupEntry(14279);
		     spells[41].reqlvl = 46;
		     spells[41].targettype = TARGET_ATTACKING;
		     spells[41].instant = false;
		     spells[41].perctrigger = (float)RandomFloat(5.0f);
		     spells[41].attackstoptimer = 1000;
		     m_spellcheck[41] = true;


		     spells[42].info = dbcSpell.LookupEntry(14295);
		     spells[42].reqlvl = 58;
		     spells[42].targettype = TARGET_ATTACKING;
		     spells[42].instant = false;
		     spells[42].perctrigger = (float)RandomFloat(5.0f);
		     spells[42].attackstoptimer = 1000;
		     m_spellcheck[42] = true;


		     spells[43].info = dbcSpell.LookupEntry(14294);
		     spells[43].reqlvl = 50;
		     spells[43].targettype = TARGET_ATTACKING;
		     spells[43].instant = false;
		     spells[43].perctrigger = (float)RandomFloat(5.0f);
		     spells[43].attackstoptimer = 1000;
		     m_spellcheck[43] = true;

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
	      			if ((spells[i].reqlvl<=_unit->GetUInt32Value(UNIT_FIELD_LEVEL)) && (spells[i].reqlvl+10>=_unit->GetUInt32Value(UNIT_FIELD_LEVEL)))
		      	  {
			      	if(m_spellcheck[i])
		      		{
				        target = _unit->GetAIInterface()->GetNextTarget();
			       	  switch(spells[i].targettype)
			    		  {
			    		    case TARGET_SELF:
				         			_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
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

class HumanoidShadowHunter : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(HumanoidShadowHunter);
	SP_AI_Spell spells[28];
 bool m_spellcheck[28];

    HumanoidShadowHunter(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 28;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		     spells[0].info = dbcSpell.LookupEntry(589);
		     spells[0].reqlvl = 4;
		     spells[0].targettype = TARGET_ATTACKING;
		     spells[0].instant = false;
		     spells[0].perctrigger = (float)RandomFloat(5.0f);
		     spells[0].attackstoptimer = 1000;
		     m_spellcheck[0] = true;


		     spells[1].info = dbcSpell.LookupEntry(594);
		     spells[1].reqlvl = 10;
		     spells[1].targettype = TARGET_ATTACKING;
		     spells[1].instant = false;
		     spells[1].perctrigger = (float)RandomFloat(5.0f);
		     spells[1].attackstoptimer = 1000;
		     m_spellcheck[1] = true;


		     spells[2].info = dbcSpell.LookupEntry(970);
		     spells[2].reqlvl = 18;
		     spells[2].targettype = TARGET_ATTACKING;
		     spells[2].instant = false;
		     spells[2].perctrigger = (float)RandomFloat(5.0f);
		     spells[2].attackstoptimer = 1000;
		     m_spellcheck[2] = true;


		     spells[3].info = dbcSpell.LookupEntry(992);
		     spells[3].reqlvl = 26;
		     spells[3].targettype = TARGET_ATTACKING;
		     spells[3].instant = false;
		     spells[3].perctrigger = (float)RandomFloat(5.0f);
		     spells[3].attackstoptimer = 1000;
		     m_spellcheck[3] = true;


		     spells[4].info = dbcSpell.LookupEntry(2767);
		     spells[4].reqlvl = 34;
		     spells[4].targettype = TARGET_ATTACKING;
		     spells[4].instant = false;
		     spells[4].perctrigger = (float)RandomFloat(5.0f);
		     spells[4].attackstoptimer = 1000;
		     m_spellcheck[4] = true;


		     spells[5].info = dbcSpell.LookupEntry(10892);
		     spells[5].reqlvl = 42;
		     spells[5].targettype = TARGET_ATTACKING;
		     spells[5].instant = false;
		     spells[5].perctrigger = (float)RandomFloat(5.0f);
		     spells[5].attackstoptimer = 1000;
		     m_spellcheck[5] = true;


		     spells[6].info = dbcSpell.LookupEntry(10893);
		     spells[6].reqlvl = 50;
		     spells[6].targettype = TARGET_ATTACKING;
		     spells[6].instant = false;
		     spells[6].perctrigger = (float)RandomFloat(5.0f);
		     spells[6].attackstoptimer = 1000;
		     m_spellcheck[6] = true;


		     spells[7].info = dbcSpell.LookupEntry(695);
		     spells[7].reqlvl = 6;
		     spells[7].targettype = TARGET_ATTACKING;
		     spells[7].instant = false;
		     spells[7].perctrigger = (float)RandomFloat(5.0f);
		     spells[7].attackstoptimer = 1000;
		     m_spellcheck[7] = true;


		     spells[8].info = dbcSpell.LookupEntry(705);
		     spells[8].reqlvl = 12;
		     spells[8].targettype = TARGET_ATTACKING;
		     spells[8].instant = false;
		     spells[8].perctrigger = (float)RandomFloat(5.0f);
		     spells[8].attackstoptimer = 1000;
		     m_spellcheck[8] = true;


		     spells[9].info = dbcSpell.LookupEntry(1106);
		     spells[9].reqlvl = 28;
		     spells[9].targettype = TARGET_ATTACKING;
		     spells[9].instant = false;
		     spells[9].perctrigger = (float)RandomFloat(5.0f);
		     spells[9].attackstoptimer = 1000;
		     m_spellcheck[9] = true;


		     spells[10].info = dbcSpell.LookupEntry(7641);
		     spells[10].reqlvl = 36;
		     spells[10].targettype = TARGET_ATTACKING;
		     spells[10].instant = false;
		     spells[10].perctrigger = (float)RandomFloat(5.0f);
		     spells[10].attackstoptimer = 1000;
		     m_spellcheck[10] = true;


		     spells[11].info = dbcSpell.LookupEntry(11659);
		     spells[11].reqlvl = 44;
		     spells[11].targettype = TARGET_ATTACKING;
		     spells[11].instant = false;
		     spells[11].perctrigger = (float)RandomFloat(5.0f);
		     spells[11].attackstoptimer = 1000;
		     m_spellcheck[11] = true;


		     spells[12].info = dbcSpell.LookupEntry(11660);
		     spells[12].reqlvl = 52;
		     spells[12].targettype = TARGET_ATTACKING;
		     spells[12].instant = false;
		     spells[12].perctrigger = (float)RandomFloat(5.0f);
		     spells[12].attackstoptimer = 1000;
		     m_spellcheck[12] = true;


		     spells[13].info = dbcSpell.LookupEntry(11661);
		     spells[13].reqlvl = 60;
		     spells[13].targettype = TARGET_ATTACKING;
		     spells[13].instant = false;
		     spells[13].perctrigger = (float)RandomFloat(5.0f);
		     spells[13].attackstoptimer = 1000;
		     m_spellcheck[13] = true;


		     spells[14].info = dbcSpell.LookupEntry(1978);
		     spells[14].reqlvl = 4;
		     spells[14].targettype = TARGET_ATTACKING;
		     spells[14].instant = false;
		     spells[14].perctrigger = (float)RandomFloat(5.0f);
		     spells[14].attackstoptimer = 1000;
		     m_spellcheck[14] = true;


		     spells[15].info = dbcSpell.LookupEntry(13549);
		     spells[15].reqlvl = 10;
		     spells[15].targettype = TARGET_ATTACKING;
		     spells[15].instant = false;
		     spells[15].perctrigger = (float)RandomFloat(5.0f);
		     spells[15].attackstoptimer = 1000;
		     m_spellcheck[15] = true;


		     spells[16].info = dbcSpell.LookupEntry(13550);
		     spells[16].reqlvl = 18;
		     spells[16].targettype = TARGET_ATTACKING;
		     spells[16].instant = false;
		     spells[16].perctrigger = (float)RandomFloat(5.0f);
		     spells[16].attackstoptimer = 1000;
		     m_spellcheck[16] = true;


		     spells[17].info = dbcSpell.LookupEntry(3551);
		     spells[17].reqlvl = 20;
		     spells[17].targettype = TARGET_ATTACKING;
		     spells[17].instant = false;
		     spells[17].perctrigger = (float)RandomFloat(5.0f);
		     spells[17].attackstoptimer = 1000;
		     m_spellcheck[17] = true;


		     spells[18].info = dbcSpell.LookupEntry(13552);
		     spells[18].reqlvl = 34;
		     spells[18].targettype = TARGET_ATTACKING;
		     spells[18].instant = false;
		     spells[18].perctrigger = (float)RandomFloat(5.0f);
		     spells[18].attackstoptimer = 1000;
		     m_spellcheck[18] = true;


		     spells[19].info = dbcSpell.LookupEntry(13554);
		     spells[19].reqlvl = 50;
		     spells[19].targettype = TARGET_ATTACKING;
		     spells[19].instant = false;
		     spells[19].perctrigger = (float)RandomFloat(5.0f);
		     spells[19].attackstoptimer = 1000;
		     m_spellcheck[19] = true;


		     spells[20].info = dbcSpell.LookupEntry(13555);
		     spells[20].reqlvl = 58;
		     spells[20].targettype = TARGET_ATTACKING;
		     spells[20].instant = false;
		     spells[20].perctrigger = (float)RandomFloat(5.0f);
		     spells[20].attackstoptimer = 1000;
		     m_spellcheck[20] = true;


		     spells[21].info = dbcSpell.LookupEntry(3043);
		     spells[21].reqlvl = 22;
		     spells[21].targettype = TARGET_ATTACKING;
		     spells[21].instant = false;
		     spells[21].perctrigger = (float)RandomFloat(5.0f);
		     spells[21].attackstoptimer = 1000;
		     m_spellcheck[21] = true;


		     spells[22].info = dbcSpell.LookupEntry(14275);
		     spells[22].reqlvl = 32;
		     spells[22].targettype = TARGET_ATTACKING;
		     spells[22].instant = false;
		     spells[22].perctrigger = (float)RandomFloat(5.0f);
		     spells[22].attackstoptimer = 1000;
		     m_spellcheck[22] = true;


		     spells[23].info = dbcSpell.LookupEntry(14276);
		     spells[23].reqlvl = 42;
		     spells[23].targettype = TARGET_ATTACKING;
		     spells[23].instant = false;
		     spells[23].perctrigger = (float)RandomFloat(5.0f);
		     spells[23].attackstoptimer = 1000;
		     m_spellcheck[23] = true;


		     spells[24].info = dbcSpell.LookupEntry(14277);
		     spells[24].reqlvl = 52;
		     spells[24].targettype = TARGET_ATTACKING;
		     spells[24].instant = false;
		     spells[24].perctrigger = (float)RandomFloat(5.0f);
		     spells[24].attackstoptimer = 1000;
		     m_spellcheck[24] = true;


		     spells[25].info = dbcSpell.LookupEntry(3034);
		     spells[25].reqlvl = 36;
		     spells[25].targettype = TARGET_ATTACKING;
		     spells[25].instant = false;
		     spells[25].perctrigger = (float)RandomFloat(5.0f);
		     spells[25].attackstoptimer = 1000;
		     m_spellcheck[25] = true;


		     spells[26].info = dbcSpell.LookupEntry(14279);
		     spells[26].reqlvl = 46;
		     spells[26].targettype = TARGET_ATTACKING;
		     spells[26].instant = false;
		     spells[26].perctrigger = (float)RandomFloat(5.0f);
		     spells[26].attackstoptimer = 1000;
		     m_spellcheck[26] = true;


		     spells[27].info = dbcSpell.LookupEntry(14280);
		     spells[27].reqlvl = 56;
		     spells[27].targettype = TARGET_ATTACKING;
		     spells[27].instant = false;
		     spells[27].perctrigger = (float)RandomFloat(5.0f);
		     spells[27].attackstoptimer = 1000;
		     m_spellcheck[27] = true;

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
	      			if ((spells[i].reqlvl<=_unit->GetUInt32Value(UNIT_FIELD_LEVEL)) && (spells[i].reqlvl+10>=_unit->GetUInt32Value(UNIT_FIELD_LEVEL)))
		      	  {
			      	if(m_spellcheck[i])
		      		{
				        target = _unit->GetAIInterface()->GetNextTarget();
			       	  switch(spells[i].targettype)
			    		  {
			    		    case TARGET_SELF:
				         			_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
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
class HumanoidMage : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(HumanoidMage);
	SP_AI_Spell spells[57];
 bool m_spellcheck[57];

    HumanoidMage(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 57;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		     spells[0].info = dbcSpell.LookupEntry(133);
		     spells[0].reqlvl = 1;
		     spells[0].targettype = TARGET_ATTACKING;
		     spells[0].instant = false;
		     spells[0].perctrigger = (float)RandomFloat(5.0f);
		     spells[0].attackstoptimer = 1000;
		     m_spellcheck[0] = true;


		     spells[1].info = dbcSpell.LookupEntry(143);
		     spells[1].reqlvl = 6;
		     spells[1].targettype = TARGET_ATTACKING;
		     spells[1].instant = false;
		     spells[1].perctrigger = (float)RandomFloat(5.0f);
		     spells[1].attackstoptimer = 1000;
		     m_spellcheck[1] = true;


		     spells[2].info = dbcSpell.LookupEntry(3140);
		     spells[2].reqlvl = 18;
		     spells[2].targettype = TARGET_ATTACKING;
		     spells[2].instant = false;
		     spells[2].perctrigger = (float)RandomFloat(5.0f);
		     spells[2].attackstoptimer = 1000;
		     m_spellcheck[2] = true;


		     spells[3].info = dbcSpell.LookupEntry(8401);
		     spells[3].reqlvl = 30;
		     spells[3].targettype = TARGET_ATTACKING;
		     spells[3].instant = false;
		     spells[3].perctrigger = (float)RandomFloat(5.0f);
		     spells[3].attackstoptimer = 1000;
		     m_spellcheck[3] = true;


		     spells[4].info = dbcSpell.LookupEntry(10148);
		     spells[4].reqlvl = 42;
		     spells[4].targettype = TARGET_ATTACKING;
		     spells[4].instant = false;
		     spells[4].perctrigger = (float)RandomFloat(5.0f);
		     spells[4].attackstoptimer = 1000;
		     m_spellcheck[4] = true;


		     spells[5].info = dbcSpell.LookupEntry(10149);
		     spells[5].reqlvl = 48;
		     spells[5].targettype = TARGET_ATTACKING;
		     spells[5].instant = false;
		     spells[5].perctrigger = (float)RandomFloat(5.0f);
		     spells[5].attackstoptimer = 1000;
		     m_spellcheck[5] = true;


		     spells[6].info = dbcSpell.LookupEntry(10151);
		     spells[6].reqlvl = 60;
		     spells[6].targettype = TARGET_ATTACKING;
		     spells[6].instant = false;
		     spells[6].perctrigger = (float)RandomFloat(5.0f);
		     spells[6].attackstoptimer = 1000;
		     m_spellcheck[6] = true;


		     spells[7].info = dbcSpell.LookupEntry(116);
		     spells[7].reqlvl = 4;
		     spells[7].targettype = TARGET_ATTACKING;
		     spells[7].instant = false;
		     spells[7].perctrigger = (float)RandomFloat(5.0f);
		     spells[7].attackstoptimer = 1000;
		     m_spellcheck[7] = true;


		     spells[8].info = dbcSpell.LookupEntry(145);
		     spells[8].reqlvl = 12;
		     spells[8].targettype = TARGET_ATTACKING;
		     spells[8].instant = false;
		     spells[8].perctrigger = (float)RandomFloat(5.0f);
		     spells[8].attackstoptimer = 1000;
		     m_spellcheck[8] = true;


		     spells[9].info = dbcSpell.LookupEntry(8400);
		     spells[9].reqlvl = 24;
		     spells[9].targettype = TARGET_ATTACKING;
		     spells[9].instant = false;
		     spells[9].perctrigger = (float)RandomFloat(5.0f);
		     spells[9].attackstoptimer = 1000;
		     m_spellcheck[9] = true;


		     spells[10].info = dbcSpell.LookupEntry(7322);
		     spells[10].reqlvl = 20;
		     spells[10].targettype = TARGET_ATTACKING;
		     spells[10].instant = false;
		     spells[10].perctrigger = (float)RandomFloat(5.0f);
		     spells[10].attackstoptimer = 1000;
		     m_spellcheck[10] = true;


		     spells[11].info = dbcSpell.LookupEntry(8407);
		     spells[11].reqlvl = 32;
		     spells[11].targettype = TARGET_ATTACKING;
		     spells[11].instant = false;
		     spells[11].perctrigger = (float)RandomFloat(5.0f);
		     spells[11].attackstoptimer = 1000;
		     m_spellcheck[11] = true;


		     spells[12].info = dbcSpell.LookupEntry(10179);
		     spells[12].reqlvl = 44;
		     spells[12].targettype = TARGET_ATTACKING;
		     spells[12].instant = false;
		     spells[12].perctrigger = (float)RandomFloat(5.0f);
		     spells[12].attackstoptimer = 1000;
		     m_spellcheck[12] = true;


		     spells[13].info = dbcSpell.LookupEntry(10181);
		     spells[13].reqlvl = 56;
		     spells[13].targettype = TARGET_ATTACKING;
		     spells[13].instant = false;
		     spells[13].perctrigger = (float)RandomFloat(5.0f);
		     spells[13].attackstoptimer = 1000;
		     m_spellcheck[13] = true;


		     spells[14].info = dbcSpell.LookupEntry(2136);
		     spells[14].reqlvl = 6;
		     spells[14].targettype = TARGET_ATTACKING;
		     spells[14].instant = false;
		     spells[14].perctrigger = (float)RandomFloat(5.0f);
		     spells[14].attackstoptimer = 1000;
		     m_spellcheck[14] = true;


		     spells[15].info = dbcSpell.LookupEntry(2137);
		     spells[15].reqlvl = 14;
		     spells[15].targettype = TARGET_ATTACKING;
		     spells[15].instant = false;
		     spells[15].perctrigger = (float)RandomFloat(5.0f);
		     spells[15].attackstoptimer = 1000;
		     m_spellcheck[15] = true;


		     spells[16].info = dbcSpell.LookupEntry(8406);
		     spells[16].reqlvl = 26;
		     spells[16].targettype = TARGET_ATTACKING;
		     spells[16].instant = false;
		     spells[16].perctrigger = (float)RandomFloat(5.0f);
		     spells[16].attackstoptimer = 1000;
		     m_spellcheck[16] = true;


		     spells[17].info = dbcSpell.LookupEntry(8408);
		     spells[17].reqlvl = 38;
		     spells[17].targettype = TARGET_ATTACKING;
		     spells[17].instant = false;
		     spells[17].perctrigger = (float)RandomFloat(5.0f);
		     spells[17].attackstoptimer = 1000;
		     m_spellcheck[17] = true;


		     spells[18].info = dbcSpell.LookupEntry(10180);
		     spells[18].reqlvl = 50;
		     spells[18].targettype = TARGET_ATTACKING;
		     spells[18].instant = false;
		     spells[18].perctrigger = (float)RandomFloat(5.0f);
		     spells[18].attackstoptimer = 1000;
		     m_spellcheck[18] = true;


		     spells[19].info = dbcSpell.LookupEntry(10199);
		     spells[19].reqlvl = 54;
		     spells[19].targettype = TARGET_ATTACKING;
		     spells[19].instant = false;
		     spells[19].perctrigger = (float)RandomFloat(5.0f);
		     spells[19].attackstoptimer = 1000;
		     m_spellcheck[19] = true;


		     spells[20].info = dbcSpell.LookupEntry(205);
		     spells[20].reqlvl = 8;
		     spells[20].targettype = TARGET_ATTACKING;
		     spells[20].instant = false;
		     spells[20].perctrigger = (float)RandomFloat(5.0f);
		     spells[20].attackstoptimer = 1000;
		     m_spellcheck[20] = true;


		     spells[21].info = dbcSpell.LookupEntry(837);
		     spells[21].reqlvl = 14;
		     spells[21].targettype = TARGET_ATTACKING;
		     spells[21].instant = false;
		     spells[21].perctrigger = (float)RandomFloat(5.0f);
		     spells[21].attackstoptimer = 1000;
		     m_spellcheck[21] = true;


		     spells[22].info = dbcSpell.LookupEntry(2138);
		     spells[22].reqlvl = 22;
		     spells[22].targettype = TARGET_ATTACKING;
		     spells[22].instant = false;
		     spells[22].perctrigger = (float)RandomFloat(5.0f);
		     spells[22].attackstoptimer = 1000;
		     m_spellcheck[22] = true;


		     spells[23].info = dbcSpell.LookupEntry(8413);
		     spells[23].reqlvl = 38;
		     spells[23].targettype = TARGET_ATTACKING;
		     spells[23].instant = false;
		     spells[23].perctrigger = (float)RandomFloat(5.0f);
		     spells[23].attackstoptimer = 1000;
		     m_spellcheck[23] = true;


		     spells[24].info = dbcSpell.LookupEntry(10197);
		     spells[24].reqlvl = 46;
		     spells[24].targettype = TARGET_ATTACKING;
		     spells[24].instant = false;
		     spells[24].perctrigger = (float)RandomFloat(5.0f);
		     spells[24].attackstoptimer = 1000;
		     m_spellcheck[24] = true;


		     spells[25].info = dbcSpell.LookupEntry(10230);
		     spells[25].reqlvl = 54;
		     spells[25].targettype = TARGET_ATTACKING;
		     spells[25].instant = false;
		     spells[25].perctrigger = (float)RandomFloat(5.0f);
		     spells[25].attackstoptimer = 1000;
		     m_spellcheck[25] = true;


		     spells[26].info = dbcSpell.LookupEntry(122);
		     spells[26].reqlvl = 10;
		     spells[26].targettype = TARGET_ATTACKING;
		     spells[26].instant = false;
		     spells[26].perctrigger = (float)RandomFloat(5.0f);
		     spells[26].attackstoptimer = 1000;
		     m_spellcheck[26] = true;


		     spells[27].info = dbcSpell.LookupEntry(8412);
		     spells[27].reqlvl = 30;
		     spells[27].targettype = TARGET_ATTACKING;
		     spells[27].instant = false;
		     spells[27].perctrigger = (float)RandomFloat(5.0f);
		     spells[27].attackstoptimer = 1000;
		     m_spellcheck[27] = true;


		     spells[28].info = dbcSpell.LookupEntry(6131);
		     spells[28].reqlvl = 40;
		     spells[28].targettype = TARGET_ATTACKING;
		     spells[28].instant = false;
		     spells[28].perctrigger = (float)RandomFloat(5.0f);
		     spells[28].attackstoptimer = 1000;
		     m_spellcheck[28] = true;



		     spells[29].info = dbcSpell.LookupEntry(10161);
		     spells[29].reqlvl = 58;
		     spells[29].targettype = TARGET_ATTACKING;
		     spells[29].instant = false;
		     spells[29].perctrigger = (float)RandomFloat(5.0f);
		     spells[29].attackstoptimer = 1000;
		     m_spellcheck[29] = true;


		     spells[30].info = dbcSpell.LookupEntry(10);
		     spells[30].reqlvl = 20;
		     spells[30].targettype = TARGET_ATTACKING;
		     spells[30].instant = false;
		     spells[30].perctrigger = (float)RandomFloat(5.0f);
		     spells[30].attackstoptimer = 1000;
		     m_spellcheck[30] = true;


		     spells[31].info = dbcSpell.LookupEntry(120);
		     spells[31].reqlvl = 26;
		     spells[31].targettype = TARGET_ATTACKING;
		     spells[31].instant = false;
		     spells[31].perctrigger = (float)RandomFloat(5.0f);
		     spells[31].attackstoptimer = 1000;
		     m_spellcheck[31] = true;


		     spells[32].info = dbcSpell.LookupEntry(865);
		     spells[32].reqlvl = 26;
		     spells[32].targettype = TARGET_ATTACKING;
		     spells[32].instant = false;
		     spells[32].perctrigger = (float)RandomFloat(5.0f);
		     spells[32].attackstoptimer = 1000;
		     m_spellcheck[32] = true;


		     spells[33].info = dbcSpell.LookupEntry(10159);
		     spells[33].reqlvl = 42;
		     spells[33].targettype = TARGET_ATTACKING;
		     spells[33].instant = false;
		     spells[33].perctrigger = (float)RandomFloat(5.0f);
		     spells[33].attackstoptimer = 1000;
		     m_spellcheck[33] = true;


		     spells[34].info = dbcSpell.LookupEntry(10160);
		     spells[34].reqlvl = 50;
		     spells[34].targettype = TARGET_ATTACKING;
		     spells[34].instant = false;
		     spells[34].perctrigger = (float)RandomFloat(5.0f);
		     spells[34].attackstoptimer = 1000;
		     m_spellcheck[34] = true;


		     spells[35].info = dbcSpell.LookupEntry(13020);
		     spells[35].reqlvl = 52;
		     spells[35].targettype = TARGET_ATTACKING;
		     spells[35].instant = false;
		     spells[35].perctrigger = (float)RandomFloat(5.0f);
		     spells[35].attackstoptimer = 1000;
		     m_spellcheck[35] = true;


		     spells[36].info = dbcSpell.LookupEntry(8437);
		     spells[36].reqlvl = 22;
		     spells[36].targettype = TARGET_ATTACKING;
		     spells[36].instant = false;
		     spells[36].perctrigger = (float)RandomFloat(5.0f);
		     spells[36].attackstoptimer = 1000;
		     m_spellcheck[36] = true;


		     spells[37].info = dbcSpell.LookupEntry(8427);
		     spells[37].reqlvl = 36;
		     spells[37].targettype = TARGET_ATTACKING;
		     spells[37].instant = false;
		     spells[37].perctrigger = (float)RandomFloat(5.0f);
		     spells[37].attackstoptimer = 1000;
		     m_spellcheck[37] = true;


		     spells[38].info = dbcSpell.LookupEntry(10185);
		     spells[38].reqlvl = 44;
		     spells[38].targettype = TARGET_ATTACKING;
		     spells[38].instant = false;
		     spells[38].perctrigger = (float)RandomFloat(5.0f);
		     spells[38].attackstoptimer = 1000;
		     m_spellcheck[38] = true;


		     spells[39].info = dbcSpell.LookupEntry(10186);
		     spells[39].reqlvl = 52;
		     spells[39].targettype = TARGET_ATTACKING;
		     spells[39].instant = false;
		     spells[39].perctrigger = (float)RandomFloat(5.0f);
		     spells[39].attackstoptimer = 1000;
		     m_spellcheck[39] = true;


		     spells[40].info = dbcSpell.LookupEntry(10202);
		     spells[40].reqlvl = 54;
		     spells[40].targettype = TARGET_ATTACKING;
		     spells[40].instant = false;
		     spells[40].perctrigger = (float)RandomFloat(5.0f);
		     spells[40].attackstoptimer = 1000;
		     m_spellcheck[40] = true;


		     spells[41].info = dbcSpell.LookupEntry(8438);
		     spells[41].reqlvl = 30;
		     spells[41].targettype = TARGET_ATTACKING;
		     spells[41].instant = false;
		     spells[41].perctrigger = (float)RandomFloat(5.0f);
		     spells[41].attackstoptimer = 1000;
		     m_spellcheck[41] = true;


		     spells[42].info = dbcSpell.LookupEntry(8439);
		     spells[42].reqlvl = 38;
		     spells[42].targettype = TARGET_ATTACKING;
		     spells[42].instant = false;
		     spells[42].perctrigger = (float)RandomFloat(5.0f);
		     spells[42].attackstoptimer = 1000;
		     m_spellcheck[42] = true;


		     spells[43].info = dbcSpell.LookupEntry(10201);
		     spells[43].reqlvl = 46;
		     spells[43].targettype = TARGET_ATTACKING;
		     spells[43].instant = false;
		     spells[43].perctrigger = (float)RandomFloat(5.0f);
		     spells[43].attackstoptimer = 1000;
		     m_spellcheck[43] = true;


		     spells[44].info = dbcSpell.LookupEntry(168);
		     spells[44].reqlvl = 1;
		     spells[44].targettype = TARGET_SELF;
		     spells[44].instant = false;
		     spells[44].perctrigger = (float)RandomFloat(5.0f);
		     spells[44].attackstoptimer = 1000;
		     m_spellcheck[44] = true;


		     spells[45].info = dbcSpell.LookupEntry(7300);
		     spells[45].reqlvl = 10;
		     spells[45].targettype = TARGET_SELF;
		     spells[45].instant = false;
		     spells[45].perctrigger = (float)RandomFloat(5.0f);
		     spells[45].attackstoptimer = 1000;
		     m_spellcheck[45] = true;


		     spells[46].info = dbcSpell.LookupEntry(7301);
		     spells[46].reqlvl = 20;
		     spells[46].targettype = TARGET_SELF;
		     spells[46].instant = false;
		     spells[46].perctrigger = (float)RandomFloat(5.0f);
		     spells[46].attackstoptimer = 1000;
		     m_spellcheck[46] = true;


		     spells[47].info = dbcSpell.LookupEntry(7302);
		     spells[47].reqlvl = 30;
		     spells[47].targettype = TARGET_SELF;
		     spells[47].instant = false;
		     spells[47].perctrigger = (float)RandomFloat(5.0f);
		     spells[47].attackstoptimer = 1000;
		     m_spellcheck[47] = true;


		     spells[48].info = dbcSpell.LookupEntry(7320);
		     spells[48].reqlvl = 40;
		     spells[48].targettype = TARGET_SELF;
		     spells[48].instant = false;
		     spells[48].perctrigger = (float)RandomFloat(5.0f);
		     spells[48].attackstoptimer = 1000;
		     m_spellcheck[48] = true;


		     spells[49].info = dbcSpell.LookupEntry(10219);
		     spells[49].reqlvl = 50;
		     spells[49].targettype = TARGET_SELF;
		     spells[49].instant = false;
		     spells[49].perctrigger = (float)RandomFloat(5.0f);
		     spells[49].attackstoptimer = 1000;
		     m_spellcheck[49] = true;


		     spells[50].info = dbcSpell.LookupEntry(10220);
		     spells[50].reqlvl = 60;
		     spells[50].targettype = TARGET_SELF;
		     spells[50].instant = false;
		     spells[50].perctrigger = (float)RandomFloat(5.0f);
		     spells[50].attackstoptimer = 1000;
		     m_spellcheck[50] = true;


		     spells[51].info = dbcSpell.LookupEntry(1463);
		     spells[51].reqlvl = 20;
		     spells[51].targettype = TARGET_SELF;
		     spells[51].instant = false;
		     spells[51].perctrigger = (float)RandomFloat(5.0f);
		     spells[51].attackstoptimer = 1000;
		     m_spellcheck[51] = true;


		     spells[52].info = dbcSpell.LookupEntry(8494);
		     spells[52].reqlvl = 28;
		     spells[52].targettype = TARGET_SELF;
		     spells[52].instant = false;
		     spells[52].perctrigger = (float)RandomFloat(5.0f);
		     spells[52].attackstoptimer = 1000;
		     m_spellcheck[52] = true;


		     spells[53].info = dbcSpell.LookupEntry(8495);
		     spells[53].reqlvl = 36;
		     spells[53].targettype = TARGET_SELF;
		     spells[53].instant = false;
		     spells[53].perctrigger = (float)RandomFloat(5.0f);
		     spells[53].attackstoptimer = 1000;
		     m_spellcheck[53] = true;


		     spells[54].info = dbcSpell.LookupEntry(10191);
		     spells[54].reqlvl = 44;
		     spells[54].targettype = TARGET_SELF;
		     spells[54].instant = false;
		     spells[54].perctrigger = (float)RandomFloat(5.0f);
		     spells[54].attackstoptimer = 1000;
		     m_spellcheck[54] = true;


		     spells[55].info = dbcSpell.LookupEntry(10192);
		     spells[55].reqlvl = 52;
		     spells[55].targettype = TARGET_SELF;
		     spells[55].instant = false;
		     spells[55].perctrigger = (float)RandomFloat(5.0f);
		     spells[55].attackstoptimer = 1000;
		     m_spellcheck[55] = true;


		     spells[56].info = dbcSpell.LookupEntry(10193);
		     spells[56].reqlvl = 60;
		     spells[56].targettype = TARGET_SELF;
		     spells[56].instant = false;
		     spells[56].perctrigger = (float)RandomFloat(5.0f);
		     spells[56].attackstoptimer = 1000;
		     m_spellcheck[56] = true;

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
	      			if ((spells[i].reqlvl<=_unit->GetUInt32Value(UNIT_FIELD_LEVEL)) && (spells[i].reqlvl+10>=_unit->GetUInt32Value(UNIT_FIELD_LEVEL)))
		      	  {
			      	if(m_spellcheck[i])
		      		{
				        target = _unit->GetAIInterface()->GetNextTarget();
			       	  switch(spells[i].targettype)
			    		  {
			    		    case TARGET_SELF:
				         			_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
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
class HumanoidWarlock : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(HumanoidWarlock);
	SP_AI_Spell spells[56];
 bool m_spellcheck[56];

    HumanoidWarlock(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 56;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		     spells[0].info = dbcSpell.LookupEntry(348);
		     spells[0].reqlvl = 1;
		     spells[0].targettype = TARGET_ATTACKING;
		     spells[0].instant = false;
		     spells[0].perctrigger = (float)RandomFloat(5.0f);
		     spells[0].attackstoptimer = 1000;
		     m_spellcheck[0] = true;


		     spells[1].info = dbcSpell.LookupEntry(172);
		     spells[1].reqlvl = 4;
		     spells[1].targettype = TARGET_ATTACKING;
		     spells[1].instant = false;
		     spells[1].perctrigger = (float)RandomFloat(5.0f);
		     spells[1].attackstoptimer = 1000;
		     m_spellcheck[1] = true;


		     spells[2].info = dbcSpell.LookupEntry(686);
		     spells[2].reqlvl = 1;
		     spells[2].targettype = TARGET_ATTACKING;
		     spells[2].instant = false;
		     spells[2].perctrigger = (float)RandomFloat(5.0f);
		     spells[2].attackstoptimer = 1000;
		     m_spellcheck[2] = true;


		     spells[3].info = dbcSpell.LookupEntry(695);
		     spells[3].reqlvl = 6;
		     spells[3].targettype = TARGET_ATTACKING;
		     spells[3].instant = false;
		     spells[3].perctrigger = (float)RandomFloat(5.0f);
		     spells[3].attackstoptimer = 1000;
		     m_spellcheck[3] = true;


		     spells[4].info = dbcSpell.LookupEntry(1454);
		     spells[4].reqlvl = 6;
		     spells[4].targettype = TARGET_ATTACKING;
		     spells[4].instant = false;
		     spells[4].perctrigger = (float)RandomFloat(5.0f);
		     spells[4].attackstoptimer = 1000;
		     m_spellcheck[4] = true;


		     spells[5].info = dbcSpell.LookupEntry(980);
		     spells[5].reqlvl = 8;
		     spells[5].targettype = TARGET_ATTACKING;
		     spells[5].instant = false;
		     spells[5].perctrigger = (float)RandomFloat(5.0f);
		     spells[5].attackstoptimer = 1000;
		     m_spellcheck[5] = true;


		     spells[6].info = dbcSpell.LookupEntry(5782);
		     spells[6].reqlvl = 8;
		     spells[6].targettype = TARGET_ATTACKING;
		     spells[6].instant = false;
		     spells[6].perctrigger = 0; //fear crash server
		     spells[6].attackstoptimer = 1000;
		     m_spellcheck[6] = true;


		     spells[7].info = dbcSpell.LookupEntry(707);
		     spells[7].reqlvl = 10;
		     spells[7].targettype = TARGET_ATTACKING;
		     spells[7].instant = false;
		     spells[7].perctrigger = (float)RandomFloat(5.0f);
		     spells[7].attackstoptimer = 1000;
		     m_spellcheck[7] = true;


		     spells[8].info = dbcSpell.LookupEntry(172);
		     spells[8].reqlvl = 4;
		     spells[8].targettype = TARGET_ATTACKING;
		     spells[8].instant = false;
		     spells[8].perctrigger = (float)RandomFloat(5.0f);
		     spells[8].attackstoptimer = 1000;
		     m_spellcheck[8] = true;


		     spells[9].info = dbcSpell.LookupEntry(705);
		     spells[9].reqlvl = 12;
		     spells[9].targettype = TARGET_ATTACKING;
		     spells[9].instant = false;
		     spells[9].perctrigger = (float)RandomFloat(5.0f);
		     spells[9].attackstoptimer = 1000;
		     m_spellcheck[9] = true;


		     spells[10].info = dbcSpell.LookupEntry(6222);
		     spells[10].reqlvl = 14;
		     spells[10].targettype = TARGET_ATTACKING;
		     spells[10].instant = false;
		     spells[10].perctrigger = (float)RandomFloat(5.0f);
		     spells[10].attackstoptimer = 1000;
		     m_spellcheck[10] = true;


		     spells[11].info = dbcSpell.LookupEntry(704);
		     spells[11].reqlvl = 14;
		     spells[11].targettype = TARGET_ATTACKING;
		     spells[11].instant = false;
		     spells[11].perctrigger = (float)RandomFloat(5.0f);
		     spells[11].attackstoptimer = 1000;
		     m_spellcheck[11] = true;


		     spells[12].info = dbcSpell.LookupEntry(689);
		     spells[12].reqlvl = 14;
		     spells[12].targettype = TARGET_ATTACKING;
		     spells[12].instant = false;
		     spells[12].perctrigger = (float)RandomFloat(5.0f);
		     spells[12].attackstoptimer = 1000;
		     m_spellcheck[12] = true;


		     spells[13].info = dbcSpell.LookupEntry(1455);
		     spells[13].reqlvl = 16;
		     spells[13].targettype = TARGET_ATTACKING;
		     spells[13].instant = false;
		     spells[13].perctrigger = (float)RandomFloat(5.0f);
		     spells[13].attackstoptimer = 1000;
		     m_spellcheck[13] = true;


		     spells[14].info = dbcSpell.LookupEntry(1014);
		     spells[14].reqlvl = 18;
		     spells[14].targettype = TARGET_ATTACKING;
		     spells[14].instant = false;
		     spells[14].perctrigger = (float)RandomFloat(5.0f);
		     spells[14].attackstoptimer = 1000;
		     m_spellcheck[14] = true;


		     spells[15].info = dbcSpell.LookupEntry(1094);
		     spells[15].reqlvl = 20;
		     spells[15].targettype = TARGET_ATTACKING;
		     spells[15].instant = false;
		     spells[15].perctrigger = (float)RandomFloat(5.0f);
		     spells[15].attackstoptimer = 1000;
		     m_spellcheck[15] = true;


		     spells[16].info = dbcSpell.LookupEntry(1088);
		     spells[16].reqlvl = 20;
		     spells[16].targettype = TARGET_ATTACKING;
		     spells[16].instant = false;
		     spells[16].perctrigger = (float)RandomFloat(5.0f);
		     spells[16].attackstoptimer = 1000;
		     m_spellcheck[16] = true;


		     spells[17].info = dbcSpell.LookupEntry(1106);
		     spells[17].reqlvl = 28;
		     spells[17].targettype = TARGET_ATTACKING;
		     spells[17].instant = false;
		     spells[17].perctrigger = (float)RandomFloat(5.0f);
		     spells[17].attackstoptimer = 1000;
		     m_spellcheck[17] = true;


		     spells[18].info = dbcSpell.LookupEntry(699);
		     spells[18].reqlvl = 22;
		     spells[18].targettype = TARGET_ATTACKING;
		     spells[18].instant = false;
		     spells[18].perctrigger = (float)RandomFloat(5.0f);
		     spells[18].attackstoptimer = 1000;
		     m_spellcheck[18] = true;


		     spells[19].info = dbcSpell.LookupEntry(6223);
		     spells[19].reqlvl = 24;
		     spells[19].targettype = TARGET_ATTACKING;
		     spells[19].instant = false;
		     spells[19].perctrigger = (float)RandomFloat(5.0f);
		     spells[19].attackstoptimer = 1000;
		     m_spellcheck[19] = true;


		     spells[20].info = dbcSpell.LookupEntry(5138);
		     spells[20].reqlvl = 24;
		     spells[20].targettype = TARGET_ATTACKING;
		     spells[20].instant = false;
		     spells[20].perctrigger = (float)RandomFloat(5.0f);
		     spells[20].attackstoptimer = 1000;
		     m_spellcheck[20] = true;


		     spells[21].info = dbcSpell.LookupEntry(1456);
		     spells[21].reqlvl = 26;
		     spells[21].targettype = TARGET_ATTACKING;
		     spells[21].instant = false;
		     spells[21].perctrigger = (float)RandomFloat(5.0f);
		     spells[21].attackstoptimer = 1000;
		     m_spellcheck[21] = true;


		     spells[22].info = dbcSpell.LookupEntry(6217);
		     spells[22].reqlvl = 28;
		     spells[22].targettype = TARGET_ATTACKING;
		     spells[22].instant = false;
		     spells[22].perctrigger = (float)RandomFloat(5.0f);
		     spells[22].attackstoptimer = 1000;
		     m_spellcheck[22] = true;


		     spells[23].info = dbcSpell.LookupEntry(7658);
		     spells[23].reqlvl = 28;
		     spells[23].targettype = TARGET_ATTACKING;
		     spells[23].instant = false;
		     spells[23].perctrigger = (float)RandomFloat(5.0f);
		     spells[23].attackstoptimer = 1000;
		     m_spellcheck[23] = true;


		     spells[24].info = dbcSpell.LookupEntry(7641);
		     spells[24].reqlvl = 36;
		     spells[24].targettype = TARGET_ATTACKING;
		     spells[24].instant = false;
		     spells[24].perctrigger = (float)RandomFloat(5.0f);
		     spells[24].attackstoptimer = 1000;
		     m_spellcheck[24] = true;


		     spells[25].info = dbcSpell.LookupEntry(18223);
		     spells[25].reqlvl = 0;
		     spells[25].targettype = TARGET_ATTACKING;
		     spells[25].instant = false;
		     spells[25].perctrigger = (float)RandomFloat(5.0f);
		     spells[25].attackstoptimer = 1000;
		     m_spellcheck[25] = true;


		     spells[26].info = dbcSpell.LookupEntry(709);
		     spells[26].reqlvl = 30;
		     spells[26].targettype = TARGET_ATTACKING;
		     spells[26].instant = false;
		     spells[26].perctrigger = (float)RandomFloat(5.0f);
		     spells[26].attackstoptimer = 1000;
		     m_spellcheck[26] = true;


		     spells[27].info = dbcSpell.LookupEntry(2941);
		     spells[27].reqlvl = 30;
		     spells[27].targettype = TARGET_ATTACKING;
		     spells[27].instant = false;
		     spells[27].perctrigger = (float)RandomFloat(5.0f);
		     spells[27].attackstoptimer = 1000;
		     m_spellcheck[27] = true;


		     spells[28].info = dbcSpell.LookupEntry(6213);
		     spells[28].reqlvl = 32;
		     spells[28].targettype = TARGET_ATTACKING;
		     spells[28].instant = false;
		     spells[28].perctrigger = 0; //Fear crash server
		     spells[28].attackstoptimer = 1000;
		     m_spellcheck[28] = true;


		     spells[29].info = dbcSpell.LookupEntry(7648);
		     spells[29].reqlvl = 34;
		     spells[29].targettype = TARGET_ATTACKING;
		     spells[29].instant = false;
		     spells[29].perctrigger = (float)RandomFloat(5.0f);
		     spells[29].attackstoptimer = 1000;
		     m_spellcheck[29] = true;


		     spells[30].info = dbcSpell.LookupEntry(6226);
		     spells[30].reqlvl = 34;
		     spells[30].targettype = TARGET_ATTACKING;
		     spells[30].instant = false;
		     spells[30].perctrigger = (float)RandomFloat(5.0f);
		     spells[30].attackstoptimer = 1000;
		     m_spellcheck[30] = true;


		     spells[31].info = dbcSpell.LookupEntry(11687);
		     spells[31].reqlvl = 36;
		     spells[31].targettype = TARGET_ATTACKING;
		     spells[31].instant = false;
		     spells[31].perctrigger = (float)RandomFloat(5.0f);
		     spells[31].attackstoptimer = 1000;
		     m_spellcheck[31] = true;


		     spells[32].info = dbcSpell.LookupEntry(11711);
		     spells[32].reqlvl = 38;
		     spells[32].targettype = TARGET_ATTACKING;
		     spells[32].instant = false;
		     spells[32].perctrigger = (float)RandomFloat(5.0f);
		     spells[32].attackstoptimer = 1000;
		     m_spellcheck[32] = true;


		     spells[33].info = dbcSpell.LookupEntry(11659);
		     spells[33].reqlvl = 44;
		     spells[33].targettype = TARGET_ATTACKING;
		     spells[33].instant = false;
		     spells[33].perctrigger = (float)RandomFloat(5.0f);
		     spells[33].attackstoptimer = 1000;
		     m_spellcheck[33] = true;


		     spells[34].info = dbcSpell.LookupEntry(11665);
		     spells[34].reqlvl = 40;
		     spells[34].targettype = TARGET_ATTACKING;
		     spells[34].instant = false;
		     spells[34].perctrigger = (float)RandomFloat(5.0f);
		     spells[34].attackstoptimer = 1000;
		     m_spellcheck[34] = true;


		     spells[35].info = dbcSpell.LookupEntry(7651);
		     spells[35].reqlvl = 38;
		     spells[35].targettype = TARGET_ATTACKING;
		     spells[35].instant = false;
		     spells[35].perctrigger = (float)RandomFloat(5.0f);
		     spells[35].attackstoptimer = 1000;
		     m_spellcheck[35] = true;


		     spells[36].info = dbcSpell.LookupEntry(7659);
		     spells[36].reqlvl = 42;
		     spells[36].targettype = TARGET_ATTACKING;
		     spells[36].instant = false;
		     spells[36].perctrigger = (float)RandomFloat(5.0f);
		     spells[36].attackstoptimer = 1000;
		     m_spellcheck[36] = true;


		     spells[37].info = dbcSpell.LookupEntry(6789);
		     spells[37].reqlvl = 42;
		     spells[37].targettype = TARGET_ATTACKING;
		     spells[37].instant = false;
		     spells[37].perctrigger = (float)RandomFloat(5.0f);
		     spells[37].attackstoptimer = 1000;
		     m_spellcheck[37] = true;


		     spells[38].info = dbcSpell.LookupEntry(11671);
		     spells[38].reqlvl = 44;
		     spells[38].targettype = TARGET_ATTACKING;
		     spells[38].instant = false;
		     spells[38].perctrigger = (float)RandomFloat(5.0f);
		     spells[38].attackstoptimer = 1000;
		     m_spellcheck[38] = true;


		     spells[39].info = dbcSpell.LookupEntry(17862);
		     spells[39].reqlvl = 44;
		     spells[39].targettype = TARGET_ATTACKING;
		     spells[39].instant = false;
		     spells[39].perctrigger = (float)RandomFloat(5.0f);
		     spells[39].attackstoptimer = 1000;
		     m_spellcheck[39] = true;


		     spells[40].info = dbcSpell.LookupEntry(11699);
		     spells[40].reqlvl = 46;
		     spells[40].targettype = TARGET_ATTACKING;
		     spells[40].instant = false;
		     spells[40].perctrigger = (float)RandomFloat(5.0f);
		     spells[40].attackstoptimer = 1000;
		     m_spellcheck[40] = true;


		     spells[41].info = dbcSpell.LookupEntry(11688);
		     spells[41].reqlvl = 46;
		     spells[41].targettype = TARGET_ATTACKING;
		     spells[41].instant = false;
		     spells[41].perctrigger = (float)RandomFloat(5.0f);
		     spells[41].attackstoptimer = 1000;
		     m_spellcheck[41] = true;


		     spells[42].info = dbcSpell.LookupEntry(11712);
		     spells[42].reqlvl = 48;
		     spells[42].targettype = TARGET_ATTACKING;
		     spells[42].instant = false;
		     spells[42].perctrigger = (float)RandomFloat(5.0f);
		     spells[42].attackstoptimer = 1000;
		     m_spellcheck[42] = true;


		     spells[43].info = dbcSpell.LookupEntry(11660);
		     spells[43].reqlvl = 52;
		     spells[43].targettype = TARGET_ATTACKING;
		     spells[43].instant = false;
		     spells[43].perctrigger = (float)RandomFloat(5.0f);
		     spells[43].attackstoptimer = 1000;
		     m_spellcheck[43] = true;


		     spells[44].info = dbcSpell.LookupEntry(11661);
		     spells[44].reqlvl = 60;
		     spells[44].targettype = TARGET_ATTACKING;
		     spells[44].instant = false;
		     spells[44].perctrigger = (float)RandomFloat(5.0f);
		     spells[44].attackstoptimer = 1000;
		     m_spellcheck[44] = true;


		     spells[45].info = dbcSpell.LookupEntry(11661);
		     spells[45].reqlvl = 60;
		     spells[45].targettype = TARGET_ATTACKING;
		     spells[45].instant = false;
		     spells[45].perctrigger = (float)RandomFloat(5.0f);
		     spells[45].attackstoptimer = 1000;
		     m_spellcheck[45] = true;


		     spells[46].info = dbcSpell.LookupEntry(17926);
		     spells[46].reqlvl = 58;
		     spells[46].targettype = TARGET_ATTACKING;
		     spells[46].instant = false;
		     spells[46].perctrigger = (float)RandomFloat(5.0f);
		     spells[46].attackstoptimer = 1000;
		     m_spellcheck[46] = true;


		     spells[47].info = dbcSpell.LookupEntry(11713);
		     spells[47].reqlvl = 58;
		     spells[47].targettype = TARGET_ATTACKING;
		     spells[47].instant = false;
		     spells[47].perctrigger = (float)RandomFloat(5.0f);
		     spells[47].attackstoptimer = 1000;
		     m_spellcheck[47] = true;


		     spells[48].info = dbcSpell.LookupEntry(6215);
		     spells[48].reqlvl = 56;
		     spells[48].targettype = TARGET_ATTACKING;
		     spells[48].instant = false;
		     spells[48].perctrigger = 0; //fear crash server
		     spells[48].attackstoptimer = 1000;
		     m_spellcheck[48] = true;


		     spells[49].info = dbcSpell.LookupEntry(11689);
		     spells[49].reqlvl = 56;
		     spells[49].targettype = TARGET_ATTACKING;
		     spells[49].instant = false;
		     spells[49].perctrigger = (float)RandomFloat(5.0f);
		     spells[49].attackstoptimer = 1000;
		     m_spellcheck[49] = true;


		     spells[50].info = dbcSpell.LookupEntry(11717);
		     spells[50].reqlvl = 56;
		     spells[50].targettype = TARGET_ATTACKING;
		     spells[50].instant = false;
		     spells[50].perctrigger = (float)RandomFloat(5.0f);
		     spells[50].attackstoptimer = 1000;
		     m_spellcheck[50] = true;


		     spells[51].info = dbcSpell.LookupEntry(11704);
		     spells[51].reqlvl = 54;
		     spells[51].targettype = TARGET_ATTACKING;
		     spells[51].instant = false;
		     spells[51].perctrigger = (float)RandomFloat(5.0f);
		     spells[51].attackstoptimer = 1000;
		     m_spellcheck[51] = true;


		     spells[52].info = dbcSpell.LookupEntry(11700);
		     spells[52].reqlvl = 54;
		     spells[52].targettype = TARGET_ATTACKING;
		     spells[52].instant = false;
		     spells[52].perctrigger = (float)RandomFloat(5.0f);
		     spells[52].attackstoptimer = 1000;
		     m_spellcheck[52] = true;


		     spells[53].info = dbcSpell.LookupEntry(11672);
		     spells[53].reqlvl = 54;
		     spells[53].targettype = TARGET_ATTACKING;
		     spells[53].instant = false;
		     spells[53].perctrigger = (float)RandomFloat(5.0f);
		     spells[53].attackstoptimer = 1000;
		     m_spellcheck[53] = true;


		     spells[54].info = dbcSpell.LookupEntry(17925);
		     spells[54].reqlvl = 50;
		     spells[54].targettype = TARGET_ATTACKING;
		     spells[54].instant = false;
		     spells[54].perctrigger = (float)RandomFloat(5.0f);
		     spells[54].attackstoptimer = 1000;
		     m_spellcheck[54] = true;


		     spells[55].info = dbcSpell.LookupEntry(11667);
		     spells[55].reqlvl = 50;
		     spells[55].targettype = TARGET_ATTACKING;
		     spells[55].instant = false;
		     spells[55].perctrigger = (float)RandomFloat(5.0f);
		     spells[55].attackstoptimer = 1000;
		     m_spellcheck[55] = true;

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
	      			if ((spells[i].reqlvl<=_unit->GetUInt32Value(UNIT_FIELD_LEVEL)) && (spells[i].reqlvl+10>=_unit->GetUInt32Value(UNIT_FIELD_LEVEL)))
		      	  {
			      	if(m_spellcheck[i])
		      		{
				        target = _unit->GetAIInterface()->GetNextTarget();
			       	  switch(spells[i].targettype)
			    		  {
			    		    case TARGET_SELF:
				         			_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
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

class HumanoidPriest : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(HumanoidPriest);
	SP_AI_Spell spells[49];
 bool m_spellcheck[49];

    HumanoidPriest(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 49;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		     spells[0].info = dbcSpell.LookupEntry(589);
		     spells[0].reqlvl = 4;
		     spells[0].hpreqtocast = 85;
		     spells[0].targettype = TARGET_ATTACKING;
		     spells[0].instant = false;
		     spells[0].perctrigger = (float)RandomFloat(5.0f);
		     spells[0].attackstoptimer = 1000;
		     m_spellcheck[0] = true;


		     spells[1].info = dbcSpell.LookupEntry(585);
		     spells[1].reqlvl = 1;
		     spells[1].hpreqtocast = 85;
		     spells[1].targettype = TARGET_ATTACKING;
		     spells[1].instant = false;
		     spells[1].perctrigger = (float)RandomFloat(5.0f);
		     spells[1].attackstoptimer = 1000;
		     m_spellcheck[1] = true;


		     spells[2].info = dbcSpell.LookupEntry(594);
		     spells[2].reqlvl = 10;
		     spells[2].hpreqtocast = 85;
		     spells[2].targettype = TARGET_ATTACKING;
		     spells[2].instant = false;
		     spells[2].perctrigger = (float)RandomFloat(5.0f);
		     spells[2].attackstoptimer = 1000;
		     m_spellcheck[2] = true;


		     spells[3].info = dbcSpell.LookupEntry(591);
		     spells[3].reqlvl = 6;
		     spells[3].hpreqtocast = 85;
		     spells[3].targettype = TARGET_ATTACKING;
		     spells[3].instant = false;
		     spells[3].perctrigger = (float)RandomFloat(5.0f);
		     spells[3].attackstoptimer = 1000;
		     m_spellcheck[3] = true;


		     spells[4].info = dbcSpell.LookupEntry(8092);
		     spells[4].reqlvl = 10;
		     spells[4].hpreqtocast = 85;
		     spells[4].targettype = TARGET_ATTACKING;
		     spells[4].instant = false;
		     spells[4].perctrigger = (float)RandomFloat(5.0f);
		     spells[4].attackstoptimer = 1000;
		     m_spellcheck[4] = true;


		     spells[5].info = dbcSpell.LookupEntry(970);
		     spells[5].reqlvl = 18;
		     spells[5].hpreqtocast = 85;
		     spells[5].targettype = TARGET_ATTACKING;
		     spells[5].instant = false;
		     spells[5].perctrigger = (float)RandomFloat(5.0f);
		     spells[5].attackstoptimer = 1000;
		     m_spellcheck[5] = true;


		     spells[6].info = dbcSpell.LookupEntry(598);
		     spells[6].reqlvl = 14;
		     spells[6].hpreqtocast = 85;
		     spells[6].targettype = TARGET_ATTACKING;
		     spells[6].instant = false;
		     spells[6].perctrigger = (float)RandomFloat(5.0f);
		     spells[6].attackstoptimer = 1000;
		     m_spellcheck[6] = true;


		     spells[7].info = dbcSpell.LookupEntry(8102);
		     spells[7].reqlvl = 16;
		     spells[7].hpreqtocast = 85;
		     spells[7].targettype = TARGET_ATTACKING;
		     spells[7].instant = false;
		     spells[7].perctrigger = (float)RandomFloat(5.0f);
		     spells[7].attackstoptimer = 1000;
		     m_spellcheck[7] = true;


		     spells[8].info = dbcSpell.LookupEntry(8122);
		     spells[8].reqlvl = 14;
		     spells[8].hpreqtocast = 85;
		     spells[8].targettype = TARGET_ATTACKING;
		     spells[8].instant = false;
		     spells[8].perctrigger = (float)RandomFloat(5.0f);
		     spells[8].attackstoptimer = 1000;
		     m_spellcheck[8] = true;


		     spells[9].info = dbcSpell.LookupEntry(992);
		     spells[9].reqlvl = 26;
		     spells[9].hpreqtocast = 85;
		     spells[9].targettype = TARGET_ATTACKING;
		     spells[9].instant = false;
		     spells[9].perctrigger = (float)RandomFloat(5.0f);
		     spells[9].attackstoptimer = 1000;
		     m_spellcheck[9] = true;


		     spells[10].info = dbcSpell.LookupEntry(984);
		     spells[10].reqlvl = 22;
		     spells[10].hpreqtocast = 85;
		     spells[10].targettype = TARGET_ATTACKING;
		     spells[10].instant = false;
		     spells[10].perctrigger = (float)RandomFloat(5.0f);
		     spells[10].attackstoptimer = 1000;
		     m_spellcheck[10] = true;


		     spells[11].info = dbcSpell.LookupEntry(8103);
		     spells[11].reqlvl = 22;
		     spells[11].hpreqtocast = 85;
		     spells[11].targettype = TARGET_ATTACKING;
		     spells[11].instant = false;
		     spells[11].perctrigger = (float)RandomFloat(5.0f);
		     spells[11].attackstoptimer = 1000;
		     m_spellcheck[11] = true;


		     spells[12].info = dbcSpell.LookupEntry(8104);
		     spells[12].reqlvl = 28;
		     spells[12].hpreqtocast = 85;
		     spells[12].targettype = TARGET_ATTACKING;
		     spells[12].instant = false;
		     spells[12].perctrigger = (float)RandomFloat(5.0f);
		     spells[12].attackstoptimer = 1000;
		     m_spellcheck[12] = true;


		     spells[13].info = dbcSpell.LookupEntry(8122);
		     spells[13].reqlvl = 14;
		     spells[13].hpreqtocast = 85;
		     spells[13].targettype = TARGET_ATTACKING;
		     spells[13].instant = false;
		     spells[13].perctrigger = (float)RandomFloat(5.0f);
		     spells[13].attackstoptimer = 1000;
		     m_spellcheck[13] = true;


		     spells[14].info = dbcSpell.LookupEntry(2767);
		     spells[14].reqlvl = 34;
		     spells[14].hpreqtocast = 85;
		     spells[14].targettype = TARGET_ATTACKING;
		     spells[14].instant = false;
		     spells[14].perctrigger = (float)RandomFloat(5.0f);
		     spells[14].attackstoptimer = 1000;
		     m_spellcheck[14] = true;


		     spells[15].info = dbcSpell.LookupEntry(1004);
		     spells[15].reqlvl = 30;
		     spells[15].hpreqtocast = 85;
		     spells[15].targettype = TARGET_ATTACKING;
		     spells[15].instant = false;
		     spells[15].perctrigger = (float)RandomFloat(5.0f);
		     spells[15].attackstoptimer = 1000;
		     m_spellcheck[15] = true;


		     spells[16].info = dbcSpell.LookupEntry(8105);
		     spells[16].reqlvl = 34;
		     spells[16].hpreqtocast = 85;
		     spells[16].targettype = TARGET_ATTACKING;
		     spells[16].instant = false;
		     spells[16].perctrigger = (float)RandomFloat(5.0f);
		     spells[16].attackstoptimer = 1000;
		     m_spellcheck[16] = true;


		     spells[17].info = dbcSpell.LookupEntry(8122);
		     spells[17].reqlvl = 14;
		     spells[17].hpreqtocast = 85;
		     spells[17].targettype = TARGET_ATTACKING;
		     spells[17].instant = false;
		     spells[17].perctrigger = (float)RandomFloat(5.0f);
		     spells[17].attackstoptimer = 1000;
		     m_spellcheck[17] = true;


		     spells[18].info = dbcSpell.LookupEntry(10892);
		     spells[18].reqlvl = 42;
		     spells[18].hpreqtocast = 85;
		     spells[18].targettype = TARGET_ATTACKING;
		     spells[18].instant = false;
		     spells[18].perctrigger = (float)RandomFloat(5.0f);
		     spells[18].attackstoptimer = 1000;
		     m_spellcheck[18] = true;


		     spells[19].info = dbcSpell.LookupEntry(6060);
		     spells[19].reqlvl = 38;
		     spells[19].hpreqtocast = 85;
		     spells[19].targettype = TARGET_ATTACKING;
		     spells[19].instant = false;
		     spells[19].perctrigger = (float)RandomFloat(5.0f);
		     spells[19].attackstoptimer = 1000;
		     m_spellcheck[19] = true;


		     spells[20].info = dbcSpell.LookupEntry(10933);
		     spells[20].reqlvl = 46;
		     spells[20].hpreqtocast = 85;
		     spells[20].targettype = TARGET_ATTACKING;
		     spells[20].instant = false;
		     spells[20].perctrigger = (float)RandomFloat(5.0f);
		     spells[20].attackstoptimer = 1000;
		     m_spellcheck[20] = true;


		     spells[21].info = dbcSpell.LookupEntry(8106);
		     spells[21].reqlvl = 40;
		     spells[21].hpreqtocast = 85;
		     spells[21].targettype = TARGET_ATTACKING;
		     spells[21].instant = false;
		     spells[21].perctrigger = (float)RandomFloat(5.0f);
		     spells[21].attackstoptimer = 1000;
		     m_spellcheck[21] = true;


		     spells[22].info = dbcSpell.LookupEntry(10945);
		     spells[22].reqlvl = 46;
		     spells[22].hpreqtocast = 85;
		     spells[22].targettype = TARGET_ATTACKING;
		     spells[22].instant = false;
		     spells[22].perctrigger = (float)RandomFloat(5.0f);
		     spells[22].attackstoptimer = 1000;
		     m_spellcheck[22] = true;


		     spells[23].info = dbcSpell.LookupEntry(8122);
		     spells[23].reqlvl = 14;
		     spells[23].hpreqtocast = 85;
		     spells[23].targettype = TARGET_ATTACKING;
		     spells[23].instant = false;
		     spells[23].perctrigger = (float)RandomFloat(5.0f);
		     spells[23].attackstoptimer = 1000;
		     m_spellcheck[23] = true;


		     spells[24].info = dbcSpell.LookupEntry(10893);
		     spells[24].reqlvl = 50;
		     spells[24].hpreqtocast = 85;
		     spells[24].targettype = TARGET_ATTACKING;
		     spells[24].instant = false;
		     spells[24].perctrigger = (float)RandomFloat(5.0f);
		     spells[24].attackstoptimer = 1000;
		     m_spellcheck[24] = true;


		     spells[25].info = dbcSpell.LookupEntry(10894);
		     spells[25].reqlvl = 58;
		     spells[25].hpreqtocast = 85;
		     spells[25].targettype = TARGET_ATTACKING;
		     spells[25].instant = false;
		     spells[25].perctrigger = (float)RandomFloat(5.0f);
		     spells[25].attackstoptimer = 1000;
		     m_spellcheck[25] = true;


		     spells[26].info = dbcSpell.LookupEntry(10934);
		     spells[26].reqlvl = 54;
		     spells[26].hpreqtocast = 85;
		     spells[26].targettype = TARGET_ATTACKING;
		     spells[26].instant = false;
		     spells[26].perctrigger = (float)RandomFloat(5.0f);
		     spells[26].attackstoptimer = 1000;
		     m_spellcheck[26] = true;


		     spells[27].info = dbcSpell.LookupEntry(10946);
		     spells[27].reqlvl = 52;
		     spells[27].hpreqtocast = 85;
		     spells[27].targettype = TARGET_ATTACKING;
		     spells[27].instant = false;
		     spells[27].perctrigger = (float)RandomFloat(5.0f);
		     spells[27].attackstoptimer = 1000;
		     m_spellcheck[27] = true;


		     spells[28].info = dbcSpell.LookupEntry(10947);
		     spells[28].reqlvl = 58;
		     spells[28].hpreqtocast = 85;
		     spells[28].targettype = TARGET_ATTACKING;
		     spells[28].instant = false;
		     spells[28].perctrigger = (float)RandomFloat(5.0f);
		     spells[28].attackstoptimer = 1000;
		     m_spellcheck[28] = true;


		     spells[29].info = dbcSpell.LookupEntry(17);
		     spells[29].reqlvl = 6;
		     spells[29].hpreqtocast = 85;
		     spells[29].targettype = TARGET_SELF;
		     spells[29].instant = false;
		     spells[29].perctrigger = (float)RandomFloat(5.0f);
		     spells[29].attackstoptimer = 1000;
		     m_spellcheck[29] = true;


		     spells[30].info = dbcSpell.LookupEntry(592);
		     spells[30].reqlvl = 12;
		     spells[30].hpreqtocast = 85;
		     spells[30].targettype = TARGET_SELF;
		     spells[30].instant = false;
		     spells[30].perctrigger = (float)RandomFloat(5.0f);
		     spells[30].attackstoptimer = 1000;
		     m_spellcheck[30] = true;


		     spells[31].info = dbcSpell.LookupEntry(600);
		     spells[31].reqlvl = 18;
		     spells[31].hpreqtocast = 85;
		     spells[31].targettype = TARGET_SELF;
		     spells[31].instant = false;
		     spells[31].perctrigger = (float)RandomFloat(5.0f);
		     spells[31].attackstoptimer = 1000;
		     m_spellcheck[31] = true;


		     spells[32].info = dbcSpell.LookupEntry(3747);
		     spells[32].reqlvl = 24;
		     spells[32].hpreqtocast = 85;
		     spells[32].targettype = TARGET_SELF;
		     spells[32].instant = false;
		     spells[32].perctrigger = (float)RandomFloat(5.0f);
		     spells[32].attackstoptimer = 1000;
		     m_spellcheck[32] = true;


		     spells[33].info = dbcSpell.LookupEntry(6066);
		     spells[33].reqlvl = 36;
		     spells[33].hpreqtocast = 85;
		     spells[33].targettype = TARGET_SELF;
		     spells[33].instant = false;
		     spells[33].perctrigger = (float)RandomFloat(5.0f);
		     spells[33].attackstoptimer = 1000;
		     m_spellcheck[33] = true;


		     spells[34].info = dbcSpell.LookupEntry(10899);
		     spells[34].reqlvl = 48;
		     spells[34].hpreqtocast = 85;
		     spells[34].targettype = TARGET_SELF;
		     spells[34].instant = false;
		     spells[34].perctrigger = (float)RandomFloat(5.0f);
		     spells[34].attackstoptimer = 1000;
		     m_spellcheck[34] = true;


		     spells[35].info = dbcSpell.LookupEntry(10900);
		     spells[35].reqlvl = 54;
		     spells[35].hpreqtocast = 85;
		     spells[35].targettype = TARGET_SELF;
		     spells[35].instant = false;
		     spells[35].perctrigger = (float)RandomFloat(5.0f);
		     spells[35].attackstoptimer = 1000;
		     m_spellcheck[35] = true;


		     spells[36].info = dbcSpell.LookupEntry(2052);
		     spells[36].reqlvl = 4;
		     spells[36].hpreqtocast = 50;
		     spells[36].targettype = TARGET_SELF;
		     spells[36].instant = false;
		     spells[36].perctrigger = (float)RandomFloat(5.0f);
		     spells[36].attackstoptimer = 1000;
		     m_spellcheck[36] = true;


		     spells[37].info = dbcSpell.LookupEntry(2053);
		     spells[37].reqlvl = 10;
		     spells[37].hpreqtocast = 50;
		     spells[37].targettype = TARGET_SELF;
		     spells[37].instant = false;
		     spells[37].perctrigger = (float)RandomFloat(5.0f);
		     spells[37].attackstoptimer = 1000;
		     m_spellcheck[37] = true;


		     spells[38].info = dbcSpell.LookupEntry(9472);
		     spells[38].reqlvl = 26;
		     spells[38].hpreqtocast = 50;
		     spells[38].targettype = TARGET_SELF;
		     spells[38].instant = false;
		     spells[38].perctrigger = (float)RandomFloat(5.0f);
		     spells[38].attackstoptimer = 1000;
		     m_spellcheck[38] = true;


		     spells[39].info = dbcSpell.LookupEntry(9474);
		     spells[39].reqlvl = 38;
		     spells[39].hpreqtocast = 50;
		     spells[39].targettype = TARGET_SELF;
		     spells[39].instant = false;
		     spells[39].perctrigger = (float)RandomFloat(5.0f);
		     spells[39].attackstoptimer = 1000;
		     m_spellcheck[39] = true;


		     spells[40].info = dbcSpell.LookupEntry(10915);
		     spells[40].reqlvl = 44;
		     spells[40].hpreqtocast = 50;
		     spells[40].targettype = TARGET_SELF;
		     spells[40].instant = false;
		     spells[40].perctrigger = (float)RandomFloat(5.0f);
		     spells[40].attackstoptimer = 1000;
		     m_spellcheck[40] = true;


		     spells[41].info = dbcSpell.LookupEntry(10916);
		     spells[41].reqlvl = 50;
		     spells[41].hpreqtocast = 50;
		     spells[41].targettype = TARGET_SELF;
		     spells[41].instant = false;
		     spells[41].perctrigger = (float)RandomFloat(5.0f);
		     spells[41].attackstoptimer = 1000;
		     m_spellcheck[41] = true;


		     spells[42].info = dbcSpell.LookupEntry(10917);
		     spells[42].reqlvl = 56;
		     spells[42].hpreqtocast = 50;
		     spells[42].targettype = TARGET_SELF;
		     spells[42].instant = false;
		     spells[42].perctrigger = (float)RandomFloat(5.0f);
		     spells[42].attackstoptimer = 1000;
		     m_spellcheck[42] = true;


		     spells[43].info = dbcSpell.LookupEntry(139);
		     spells[43].reqlvl = 8;
		     spells[43].hpreqtocast = 50;
		     spells[43].targettype = TARGET_SELF;
		     spells[43].instant = false;
		     spells[43].perctrigger = (float)RandomFloat(5.0f);
		     spells[43].attackstoptimer = 1000;
		     m_spellcheck[43] = true;


		     spells[44].info = dbcSpell.LookupEntry(6074);
		     spells[44].reqlvl = 14;
		     spells[44].hpreqtocast = 50;
		     spells[44].targettype = TARGET_SELF;
		     spells[44].instant = false;
		     spells[44].perctrigger = (float)RandomFloat(5.0f);
		     spells[44].attackstoptimer = 1000;
		     m_spellcheck[44] = true;


		     spells[45].info = dbcSpell.LookupEntry(6075);
		     spells[45].reqlvl = 20;
		     spells[45].hpreqtocast = 50;
		     spells[45].targettype = TARGET_SELF;
		     spells[45].instant = false;
		     spells[45].perctrigger = (float)RandomFloat(5.0f);
		     spells[45].attackstoptimer = 1000;
		     m_spellcheck[45] = true;


		     spells[46].info = dbcSpell.LookupEntry(6078);
		     spells[46].reqlvl = 38;
		     spells[46].hpreqtocast = 50;
		     spells[46].targettype = TARGET_SELF;
		     spells[46].instant = false;
		     spells[46].perctrigger = (float)RandomFloat(5.0f);
		     spells[46].attackstoptimer = 1000;
		     m_spellcheck[46] = true;


		     spells[47].info = dbcSpell.LookupEntry(10927);
		     spells[47].reqlvl = 44;
		     spells[47].hpreqtocast = 50;
		     spells[47].targettype = TARGET_SELF;
		     spells[47].instant = false;
		     spells[47].perctrigger = (float)RandomFloat(5.0f);
		     spells[47].attackstoptimer = 1000;
		     m_spellcheck[47] = true;


		     spells[48].info = dbcSpell.LookupEntry(10929);
		     spells[48].reqlvl = 56;
		     spells[48].hpreqtocast = 50;
		     spells[48].targettype = TARGET_SELF;
		     spells[48].instant = false;
		     spells[48].perctrigger = (float)RandomFloat(5.0f);
		     spells[48].attackstoptimer = 1000;
		     m_spellcheck[48] = true;

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
	      			if (_unit->GetHealthPct()<=spells[i].hpreqtocast)
		      	  {
	      			if ((spells[i].reqlvl<=_unit->GetUInt32Value(UNIT_FIELD_LEVEL)) && (spells[i].reqlvl+10>=_unit->GetUInt32Value(UNIT_FIELD_LEVEL)))
		      	  {
			      	if(m_spellcheck[i])
		      		{
				        target = _unit->GetAIInterface()->GetNextTarget();
			       	  switch(spells[i].targettype)
			    		  {
			    		    case TARGET_SELF:
				         			_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
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
	        	}
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
class HumanoidWitchDoctor : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(HumanoidWitchDoctor);
	SP_AI_Spell spells[30];
 bool m_spellcheck[30];

    HumanoidWitchDoctor(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 0;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		     spells[0].info = dbcSpell.LookupEntry(589);
		     spells[0].reqlvl = 4;
		     spells[0].hpreqtocast = 85;
		     spells[0].targettype = TARGET_ATTACKING;
		     spells[0].instant = false;
		     spells[0].perctrigger = (float)RandomFloat(5.0f);
		     spells[0].attackstoptimer = 1000;
		     m_spellcheck[0] = true;


		     spells[1].info = dbcSpell.LookupEntry(24053);
		     spells[1].reqlvl = 30;
		     spells[1].hpreqtocast = 85;
		     spells[1].targettype = TARGET_ATTACKING;
		     spells[1].instant = false;
		     spells[1].perctrigger = (float)RandomFloat(5.0f);
		     spells[1].attackstoptimer = 1000;
		     m_spellcheck[1] = true;


		     spells[2].info = dbcSpell.LookupEntry(8042);
		     spells[2].reqlvl = 4;
		     spells[2].hpreqtocast = 85;
		     spells[2].targettype = TARGET_ATTACKING;
		     spells[2].instant = false;
		     spells[2].perctrigger = (float)RandomFloat(5.0f);
		     spells[2].attackstoptimer = 1000;
		     m_spellcheck[2] = true;


		     spells[3].info = dbcSpell.LookupEntry(594);
		     spells[3].reqlvl = 10;
		     spells[3].hpreqtocast = 85;
		     spells[3].targettype = TARGET_ATTACKING;
		     spells[3].instant = false;
		     spells[3].perctrigger = (float)RandomFloat(5.0f);
		     spells[3].attackstoptimer = 1000;
		     m_spellcheck[3] = true;


		     spells[4].info = dbcSpell.LookupEntry(8092);
		     spells[4].reqlvl = 10;
		     spells[4].hpreqtocast = 85;
		     spells[4].targettype = TARGET_ATTACKING;
		     spells[4].instant = false;
		     spells[4].perctrigger = (float)RandomFloat(5.0f);
		     spells[4].attackstoptimer = 1000;
		     m_spellcheck[4] = true;


		     spells[5].info = dbcSpell.LookupEntry(24053);
		     spells[5].reqlvl = 30;
		     spells[5].hpreqtocast = 85;
		     spells[5].targettype = TARGET_ATTACKING;
		     spells[5].instant = false;
		     spells[5].perctrigger = (float)RandomFloat(5.0f);
		     spells[5].attackstoptimer = 1000;
		     m_spellcheck[5] = true;


		     spells[6].info = dbcSpell.LookupEntry(8044);
		     spells[6].reqlvl = 8;
		     spells[6].hpreqtocast = 85;
		     spells[6].targettype = TARGET_ATTACKING;
		     spells[6].instant = false;
		     spells[6].perctrigger = (float)RandomFloat(5.0f);
		     spells[6].attackstoptimer = 1000;
		     m_spellcheck[6] = true;


		     spells[7].info = dbcSpell.LookupEntry(8277);
		     spells[7].reqlvl = 4;
		     spells[7].hpreqtocast = 85;
		     spells[7].targettype = TARGET_ATTACKING;
		     spells[7].instant = false;
		     spells[7].perctrigger = (float)RandomFloat(5.0f);
		     spells[7].attackstoptimer = 1000;
		     m_spellcheck[7] = true;


		     spells[8].info = dbcSpell.LookupEntry(970);
		     spells[8].reqlvl = 18;
		     spells[8].hpreqtocast = 85;
		     spells[8].targettype = TARGET_ATTACKING;
		     spells[8].instant = false;
		     spells[8].perctrigger = (float)RandomFloat(5.0f);
		     spells[8].attackstoptimer = 1000;
		     m_spellcheck[8] = true;


		     spells[9].info = dbcSpell.LookupEntry(8102);
		     spells[9].reqlvl = 16;
		     spells[9].hpreqtocast = 85;
		     spells[9].targettype = TARGET_ATTACKING;
		     spells[9].instant = false;
		     spells[9].perctrigger = (float)RandomFloat(5.0f);
		     spells[9].attackstoptimer = 1000;
		     m_spellcheck[9] = true;


		     spells[10].info = dbcSpell.LookupEntry(8045);
		     spells[10].reqlvl = 14;
		     spells[10].hpreqtocast = 85;
		     spells[10].targettype = TARGET_ATTACKING;
		     spells[10].instant = false;
		     spells[10].perctrigger = (float)RandomFloat(5.0f);
		     spells[10].attackstoptimer = 1000;
		     m_spellcheck[10] = true;


		     spells[11].info = dbcSpell.LookupEntry(992);
		     spells[11].reqlvl = 26;
		     spells[11].hpreqtocast = 85;
		     spells[11].targettype = TARGET_ATTACKING;
		     spells[11].instant = false;
		     spells[11].perctrigger = (float)RandomFloat(5.0f);
		     spells[11].attackstoptimer = 1000;
		     m_spellcheck[11] = true;


		     spells[12].info = dbcSpell.LookupEntry(8104);
		     spells[12].reqlvl = 28;
		     spells[12].hpreqtocast = 85;
		     spells[12].targettype = TARGET_ATTACKING;
		     spells[12].instant = false;
		     spells[12].perctrigger = (float)RandomFloat(5.0f);
		     spells[12].attackstoptimer = 1000;
		     m_spellcheck[12] = true;


		     spells[13].info = dbcSpell.LookupEntry(8046);
		     spells[13].reqlvl = 24;
		     spells[13].hpreqtocast = 85;
		     spells[13].targettype = TARGET_ATTACKING;
		     spells[13].instant = false;
		     spells[13].perctrigger = (float)RandomFloat(5.0f);
		     spells[13].attackstoptimer = 1000;
		     m_spellcheck[13] = true;


		     spells[14].info = dbcSpell.LookupEntry(2767);
		     spells[14].reqlvl = 34;
		     spells[14].hpreqtocast = 85;
		     spells[14].targettype = TARGET_ATTACKING;
		     spells[14].instant = false;
		     spells[14].perctrigger = (float)RandomFloat(5.0f);
		     spells[14].attackstoptimer = 1000;
		     m_spellcheck[14] = true;


		     spells[15].info = dbcSpell.LookupEntry(8105);
		     spells[15].reqlvl = 34;
		     spells[15].hpreqtocast = 85;
		     spells[15].targettype = TARGET_ATTACKING;
		     spells[15].instant = false;
		     spells[15].perctrigger = (float)RandomFloat(5.0f);
		     spells[15].attackstoptimer = 1000;
		     m_spellcheck[15] = true;


		     spells[16].info = dbcSpell.LookupEntry(10412);
		     spells[16].reqlvl = 36;
		     spells[16].hpreqtocast = 85;
		     spells[16].targettype = TARGET_ATTACKING;
		     spells[16].instant = false;
		     spells[16].perctrigger = (float)RandomFloat(5.0f);
		     spells[16].attackstoptimer = 1000;
		     m_spellcheck[16] = true;


		     spells[17].info = dbcSpell.LookupEntry(10892);
		     spells[17].reqlvl = 42;
		     spells[17].hpreqtocast = 85;
		     spells[17].targettype = TARGET_ATTACKING;
		     spells[17].instant = false;
		     spells[17].perctrigger = (float)RandomFloat(5.0f);
		     spells[17].attackstoptimer = 1000;
		     m_spellcheck[17] = true;


		     spells[18].info = dbcSpell.LookupEntry(10945);
		     spells[18].reqlvl = 46;
		     spells[18].hpreqtocast = 85;
		     spells[18].targettype = TARGET_ATTACKING;
		     spells[18].instant = false;
		     spells[18].perctrigger = (float)RandomFloat(5.0f);
		     spells[18].attackstoptimer = 1000;
		     m_spellcheck[18] = true;


		     spells[19].info = dbcSpell.LookupEntry(10413);
		     spells[19].reqlvl = 48;
		     spells[19].hpreqtocast = 85;
		     spells[19].targettype = TARGET_ATTACKING;
		     spells[19].instant = false;
		     spells[19].perctrigger = (float)RandomFloat(5.0f);
		     spells[19].attackstoptimer = 1000;
		     m_spellcheck[19] = true;


		     spells[20].info = dbcSpell.LookupEntry(10894);
		     spells[20].reqlvl = 58;
		     spells[20].hpreqtocast = 85;
		     spells[20].targettype = TARGET_ATTACKING;
		     spells[20].instant = false;
		     spells[20].perctrigger = (float)RandomFloat(5.0f);
		     spells[20].attackstoptimer = 1000;
		     m_spellcheck[20] = true;


		     spells[21].info = dbcSpell.LookupEntry(10947);
		     spells[21].reqlvl = 58;
		     spells[21].hpreqtocast = 85;
		     spells[21].targettype = TARGET_ATTACKING;
		     spells[21].instant = false;
		     spells[21].perctrigger = (float)RandomFloat(5.0f);
		     spells[21].attackstoptimer = 1000;
		     m_spellcheck[21] = true;


		     spells[22].info = dbcSpell.LookupEntry(10414);
		     spells[22].reqlvl = 60;
		     spells[22].hpreqtocast = 85;
		     spells[22].targettype = TARGET_ATTACKING;
		     spells[22].instant = false;
		     spells[22].perctrigger = (float)RandomFloat(5.0f);
		     spells[22].attackstoptimer = 1000;
		     m_spellcheck[22] = true;


		     spells[23].info = dbcSpell.LookupEntry(331);
		     spells[23].reqlvl = 1;
		     spells[23].hpreqtocast = 50;
		     spells[23].targettype = TARGET_SELF;
		     spells[23].instant = false;
		     spells[23].perctrigger = (float)RandomFloat(5.0f);
		     spells[23].attackstoptimer = 1000;
		     m_spellcheck[23] = true;


		     spells[24].info = dbcSpell.LookupEntry(332);
		     spells[24].reqlvl = 6;
		     spells[24].hpreqtocast = 50;
		     spells[24].targettype = TARGET_SELF;
		     spells[24].instant = false;
		     spells[24].perctrigger = (float)RandomFloat(5.0f);
		     spells[24].attackstoptimer = 1000;
		     m_spellcheck[24] = true;


		     spells[25].info = dbcSpell.LookupEntry(8004);
		     spells[25].reqlvl = 20;
		     spells[25].hpreqtocast = 50;
		     spells[25].targettype = TARGET_SELF;
		     spells[25].instant = false;
		     spells[25].perctrigger = (float)RandomFloat(5.0f);
		     spells[25].attackstoptimer = 1000;
		     m_spellcheck[25] = true;


		     spells[26].info = dbcSpell.LookupEntry(8008);
		     spells[26].reqlvl = 28;
		     spells[26].hpreqtocast = 50;
		     spells[26].targettype = TARGET_SELF;
		     spells[26].instant = false;
		     spells[26].perctrigger = (float)RandomFloat(5.0f);
		     spells[26].attackstoptimer = 1000;
		     m_spellcheck[26] = true;


		     spells[27].info = dbcSpell.LookupEntry(8010);
		     spells[27].reqlvl = 36;
		     spells[27].hpreqtocast = 50;
		     spells[27].targettype = TARGET_SELF;
		     spells[27].instant = false;
		     spells[27].perctrigger = (float)RandomFloat(5.0f);
		     spells[27].attackstoptimer = 1000;
		     m_spellcheck[27] = true;


		     spells[28].info = dbcSpell.LookupEntry(10466);
		     spells[28].reqlvl = 44;
		     spells[28].hpreqtocast = 50;
		     spells[28].targettype = TARGET_SELF;
		     spells[28].instant = false;
		     spells[28].perctrigger = (float)RandomFloat(5.0f);
		     spells[28].attackstoptimer = 1000;
		     m_spellcheck[28] = true;


		     spells[29].info = dbcSpell.LookupEntry(10468);
		     spells[29].reqlvl = 60;
		     spells[29].hpreqtocast = 50;
		     spells[29].targettype = TARGET_SELF;
		     spells[29].instant = false;
		     spells[29].perctrigger = (float)RandomFloat(5.0f);
		     spells[29].attackstoptimer = 1000;
		     m_spellcheck[29] = true;

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
	      			if (spells[i].hpreqtocast<=_unit->GetHealthPct())
		      	  {
	      			if ((spells[i].reqlvl<=_unit->GetUInt32Value(UNIT_FIELD_LEVEL)) && (spells[i].reqlvl+10>=_unit->GetUInt32Value(UNIT_FIELD_LEVEL)))
		      	  {
			      	if(m_spellcheck[i])
		      		{
				        target = _unit->GetAIInterface()->GetNextTarget();
			       	  switch(spells[i].targettype)
			    		  {
			    		    case TARGET_SELF:
				         			_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
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
	        	}
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

class HumanoidShadowPriest : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(HumanoidShadowPriest);
	SP_AI_Spell spells[27];
 bool m_spellcheck[27];

    HumanoidShadowPriest(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 27;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}


		     spells[0].info = dbcSpell.LookupEntry(589);
		     spells[0].reqlvl = 4;
		     spells[0].hpreqtocast = 85;
		     spells[0].targettype = TARGET_ATTACKING;
		     spells[0].instant = false;
		     spells[0].perctrigger = (float)RandomFloat(5.0f);
		     spells[0].attackstoptimer = 1000;
		     m_spellcheck[0] = true;


		     spells[1].info = dbcSpell.LookupEntry(15407);
		     spells[1].reqlvl = 20;
		     spells[1].hpreqtocast = 85;
		     spells[1].targettype = TARGET_ATTACKING;
		     spells[1].instant = false;
		     spells[1].perctrigger = (float)RandomFloat(5.0f);
		     spells[1].attackstoptimer = 1000;
		     m_spellcheck[1] = true;


		     spells[2].info = dbcSpell.LookupEntry(594);
		     spells[2].reqlvl = 10;
		     spells[2].hpreqtocast = 85;
		     spells[2].targettype = TARGET_ATTACKING;
		     spells[2].instant = false;
		     spells[2].perctrigger = (float)RandomFloat(5.0f);
		     spells[2].attackstoptimer = 1000;
		     m_spellcheck[2] = true;


		     spells[3].info = dbcSpell.LookupEntry(8092);
		     spells[3].reqlvl = 10;
		     spells[3].hpreqtocast = 85;
		     spells[3].targettype = TARGET_ATTACKING;
		     spells[3].instant = false;
		     spells[3].perctrigger = (float)RandomFloat(5.0f);
		     spells[3].attackstoptimer = 1000;
		     m_spellcheck[3] = true;


		     spells[4].info = dbcSpell.LookupEntry(17311);
		     spells[4].reqlvl = 28;
		     spells[4].hpreqtocast = 85;
		     spells[4].targettype = TARGET_ATTACKING;
		     spells[4].instant = false;
		     spells[4].perctrigger = (float)RandomFloat(5.0f);
		     spells[4].attackstoptimer = 1000;
		     m_spellcheck[4] = true;


		     spells[5].info = dbcSpell.LookupEntry(970);
		     spells[5].reqlvl = 18;
		     spells[5].hpreqtocast = 85;
		     spells[5].targettype = TARGET_ATTACKING;
		     spells[5].instant = false;
		     spells[5].perctrigger = (float)RandomFloat(5.0f);
		     spells[5].attackstoptimer = 1000;
		     m_spellcheck[5] = true;


		     spells[6].info = dbcSpell.LookupEntry(8102);
		     spells[6].reqlvl = 16;
		     spells[6].hpreqtocast = 85;
		     spells[6].targettype = TARGET_ATTACKING;
		     spells[6].instant = false;
		     spells[6].perctrigger = (float)RandomFloat(5.0f);
		     spells[6].attackstoptimer = 1000;
		     m_spellcheck[6] = true;


		     spells[7].info = dbcSpell.LookupEntry(8122);
		     spells[7].reqlvl = 14;
		     spells[7].hpreqtocast = 85;
		     spells[7].targettype = TARGET_ATTACKING;
		     spells[7].instant = false;
		     spells[7].perctrigger = (float)RandomFloat(5.0f);
		     spells[7].attackstoptimer = 1000;
		     m_spellcheck[7] = true;


		     spells[8].info = dbcSpell.LookupEntry(15487);
		     spells[8].reqlvl = 0;
		     spells[8].hpreqtocast = 85;
		     spells[8].targettype = TARGET_ATTACKING;
		     spells[8].instant = false;
		     spells[8].perctrigger = (float)RandomFloat(5.0f);
		     spells[8].attackstoptimer = 1000;
		     m_spellcheck[8] = true;


		     spells[9].info = dbcSpell.LookupEntry(17312);
		     spells[9].reqlvl = 36;
		     spells[9].hpreqtocast = 85;
		     spells[9].targettype = TARGET_ATTACKING;
		     spells[9].instant = false;
		     spells[9].perctrigger = (float)RandomFloat(5.0f);
		     spells[9].attackstoptimer = 1000;
		     m_spellcheck[9] = true;


		     spells[10].info = dbcSpell.LookupEntry(992);
		     spells[10].reqlvl = 26;
		     spells[10].hpreqtocast = 85;
		     spells[10].targettype = TARGET_ATTACKING;
		     spells[10].instant = false;
		     spells[10].perctrigger = (float)RandomFloat(5.0f);
		     spells[10].attackstoptimer = 1000;
		     m_spellcheck[10] = true;


		     spells[11].info = dbcSpell.LookupEntry(984);
		     spells[11].reqlvl = 22;
		     spells[11].hpreqtocast = 85;
		     spells[11].targettype = TARGET_ATTACKING;
		     spells[11].instant = false;
		     spells[11].perctrigger = (float)RandomFloat(5.0f);
		     spells[11].attackstoptimer = 1000;
		     m_spellcheck[11] = true;


		     spells[12].info = dbcSpell.LookupEntry(8103);
		     spells[12].reqlvl = 22;
		     spells[12].hpreqtocast = 85;
		     spells[12].targettype = TARGET_ATTACKING;
		     spells[12].instant = false;
		     spells[12].perctrigger = (float)RandomFloat(5.0f);
		     spells[12].attackstoptimer = 1000;
		     m_spellcheck[12] = true;


		     spells[13].info = dbcSpell.LookupEntry(8104);
		     spells[13].reqlvl = 28;
		     spells[13].hpreqtocast = 85;
		     spells[13].targettype = TARGET_ATTACKING;
		     spells[13].instant = false;
		     spells[13].perctrigger = (float)RandomFloat(5.0f);
		     spells[13].attackstoptimer = 1000;
		     m_spellcheck[13] = true;


		     spells[14].info = dbcSpell.LookupEntry(8122);
		     spells[14].reqlvl = 14;
		     spells[14].hpreqtocast = 85;
		     spells[14].targettype = TARGET_ATTACKING;
		     spells[14].instant = false;
		     spells[14].perctrigger = (float)RandomFloat(5.0f);
		     spells[14].attackstoptimer = 1000;
		     m_spellcheck[14] = true;


		     spells[15].info = dbcSpell.LookupEntry(17313);
		     spells[15].reqlvl = 44;
		     spells[15].hpreqtocast = 85;
		     spells[15].targettype = TARGET_ATTACKING;
		     spells[15].instant = false;
		     spells[15].perctrigger = (float)RandomFloat(5.0f);
		     spells[15].attackstoptimer = 1000;
		     m_spellcheck[15] = true;


		     spells[16].info = dbcSpell.LookupEntry(2767);
		     spells[16].reqlvl = 34;
		     spells[16].hpreqtocast = 85;
		     spells[16].targettype = TARGET_ATTACKING;
		     spells[16].instant = false;
		     spells[16].perctrigger = (float)RandomFloat(5.0f);
		     spells[16].attackstoptimer = 1000;
		     m_spellcheck[16] = true;


		     spells[17].info = dbcSpell.LookupEntry(8105);
		     spells[17].reqlvl = 34;
		     spells[17].hpreqtocast = 85;
		     spells[17].targettype = TARGET_ATTACKING;
		     spells[17].instant = false;
		     spells[17].perctrigger = (float)RandomFloat(5.0f);
		     spells[17].attackstoptimer = 1000;
		     m_spellcheck[17] = true;


		     spells[18].info = dbcSpell.LookupEntry(17314);
		     spells[18].reqlvl = 52;
		     spells[18].hpreqtocast = 85;
		     spells[18].targettype = TARGET_ATTACKING;
		     spells[18].instant = false;
		     spells[18].perctrigger = (float)RandomFloat(5.0f);
		     spells[18].attackstoptimer = 1000;
		     m_spellcheck[18] = true;


		     spells[19].info = dbcSpell.LookupEntry(10892);
		     spells[19].reqlvl = 42;
		     spells[19].hpreqtocast = 85;
		     spells[19].targettype = TARGET_ATTACKING;
		     spells[19].instant = false;
		     spells[19].perctrigger = (float)RandomFloat(5.0f);
		     spells[19].attackstoptimer = 1000;
		     m_spellcheck[19] = true;


		     spells[20].info = dbcSpell.LookupEntry(8106);
		     spells[20].reqlvl = 40;
		     spells[20].hpreqtocast = 85;
		     spells[20].targettype = TARGET_ATTACKING;
		     spells[20].instant = false;
		     spells[20].perctrigger = (float)RandomFloat(5.0f);
		     spells[20].attackstoptimer = 1000;
		     m_spellcheck[20] = true;


		     spells[21].info = dbcSpell.LookupEntry(10945);
		     spells[21].reqlvl = 46;
		     spells[21].hpreqtocast = 85;
		     spells[21].targettype = TARGET_ATTACKING;
		     spells[21].instant = false;
		     spells[21].perctrigger = (float)RandomFloat(5.0f);
		     spells[21].attackstoptimer = 1000;
		     m_spellcheck[21] = true;


		     spells[22].info = dbcSpell.LookupEntry(18807);
		     spells[22].reqlvl = 60;
		     spells[22].hpreqtocast = 85;
		     spells[22].targettype = TARGET_ATTACKING;
		     spells[22].instant = false;
		     spells[22].perctrigger = (float)RandomFloat(5.0f);
		     spells[22].attackstoptimer = 1000;
		     m_spellcheck[22] = true;


		     spells[23].info = dbcSpell.LookupEntry(10893);
		     spells[23].reqlvl = 50;
		     spells[23].hpreqtocast = 85;
		     spells[23].targettype = TARGET_ATTACKING;
		     spells[23].instant = false;
		     spells[23].perctrigger = (float)RandomFloat(5.0f);
		     spells[23].attackstoptimer = 1000;
		     m_spellcheck[23] = true;


		     spells[24].info = dbcSpell.LookupEntry(10894);
		     spells[24].reqlvl = 58;
		     spells[24].hpreqtocast = 85;
		     spells[24].targettype = TARGET_ATTACKING;
		     spells[24].instant = false;
		     spells[24].perctrigger = (float)RandomFloat(5.0f);
		     spells[24].attackstoptimer = 1000;
		     m_spellcheck[24] = true;


		     spells[25].info = dbcSpell.LookupEntry(10946);
		     spells[25].reqlvl = 52;
		     spells[25].hpreqtocast = 85;
		     spells[25].targettype = TARGET_ATTACKING;
		     spells[25].instant = false;
		     spells[25].perctrigger = (float)RandomFloat(5.0f);
		     spells[25].attackstoptimer = 1000;
		     m_spellcheck[25] = true;


		     spells[26].info = dbcSpell.LookupEntry(10947);
		     spells[26].reqlvl = 58;
		     spells[26].hpreqtocast = 85;
		     spells[26].targettype = TARGET_ATTACKING;
		     spells[26].instant = false;
		     spells[26].perctrigger = (float)RandomFloat(5.0f);
		     spells[26].attackstoptimer = 1000;
		     m_spellcheck[26] = true;

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
	      			if (spells[i].hpreqtocast<=_unit->GetHealthPct())
		      	  {
	      			if ((spells[i].reqlvl<=_unit->GetUInt32Value(UNIT_FIELD_LEVEL)) && (spells[i].reqlvl+10>=_unit->GetUInt32Value(UNIT_FIELD_LEVEL)))
		      	  {
			      	if(m_spellcheck[i])
		      		{
				        target = _unit->GetAIInterface()->GetNextTarget();
			       	  switch(spells[i].targettype)
			    		  {
			    		    case TARGET_SELF:
				         			_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
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
	        	}
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

class HumanoidHolyPriest : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(HumanoidHolyPriest);
	SP_AI_Spell spells[37];
 bool m_spellcheck[37];

    HumanoidHolyPriest(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 37;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		     spells[0].info = dbcSpell.LookupEntry(585);
		     spells[0].reqlvl = 1;
		     spells[0].hpreqtocast = 85;
		     spells[0].targettype = TARGET_ATTACKING;
		     spells[0].instant = false;
		     spells[0].perctrigger = (float)RandomFloat(5.0f);
		     spells[0].attackstoptimer = 1000;
		     m_spellcheck[0] = true;


		     spells[1].info = dbcSpell.LookupEntry(591);
		     spells[1].reqlvl = 6;
		     spells[1].hpreqtocast = 85;
		     spells[1].targettype = TARGET_ATTACKING;
		     spells[1].instant = false;
		     spells[1].perctrigger = (float)RandomFloat(5.0f);
		     spells[1].attackstoptimer = 1000;
		     m_spellcheck[1] = true;


		     spells[2].info = dbcSpell.LookupEntry(598);
		     spells[2].reqlvl = 14;
		     spells[2].hpreqtocast = 85;
		     spells[2].targettype = TARGET_ATTACKING;
		     spells[2].instant = false;
		     spells[2].perctrigger = (float)RandomFloat(5.0f);
		     spells[2].attackstoptimer = 1000;
		     m_spellcheck[2] = true;


		     spells[3].info = dbcSpell.LookupEntry(15237);
		     spells[3].reqlvl = 20;
		     spells[3].hpreqtocast = 85;
		     spells[3].targettype = TARGET_ATTACKING;
		     spells[3].instant = false;
		     spells[3].perctrigger = (float)RandomFloat(5.0f);
		     spells[3].attackstoptimer = 1000;
		     m_spellcheck[3] = true;


		     spells[4].info = dbcSpell.LookupEntry(15262);
		     spells[4].reqlvl = 24;
		     spells[4].hpreqtocast = 85;
		     spells[4].targettype = TARGET_ATTACKING;
		     spells[4].instant = false;
		     spells[4].perctrigger = (float)RandomFloat(5.0f);
		     spells[4].attackstoptimer = 1000;
		     m_spellcheck[4] = true;


		     spells[5].info = dbcSpell.LookupEntry(984);
		     spells[5].reqlvl = 22;
		     spells[5].hpreqtocast = 85;
		     spells[5].targettype = TARGET_ATTACKING;
		     spells[5].instant = false;
		     spells[5].perctrigger = (float)RandomFloat(5.0f);
		     spells[5].attackstoptimer = 1000;
		     m_spellcheck[5] = true;


		     spells[6].info = dbcSpell.LookupEntry(15430);
		     spells[6].reqlvl = 28;
		     spells[6].hpreqtocast = 85;
		     spells[6].targettype = TARGET_ATTACKING;
		     spells[6].instant = false;
		     spells[6].perctrigger = (float)RandomFloat(5.0f);
		     spells[6].attackstoptimer = 1000;
		     m_spellcheck[6] = true;


		     spells[7].info = dbcSpell.LookupEntry(15264);
		     spells[7].reqlvl = 36;
		     spells[7].hpreqtocast = 85;
		     spells[7].targettype = TARGET_ATTACKING;
		     spells[7].instant = false;
		     spells[7].perctrigger = (float)RandomFloat(5.0f);
		     spells[7].attackstoptimer = 1000;
		     m_spellcheck[7] = true;


		     spells[8].info = dbcSpell.LookupEntry(1004);
		     spells[8].reqlvl = 30;
		     spells[8].hpreqtocast = 85;
		     spells[8].targettype = TARGET_ATTACKING;
		     spells[8].instant = false;
		     spells[8].perctrigger = (float)RandomFloat(5.0f);
		     spells[8].attackstoptimer = 1000;
		     m_spellcheck[8] = true;


		     spells[9].info = dbcSpell.LookupEntry(15431);
		     spells[9].reqlvl = 36;
		     spells[9].hpreqtocast = 85;
		     spells[9].targettype = TARGET_ATTACKING;
		     spells[9].instant = false;
		     spells[9].perctrigger = (float)RandomFloat(5.0f);
		     spells[9].attackstoptimer = 1000;
		     m_spellcheck[9] = true;


		     spells[10].info = dbcSpell.LookupEntry(15266);
		     spells[10].reqlvl = 48;
		     spells[10].hpreqtocast = 85;
		     spells[10].targettype = TARGET_ATTACKING;
		     spells[10].instant = false;
		     spells[10].perctrigger = (float)RandomFloat(5.0f);
		     spells[10].attackstoptimer = 1000;
		     m_spellcheck[10] = true;


		     spells[11].info = dbcSpell.LookupEntry(10933);
		     spells[11].reqlvl = 46;
		     spells[11].hpreqtocast = 85;
		     spells[11].targettype = TARGET_ATTACKING;
		     spells[11].instant = false;
		     spells[11].perctrigger = (float)RandomFloat(5.0f);
		     spells[11].attackstoptimer = 1000;
		     m_spellcheck[11] = true;


		     spells[12].info = dbcSpell.LookupEntry(15431);
		     spells[12].reqlvl = 36;
		     spells[12].hpreqtocast = 85;
		     spells[12].targettype = TARGET_ATTACKING;
		     spells[12].instant = false;
		     spells[12].perctrigger = (float)RandomFloat(5.0f);
		     spells[12].attackstoptimer = 1000;
		     m_spellcheck[12] = true;


		     spells[13].info = dbcSpell.LookupEntry(15267);
		     spells[13].reqlvl = 54;
		     spells[13].hpreqtocast = 85;
		     spells[13].targettype = TARGET_ATTACKING;
		     spells[13].instant = false;
		     spells[13].perctrigger = (float)RandomFloat(5.0f);
		     spells[13].attackstoptimer = 1000;
		     m_spellcheck[13] = true;


		     spells[14].info = dbcSpell.LookupEntry(10934);
		     spells[14].reqlvl = 54;
		     spells[14].hpreqtocast = 85;
		     spells[14].targettype = TARGET_ATTACKING;
		     spells[14].instant = false;
		     spells[14].perctrigger = (float)RandomFloat(5.0f);
		     spells[14].attackstoptimer = 1000;
		     m_spellcheck[14] = true;


		     spells[15].info = dbcSpell.LookupEntry(15431);
		     spells[15].reqlvl = 36;
		     spells[15].hpreqtocast = 85;
		     spells[15].targettype = TARGET_ATTACKING;
		     spells[15].instant = false;
		     spells[15].perctrigger = (float)RandomFloat(5.0f);
		     spells[15].attackstoptimer = 1000;
		     m_spellcheck[15] = true;


		     spells[16].info = dbcSpell.LookupEntry(15261);
		     spells[16].reqlvl = 60;
		     spells[16].hpreqtocast = 85;
		     spells[16].targettype = TARGET_ATTACKING;
		     spells[16].instant = false;
		     spells[16].perctrigger = (float)RandomFloat(5.0f);
		     spells[16].attackstoptimer = 1000;
		     m_spellcheck[16] = true;


		     spells[17].info = dbcSpell.LookupEntry(17);
		     spells[17].reqlvl = 6;
		     spells[17].hpreqtocast = 85;
		     spells[17].targettype = TARGET_SELF;
		     spells[17].instant = false;
		     spells[17].perctrigger = (float)RandomFloat(5.0f);
		     spells[17].attackstoptimer = 1000;
		     m_spellcheck[17] = true;


		     spells[18].info = dbcSpell.LookupEntry(592);
		     spells[18].reqlvl = 12;
		     spells[18].hpreqtocast = 85;
		     spells[18].targettype = TARGET_SELF;
		     spells[18].instant = false;
		     spells[18].perctrigger = (float)RandomFloat(5.0f);
		     spells[18].attackstoptimer = 1000;
		     m_spellcheck[18] = true;


		     spells[19].info = dbcSpell.LookupEntry(600);
		     spells[19].reqlvl = 18;
		     spells[19].hpreqtocast = 85;
		     spells[19].targettype = TARGET_SELF;
		     spells[19].instant = false;
		     spells[19].perctrigger = (float)RandomFloat(5.0f);
		     spells[19].attackstoptimer = 1000;
		     m_spellcheck[19] = true;


		     spells[20].info = dbcSpell.LookupEntry(3747);
		     spells[20].reqlvl = 24;
		     spells[20].hpreqtocast = 85;
		     spells[20].targettype = TARGET_SELF;
		     spells[20].instant = false;
		     spells[20].perctrigger = (float)RandomFloat(5.0f);
		     spells[20].attackstoptimer = 1000;
		     m_spellcheck[20] = true;


		     spells[21].info = dbcSpell.LookupEntry(6066);
		     spells[21].reqlvl = 36;
		     spells[21].hpreqtocast = 85;
		     spells[21].targettype = TARGET_SELF;
		     spells[21].instant = false;
		     spells[21].perctrigger = (float)RandomFloat(5.0f);
		     spells[21].attackstoptimer = 1000;
		     m_spellcheck[21] = true;


		     spells[22].info = dbcSpell.LookupEntry(10899);
		     spells[22].reqlvl = 48;
		     spells[22].hpreqtocast = 85;
		     spells[22].targettype = TARGET_SELF;
		     spells[22].instant = false;
		     spells[22].perctrigger = (float)RandomFloat(5.0f);
		     spells[22].attackstoptimer = 1000;
		     m_spellcheck[22] = true;


		     spells[23].info = dbcSpell.LookupEntry(10900);
		     spells[23].reqlvl = 54;
		     spells[23].hpreqtocast = 85;
		     spells[23].targettype = TARGET_SELF;
		     spells[23].instant = false;
		     spells[23].perctrigger = (float)RandomFloat(5.0f);
		     spells[23].attackstoptimer = 1000;
		     m_spellcheck[23] = true;


		     spells[24].info = dbcSpell.LookupEntry(2052);
		     spells[24].reqlvl = 4;
		     spells[24].hpreqtocast = 50;
		     spells[24].targettype = TARGET_SELF;
		     spells[24].instant = false;
		     spells[24].perctrigger = (float)RandomFloat(5.0f);
		     spells[24].attackstoptimer = 1000;
		     m_spellcheck[24] = true;


		     spells[25].info = dbcSpell.LookupEntry(2053);
		     spells[25].reqlvl = 10;
		     spells[25].hpreqtocast = 50;
		     spells[25].targettype = TARGET_SELF;
		     spells[25].instant = false;
		     spells[25].perctrigger = (float)RandomFloat(5.0f);
		     spells[25].attackstoptimer = 1000;
		     m_spellcheck[25] = true;


		     spells[26].info = dbcSpell.LookupEntry(9472);
		     spells[26].reqlvl = 26;
		     spells[26].hpreqtocast = 50;
		     spells[26].targettype = TARGET_SELF;
		     spells[26].instant = false;
		     spells[26].perctrigger = (float)RandomFloat(5.0f);
		     spells[26].attackstoptimer = 1000;
		     m_spellcheck[26] = true;


		     spells[27].info = dbcSpell.LookupEntry(9474);
		     spells[27].reqlvl = 38;
		     spells[27].hpreqtocast = 50;
		     spells[27].targettype = TARGET_SELF;
		     spells[27].instant = false;
		     spells[27].perctrigger = (float)RandomFloat(5.0f);
		     spells[27].attackstoptimer = 1000;
		     m_spellcheck[27] = true;


		     spells[28].info = dbcSpell.LookupEntry(10915);
		     spells[28].reqlvl = 44;
		     spells[28].hpreqtocast = 50;
		     spells[28].targettype = TARGET_SELF;
		     spells[28].instant = false;
		     spells[28].perctrigger = (float)RandomFloat(5.0f);
		     spells[28].attackstoptimer = 1000;
		     m_spellcheck[28] = true;


		     spells[29].info = dbcSpell.LookupEntry(10916);
		     spells[29].reqlvl = 50;
		     spells[29].hpreqtocast = 50;
		     spells[29].targettype = TARGET_SELF;
		     spells[29].instant = false;
		     spells[29].perctrigger = (float)RandomFloat(5.0f);
		     spells[29].attackstoptimer = 1000;
		     m_spellcheck[29] = true;


		     spells[30].info = dbcSpell.LookupEntry(10917);
		     spells[30].reqlvl = 56;
		     spells[30].hpreqtocast = 50;
		     spells[30].targettype = TARGET_SELF;
		     spells[30].instant = false;
		     spells[30].perctrigger = (float)RandomFloat(5.0f);
		     spells[30].attackstoptimer = 1000;
		     m_spellcheck[30] = true;


		     spells[31].info = dbcSpell.LookupEntry(139);
		     spells[31].reqlvl = 8;
		     spells[31].hpreqtocast = 50;
		     spells[31].targettype = TARGET_SELF;
		     spells[31].instant = false;
		     spells[31].perctrigger = (float)RandomFloat(5.0f);
		     spells[31].attackstoptimer = 1000;
		     m_spellcheck[31] = true;


		     spells[32].info = dbcSpell.LookupEntry(6074);
		     spells[32].reqlvl = 14;
		     spells[32].hpreqtocast = 50;
		     spells[32].targettype = TARGET_SELF;
		     spells[32].instant = false;
		     spells[32].perctrigger = (float)RandomFloat(5.0f);
		     spells[32].attackstoptimer = 1000;
		     m_spellcheck[32] = true;


		     spells[33].info = dbcSpell.LookupEntry(6075);
		     spells[33].reqlvl = 20;
		     spells[33].hpreqtocast = 50;
		     spells[33].targettype = TARGET_SELF;
		     spells[33].instant = false;
		     spells[33].perctrigger = (float)RandomFloat(5.0f);
		     spells[33].attackstoptimer = 1000;
		     m_spellcheck[33] = true;


		     spells[34].info = dbcSpell.LookupEntry(6078);
		     spells[34].reqlvl = 38;
		     spells[34].hpreqtocast = 50;
		     spells[34].targettype = TARGET_SELF;
		     spells[34].instant = false;
		     spells[34].perctrigger = (float)RandomFloat(5.0f);
		     spells[34].attackstoptimer = 1000;
		     m_spellcheck[34] = true;


		     spells[35].info = dbcSpell.LookupEntry(10927);
		     spells[35].reqlvl = 44;
		     spells[35].hpreqtocast = 50;
		     spells[35].targettype = TARGET_SELF;
		     spells[35].instant = false;
		     spells[35].perctrigger = (float)RandomFloat(5.0f);
		     spells[35].attackstoptimer = 1000;
		     m_spellcheck[35] = true;


		     spells[36].info = dbcSpell.LookupEntry(10929);
		     spells[36].reqlvl = 56;
		     spells[36].hpreqtocast = 50;
		     spells[36].targettype = TARGET_SELF;
		     spells[36].instant = false;
		     spells[36].perctrigger = (float)RandomFloat(5.0f);
		     spells[36].attackstoptimer = 1000;
		     m_spellcheck[36] = true;

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
	      			if (spells[i].hpreqtocast<=_unit->GetHealthPct())
		      	  {
	      			if ((spells[i].reqlvl<=_unit->GetUInt32Value(UNIT_FIELD_LEVEL)) && (spells[i].reqlvl+10>=_unit->GetUInt32Value(UNIT_FIELD_LEVEL)))
		      	  {
			      	if(m_spellcheck[i])
		      		{
				        target = _unit->GetAIInterface()->GetNextTarget();
			       	  switch(spells[i].targettype)
			    		  {
			    		    case TARGET_SELF:
				         			_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
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
	        	}
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

class HumanoidDruid : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(HumanoidDruid);
	SP_AI_Spell spells[68];
 bool m_spellcheck[68];

    HumanoidDruid(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 68;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		     spells[0].info = dbcSpell.LookupEntry(5176);
		     spells[0].reqlvl = 1;
		     spells[0].hpreqtocast = 85;
		     spells[0].targettype = TARGET_ATTACKING;
		     spells[0].instant = false;
		     spells[0].perctrigger = (float)RandomFloat(5.0f);
		     spells[0].attackstoptimer = 1000;
		     m_spellcheck[0] = true;


		     spells[1].info = dbcSpell.LookupEntry(8921);
		     spells[1].reqlvl = 4;
		     spells[1].hpreqtocast = 85;
		     spells[1].targettype = TARGET_ATTACKING;
		     spells[1].instant = false;
		     spells[1].perctrigger = (float)RandomFloat(5.0f);
		     spells[1].attackstoptimer = 1000;
		     m_spellcheck[1] = true;


		     spells[2].info = dbcSpell.LookupEntry(5177);
		     spells[2].reqlvl = 6;
		     spells[2].hpreqtocast = 85;
		     spells[2].targettype = TARGET_ATTACKING;
		     spells[2].instant = false;
		     spells[2].perctrigger = (float)RandomFloat(5.0f);
		     spells[2].attackstoptimer = 1000;
		     m_spellcheck[2] = true;


		     spells[3].info = dbcSpell.LookupEntry(8924);
		     spells[3].reqlvl = 10;
		     spells[3].hpreqtocast = 85;
		     spells[3].targettype = TARGET_ATTACKING;
		     spells[3].instant = false;
		     spells[3].perctrigger = (float)RandomFloat(5.0f);
		     spells[3].attackstoptimer = 1000;
		     m_spellcheck[3] = true;


		     spells[4].info = dbcSpell.LookupEntry(5178);
		     spells[4].reqlvl = 14;
		     spells[4].hpreqtocast = 85;
		     spells[4].targettype = TARGET_ATTACKING;
		     spells[4].instant = false;
		     spells[4].perctrigger = (float)RandomFloat(5.0f);
		     spells[4].attackstoptimer = 1000;
		     m_spellcheck[4] = true;


		     spells[5].info = dbcSpell.LookupEntry(8925);
		     spells[5].reqlvl = 16;
		     spells[5].hpreqtocast = 85;
		     spells[5].targettype = TARGET_ATTACKING;
		     spells[5].instant = false;
		     spells[5].perctrigger = (float)RandomFloat(5.0f);
		     spells[5].attackstoptimer = 1000;
		     m_spellcheck[5] = true;


		     spells[6].info = dbcSpell.LookupEntry(770);
		     spells[6].reqlvl = 18;
		     spells[6].hpreqtocast = 85;
		     spells[6].targettype = TARGET_ATTACKING;
		     spells[6].instant = false;
		     spells[6].perctrigger = (float)RandomFloat(5.0f);
		     spells[6].attackstoptimer = 1000;
		     m_spellcheck[6] = true;


		     spells[7].info = dbcSpell.LookupEntry(5570);
		     spells[7].reqlvl = 20;
		     spells[7].hpreqtocast = 85;
		     spells[7].targettype = TARGET_ATTACKING;
		     spells[7].instant = false;
		     spells[7].perctrigger = (float)RandomFloat(5.0f);
		     spells[7].attackstoptimer = 1000;
		     m_spellcheck[7] = true;


		     spells[8].info = dbcSpell.LookupEntry(2912);
		     spells[8].reqlvl = 20;
		     spells[8].hpreqtocast = 85;
		     spells[8].targettype = TARGET_ATTACKING;
		     spells[8].instant = false;
		     spells[8].perctrigger = (float)RandomFloat(5.0f);
		     spells[8].attackstoptimer = 1000;
		     m_spellcheck[8] = true;


		     spells[9].info = dbcSpell.LookupEntry(5179);
		     spells[9].reqlvl = 22;
		     spells[9].hpreqtocast = 85;
		     spells[9].targettype = TARGET_ATTACKING;
		     spells[9].instant = false;
		     spells[9].perctrigger = (float)RandomFloat(5.0f);
		     spells[9].attackstoptimer = 1000;
		     m_spellcheck[9] = true;


		     spells[10].info = dbcSpell.LookupEntry(8926);
		     spells[10].reqlvl = 22;
		     spells[10].hpreqtocast = 85;
		     spells[10].targettype = TARGET_ATTACKING;
		     spells[10].instant = false;
		     spells[10].perctrigger = (float)RandomFloat(5.0f);
		     spells[10].attackstoptimer = 1000;
		     m_spellcheck[10] = true;


		     spells[11].info = dbcSpell.LookupEntry(778);
		     spells[11].reqlvl = 30;
		     spells[11].hpreqtocast = 85;
		     spells[11].targettype = TARGET_ATTACKING;
		     spells[11].instant = false;
		     spells[11].perctrigger = (float)RandomFloat(5.0f);
		     spells[11].attackstoptimer = 1000;
		     m_spellcheck[11] = true;


		     spells[12].info = dbcSpell.LookupEntry(24974);
		     spells[12].reqlvl = 30;
		     spells[12].hpreqtocast = 85;
		     spells[12].targettype = TARGET_ATTACKING;
		     spells[12].instant = false;
		     spells[12].perctrigger = (float)RandomFloat(5.0f);
		     spells[12].attackstoptimer = 1000;
		     m_spellcheck[12] = true;


		     spells[13].info = dbcSpell.LookupEntry(8949);
		     spells[13].reqlvl = 26;
		     spells[13].hpreqtocast = 85;
		     spells[13].targettype = TARGET_ATTACKING;
		     spells[13].instant = false;
		     spells[13].perctrigger = (float)RandomFloat(5.0f);
		     spells[13].attackstoptimer = 1000;
		     m_spellcheck[13] = true;


		     spells[14].info = dbcSpell.LookupEntry(5180);
		     spells[14].reqlvl = 30;
		     spells[14].hpreqtocast = 85;
		     spells[14].targettype = TARGET_ATTACKING;
		     spells[14].instant = false;
		     spells[14].perctrigger = (float)RandomFloat(5.0f);
		     spells[14].attackstoptimer = 1000;
		     m_spellcheck[14] = true;


		     spells[15].info = dbcSpell.LookupEntry(8927);
		     spells[15].reqlvl = 28;
		     spells[15].hpreqtocast = 85;
		     spells[15].targettype = TARGET_ATTACKING;
		     spells[15].instant = false;
		     spells[15].perctrigger = (float)RandomFloat(5.0f);
		     spells[15].attackstoptimer = 1000;
		     m_spellcheck[15] = true;


		     spells[16].info = dbcSpell.LookupEntry(9749);
		     spells[16].reqlvl = 42;
		     spells[16].hpreqtocast = 85;
		     spells[16].targettype = TARGET_ATTACKING;
		     spells[16].instant = false;
		     spells[16].perctrigger = (float)RandomFloat(5.0f);
		     spells[16].attackstoptimer = 1000;
		     m_spellcheck[16] = true;


		     spells[17].info = dbcSpell.LookupEntry(24975);
		     spells[17].reqlvl = 40;
		     spells[17].hpreqtocast = 85;
		     spells[17].targettype = TARGET_ATTACKING;
		     spells[17].instant = false;
		     spells[17].perctrigger = (float)RandomFloat(5.0f);
		     spells[17].attackstoptimer = 1000;
		     m_spellcheck[17] = true;


		     spells[18].info = dbcSpell.LookupEntry(8950);
		     spells[18].reqlvl = 34;
		     spells[18].hpreqtocast = 85;
		     spells[18].targettype = TARGET_ATTACKING;
		     spells[18].instant = false;
		     spells[18].perctrigger = (float)RandomFloat(5.0f);
		     spells[18].attackstoptimer = 1000;
		     m_spellcheck[18] = true;


		     spells[19].info = dbcSpell.LookupEntry(16914);
		     spells[19].reqlvl = 40;
		     spells[19].hpreqtocast = 85;
		     spells[19].targettype = TARGET_ATTACKING;
		     spells[19].instant = false;
		     spells[19].perctrigger = (float)RandomFloat(5.0f);
		     spells[19].attackstoptimer = 1000;
		     m_spellcheck[19] = true;


		     spells[20].info = dbcSpell.LookupEntry(6780);
		     spells[20].reqlvl = 38;
		     spells[20].hpreqtocast = 85;
		     spells[20].targettype = TARGET_ATTACKING;
		     spells[20].instant = false;
		     spells[20].perctrigger = (float)RandomFloat(5.0f);
		     spells[20].attackstoptimer = 1000;
		     m_spellcheck[20] = true;


		     spells[21].info = dbcSpell.LookupEntry(8905);
		     spells[21].reqlvl = 46;
		     spells[21].hpreqtocast = 85;
		     spells[21].targettype = TARGET_ATTACKING;
		     spells[21].instant = false;
		     spells[21].perctrigger = (float)RandomFloat(5.0f);
		     spells[21].attackstoptimer = 1000;
		     m_spellcheck[21] = true;


		     spells[22].info = dbcSpell.LookupEntry(8928);
		     spells[22].reqlvl = 34;
		     spells[22].hpreqtocast = 85;
		     spells[22].targettype = TARGET_ATTACKING;
		     spells[22].instant = false;
		     spells[22].perctrigger = (float)RandomFloat(5.0f);
		     spells[22].attackstoptimer = 1000;
		     m_spellcheck[22] = true;


		     spells[23].info = dbcSpell.LookupEntry(8929);
		     spells[23].reqlvl = 40;
		     spells[23].hpreqtocast = 85;
		     spells[23].targettype = TARGET_ATTACKING;
		     spells[23].instant = false;
		     spells[23].perctrigger = (float)RandomFloat(5.0f);
		     spells[23].attackstoptimer = 1000;
		     m_spellcheck[23] = true;


		     spells[24].info = dbcSpell.LookupEntry(9907);
		     spells[24].reqlvl = 54;
		     spells[24].hpreqtocast = 85;
		     spells[24].targettype = TARGET_ATTACKING;
		     spells[24].instant = false;
		     spells[24].perctrigger = (float)RandomFloat(5.0f);
		     spells[24].attackstoptimer = 1000;
		     m_spellcheck[24] = true;


		     spells[25].info = dbcSpell.LookupEntry(24976);
		     spells[25].reqlvl = 50;
		     spells[25].hpreqtocast = 85;
		     spells[25].targettype = TARGET_ATTACKING;
		     spells[25].instant = false;
		     spells[25].perctrigger = (float)RandomFloat(5.0f);
		     spells[25].attackstoptimer = 1000;
		     m_spellcheck[25] = true;


		     spells[26].info = dbcSpell.LookupEntry(8951);
		     spells[26].reqlvl = 42;
		     spells[26].hpreqtocast = 85;
		     spells[26].targettype = TARGET_ATTACKING;
		     spells[26].instant = false;
		     spells[26].perctrigger = (float)RandomFloat(5.0f);
		     spells[26].attackstoptimer = 1000;
		     m_spellcheck[26] = true;


		     spells[27].info = dbcSpell.LookupEntry(17401);
		     spells[27].reqlvl = 50;
		     spells[27].hpreqtocast = 85;
		     spells[27].targettype = TARGET_ATTACKING;
		     spells[27].instant = false;
		     spells[27].perctrigger = (float)RandomFloat(5.0f);
		     spells[27].attackstoptimer = 1000;
		     m_spellcheck[27] = true;


		     spells[28].info = dbcSpell.LookupEntry(9912);
		     spells[28].reqlvl = 54;
		     spells[28].hpreqtocast = 85;
		     spells[28].targettype = TARGET_ATTACKING;
		     spells[28].instant = false;
		     spells[28].perctrigger = (float)RandomFloat(5.0f);
		     spells[28].attackstoptimer = 1000;
		     m_spellcheck[28] = true;


		     spells[29].info = dbcSpell.LookupEntry(9833);
		     spells[29].reqlvl = 46;
		     spells[29].hpreqtocast = 85;
		     spells[29].targettype = TARGET_ATTACKING;
		     spells[29].instant = false;
		     spells[29].perctrigger = (float)RandomFloat(5.0f);
		     spells[29].attackstoptimer = 1000;
		     m_spellcheck[29] = true;


		     spells[30].info = dbcSpell.LookupEntry(9834);
		     spells[30].reqlvl = 52;
		     spells[30].hpreqtocast = 85;
		     spells[30].targettype = TARGET_ATTACKING;
		     spells[30].instant = false;
		     spells[30].perctrigger = (float)RandomFloat(5.0f);
		     spells[30].attackstoptimer = 1000;
		     m_spellcheck[30] = true;


		     spells[31].info = dbcSpell.LookupEntry(9907);
		     spells[31].reqlvl = 54;
		     spells[31].hpreqtocast = 85;
		     spells[31].targettype = TARGET_ATTACKING;
		     spells[31].instant = false;
		     spells[31].perctrigger = (float)RandomFloat(5.0f);
		     spells[31].attackstoptimer = 1000;
		     m_spellcheck[31] = true;


		     spells[32].info = dbcSpell.LookupEntry(24977);
		     spells[32].reqlvl = 60;
		     spells[32].hpreqtocast = 85;
		     spells[32].targettype = TARGET_ATTACKING;
		     spells[32].instant = false;
		     spells[32].perctrigger = (float)RandomFloat(5.0f);
		     spells[32].attackstoptimer = 1000;
		     m_spellcheck[32] = true;


		     spells[33].info = dbcSpell.LookupEntry(9875);
		     spells[33].reqlvl = 50;
		     spells[33].hpreqtocast = 85;
		     spells[33].targettype = TARGET_ATTACKING;
		     spells[33].instant = false;
		     spells[33].perctrigger = (float)RandomFloat(5.0f);
		     spells[33].attackstoptimer = 1000;
		     m_spellcheck[33] = true;


		     spells[34].info = dbcSpell.LookupEntry(9876);
		     spells[34].reqlvl = 58;
		     spells[34].hpreqtocast = 85;
		     spells[34].targettype = TARGET_ATTACKING;
		     spells[34].instant = false;
		     spells[34].perctrigger = (float)RandomFloat(5.0f);
		     spells[34].attackstoptimer = 1000;
		     m_spellcheck[34] = true;


		     spells[35].info = dbcSpell.LookupEntry(17402);
		     spells[35].reqlvl = 60;
		     spells[35].hpreqtocast = 85;
		     spells[35].targettype = TARGET_ATTACKING;
		     spells[35].instant = false;
		     spells[35].perctrigger = (float)RandomFloat(5.0f);
		     spells[35].attackstoptimer = 1000;
		     m_spellcheck[35] = true;


		     spells[36].info = dbcSpell.LookupEntry(467);
		     spells[36].reqlvl = 6;
		     spells[36].hpreqtocast = 85;
		     spells[36].targettype = TARGET_SELF;
		     spells[36].instant = false;
		     spells[36].perctrigger = (float)RandomFloat(5.0f);
		     spells[36].attackstoptimer = 1000;
		     m_spellcheck[36] = true;


		     spells[37].info = dbcSpell.LookupEntry(782);
		     spells[37].reqlvl = 14;
		     spells[37].hpreqtocast = 85;
		     spells[37].targettype = TARGET_SELF;
		     spells[37].instant = false;
		     spells[37].perctrigger = (float)RandomFloat(5.0f);
		     spells[37].attackstoptimer = 1000;
		     m_spellcheck[37] = true;


		     spells[38].info = dbcSpell.LookupEntry(1075);
		     spells[38].reqlvl = 24;
		     spells[38].hpreqtocast = 85;
		     spells[38].targettype = TARGET_SELF;
		     spells[38].instant = false;
		     spells[38].perctrigger = (float)RandomFloat(5.0f);
		     spells[38].attackstoptimer = 1000;
		     m_spellcheck[38] = true;


		     spells[39].info = dbcSpell.LookupEntry(8914);
		     spells[39].reqlvl = 34;
		     spells[39].hpreqtocast = 85;
		     spells[39].targettype = TARGET_SELF;
		     spells[39].instant = false;
		     spells[39].perctrigger = (float)RandomFloat(5.0f);
		     spells[39].attackstoptimer = 1000;
		     m_spellcheck[39] = true;


		     spells[40].info = dbcSpell.LookupEntry(9756);
		     spells[40].reqlvl = 44;
		     spells[40].hpreqtocast = 85;
		     spells[40].targettype = TARGET_SELF;
		     spells[40].instant = false;
		     spells[40].perctrigger = (float)RandomFloat(5.0f);
		     spells[40].attackstoptimer = 1000;
		     m_spellcheck[40] = true;


		     spells[41].info = dbcSpell.LookupEntry(9910);
		     spells[41].reqlvl = 54;
		     spells[41].hpreqtocast = 85;
		     spells[41].targettype = TARGET_SELF;
		     spells[41].instant = false;
		     spells[41].perctrigger = (float)RandomFloat(5.0f);
		     spells[41].attackstoptimer = 1000;
		     m_spellcheck[41] = true;


		     spells[42].info = dbcSpell.LookupEntry(16689);
		     spells[42].reqlvl = 10;
		     spells[42].hpreqtocast = 50;
		     spells[42].targettype = TARGET_SELF;
		     spells[42].instant = false;
		     spells[42].perctrigger = (float)RandomFloat(5.0f);
		     spells[42].attackstoptimer = 1000;
		     m_spellcheck[42] = true;


		     spells[43].info = dbcSpell.LookupEntry(16810);
		     spells[43].reqlvl = 18;
		     spells[43].hpreqtocast = 50;
		     spells[43].targettype = TARGET_SELF;
		     spells[43].instant = false;
		     spells[43].perctrigger = (float)RandomFloat(5.0f);
		     spells[43].attackstoptimer = 1000;
		     m_spellcheck[43] = true;


		     spells[44].info = dbcSpell.LookupEntry(16811);
		     spells[44].reqlvl = 28;
		     spells[44].hpreqtocast = 50;
		     spells[44].targettype = TARGET_SELF;
		     spells[44].instant = false;
		     spells[44].perctrigger = (float)RandomFloat(5.0f);
		     spells[44].attackstoptimer = 1000;
		     m_spellcheck[44] = true;


		     spells[45].info = dbcSpell.LookupEntry(16812);
		     spells[45].reqlvl = 38;
		     spells[45].hpreqtocast = 50;
		     spells[45].targettype = TARGET_SELF;
		     spells[45].instant = false;
		     spells[45].perctrigger = (float)RandomFloat(5.0f);
		     spells[45].attackstoptimer = 1000;
		     m_spellcheck[45] = true;


		     spells[46].info = dbcSpell.LookupEntry(16813);
		     spells[46].reqlvl = 48;
		     spells[46].hpreqtocast = 50;
		     spells[46].targettype = TARGET_SELF;
		     spells[46].instant = false;
		     spells[46].perctrigger = (float)RandomFloat(5.0f);
		     spells[46].attackstoptimer = 1000;
		     m_spellcheck[46] = true;


		     spells[47].info = dbcSpell.LookupEntry(17329);
		     spells[47].reqlvl = 58;
		     spells[47].hpreqtocast = 50;
		     spells[47].targettype = TARGET_SELF;
		     spells[47].instant = false;
		     spells[47].perctrigger = (float)RandomFloat(5.0f);
		     spells[47].attackstoptimer = 1000;
		     m_spellcheck[47] = true;


		     spells[48].info = dbcSpell.LookupEntry(774);
		     spells[48].reqlvl = 4;
		     spells[48].hpreqtocast = 50;
		     spells[48].targettype = TARGET_SELF;
		     spells[48].instant = false;
		     spells[48].perctrigger = (float)RandomFloat(5.0f);
		     spells[48].attackstoptimer = 1000;
		     m_spellcheck[48] = true;


		     spells[49].info = dbcSpell.LookupEntry(1058);
		     spells[49].reqlvl = 10;
		     spells[49].hpreqtocast = 50;
		     spells[49].targettype = TARGET_SELF;
		     spells[49].instant = false;
		     spells[49].perctrigger = (float)RandomFloat(5.0f);
		     spells[49].attackstoptimer = 1000;
		     m_spellcheck[49] = true;


		     spells[50].info = dbcSpell.LookupEntry(1430);
		     spells[50].reqlvl = 16;
		     spells[50].hpreqtocast = 50;
		     spells[50].targettype = TARGET_SELF;
		     spells[50].instant = false;
		     spells[50].perctrigger = (float)RandomFloat(5.0f);
		     spells[50].attackstoptimer = 1000;
		     m_spellcheck[50] = true;


		     spells[51].info = dbcSpell.LookupEntry(2090);
		     spells[51].reqlvl = 22;
		     spells[51].hpreqtocast = 50;
		     spells[51].targettype = TARGET_SELF;
		     spells[51].instant = false;
		     spells[51].perctrigger = (float)RandomFloat(5.0f);
		     spells[51].attackstoptimer = 1000;
		     m_spellcheck[51] = true;


		     spells[52].info = dbcSpell.LookupEntry(2091);
		     spells[52].reqlvl = 28;
		     spells[52].hpreqtocast = 50;
		     spells[52].targettype = TARGET_SELF;
		     spells[52].instant = false;
		     spells[52].perctrigger = (float)RandomFloat(5.0f);
		     spells[52].attackstoptimer = 1000;
		     m_spellcheck[52] = true;


		     spells[53].info = dbcSpell.LookupEntry(3627);
		     spells[53].reqlvl = 34;
		     spells[53].hpreqtocast = 50;
		     spells[53].targettype = TARGET_SELF;
		     spells[53].instant = false;
		     spells[53].perctrigger = (float)RandomFloat(5.0f);
		     spells[53].attackstoptimer = 1000;
		     m_spellcheck[53] = true;


		     spells[54].info = dbcSpell.LookupEntry(8910);
		     spells[54].reqlvl = 40;
		     spells[54].hpreqtocast = 50;
		     spells[54].targettype = TARGET_SELF;
		     spells[54].instant = false;
		     spells[54].perctrigger = (float)RandomFloat(5.0f);
		     spells[54].attackstoptimer = 1000;
		     m_spellcheck[54] = true;


		     spells[55].info = dbcSpell.LookupEntry(5185);
		     spells[55].reqlvl = 1;
		     spells[55].hpreqtocast = 50;
		     spells[55].targettype = TARGET_SELF;
		     spells[55].instant = false;
		     spells[55].perctrigger = (float)RandomFloat(5.0f);
		     spells[55].attackstoptimer = 1000;
		     m_spellcheck[55] = true;


		     spells[56].info = dbcSpell.LookupEntry(5186);
		     spells[56].reqlvl = 8;
		     spells[56].hpreqtocast = 50;
		     spells[56].targettype = TARGET_SELF;
		     spells[56].instant = false;
		     spells[56].perctrigger = (float)RandomFloat(5.0f);
		     spells[56].attackstoptimer = 1000;
		     m_spellcheck[56] = true;


		     spells[57].info = dbcSpell.LookupEntry(5187);
		     spells[57].reqlvl = 14;
		     spells[57].hpreqtocast = 50;
		     spells[57].targettype = TARGET_SELF;
		     spells[57].instant = false;
		     spells[57].perctrigger = (float)RandomFloat(5.0f);
		     spells[57].attackstoptimer = 1000;
		     m_spellcheck[57] = true;


		     spells[58].info = dbcSpell.LookupEntry(5188);
		     spells[58].reqlvl = 20;
		     spells[58].hpreqtocast = 50;
		     spells[58].targettype = TARGET_SELF;
		     spells[58].instant = false;
		     spells[58].perctrigger = (float)RandomFloat(5.0f);
		     spells[58].attackstoptimer = 1000;
		     m_spellcheck[58] = true;


		     spells[59].info = dbcSpell.LookupEntry(5189);
		     spells[59].reqlvl = 26;
		     spells[59].hpreqtocast = 50;
		     spells[59].targettype = TARGET_SELF;
		     spells[59].instant = false;
		     spells[59].perctrigger = (float)RandomFloat(5.0f);
		     spells[59].attackstoptimer = 1000;
		     m_spellcheck[59] = true;


		     spells[60].info = dbcSpell.LookupEntry(6778);
		     spells[60].reqlvl = 32;
		     spells[60].hpreqtocast = 50;
		     spells[60].targettype = TARGET_SELF;
		     spells[60].instant = false;
		     spells[60].perctrigger = (float)RandomFloat(5.0f);
		     spells[60].attackstoptimer = 1000;
		     m_spellcheck[60] = true;


		     spells[61].info = dbcSpell.LookupEntry(8903);
		     spells[61].reqlvl = 38;
		     spells[61].hpreqtocast = 50;
		     spells[61].targettype = TARGET_SELF;
		     spells[61].instant = false;
		     spells[61].perctrigger = (float)RandomFloat(5.0f);
		     spells[61].attackstoptimer = 1000;
		     m_spellcheck[61] = true;


		     spells[62].info = dbcSpell.LookupEntry(339);
		     spells[62].reqlvl = 8;
		     spells[62].hpreqtocast = 20;
		     spells[62].targettype = TARGET_ATTACKING;
		     spells[62].instant = false;
		     spells[62].perctrigger = (float)RandomFloat(5.0f);
		     spells[62].attackstoptimer = 1000;
		     m_spellcheck[62] = true;


		     spells[63].info = dbcSpell.LookupEntry(1062);
		     spells[63].reqlvl = 18;
		     spells[63].hpreqtocast = 20;
		     spells[63].targettype = TARGET_ATTACKING;
		     spells[63].instant = false;
		     spells[63].perctrigger = (float)RandomFloat(5.0f);
		     spells[63].attackstoptimer = 1000;
		     m_spellcheck[63] = true;


		     spells[64].info = dbcSpell.LookupEntry(5195);
		     spells[64].reqlvl = 28;
		     spells[64].hpreqtocast = 20;
		     spells[64].targettype = TARGET_ATTACKING;
		     spells[64].instant = false;
		     spells[64].perctrigger = (float)RandomFloat(5.0f);
		     spells[64].attackstoptimer = 1000;
		     m_spellcheck[64] = true;


		     spells[65].info = dbcSpell.LookupEntry(5196);
		     spells[65].reqlvl = 38;
		     spells[65].hpreqtocast = 20;
		     spells[65].targettype = TARGET_ATTACKING;
		     spells[65].instant = false;
		     spells[65].perctrigger = (float)RandomFloat(5.0f);
		     spells[65].attackstoptimer = 1000;
		     m_spellcheck[65] = true;


		     spells[66].info = dbcSpell.LookupEntry(9852);
		     spells[66].reqlvl = 48;
		     spells[66].hpreqtocast = 20;
		     spells[66].targettype = TARGET_ATTACKING;
		     spells[66].instant = false;
		     spells[66].perctrigger = (float)RandomFloat(5.0f);
		     spells[66].attackstoptimer = 1000;
		     m_spellcheck[66] = true;


		     spells[67].info = dbcSpell.LookupEntry(9853);
		     spells[67].reqlvl = 58;
		     spells[67].hpreqtocast = 20;
		     spells[67].targettype = TARGET_ATTACKING;
		     spells[67].instant = false;
		     spells[67].perctrigger = (float)RandomFloat(5.0f);
		     spells[67].attackstoptimer = 1000;
		     m_spellcheck[67] = true;

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
	      			if (spells[i].hpreqtocast<=_unit->GetHealthPct())
		      	  {
	      			if ((spells[i].reqlvl<=_unit->GetUInt32Value(UNIT_FIELD_LEVEL)) && (spells[i].reqlvl+10>=_unit->GetUInt32Value(UNIT_FIELD_LEVEL)))
		      	  {
			      	if(m_spellcheck[i])
		      		{
				        target = _unit->GetAIInterface()->GetNextTarget();
			       	  switch(spells[i].targettype)
			    		  {
			    		    case TARGET_SELF:
				         			_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
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
	        	}
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

class HumanoidRogue : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(HumanoidRogue);
	SP_AI_Spell spells[21];
 bool m_spellcheck[21];

    HumanoidRogue(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 21;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		     spells[0].info = dbcSpell.LookupEntry(13518);
		     spells[0].reqlvl = 10;
		     spells[0].hpreqtocast = 85;
		     spells[0].targettype = TARGET_ATTACKING;
		     spells[0].instant = false;
		     spells[0].perctrigger = (float)RandomFloat(5.0f);
		     spells[0].attackstoptimer = 1000;
		     m_spellcheck[0] = true;


		     spells[1].info = dbcSpell.LookupEntry(15614);
		     spells[1].reqlvl = 20;
		     spells[1].hpreqtocast = 85;
		     spells[1].targettype = TARGET_ATTACKING;
		     spells[1].instant = false;
		     spells[1].perctrigger = (float)RandomFloat(5.0f);
		     spells[1].attackstoptimer = 1000;
		     m_spellcheck[1] = true;


		     spells[2].info = dbcSpell.LookupEntry(16400);
		     spells[2].reqlvl = 10;
		     spells[2].hpreqtocast = 85;
		     spells[2].targettype = TARGET_ATTACKING;
		     spells[2].instant = false;
		     spells[2].perctrigger = (float)RandomFloat(5.0f);
		     spells[2].attackstoptimer = 1000;
		     m_spellcheck[2] = true;


		     spells[3].info = dbcSpell.LookupEntry(15614);
		     spells[3].reqlvl = 20;
		     spells[3].hpreqtocast = 85;
		     spells[3].targettype = TARGET_ATTACKING;
		     spells[3].instant = false;
		     spells[3].perctrigger = (float)RandomFloat(5.0f);
		     spells[3].attackstoptimer = 1000;
		     m_spellcheck[3] = true;


		     spells[4].info = dbcSpell.LookupEntry(12540);
		     spells[4].reqlvl = 18;
		     spells[4].hpreqtocast = 85;
		     spells[4].targettype = TARGET_ATTACKING;
		     spells[4].instant = false;
		     spells[4].perctrigger = (float)RandomFloat(5.0f);
		     spells[4].attackstoptimer = 1000;
		     m_spellcheck[4] = true;


		     spells[5].info = dbcSpell.LookupEntry(15614);
		     spells[5].reqlvl = 20;
		     spells[5].hpreqtocast = 85;
		     spells[5].targettype = TARGET_ATTACKING;
		     spells[5].instant = false;
		     spells[5].perctrigger = (float)RandomFloat(5.0f);
		     spells[5].attackstoptimer = 1000;
		     m_spellcheck[5] = true;


		     spells[6].info = dbcSpell.LookupEntry(14873);
		     spells[6].reqlvl = 20;
		     spells[6].hpreqtocast = 85;
		     spells[6].targettype = TARGET_ATTACKING;
		     spells[6].instant = false;
		     spells[6].perctrigger = (float)RandomFloat(5.0f);
		     spells[6].attackstoptimer = 1000;
		     m_spellcheck[6] = true;


		     spells[7].info = dbcSpell.LookupEntry(7159);
		     spells[7].reqlvl = 4;
		     spells[7].hpreqtocast = 85;
		     spells[7].targettype = TARGET_ATTACKING;
		     spells[7].instant = false;
		     spells[7].perctrigger = (float)RandomFloat(5.0f);
		     spells[7].attackstoptimer = 1000;
		     m_spellcheck[7] = true;


		     spells[8].info = dbcSpell.LookupEntry(8313);
		     spells[8].reqlvl = 25;
		     spells[8].hpreqtocast = 85;
		     spells[8].targettype = TARGET_ATTACKING;
		     spells[8].instant = false;
		     spells[8].perctrigger = (float)RandomFloat(5.0f);
		     spells[8].attackstoptimer = 1000;
		     m_spellcheck[8] = true;


		     spells[9].info = dbcSpell.LookupEntry(6409);
		     spells[9].reqlvl = 20;
		     spells[9].hpreqtocast = 85;
		     spells[9].targettype = TARGET_ATTACKING;
		     spells[9].instant = false;
		     spells[9].perctrigger = (float)RandomFloat(5.0f);
		     spells[9].attackstoptimer = 1000;
		     m_spellcheck[9] = true;


		     spells[10].info = dbcSpell.LookupEntry(12540);
		     spells[10].reqlvl = 18;
		     spells[10].hpreqtocast = 85;
		     spells[10].targettype = TARGET_ATTACKING;
		     spells[10].instant = false;
		     spells[10].perctrigger = (float)RandomFloat(5.0f);
		     spells[10].attackstoptimer = 1000;
		     m_spellcheck[10] = true;


		     spells[11].info = dbcSpell.LookupEntry(15614);
		     spells[11].reqlvl = 20;
		     spells[11].hpreqtocast = 85;
		     spells[11].targettype = TARGET_ATTACKING;
		     spells[11].instant = false;
		     spells[11].perctrigger = (float)RandomFloat(5.0f);
		     spells[11].attackstoptimer = 1000;
		     m_spellcheck[11] = true;


		     spells[12].info = dbcSpell.LookupEntry(15581);
		     spells[12].reqlvl = 20;
		     spells[12].hpreqtocast = 85;
		     spells[12].targettype = TARGET_ATTACKING;
		     spells[12].instant = false;
		     spells[12].perctrigger = (float)RandomFloat(5.0f);
		     spells[12].attackstoptimer = 1000;
		     m_spellcheck[12] = true;


		     spells[13].info = dbcSpell.LookupEntry(15657);
		     spells[13].reqlvl = 4;
		     spells[13].hpreqtocast = 85;
		     spells[13].targettype = TARGET_ATTACKING;
		     spells[13].instant = false;
		     spells[13].perctrigger = (float)RandomFloat(5.0f);
		     spells[13].attackstoptimer = 1000;
		     m_spellcheck[13] = true;


		     spells[14].info = dbcSpell.LookupEntry(13298);
		     spells[14].reqlvl = 20;
		     spells[14].hpreqtocast = 85;
		     spells[14].targettype = TARGET_ATTACKING;
		     spells[14].instant = false;
		     spells[14].perctrigger = (float)RandomFloat(5.0f);
		     spells[14].attackstoptimer = 1000;
		     m_spellcheck[14] = true;


		     spells[15].info = dbcSpell.LookupEntry(15667);
		     spells[15].reqlvl = 20;
		     spells[15].hpreqtocast = 85;
		     spells[15].targettype = TARGET_ATTACKING;
		     spells[15].instant = false;
		     spells[15].perctrigger = (float)RandomFloat(5.0f);
		     spells[15].attackstoptimer = 1000;
		     m_spellcheck[15] = true;


		     spells[16].info = dbcSpell.LookupEntry(15582);
		     spells[16].reqlvl = 4;
		     spells[16].hpreqtocast = 85;
		     spells[16].targettype = TARGET_ATTACKING;
		     spells[16].instant = false;
		     spells[16].perctrigger = (float)RandomFloat(5.0f);
		     spells[16].attackstoptimer = 1000;
		     m_spellcheck[16] = true;


		     spells[17].info = dbcSpell.LookupEntry(16401);
		     spells[17].reqlvl = 35;
		     spells[17].hpreqtocast = 85;
		     spells[17].targettype = TARGET_ATTACKING;
		     spells[17].instant = false;
		     spells[17].perctrigger = (float)RandomFloat(5.0f);
		     spells[17].attackstoptimer = 1000;
		     m_spellcheck[17] = true;


		     spells[18].info = dbcSpell.LookupEntry(19472);
		     spells[18].reqlvl = 20;
		     spells[18].hpreqtocast = 85;
		     spells[18].targettype = TARGET_ATTACKING;
		     spells[18].instant = false;
		     spells[18].perctrigger = (float)RandomFloat(5.0f);
		     spells[18].attackstoptimer = 1000;
		     m_spellcheck[18] = true;


		     spells[19].info = dbcSpell.LookupEntry(22416);
		     spells[19].reqlvl = 4;
		     spells[19].hpreqtocast = 85;
		     spells[19].targettype = TARGET_ATTACKING;
		     spells[19].instant = false;
		     spells[19].perctrigger = (float)RandomFloat(5.0f);
		     spells[19].attackstoptimer = 1000;
		     m_spellcheck[19] = true;


		     spells[20].info = dbcSpell.LookupEntry(13298);
		     spells[20].reqlvl = 20;
		     spells[20].hpreqtocast = 85;
		     spells[20].targettype = TARGET_ATTACKING;
		     spells[20].instant = false;
		     spells[20].perctrigger = (float)RandomFloat(5.0f);
		     spells[20].attackstoptimer = 1000;
		     m_spellcheck[20] = true;

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
	      			if (spells[i].hpreqtocast<=_unit->GetHealthPct())
		      	  {
	      			if ((spells[i].reqlvl<=_unit->GetUInt32Value(UNIT_FIELD_LEVEL)))
		      	  {
			      	if(m_spellcheck[i])
		      		{
				        target = _unit->GetAIInterface()->GetNextTarget();
			       	  switch(spells[i].targettype)
			    		  {
			    		    case TARGET_SELF:
				         			_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
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
	        	}
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

class HumanoidWarrior : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(HumanoidWarrior);
	SP_AI_Spell spells[20];
 bool m_spellcheck[20];

    HumanoidWarrior(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells =20;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		     spells[0].info = dbcSpell.LookupEntry(11998);
		     spells[0].reqlvl = 20;
		     spells[0].hpreqtocast = 85;
		     spells[0].targettype = TARGET_ATTACKING;
		     spells[0].instant = false;
		     spells[0].perctrigger = (float)RandomFloat(5.0f);
		     spells[0].attackstoptimer = 1000;
		     m_spellcheck[0] = true;


		     spells[1].info = dbcSpell.LookupEntry(18078);
		     spells[1].reqlvl = 30;
		     spells[1].hpreqtocast = 85;
		     spells[1].targettype = TARGET_ATTACKING;
		     spells[1].instant = false;
		     spells[1].perctrigger = (float)RandomFloat(5.0f);
		     spells[1].attackstoptimer = 1000;
		     m_spellcheck[1] = true;


		     spells[2].info = dbcSpell.LookupEntry(13446);
		     spells[2].reqlvl = 1;
		     spells[2].hpreqtocast = 85;
		     spells[2].targettype = TARGET_ATTACKING;
		     spells[2].instant = false;
		     spells[2].perctrigger = (float)RandomFloat(5.0f);
		     spells[2].attackstoptimer = 1000;
		     m_spellcheck[2] = true;


		     spells[3].info = dbcSpell.LookupEntry(8242);
		     spells[3].reqlvl = 10;
		     spells[3].hpreqtocast = 85;
		     spells[3].targettype = TARGET_ATTACKING;
		     spells[3].instant = false;
		     spells[3].perctrigger = (float)RandomFloat(5.0f);
		     spells[3].attackstoptimer = 1000;
		     m_spellcheck[3] = true;


		     spells[4].info = dbcSpell.LookupEntry(11977);
		     spells[4].reqlvl = 20;
		     spells[4].hpreqtocast = 85;
		     spells[4].targettype = TARGET_ATTACKING;
		     spells[4].instant = false;
		     spells[4].perctrigger = (float)RandomFloat(5.0f);
		     spells[4].attackstoptimer = 1000;
		     m_spellcheck[4] = true;


		     spells[5].info = dbcSpell.LookupEntry(13446);
		     spells[5].reqlvl = 1;
		     spells[5].hpreqtocast = 85;
		     spells[5].targettype = TARGET_ATTACKING;
		     spells[5].instant = false;
		     spells[5].perctrigger = (float)RandomFloat(5.0f);
		     spells[5].attackstoptimer = 1000;
		     m_spellcheck[5] = true;


		     spells[6].info = dbcSpell.LookupEntry(15655);
		     spells[6].reqlvl = 10;
		     spells[6].hpreqtocast = 85;
		     spells[6].targettype = TARGET_ATTACKING;
		     spells[6].instant = false;
		     spells[6].perctrigger = (float)RandomFloat(5.0f);
		     spells[6].attackstoptimer = 1000;
		     m_spellcheck[6] = true;


		     spells[7].info = dbcSpell.LookupEntry(12170);
		     spells[7].reqlvl = 20;
		     spells[7].hpreqtocast = 85;
		     spells[7].targettype = TARGET_ATTACKING;
		     spells[7].instant = false;
		     spells[7].perctrigger = (float)RandomFloat(5.0f);
		     spells[7].attackstoptimer = 1000;
		     m_spellcheck[7] = true;


		     spells[8].info = dbcSpell.LookupEntry(13443);
		     spells[8].reqlvl = 20;
		     spells[8].hpreqtocast = 85;
		     spells[8].targettype = TARGET_ATTACKING;
		     spells[8].instant = false;
		     spells[8].perctrigger = (float)RandomFloat(5.0f);
		     spells[8].attackstoptimer = 1000;
		     m_spellcheck[8] = true;


		     spells[9].info = dbcSpell.LookupEntry(14516);
		     spells[9].reqlvl = 1;
		     spells[9].hpreqtocast = 85;
		     spells[9].targettype = TARGET_ATTACKING;
		     spells[9].instant = false;
		     spells[9].perctrigger = (float)RandomFloat(5.0f);
		     spells[9].attackstoptimer = 1000;
		     m_spellcheck[9] = true;


		     spells[10].info = dbcSpell.LookupEntry(11972);
		     spells[10].reqlvl = 20;
		     spells[10].hpreqtocast = 85;
		     spells[10].targettype = TARGET_ATTACKING;
		     spells[10].instant = false;
		     spells[10].perctrigger = (float)RandomFloat(5.0f);
		     spells[10].attackstoptimer = 1000;
		     m_spellcheck[10] = true;


		     spells[11].info = dbcSpell.LookupEntry(19130);
		     spells[11].reqlvl = 20;
		     spells[11].hpreqtocast = 85;
		     spells[11].targettype = TARGET_ATTACKING;
		     spells[11].instant = false;
		     spells[11].perctrigger = (float)RandomFloat(5.0f);
		     spells[11].attackstoptimer = 1000;
		     m_spellcheck[11] = true;


		     spells[12].info = dbcSpell.LookupEntry(13738);
		     spells[12].reqlvl = 20;
		     spells[12].hpreqtocast = 85;
		     spells[12].targettype = TARGET_ATTACKING;
		     spells[12].instant = false;
		     spells[12].perctrigger = (float)RandomFloat(5.0f);
		     spells[12].attackstoptimer = 1000;
		     m_spellcheck[12] = true;


		     spells[13].info = dbcSpell.LookupEntry(15623);
		     spells[13].reqlvl = 10;
		     spells[13].hpreqtocast = 85;
		     spells[13].targettype = TARGET_ATTACKING;
		     spells[13].instant = false;
		     spells[13].perctrigger = (float)RandomFloat(5.0f);
		     spells[13].attackstoptimer = 1000;
		     m_spellcheck[13] = true;


		     spells[14].info = dbcSpell.LookupEntry(14087);
		     spells[14].reqlvl = 35;
		     spells[14].hpreqtocast = 85;
		     spells[14].targettype = TARGET_ATTACKING;
		     spells[14].instant = false;
		     spells[14].perctrigger = (float)RandomFloat(5.0f);
		     spells[14].attackstoptimer = 1000;
		     m_spellcheck[14] = true;


		     spells[15].info = dbcSpell.LookupEntry(15613);
		     spells[15].reqlvl = 10;
		     spells[15].hpreqtocast = 85;
		     spells[15].targettype = TARGET_ATTACKING;
		     spells[15].instant = false;
		     spells[15].perctrigger = (float)RandomFloat(5.0f);
		     spells[15].attackstoptimer = 1000;
		     m_spellcheck[15] = true;


		     spells[16].info = dbcSpell.LookupEntry(16406);
		     spells[16].reqlvl = 45;
		     spells[16].hpreqtocast = 85;
		     spells[16].targettype = TARGET_ATTACKING;
		     spells[16].instant = false;
		     spells[16].perctrigger = (float)RandomFloat(5.0f);
		     spells[16].attackstoptimer = 1000;
		     m_spellcheck[16] = true;


		     spells[17].info = dbcSpell.LookupEntry(15584);
		     spells[17].reqlvl = 10;
		     spells[17].hpreqtocast = 85;
		     spells[17].targettype = TARGET_ATTACKING;
		     spells[17].instant = false;
		     spells[17].perctrigger = (float)RandomFloat(5.0f);
		     spells[17].attackstoptimer = 1000;
		     m_spellcheck[17] = true;


		     spells[18].info = dbcSpell.LookupEntry(13737);
		     spells[18].reqlvl = 0;
		     spells[18].hpreqtocast = 85;
		     spells[18].targettype = TARGET_ATTACKING;
		     spells[18].instant = false;
		     spells[18].perctrigger = (float)RandomFloat(5.0f);
		     spells[18].attackstoptimer = 1000;
		     m_spellcheck[18] = true;


		     spells[19].info = dbcSpell.LookupEntry(17504);
		     spells[19].reqlvl = 60;
		     spells[19].hpreqtocast = 85;
		     spells[19].targettype = TARGET_ATTACKING;
		     spells[19].instant = false;
		     spells[19].perctrigger = (float)RandomFloat(5.0f);
		     spells[19].attackstoptimer = 1000;
		     m_spellcheck[19] = true;

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
	      			if (spells[i].hpreqtocast<=_unit->GetHealthPct())
		      	  {
	      			if ((spells[i].reqlvl<=_unit->GetUInt32Value(UNIT_FIELD_LEVEL)))
		      	  {
			      	if(m_spellcheck[i])
		      		{
				        target = _unit->GetAIInterface()->GetNextTarget();
			       	  switch(spells[i].targettype)
			    		  {
			    		    case TARGET_SELF:
				         			_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
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
	        	}
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

class HumanoidFuryWarrior : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(HumanoidFuryWarrior);
	SP_AI_Spell spells[18];
 bool m_spellcheck[18];

    HumanoidFuryWarrior(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 18;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		     spells[0].info = dbcSpell.LookupEntry(11998);
		     spells[0].reqlvl = 20;
		     spells[0].hpreqtocast = 85;
		     spells[0].targettype = TARGET_ATTACKING;
		     spells[0].instant = false;
		     spells[0].perctrigger = (float)RandomFloat(5.0f);
		     spells[0].attackstoptimer = 1000;
		     m_spellcheck[0] = true;


		     spells[1].info = dbcSpell.LookupEntry(18078);
		     spells[1].reqlvl = 30;
		     spells[1].hpreqtocast = 85;
		     spells[1].targettype = TARGET_ATTACKING;
		     spells[1].instant = false;
		     spells[1].perctrigger = (float)RandomFloat(5.0f);
		     spells[1].attackstoptimer = 1000;
		     m_spellcheck[1] = true;


		     spells[2].info = dbcSpell.LookupEntry(13446);
		     spells[2].reqlvl = 1;
		     spells[2].hpreqtocast = 85;
		     spells[2].targettype = TARGET_ATTACKING;
		     spells[2].instant = false;
		     spells[2].perctrigger = (float)RandomFloat(5.0f);
		     spells[2].attackstoptimer = 1000;
		     m_spellcheck[2] = true;


		     spells[3].info = dbcSpell.LookupEntry(11977);
		     spells[3].reqlvl = 20;
		     spells[3].hpreqtocast = 85;
		     spells[3].targettype = TARGET_ATTACKING;
		     spells[3].instant = false;
		     spells[3].perctrigger = (float)RandomFloat(5.0f);
		     spells[3].attackstoptimer = 1000;
		     m_spellcheck[3] = true;


		     spells[4].info = dbcSpell.LookupEntry(13446);
		     spells[4].reqlvl = 1;
		     spells[4].hpreqtocast = 85;
		     spells[4].targettype = TARGET_ATTACKING;
		     spells[4].instant = false;
		     spells[4].perctrigger = (float)RandomFloat(5.0f);
		     spells[4].attackstoptimer = 1000;
		     m_spellcheck[4] = true;


		     spells[5].info = dbcSpell.LookupEntry(23600);
		     spells[5].reqlvl = 0;
		     spells[5].hpreqtocast = 85;
		     spells[5].targettype = TARGET_ATTACKING;
		     spells[5].instant = false;
		     spells[5].perctrigger = (float)RandomFloat(5.0f);
		     spells[5].attackstoptimer = 1000;
		     m_spellcheck[5] = true;


		     spells[6].info = dbcSpell.LookupEntry(13443);
		     spells[6].reqlvl = 20;
		     spells[6].hpreqtocast = 85;
		     spells[6].targettype = TARGET_ATTACKING;
		     spells[6].instant = false;
		     spells[6].perctrigger = (float)RandomFloat(5.0f);
		     spells[6].attackstoptimer = 1000;
		     m_spellcheck[6] = true;


		     spells[7].info = dbcSpell.LookupEntry(14516);
		     spells[7].reqlvl = 1;
		     spells[7].hpreqtocast = 85;
		     spells[7].targettype = TARGET_ATTACKING;
		     spells[7].instant = false;
		     spells[7].perctrigger = (float)RandomFloat(5.0f);
		     spells[7].attackstoptimer = 1000;
		     m_spellcheck[7] = true;


		     spells[8].info = dbcSpell.LookupEntry(5532);
		     spells[8].reqlvl = 10;
		     spells[8].hpreqtocast = 85;
		     spells[8].targettype = TARGET_ATTACKING;
		     spells[8].instant = false;
		     spells[8].perctrigger = (float)RandomFloat(5.0f);
		     spells[8].attackstoptimer = 1000;
		     m_spellcheck[8] = true;


		     spells[9].info = dbcSpell.LookupEntry(23600);
		     spells[9].reqlvl = 0;
		     spells[9].hpreqtocast = 85;
		     spells[9].targettype = TARGET_ATTACKING;
		     spells[9].instant = false;
		     spells[9].perctrigger = (float)RandomFloat(5.0f);
		     spells[9].attackstoptimer = 1000;
		     m_spellcheck[9] = true;


		     spells[10].info = dbcSpell.LookupEntry(13738);
		     spells[10].reqlvl = 20;
		     spells[10].hpreqtocast = 85;
		     spells[10].targettype = TARGET_ATTACKING;
		     spells[10].instant = false;
		     spells[10].perctrigger = (float)RandomFloat(5.0f);
		     spells[10].attackstoptimer = 1000;
		     m_spellcheck[10] = true;


		     spells[11].info = dbcSpell.LookupEntry(15580);
		     spells[11].reqlvl = 1;
		     spells[11].hpreqtocast = 85;
		     spells[11].targettype = TARGET_ATTACKING;
		     spells[11].instant = false;
		     spells[11].perctrigger = (float)RandomFloat(5.0f);
		     spells[11].attackstoptimer = 1000;
		     m_spellcheck[11] = true;


		     spells[12].info = dbcSpell.LookupEntry(15623);
		     spells[12].reqlvl = 10;
		     spells[12].hpreqtocast = 85;
		     spells[12].targettype = TARGET_ATTACKING;
		     spells[12].instant = false;
		     spells[12].perctrigger = (float)RandomFloat(5.0f);
		     spells[12].attackstoptimer = 1000;
		     m_spellcheck[12] = true;


		     spells[13].info = dbcSpell.LookupEntry(14087);
		     spells[13].reqlvl = 35;
		     spells[13].hpreqtocast = 85;
		     spells[13].targettype = TARGET_ATTACKING;
		     spells[13].instant = false;
		     spells[13].perctrigger = (float)RandomFloat(5.0f);
		     spells[13].attackstoptimer = 1000;
		     m_spellcheck[13] = true;


		     spells[14].info = dbcSpell.LookupEntry(15613);
		     spells[14].reqlvl = 10;
		     spells[14].hpreqtocast = 85;
		     spells[14].targettype = TARGET_ATTACKING;
		     spells[14].instant = false;
		     spells[14].perctrigger = (float)RandomFloat(5.0f);
		     spells[14].attackstoptimer = 1000;
		     m_spellcheck[14] = true;


		     spells[15].info = dbcSpell.LookupEntry(16406);
		     spells[15].reqlvl = 45;
		     spells[15].hpreqtocast = 85;
		     spells[15].targettype = TARGET_ATTACKING;
		     spells[15].instant = false;
		     spells[15].perctrigger = (float)RandomFloat(5.0f);
		     spells[15].attackstoptimer = 1000;
		     m_spellcheck[15] = true;


		     spells[16].info = dbcSpell.LookupEntry(15584);
		     spells[16].reqlvl = 10;
		     spells[16].hpreqtocast = 85;
		     spells[16].targettype = TARGET_ATTACKING;
		     spells[16].instant = false;
		     spells[16].perctrigger = (float)RandomFloat(5.0f);
		     spells[16].attackstoptimer = 1000;
		     m_spellcheck[16] = true;


		     spells[17].info = dbcSpell.LookupEntry(17504);
		     spells[17].reqlvl = 60;
		     spells[17].hpreqtocast = 85;
		     spells[17].targettype = TARGET_ATTACKING;
		     spells[17].instant = false;
		     spells[17].perctrigger = (float)RandomFloat(5.0f);
		     spells[17].attackstoptimer = 1000;
		     m_spellcheck[17] = true;

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
	      			if (spells[i].hpreqtocast<=_unit->GetHealthPct())
		      	  {
	      			if ((spells[i].reqlvl<=_unit->GetUInt32Value(UNIT_FIELD_LEVEL)))
		      	  {
			      	if(m_spellcheck[i])
		      		{
				        target = _unit->GetAIInterface()->GetNextTarget();
			       	  switch(spells[i].targettype)
			    		  {
			    		    case TARGET_SELF:
				         			_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
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
	        	}
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

class HumanoidUnarmed : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(HumanoidUnarmed);
	SP_AI_Spell spells[13];
 bool m_spellcheck[13];

    HumanoidUnarmed(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 13;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		     spells[0].info = dbcSpell.LookupEntry(19639);
		     spells[0].reqlvl = 20;
		     spells[0].hpreqtocast = 85;
		     spells[0].targettype = TARGET_ATTACKING;
		     spells[0].instant = false;
		     spells[0].perctrigger = (float)RandomFloat(5.0f);
		     spells[0].attackstoptimer = 1000;
		     m_spellcheck[0] = true;


		     spells[1].info = dbcSpell.LookupEntry(11978);
		     spells[1].reqlvl = 20;
		     spells[1].hpreqtocast = 85;
		     spells[1].targettype = TARGET_ATTACKING;
		     spells[1].instant = false;
		     spells[1].perctrigger = (float)RandomFloat(5.0f);
		     spells[1].attackstoptimer = 1000;
		     m_spellcheck[1] = true;


		     spells[2].info = dbcSpell.LookupEntry(19639);
		     spells[2].reqlvl = 20;
		     spells[2].hpreqtocast = 85;
		     spells[2].targettype = TARGET_ATTACKING;
		     spells[2].instant = false;
		     spells[2].perctrigger = (float)RandomFloat(5.0f);
		     spells[2].attackstoptimer = 1000;
		     m_spellcheck[2] = true;


		     spells[3].info = dbcSpell.LookupEntry(15610);
		     spells[3].reqlvl = 20;
		     spells[3].hpreqtocast = 85;
		     spells[3].targettype = TARGET_ATTACKING;
		     spells[3].instant = false;
		     spells[3].perctrigger = (float)RandomFloat(5.0f);
		     spells[3].attackstoptimer = 1000;
		     m_spellcheck[3] = true;


		     spells[4].info = dbcSpell.LookupEntry(12555);
		     spells[4].reqlvl = 20;
		     spells[4].hpreqtocast = 85;
		     spells[4].targettype = TARGET_ATTACKING;
		     spells[4].instant = false;
		     spells[4].perctrigger = (float)RandomFloat(5.0f);
		     spells[4].attackstoptimer = 1000;
		     m_spellcheck[4] = true;


		     spells[5].info = dbcSpell.LookupEntry(10966);
		     spells[5].reqlvl = 40;
		     spells[5].hpreqtocast = 85;
		     spells[5].targettype = TARGET_ATTACKING;
		     spells[5].instant = false;
		     spells[5].perctrigger = (float)RandomFloat(5.0f);
		     spells[5].attackstoptimer = 1000;
		     m_spellcheck[5] = true;


		     spells[6].info = dbcSpell.LookupEntry(15614);
		     spells[6].reqlvl = 20;
		     spells[6].hpreqtocast = 85;
		     spells[6].targettype = TARGET_ATTACKING;
		     spells[6].instant = false;
		     spells[6].perctrigger = (float)RandomFloat(5.0f);
		     spells[6].attackstoptimer = 1000;
		     m_spellcheck[6] = true;


		     spells[7].info = dbcSpell.LookupEntry(12555);
		     spells[7].reqlvl = 20;
		     spells[7].hpreqtocast = 85;
		     spells[7].targettype = TARGET_ATTACKING;
		     spells[7].instant = false;
		     spells[7].perctrigger = (float)RandomFloat(5.0f);
		     spells[7].attackstoptimer = 1000;
		     m_spellcheck[7] = true;


		     spells[8].info = dbcSpell.LookupEntry(10966);
		     spells[8].reqlvl = 40;
		     spells[8].hpreqtocast = 85;
		     spells[8].targettype = TARGET_ATTACKING;
		     spells[8].instant = false;
		     spells[8].perctrigger = (float)RandomFloat(5.0f);
		     spells[8].attackstoptimer = 1000;
		     m_spellcheck[8] = true;


		     spells[9].info = dbcSpell.LookupEntry(15614);
		     spells[9].reqlvl = 20;
		     spells[9].hpreqtocast = 85;
		     spells[9].targettype = TARGET_ATTACKING;
		     spells[9].instant = false;
		     spells[9].perctrigger = (float)RandomFloat(5.0f);
		     spells[9].attackstoptimer = 1000;
		     m_spellcheck[9] = true;


		     spells[10].info = dbcSpell.LookupEntry(15615);
		     spells[10].reqlvl = 20;
		     spells[10].hpreqtocast = 85;
		     spells[10].targettype = TARGET_ATTACKING;
		     spells[10].instant = false;
		     spells[10].perctrigger = (float)RandomFloat(5.0f);
		     spells[10].attackstoptimer = 1000;
		     m_spellcheck[10] = true;


		     spells[11].info = dbcSpell.LookupEntry(8716);
		     spells[11].reqlvl = 0;
		     spells[11].hpreqtocast = 85;
		     spells[11].targettype = TARGET_ATTACKING;
		     spells[11].instant = false;
		     spells[11].perctrigger = (float)RandomFloat(5.0f);
		     spells[11].attackstoptimer = 1000;
		     m_spellcheck[11] = true;


		     spells[12].info = dbcSpell.LookupEntry(15618);
		     spells[12].reqlvl = 1;
		     spells[12].hpreqtocast = 85;
		     spells[12].targettype = TARGET_ATTACKING;
		     spells[12].instant = false;
		     spells[12].perctrigger = (float)RandomFloat(5.0f);
		     spells[12].attackstoptimer = 1000;
		     m_spellcheck[12] = true;

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
	      			if (spells[i].hpreqtocast<=_unit->GetHealthPct())
		      	  {
	      			if ((spells[i].reqlvl<=_unit->GetUInt32Value(UNIT_FIELD_LEVEL)))
		      	  {
			      	if(m_spellcheck[i])
		      		{
				        target = _unit->GetAIInterface()->GetNextTarget();
			       	  switch(spells[i].targettype)
			    		  {
			    		    case TARGET_SELF:
				         			_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
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
	        	}
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

class HumanoidShaman : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(HumanoidShaman);
	SP_AI_Spell spells[43];
 bool m_spellcheck[43];

    HumanoidShaman(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 43;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		     spells[0].info = dbcSpell.LookupEntry(403);
		     spells[0].reqlvl = 1;
		     spells[0].hpreqtocast = 85;
		     spells[0].targettype = TARGET_ATTACKING;
		     spells[0].instant = false;
		     spells[0].perctrigger = (float)RandomFloat(5.0f);
		     spells[0].attackstoptimer = 1000;
		     m_spellcheck[0] = true;


		     spells[1].info = dbcSpell.LookupEntry(8042);
		     spells[1].reqlvl = 4;
		     spells[1].hpreqtocast = 85;
		     spells[1].targettype = TARGET_ATTACKING;
		     spells[1].instant = false;
		     spells[1].perctrigger = (float)RandomFloat(5.0f);
		     spells[1].attackstoptimer = 1000;
		     m_spellcheck[1] = true;


		     spells[2].info = dbcSpell.LookupEntry(529);
		     spells[2].reqlvl = 8;
		     spells[2].hpreqtocast = 85;
		     spells[2].targettype = TARGET_ATTACKING;
		     spells[2].instant = false;
		     spells[2].perctrigger = (float)RandomFloat(5.0f);
		     spells[2].attackstoptimer = 1000;
		     m_spellcheck[2] = true;


		     spells[3].info = dbcSpell.LookupEntry(8044);
		     spells[3].reqlvl = 8;
		     spells[3].hpreqtocast = 85;
		     spells[3].targettype = TARGET_ATTACKING;
		     spells[3].instant = false;
		     spells[3].perctrigger = (float)RandomFloat(5.0f);
		     spells[3].attackstoptimer = 1000;
		     m_spellcheck[3] = true;


		     spells[4].info = dbcSpell.LookupEntry(8050);
		     spells[4].reqlvl = 10;
		     spells[4].hpreqtocast = 85;
		     spells[4].targettype = TARGET_ATTACKING;
		     spells[4].instant = false;
		     spells[4].perctrigger = (float)RandomFloat(5.0f);
		     spells[4].attackstoptimer = 1000;
		     m_spellcheck[4] = true;


		     spells[5].info = dbcSpell.LookupEntry(548);
		     spells[5].reqlvl = 14;
		     spells[5].hpreqtocast = 85;
		     spells[5].targettype = TARGET_ATTACKING;
		     spells[5].instant = false;
		     spells[5].perctrigger = (float)RandomFloat(5.0f);
		     spells[5].attackstoptimer = 1000;
		     m_spellcheck[5] = true;


		     spells[6].info = dbcSpell.LookupEntry(8045);
		     spells[6].reqlvl = 14;
		     spells[6].hpreqtocast = 85;
		     spells[6].targettype = TARGET_ATTACKING;
		     spells[6].instant = false;
		     spells[6].perctrigger = (float)RandomFloat(5.0f);
		     spells[6].attackstoptimer = 1000;
		     m_spellcheck[6] = true;


		     spells[7].info = dbcSpell.LookupEntry(8052);
		     spells[7].reqlvl = 18;
		     spells[7].hpreqtocast = 85;
		     spells[7].targettype = TARGET_ATTACKING;
		     spells[7].instant = false;
		     spells[7].perctrigger = (float)RandomFloat(5.0f);
		     spells[7].attackstoptimer = 1000;
		     m_spellcheck[7] = true;


		     spells[8].info = dbcSpell.LookupEntry(915);
		     spells[8].reqlvl = 20;
		     spells[8].hpreqtocast = 85;
		     spells[8].targettype = TARGET_ATTACKING;
		     spells[8].instant = false;
		     spells[8].perctrigger = (float)RandomFloat(5.0f);
		     spells[8].attackstoptimer = 1000;
		     m_spellcheck[8] = true;


		     spells[9].info = dbcSpell.LookupEntry(943);
		     spells[9].reqlvl = 26;
		     spells[9].hpreqtocast = 85;
		     spells[9].targettype = TARGET_ATTACKING;
		     spells[9].instant = false;
		     spells[9].perctrigger = (float)RandomFloat(5.0f);
		     spells[9].attackstoptimer = 1000;
		     m_spellcheck[9] = true;


		     spells[10].info = dbcSpell.LookupEntry(8046);
		     spells[10].reqlvl = 24;
		     spells[10].hpreqtocast = 85;
		     spells[10].targettype = TARGET_ATTACKING;
		     spells[10].instant = false;
		     spells[10].perctrigger = (float)RandomFloat(5.0f);
		     spells[10].attackstoptimer = 1000;
		     m_spellcheck[10] = true;


		     spells[11].info = dbcSpell.LookupEntry(8053);
		     spells[11].reqlvl = 28;
		     spells[11].hpreqtocast = 85;
		     spells[11].targettype = TARGET_ATTACKING;
		     spells[11].instant = false;
		     spells[11].perctrigger = (float)RandomFloat(5.0f);
		     spells[11].attackstoptimer = 1000;
		     m_spellcheck[11] = true;


		     spells[12].info = dbcSpell.LookupEntry(8056);
		     spells[12].reqlvl = 20;
		     spells[12].hpreqtocast = 85;
		     spells[12].targettype = TARGET_ATTACKING;
		     spells[12].instant = false;
		     spells[12].perctrigger = (float)RandomFloat(5.0f);
		     spells[12].attackstoptimer = 1000;
		     m_spellcheck[12] = true;


		     spells[13].info = dbcSpell.LookupEntry(6041);
		     spells[13].reqlvl = 32;
		     spells[13].hpreqtocast = 85;
		     spells[13].targettype = TARGET_ATTACKING;
		     spells[13].instant = false;
		     spells[13].perctrigger = (float)RandomFloat(5.0f);
		     spells[13].attackstoptimer = 1000;
		     m_spellcheck[13] = true;


		     spells[14].info = dbcSpell.LookupEntry(10391);
		     spells[14].reqlvl = 38;
		     spells[14].hpreqtocast = 85;
		     spells[14].targettype = TARGET_ATTACKING;
		     spells[14].instant = false;
		     spells[14].perctrigger = (float)RandomFloat(5.0f);
		     spells[14].attackstoptimer = 1000;
		     m_spellcheck[14] = true;


		     spells[15].info = dbcSpell.LookupEntry(421);
		     spells[15].reqlvl = 32;
		     spells[15].hpreqtocast = 85;
		     spells[15].targettype = TARGET_ATTACKING;
		     spells[15].instant = false;
		     spells[15].perctrigger = (float)RandomFloat(5.0f);
		     spells[15].attackstoptimer = 1000;
		     m_spellcheck[15] = true;


		     spells[16].info = dbcSpell.LookupEntry(10412);
		     spells[16].reqlvl = 36;
		     spells[16].hpreqtocast = 85;
		     spells[16].targettype = TARGET_ATTACKING;
		     spells[16].instant = false;
		     spells[16].perctrigger = (float)RandomFloat(5.0f);
		     spells[16].attackstoptimer = 1000;
		     m_spellcheck[16] = true;


		     spells[17].info = dbcSpell.LookupEntry(8056);
		     spells[17].reqlvl = 20;
		     spells[17].hpreqtocast = 85;
		     spells[17].targettype = TARGET_ATTACKING;
		     spells[17].instant = false;
		     spells[17].perctrigger = (float)RandomFloat(5.0f);
		     spells[17].attackstoptimer = 1000;
		     m_spellcheck[17] = true;


		     spells[18].info = dbcSpell.LookupEntry(10413);
		     spells[18].reqlvl = 48;
		     spells[18].hpreqtocast = 85;
		     spells[18].targettype = TARGET_ATTACKING;
		     spells[18].instant = false;
		     spells[18].perctrigger = (float)RandomFloat(5.0f);
		     spells[18].attackstoptimer = 1000;
		     m_spellcheck[18] = true;


		     spells[19].info = dbcSpell.LookupEntry(930);
		     spells[19].reqlvl = 40;
		     spells[19].hpreqtocast = 85;
		     spells[19].targettype = TARGET_ATTACKING;
		     spells[19].instant = false;
		     spells[19].perctrigger = (float)RandomFloat(5.0f);
		     spells[19].attackstoptimer = 1000;
		     m_spellcheck[19] = true;


		     spells[20].info = dbcSpell.LookupEntry(2860);
		     spells[20].reqlvl = 48;
		     spells[20].hpreqtocast = 85;
		     spells[20].targettype = TARGET_ATTACKING;
		     spells[20].instant = false;
		     spells[20].perctrigger = (float)RandomFloat(5.0f);
		     spells[20].attackstoptimer = 1000;
		     m_spellcheck[20] = true;


		     spells[21].info = dbcSpell.LookupEntry(10447);
		     spells[21].reqlvl = 40;
		     spells[21].hpreqtocast = 85;
		     spells[21].targettype = TARGET_ATTACKING;
		     spells[21].instant = false;
		     spells[21].perctrigger = (float)RandomFloat(5.0f);
		     spells[21].attackstoptimer = 1000;
		     m_spellcheck[21] = true;


		     spells[22].info = dbcSpell.LookupEntry(10392);
		     spells[22].reqlvl = 44;
		     spells[22].hpreqtocast = 85;
		     spells[22].targettype = TARGET_ATTACKING;
		     spells[22].instant = false;
		     spells[22].perctrigger = (float)RandomFloat(5.0f);
		     spells[22].attackstoptimer = 1000;
		     m_spellcheck[22] = true;


		     spells[23].info = dbcSpell.LookupEntry(10472);
		     spells[23].reqlvl = 46;
		     spells[23].hpreqtocast = 85;
		     spells[23].targettype = TARGET_ATTACKING;
		     spells[23].instant = false;
		     spells[23].perctrigger = (float)RandomFloat(5.0f);
		     spells[23].attackstoptimer = 1000;
		     m_spellcheck[23] = true;


		     spells[24].info = dbcSpell.LookupEntry(10414);
		     spells[24].reqlvl = 60;
		     spells[24].hpreqtocast = 85;
		     spells[24].targettype = TARGET_ATTACKING;
		     spells[24].instant = false;
		     spells[24].perctrigger = (float)RandomFloat(5.0f);
		     spells[24].attackstoptimer = 1000;
		     m_spellcheck[24] = true;


		     spells[25].info = dbcSpell.LookupEntry(10473);
		     spells[25].reqlvl = 58;
		     spells[25].hpreqtocast = 85;
		     spells[25].targettype = TARGET_ATTACKING;
		     spells[25].instant = false;
		     spells[25].perctrigger = (float)RandomFloat(5.0f);
		     spells[25].attackstoptimer = 1000;
		     m_spellcheck[25] = true;


		     spells[26].info = dbcSpell.LookupEntry(15207);
		     spells[26].reqlvl = 50;
		     spells[26].hpreqtocast = 85;
		     spells[26].targettype = TARGET_ATTACKING;
		     spells[26].instant = false;
		     spells[26].perctrigger = (float)RandomFloat(5.0f);
		     spells[26].attackstoptimer = 1000;
		     m_spellcheck[26] = true;


		     spells[27].info = dbcSpell.LookupEntry(10448);
		     spells[27].reqlvl = 52;
		     spells[27].hpreqtocast = 85;
		     spells[27].targettype = TARGET_ATTACKING;
		     spells[27].instant = false;
		     spells[27].perctrigger = (float)RandomFloat(5.0f);
		     spells[27].attackstoptimer = 1000;
		     m_spellcheck[27] = true;


		     spells[28].info = dbcSpell.LookupEntry(10605);
		     spells[28].reqlvl = 56;
		     spells[28].hpreqtocast = 85;
		     spells[28].targettype = TARGET_ATTACKING;
		     spells[28].instant = false;
		     spells[28].perctrigger = (float)RandomFloat(5.0f);
		     spells[28].attackstoptimer = 1000;
		     m_spellcheck[28] = true;


		     spells[29].info = dbcSpell.LookupEntry(15208);
		     spells[29].reqlvl = 56;
		     spells[29].hpreqtocast = 85;
		     spells[29].targettype = TARGET_ATTACKING;
		     spells[29].instant = false;
		     spells[29].perctrigger = (float)RandomFloat(5.0f);
		     spells[29].attackstoptimer = 1000;
		     m_spellcheck[29] = true;


		     spells[30].info = dbcSpell.LookupEntry(324);
		     spells[30].reqlvl = 8;
		     spells[30].hpreqtocast = 85;
		     spells[30].targettype = TARGET_SELF;
		     spells[30].instant = false;
		     spells[30].perctrigger = (float)RandomFloat(5.0f);
		     spells[30].attackstoptimer = 1000;
		     m_spellcheck[30] = true;


		     spells[31].info = dbcSpell.LookupEntry(325);
		     spells[31].reqlvl = 16;
		     spells[31].hpreqtocast = 85;
		     spells[31].targettype = TARGET_SELF;
		     spells[31].instant = false;
		     spells[31].perctrigger = (float)RandomFloat(5.0f);
		     spells[31].attackstoptimer = 1000;
		     m_spellcheck[31] = true;


		     spells[32].info = dbcSpell.LookupEntry(905);
		     spells[32].reqlvl = 24;
		     spells[32].hpreqtocast = 85;
		     spells[32].targettype = TARGET_SELF;
		     spells[32].instant = false;
		     spells[32].perctrigger = (float)RandomFloat(5.0f);
		     spells[32].attackstoptimer = 1000;
		     m_spellcheck[32] = true;


		     spells[33].info = dbcSpell.LookupEntry(8134);
		     spells[33].reqlvl = 40;
		     spells[33].hpreqtocast = 85;
		     spells[33].targettype = TARGET_SELF;
		     spells[33].instant = false;
		     spells[33].perctrigger = (float)RandomFloat(5.0f);
		     spells[33].attackstoptimer = 1000;
		     m_spellcheck[33] = true;


		     spells[34].info = dbcSpell.LookupEntry(10431);
		     spells[34].reqlvl = 48;
		     spells[34].hpreqtocast = 85;
		     spells[34].targettype = TARGET_SELF;
		     spells[34].instant = false;
		     spells[34].perctrigger = (float)RandomFloat(5.0f);
		     spells[34].attackstoptimer = 1000;
		     m_spellcheck[34] = true;


		     spells[35].info = dbcSpell.LookupEntry(10432);
		     spells[35].reqlvl = 56;
		     spells[35].hpreqtocast = 85;
		     spells[35].targettype = TARGET_SELF;
		     spells[35].instant = false;
		     spells[35].perctrigger = (float)RandomFloat(5.0f);
		     spells[35].attackstoptimer = 1000;
		     m_spellcheck[35] = true;


		     spells[36].info = dbcSpell.LookupEntry(331);
		     spells[36].reqlvl = 1;
		     spells[36].hpreqtocast = 50;
		     spells[36].targettype = TARGET_SELF;
		     spells[36].instant = false;
		     spells[36].perctrigger = (float)RandomFloat(5.0f);
		     spells[36].attackstoptimer = 1000;
		     m_spellcheck[36] = true;


		     spells[37].info = dbcSpell.LookupEntry(332);
		     spells[37].reqlvl = 6;
		     spells[37].hpreqtocast = 50;
		     spells[37].targettype = TARGET_SELF;
		     spells[37].instant = false;
		     spells[37].perctrigger = (float)RandomFloat(5.0f);
		     spells[37].attackstoptimer = 1000;
		     m_spellcheck[37] = true;


		     spells[38].info = dbcSpell.LookupEntry(8004);
		     spells[38].reqlvl = 20;
		     spells[38].hpreqtocast = 50;
		     spells[38].targettype = TARGET_SELF;
		     spells[38].instant = false;
		     spells[38].perctrigger = (float)RandomFloat(5.0f);
		     spells[38].attackstoptimer = 1000;
		     m_spellcheck[38] = true;


		     spells[39].info = dbcSpell.LookupEntry(8008);
		     spells[39].reqlvl = 28;
		     spells[39].hpreqtocast = 50;
		     spells[39].targettype = TARGET_SELF;
		     spells[39].instant = false;
		     spells[39].perctrigger = (float)RandomFloat(5.0f);
		     spells[39].attackstoptimer = 1000;
		     m_spellcheck[39] = true;


		     spells[40].info = dbcSpell.LookupEntry(8010);
		     spells[40].reqlvl = 36;
		     spells[40].hpreqtocast = 50;
		     spells[40].targettype = TARGET_SELF;
		     spells[40].instant = false;
		     spells[40].perctrigger = (float)RandomFloat(5.0f);
		     spells[40].attackstoptimer = 1000;
		     m_spellcheck[40] = true;


		     spells[41].info = dbcSpell.LookupEntry(10466);
		     spells[41].reqlvl = 44;
		     spells[41].hpreqtocast = 50;
		     spells[41].targettype = TARGET_SELF;
		     spells[41].instant = false;
		     spells[41].perctrigger = (float)RandomFloat(5.0f);
		     spells[41].attackstoptimer = 1000;
		     m_spellcheck[41] = true;


		     spells[42].info = dbcSpell.LookupEntry(10468);
		     spells[42].reqlvl = 60;
		     spells[42].hpreqtocast = 50;
		     spells[42].targettype = TARGET_SELF;
		     spells[42].instant = false;
		     spells[42].perctrigger = (float)RandomFloat(5.0f);
		     spells[42].attackstoptimer = 1000;
		     m_spellcheck[42] = true;

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
	      			if (spells[i].hpreqtocast<=_unit->GetHealthPct())
		      	  {
	      			if ((spells[i].reqlvl<=_unit->GetUInt32Value(UNIT_FIELD_LEVEL)) && (spells[i].reqlvl+10>=_unit->GetUInt32Value(UNIT_FIELD_LEVEL)))
		      	  {
			      	if(m_spellcheck[i])
		      		{
				        target = _unit->GetAIInterface()->GetNextTarget();
			       	  switch(spells[i].targettype)
			    		  {
			    		    case TARGET_SELF:
				         			_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
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
	        	}
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


void SetupGenericAI(ScriptMgr * mgr)
{
// Family based AI's
/*
mgr->register_creature_script(16350, &SpiderFamily::Create); 
mgr->register_creature_script(18466, &SpiderFamily::Create);
mgr->register_creature_script(11370, &SpiderFamily::Create);
mgr->register_creature_script(15041, &SpiderFamily::Create);
mgr->register_creature_script(4005, &SpiderFamily::Create);
mgr->register_creature_script(16351, &SpiderFamily::Create);
mgr->register_creature_script(16352, &SpiderFamily::Create);
mgr->register_creature_script(7319, &SpiderFamily::Create);
mgr->register_creature_script(2563, &SpiderFamily::Create);
mgr->register_creature_script(14880, &SpiderFamily::Create);
mgr->register_creature_script(20714, &SpiderFamily::Create);
mgr->register_creature_script(20027, &SpiderFamily::Create);
mgr->register_creature_script(3821, &SpiderFamily::Create);
mgr->register_creature_script(8762, &SpiderFamily::Create);
mgr->register_creature_script(2565, &SpiderFamily::Create);
mgr->register_creature_script(505, &SpiderFamily::Create);
mgr->register_creature_script(1688, &SpiderFamily::Create);
mgr->register_creature_script(4376, &SpiderFamily::Create);
mgr->register_creature_script(4377, &SpiderFamily::Create);
mgr->register_creature_script(4379, &SpiderFamily::Create);
mgr->register_creature_script(4380, &SpiderFamily::Create);
mgr->register_creature_script(1504, &SpiderFamily::Create);
mgr->register_creature_script(1195, &SpiderFamily::Create);
mgr->register_creature_script(471, &SpiderFamily::Create);
mgr->register_creature_script(616, &SpiderFamily::Create);
mgr->register_creature_script(769, &SpiderFamily::Create);
mgr->register_creature_script(1112, &SpiderFamily::Create);
mgr->register_creature_script(1780, &SpiderFamily::Create);
mgr->register_creature_script(1781, &SpiderFamily::Create);
mgr->register_creature_script(1823, &SpiderFamily::Create);
mgr->register_creature_script(1825, &SpiderFamily::Create);
mgr->register_creature_script(1986, &SpiderFamily::Create);
mgr->register_creature_script(1994, &SpiderFamily::Create);
mgr->register_creature_script(2686, &SpiderFamily::Create);
mgr->register_creature_script(5856, &SpiderFamily::Create);
mgr->register_creature_script(1505, &SpiderFamily::Create);
mgr->register_creature_script(14532, &SpiderFamily::Create);
mgr->register_creature_script(18983, &SpiderFamily::Create);
mgr->register_creature_script(4415, &SpiderFamily::Create);
mgr->register_creature_script(5446, &SpiderFamily::Create);
mgr->register_creature_script(4007, &SpiderFamily::Create);
mgr->register_creature_script(8277, &SpiderFamily::Create);
mgr->register_creature_script(8933, &SpiderFamily::Create);
mgr->register_creature_script(10359, &SpiderFamily::Create);
mgr->register_creature_script(10375, &SpiderFamily::Create);
mgr->register_creature_script(12433, &SpiderFamily::Create);
mgr->register_creature_script(14266, &SpiderFamily::Create);
mgr->register_creature_script(14472, &SpiderFamily::Create);
mgr->register_creature_script(1185, &SpiderFamily::Create);
mgr->register_creature_script(539, &SpiderFamily::Create);
mgr->register_creature_script(442, &SpiderFamily::Create);
mgr->register_creature_script(18467, &SpiderFamily::Create);
mgr->register_creature_script(16171, &SpiderFamily::Create);
mgr->register_creature_script(16170, &SpiderFamily::Create);
mgr->register_creature_script(4411, &SpiderFamily::Create);
mgr->register_creature_script(4412, &SpiderFamily::Create);
mgr->register_creature_script(22044, &SpiderFamily::Create);
mgr->register_creature_script(2350, &SpiderFamily::Create);
mgr->register_creature_script(2349, &SpiderFamily::Create);
mgr->register_creature_script(3820, &SpiderFamily::Create);
mgr->register_creature_script(17522, &SpiderFamily::Create);
mgr->register_creature_script(17523, &SpiderFamily::Create);
mgr->register_creature_script(18647, &SpiderFamily::Create);
mgr->register_creature_script(4414, &SpiderFamily::Create);
mgr->register_creature_script(1821, &SpiderFamily::Create);
mgr->register_creature_script(1824, &SpiderFamily::Create);
mgr->register_creature_script(4413, &SpiderFamily::Create);
mgr->register_creature_script(22132, &SpiderFamily::Create);
mgr->register_creature_script(2000, &SpiderFamily::Create);
mgr->register_creature_script(20998, &SpiderFamily::Create);
mgr->register_creature_script(3819, &SpiderFamily::Create);
mgr->register_creature_script(1822, &SpiderFamily::Create);
mgr->register_creature_script(5857, &SpiderFamily::Create);
mgr->register_creature_script(1999, &SpiderFamily::Create);
mgr->register_creature_script(1998, &SpiderFamily::Create);
mgr->register_creature_script(930, &SpiderFamily::Create);
mgr->register_creature_script(11738, &SpiderFamily::Create);
mgr->register_creature_script(11739, &SpiderFamily::Create);
mgr->register_creature_script(858, &SpiderFamily::Create);
mgr->register_creature_script(17683, &SpiderFamily::Create);
mgr->register_creature_script(14279, &SpiderFamily::Create);
mgr->register_creature_script(2348, &SpiderFamily::Create);
mgr->register_creature_script(574, &SpiderFamily::Create);
mgr->register_creature_script(4006, &SpiderFamily::Create);
mgr->register_creature_script(4263, &SpiderFamily::Create);
mgr->register_creature_script(11921, &SpiderFamily::Create);
mgr->register_creature_script(4264, &SpiderFamily::Create);
mgr->register_creature_script(30, &SpiderFamily::Create);
mgr->register_creature_script(1555, &SpiderFamily::Create);
mgr->register_creature_script(4378, &SpiderFamily::Create);
mgr->register_creature_script(43, &SpiderFamily::Create);
mgr->register_creature_script(1111, &SpiderFamily::Create);
mgr->register_creature_script(1184, &SpiderFamily::Create);
mgr->register_creature_script(5858, &SpiderFamily::Create);
mgr->register_creature_script(217, &SpiderFamily::Create);
mgr->register_creature_script(949, &SpiderFamily::Create);
mgr->register_creature_script(569, &SpiderFamily::Create);
mgr->register_creature_script(2001, &SpiderFamily::Create);
mgr->register_creature_script(4040, &SpiderFamily::Create);
mgr->register_creature_script(17112, &SpiderFamily::Create);
mgr->register_creature_script(16179, &SpiderFamily::Create);
mgr->register_creature_script(4692, &BirdFamily::Create);
mgr->register_creature_script(16973, &BirdFamily::Create);
mgr->register_creature_script(18470, &BirdFamily::Create);
mgr->register_creature_script(199, &BirdFamily::Create);
mgr->register_creature_script(5982, &BirdFamily::Create);
mgr->register_creature_script(2580, &BirdFamily::Create);
mgr->register_creature_script(2579, &BirdFamily::Create);
mgr->register_creature_script(462, &BirdFamily::Create);
mgr->register_creature_script(4695, &BirdFamily::Create);
mgr->register_creature_script(2578, &BirdFamily::Create);
mgr->register_creature_script(428, &BirdFamily::Create);
mgr->register_creature_script(2801, &BirdFamily::Create);
mgr->register_creature_script(2870, &BirdFamily::Create);
mgr->register_creature_script(2880, &BirdFamily::Create);
mgr->register_creature_script(5430, &BirdFamily::Create);
mgr->register_creature_script(154, &BirdFamily::Create);
mgr->register_creature_script(6013, &BirdFamily::Create);
mgr->register_creature_script(8207, &BirdFamily::Create);
mgr->register_creature_script(2931, &BirdFamily::Create);
mgr->register_creature_script(14268, &BirdFamily::Create); 
mgr->register_creature_script(4154, &BirdFamily::Create);
mgr->register_creature_script(1809, &BirdFamily::Create);
mgr->register_creature_script(21515, &BirdFamily::Create);
mgr->register_creature_script(2971, &BirdFamily::Create);
mgr->register_creature_script(16972, &BirdFamily::Create);
mgr->register_creature_script(2830, &BirdFamily::Create);
mgr->register_creature_script(2831, &BirdFamily::Create);
mgr->register_creature_script(21042, &BirdFamily::Create);
mgr->register_creature_script(2970, &BirdFamily::Create);
mgr->register_creature_script(2969, &BirdFamily::Create);
mgr->register_creature_script(4693, &BirdFamily::Create);
mgr->register_creature_script(4694, &BirdFamily::Create);
mgr->register_creature_script(4158, &BirdFamily::Create);
mgr->register_creature_script(1109, &BirdFamily::Create);
mgr->register_creature_script(5428, &BirdFamily::Create);
mgr->register_creature_script(5429, &BirdFamily::Create);
mgr->register_creature_script(2829, &BirdFamily::Create);
mgr->register_creature_script(8299, &BirdFamily::Create);
mgr->register_creature_script(1194, &BirdFamily::Create);
mgr->register_creature_script(1722, &BirdFamily::Create);
mgr->register_creature_script(1723, &BirdFamily::Create);
mgr->register_creature_script(1724, &BirdFamily::Create);
mgr->register_creature_script(8001, &BirdFamily::Create);
mgr->register_creature_script(8996, &VoidwalkerFamily::Create);
mgr->register_creature_script(14389, &VoidwalkerFamily::Create);
mgr->register_creature_script(1860, &VoidwalkerFamily::Create);
mgr->register_creature_script(8656, &VoidwalkerFamily::Create);
mgr->register_creature_script(9695, &ScorpionFamily::Create);
mgr->register_creature_script(22257, &ScorpionFamily::Create);
mgr->register_creature_script(7078, &ScorpionFamily::Create);
mgr->register_creature_script(9691, &ScorpionFamily::Create);
mgr->register_creature_script(3126, &ScorpionFamily::Create);
mgr->register_creature_script(7022, &ScorpionFamily::Create);
mgr->register_creature_script(5422, &ScorpionFamily::Create);
mgr->register_creature_script(5937, &ScorpionFamily::Create);
mgr->register_creature_script(7803, &ScorpionFamily::Create);
mgr->register_creature_script(8301, &ScorpionFamily::Create);
mgr->register_creature_script(8926, &ScorpionFamily::Create);
mgr->register_creature_script(7405, &ScorpionFamily::Create);
mgr->register_creature_script(5988, &ScorpionFamily::Create);
mgr->register_creature_script(3127, &ScorpionFamily::Create);
mgr->register_creature_script(5423, &ScorpionFamily::Create);
mgr->register_creature_script(21864, &ScorpionFamily::Create);
mgr->register_creature_script(4140, &ScorpionFamily::Create);
mgr->register_creature_script(4139, &ScorpionFamily::Create);
mgr->register_creature_script(4697, &ScorpionFamily::Create);
mgr->register_creature_script(14476, &ScorpionFamily::Create);
mgr->register_creature_script(3250, &ScorpionFamily::Create);
mgr->register_creature_script(3252, &ScorpionFamily::Create);
mgr->register_creature_script(22100, &ScorpionFamily::Create);
mgr->register_creature_script(4696, &ScorpionFamily::Create);
mgr->register_creature_script(4699, &ScorpionFamily::Create);
mgr->register_creature_script(3226, &ScorpionFamily::Create);
mgr->register_creature_script(5823, &ScorpionFamily::Create);
mgr->register_creature_script(11735, &ScorpionFamily::Create);
mgr->register_creature_script(11736, &ScorpionFamily::Create);
mgr->register_creature_script(11737, &ScorpionFamily::Create);
mgr->register_creature_script(3125, &ScorpionFamily::Create);
mgr->register_creature_script(3124, &ScorpionFamily::Create);
mgr->register_creature_script(3281, &ScorpionFamily::Create);
mgr->register_creature_script(5424, &ScorpionFamily::Create);
mgr->register_creature_script(9698, &ScorpionFamily::Create);
mgr->register_creature_script(2959, &WolfFamily::Create);
mgr->register_creature_script(20748, &WolfFamily::Create);
mgr->register_creature_script(2730, &WolfFamily::Create);
mgr->register_creature_script(17669, &WolfFamily::Create);
mgr->register_creature_script(2925, &WolfFamily::Create);
mgr->register_creature_script(2680, &WolfFamily::Create);
mgr->register_creature_script(521, &WolfFamily::Create);
mgr->register_creature_script(1131, &WolfFamily::Create);
mgr->register_creature_script(17280, &WolfFamily::Create);
mgr->register_creature_script(2727, &WolfFamily::Create);
mgr->register_creature_script(10981, &WolfFamily::Create);
mgr->register_creature_script(10221, &WolfFamily::Create);
mgr->register_creature_script(923, &WolfFamily::Create);
mgr->register_creature_script(9416, &WolfFamily::Create);
mgr->register_creature_script(2924, &WolfFamily::Create);
mgr->register_creature_script(704, &WolfFamily::Create);
mgr->register_creature_script(5286, &WolfFamily::Create);
mgr->register_creature_script(19616, &WolfFamily::Create);
mgr->register_creature_script(2958, &WolfFamily::Create);
mgr->register_creature_script(1508, &WolfFamily::Create);
mgr->register_creature_script(2681, &WolfFamily::Create);
mgr->register_creature_script(1133, &WolfFamily::Create);
mgr->register_creature_script(21952, &WolfFamily::Create);
mgr->register_creature_script(1509, &WolfFamily::Create);
mgr->register_creature_script(2926, &WolfFamily::Create);
mgr->register_creature_script(10644, &WolfFamily::Create);
mgr->register_creature_script(5433, &WolfFamily::Create);
mgr->register_creature_script(5437, &WolfFamily::Create);
mgr->register_creature_script(5438, &WolfFamily::Create);
mgr->register_creature_script(5444, &WolfFamily::Create);
mgr->register_creature_script(5449, &WolfFamily::Create);
mgr->register_creature_script(834, &WolfFamily::Create);
mgr->register_creature_script(3825, &WolfFamily::Create);
mgr->register_creature_script(11871, &WolfFamily::Create);
mgr->register_creature_script(628, &WolfFamily::Create);
mgr->register_creature_script(17548, &WolfFamily::Create);
mgr->register_creature_script(20058, &WolfFamily::Create);
mgr->register_creature_script(11614, &WolfFamily::Create);
mgr->register_creature_script(12345, &WolfFamily::Create);
mgr->register_creature_script(12348, &WolfFamily::Create);
mgr->register_creature_script(12356, &WolfFamily::Create);
mgr->register_creature_script(12357, &WolfFamily::Create);
mgr->register_creature_script(5356, &WolfFamily::Create);
mgr->register_creature_script(3824, &WolfFamily::Create);
mgr->register_creature_script(20330, &WolfFamily::Create);
mgr->register_creature_script(9696, &WolfFamily::Create);
mgr->register_creature_script(2960, &WolfFamily::Create);
mgr->register_creature_script(3823, &WolfFamily::Create);
mgr->register_creature_script(1132, &WolfFamily::Create);
mgr->register_creature_script(18476, &WolfFamily::Create);
mgr->register_creature_script(9694, &WolfFamily::Create);
mgr->register_creature_script(5288, &WolfFamily::Create);
mgr->register_creature_script(9697, &WolfFamily::Create);
mgr->register_creature_script(8959, &WolfFamily::Create);
mgr->register_creature_script(11024, &WolfFamily::Create);
mgr->register_creature_script(2728, &WolfFamily::Create);
mgr->register_creature_script(18670, &WolfFamily::Create);
mgr->register_creature_script(1765, &WolfFamily::Create);
mgr->register_creature_script(1766, &WolfFamily::Create);
mgr->register_creature_script(21340, &WolfFamily::Create);
mgr->register_creature_script(18477, &WolfFamily::Create);
mgr->register_creature_script(8960, &WolfFamily::Create);
mgr->register_creature_script(18033, &WolfFamily::Create);
mgr->register_creature_script(8961, &WolfFamily::Create);
mgr->register_creature_script(21956, &WolfFamily::Create);
mgr->register_creature_script(1817, &WolfFamily::Create);
mgr->register_creature_script(2729, &WolfFamily::Create);
mgr->register_creature_script(1258, &WolfFamily::Create);
mgr->register_creature_script(3862, &WolfFamily::Create);
mgr->register_creature_script(118, &WolfFamily::Create);
mgr->register_creature_script(5287, &WolfFamily::Create);
mgr->register_creature_script(3939, &WolfFamily::Create);
mgr->register_creature_script(833, &WolfFamily::Create);
mgr->register_creature_script(12431, &WolfFamily::Create);
mgr->register_creature_script(19423, &WolfFamily::Create);
mgr->register_creature_script(19640, &WolfFamily::Create);
mgr->register_creature_script(19458, &WolfFamily::Create);
mgr->register_creature_script(19459, &WolfFamily::Create);
mgr->register_creature_script(1923, &WolfFamily::Create);
mgr->register_creature_script(3861, &WolfFamily::Create);
mgr->register_creature_script(5058, &WolfFamily::Create);
mgr->register_creature_script(2753, &WolfFamily::Create);
mgr->register_creature_script(2275, &WolfFamily::Create);
mgr->register_creature_script(565, &WolfFamily::Create);
mgr->register_creature_script(525, &WolfFamily::Create);
mgr->register_creature_script(299, &WolfFamily::Create);
mgr->register_creature_script(213, &WolfFamily::Create);
mgr->register_creature_script(1138, &WolfFamily::Create);
mgr->register_creature_script(14282, &WolfFamily::Create);
mgr->register_creature_script(69, &WolfFamily::Create);
mgr->register_creature_script(8211, &WolfFamily::Create);
mgr->register_creature_script(14339, &WolfFamily::Create);
mgr->register_creature_script(4950, &WolfFamily::Create);
mgr->register_creature_script(2923, &WolfFamily::Create);
mgr->register_creature_script(9690, &WolfFamily::Create);
mgr->register_creature_script(7055, &WolfFamily::Create);
mgr->register_creature_script(1922, &WolfFamily::Create);
mgr->register_creature_script(10077, &WolfFamily::Create);
mgr->register_creature_script(705, &WolfFamily::Create);
mgr->register_creature_script(1108, &GorillaFamily::Create);
mgr->register_creature_script(6516, &GorillaFamily::Create);
mgr->register_creature_script(1511, &GorillaFamily::Create);
mgr->register_creature_script(1516, &GorillaFamily::Create);
mgr->register_creature_script(6513, &GorillaFamily::Create);
mgr->register_creature_script(6514, &GorillaFamily::Create);
mgr->register_creature_script(12364, &GorillaFamily::Create);
mgr->register_creature_script(12368, &GorillaFamily::Create);
mgr->register_creature_script(14491, &GorillaFamily::Create);
mgr->register_creature_script(5262, &GorillaFamily::Create);
mgr->register_creature_script(2521, &GorillaFamily::Create);
mgr->register_creature_script(1114, &GorillaFamily::Create);
mgr->register_creature_script(5260, &GorillaFamily::Create);
mgr->register_creature_script(1557, &GorillaFamily::Create);
mgr->register_creature_script(6585, &GorillaFamily::Create);
mgr->register_creature_script(9622, &GorillaFamily::Create);
mgr->register_creature_script(1558, &GorillaFamily::Create);
mgr->register_creature_script(3228, &CrabFamily::Create);
mgr->register_creature_script(830, &CrabFamily::Create);
mgr->register_creature_script(3107, &CrabFamily::Create);
mgr->register_creature_script(922, &CrabFamily::Create);
mgr->register_creature_script(1088, &CrabFamily::Create);
mgr->register_creature_script(3812, &CrabFamily::Create);
mgr->register_creature_script(1216, &CrabFamily::Create);
mgr->register_creature_script(3814, &CrabFamily::Create);
mgr->register_creature_script(3108, &CrabFamily::Create);
mgr->register_creature_script(17216, &CrabFamily::Create);
mgr->register_creature_script(2544, &CrabFamily::Create);
mgr->register_creature_script(17217, &CrabFamily::Create);
mgr->register_creature_script(2234, &CrabFamily::Create);
mgr->register_creature_script(2231, &CrabFamily::Create);
mgr->register_creature_script(3106, &CrabFamily::Create);
mgr->register_creature_script(2236, &CrabFamily::Create);
mgr->register_creature_script(4821, &CrabFamily::Create);
mgr->register_creature_script(4822, &CrabFamily::Create);
mgr->register_creature_script(4823, &CrabFamily::Create);
mgr->register_creature_script(3503, &CrabFamily::Create);
mgr->register_creature_script(2233, &CrabFamily::Create);
mgr->register_creature_script(2235, &CrabFamily::Create);
mgr->register_creature_script(2232, &CrabFamily::Create);
mgr->register_creature_script(12347, &CrabFamily::Create);
mgr->register_creature_script(18241, &CrabFamily::Create);
mgr->register_creature_script(6250, &CrabFamily::Create);
mgr->register_creature_script(831, &CrabFamily::Create);
mgr->register_creature_script(5272, &BearFamily::Create);
mgr->register_creature_script(1186, &BearFamily::Create);
mgr->register_creature_script(14344, &BearFamily::Create);
mgr->register_creature_script(1129, &BearFamily::Create);
mgr->register_creature_script(3811, &BearFamily::Create);
mgr->register_creature_script(2351, &BearFamily::Create);
mgr->register_creature_script(3810, &BearFamily::Create);
mgr->register_creature_script(12037, &BearFamily::Create);
mgr->register_creature_script(5352, &BearFamily::Create);
mgr->register_creature_script(7443, &BearFamily::Create);
mgr->register_creature_script(1196, &BearFamily::Create);
mgr->register_creature_script(2163, &BearFamily::Create);
mgr->register_creature_script(1188, &BearFamily::Create);
mgr->register_creature_script(5274, &BearFamily::Create);
mgr->register_creature_script(14280, &BearFamily::Create);
mgr->register_creature_script(8956, &BearFamily::Create);
mgr->register_creature_script(8958, &BearFamily::Create);
mgr->register_creature_script(1797, &BearFamily::Create);
mgr->register_creature_script(1778, &BearFamily::Create);
mgr->register_creature_script(7444, &BearFamily::Create);
mgr->register_creature_script(17348, &BearFamily::Create);
mgr->register_creature_script(17661, &BearFamily::Create);
mgr->register_creature_script(8957, &BearFamily::Create);
mgr->register_creature_script(1816, &BearFamily::Create);
mgr->register_creature_script(2165, &BearFamily::Create);
mgr->register_creature_script(10806, &BearFamily::Create);
mgr->register_creature_script(2354, &BearFamily::Create);
mgr->register_creature_script(2356, &BearFamily::Create);
mgr->register_creature_script(22136, &BearFamily::Create);
mgr->register_creature_script(3809, &BearFamily::Create);
mgr->register_creature_script(1815, &BearFamily::Create);
mgr->register_creature_script(1961, &BearFamily::Create);
mgr->register_creature_script(5268, &BearFamily::Create);
mgr->register_creature_script(7446, &BearFamily::Create);
mgr->register_creature_script(7445, &BearFamily::Create);
mgr->register_creature_script(6789, &BearFamily::Create);
mgr->register_creature_script(12432, &BearFamily::Create);
mgr->register_creature_script(822, &BearFamily::Create);
mgr->register_creature_script(1130, &BearFamily::Create);
mgr->register_creature_script(1128, &BearFamily::Create);
mgr->register_creature_script(1225, &BearFamily::Create);
mgr->register_creature_script(17345, &BearFamily::Create);
mgr->register_creature_script(17347, &BearFamily::Create);
mgr->register_creature_script(6788, &BearFamily::Create);
mgr->register_creature_script(1189, &BearFamily::Create);
mgr->register_creature_script(4514, &BoarFamily::Create);
mgr->register_creature_script(454, &BoarFamily::Create);
mgr->register_creature_script(5992, &BoarFamily::Create);
mgr->register_creature_script(3100, &BoarFamily::Create);
mgr->register_creature_script(1689, &BoarFamily::Create);
mgr->register_creature_script(390, &BoarFamily::Create);
mgr->register_creature_script(1190, &BoarFamily::Create);
mgr->register_creature_script(1984, &BoarFamily::Create);
mgr->register_creature_script(3225, &BoarFamily::Create);
mgr->register_creature_script(547, &BoarFamily::Create);
mgr->register_creature_script(157, &BoarFamily::Create);
mgr->register_creature_script(1192, &BoarFamily::Create);
mgr->register_creature_script(708, &BoarFamily::Create);
mgr->register_creature_script(1127, &BoarFamily::Create);
mgr->register_creature_script(1126, &BoarFamily::Create);
mgr->register_creature_script(4535, &BoarFamily::Create);
mgr->register_creature_script(16117, &BoarFamily::Create);
mgr->register_creature_script(4511, &BoarFamily::Create);
mgr->register_creature_script(4512, &BoarFamily::Create);
mgr->register_creature_script(3099, &BoarFamily::Create);
mgr->register_creature_script(524, &BoarFamily::Create);
mgr->register_creature_script(3098, &BoarFamily::Create);
mgr->register_creature_script(1191, &BoarFamily::Create);
mgr->register_creature_script(345, &BoarFamily::Create);
mgr->register_creature_script(113, &BoarFamily::Create);
mgr->register_creature_script(2966, &BoarFamily::Create);
mgr->register_creature_script(2954, &BoarFamily::Create);
mgr->register_creature_script(119, &BoarFamily::Create);
mgr->register_creature_script(1125, &BoarFamily::Create);
mgr->register_creature_script(330, &BoarFamily::Create);
mgr->register_creature_script(1985, &BoarFamily::Create);
mgr->register_creature_script(8303, &BoarFamily::Create);
//End of family based AI's
//Type based AI's
mgr->register_creature_script(20142, &DragonType::Create);
mgr->register_creature_script(15192, &DragonType::Create);
mgr->register_creature_script(19951, &DragonType::Create);
mgr->register_creature_script(17918, &DragonType::Create);
mgr->register_creature_script(18994, &DragonType::Create);
mgr->register_creature_script(21104, &DragonType::Create);
mgr->register_creature_script(17880, &DragonType::Create);
mgr->register_creature_script(7048, &DragonType::Create);
mgr->register_creature_script(12478, &DragonType::Create);
mgr->register_creature_script(12479, &DragonType::Create);
mgr->register_creature_script(12477, &DragonType::Create);
mgr->register_creature_script(14890, &DragonType::Create);
mgr->register_creature_script(5718, &DragonType::Create);
mgr->register_creature_script(12496, &DragonType::Create);
mgr->register_creature_script(15302, &DragonType::Create);
mgr->register_creature_script(12468, &DragonType::Create);
mgr->register_creature_script(12435, &DragonType::Create);
mgr->register_creature_script(12465, &DragonType::Create);
mgr->register_creature_script(12463, &DragonType::Create);
mgr->register_creature_script(14024, &DragonType::Create);
mgr->register_creature_script(14025, &DragonType::Create);
mgr->register_creature_script(14023, &DragonType::Create);
mgr->register_creature_script(14022, &DragonType::Create);
mgr->register_creature_script(12017, &DragonType::Create);
mgr->register_creature_script(12464, &DragonType::Create);
mgr->register_creature_script(12467, &DragonType::Create);
mgr->register_creature_script(13020, &DragonType::Create);
mgr->register_creature_script(12422, &DragonType::Create);
mgr->register_creature_script(21148, &DragonType::Create);
mgr->register_creature_script(17879, &DragonType::Create);
mgr->register_creature_script(14398, &DragonType::Create);
mgr->register_creature_script(1042, &DragonType::Create);
mgr->register_creature_script(9461, &DragonType::Create);
mgr->register_creature_script(21138, &DragonType::Create);
mgr->register_creature_script(10184, &DragonType::Create);
mgr->register_creature_script(12129, &DragonType::Create);
mgr->register_creature_script(11262, &DragonType::Create);
mgr->register_creature_script(18418, &DragonType::Create);
mgr->register_creature_script(745, &DragonType::Create);
mgr->register_creature_script(743, &DragonType::Create);
mgr->register_creature_script(742, &DragonType::Create);
mgr->register_creature_script(744, &DragonType::Create);
mgr->register_creature_script(12457, &DragonType::Create);
mgr->register_creature_script(14601, &DragonType::Create);
mgr->register_creature_script(11981, &DragonType::Create);
mgr->register_creature_script(13976, &DragonType::Create);
mgr->register_creature_script(10372, &DragonType::Create);
mgr->register_creature_script(10366, &DragonType::Create);
mgr->register_creature_script(20201, &DragonType::Create);
mgr->register_creature_script(19933, &DragonType::Create);
mgr->register_creature_script(6109, &DragonType::Create);
mgr->register_creature_script(15481, &DragonType::Create);
mgr->register_creature_script(6129, &DragonType::Create);
mgr->register_creature_script(6130, &DragonType::Create);
mgr->register_creature_script(193, &DragonType::Create);
mgr->register_creature_script(6131, &DragonType::Create);
mgr->register_creature_script(22332, &DragonType::Create);
mgr->register_creature_script(17307, &DragonType::Create);
mgr->register_creature_script(17536, &DragonType::Create);
mgr->register_creature_script(5314, &DragonType::Create);
mgr->register_creature_script(21818, &DragonType::Create);
mgr->register_creature_script(17892, &DragonType::Create);
mgr->register_creature_script(21140, &DragonType::Create);
mgr->register_creature_script(21137, &DragonType::Create);
mgr->register_creature_script(21136, &DragonType::Create);
mgr->register_creature_script(14889, &DragonType::Create);
mgr->register_creature_script(18692, &DragonType::Create);
mgr->register_creature_script(10162, &DragonType::Create);
mgr->register_creature_script(10371, &DragonType::Create);
mgr->register_creature_script(10363, &DragonType::Create);
mgr->register_creature_script(10814, &DragonType::Create);
mgr->register_creature_script(9096, &DragonType::Create);
mgr->register_creature_script(10083, &DragonType::Create);
mgr->register_creature_script(21004, &DragonType::Create);
mgr->register_creature_script(2447, &DragonType::Create);
mgr->register_creature_script(10161, &DragonType::Create);
mgr->register_creature_script(10442, &DragonType::Create);
mgr->register_creature_script(10447, &DragonType::Create);
mgr->register_creature_script(10740, &DragonType::Create);
mgr->register_creature_script(10664, &DragonType::Create);
mgr->register_creature_script(10929, &DragonType::Create);
mgr->register_creature_script(746, &DragonType::Create);
mgr->register_creature_script(1044, &DragonType::Create);
mgr->register_creature_script(1045, &DragonType::Create);
mgr->register_creature_script(1046, &DragonType::Create);
mgr->register_creature_script(1047, &DragonType::Create);
mgr->register_creature_script(1048, &DragonType::Create);
mgr->register_creature_script(1049, &DragonType::Create);
mgr->register_creature_script(1050, &DragonType::Create);
mgr->register_creature_script(1063, &DragonType::Create);
mgr->register_creature_script(1069, &DragonType::Create);
mgr->register_creature_script(22112, &DragonType::Create);
mgr->register_creature_script(10339, &DragonType::Create);
mgr->register_creature_script(2725, &DragonType::Create);
mgr->register_creature_script(2726, &DragonType::Create);
mgr->register_creature_script(2757, &DragonType::Create);
mgr->register_creature_script(2759, &DragonType::Create);
mgr->register_creature_script(17839, &DragonType::Create);
mgr->register_creature_script(12476, &DragonType::Create);
mgr->register_creature_script(12474, &DragonType::Create);
mgr->register_creature_script(4066, &DragonType::Create);
mgr->register_creature_script(19959, &DragonType::Create);
mgr->register_creature_script(19932, &DragonType::Create);
mgr->register_creature_script(21811, &DragonType::Create);
mgr->register_creature_script(12475, &DragonType::Create);
mgr->register_creature_script(7047, &DragonType::Create);
mgr->register_creature_script(7041, &DragonType::Create);
mgr->register_creature_script(5277, &DragonType::Create);
mgr->register_creature_script(5280, &DragonType::Create);
mgr->register_creature_script(5283, &DragonType::Create);
mgr->register_creature_script(5312, &DragonType::Create);
mgr->register_creature_script(10538, &DragonType::Create);
mgr->register_creature_script(10340, &DragonType::Create);
mgr->register_creature_script(5709, &DragonType::Create);
mgr->register_creature_script(5719, &DragonType::Create);
mgr->register_creature_script(5720, &DragonType::Create);
mgr->register_creature_script(5721, &DragonType::Create);
mgr->register_creature_script(5722, &DragonType::Create);
mgr->register_creature_script(5912, &DragonType::Create);
mgr->register_creature_script(14888, &DragonType::Create);
mgr->register_creature_script(10678, &DragonType::Create);
mgr->register_creature_script(8319, &DragonType::Create);
mgr->register_creature_script(8480, &DragonType::Create);
mgr->register_creature_script(8497, &DragonType::Create);
mgr->register_creature_script(17881, &DragonType::Create);
mgr->register_creature_script(335, &DragonType::Create);
mgr->register_creature_script(8776, &DragonType::Create);
mgr->register_creature_script(8976, &DragonType::Create);
mgr->register_creature_script(10196, &DragonType::Create);
mgr->register_creature_script(10202, &DragonType::Create);
mgr->register_creature_script(10258, &DragonType::Create);
mgr->register_creature_script(10264, &DragonType::Create);
mgr->register_creature_script(10296, &DragonType::Create);
mgr->register_creature_script(10663, &DragonType::Create);
mgr->register_creature_script(10683, &DragonType::Create);
mgr->register_creature_script(12498, &DragonType::Create);
mgr->register_creature_script(11583, &DragonType::Create);
mgr->register_creature_script(17835, &DragonType::Create);
mgr->register_creature_script(12497, &DragonType::Create);
mgr->register_creature_script(12739, &DragonType::Create);
mgr->register_creature_script(12756, &DragonType::Create);
mgr->register_creature_script(12899, &DragonType::Create);
mgr->register_creature_script(12900, &DragonType::Create);
mgr->register_creature_script(14272, &DragonType::Create);
mgr->register_creature_script(14445, &DragonType::Create);
mgr->register_creature_script(9568, &DragonType::Create);
mgr->register_creature_script(12460, &DragonType::Create);
mgr->register_creature_script(14887, &DragonType::Create);
mgr->register_creature_script(8198, &DragonType::Create);
mgr->register_creature_script(14020, &DragonType::Create);
mgr->register_creature_script(11983, &DragonType::Create);
mgr->register_creature_script(441, &DragonType::Create);
mgr->register_creature_script(8479, &DragonType::Create);
mgr->register_creature_script(19438, &DragonType::Create);
mgr->register_creature_script(4323, &DragonType::Create);
mgr->register_creature_script(4328, &DragonType::Create);
mgr->register_creature_script(4324, &DragonType::Create);
mgr->register_creature_script(4329, &DragonType::Create);
mgr->register_creature_script(4331, &DragonType::Create);
mgr->register_creature_script(4339, &DragonType::Create);
mgr->register_creature_script(4334, &DragonType::Create);
mgr->register_creature_script(10321, &DragonType::Create);
mgr->register_creature_script(15378, &DragonType::Create);
mgr->register_creature_script(15379, &DragonType::Create);
mgr->register_creature_script(15380, &DragonType::Create);
mgr->register_creature_script(5276, &DragonType::Create);
mgr->register_creature_script(5319, &DragonType::Create);
mgr->register_creature_script(5317, &DragonType::Create);
mgr->register_creature_script(5320, &DragonType::Create);
mgr->register_creature_script(8197, &DragonType::Create);
mgr->register_creature_script(3815, &DragonType::Create);
mgr->register_creature_script(19918, &DragonType::Create);
mgr->register_creature_script(20130, &DragonType::Create);
mgr->register_creature_script(19935, &DragonType::Create);
mgr->register_creature_script(19936, &DragonType::Create);
mgr->register_creature_script(19950, &DragonType::Create);
mgr->register_creature_script(19934, &DragonType::Create);
mgr->register_creature_script(21648, &DragonType::Create);
mgr->register_creature_script(21657, &DragonType::Create);
mgr->register_creature_script(18995, &DragonType::Create);
mgr->register_creature_script(21497, &DragonType::Create);
mgr->register_creature_script(21387, &DragonType::Create);
mgr->register_creature_script(21492, &DragonType::Create);
mgr->register_creature_script(21389, &DragonType::Create);
mgr->register_creature_script(22108, &DragonType::Create);
mgr->register_creature_script(22130, &DragonType::Create);
mgr->register_creature_script(21722, &DragonType::Create);
mgr->register_creature_script(20129, &DragonType::Create);
mgr->register_creature_script(18725, &DragonType::Create);
mgr->register_creature_script(18723, &DragonType::Create);
mgr->register_creature_script(21032, &DragonType::Create);
mgr->register_creature_script(20911, &DragonType::Create);
mgr->register_creature_script(21139, &DragonType::Create);
mgr->register_creature_script(21823, &DragonType::Create);
mgr->register_creature_script(22106, &DragonType::Create);
mgr->register_creature_script(18171, &DragonType::Create);
mgr->register_creature_script(18172, &DragonType::Create);
mgr->register_creature_script(18170, &DragonType::Create);
mgr->register_creature_script(18096, &DragonType::Create);
mgr->register_creature_script(20910, &DragonType::Create);
mgr->register_creature_script(17652, &DragonType::Create);
mgr->register_creature_script(5278, &DragonType::Create);
mgr->register_creature_script(8662, &DragonType::Create);
mgr->register_creature_script(21820, &DragonType::Create);
mgr->register_creature_script(21817, &DragonType::Create);
mgr->register_creature_script(20021, &DragonType::Create);
mgr->register_creature_script(18877, &DragonType::Create);
mgr->register_creature_script(20332, &DragonType::Create);
mgr->register_creature_script(20903, &DragonType::Create);
mgr->register_creature_script(21721, &DragonType::Create);
mgr->register_creature_script(22072, &DragonType::Create);
mgr->register_creature_script(7997, &DragonType::Create);
mgr->register_creature_script(15185, &DragonType::Create);
mgr->register_creature_script(21821, &DragonType::Create);
mgr->register_creature_script(740, &DragonType::Create);
mgr->register_creature_script(741, &DragonType::Create);
mgr->register_creature_script(10667, &DragonType::Create);
mgr->register_creature_script(4016, &DragonType::Create);
mgr->register_creature_script(4017, &DragonType::Create);
mgr->register_creature_script(8196, &DragonType::Create);
mgr->register_creature_script(20713, &DragonType::Create);
mgr->register_creature_script(1749, &DragonType::Create);
mgr->register_creature_script(8506, &DragonType::Create);
mgr->register_creature_script(7846, &DragonType::Create);
mgr->register_creature_script(10659, &DragonType::Create);
mgr->register_creature_script(7437, &DragonType::Create);
mgr->register_creature_script(10660, &DragonType::Create);
mgr->register_creature_script(7436, &DragonType::Create);
mgr->register_creature_script(7435, &DragonType::Create);
mgr->register_creature_script(10661, &DragonType::Create);
mgr->register_creature_script(10662, &DragonType::Create);
mgr->register_creature_script(7043, &DragonType::Create);
mgr->register_creature_script(7049, &DragonType::Create);
mgr->register_creature_script(14388, &DragonType::Create);
mgr->register_creature_script(8964, &DragonType::Create);
mgr->register_creature_script(7042, &DragonType::Create);
mgr->register_creature_script(7046, &DragonType::Create);
mgr->register_creature_script(7045, &DragonType::Create);
mgr->register_creature_script(7040, &DragonType::Create);
mgr->register_creature_script(7044, &DragonType::Create);
mgr->register_creature_script(9459, &DragonType::Create);
mgr->register_creature_script(1043, &DragonType::Create);
mgr->register_creature_script(15381, &DragonType::Create);
mgr->register_creature_script(22360, &DragonType::Create);
mgr->register_creature_script(22496, &DragonType::Create);
mgr->register_creature_script(15491, &DragonType::Create);
mgr->register_creature_script(18544, &DragonType::Create);
mgr->register_creature_script(21510, &DragonType::Create);
mgr->register_creature_script(22000, &DragonType::Create);
mgr->register_creature_script(21698, &DragonType::Create);
mgr->register_creature_script(21697, &DragonType::Create);
mgr->register_creature_script(15628, &DragonType::Create);
mgr->register_creature_script(16597, &DragonType::Create);
mgr->register_creature_script(18543, &DragonType::Create);
mgr->register_creature_script(19582, &MechanicalType::Create);
mgr->register_creature_script(19588, &MechanicalType::Create);
mgr->register_creature_script(21233, &MechanicalType::Create);
mgr->register_creature_script(9656, &MechanicalType::Create);
mgr->register_creature_script(5202, &MechanicalType::Create);
mgr->register_creature_script(6233, &MechanicalType::Create);
mgr->register_creature_script(2673, &MechanicalType::Create);
mgr->register_creature_script(9657, &MechanicalType::Create);
mgr->register_creature_script(18733, &MechanicalType::Create);
mgr->register_creature_script(17059, &MechanicalType::Create);
mgr->register_creature_script(17578, &MechanicalType::Create);
mgr->register_creature_script(20869, &MechanicalType::Create);
mgr->register_creature_script(17178, &MechanicalType::Create);
mgr->register_creature_script(20243, &MechanicalType::Create);
mgr->register_creature_script(16111, &MechanicalType::Create);
mgr->register_creature_script(3538, &MechanicalType::Create);
mgr->register_creature_script(4872, &MechanicalType::Create);
mgr->register_creature_script(7023, &MechanicalType::Create);
mgr->register_creature_script(5674, &MechanicalType::Create);
mgr->register_creature_script(19139, &MechanicalType::Create);
mgr->register_creature_script(22196, &MechanicalType::Create);
mgr->register_creature_script(22293, &MechanicalType::Create);
mgr->register_creature_script(19589, &MechanicalType::Create);
mgr->register_creature_script(20392, &MechanicalType::Create);
mgr->register_creature_script(19692, &MechanicalType::Create);
mgr->register_creature_script(7527, &MechanicalType::Create);
mgr->register_creature_script(13416, &MechanicalType::Create);
mgr->register_creature_script(8035, &MechanicalType::Create);
mgr->register_creature_script(573, &MechanicalType::Create);
mgr->register_creature_script(6225, &MechanicalType::Create);
mgr->register_creature_script(6235, &MechanicalType::Create);
mgr->register_creature_script(21976, &MechanicalType::Create);
mgr->register_creature_script(12473, &MechanicalType::Create);
mgr->register_creature_script(2678, &MechanicalType::Create);
mgr->register_creature_script(19210, &MechanicalType::Create);
mgr->register_creature_script(7915, &MechanicalType::Create);
mgr->register_creature_script(6227, &MechanicalType::Create);
mgr->register_creature_script(6229, &MechanicalType::Create);
mgr->register_creature_script(6230, &MechanicalType::Create);
mgr->register_creature_script(6232, &MechanicalType::Create);
mgr->register_creature_script(5723, &MechanicalType::Create);
mgr->register_creature_script(6234, &MechanicalType::Create);
mgr->register_creature_script(6231, &MechanicalType::Create);
mgr->register_creature_script(6386, &MechanicalType::Create);
mgr->register_creature_script(7784, &MechanicalType::Create);
mgr->register_creature_script(7806, &MechanicalType::Create);
mgr->register_creature_script(2667, &MechanicalType::Create);
mgr->register_creature_script(7209, &MechanicalType::Create);
mgr->register_creature_script(9623, &MechanicalType::Create);
mgr->register_creature_script(17543, &MechanicalType::Create);
mgr->register_creature_script(7849, &MechanicalType::Create);
mgr->register_creature_script(7800, &MechanicalType::Create);
mgr->register_creature_script(17547, &MechanicalType::Create);
mgr->register_creature_script(7897, &MechanicalType::Create);
mgr->register_creature_script(11875, &MechanicalType::Create);
mgr->register_creature_script(12364, &MechanicalType::Create);
mgr->register_creature_script(12368, &MechanicalType::Create);
mgr->register_creature_script(12385, &MechanicalType::Create);
mgr->register_creature_script(12426, &MechanicalType::Create);
mgr->register_creature_script(13378, &MechanicalType::Create);
mgr->register_creature_script(14224, &MechanicalType::Create);
mgr->register_creature_script(15328, &MechanicalType::Create);
mgr->register_creature_script(15368, &MechanicalType::Create);
mgr->register_creature_script(22122, &MechanicalType::Create);
mgr->register_creature_script(17796, &MechanicalType::Create);
mgr->register_creature_script(17954, &MechanicalType::Create);
mgr->register_creature_script(4945, &MechanicalType::Create);
mgr->register_creature_script(4251, &MechanicalType::Create);
mgr->register_creature_script(16504, &MechanicalType::Create);
mgr->register_creature_script(4252, &MechanicalType::Create);
mgr->register_creature_script(21761, &MechanicalType::Create);
mgr->register_creature_script(6226, &MechanicalType::Create);
mgr->register_creature_script(8447, &MechanicalType::Create);
mgr->register_creature_script(16121, &MechanicalType::Create);
mgr->register_creature_script(14337, &MechanicalType::Create);
mgr->register_creature_script(15691, &MechanicalType::Create);
mgr->register_creature_script(16485, &MechanicalType::Create);
mgr->register_creature_script(8836, &MechanicalType::Create);
mgr->register_creature_script(8615, &MechanicalType::Create);
mgr->register_creature_script(7807, &MechanicalType::Create);
mgr->register_creature_script(15079, &MechanicalType::Create);
mgr->register_creature_script(18405, &MechanicalType::Create);
mgr->register_creature_script(16211, &MechanicalType::Create);
mgr->register_creature_script(22461, &MechanicalType::Create);
mgr->register_creature_script(19849, &MechanicalType::Create);
mgr->register_creature_script(4946, &MechanicalType::Create);
mgr->register_creature_script(21949, &MechanicalType::Create);
mgr->register_creature_script(19851, &MechanicalType::Create);
mgr->register_creature_script(21404, &MechanicalType::Create);
mgr->register_creature_script(17711, &MechanicalType::Create);
mgr->register_creature_script(20076, &MechanicalType::Create);
mgr->register_creature_script(11684, &MechanicalType::Create);
mgr->register_creature_script(2674, &MechanicalType::Create);
mgr->register_creature_script(22443, &MechanicalType::Create);
mgr->register_creature_script(22451, &MechanicalType::Create);
mgr->register_creature_script(22295, &MechanicalType::Create);
mgr->register_creature_script(22389, &MechanicalType::Create);
mgr->register_creature_script(20420, &MechanicalType::Create);
mgr->register_creature_script(7166, &MechanicalType::Create);
mgr->register_creature_script(5652, &MechanicalType::Create);
mgr->register_creature_script(4260, &MechanicalType::Create);
mgr->register_creature_script(21426, &MechanicalType::Create);
mgr->register_creature_script(22067, &MechanicalType::Create);
mgr->register_creature_script(22064, &MechanicalType::Create);
mgr->register_creature_script(22315, &MechanicalType::Create);
mgr->register_creature_script(19166, &MechanicalType::Create);
mgr->register_creature_script(19735, &MechanicalType::Create);
mgr->register_creature_script(19219, &MechanicalType::Create);
mgr->register_creature_script(20478, &MechanicalType::Create);
mgr->register_creature_script(19399, &MechanicalType::Create);
mgr->register_creature_script(19400, &MechanicalType::Create);
mgr->register_creature_script(17060, &MechanicalType::Create);
mgr->register_creature_script(2676, &MechanicalType::Create);
mgr->register_creature_script(4074, &MechanicalType::Create);
mgr->register_creature_script(4073, &MechanicalType::Create);
mgr->register_creature_script(480, &MechanicalType::Create);
mgr->register_creature_script(36, &MechanicalType::Create);
mgr->register_creature_script(114, &MechanicalType::Create);
mgr->register_creature_script(8856, &MechanicalType::Create);
mgr->register_creature_script(12363, &MechanicalType::Create);
mgr->register_creature_script(12367, &MechanicalType::Create);
mgr->register_creature_script(12365, &MechanicalType::Create);
mgr->register_creature_script(14553, &MechanicalType::Create);
mgr->register_creature_script(14552, &MechanicalType::Create);
mgr->register_creature_script(12366, &MechanicalType::Create);
mgr->register_creature_script(14551, &MechanicalType::Create);
mgr->register_creature_script(19405, &MechanicalType::Create);
mgr->register_creature_script(21690, &MechanicalType::Create);
mgr->register_creature_script(6669, &MechanicalType::Create);
mgr->register_creature_script(22085, &MechanicalType::Create);
mgr->register_creature_script(11199, &MechanicalType::Create);
mgr->register_creature_script(2922, &MechanicalType::Create);
mgr->register_creature_script(4952, &MechanicalType::Create);
mgr->register_creature_script(115, &MechanicalType::Create);
mgr->register_creature_script(642, &MechanicalType::Create);
mgr->register_creature_script(2520, &MechanicalType::Create);
mgr->register_creature_script(25, &MechanicalType::Create);
mgr->register_creature_script(15104, &MechanicalType::Create);
mgr->register_creature_script(15319, &MechanicalType::Create);
mgr->register_creature_script(15323, &MechanicalType::Create);
mgr->register_creature_script(15333, &MechanicalType::Create);
mgr->register_creature_script(15336, &MechanicalType::Create);
mgr->register_creature_script(15340, &MechanicalType::Create);
mgr->register_creature_script(15370, &MechanicalType::Create);
mgr->register_creature_script(15428, &MechanicalType::Create);
mgr->register_creature_script(15505, &MechanicalType::Create);
mgr->register_creature_script(15963, &MechanicalType::Create);
mgr->register_creature_script(15964, &MechanicalType::Create);
mgr->register_creature_script(17299, &MechanicalType::Create);
mgr->register_creature_script(20041, &MechanicalType::Create);
mgr->register_creature_script(20040, &MechanicalType::Create);
mgr->register_creature_script(19516, &MechanicalType::Create);
mgr->register_creature_script(17706, &MechanicalType::Create);
mgr->register_creature_script(15935, &MechanicalType::Create);
mgr->register_creature_script(19552, &MechanicalType::Create);
mgr->register_creature_script(20779, &UnknownType::Create);
mgr->register_creature_script(15883, &UnknownType::Create);
mgr->register_creature_script(15882, &UnknownType::Create);
mgr->register_creature_script(16364, &UnknownType::Create);
mgr->register_creature_script(19475, &UnknownType::Create);
mgr->register_creature_script(6509, &UnknownType::Create);
mgr->register_creature_script(20208, &UnknownType::Create);
mgr->register_creature_script(11729, &UnknownType::Create);
mgr->register_creature_script(11726, &UnknownType::Create);
mgr->register_creature_script(11728, &UnknownType::Create);
mgr->register_creature_script(11725, &UnknownType::Create);
mgr->register_creature_script(6556, &UnknownType::Create);
mgr->register_creature_script(17408, &UnknownType::Create);
mgr->register_creature_script(22422, &UnknownType::Create);
mgr->register_creature_script(20736, &UnknownType::Create);
mgr->register_creature_script(22068, &UnknownType::Create);
mgr->register_creature_script(22065, &UnknownType::Create);
mgr->register_creature_script(22069, &UnknownType::Create);
mgr->register_creature_script(2992, &UnknownType::Create);
mgr->register_creature_script(21899, &UnknownType::Create);
mgr->register_creature_script(21039, &UnknownType::Create);
mgr->register_creature_script(20767, &UnknownType::Create);
mgr->register_creature_script(15324, &UnknownType::Create);
mgr->register_creature_script(15047, &UnknownType::Create);
mgr->register_creature_script(4130, &UnknownType::Create);
mgr->register_creature_script(15343, &UnknownType::Create);
mgr->register_creature_script(18133, &UnknownType::Create);
mgr->register_creature_script(15608, &UnknownType::Create);
mgr->register_creature_script(21862, &UnknownType::Create);
mgr->register_creature_script(10415, &UnknownType::Create);
mgr->register_creature_script(18781, &UnknownType::Create);
mgr->register_creature_script(19680, &UnknownType::Create);
mgr->register_creature_script(20808, &UnknownType::Create);
mgr->register_creature_script(19165, &UnknownType::Create);
mgr->register_creature_script(19688, &UnknownType::Create);
mgr->register_creature_script(18481, &UnknownType::Create);
mgr->register_creature_script(18593, &UnknownType::Create);
mgr->register_creature_script(18228, &UnknownType::Create);
mgr->register_creature_script(18842, &UnknownType::Create);
mgr->register_creature_script(12276, &UnknownType::Create);
mgr->register_creature_script(15288, &UnknownType::Create);
mgr->register_creature_script(15290, &UnknownType::Create);
mgr->register_creature_script(15286, &UnknownType::Create);
mgr->register_creature_script(6219, &UnknownType::Create);
mgr->register_creature_script(16047, &UnknownType::Create);
mgr->register_creature_script(20796, &UnknownType::Create);
mgr->register_creature_script(19869, &UnknownType::Create);
mgr->register_creature_script(21101, &UnknownType::Create);
mgr->register_creature_script(21346, &UnknownType::Create);
mgr->register_creature_script(20865, &UnknownType::Create);
mgr->register_creature_script(20864, &UnknownType::Create);
mgr->register_creature_script(20780, &UnknownType::Create);
mgr->register_creature_script(14453, &UnknownType::Create);
mgr->register_creature_script(19686, &UnknownType::Create);
mgr->register_creature_script(14449, &UnknownType::Create);
mgr->register_creature_script(14459, &UnknownType::Create);
mgr->register_creature_script(6554, &UnknownType::Create);
mgr->register_creature_script(10925, &UnknownType::Create);
mgr->register_creature_script(16899, &UnknownType::Create);
mgr->register_creature_script(20023, &UnknownType::Create);
mgr->register_creature_script(20024, &UnknownType::Create);
mgr->register_creature_script(20003, &UnknownType::Create);
mgr->register_creature_script(2656, &UnknownType::Create);
mgr->register_creature_script(1032, &UnknownType::Create);
mgr->register_creature_script(16897, &UnknownType::Create);
mgr->register_creature_script(18504, &UnknownType::Create);
mgr->register_creature_script(18560, &UnknownType::Create);
mgr->register_creature_script(15894, &UnknownType::Create);
mgr->register_creature_script(15893, &UnknownType::Create);
mgr->register_creature_script(6348, &UnknownType::Create);
mgr->register_creature_script(6140, &UnknownType::Create);
mgr->register_creature_script(20809, &UnknownType::Create);
mgr->register_creature_script(19301, &UnknownType::Create);
mgr->register_creature_script(19302, &UnknownType::Create);
mgr->register_creature_script(19303, &UnknownType::Create);
mgr->register_creature_script(19304, &UnknownType::Create);
mgr->register_creature_script(19913, &UnknownType::Create);
mgr->register_creature_script(17687, &UnknownType::Create);
mgr->register_creature_script(19032, &UnknownType::Create);
mgr->register_creature_script(17795, &UnknownType::Create);
mgr->register_creature_script(17794, &UnknownType::Create);
mgr->register_creature_script(19029, &UnknownType::Create);
mgr->register_creature_script(19028, &UnknownType::Create);
mgr->register_creature_script(17356, &UnknownType::Create);
mgr->register_creature_script(17357, &UnknownType::Create);
mgr->register_creature_script(17552, &UnknownType::Create);
mgr->register_creature_script(18370, &UnknownType::Create);
mgr->register_creature_script(17471, &UnknownType::Create);
mgr->register_creature_script(18782, &UnknownType::Create);
mgr->register_creature_script(21395, &UnknownType::Create);
mgr->register_creature_script(16968, &UnknownType::Create);
mgr->register_creature_script(11101, &UnknownType::Create);
mgr->register_creature_script(19646, &UnknownType::Create);
mgr->register_creature_script(14848, &UnknownType::Create);
mgr->register_creature_script(18818, &UnknownType::Create);
mgr->register_creature_script(19466, &UnknownType::Create);
mgr->register_creature_script(15091, &UnknownType::Create);
mgr->register_creature_script(18381, &UnknownType::Create);
mgr->register_creature_script(14466, &UnknownType::Create);
mgr->register_creature_script(2462, &UnknownType::Create);
mgr->register_creature_script(16046, &UnknownType::Create);
mgr->register_creature_script(14646, &UnknownType::Create);
mgr->register_creature_script(19193, &UnknownType::Create);
mgr->register_creature_script(7863, &UnknownType::Create);
mgr->register_creature_script(18563, &UnknownType::Create);
mgr->register_creature_script(19437, &UnknownType::Create);
mgr->register_creature_script(19421, &UnknownType::Create);
mgr->register_creature_script(18967, &UnknownType::Create);
mgr->register_creature_script(20617, &UnknownType::Create);
mgr->register_creature_script(18240, &UnknownType::Create);
mgr->register_creature_script(8766, &UnknownType::Create);
mgr->register_creature_script(15064, &UnknownType::Create);
mgr->register_creature_script(17984, &UnknownType::Create);
mgr->register_creature_script(22336, &UnknownType::Create);
mgr->register_creature_script(18913, &UnknownType::Create);
mgr->register_creature_script(19272, &UnknownType::Create);
mgr->register_creature_script(19216, &UnknownType::Create);
mgr->register_creature_script(19179, &UnknownType::Create);
mgr->register_creature_script(12219, &UnknownType::Create);
mgr->register_creature_script(12220, &UnknownType::Create);
mgr->register_creature_script(12222, &UnknownType::Create);
mgr->register_creature_script(12258, &UnknownType::Create);
mgr->register_creature_script(12221, &UnknownType::Create);
mgr->register_creature_script(3560, &UnknownType::Create);
mgr->register_creature_script(7483, &UnknownType::Create);
mgr->register_creature_script(19845, &UnknownType::Create);
mgr->register_creature_script(19651, &UnknownType::Create);
mgr->register_creature_script(22063, &UnknownType::Create);
mgr->register_creature_script(21974, &UnknownType::Create);
mgr->register_creature_script(20162, &UnknownType::Create);
mgr->register_creature_script(21872, &UnknownType::Create);
mgr->register_creature_script(22395, &UnknownType::Create);
mgr->register_creature_script(22417, &UnknownType::Create);
mgr->register_creature_script(22437, &UnknownType::Create);
mgr->register_creature_script(21944, &UnknownType::Create);
mgr->register_creature_script(18589, &UnknownType::Create);
mgr->register_creature_script(16243, &UnknownType::Create);
mgr->register_creature_script(18600, &UnknownType::Create);
mgr->register_creature_script(21186, &UnknownType::Create);
mgr->register_creature_script(21903, &UnknownType::Create);
mgr->register_creature_script(21957, &UnknownType::Create);
mgr->register_creature_script(21997, &UnknownType::Create);
mgr->register_creature_script(1030, &UnknownType::Create);
mgr->register_creature_script(19067, &UnknownType::Create);
mgr->register_creature_script(19212, &UnknownType::Create);
mgr->register_creature_script(18532, &UnknownType::Create);
mgr->register_creature_script(16079, &UnknownType::Create);
mgr->register_creature_script(22358, &UnknownType::Create);
mgr->register_creature_script(21814, &UnknownType::Create);
mgr->register_creature_script(22090, &UnknownType::Create);
mgr->register_creature_script(19660, &UnknownType::Create);
mgr->register_creature_script(18904, &UnknownType::Create);
mgr->register_creature_script(18215, &UnknownType::Create);
mgr->register_creature_script(18208, &UnknownType::Create);
mgr->register_creature_script(18840, &UnknownType::Create);
mgr->register_creature_script(20289, &UnknownType::Create);
mgr->register_creature_script(16085, &UnknownType::Create);
mgr->register_creature_script(18688, &UnknownType::Create);
mgr->register_creature_script(19063, &UnknownType::Create);
mgr->register_creature_script(18538, &UnknownType::Create);
mgr->register_creature_script(19009, &UnknownType::Create);
mgr->register_creature_script(21236, &UnknownType::Create);
mgr->register_creature_script(16901, &UnknownType::Create);
mgr->register_creature_script(16903, &UnknownType::Create);
mgr->register_creature_script(14345, &UnknownType::Create);
mgr->register_creature_script(21849, &UnknownType::Create);
mgr->register_creature_script(19433, &UnknownType::Create);
mgr->register_creature_script(19939, &UnknownType::Create);
mgr->register_creature_script(19727, &UnknownType::Create);
mgr->register_creature_script(19731, &UnknownType::Create);
mgr->register_creature_script(21898, &UnknownType::Create);
mgr->register_creature_script(5451, &UnknownType::Create);
mgr->register_creature_script(15897, &UnknownType::Create);
mgr->register_creature_script(18530, &UnknownType::Create);
mgr->register_creature_script(8149, &UnknownType::Create);
mgr->register_creature_script(7785, &UnknownType::Create);
mgr->register_creature_script(7732, &UnknownType::Create);
mgr->register_creature_script(19655, &UnknownType::Create);
mgr->register_creature_script(15063, &UnknownType::Create);
mgr->register_creature_script(21203, &UnknownType::Create);
mgr->register_creature_script(4469, &UnknownType::Create);
mgr->register_creature_script(22400, &UnknownType::Create);
mgr->register_creature_script(21858, &UnknownType::Create);
mgr->register_creature_script(22407, &UnknownType::Create);
mgr->register_creature_script(19467, &UnknownType::Create);
mgr->register_creature_script(21305, &UnknownType::Create);
mgr->register_creature_script(13876, &UnknownType::Create);
mgr->register_creature_script(17690, &UnknownType::Create);
mgr->register_creature_script(15390, &UnknownType::Create);
mgr->register_creature_script(21996, &UnknownType::Create);
mgr->register_creature_script(21237, &UnknownType::Create);
mgr->register_creature_script(18778, &UnknownType::Create);
mgr->register_creature_script(19523, &UnknownType::Create);
mgr->register_creature_script(19524, &UnknownType::Create);
mgr->register_creature_script(15384, &UnknownType::Create);
mgr->register_creature_script(20709, &UnknownType::Create);
mgr->register_creature_script(17611, &UnknownType::Create);
mgr->register_creature_script(15089, &UnknownType::Create);
mgr->register_creature_script(15107, &UnknownType::Create);
mgr->register_creature_script(15086, &UnknownType::Create);
mgr->register_creature_script(15075, &UnknownType::Create);
mgr->register_creature_script(15074, &UnknownType::Create);
mgr->register_creature_script(15108, &UnknownType::Create);
mgr->register_creature_script(15087, &UnknownType::Create);
mgr->register_creature_script(15062, &UnknownType::Create);
mgr->register_creature_script(15046, &UnknownType::Create);
mgr->register_creature_script(22021, &UnknownType::Create);
mgr->register_creature_script(18263, &UnknownType::Create);
mgr->register_creature_script(19211, &UnknownType::Create);
mgr->register_creature_script(18264, &UnknownType::Create);
mgr->register_creature_script(19469, &UnknownType::Create);
mgr->register_creature_script(19468, &UnknownType::Create);
mgr->register_creature_script(19390, &UnknownType::Create);
mgr->register_creature_script(1806, &UnknownType::Create);
mgr->register_creature_script(1808, &UnknownType::Create);
mgr->register_creature_script(6555, &UnknownType::Create);
mgr->register_creature_script(21262, &UnknownType::Create);
mgr->register_creature_script(21261, &UnknownType::Create);
mgr->register_creature_script(21334, &UnknownType::Create);
mgr->register_creature_script(21856, &UnknownType::Create);
mgr->register_creature_script(21855, &UnknownType::Create);
mgr->register_creature_script(4541, &UnknownType::Create);
mgr->register_creature_script(19654, &UnknownType::Create);
mgr->register_creature_script(8120, &UnknownType::Create);
mgr->register_creature_script(14732, &UnknownType::Create);
mgr->register_creature_script(20851, &UnknownType::Create);
mgr->register_creature_script(21159, &UnknownType::Create);
mgr->register_creature_script(18793, &UnknownType::Create);
mgr->register_creature_script(3652, &UnknownType::Create);
mgr->register_creature_script(18625, &UnknownType::Create);
mgr->register_creature_script(18553, &UnknownType::Create);
mgr->register_creature_script(18555, &UnknownType::Create);
mgr->register_creature_script(17838, &UnknownType::Create);
mgr->register_creature_script(14894, &UnknownType::Create);
mgr->register_creature_script(17310, &UnknownType::Create);
mgr->register_creature_script(4392, &UnknownType::Create);
mgr->register_creature_script(4393, &UnknownType::Create);
mgr->register_creature_script(19050, &UnknownType::Create);
mgr->register_creature_script(19161, &UnknownType::Create);
mgr->register_creature_script(19377, &UnknownType::Create);
mgr->register_creature_script(15045, &UnknownType::Create);
mgr->register_creature_script(21999, &UnknownType::Create);
mgr->register_creature_script(4795, &UnknownType::Create);
mgr->register_creature_script(17066, &UnknownType::Create);
mgr->register_creature_script(15902, &UnknownType::Create);
mgr->register_creature_script(5228, &UnknownType::Create);
mgr->register_creature_script(5235, &UnknownType::Create);
mgr->register_creature_script(16044, &UnknownType::Create);
mgr->register_creature_script(5350, &UnknownType::Create);
mgr->register_creature_script(5441, &UnknownType::Create);
mgr->register_creature_script(5450, &UnknownType::Create);
mgr->register_creature_script(5452, &UnknownType::Create);
mgr->register_creature_script(5453, &UnknownType::Create);
mgr->register_creature_script(5454, &UnknownType::Create);
mgr->register_creature_script(21935, &UnknownType::Create);
mgr->register_creature_script(15325, &UnknownType::Create);
mgr->register_creature_script(22002, &UnknownType::Create);
mgr->register_creature_script(12247, &UnknownType::Create);
mgr->register_creature_script(12252, &UnknownType::Create);
mgr->register_creature_script(16431, &UnknownType::Create);
mgr->register_creature_script(6347, &UnknownType::Create);
mgr->register_creature_script(6582, &UnknownType::Create);
mgr->register_creature_script(21798, &UnknownType::Create);
mgr->register_creature_script(21760, &UnknownType::Create);
mgr->register_creature_script(7412, &UnknownType::Create);
mgr->register_creature_script(7413, &UnknownType::Create);
mgr->register_creature_script(7423, &UnknownType::Create);
mgr->register_creature_script(7424, &UnknownType::Create);
mgr->register_creature_script(16844, &UnknownType::Create);
mgr->register_creature_script(16092, &UnknownType::Create);
mgr->register_creature_script(7468, &UnknownType::Create);
mgr->register_creature_script(18726, &UnknownType::Create);
mgr->register_creature_script(7768, &UnknownType::Create);
mgr->register_creature_script(7769, &UnknownType::Create);
mgr->register_creature_script(16048, &UnknownType::Create);
mgr->register_creature_script(8204, &UnknownType::Create);
mgr->register_creature_script(8205, &UnknownType::Create);
mgr->register_creature_script(8212, &UnknownType::Create);
mgr->register_creature_script(8257, &UnknownType::Create);
mgr->register_creature_script(8311, &UnknownType::Create);
mgr->register_creature_script(21512, &UnknownType::Create);
mgr->register_creature_script(8437, &UnknownType::Create);
mgr->register_creature_script(8438, &UnknownType::Create);
mgr->register_creature_script(8440, &UnknownType::Create);
mgr->register_creature_script(8443, &UnknownType::Create);
mgr->register_creature_script(8510, &UnknownType::Create);
mgr->register_creature_script(19427, &UnknownType::Create);
mgr->register_creature_script(21759, &UnknownType::Create);
mgr->register_creature_script(9157, &UnknownType::Create);
mgr->register_creature_script(9436, &UnknownType::Create);
mgr->register_creature_script(9477, &UnknownType::Create);
mgr->register_creature_script(9496, &UnknownType::Create);
mgr->register_creature_script(9498, &UnknownType::Create);
mgr->register_creature_script(6218, &UnknownType::Create);
mgr->register_creature_script(16592, &UnknownType::Create);
mgr->register_creature_script(9621, &UnknownType::Create);
mgr->register_creature_script(9707, &UnknownType::Create);
mgr->register_creature_script(10040, &UnknownType::Create);
mgr->register_creature_script(10041, &UnknownType::Create);
mgr->register_creature_script(10183, &UnknownType::Create);
mgr->register_creature_script(10217, &UnknownType::Create);
mgr->register_creature_script(10290, &UnknownType::Create);
mgr->register_creature_script(20333, &UnknownType::Create);
mgr->register_creature_script(20922, &UnknownType::Create);
mgr->register_creature_script(12251, &UnknownType::Create);
mgr->register_creature_script(10697, &UnknownType::Create);
mgr->register_creature_script(21234, &UnknownType::Create);
mgr->register_creature_script(8607, &UnknownType::Create);
mgr->register_creature_script(11100, &UnknownType::Create);
mgr->register_creature_script(11122, &UnknownType::Create);
mgr->register_creature_script(11136, &UnknownType::Create);
mgr->register_creature_script(17535, &UnknownType::Create);
mgr->register_creature_script(15224, &UnknownType::Create);
mgr->register_creature_script(11623, &UnknownType::Create);
mgr->register_creature_script(18162, &UnknownType::Create);
mgr->register_creature_script(12381, &UnknownType::Create);
mgr->register_creature_script(12382, &UnknownType::Create);
mgr->register_creature_script(12387, &UnknownType::Create);
mgr->register_creature_script(19300, &UnknownType::Create);
mgr->register_creature_script(13136, &UnknownType::Create);
mgr->register_creature_script(13301, &UnknownType::Create);
mgr->register_creature_script(13602, &UnknownType::Create);
mgr->register_creature_script(13636, &UnknownType::Create);
mgr->register_creature_script(13756, &UnknownType::Create);
mgr->register_creature_script(13778, &UnknownType::Create);
mgr->register_creature_script(13796, &UnknownType::Create);
mgr->register_creature_script(13916, &UnknownType::Create);
mgr->register_creature_script(13936, &UnknownType::Create);
mgr->register_creature_script(14027, &UnknownType::Create);
mgr->register_creature_script(14028, &UnknownType::Create);
mgr->register_creature_script(8606, &UnknownType::Create);
mgr->register_creature_script(14081, &UnknownType::Create);
mgr->register_creature_script(18537, &UnknownType::Create);
mgr->register_creature_script(14235, &UnknownType::Create);
mgr->register_creature_script(16467, &UnknownType::Create);
mgr->register_creature_script(16465, &UnknownType::Create);
mgr->register_creature_script(14370, &UnknownType::Create);
mgr->register_creature_script(14396, &UnknownType::Create);
mgr->register_creature_script(22194, &UnknownType::Create);
mgr->register_creature_script(14434, &UnknownType::Create);
mgr->register_creature_script(14473, &UnknownType::Create);
mgr->register_creature_script(14474, &UnknownType::Create);
mgr->register_creature_script(14475, &UnknownType::Create);
mgr->register_creature_script(14495, &UnknownType::Create);
mgr->register_creature_script(14524, &UnknownType::Create);
mgr->register_creature_script(14525, &UnknownType::Create);
mgr->register_creature_script(14526, &UnknownType::Create);
mgr->register_creature_script(14661, &UnknownType::Create);
mgr->register_creature_script(14751, &UnknownType::Create);
mgr->register_creature_script(14752, &UnknownType::Create);
mgr->register_creature_script(14761, &UnknownType::Create);
mgr->register_creature_script(15001, &UnknownType::Create);
mgr->register_creature_script(15002, &UnknownType::Create);
mgr->register_creature_script(15003, &UnknownType::Create);
mgr->register_creature_script(15004, &UnknownType::Create);
mgr->register_creature_script(15005, &UnknownType::Create);
mgr->register_creature_script(15069, &UnknownType::Create);
mgr->register_creature_script(15073, &UnknownType::Create);
mgr->register_creature_script(15113, &UnknownType::Create);
mgr->register_creature_script(15115, &UnknownType::Create);
mgr->register_creature_script(15122, &UnknownType::Create);
mgr->register_creature_script(15207, &UnknownType::Create);
mgr->register_creature_script(15218, &UnknownType::Create);
mgr->register_creature_script(15260, &UnknownType::Create);
mgr->register_creature_script(15261, &UnknownType::Create);
mgr->register_creature_script(15305, &UnknownType::Create);
mgr->register_creature_script(15363, &UnknownType::Create);
mgr->register_creature_script(15415, &UnknownType::Create);
mgr->register_creature_script(15425, &UnknownType::Create);
mgr->register_creature_script(19483, &UnknownType::Create);
mgr->register_creature_script(19439, &UnknownType::Create);
mgr->register_creature_script(19484, &UnknownType::Create);
mgr->register_creature_script(20336, &UnknownType::Create);
mgr->register_creature_script(15730, &UnknownType::Create);
mgr->register_creature_script(15624, &UnknownType::Create);
mgr->register_creature_script(15901, &UnknownType::Create);
mgr->register_creature_script(20978, &UnknownType::Create);
mgr->register_creature_script(18531, &UnknownType::Create);
mgr->register_creature_script(22497, &UnknownType::Create);
mgr->register_creature_script(21893, &UnknownType::Create);
mgr->register_creature_script(18549, &UnknownType::Create);
mgr->register_creature_script(19142, &UnknownType::Create);
mgr->register_creature_script(15335, &UnknownType::Create);
mgr->register_creature_script(18132, &UnknownType::Create);
mgr->register_creature_script(22126, &UnknownType::Create);
mgr->register_creature_script(22094, &UnknownType::Create);
mgr->register_creature_script(22439, &UnknownType::Create);
mgr->register_creature_script(22438, &UnknownType::Create);
mgr->register_creature_script(22124, &UnknownType::Create);
mgr->register_creature_script(18395, &UnknownType::Create);
mgr->register_creature_script(18393, &UnknownType::Create);
mgr->register_creature_script(18841, &UnknownType::Create);
mgr->register_creature_script(15391, &UnknownType::Create);
mgr->register_creature_script(17915, &UnknownType::Create);
mgr->register_creature_script(22125, &UnknownType::Create);
mgr->register_creature_script(22440, &UnknownType::Create);
mgr->register_creature_script(15241, &UnknownType::Create);
mgr->register_creature_script(18597, &UnknownType::Create);
mgr->register_creature_script(18757, &UnknownType::Create);
mgr->register_creature_script(19668, &UnknownType::Create);
mgr->register_creature_script(17378, &UnknownType::Create);
mgr->register_creature_script(20926, &UnknownType::Create);
mgr->register_creature_script(14994, &UnknownType::Create);
mgr->register_creature_script(15168, &UnknownType::Create);
mgr->register_creature_script(15348, &UnknownType::Create);
mgr->register_creature_script(19008, &UnknownType::Create);
mgr->register_creature_script(20093, &UnknownType::Create);
mgr->register_creature_script(3398, &UnknownType::Create);
mgr->register_creature_script(20214, &UnknownType::Create);
mgr->register_creature_script(21347, &UnknownType::Create);
mgr->register_creature_script(21756, &UnknownType::Create);
mgr->register_creature_script(14443, &UnknownType::Create);
mgr->register_creature_script(15387, &UnknownType::Create);
mgr->register_creature_script(2334, &UnknownType::Create);
mgr->register_creature_script(17847, &UnknownType::Create);
mgr->register_creature_script(17544, &UnknownType::Create);
mgr->register_creature_script(15344, &UnknownType::Create);
mgr->register_creature_script(7092, &UnknownType::Create);
mgr->register_creature_script(18654, &UnknownType::Create);
mgr->register_creature_script(15389, &UnknownType::Create);
mgr->register_creature_script(19525, &UnknownType::Create);
mgr->register_creature_script(15392, &UnknownType::Create);
mgr->register_creature_script(15385, &UnknownType::Create);
mgr->register_creature_script(10902, &UnknownType::Create);
mgr->register_creature_script(15386, &UnknownType::Create);
mgr->register_creature_script(21414, &UnknownType::Create);
mgr->register_creature_script(15327, &UnknownType::Create);
mgr->register_creature_script(22001, &UnknownType::Create);
mgr->register_creature_script(21993, &UnknownType::Create);
mgr->register_creature_script(2615, &UnknownType::Create);
mgr->register_creature_script(18594, &UnknownType::Create);
mgr->register_creature_script(18547, &UnknownType::Create);
mgr->register_creature_script(18568, &UnknownType::Create);
mgr->register_creature_script(22317, &UnknownType::Create);
mgr->register_creature_script(21699, &UnknownType::Create);
mgr->register_creature_script(17985, &UnknownType::Create);
mgr->register_creature_script(19224, &UnknownType::Create);
mgr->register_creature_script(17645, &UnknownType::Create);
mgr->register_creature_script(16457, &UnknownType::Create);
mgr->register_creature_script(17260, &UnknownType::Create);
mgr->register_creature_script(21402, &UnknownType::Create);
mgr->register_creature_script(19528, &UnknownType::Create);
mgr->register_creature_script(21822, &UnknownType::Create);
mgr->register_creature_script(19532, &UnknownType::Create);
mgr->register_creature_script(15449, &UnknownType::Create);
mgr->register_creature_script(20286, &UnknownType::Create);
mgr->register_creature_script(20288, &UnknownType::Create);
mgr->register_creature_script(21075, &UnknownType::Create);
mgr->register_creature_script(19215, &UnknownType::Create);
mgr->register_creature_script(19291, &UnknownType::Create);
mgr->register_creature_script(19871, &UnknownType::Create);
mgr->register_creature_script(18968, &UnknownType::Create);
mgr->register_creature_script(19358, &UnknownType::Create);
mgr->register_creature_script(19276, &UnknownType::Create);
mgr->register_creature_script(19717, &UnknownType::Create);
mgr->register_creature_script(19277, &UnknownType::Create);
mgr->register_creature_script(6349, &UnknownType::Create);
mgr->register_creature_script(4391, &UnknownType::Create);
mgr->register_creature_script(10718, &UnknownType::Create);
mgr->register_creature_script(15454, &UnknownType::Create);
mgr->register_creature_script(15801, &UnknownType::Create);
mgr->register_creature_script(15247, &UnknownType::Create);
mgr->register_creature_script(15233, &UnknownType::Create);
mgr->register_creature_script(15262, &UnknownType::Create);
mgr->register_creature_script(15264, &UnknownType::Create);
mgr->register_creature_script(15263, &UnknownType::Create);
mgr->register_creature_script(15426, &UnknownType::Create);
mgr->register_creature_script(15230, &UnknownType::Create);
mgr->register_creature_script(15896, &UnknownType::Create);
mgr->register_creature_script(15589, &UnknownType::Create);
mgr->register_creature_script(15727, &UnknownType::Create);
mgr->register_creature_script(15511, &UnknownType::Create);
mgr->register_creature_script(15544, &UnknownType::Create);
mgr->register_creature_script(15543, &UnknownType::Create);
mgr->register_creature_script(15802, &UnknownType::Create);
mgr->register_creature_script(15622, &UnknownType::Create);
mgr->register_creature_script(15509, &UnknownType::Create);
mgr->register_creature_script(15933, &UnknownType::Create);
mgr->register_creature_script(15621, &UnknownType::Create);
mgr->register_creature_script(15516, &UnknownType::Create);
mgr->register_creature_script(15984, &UnknownType::Create);
mgr->register_creature_script(15300, &UnknownType::Create);
mgr->register_creature_script(15229, &UnknownType::Create);
mgr->register_creature_script(15620, &UnknownType::Create);
mgr->register_creature_script(18144, &UnknownType::Create);
mgr->register_creature_script(7273, &UnknownType::Create);
mgr->register_creature_script(5246, &UnknownType::Create);
mgr->register_creature_script(5247, &UnknownType::Create);
mgr->register_creature_script(5244, &UnknownType::Create);
mgr->register_creature_script(5245, &UnknownType::Create);
mgr->register_creature_script(2655, &UnknownType::Create);
mgr->register_creature_script(15112, &UnknownType::Create);
mgr->register_creature_script(14987, &UnknownType::Create);
mgr->register_creature_script(15141, &UnknownType::Create);
mgr->register_creature_script(15114, &UnknownType::Create);
mgr->register_creature_script(15140, &UnknownType::Create);
mgr->register_creature_script(16466, &UnknownType::Create);
mgr->register_creature_script(15388, &UnknownType::Create);
mgr->register_creature_script(15341, &UnknownType::Create);
mgr->register_creature_script(16420, &UnknownType::Create);
mgr->register_creature_script(16137, &UnknownType::Create);
mgr->register_creature_script(16400, &UnknownType::Create);
mgr->register_creature_script(16082, &UnknownType::Create);
mgr->register_creature_script(16142, &UnknownType::Create);
mgr->register_creature_script(16363, &UnknownType::Create);
mgr->register_creature_script(16218, &UnknownType::Create);
mgr->register_creature_script(16297, &UnknownType::Create);
mgr->register_creature_script(17231, &UnknownType::Create);
mgr->register_creature_script(16236, &UnknownType::Create);
mgr->register_creature_script(17293, &UnknownType::Create);
mgr->register_creature_script(16698, &UnknownType::Create);
mgr->register_creature_script(16486, &UnknownType::Create);
mgr->register_creature_script(15510, &UnknownType::Create);
mgr->register_creature_script(15240, &UnknownType::Create);
mgr->register_creature_script(15962, &UnknownType::Create);
mgr->register_creature_script(15630, &UnknownType::Create);
mgr->register_creature_script(15236, &UnknownType::Create);
mgr->register_creature_script(15235, &UnknownType::Create);
mgr->register_creature_script(15249, &UnknownType::Create);
mgr->register_creature_script(15277, &UnknownType::Create);
mgr->register_creature_script(15317, &UnknownType::Create);
mgr->register_creature_script(15537, &UnknownType::Create);
mgr->register_creature_script(15316, &UnknownType::Create);
mgr->register_creature_script(15538, &UnknownType::Create);
mgr->register_creature_script(15250, &UnknownType::Create);
mgr->register_creature_script(15252, &UnknownType::Create);
mgr->register_creature_script(15246, &UnknownType::Create);
mgr->register_creature_script(15276, &UnknownType::Create);
mgr->register_creature_script(15275, &UnknownType::Create);
mgr->register_creature_script(15695, &UnknownType::Create);
mgr->register_creature_script(15957, &UnknownType::Create);
mgr->register_creature_script(15517, &UnknownType::Create);
mgr->register_creature_script(15712, &UnknownType::Create);
mgr->register_creature_script(15718, &UnknownType::Create);
mgr->register_creature_script(16604, &UnknownType::Create);
mgr->register_creature_script(14309, &UnknownType::Create);
mgr->register_creature_script(14310, &UnknownType::Create);
mgr->register_creature_script(16697, &UnknownType::Create);
mgr->register_creature_script(15311, &UnknownType::Create);
mgr->register_creature_script(15312, &UnknownType::Create);
mgr->register_creature_script(14311, &UnknownType::Create);
mgr->register_creature_script(15904, &UnknownType::Create);
mgr->register_creature_script(15725, &UnknownType::Create);
mgr->register_creature_script(15726, &UnknownType::Create);
mgr->register_creature_script(15728, &UnknownType::Create);
mgr->register_creature_script(15910, &UnknownType::Create);
mgr->register_creature_script(15334, &UnknownType::Create);
mgr->register_creature_script(15800, &UnknownType::Create);
mgr->register_creature_script(16286, &UnknownType::Create);
mgr->register_creature_script(17698, &UnknownType::Create);
mgr->register_creature_script(16980, &UnknownType::Create);
mgr->register_creature_script(16129, &UnknownType::Create);
mgr->register_creature_script(16474, &UnknownType::Create);
mgr->register_creature_script(17025, &UnknownType::Create);
mgr->register_creature_script(12434, &UnknownType::Create);
mgr->register_creature_script(14307, &UnknownType::Create);
mgr->register_creature_script(14312, &UnknownType::Create);
mgr->register_creature_script(17090, &UnknownType::Create);
mgr->register_creature_script(18225, &UnknownType::Create);
mgr->register_creature_script(18283, &UnknownType::Create);
mgr->register_creature_script(18275, &UnknownType::Create);
mgr->register_creature_script(15468, &UnknownType::Create);
mgr->register_creature_script(17047, &UnknownType::Create);
mgr->register_creature_script(22003, &UnknownType::Create);
mgr->register_creature_script(12999, &UnknownType::Create);
mgr->register_creature_script(16857, &UnknownType::Create);
mgr->register_creature_script(15242, &UnknownType::Create);
mgr->register_creature_script(18729, &UnknownType::Create);
mgr->register_creature_script(19656, &UnknownType::Create);
mgr->register_creature_script(21429, &UnknownType::Create);
mgr->register_creature_script(4133, &UnknownType::Create);
mgr->register_creature_script(4468, &UnknownType::Create);
mgr->register_creature_script(6033, &UnknownType::Create);
mgr->register_creature_script(21096, &UnknownType::Create);
mgr->register_creature_script(21807, &UnknownType::Create);
mgr->register_creature_script(11326, &UnknownType::Create);
mgr->register_creature_script(21380, &UnknownType::Create);
mgr->register_creature_script(21030, &UnknownType::Create);
mgr->register_creature_script(22146, &UnknownType::Create);
mgr->register_creature_script(21443, &UnknownType::Create);
mgr->register_creature_script(19387, &UnknownType::Create);
mgr->register_creature_script(19677, &UnknownType::Create);
mgr->register_creature_script(19381, &UnknownType::Create);
mgr->register_creature_script(19388, &UnknownType::Create);
mgr->register_creature_script(12256, &UnknownType::Create);
mgr->register_creature_script(17364, &UnknownType::Create);
mgr->register_creature_script(16922, &UnknownType::Create);
mgr->register_creature_script(20239, &UnknownType::Create);
mgr->register_creature_script(17947, &UnknownType::Create);
mgr->register_creature_script(17362, &UnknownType::Create);
mgr->register_creature_script(17363, &UnknownType::Create);
mgr->register_creature_script(17274, &UnknownType::Create);
mgr->register_creature_script(17361, &UnknownType::Create);
mgr->register_creature_script(18173, &UnknownType::Create);
mgr->register_creature_script(17680, &UnknownType::Create);
mgr->register_creature_script(17889, &UnknownType::Create);
mgr->register_creature_script(17979, &UnknownType::Create);
mgr->register_creature_script(17850, &UnknownType::Create);
mgr->register_creature_script(17529, &UnknownType::Create);
mgr->register_creature_script(18896, &UnknownType::Create);
mgr->register_creature_script(21403, &UnknownType::Create);
mgr->register_creature_script(21417, &UnknownType::Create);
mgr->register_creature_script(21348, &UnknownType::Create);
mgr->register_creature_script(21080, &UnknownType::Create);
mgr->register_creature_script(20431, &UnknownType::Create);
mgr->register_creature_script(21953, &UnknownType::Create);
mgr->register_creature_script(19504, &UnknownType::Create);
mgr->register_creature_script(22066, &UnknownType::Create);
mgr->register_creature_script(21954, &UnknownType::Create);
mgr->register_creature_script(22071, &UnknownType::Create);
mgr->register_creature_script(19521, &UnknownType::Create);
mgr->register_creature_script(22134, &UnknownType::Create);
mgr->register_creature_script(22230, &UnknownType::Create);
mgr->register_creature_script(21924, &UnknownType::Create);
mgr->register_creature_script(21909, &UnknownType::Create);
mgr->register_creature_script(17407, &UnknownType::Create);
mgr->register_creature_script(22080, &UnknownType::Create);
mgr->register_creature_script(22078, &UnknownType::Create);
mgr->register_creature_script(21986, &UnknownType::Create);
mgr->register_creature_script(19526, &UnknownType::Create);
mgr->register_creature_script(22079, &UnknownType::Create);
mgr->register_creature_script(22214, &UnknownType::Create);
mgr->register_creature_script(22077, &UnknownType::Create);
mgr->register_creature_script(19530, &UnknownType::Create);
mgr->register_creature_script(22070, &UnknownType::Create);
mgr->register_creature_script(19518, &UnknownType::Create);
mgr->register_creature_script(22211, &UnknownType::Create);
mgr->register_creature_script(21955, &UnknownType::Create);
mgr->register_creature_script(19520, &UnknownType::Create);
mgr->register_creature_script(19517, &UnknownType::Create);
mgr->register_creature_script(21095, &UnknownType::Create);
mgr->register_creature_script(21071, &UnknownType::Create);
mgr->register_creature_script(21094, &UnknownType::Create);
mgr->register_creature_script(21092, &UnknownType::Create);
mgr->register_creature_script(21052, &UnknownType::Create);
mgr->register_creature_script(21310, &UnknownType::Create);
mgr->register_creature_script(16898, &UnknownType::Create);
mgr->register_creature_script(21655, &UnknownType::Create);
mgr->register_creature_script(19412, &UnknownType::Create);
mgr->register_creature_script(21211, &UnknownType::Create);
mgr->register_creature_script(21840, &UnknownType::Create);
mgr->register_creature_script(14645, &UnknownType::Create);
mgr->register_creature_script(21892, &UnknownType::Create);
mgr->register_creature_script(22228, &UnknownType::Create);
mgr->register_creature_script(18759, &UnknownType::Create);
mgr->register_creature_script(22039, &UnknownType::Create);
mgr->register_creature_script(18000, &UnknownType::Create);
mgr->register_creature_script(20197, &UnknownType::Create);
mgr->register_creature_script(17998, &UnknownType::Create);
mgr->register_creature_script(10218, &UnknownType::Create);
mgr->register_creature_script(17999, &UnknownType::Create);
mgr->register_creature_script(3928, &UnknownType::Create);
mgr->register_creature_script(22040, &UnknownType::Create);
mgr->register_creature_script(22116, &UnknownType::Create);
mgr->register_creature_script(22801, &UnknownType::Create);
mgr->register_creature_script(21393, &UnknownType::Create);
mgr->register_creature_script(21942, &UnknownType::Create);
mgr->register_creature_script(21447, &UnknownType::Create);
mgr->register_creature_script(21877, &UnknownType::Create);
mgr->register_creature_script(21876, &UnknownType::Create);
mgr->register_creature_script(18650, &UnknownType::Create);
mgr->register_creature_script(7093, &UnknownType::Create);
mgr->register_creature_script(18143, &UnknownType::Create);
mgr->register_creature_script(17474, &UnknownType::Create);
mgr->register_creature_script(17689, &UnknownType::Create);
mgr->register_creature_script(19620, &UnknownType::Create);
mgr->register_creature_script(19619, &UnknownType::Create);
mgr->register_creature_script(19618, &UnknownType::Create);
mgr->register_creature_script(20417, &UnknownType::Create);
mgr->register_creature_script(21210, &UnknownType::Create);
mgr->register_creature_script(18142, &UnknownType::Create);
mgr->register_creature_script(20296, &UnknownType::Create);
mgr->register_creature_script(18110, &UnknownType::Create);
mgr->register_creature_script(19159, &UnknownType::Create);
mgr->register_creature_script(21120, &UnknownType::Create);
mgr->register_creature_script(21119, &UnknownType::Create);
mgr->register_creature_script(20982, &UnknownType::Create);
mgr->register_creature_script(21157, &UnknownType::Create);
mgr->register_creature_script(20475, &UnknownType::Create);
mgr->register_creature_script(20476, &UnknownType::Create);
mgr->register_creature_script(20473, &UnknownType::Create);
mgr->register_creature_script(22351, &UnknownType::Create);
mgr->register_creature_script(22350, &UnknownType::Create);
mgr->register_creature_script(22348, &UnknownType::Create);
mgr->register_creature_script(20804, &UnknownType::Create);
mgr->register_creature_script(20920, &UnknownType::Create);
mgr->register_creature_script(20608, &UnknownType::Create);
mgr->register_creature_script(20335, &UnknownType::Create);
mgr->register_creature_script(20554, &UnknownType::Create);
mgr->register_creature_script(20564, &UnknownType::Create);
mgr->register_creature_script(20899, &UnknownType::Create);
mgr->register_creature_script(19565, &UnknownType::Create);
mgr->register_creature_script(22502, &UnknownType::Create);
mgr->register_creature_script(21930, &UnknownType::Create);
mgr->register_creature_script(22046, &UnknownType::Create);
mgr->register_creature_script(22799, &UnknownType::Create);
mgr->register_creature_script(22800, &UnknownType::Create);
mgr->register_creature_script(21851, &UnknownType::Create);
mgr->register_creature_script(22798, &UnknownType::Create);
mgr->register_creature_script(22447, &UnknownType::Create);
mgr->register_creature_script(22368, &UnknownType::Create);
mgr->register_creature_script(22356, &UnknownType::Create);
mgr->register_creature_script(22367, &UnknownType::Create);
mgr->register_creature_script(4196, &UnknownType::Create);
mgr->register_creature_script(20085, &UnknownType::Create);
mgr->register_creature_script(20209, &UnknownType::Create);
mgr->register_creature_script(20806, &UnknownType::Create);
mgr->register_creature_script(15164, &UnknownType::Create);
mgr->register_creature_script(20553, &UnknownType::Create);
mgr->register_creature_script(21265, &UnknownType::Create);
mgr->register_creature_script(20852, &UnknownType::Create);
mgr->register_creature_script(22436, &UnknownType::Create);
mgr->register_creature_script(22182, &UnknownType::Create);
mgr->register_creature_script(22282, &UnknownType::Create);
mgr->register_creature_script(22320, &UnknownType::Create);
mgr->register_creature_script(21467, &UnknownType::Create);
mgr->register_creature_script(20912, &UnknownType::Create);
mgr->register_creature_script(21466, &UnknownType::Create);
mgr->register_creature_script(21440, &UnknownType::Create);
mgr->register_creature_script(21437, &UnknownType::Create);
mgr->register_creature_script(21439, &UnknownType::Create);
mgr->register_creature_script(21438, &UnknownType::Create);
mgr->register_creature_script(21436, &UnknownType::Create);
mgr->register_creature_script(20155, &UnknownType::Create);
mgr->register_creature_script(7086, &UnknownType::Create);
mgr->register_creature_script(17992, &UnknownType::Create);
mgr->register_creature_script(17990, &UnknownType::Create);
mgr->register_creature_script(19632, &UnknownType::Create);
mgr->register_creature_script(17732, &UnknownType::Create);
mgr->register_creature_script(18185, &UnknownType::Create);
mgr->register_creature_script(20198, &UnknownType::Create);
mgr->register_creature_script(20337, &UnknownType::Create);
mgr->register_creature_script(22288, &UnknownType::Create);
mgr->register_creature_script(20391, &UnknownType::Create);
mgr->register_creature_script(20807, &UnknownType::Create);
mgr->register_creature_script(18766, &UnknownType::Create);
mgr->register_creature_script(15631, &UnknownType::Create);
mgr->register_creature_script(21861, &UnknownType::Create);
mgr->register_creature_script(21394, &UnknownType::Create);
mgr->register_creature_script(21381, &UnknownType::Create);
mgr->register_creature_script(21025, &UnknownType::Create);
mgr->register_creature_script(21051, &UnknownType::Create);
mgr->register_creature_script(21946, &UnknownType::Create);
mgr->register_creature_script(18161, &UnknownType::Create);
mgr->register_creature_script(20755, &UnknownType::Create);
mgr->register_creature_script(20764, &UnknownType::Create);
mgr->register_creature_script(20778, &UnknownType::Create);
mgr->register_creature_script(20769, &UnknownType::Create);
mgr->register_creature_script(19687, &UnknownType::Create);
mgr->register_creature_script(20331, &UnknownType::Create);
mgr->register_creature_script(21432, &UnknownType::Create);
mgr->register_creature_script(18525, &UnknownType::Create);
mgr->register_creature_script(19318, &UnknownType::Create);
mgr->register_creature_script(19337, &UnknownType::Create);
mgr->register_creature_script(19153, &UnknownType::Create);
mgr->register_creature_script(19321, &UnknownType::Create);
mgr->register_creature_script(19590, &UnknownType::Create);
mgr->register_creature_script(22008, &UnknownType::Create);
mgr->register_creature_script(20156, &UnknownType::Create);
mgr->register_creature_script(21322, &UnknownType::Create);
mgr->register_creature_script(20418, &UnknownType::Create);
mgr->register_creature_script(19924, &UnknownType::Create);
mgr->register_creature_script(19916, &UnknownType::Create);
mgr->register_creature_script(19870, &UnknownType::Create);
mgr->register_creature_script(19842, &UnknownType::Create);
mgr->register_creature_script(21264, &UnknownType::Create);
mgr->register_creature_script(20440, &UnknownType::Create);
mgr->register_creature_script(19555, &UnknownType::Create);
mgr->register_creature_script(20061, &UnknownType::Create);
mgr->register_creature_script(19346, &UnknownType::Create);
mgr->register_creature_script(22089, &UnknownType::Create);
mgr->register_creature_script(18506, &UnknownType::Create);
mgr->register_creature_script(18374, &UnknownType::Create);
mgr->register_creature_script(11152, &UnknownType::Create);
mgr->register_creature_script(10905, &UnknownType::Create);
mgr->register_creature_script(10904, &UnknownType::Create);
mgr->register_creature_script(10903, &UnknownType::Create);
mgr->register_creature_script(20805, &UnknownType::Create);
mgr->register_creature_script(21286, &UnknownType::Create);
mgr->register_creature_script(20782, &UnknownType::Create);
mgr->register_creature_script(20520, &UnknownType::Create);
mgr->register_creature_script(20825, &UnknownType::Create);
mgr->register_creature_script(19034, &UnknownType::Create);
mgr->register_creature_script(18596, &UnknownType::Create);
mgr->register_creature_script(19198, &UnknownType::Create);
mgr->register_creature_script(18412, &UnknownType::Create);
mgr->register_creature_script(16595, &UnknownType::Create);
mgr->register_creature_script(22519, &UnknownType::Create);
mgr->register_creature_script(22520, &UnknownType::Create);
mgr->register_creature_script(17316, &UnknownType::Create);
mgr->register_creature_script(17317, &UnknownType::Create);
mgr->register_creature_script(17208, &UnknownType::Create);
mgr->register_creature_script(17305, &UnknownType::Create);
mgr->register_creature_script(22523, &UnknownType::Create);
mgr->register_creature_script(16596, &UnknownType::Create);
mgr->register_creature_script(22524, &UnknownType::Create);
mgr->register_creature_script(21921, &UnknownType::Create);
mgr->register_creature_script(17459, &UnknownType::Create);
mgr->register_creature_script(17174, &UnknownType::Create);
mgr->register_creature_script(17171, &UnknownType::Create);
mgr->register_creature_script(17175, &UnknownType::Create);
mgr->register_creature_script(17096, &UnknownType::Create);
mgr->register_creature_script(19781, &UnknownType::Create);
mgr->register_creature_script(19782, &UnknownType::Create);
mgr->register_creature_script(17161, &UnknownType::Create);
mgr->register_creature_script(19783, &UnknownType::Create);
mgr->register_creature_script(17176, &UnknownType::Create);
mgr->register_creature_script(17168, &UnknownType::Create);
mgr->register_creature_script(17169, &UnknownType::Create);
mgr->register_creature_script(17172, &UnknownType::Create);
mgr->register_creature_script(17170, &UnknownType::Create);
mgr->register_creature_script(17173, &UnknownType::Create);
mgr->register_creature_script(17283, &UnknownType::Create);
mgr->register_creature_script(22521, &UnknownType::Create);
mgr->register_creature_script(17644, &UnknownType::Create);
mgr->register_creature_script(17650, &UnknownType::Create);
mgr->register_creature_script(17265, &UnknownType::Create);
mgr->register_creature_script(17248, &UnknownType::Create);
mgr->register_creature_script(17368, &UnknownType::Create);
mgr->register_creature_script(17367, &UnknownType::Create);
mgr->register_creature_script(17369, &UnknownType::Create);
mgr->register_creature_script(21933, &UnknownType::Create);
mgr->register_creature_script(22057, &UnknownType::Create);
mgr->register_creature_script(12207, &UnknownType::Create);
mgr->register_creature_script(8611, &UnknownType::Create);
mgr->register_creature_script(20338, &UnknownType::Create);
mgr->register_creature_script(20226, &UnknownType::Create);
mgr->register_creature_script(15918, &UnknownType::Create);
mgr->register_creature_script(15879, &UnknownType::Create);
mgr->register_creature_script(15872, &UnknownType::Create);
mgr->register_creature_script(14465, &UnknownType::Create);
mgr->register_creature_script(22139, &UnknownType::Create);
mgr->register_creature_script(22137, &UnknownType::Create);
mgr->register_creature_script(21335, &UnknownType::Create);
mgr->register_creature_script(20670, &UnknownType::Create);
mgr->register_creature_script(20853, &UnknownType::Create);
mgr->register_creature_script(20668, &UnknownType::Create);
mgr->register_creature_script(20771, &UnknownType::Create);
mgr->register_creature_script(20855, &UnknownType::Create);
mgr->register_creature_script(20635, &UnknownType::Create);
mgr->register_creature_script(20845, &UnknownType::Create);
mgr->register_creature_script(20666, &UnknownType::Create);
mgr->register_creature_script(20856, &UnknownType::Create);
mgr->register_creature_script(21053, &UnknownType::Create);
mgr->register_creature_script(20933, &UnknownType::Create);
mgr->register_creature_script(20153, &UnknownType::Create);
mgr->register_creature_script(20451, &UnknownType::Create);
mgr->register_creature_script(19336, &UnknownType::Create);
mgr->register_creature_script(20501, &UnknownType::Create);
mgr->register_creature_script(20676, &UnknownType::Create);
mgr->register_creature_script(20858, &UnknownType::Create);
mgr->register_creature_script(20619, &UnknownType::Create);
mgr->register_creature_script(20482, &UnknownType::Create);
mgr->register_creature_script(20340, &UnknownType::Create);
mgr->register_creature_script(21940, &UnknownType::Create);
mgr->register_creature_script(21939, &UnknownType::Create);
mgr->register_creature_script(21654, &UnknownType::Create);
mgr->register_creature_script(21793, &UnknownType::Create);
mgr->register_creature_script(21792, &UnknownType::Create);
mgr->register_creature_script(21794, &UnknownType::Create);
mgr->register_creature_script(21791, &UnknownType::Create);
mgr->register_creature_script(20405, &UnknownType::Create);
mgr->register_creature_script(22096, &UnknownType::Create);
mgr->register_creature_script(22058, &UnknownType::Create);
mgr->register_creature_script(22121, &UnknownType::Create);
mgr->register_creature_script(21422, &UnknownType::Create);
mgr->register_creature_script(21413, &UnknownType::Create);
mgr->register_creature_script(21819, &UnknownType::Create);
mgr->register_creature_script(11731, &UnknownType::Create);
mgr->register_creature_script(11730, &UnknownType::Create);
mgr->register_creature_script(11732, &UnknownType::Create);
mgr->register_creature_script(11733, &UnknownType::Create);
mgr->register_creature_script(11698, &UnknownType::Create);
mgr->register_creature_script(17074, &UnknownType::Create);
mgr->register_creature_script(11724, &UnknownType::Create);
mgr->register_creature_script(11723, &UnknownType::Create);
mgr->register_creature_script(11722, &UnknownType::Create);
mgr->register_creature_script(11721, &UnknownType::Create);
mgr->register_creature_script(18199, &UnknownType::Create);
mgr->register_creature_script(11734, &UnknownType::Create);
mgr->register_creature_script(19723, &UnknownType::Create);
mgr->register_creature_script(19724, &UnknownType::Create);
mgr->register_creature_script(19867, &UnknownType::Create);
mgr->register_creature_script(19547, &UnknownType::Create);
mgr->register_creature_script(19868, &UnknownType::Create);
mgr->register_creature_script(19839, &UnknownType::Create);
mgr->register_creature_script(19840, &UnknownType::Create);
mgr->register_creature_script(21290, &UnknownType::Create);
mgr->register_creature_script(19549, &UnknownType::Create);
mgr->register_creature_script(19548, &UnknownType::Create);
mgr->register_creature_script(19550, &UnknownType::Create);
mgr->register_creature_script(19866, &UnknownType::Create);
mgr->register_creature_script(19041, &UnknownType::Create);
mgr->register_creature_script(19652, &UnknownType::Create);
mgr->register_creature_script(19278, &UnknownType::Create);
mgr->register_creature_script(19838, &UnknownType::Create);
mgr->register_creature_script(19279, &UnknownType::Create);
mgr->register_creature_script(19359, &UnknownType::Create);
mgr->register_creature_script(19292, &UnknownType::Create);
mgr->register_creature_script(21365, &UnknownType::Create);
mgr->register_creature_script(18849, &UnknownType::Create);
mgr->register_creature_script(18828, &UnknownType::Create);
mgr->register_creature_script(21391, &UnknownType::Create);
mgr->register_creature_script(22177, &UnknownType::Create);
mgr->register_creature_script(21182, &UnknownType::Create);
mgr->register_creature_script(20816, &UnknownType::Create);
mgr->register_creature_script(19440, &UnknownType::Create);
mgr->register_creature_script(20814, &UnknownType::Create);
mgr->register_creature_script(22467, &UnknownType::Create);
mgr->register_creature_script(19393, &UnknownType::Create);
mgr->register_creature_script(22224, &UnknownType::Create);
mgr->register_creature_script(19681, &UnknownType::Create);
mgr->register_creature_script(19376, &UnknownType::Create);
mgr->register_creature_script(20143, &UnknownType::Create);
mgr->register_creature_script(22401, &UnknownType::Create);
mgr->register_creature_script(21173, &UnknownType::Create);
mgr->register_creature_script(22402, &UnknownType::Create);
mgr->register_creature_script(18678, &UnknownType::Create);
mgr->register_creature_script(20781, &UnknownType::Create);
mgr->register_creature_script(22258, &UnknownType::Create);
mgr->register_creature_script(22260, &UnknownType::Create);
mgr->register_creature_script(22259, &UnknownType::Create);
mgr->register_creature_script(22267, &UnknownType::Create);
mgr->register_creature_script(22273, &UnknownType::Create);
mgr->register_creature_script(20813, &UnknownType::Create);
mgr->register_creature_script(20229, &UnknownType::Create);
mgr->register_creature_script(20815, &UnknownType::Create);
mgr->register_creature_script(15705, &UnknownType::Create);
mgr->register_creature_script(22403, &UnknownType::Create);
mgr->register_creature_script(22087, &UnknownType::Create);
mgr->register_creature_script(22088, &UnknownType::Create);
mgr->register_creature_script(22086, &UnknownType::Create);
mgr->register_creature_script(14834, &UnknownType::Create);
mgr->register_creature_script(14758, &UnknownType::Create);
mgr->register_creature_script(14989, &UnknownType::Create);
mgr->register_creature_script(16006, &UnknownType::Create);
mgr->register_creature_script(17662, &UnknownType::Create);
mgr->register_creature_script(18002, &UnknownType::Create);
mgr->register_creature_script(3638, &UnknownType::Create);
mgr->register_creature_script(3640, &UnknownType::Create);
mgr->register_creature_script(5763, &UnknownType::Create);
mgr->register_creature_script(5914, &UnknownType::Create);
mgr->register_creature_script(3721, &UnknownType::Create);
mgr->register_creature_script(4829, &UnknownType::Create);
mgr->register_creature_script(5780, &UnknownType::Create);
mgr->register_creature_script(3295, &UnknownType::Create);
mgr->register_creature_script(17538, &UnknownType::Create);
mgr->register_creature_script(18955, &UnknownType::Create);
mgr->register_creature_script(18814, &UnknownType::Create);
mgr->register_creature_script(17360, &UnknownType::Create);
mgr->register_creature_script(17437, &UnknownType::Create);
mgr->register_creature_script(17438, &UnknownType::Create);
mgr->register_creature_script(5781, &UnknownType::Create);
mgr->register_creature_script(3253, &UnknownType::Create);
mgr->register_creature_script(3844, &UnknownType::Create);
mgr->register_creature_script(4020, &UnknownType::Create);
mgr->register_creature_script(4021, &UnknownType::Create);
mgr->register_creature_script(4059, &UnknownType::Create);
mgr->register_creature_script(12940, &UnknownType::Create);
mgr->register_creature_script(3722, &UnknownType::Create);
mgr->register_creature_script(4131, &UnknownType::Create);
mgr->register_creature_script(4132, &UnknownType::Create);
mgr->register_creature_script(20479, &UnknownType::Create);
mgr->register_creature_script(22419, &UnknownType::Create);
mgr->register_creature_script(21451, &UnknownType::Create);
mgr->register_creature_script(21489, &UnknownType::Create);
mgr->register_creature_script(21463, &UnknownType::Create);
mgr->register_creature_script(18843, &UnknownType::Create);
mgr->register_creature_script(22340, &UnknownType::Create);
mgr->register_creature_script(22453, &UnknownType::Create);
mgr->register_creature_script(21859, &UnknownType::Create);
mgr->register_creature_script(21846, &UnknownType::Create);
mgr->register_creature_script(22449, &UnknownType::Create);
mgr->register_creature_script(22462, &UnknownType::Create);
mgr->register_creature_script(22463, &UnknownType::Create);
mgr->register_creature_script(22355, &UnknownType::Create);
mgr->register_creature_script(22371, &UnknownType::Create);
mgr->register_creature_script(22466, &UnknownType::Create);
mgr->register_creature_script(22482, &UnknownType::Create);
mgr->register_creature_script(21967, &UnknownType::Create);
mgr->register_creature_script(21445, &UnknownType::Create);
mgr->register_creature_script(21632, &UnknownType::Create);
mgr->register_creature_script(21424, &UnknownType::Create);
mgr->register_creature_script(22038, &UnknownType::Create);
mgr->register_creature_script(18305, &UnknownType::Create);
mgr->register_creature_script(18307, &UnknownType::Create);
mgr->register_creature_script(18306, &UnknownType::Create);
mgr->register_creature_script(18388, &UnknownType::Create);
mgr->register_creature_script(18308, &UnknownType::Create);
mgr->register_creature_script(19480, &UnknownType::Create);
mgr->register_creature_script(18590, &UnknownType::Create);
mgr->register_creature_script(18444, &UnknownType::Create);
mgr->register_creature_script(18196, &UnknownType::Create);
mgr->register_creature_script(18662, &UnknownType::Create);
mgr->register_creature_script(17545, &UnknownType::Create);
mgr->register_creature_script(18914, &UnknownType::Create);
mgr->register_creature_script(18354, &UnknownType::Create);
mgr->register_creature_script(21074, &UnknownType::Create);
mgr->register_creature_script(20230, &UnknownType::Create);
mgr->register_creature_script(17253, &UnknownType::Create);
mgr->register_creature_script(14026, &UnknownType::Create);
mgr->register_creature_script(14030, &UnknownType::Create);
mgr->register_creature_script(14031, &UnknownType::Create);
mgr->register_creature_script(14029, &UnknownType::Create);
mgr->register_creature_script(4785, &UnknownType::Create);
mgr->register_creature_script(15222, &UnknownType::Create);
mgr->register_creature_script(15221, &UnknownType::Create);
mgr->register_creature_script(22507, &UnknownType::Create);
mgr->register_creature_script(22505, &UnknownType::Create);
mgr->register_creature_script(22506, &UnknownType::Create);
mgr->register_creature_script(22508, &UnknownType::Create);
mgr->register_creature_script(14862, &UnknownType::Create);
mgr->register_creature_script(11727, &UnknownType::Create);
mgr->register_creature_script(8179, &UnknownType::Create);
mgr->register_creature_script(8130, &UnknownType::Create);
mgr->register_creature_script(8138, &UnknownType::Create);
mgr->register_creature_script(8156, &UnknownType::Create);
mgr->register_creature_script(6559, &UnknownType::Create);
mgr->register_creature_script(6557, &UnknownType::Create);
mgr->register_creature_script(6553, &UnknownType::Create);
mgr->register_creature_script(6551, &UnknownType::Create);
mgr->register_creature_script(6552, &UnknownType::Create);
mgr->register_creature_script(5455, &UnknownType::Create);
mgr->register_creature_script(5457, &UnknownType::Create);
mgr->register_creature_script(5459, &UnknownType::Create);
mgr->register_creature_script(5458, &UnknownType::Create);
mgr->register_creature_script(5456, &UnknownType::Create);
mgr->register_creature_script(5460, &UnknownType::Create);
mgr->register_creature_script(4374, &UnknownType::Create);
mgr->register_creature_script(14366, &UnknownType::Create);
mgr->register_creature_script(14122, &UnknownType::Create);
mgr->register_creature_script(17883, &UnknownType::Create);
mgr->register_creature_script(19297, &UnknownType::Create);
mgr->register_creature_script(17974, &UnknownType::Create);
mgr->register_creature_script(17988, &UnknownType::Create);
mgr->register_creature_script(1033, &UnknownType::Create);
mgr->register_creature_script(1031, &UnknownType::Create);
mgr->register_creature_script(17696, &UnknownType::Create);
mgr->register_creature_script(12255, &UnknownType::Create);
mgr->register_creature_script(12254, &UnknownType::Create);
mgr->register_creature_script(12249, &UnknownType::Create);
mgr->register_creature_script(12244, &UnknownType::Create);
mgr->register_creature_script(12253, &UnknownType::Create);
mgr->register_creature_script(19328, &UnknownType::Create);
mgr->register_creature_script(19329, &UnknownType::Create);
mgr->register_creature_script(17413, &UnknownType::Create);
mgr->register_creature_script(20251, &UnknownType::Create);
mgr->register_creature_script(19326, &UnknownType::Create);
mgr->register_creature_script(21134, &UnknownType::Create);
mgr->register_creature_script(3616, &UnknownType::Create);
mgr->register_creature_script(19848, &UnknownType::Create);
mgr->register_creature_script(14433, &UnknownType::Create);
mgr->register_creature_script(14505, &UnknownType::Create);
mgr->register_creature_script(14565, &UnknownType::Create);
mgr->register_creature_script(358, &UnknownType::Create);
mgr->register_creature_script(758, &UnknownType::Create);
mgr->register_creature_script(8999, &UnknownType::Create);
mgr->register_creature_script(11111, &UnknownType::Create);
mgr->register_creature_script(18392, &UnknownType::Create);
mgr->register_creature_script(14021, &UnknownType::Create);
mgr->register_creature_script(14329, &UnknownType::Create);
mgr->register_creature_script(14330, &UnknownType::Create);
mgr->register_creature_script(14331, &UnknownType::Create);
mgr->register_creature_script(14332, &UnknownType::Create);
mgr->register_creature_script(14333, &UnknownType::Create);
mgr->register_creature_script(14334, &UnknownType::Create);
mgr->register_creature_script(14335, &UnknownType::Create);
mgr->register_creature_script(14336, &UnknownType::Create);
mgr->register_creature_script(14557, &UnknownType::Create);
mgr->register_creature_script(14662, &UnknownType::Create);
mgr->register_creature_script(14663, &UnknownType::Create);
mgr->register_creature_script(14664, &UnknownType::Create);
mgr->register_creature_script(14666, &UnknownType::Create);
mgr->register_creature_script(14667, &UnknownType::Create);
mgr->register_creature_script(14744, &UnknownType::Create);
mgr->register_creature_script(14745, &UnknownType::Create);
mgr->register_creature_script(15943, &UnknownType::Create);
mgr->register_creature_script(16045, &UnknownType::Create);
mgr->register_creature_script(16531, &UnknownType::Create);
mgr->register_creature_script(19647, &UnknownType::Create);
mgr->register_creature_script(21289, &UnknownType::Create);
mgr->register_creature_script(21090, &UnknownType::Create);
mgr->register_creature_script(20603, &UnknownType::Create);
mgr->register_creature_script(20518, &UnknownType::Create);
mgr->register_creature_script(17473, &UnknownType::Create);
mgr->register_creature_script(17266, &UnknownType::Create);
mgr->register_creature_script(17234, &UnknownType::Create);
mgr->register_creature_script(20114, &UnknownType::Create);
mgr->register_creature_script(20086, &UnknownType::Create);
mgr->register_creature_script(19703, &UnknownType::Create);
mgr->register_creature_script(18545, &UnknownType::Create);
mgr->register_creature_script(18076, &UnknownType::Create);
mgr->register_creature_script(18075, &UnknownType::Create);
mgr->register_creature_script(17532, &UnknownType::Create);
mgr->register_creature_script(17500, &UnknownType::Create);
mgr->register_creature_script(17001, &UnknownType::Create);
mgr->register_creature_script(16914, &UnknownType::Create);
mgr->register_creature_script(22009, &UnknownType::Create);
mgr->register_creature_script(22014, &UnknownType::Create);
mgr->register_creature_script(15090, &UnknownType::Create);
mgr->register_creature_script(22918, &UnknownType::Create);
mgr->register_creature_script(22366, &UnknownType::Create);
mgr->register_creature_script(22335, &UnknownType::Create);
mgr->register_creature_script(22210, &UnknownType::Create);
mgr->register_creature_script(22207, &UnknownType::Create);
mgr->register_creature_script(22104, &UnknownType::Create);
mgr->register_creature_script(21987, &UnknownType::Create);
mgr->register_creature_script(21934, &UnknownType::Create);
mgr->register_creature_script(17499, &UnknownType::Create);
mgr->register_creature_script(21252, &UnknownType::Create);
mgr->register_creature_script(18242, &UnknownType::Create);
mgr->register_creature_script(19577, &UnknownType::Create);
mgr->register_creature_script(19551, &UnknownType::Create);
mgr->register_creature_script(18932, &UnknownType::Create);
mgr->register_creature_script(18928, &UnknownType::Create);
mgr->register_creature_script(18665, &UnknownType::Create);
mgr->register_creature_script(17677, &UnknownType::Create);
mgr->register_creature_script(17516, &UnknownType::Create);
mgr->register_creature_script(17458, &UnknownType::Create);
mgr->register_creature_script(17376, &UnknownType::Create);
mgr->register_creature_script(17302, &UnknownType::Create);
mgr->register_creature_script(17126, &UnknownType::Create);
mgr->register_creature_script(17125, &UnknownType::Create);
mgr->register_creature_script(17124, &UnknownType::Create);
mgr->register_creature_script(16511, &UnknownType::Create);
mgr->register_creature_script(16421, &UnknownType::Create);
mgr->register_creature_script(16356, &UnknownType::Create);
mgr->register_creature_script(16172, &UnknownType::Create);
mgr->register_creature_script(16166, &UnknownType::Create);
mgr->register_creature_script(16136, &UnknownType::Create);
mgr->register_creature_script(16100, &UnknownType::Create);
mgr->register_creature_script(15934, &UnknownType::Create);
mgr->register_creature_script(15925, &UnknownType::Create);
mgr->register_creature_script(15914, &UnknownType::Create);
mgr->register_creature_script(15912, &UnknownType::Create);
mgr->register_creature_script(15911, &UnknownType::Create);
mgr->register_creature_script(15888, &UnknownType::Create);
mgr->register_creature_script(15886, &UnknownType::Create);
mgr->register_creature_script(15885, &UnknownType::Create);
mgr->register_creature_script(15884, &UnknownType::Create);
mgr->register_creature_script(15880, &UnknownType::Create);
mgr->register_creature_script(15878, &UnknownType::Create);
mgr->register_creature_script(15874, &UnknownType::Create);
mgr->register_creature_script(15873, &UnknownType::Create);
mgr->register_creature_script(15817, &UnknownType::Create);
mgr->register_creature_script(15816, &UnknownType::Create);
mgr->register_creature_script(15815, &UnknownType::Create);
mgr->register_creature_script(15814, &UnknownType::Create);
mgr->register_creature_script(15813, &UnknownType::Create);
mgr->register_creature_script(15812, &UnknownType::Create);
mgr->register_creature_script(15811, &UnknownType::Create);
mgr->register_creature_script(15810, &UnknownType::Create);
mgr->register_creature_script(15808, &UnknownType::Create);
mgr->register_creature_script(15807, &UnknownType::Create);
mgr->register_creature_script(15806, &UnknownType::Create);
mgr->register_creature_script(15805, &UnknownType::Create);
mgr->register_creature_script(15804, &UnknownType::Create);
mgr->register_creature_script(15771, &UnknownType::Create);
mgr->register_creature_script(15770, &UnknownType::Create);
mgr->register_creature_script(15769, &UnknownType::Create);
mgr->register_creature_script(15759, &UnknownType::Create);
mgr->register_creature_script(15758, &UnknownType::Create);
mgr->register_creature_script(15757, &UnknownType::Create);
mgr->register_creature_script(15756, &UnknownType::Create);
mgr->register_creature_script(15754, &UnknownType::Create);
mgr->register_creature_script(15753, &UnknownType::Create);
mgr->register_creature_script(15752, &UnknownType::Create);
mgr->register_creature_script(15751, &UnknownType::Create);
mgr->register_creature_script(15750, &UnknownType::Create);
mgr->register_creature_script(15749, &UnknownType::Create);
mgr->register_creature_script(15748, &UnknownType::Create);
mgr->register_creature_script(15747, &UnknownType::Create);
mgr->register_creature_script(15629, &UnknownType::Create);
mgr->register_creature_script(15590, &UnknownType::Create);
mgr->register_creature_script(15553, &UnknownType::Create);
mgr->register_creature_script(15514, &UnknownType::Create);
mgr->register_creature_script(15424, &UnknownType::Create);
mgr->register_creature_script(15422, &UnknownType::Create);
mgr->register_creature_script(15421, &UnknownType::Create);
mgr->register_creature_script(15414, &UnknownType::Create);
mgr->register_creature_script(13148, &UnknownType::Create);
mgr->register_creature_script(23033, &UnknownType::Create);
mgr->register_creature_script(18528, &UnknownType::Create);
mgr->register_creature_script(22974, &UnknownType::Create);
mgr->register_creature_script(22934, &UnknownType::Create);
mgr->register_creature_script(22862, &UnknownType::Create);
mgr->register_creature_script(22849, &UnknownType::Create);
mgr->register_creature_script(22517, &UnknownType::Create);
mgr->register_creature_script(22515, &UnknownType::Create);
mgr->register_creature_script(23496, &UnknownType::Create);
mgr->register_creature_script(23409, &UnknownType::Create);
mgr->register_creature_script(23273, &UnknownType::Create);
mgr->register_creature_script(23270, &UnknownType::Create);
mgr->register_creature_script(23272, &UnknownType::Create);
mgr->register_creature_script(23271, &UnknownType::Create);
mgr->register_creature_script(18768, &UnknownType::Create);
mgr->register_creature_script(18410, &UnknownType::Create);
mgr->register_creature_script(22872, &UnknownType::Create);
mgr->register_creature_script(38, &HumanoidUnarmed::Create);
mgr->register_creature_script(68, &HumanoidWarrior::Create);
mgr->register_creature_script(125, &HumanoidPriest::Create);
mgr->register_creature_script(197, &HumanoidWarrior::Create);
mgr->register_creature_script(203, &HumanoidMage::Create);
mgr->register_creature_script(261, &HumanoidWarrior::Create);
mgr->register_creature_script(263, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(331, &HumanoidMage::Create);
mgr->register_creature_script(392, &HumanoidWarrior::Create);
mgr->register_creature_script(426, &HumanoidUnarmed::Create);
mgr->register_creature_script(429, &HumanoidWarlock::Create);
mgr->register_creature_script(431, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(436, &HumanoidShadowPriest::Create);
mgr->register_creature_script(450, &HumanoidMage::Create);
mgr->register_creature_script(452, &HumanoidRogue::Create);
mgr->register_creature_script(453, &HumanoidPriest::Create);
mgr->register_creature_script(456, &HumanoidPriest::Create);
mgr->register_creature_script(464, &HumanoidWarrior::Create);
mgr->register_creature_script(476, &HumanoidShaman::Create);
mgr->register_creature_script(481, &HumanoidRogue::Create);
mgr->register_creature_script(503, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(515, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(583, &HumanoidRogue::Create);
mgr->register_creature_script(588, &HumanoidWarrior::Create);
mgr->register_creature_script(597, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(636, &HumanoidWarrior::Create);
mgr->register_creature_script(660, &HumanoidWitchDoctor::Create);
mgr->register_creature_script(669, &HumanoidHunter::Create);
mgr->register_creature_script(670, &HumanoidWitchDoctor::Create);
mgr->register_creature_script(671, &HumanoidHunter::Create);
mgr->register_creature_script(672, &HumanoidWitchDoctor::Create);
mgr->register_creature_script(678, &HumanoidUnarmed::Create);
mgr->register_creature_script(679, &HumanoidShaman::Create);
mgr->register_creature_script(680, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(697, &HumanoidShaman::Create);
mgr->register_creature_script(709, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(710, &HumanoidMage::Create);
mgr->register_creature_script(723, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(752, &HumanoidHolyPriest::Create);
mgr->register_creature_script(759, &HumanoidHunter::Create);
mgr->register_creature_script(761, &HumanoidPriest::Create);
mgr->register_creature_script(780, &HumanoidPriest::Create);
mgr->register_creature_script(781, &HumanoidHunter::Create);
mgr->register_creature_script(782, &HumanoidWarrior::Create);
mgr->register_creature_script(783, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(787, &HumanoidPriest::Create);
mgr->register_creature_script(820, &HumanoidWarrior::Create);
mgr->register_creature_script(821, &HumanoidWarrior::Create);
mgr->register_creature_script(859, &HumanoidWarrior::Create);
mgr->register_creature_script(871, &HumanoidWarrior::Create);
mgr->register_creature_script(873, &HumanoidPriest::Create);
mgr->register_creature_script(875, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(878, &HumanoidWarrior::Create);
mgr->register_creature_script(879, &HumanoidHunter::Create);
mgr->register_creature_script(903, &HumanoidWarrior::Create);
mgr->register_creature_script(940, &HumanoidPriest::Create);
mgr->register_creature_script(941, &HumanoidWitchDoctor::Create);
mgr->register_creature_script(942, &HumanoidWitchDoctor::Create);
mgr->register_creature_script(979, &HumanoidShadowHunter::Create);
mgr->register_creature_script(1012, &HumanoidUnarmed::Create);
mgr->register_creature_script(1050, &HumanoidWarrior::Create);
mgr->register_creature_script(1060, &HumanoidPriest::Create);
mgr->register_creature_script(1062, &HumanoidPriest::Create);
mgr->register_creature_script(1065, &HumanoidShaman::Create);
mgr->register_creature_script(1081, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(1092, &HumanoidWarrior::Create);
mgr->register_creature_script(1124, &HumanoidShadowPriest::Create);
mgr->register_creature_script(1142, &HumanoidUnarmed::Create);
mgr->register_creature_script(1144, &HumanoidWitchDoctor::Create);
mgr->register_creature_script(1165, &HumanoidShaman::Create);
mgr->register_creature_script(1174, &HumanoidShaman::Create);
mgr->register_creature_script(1181, &HumanoidShaman::Create);
mgr->register_creature_script(1197, &HumanoidShaman::Create);
mgr->register_creature_script(1399, &HumanoidShaman::Create);
mgr->register_creature_script(1439, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(1449, &HumanoidWitchDoctor::Create);
mgr->register_creature_script(1481, &HumanoidWarrior::Create);
mgr->register_creature_script(1489, &HumanoidHunter::Create);
mgr->register_creature_script(1490, &HumanoidWitchDoctor::Create);
mgr->register_creature_script(1495, &HumanoidWarrior::Create);
mgr->register_creature_script(1496, &HumanoidWarrior::Create);
mgr->register_creature_script(1519, &HumanoidWarrior::Create);
mgr->register_creature_script(1561, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(1562, &HumanoidMage::Create);
mgr->register_creature_script(1564, &HumanoidWarlock::Create);
mgr->register_creature_script(1569, &HumanoidShadowPriest::Create);
mgr->register_creature_script(1652, &HumanoidWarrior::Create);
mgr->register_creature_script(1658, &HumanoidWarrior::Create);
mgr->register_creature_script(1662, &HumanoidWarrior::Create);
mgr->register_creature_script(1718, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(1740, &HumanoidWarrior::Create);
mgr->register_creature_script(1746, &HumanoidWarrior::Create);
mgr->register_creature_script(1784, &HumanoidMage::Create);
mgr->register_creature_script(1788, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(1826, &HumanoidMage::Create);
mgr->register_creature_script(1831, &HumanoidHunter::Create);
mgr->register_creature_script(1832, &HumanoidMage::Create);
mgr->register_creature_script(1835, &HumanoidMage::Create);
mgr->register_creature_script(1842, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(1848, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(1852, &HumanoidWarlock::Create);
mgr->register_creature_script(1867, &HumanoidMage::Create);
mgr->register_creature_script(1871, &HumanoidWarrior::Create);
mgr->register_creature_script(1889, &HumanoidMage::Create);
mgr->register_creature_script(1897, &HumanoidShadowPriest::Create);
mgr->register_creature_script(1908, &HumanoidPriest::Create);
mgr->register_creature_script(1909, &HumanoidRogue::Create);
mgr->register_creature_script(1914, &HumanoidMage::Create);
mgr->register_creature_script(1915, &HumanoidWarlock::Create);
mgr->register_creature_script(1920, &HumanoidMage::Create);
mgr->register_creature_script(1950, &HumanoidRogue::Create);
mgr->register_creature_script(1973, &HumanoidWarrior::Create);
mgr->register_creature_script(1978, &HumanoidRogue::Create);
mgr->register_creature_script(1981, &HumanoidRogue::Create);
mgr->register_creature_script(1997, &HumanoidHunter::Create);
mgr->register_creature_script(2008, &HumanoidWarrior::Create);
mgr->register_creature_script(2009, &HumanoidShaman::Create);
mgr->register_creature_script(2014, &HumanoidShaman::Create);
mgr->register_creature_script(2017, &HumanoidRogue::Create);
mgr->register_creature_script(2058, &HumanoidRogue::Create);
mgr->register_creature_script(2068, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(2090, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(2104, &HumanoidWarrior::Create);
mgr->register_creature_script(2121, &HumanoidShadowPriest::Create);
mgr->register_creature_script(2149, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(2151, &HumanoidPriest::Create);
mgr->register_creature_script(2152, &HumanoidRogue::Create);
mgr->register_creature_script(2158, &HumanoidWarrior::Create);
mgr->register_creature_script(2160, &HumanoidShaman::Create);
mgr->register_creature_script(2169, &HumanoidShaman::Create);
mgr->register_creature_script(2171, &HumanoidShaman::Create);
mgr->register_creature_script(2183, &HumanoidWarrior::Create);
mgr->register_creature_script(2201, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(2208, &HumanoidHunter::Create);
mgr->register_creature_script(2209, &HumanoidWarrior::Create);
mgr->register_creature_script(2210, &HumanoidWarrior::Create);
mgr->register_creature_script(2214, &HumanoidRogue::Create);
mgr->register_creature_script(2240, &HumanoidRogue::Create);
mgr->register_creature_script(2241, &HumanoidRogue::Create);
mgr->register_creature_script(2244, &HumanoidShadowPriest::Create);
mgr->register_creature_script(2246, &HumanoidRogue::Create);
mgr->register_creature_script(2247, &HumanoidWarrior::Create);
mgr->register_creature_script(2253, &HumanoidUnarmed::Create);
mgr->register_creature_script(2255, &HumanoidMage::Create);
mgr->register_creature_script(2260, &HumanoidRogue::Create);
mgr->register_creature_script(2271, &HumanoidWarrior::Create);
mgr->register_creature_script(2272, &HumanoidMage::Create);
mgr->register_creature_script(2304, &HumanoidWarrior::Create);
mgr->register_creature_script(2318, &HumanoidShadowPriest::Create);
mgr->register_creature_script(2319, &HumanoidMage::Create);
mgr->register_creature_script(2333, &HumanoidUnarmed::Create);
mgr->register_creature_script(2346, &HumanoidPriest::Create);
mgr->register_creature_script(2358, &HumanoidWarlock::Create);
mgr->register_creature_script(2365, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(2373, &HumanoidShaman::Create);
mgr->register_creature_script(2376, &HumanoidPriest::Create);
mgr->register_creature_script(2377, &HumanoidHunter::Create);
mgr->register_creature_script(2410, &HumanoidMage::Create);
mgr->register_creature_script(2418, &HumanoidWarrior::Create);
mgr->register_creature_script(2419, &HumanoidWarrior::Create);
mgr->register_creature_script(2423, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(2434, &HumanoidRogue::Create);
mgr->register_creature_script(2439, &HumanoidWarrior::Create);
mgr->register_creature_script(2440, &HumanoidRogue::Create);
mgr->register_creature_script(2465, &HumanoidPriest::Create);
mgr->register_creature_script(2486, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(2487, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(2488, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(2490, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(2491, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(2493, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(2500, &HumanoidWarrior::Create);
mgr->register_creature_script(2541, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(2542, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(2553, &HumanoidShadowPriest::Create);
mgr->register_creature_script(2555, &HumanoidWitchDoctor::Create);
mgr->register_creature_script(2556, &HumanoidHunter::Create);
mgr->register_creature_script(2570, &HumanoidShaman::Create);
mgr->register_creature_script(2577, &HumanoidShadowPriest::Create);
mgr->register_creature_script(2583, &HumanoidHunter::Create);
mgr->register_creature_script(2585, &HumanoidWarrior::Create);
mgr->register_creature_script(2586, &HumanoidRogue::Create);
mgr->register_creature_script(2587, &HumanoidRogue::Create);
mgr->register_creature_script(2590, &HumanoidWarlock::Create);
mgr->register_creature_script(2595, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(2596, &HumanoidMage::Create);
mgr->register_creature_script(2597, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(2598, &HumanoidWarlock::Create);
mgr->register_creature_script(2599, &HumanoidWarrior::Create);
mgr->register_creature_script(2606, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(2609, &HumanoidShaman::Create);
mgr->register_creature_script(2610, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(2640, &HumanoidWitchDoctor::Create);
mgr->register_creature_script(2641, &HumanoidHunter::Create);
mgr->register_creature_script(2642, &HumanoidShadowPriest::Create);
mgr->register_creature_script(2643, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(2645, &HumanoidShadowHunter::Create);
mgr->register_creature_script(2646, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(2647, &HumanoidWarlock::Create);
mgr->register_creature_script(2648, &HumanoidWarrior::Create);
mgr->register_creature_script(2649, &HumanoidRogue::Create);
mgr->register_creature_script(2659, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(2663, &HumanoidRogue::Create);
mgr->register_creature_script(2686, &HumanoidWarrior::Create);
mgr->register_creature_script(2692, &HumanoidWarrior::Create);
mgr->register_creature_script(2700, &HumanoidWarrior::Create);
mgr->register_creature_script(2715, &HumanoidUnarmed::Create);
mgr->register_creature_script(2716, &HumanoidHunter::Create);
mgr->register_creature_script(2717, &HumanoidUnarmed::Create);
mgr->register_creature_script(2718, &HumanoidShaman::Create);
mgr->register_creature_script(2719, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(2720, &HumanoidMage::Create);
mgr->register_creature_script(2726, &HumanoidWarrior::Create);
mgr->register_creature_script(2740, &HumanoidWarlock::Create);
mgr->register_creature_script(2743, &HumanoidWarrior::Create);
mgr->register_creature_script(2752, &HumanoidUnarmed::Create);
mgr->register_creature_script(2766, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(2767, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(2768, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(2769, &HumanoidWarrior::Create);
mgr->register_creature_script(2774, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(2778, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(2783, &HumanoidWarlock::Create);
mgr->register_creature_script(2794, &HumanoidWarrior::Create);
mgr->register_creature_script(2814, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(2818, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(2852, &HumanoidDruid::Create);
mgr->register_creature_script(2853, &HumanoidDruid::Create);
mgr->register_creature_script(2892, &HumanoidPriest::Create);
mgr->register_creature_script(2894, &HumanoidShaman::Create);
mgr->register_creature_script(2906, &HumanoidWarrior::Create);
mgr->register_creature_script(2907, &HumanoidPriest::Create);
mgr->register_creature_script(2919, &HumanoidWarrior::Create);
mgr->register_creature_script(2953, &HumanoidShaman::Create);
mgr->register_creature_script(2982, &HumanoidPriest::Create);
mgr->register_creature_script(2984, &HumanoidPriest::Create);
mgr->register_creature_script(2986, &HumanoidRogue::Create);
mgr->register_creature_script(2988, &HumanoidRogue::Create);
mgr->register_creature_script(3089, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(3195, &HumanoidUnarmed::Create);
mgr->register_creature_script(3198, &HumanoidWarlock::Create);
mgr->register_creature_script(3199, &HumanoidWarlock::Create);
mgr->register_creature_script(3258, &HumanoidHunter::Create);
mgr->register_creature_script(3263, &HumanoidShaman::Create);
mgr->register_creature_script(3265, &HumanoidHunter::Create);
mgr->register_creature_script(3269, &HumanoidShaman::Create);
mgr->register_creature_script(3270, &HumanoidPriest::Create);
mgr->register_creature_script(3271, &HumanoidPriest::Create);
mgr->register_creature_script(3279, &HumanoidRogue::Create);
mgr->register_creature_script(3283, &HumanoidWarrior::Create);
mgr->register_creature_script(3286, &HumanoidPriest::Create);
mgr->register_creature_script(3339, &HumanoidWarrior::Create);
mgr->register_creature_script(3380, &HumanoidWarlock::Create);
mgr->register_creature_script(3387, &HumanoidPriest::Create);
mgr->register_creature_script(3388, &HumanoidPriest::Create);
mgr->register_creature_script(3393, &HumanoidWarrior::Create);
mgr->register_creature_script(3411, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(3457, &HumanoidRogue::Create);
mgr->register_creature_script(3458, &HumanoidPriest::Create);
mgr->register_creature_script(3489, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(3659, &HumanoidWarrior::Create);
mgr->register_creature_script(3705, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(3711, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(3725, &HumanoidWarlock::Create);
mgr->register_creature_script(3727, &HumanoidWarrior::Create);
mgr->register_creature_script(3728, &HumanoidWarlock::Create);
mgr->register_creature_script(3739, &HumanoidWarrior::Create);
mgr->register_creature_script(3742, &HumanoidPriest::Create);
mgr->register_creature_script(3748, &HumanoidShaman::Create);
mgr->register_creature_script(3750, &HumanoidShaman::Create);
mgr->register_creature_script(3752, &HumanoidRogue::Create);
mgr->register_creature_script(3754, &HumanoidRogue::Create);
mgr->register_creature_script(3759, &HumanoidRogue::Create);
mgr->register_creature_script(3763, &HumanoidRogue::Create);
mgr->register_creature_script(3767, &HumanoidRogue::Create);
mgr->register_creature_script(3770, &HumanoidRogue::Create);
mgr->register_creature_script(3833, &HumanoidWarrior::Create);
mgr->register_creature_script(3850, &HumanoidMage::Create);
mgr->register_creature_script(3854, &HumanoidWarrior::Create);
mgr->register_creature_script(3877, &HumanoidWarrior::Create);
mgr->register_creature_script(3879, &HumanoidRogue::Create);
mgr->register_creature_script(3881, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(3882, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(3922, &HumanoidShaman::Create);
mgr->register_creature_script(3924, &HumanoidShaman::Create);
mgr->register_creature_script(3932, &HumanoidWarrior::Create);
mgr->register_creature_script(3933, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(3935, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(3944, &HumanoidPriest::Create);
mgr->register_creature_script(3950, &HumanoidWarrior::Create);
mgr->register_creature_script(3960, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(3965, &HumanoidRogue::Create);
mgr->register_creature_script(3995, &HumanoidWitchDoctor::Create);
mgr->register_creature_script(4003, &HumanoidShaman::Create);
mgr->register_creature_script(4004, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(4023, &HumanoidRogue::Create);
mgr->register_creature_script(4024, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(4025, &HumanoidRogue::Create);
mgr->register_creature_script(4047, &HumanoidPriest::Create);
mgr->register_creature_script(4052, &HumanoidDruid::Create);
mgr->register_creature_script(4083, &HumanoidWitchDoctor::Create);
mgr->register_creature_script(4094, &HumanoidWarrior::Create);
mgr->register_creature_script(4095, &HumanoidUnarmed::Create);
mgr->register_creature_script(4187, &HumanoidWarrior::Create);
mgr->register_creature_script(4281, &HumanoidWarrior::Create);
mgr->register_creature_script(4282, &HumanoidMage::Create);
mgr->register_creature_script(4289, &HumanoidMage::Create);
mgr->register_creature_script(4290, &HumanoidWarrior::Create);
mgr->register_creature_script(4291, &HumanoidHolyPriest::Create);
mgr->register_creature_script(4294, &HumanoidMage::Create);
mgr->register_creature_script(4296, &HumanoidMage::Create);
mgr->register_creature_script(4297, &HumanoidMage::Create);
mgr->register_creature_script(4299, &HumanoidHolyPriest::Create);
mgr->register_creature_script(4300, &HumanoidMage::Create);
mgr->register_creature_script(4360, &HumanoidWarrior::Create);
mgr->register_creature_script(4363, &HumanoidPriest::Create);
mgr->register_creature_script(4364, &HumanoidWarrior::Create);
mgr->register_creature_script(4366, &HumanoidWarrior::Create);
mgr->register_creature_script(4368, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(4370, &HumanoidMage::Create);
mgr->register_creature_script(4420, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(4427, &HumanoidWarrior::Create);
mgr->register_creature_script(4435, &HumanoidWarrior::Create);
mgr->register_creature_script(4436, &HumanoidWarrior::Create);
mgr->register_creature_script(4440, &HumanoidShaman::Create);
mgr->register_creature_script(4444, &HumanoidRogue::Create);
mgr->register_creature_script(4458, &HumanoidHunter::Create);
mgr->register_creature_script(4459, &HumanoidPriest::Create);
mgr->register_creature_script(4460, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(4461, &HumanoidWarrior::Create);
mgr->register_creature_script(4463, &HumanoidWarlock::Create);
mgr->register_creature_script(4465, &HumanoidWarrior::Create);
mgr->register_creature_script(4466, &HumanoidRogue::Create);
mgr->register_creature_script(4467, &HumanoidWitchDoctor::Create);
mgr->register_creature_script(4473, &HumanoidWarlock::Create);
mgr->register_creature_script(4493, &HumanoidWarrior::Create);
mgr->register_creature_script(4500, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(4517, &HumanoidPriest::Create);
mgr->register_creature_script(4520, &HumanoidShaman::Create);
mgr->register_creature_script(4528, &HumanoidUnarmed::Create);
mgr->register_creature_script(4540, &HumanoidHolyPriest::Create);
mgr->register_creature_script(4547, &HumanoidRogue::Create);
mgr->register_creature_script(4633, &HumanoidWarrior::Create);
mgr->register_creature_script(4634, &HumanoidUnarmed::Create);
mgr->register_creature_script(4636, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(4638, &HumanoidWarrior::Create);
mgr->register_creature_script(4645, &HumanoidUnarmed::Create);
mgr->register_creature_script(4647, &HumanoidWarrior::Create);
mgr->register_creature_script(4652, &HumanoidUnarmed::Create);
mgr->register_creature_script(4654, &HumanoidWarrior::Create);
mgr->register_creature_script(4656, &HumanoidUnarmed::Create);
mgr->register_creature_script(4661, &HumanoidUnarmed::Create);
mgr->register_creature_script(4665, &HumanoidWarlock::Create);
mgr->register_creature_script(4667, &HumanoidWarlock::Create);
mgr->register_creature_script(4668, &HumanoidWarlock::Create);
mgr->register_creature_script(4680, &HumanoidWarrior::Create);
mgr->register_creature_script(4705, &HumanoidWarlock::Create);
mgr->register_creature_script(4712, &HumanoidMage::Create);
mgr->register_creature_script(4713, &HumanoidWarrior::Create);
mgr->register_creature_script(4714, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(4716, &HumanoidHunter::Create);
mgr->register_creature_script(4718, &HumanoidPriest::Create);
mgr->register_creature_script(4732, &HumanoidHunter::Create);
mgr->register_creature_script(4784, &HumanoidWarrior::Create);
mgr->register_creature_script(4787, &HumanoidWarrior::Create);
mgr->register_creature_script(4798, &HumanoidRogue::Create);
mgr->register_creature_script(4802, &HumanoidPriest::Create);
mgr->register_creature_script(4813, &HumanoidWarlock::Create);
mgr->register_creature_script(4845, &HumanoidUnarmed::Create);
mgr->register_creature_script(4847, &HumanoidShadowHunter::Create);
mgr->register_creature_script(4848, &HumanoidShadowPriest::Create);
mgr->register_creature_script(4852, &HumanoidPriest::Create);
mgr->register_creature_script(4853, &HumanoidShaman::Create);
mgr->register_creature_script(4856, &HumanoidHunter::Create);
mgr->register_creature_script(4875, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(4879, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(4921, &HumanoidWarrior::Create);
mgr->register_creature_script(4944, &HumanoidWarrior::Create);
mgr->register_creature_script(4951, &HumanoidWarrior::Create);
mgr->register_creature_script(4954, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(4969, &HumanoidUnarmed::Create);
mgr->register_creature_script(4996, &HumanoidWarrior::Create);
mgr->register_creature_script(5085, &HumanoidWarrior::Create);
mgr->register_creature_script(5086, &HumanoidWarrior::Create);
mgr->register_creature_script(5095, &HumanoidWarrior::Create);
mgr->register_creature_script(5096, &HumanoidWarrior::Create);
mgr->register_creature_script(5232, &HumanoidUnarmed::Create);
mgr->register_creature_script(5234, &HumanoidUnarmed::Create);
mgr->register_creature_script(5236, &HumanoidShaman::Create);
mgr->register_creature_script(5237, &HumanoidMage::Create);
mgr->register_creature_script(5239, &HumanoidMage::Create);
mgr->register_creature_script(5240, &HumanoidWarlock::Create);
mgr->register_creature_script(5241, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(5253, &HumanoidUnarmed::Create);
mgr->register_creature_script(5254, &HumanoidPriest::Create);
mgr->register_creature_script(5256, &HumanoidWarrior::Create);
mgr->register_creature_script(5328, &HumanoidPriest::Create);
mgr->register_creature_script(5331, &HumanoidWarrior::Create);
mgr->register_creature_script(5333, &HumanoidWarrior::Create);
mgr->register_creature_script(5334, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(5336, &HumanoidMage::Create);
mgr->register_creature_script(5346, &HumanoidRogue::Create);
mgr->register_creature_script(5363, &HumanoidRogue::Create);
mgr->register_creature_script(5364, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(5386, &HumanoidWarlock::Create);
mgr->register_creature_script(5396, &HumanoidWarrior::Create);
mgr->register_creature_script(5418, &HumanoidRogue::Create);
mgr->register_creature_script(5470, &HumanoidUnarmed::Create);
mgr->register_creature_script(5472, &HumanoidWarrior::Create);
mgr->register_creature_script(5473, &HumanoidMage::Create);
mgr->register_creature_script(5474, &HumanoidUnarmed::Create);
mgr->register_creature_script(5475, &HumanoidWarlock::Create);
mgr->register_creature_script(5616, &HumanoidRogue::Create);
mgr->register_creature_script(5617, &HumanoidShadowPriest::Create);
mgr->register_creature_script(5618, &HumanoidRogue::Create);
mgr->register_creature_script(5623, &HumanoidRogue::Create);
mgr->register_creature_script(5642, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(5648, &HumanoidShadowPriest::Create);
mgr->register_creature_script(5649, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(5650, &HumanoidWitchDoctor::Create);
mgr->register_creature_script(5694, &HumanoidMage::Create);
mgr->register_creature_script(5760, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(5764, &HumanoidWarrior::Create);
mgr->register_creature_script(5769, &HumanoidDruid::Create);
mgr->register_creature_script(5787, &HumanoidWarrior::Create);
mgr->register_creature_script(5824, &HumanoidWarrior::Create);
mgr->register_creature_script(5851, &HumanoidWarrior::Create);
mgr->register_creature_script(5860, &HumanoidShaman::Create);
mgr->register_creature_script(5862, &HumanoidShaman::Create);
mgr->register_creature_script(5863, &HumanoidShaman::Create);
mgr->register_creature_script(5870, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(5888, &HumanoidPriest::Create);
mgr->register_creature_script(5901, &HumanoidPriest::Create);
mgr->register_creature_script(5975, &HumanoidMage::Create);
mgr->register_creature_script(5976, &HumanoidUnarmed::Create);
mgr->register_creature_script(5977, &HumanoidUnarmed::Create);
mgr->register_creature_script(5978, &HumanoidWarlock::Create);
mgr->register_creature_script(5994, &HumanoidPriest::Create);
mgr->register_creature_script(6004, &HumanoidWarlock::Create);
mgr->register_creature_script(6005, &HumanoidUnarmed::Create);
mgr->register_creature_script(6006, &HumanoidWarlock::Create);
mgr->register_creature_script(6007, &HumanoidWarrior::Create);
mgr->register_creature_script(6008, &HumanoidWarlock::Create);
mgr->register_creature_script(6009, &HumanoidWarlock::Create);
mgr->register_creature_script(6035, &HumanoidRogue::Create);
mgr->register_creature_script(6068, &HumanoidWarrior::Create);
mgr->register_creature_script(6069, &HumanoidWarrior::Create);
mgr->register_creature_script(6089, &HumanoidWarrior::Create);
mgr->register_creature_script(6124, &HumanoidWarrior::Create);
mgr->register_creature_script(6126, &HumanoidRogue::Create);
mgr->register_creature_script(6129, &HumanoidMage::Create);
mgr->register_creature_script(6131, &HumanoidMage::Create);
mgr->register_creature_script(6134, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(6138, &HumanoidPriest::Create);
mgr->register_creature_script(6180, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(6185, &HumanoidWarrior::Create);
mgr->register_creature_script(6186, &HumanoidShaman::Create);
mgr->register_creature_script(6188, &HumanoidShaman::Create);
mgr->register_creature_script(6190, &HumanoidWarrior::Create);
mgr->register_creature_script(6194, &HumanoidWarrior::Create);
mgr->register_creature_script(6196, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(6201, &HumanoidRogue::Create);
mgr->register_creature_script(6207, &HumanoidRogue::Create);
mgr->register_creature_script(6244, &HumanoidPriest::Create);
mgr->register_creature_script(6252, &HumanoidWarlock::Create);
mgr->register_creature_script(6253, &HumanoidWarlock::Create);
mgr->register_creature_script(6254, &HumanoidWarlock::Create);
mgr->register_creature_script(6267, &HumanoidWarlock::Create);
mgr->register_creature_script(6351, &HumanoidPriest::Create);
mgr->register_creature_script(6371, &HumanoidWarrior::Create);
mgr->register_creature_script(6389, &HumanoidWarrior::Create);
mgr->register_creature_script(6519, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(6548, &HumanoidMage::Create);
mgr->register_creature_script(6560, &HumanoidWarrior::Create);
mgr->register_creature_script(6606, &HumanoidPriest::Create);
mgr->register_creature_script(6668, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(6707, &HumanoidRogue::Create);
mgr->register_creature_script(6766, &HumanoidRogue::Create);
mgr->register_creature_script(6768, &HumanoidRogue::Create);
mgr->register_creature_script(6771, &HumanoidRogue::Create);
mgr->register_creature_script(7017, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(7026, &HumanoidMage::Create);
mgr->register_creature_script(7028, &HumanoidWarlock::Create);
mgr->register_creature_script(7034, &HumanoidMage::Create);
mgr->register_creature_script(7035, &HumanoidUnarmed::Create);
mgr->register_creature_script(7069, &HumanoidHolyPriest::Create);
mgr->register_creature_script(7075, &HumanoidMage::Create);
mgr->register_creature_script(7091, &HumanoidRogue::Create);
mgr->register_creature_script(7106, &HumanoidRogue::Create);
mgr->register_creature_script(7107, &HumanoidRogue::Create);
mgr->register_creature_script(7108, &HumanoidRogue::Create);
mgr->register_creature_script(7110, &HumanoidRogue::Create);
mgr->register_creature_script(7112, &HumanoidWarlock::Create);
mgr->register_creature_script(7114, &HumanoidWarrior::Create);
mgr->register_creature_script(7115, &HumanoidWarlock::Create);
mgr->register_creature_script(7118, &HumanoidWarlock::Create);
mgr->register_creature_script(7120, &HumanoidWarlock::Create);
mgr->register_creature_script(7135, &HumanoidWarrior::Create);
mgr->register_creature_script(7153, &HumanoidWarrior::Create);
mgr->register_creature_script(7157, &HumanoidWarrior::Create);
mgr->register_creature_script(7158, &HumanoidShaman::Create);
mgr->register_creature_script(7175, &HumanoidRogue::Create);
mgr->register_creature_script(7246, &HumanoidShadowHunter::Create);
mgr->register_creature_script(7247, &HumanoidWarlock::Create);
mgr->register_creature_script(7313, &HumanoidPriest::Create);
mgr->register_creature_script(7316, &HumanoidPriest::Create);
mgr->register_creature_script(7320, &HumanoidUnarmed::Create);
mgr->register_creature_script(7327, &HumanoidWarrior::Create);
mgr->register_creature_script(7329, &HumanoidWarrior::Create);
mgr->register_creature_script(7335, &HumanoidShaman::Create);
mgr->register_creature_script(7337, &HumanoidShadowPriest::Create);
mgr->register_creature_script(7340, &HumanoidShadowPriest::Create);
mgr->register_creature_script(7342, &HumanoidWarlock::Create);
mgr->register_creature_script(7344, &HumanoidWarrior::Create);
mgr->register_creature_script(7345, &HumanoidWarrior::Create);
mgr->register_creature_script(7369, &HumanoidUnarmed::Create);
mgr->register_creature_script(7371, &HumanoidUnarmed::Create);
mgr->register_creature_script(7372, &HumanoidWarlock::Create);
mgr->register_creature_script(7379, &HumanoidMage::Create);
mgr->register_creature_script(7404, &HumanoidWarrior::Create);
mgr->register_creature_script(7437, &HumanoidMage::Create);
mgr->register_creature_script(7439, &HumanoidShaman::Create);
mgr->register_creature_script(7441, &HumanoidShaman::Create);
mgr->register_creature_script(7457, &HumanoidRogue::Create);
mgr->register_creature_script(7463, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(7725, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(7727, &HumanoidShaman::Create);
mgr->register_creature_script(7779, &HumanoidPriest::Create);
mgr->register_creature_script(7809, &HumanoidRogue::Create);
mgr->register_creature_script(7855, &HumanoidRogue::Create);
mgr->register_creature_script(7858, &HumanoidRogue::Create);
mgr->register_creature_script(7873, &HumanoidWarrior::Create);
mgr->register_creature_script(7899, &HumanoidRogue::Create);
mgr->register_creature_script(7901, &HumanoidRogue::Create);
mgr->register_creature_script(7995, &HumanoidPriest::Create);
mgr->register_creature_script(8017, &HumanoidWarrior::Create);
mgr->register_creature_script(8115, &HumanoidWitchDoctor::Create);
mgr->register_creature_script(8125, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(8136, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(8216, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(8218, &HumanoidRogue::Create);
mgr->register_creature_script(8282, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(8298, &HumanoidPriest::Create);
mgr->register_creature_script(8309, &HumanoidRogue::Create);
mgr->register_creature_script(8380, &HumanoidWarrior::Create);
mgr->register_creature_script(8386, &HumanoidWarrior::Create);
mgr->register_creature_script(8387, &HumanoidWarrior::Create);
mgr->register_creature_script(8388, &HumanoidWarrior::Create);
mgr->register_creature_script(8389, &HumanoidWarrior::Create);
mgr->register_creature_script(8408, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(8441, &HumanoidWarrior::Create);
mgr->register_creature_script(8524, &HumanoidMage::Create);
mgr->register_creature_script(8527, &HumanoidWarrior::Create);
mgr->register_creature_script(8546, &HumanoidWarlock::Create);
mgr->register_creature_script(8547, &HumanoidWarlock::Create);
mgr->register_creature_script(8550, &HumanoidWarlock::Create);
mgr->register_creature_script(8551, &HumanoidWarlock::Create);
mgr->register_creature_script(8553, &HumanoidShadowPriest::Create);
mgr->register_creature_script(8558, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(8560, &HumanoidWarrior::Create);
mgr->register_creature_script(8561, &HumanoidShadowHunter::Create);
mgr->register_creature_script(8578, &HumanoidMage::Create);
mgr->register_creature_script(8756, &HumanoidWarrior::Create);
mgr->register_creature_script(8757, &HumanoidWarrior::Create);
mgr->register_creature_script(8758, &HumanoidWarrior::Create);
mgr->register_creature_script(8876, &HumanoidWarlock::Create);
mgr->register_creature_script(8903, &HumanoidWarrior::Create);
mgr->register_creature_script(8980, &HumanoidWarrior::Create);
mgr->register_creature_script(9045, &HumanoidWarlock::Create);
mgr->register_creature_script(9077, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(9078, &HumanoidWarlock::Create);
mgr->register_creature_script(9095, &HumanoidWarrior::Create);
mgr->register_creature_script(9196, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(9197, &HumanoidMage::Create);
mgr->register_creature_script(9198, &HumanoidPriest::Create);
mgr->register_creature_script(9199, &HumanoidWarrior::Create);
mgr->register_creature_script(9201, &HumanoidMage::Create);
mgr->register_creature_script(9216, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(9217, &HumanoidMage::Create);
mgr->register_creature_script(9218, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(9219, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(9239, &HumanoidPriest::Create);
mgr->register_creature_script(9240, &HumanoidShadowPriest::Create);
mgr->register_creature_script(9241, &HumanoidHunter::Create);
mgr->register_creature_script(9257, &HumanoidWarlock::Create);
mgr->register_creature_script(9258, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(9261, &HumanoidWarlock::Create);
mgr->register_creature_script(9262, &HumanoidWarlock::Create);
mgr->register_creature_script(9263, &HumanoidWarlock::Create);
mgr->register_creature_script(9265, &HumanoidShadowHunter::Create);
mgr->register_creature_script(9266, &HumanoidWitchDoctor::Create);
mgr->register_creature_script(9268, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(9269, &HumanoidPriest::Create);
mgr->register_creature_script(9445, &HumanoidWarrior::Create);
mgr->register_creature_script(9456, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(9464, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(9516, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(9517, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(9522, &HumanoidRogue::Create);
mgr->register_creature_script(9523, &HumanoidPriest::Create);
mgr->register_creature_script(9605, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(9692, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(9693, &HumanoidMage::Create);
mgr->register_creature_script(9716, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(9717, &HumanoidWarlock::Create);
mgr->register_creature_script(9817, &HumanoidWarlock::Create);
mgr->register_creature_script(9818, &HumanoidWarlock::Create);
mgr->register_creature_script(10040, &HumanoidWarrior::Create);
mgr->register_creature_script(10076, &HumanoidPriest::Create);
mgr->register_creature_script(10157, &HumanoidPriest::Create);
mgr->register_creature_script(10307, &HumanoidWitchDoctor::Create);
mgr->register_creature_script(10318, &HumanoidRogue::Create);
mgr->register_creature_script(10319, &HumanoidWarrior::Create);
mgr->register_creature_script(10371, &HumanoidWarrior::Create);
mgr->register_creature_script(10390, &HumanoidWarrior::Create);
mgr->register_creature_script(10391, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(10398, &HumanoidShadowPriest::Create);
mgr->register_creature_script(10399, &HumanoidWarlock::Create);
mgr->register_creature_script(10419, &HumanoidMage::Create);
mgr->register_creature_script(10422, &HumanoidMage::Create);
mgr->register_creature_script(10423, &HumanoidPriest::Create);
mgr->register_creature_script(10425, &HumanoidMage::Create);
mgr->register_creature_script(10469, &HumanoidWarlock::Create);
mgr->register_creature_script(10471, &HumanoidWarlock::Create);
mgr->register_creature_script(10472, &HumanoidWarlock::Create);
mgr->register_creature_script(10477, &HumanoidShadowPriest::Create);
mgr->register_creature_script(10486, &HumanoidWarrior::Create);
mgr->register_creature_script(10489, &HumanoidWarrior::Create);
mgr->register_creature_script(10601, &HumanoidWarrior::Create);
mgr->register_creature_script(10602, &HumanoidMage::Create);
mgr->register_creature_script(10608, &HumanoidPriest::Create);
mgr->register_creature_script(10619, &HumanoidWarrior::Create);
mgr->register_creature_script(10680, &HumanoidWarlock::Create);
mgr->register_creature_script(10720, &HumanoidRogue::Create);
mgr->register_creature_script(10721, &HumanoidWarrior::Create);
mgr->register_creature_script(10758, &HumanoidRogue::Create);
mgr->register_creature_script(10760, &HumanoidShaman::Create);
mgr->register_creature_script(10762, &HumanoidUnarmed::Create);
mgr->register_creature_script(10781, &HumanoidPriest::Create);
mgr->register_creature_script(10824, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(10826, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(10878, &HumanoidRogue::Create);
mgr->register_creature_script(10937, &HumanoidWarrior::Create);
mgr->register_creature_script(10943, &HumanoidWarrior::Create);
mgr->register_creature_script(10947, &HumanoidRogue::Create);
mgr->register_creature_script(10984, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(11034, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(11043, &HumanoidHolyPriest::Create);
mgr->register_creature_script(11053, &HumanoidPriest::Create);
mgr->register_creature_script(11055, &HumanoidShadowPriest::Create);
mgr->register_creature_script(11075, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(11076, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(11077, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(11078, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(11099, &HumanoidWarrior::Create);
mgr->register_creature_script(11121, &HumanoidWarrior::Create);
mgr->register_creature_script(11218, &HumanoidDruid::Create);
mgr->register_creature_script(11279, &HumanoidWarrior::Create);
mgr->register_creature_script(11288, &HumanoidRogue::Create);
mgr->register_creature_script(11319, &HumanoidShaman::Create);
mgr->register_creature_script(11322, &HumanoidWarlock::Create);
mgr->register_creature_script(11323, &HumanoidWarrior::Create);
mgr->register_creature_script(11324, &HumanoidWarlock::Create);
mgr->register_creature_script(11338, &HumanoidShadowPriest::Create);
mgr->register_creature_script(11339, &HumanoidShadowHunter::Create);
mgr->register_creature_script(11340, &HumanoidPriest::Create);
mgr->register_creature_script(11346, &HumanoidPriest::Create);
mgr->register_creature_script(11351, &HumanoidHunter::Create);
mgr->register_creature_script(11352, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(11353, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(11355, &HumanoidWarrior::Create);
mgr->register_creature_script(11383, &HumanoidPriest::Create);
mgr->register_creature_script(11440, &HumanoidWarrior::Create);
mgr->register_creature_script(11441, &HumanoidUnarmed::Create);
mgr->register_creature_script(11442, &HumanoidUnarmed::Create);
mgr->register_creature_script(11444, &HumanoidMage::Create);
mgr->register_creature_script(11445, &HumanoidWarrior::Create);
mgr->register_creature_script(11448, &HumanoidWarlock::Create);
mgr->register_creature_script(11452, &HumanoidRogue::Create);
mgr->register_creature_script(11453, &HumanoidRogue::Create);
mgr->register_creature_script(11454, &HumanoidRogue::Create);
mgr->register_creature_script(11456, &HumanoidRogue::Create);
mgr->register_creature_script(11466, &HumanoidWarlock::Create);
mgr->register_creature_script(11470, &HumanoidMage::Create);
mgr->register_creature_script(11518, &HumanoidWarlock::Create);
mgr->register_creature_script(11552, &HumanoidPriest::Create);
mgr->register_creature_script(11559, &HumanoidShadowPriest::Create);
mgr->register_creature_script(11582, &HumanoidWarlock::Create);
mgr->register_creature_script(11598, &HumanoidWarrior::Create);
mgr->register_creature_script(11600, &HumanoidShaman::Create);
mgr->register_creature_script(11604, &HumanoidShaman::Create);
mgr->register_creature_script(11605, &HumanoidPriest::Create);
mgr->register_creature_script(11662, &HumanoidPriest::Create);
mgr->register_creature_script(11678, &HumanoidRogue::Create);
mgr->register_creature_script(11679, &HumanoidWitchDoctor::Create);
mgr->register_creature_script(11683, &HumanoidShaman::Create);
mgr->register_creature_script(11685, &HumanoidPriest::Create);
mgr->register_creature_script(11686, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(11723, &HumanoidRogue::Create);
mgr->register_creature_script(11730, &HumanoidRogue::Create);
mgr->register_creature_script(11734, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(11746, &HumanoidUnarmed::Create);
mgr->register_creature_script(11777, &HumanoidUnarmed::Create);
mgr->register_creature_script(11778, &HumanoidUnarmed::Create);
mgr->register_creature_script(11791, &HumanoidRogue::Create);
mgr->register_creature_script(11792, &HumanoidRogue::Create);
mgr->register_creature_script(11831, &HumanoidWitchDoctor::Create);
mgr->register_creature_script(11837, &HumanoidShaman::Create);
mgr->register_creature_script(11838, &HumanoidPriest::Create);
mgr->register_creature_script(11839, &HumanoidUnarmed::Create);
mgr->register_creature_script(11880, &HumanoidWarrior::Create);
mgr->register_creature_script(11898, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(11910, &HumanoidUnarmed::Create);
mgr->register_creature_script(11912, &HumanoidUnarmed::Create);
mgr->register_creature_script(11913, &HumanoidMage::Create);
mgr->register_creature_script(11917, &HumanoidShaman::Create);
mgr->register_creature_script(11937, &HumanoidWarrior::Create);
mgr->register_creature_script(11944, &HumanoidPriest::Create);
mgr->register_creature_script(11947, &HumanoidWarrior::Create);
mgr->register_creature_script(11949, &HumanoidWarrior::Create);
mgr->register_creature_script(12052, &HumanoidWarrior::Create);
mgr->register_creature_script(12053, &HumanoidWarrior::Create);
mgr->register_creature_script(12116, &HumanoidPriest::Create);
mgr->register_creature_script(12126, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(12127, &HumanoidWarrior::Create);
mgr->register_creature_script(12140, &HumanoidWarrior::Create);
mgr->register_creature_script(12157, &HumanoidShadowHunter::Create);
mgr->register_creature_script(12158, &HumanoidHunter::Create);
mgr->register_creature_script(13284, &HumanoidShaman::Create);
mgr->register_creature_script(13301, &HumanoidRogue::Create);
mgr->register_creature_script(13324, &HumanoidWarrior::Create);
mgr->register_creature_script(13328, &HumanoidWarrior::Create);
mgr->register_creature_script(13330, &HumanoidWarrior::Create);
mgr->register_creature_script(13332, &HumanoidWarrior::Create);
mgr->register_creature_script(13333, &HumanoidWarrior::Create);
mgr->register_creature_script(13337, &HumanoidWarrior::Create);
mgr->register_creature_script(13419, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(13421, &HumanoidWarrior::Create);
mgr->register_creature_script(13442, &HumanoidDruid::Create);
mgr->register_creature_script(13443, &HumanoidDruid::Create);
mgr->register_creature_script(13836, &HumanoidWarlock::Create);
mgr->register_creature_script(13843, &HumanoidWarrior::Create);
mgr->register_creature_script(13956, &HumanoidPriest::Create);
mgr->register_creature_script(13957, &HumanoidWarrior::Create);
mgr->register_creature_script(13958, &HumanoidPriest::Create);
mgr->register_creature_script(14182, &HumanoidHunter::Create);
mgr->register_creature_script(14185, &HumanoidHunter::Create);
mgr->register_creature_script(14186, &HumanoidHunter::Create);
mgr->register_creature_script(14187, &HumanoidHunter::Create);
mgr->register_creature_script(14188, &HumanoidHunter::Create);
mgr->register_creature_script(14236, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(14284, &HumanoidWarrior::Create);
mgr->register_creature_script(14285, &HumanoidWarrior::Create);
mgr->register_creature_script(14347, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(14369, &HumanoidWarrior::Create);
mgr->register_creature_script(14372, &HumanoidRogue::Create);
mgr->register_creature_script(14392, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(14393, &HumanoidPriest::Create);
mgr->register_creature_script(14445, &HumanoidWarrior::Create);
mgr->register_creature_script(14456, &HumanoidWarrior::Create);
mgr->register_creature_script(14479, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(14483, &HumanoidWarrior::Create);
mgr->register_creature_script(14530, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(14621, &HumanoidPriest::Create);
mgr->register_creature_script(14625, &HumanoidPriest::Create);
mgr->register_creature_script(14634, &HumanoidWarrior::Create);
mgr->register_creature_script(14720, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(14739, &HumanoidPriest::Create);
mgr->register_creature_script(14876, &HumanoidWitchDoctor::Create);
mgr->register_creature_script(14911, &HumanoidWarrior::Create);
mgr->register_creature_script(14912, &HumanoidWarrior::Create);
mgr->register_creature_script(14962, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(15022, &HumanoidRogue::Create);
mgr->register_creature_script(15182, &HumanoidWarrior::Create);
mgr->register_creature_script(15184, &HumanoidWarrior::Create);
mgr->register_creature_script(15195, &HumanoidWarrior::Create);
mgr->register_creature_script(15202, &HumanoidWarlock::Create);
mgr->register_creature_script(15213, &HumanoidFuryWarrior::Create);
mgr->register_creature_script(15215, &HumanoidPriest::Create); 
mgr->register_creature_script(15440, &HumanoidWarrior::Create); 
mgr->register_creature_script(15528, &HumanoidPriest::Create); 
mgr->register_creature_script(15532, &HumanoidWarrior::Create); 
mgr->register_creature_script(15533, &HumanoidWarrior::Create); 
*/
}
