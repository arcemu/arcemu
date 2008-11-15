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
/* Instance_ScarletMonastery.cpp Script									*/
/************************************************************************/

//Houndmaster Loksey

#define LOKSEY 3974
#define ENRAGE 8269 
#define AGGRO 5841 

class LokseyAI : public CreatureAIScript
{
public:
ADD_CREATURE_FACTORY_FUNCTION(LokseyAI); 

SP_AI_Spell spells[1]; 

bool m_spellcheck[1]; 

LokseyAI(Creature *pCreature) : CreatureAIScript(pCreature)
{
nrspells = 1; 
mPhase = 0;

for(int i=0;i<nrspells;i++)
        {
            m_spellcheck[i] = false;
        }

spells[0].info = dbcSpell.LookupEntry(ENRAGE);
spells[0].targettype = TARGET_SELF;
spells[0].instant = true;
spells[0].perctrigger = 0.0f;
spells[0].attackstoptimer = 1000;
}

void OnCombatStart(Unit* mTarget)
{
_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Release the hounds!");

_unit->PlaySoundToSet(5841);

RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
}

void OnTargetDied(Unit* mTarget)
{

if (_unit->GetHealthPct() > 0)    
        {
int RandomSpeach;
(float)RandomFloat(1000);
RandomSpeach=rand()%2;
switch (RandomSpeach)
{
case 0:

_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Ha! Had enough?");

_unit->PlaySoundToSet(0000);
break;

		}
	}
}

	void OnDamageTaken(Unit* mAttacker, float fAmount)
    {
        if(fAmount < 5) return;
        // <25% hp -> We go to phase 1
        if(_unit->GetHealthPct() <= 25 && mPhase == 0) {
            ChangeToPhase1();
        }
    }
	 
	void ChangeToPhase1()
    {
        // Set phase var
        mPhase = 1;

        // Play sound, and send text.
        _unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Light, give me strength!");
		HpEnrage();
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

	void HpEnrage()
	{
	    _unit->CastSpell(_unit, spells[0].info, spells[0].instant);
	}

void AIUpdate()
    {
Timer = Timer + 1;

if (Timer == 200) 
{
    _unit->CastSpell(_unit, spells[0].info, spells[0].instant); 
}

else
{
float val = (float)RandomFloat(100.0f);
SpellCast(val);
}
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
target = _unit->GetAIInterface()->GetNextTarget();
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
_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant);
break;
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
    uint32 mPhase;  // NPC has 2 phases
    int nrspells;
    int Timer;
}; 

// Arcanist Doan

#define DOAN 6487    
#define SHIELD 642 
#define NOVA 9435 
#define POLY 13323
#define SILENCE 30225
#define AGGRO2 5842
#define AE 5843

class DoanAI : public CreatureAIScript
{
public:
ADD_CREATURE_FACTORY_FUNCTION(DoanAI); 

SP_AI_Spell spells[4]; 

bool m_spellcheck[4]; 

DoanAI(Creature *pCreature) : CreatureAIScript(pCreature)
{
nrspells = 4; 
mPhase = 0;

for(int i=0;i<nrspells;i++)
        {
            m_spellcheck[i] = false;
        }

spells[0].info = dbcSpell.LookupEntry(SHIELD);
spells[0].targettype = TARGET_SELF;
spells[0].instant = true;
spells[0].perctrigger = 0.0f;
spells[0].attackstoptimer = 1000;

spells[1].info = dbcSpell.LookupEntry(NOVA);
spells[1].targettype = TARGET_ATTACKING;
spells[1].instant = false;
spells[1].perctrigger = 0.0f;
spells[1].attackstoptimer = 1000;
spells[1].soundid = AE;
spells[1].speech = "Burn in righteous fire!";

spells[2].info = dbcSpell.LookupEntry(POLY);
spells[2].targettype = TARGET_ATTACKING;
spells[2].instant = false;
spells[2].perctrigger = 10.0f;
spells[2].attackstoptimer = 1000;

spells[3].info = dbcSpell.LookupEntry(SILENCE);
spells[3].targettype = TARGET_ATTACKING;
spells[3].instant = false;
spells[3].perctrigger = 15.0f;
spells[3].attackstoptimer = 1000;

}

void OnCombatStart(Unit* mTarget)
{
_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You will not defile these mysteries!");

_unit->PlaySoundToSet(5842);

RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
}

void OnTargetDied(Unit* mTarget)
{

if (_unit->GetHealthPct() > 0)    
        {
int RandomSpeach;
(float)RandomFloat(1000);
RandomSpeach=rand()%2;
switch (RandomSpeach)
{
case 0:

_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Ha! Had enough?");

_unit->PlaySoundToSet(0000);
break;

		}
	}
}

	void OnDamageTaken(Unit* mAttacker, float fAmount)
    {
        if(fAmount < 5) return;
        // <50% hp -> We go to phase 1
        if(_unit->GetHealthPct() <= 50 && mPhase == 0) {
            ChangeToPhase1();
        }
    }
	 
	void ChangeToPhase1()
    {
        // Set phase var
        mPhase = 1;

        // Play sound, and send text.
		/*_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Light, give me strength!");
		       ^^ Notes for myself */ 
		Bubble(); 
		Boom();
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

	void Bubble()
	{
	    _unit->CastSpell(_unit, spells[0].info, spells[0].instant);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Burn in righteous fire!");
	}
	void Boom()
	{
		_unit->CastSpell(_unit, spells[1].info, spells[1].instant);
	}

void AIUpdate()
    {
Timer = Timer + 1;

if (Timer == 30000) 
{
    _unit->CastSpell(_unit, spells[2].info, spells[2].instant); 
}

else
{
float val = (float)RandomFloat(100.0f);
SpellCast(val);
}
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
target = _unit->GetAIInterface()->GetNextTarget();
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
_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant);
break;
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
    uint32 mPhase;  // NPC has 2 phases
    int nrspells;
    int Timer;
}; 

// Interrogator Vishas

#define VISHAS 3983    
#define AGGRO3 5847 
#define SLAY 5848 
#define SPEECH1 5849
#define SPEECH2 5850

class VishasAI : public CreatureAIScript
{
public:
ADD_CREATURE_FACTORY_FUNCTION(VishasAI); 

SP_AI_Spell spells[1]; 

bool m_spellcheck[1]; 

VishasAI(Creature *pCreature) : CreatureAIScript(pCreature)
{
nrspells = 1; 
mPhase = 0;

for(int i=0;i<nrspells;i++)
        {
            m_spellcheck[i] = false;
        }

spells[0].info = dbcSpell.LookupEntry(SHIELD);
spells[0].targettype = TARGET_SELF;
spells[0].instant = true;
spells[0].perctrigger = 0.0f;
spells[0].attackstoptimer = 1000;
}

void OnCombatStart(Unit* mTarget)
{
_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Tell me...tell me everything.");

_unit->PlaySoundToSet(5847);

RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
}

void OnTargetDied(Unit* mTarget)
{

if (_unit->GetHealthPct() > 0)    
        {
int RandomSpeach;
(float)RandomFloat(1000);
RandomSpeach=rand()%2;
switch (RandomSpeach)
{
case 0:

_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Purged by pain!");

_unit->PlaySoundToSet(5848);
break;

		}
	}
}

	void OnDamageTaken(Unit* mAttacker, float fAmount)
    {
        if(fAmount < 5) return;
        // <50% hp -> We go to phase 1
        if(_unit->GetHealthPct() <= 70 && mPhase == 0) {
            ChangeToPhase1();
		if(_unit->GetHealthPct() <= 40 && mPhase == 1)
			ChangeToPhase2();
        }
    }
	 
	void ChangeToPhase1()
    {
        // Set phase var
        mPhase = 1;

        // Play sound, and send text.
		/*_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Light, give me strength!");
		       ^^ Notes for myself */ 
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Naughty Secrets!");
    }

	void ChangeToPhase2()
    {
        // Set phase var
        mPhase = 2;

        // Play sound, and send text.
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I'll rip the secrets from your flesh!");
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
Timer = Timer + 1;


/*if (Timer == 30000000000) 
{
    _unit->CastSpell(_unit, spells[1].info, spells[1].instant); 
}

else
{*/
float val = (float)RandomFloat(100.0f);
SpellCast(val);
//}
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
target = _unit->GetAIInterface()->GetNextTarget();
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
_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant);
break;
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
    uint32 mPhase;  // NPC has 2 phases
    int nrspells;
    int Timer;
};

// Scarlet Commander Mograine

#define COMMANDER_MOGRAINE 3976
#define HAMMER 32416 
#define CRUSADER 14517
#define RESTALK 5835

class MograineAI : public CreatureAIScript
{
public:
ADD_CREATURE_FACTORY_FUNCTION(MograineAI); 

SP_AI_Spell spells[3]; 

bool m_spellcheck[3]; 

MograineAI(Creature *pCreature) : CreatureAIScript(pCreature)
{
nrspells = 3; 
mPhase = 0;

for(int i=0;i<nrspells;i++)
        {
            m_spellcheck[i] = false;
        }

spells[0].info = dbcSpell.LookupEntry(SHIELD);
spells[0].targettype = TARGET_SELF;
spells[0].instant = true;
spells[0].perctrigger = 5.0f;
spells[0].attackstoptimer = 1000;

spells[1].info = dbcSpell.LookupEntry(HAMMER);
spells[1].targettype = TARGET_ATTACKING;
spells[1].instant = true;
spells[1].perctrigger = 10.0f;
spells[1].attackstoptimer = 1000;

spells[2].info = dbcSpell.LookupEntry(CRUSADER);
spells[2].targettype = TARGET_ATTACKING;
spells[2].instant = true;
spells[2].perctrigger = 30.0f;
spells[2].attackstoptimer = 1000;

}

void OnCombatStart(Unit* mTarget)
{
_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Infidels. They must be purified!");

_unit->PlaySoundToSet(5835);

RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
}

void OnTargetDied(Unit* mTarget)
{

if (_unit->GetHealthPct() > 0)    
        {
int RandomSpeach;
(float)RandomFloat(1000);
RandomSpeach=rand()%2;
switch (RandomSpeach)
{
case 0:

_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Unworthy.");

_unit->PlaySoundToSet(5836);
break;

		}
	}
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
Timer = Timer + 1;

/*if (Timer == 30000000000) 
{
    _unit->CastSpell(_unit, spells[1].info, spells[1].instant); 
}

else
{*/
float val = (float)RandomFloat(100.0f);
SpellCast(val);
//}
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
target = _unit->GetAIInterface()->GetNextTarget();
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
_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant);
break;
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
	uint32 mPhase;
    int nrspells;
    int Timer;
};

// High Inquisitor Whitemane

#define WHITEMANE 3977
#define SMITE 9481
#define SLEEP 9256
#define RESURRECTION 25435
#define RESTALK2 5840

class WhitemaneAI : public CreatureAIScript
{
public:
ADD_CREATURE_FACTORY_FUNCTION(WhitemaneAI); 
SP_AI_Spell spells[3]; 
bool m_spellcheck[3]; 

WhitemaneAI(Creature *pCreature) : CreatureAIScript(pCreature)
{
nrspells = 3; 
mPhase = 0;

for(int i=0;i<nrspells;i++)
        {
            m_spellcheck[i] = false;
        }

spells[0].info = dbcSpell.LookupEntry(SMITE);
spells[0].targettype = TARGET_ATTACKING;
spells[0].instant = false;
spells[0].perctrigger = 15.0f;
spells[0].attackstoptimer = 1000;

spells[1].info = dbcSpell.LookupEntry(SLEEP);
spells[1].targettype = TARGET_ATTACKING;
spells[1].instant = true;
spells[1].perctrigger = 0.0f;
spells[1].attackstoptimer = 1000;

spells[2].info = dbcSpell.LookupEntry(RESURRECTION);
spells[2].targettype = TARGET_VARIOUS; //Can't seem to get her to cast it on Mograine...
spells[2].instant = false;
spells[2].perctrigger = 0.0f;
spells[2].attackstoptimer = 1000;
spells[2].soundid = RESTALK2;
spells[2].speech = "Arise, my champion!";
}

void OnCombatStart(Unit* mTarget)
{
_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Mograine has fallen? You shall pay for this treachery!");

_unit->PlaySoundToSet(5838);

RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
}

void OnTargetDied(Unit* mTarget)
{

if (_unit->GetHealthPct() > 0)    
        {
int RandomSpeach;
(float)RandomFloat(1000);
RandomSpeach=rand()%2;
switch (RandomSpeach)
{
case 0:

_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The light has spoken!");

_unit->PlaySoundToSet(5839);
break;

		}
	}
}

	void OnDamageTaken(Unit* mAttacker, float fAmount)
    {
        if(fAmount < 5) return;
        // <50% hp -> We go to phase 1
        if(_unit->GetHealthPct() <= 50 && mPhase == 0) {
            ChangeToPhase1();
        }
    }
	 
	void ChangeToPhase1()
    {
        // Set phase var
        mPhase = 1;

        // Play sound, and send text.
		/*_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Light, give me strength!");
		       ^^ Notes for myself */ 
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Arise, my champion!");
			//MoveToRes();
			CastSleep();
			CastRes();
    }

	/*void MoveToRes()
	{
		_unit->GetAIInterface ()->MoveTo(1154.859009,1403.924683,32.250183,3.466254);
	}*/

	void CastSleep()
	{
		_unit->CastSpell(_unit, spells[1].info, spells[1].instant);
	}

	void CastRes()
	{
		_unit->CastSpell(_unit, spells[2].info, spells[2].instant);
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
		Timer = Timer + 1;

	/*if (Timer == 30000000000) 
	{
		 //_unit->CastSpell(_unit, spells[1].info, spells[1].instant); 
	}	

	else
	{*/
	float val = (float)RandomFloat(100.0f);
	SpellCast(val);
	//}
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
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
						_unit->CastSpell(_unit, spells[i].info, spells[i].instant);break;
						case TARGET_ATTACKING:
						_unit->CastSpell(target, spells[i].info, spells[i].instant);break;
						case TARGET_DESTINATION:
						_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant);break;
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
    uint32 mPhase;  // NPC has 2 phases
    int nrspells;
    int Timer;
};


// High Inquisitor Fairbanks

#define FAIRBANKS 4542
#define BLOOD 40412 //Need a better spell
#define PWS 11647 //PWS = Power Word: Shield 

class FairbanksAI : public CreatureAIScript
{
public:
ADD_CREATURE_FACTORY_FUNCTION(FairbanksAI); 
SP_AI_Spell spells[2]; 
bool m_spellcheck[2]; 
FairbanksAI(Creature *pCreature) : CreatureAIScript(pCreature)
{
nrspells = 2; 
mPhase = 0;

for(int i=0;i<nrspells;i++)
        {
            m_spellcheck[i] = false;
        }

spells[0].info = dbcSpell.LookupEntry(BLOOD);
spells[0].targettype = TARGET_ATTACKING;
spells[0].instant = true;
spells[0].perctrigger = 15.0f;
spells[0].attackstoptimer = 1000;

spells[1].info = dbcSpell.LookupEntry(PWS);
spells[1].targettype = TARGET_SELF;
spells[1].instant = true;
spells[1].perctrigger = 15.0f;
spells[1].attackstoptimer = 1000;
}

void OnCombatStart(Unit* mTarget)
{
RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
}

void OnTargetDied(Unit* mTarget)
{

if (_unit->GetHealthPct() > 0)    
        {
int RandomSpeach;
(float)RandomFloat(1000);
RandomSpeach=rand()%2;
switch (RandomSpeach)
{
case 0:

_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Ha! Had enough?");

_unit->PlaySoundToSet(0000);
break;

		}
	}
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
Timer = Timer + 1;

if (Timer == 20000) 
{
    _unit->CastSpell(_unit, spells[1].info, spells[1].instant); 
}

else
{
float val = (float)RandomFloat(100.0f);
SpellCast(val);
}
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
target = _unit->GetAIInterface()->GetNextTarget();
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
_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant);
break;
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
    uint32 mPhase;  // NPC has 2 phases
    int nrspells;
    int Timer;
};

// Herod

#define HEROD 3975    
#define WHIRLWINDSPELL 9632
#define CHARGE 22911 
#define ENRAGESPELL 8269
#define AGGRO4 5830
#define KILL 5831
#define WHIRLWIND 5832
#define HEROD_ENRAGE 5833

class HerodAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(HerodAI); 
	SP_AI_Spell spells[3]; 
	bool m_spellcheck[3]; 
	
	HerodAI(Creature *pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 3; 
		mPhase = 0;
	for(int i=0;i<nrspells;i++)
		    {
		     m_spellcheck[i] = false;
			}

	spells[0].info = dbcSpell.LookupEntry(WHIRLWINDSPELL); 
	spells[0].targettype = TARGET_VARIOUS; 
	spells[0].instant = true; 
	spells[0].perctrigger = 10.0f; 
	spells[0].attackstoptimer = 1000; 
	spells[0].soundid = WHIRLWIND; 
	spells[0].speech = "Blades of Light!"; 

	spells[1].info = dbcSpell.LookupEntry(CHARGE);
	spells[1].targettype = TARGET_ATTACKING;
	spells[1].instant = true;
	spells[1].perctrigger = 10.0f;
	spells[1].attackstoptimer = 2000;

	spells[2].info = dbcSpell.LookupEntry(ENRAGESPELL); 
	spells[2].targettype = TARGET_SELF; 
	spells[2].instant = true; 
	spells[2].perctrigger = 0.0f; 
	spells[2].attackstoptimer = 1000; 
	spells[2].soundid = HEROD_ENRAGE; 
	spells[2].speech = "Light, give me strength!"; 

	}

	void OnCombatStart(Unit* mTarget)
	{
	_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Ah, I've been waiting for a real challenge!");
	_unit->PlaySoundToSet(5830);
	RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
	}

	void OnTargetDied(Unit* mTarget)
	{

		if (_unit->GetHealthPct() > 0)    // Hack to prevent double yelling (OnDied and OnTargetDied when creature is dying)
        {
			int RandomSpeach;
			(float)RandomFloat(1000);
			RandomSpeach=rand()%2;
			switch (RandomSpeach)
			{
			case 0:

				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Ha, is that all?");

				_unit->PlaySoundToSet(5831); //change the XXXXXX to whatever number that is in the soundentries.dbc file
				break;

			}
		}
	}

	void OnDamageTaken(Unit* mAttacker, float fAmount)
    {
        if(fAmount < 5) return;
        // <40% hp -> We go to phase 1
        if(_unit->GetHealthPct() <= 40 && mPhase == 0) {
            ChangeToPhase1();
        }
    }
	    
	void ChangeToPhase1()
    {
        // Set phase var
        mPhase = 1;

        // Play sound, and send text.
        _unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Light, give me strength!");
		HpEnrage();
    }


	void OnCombatStop(Unit *mTarget)
		{
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
		} 

	void HpEnrage()
	{
	    _unit->CastSpell(_unit, spells[2].info, spells[2].instant);
	}

	void AIUpdate()
	  {
	Timer = Timer + 1;

	if (Timer == 30000) 
	{
	   _unit->CastSpell(_unit, spells[1].info, spells[1].instant);
	}

	else
	{
	float val = (float)RandomFloat(100.0f);
	SpellCast(val);
	}
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
	target = _unit->GetAIInterface()->GetNextTarget();
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
	_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant);
	break;
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
    uint32 mPhase;  // NPC has 2 phases
    int nrspells;
    long Timer;
}; 

void SetupScarletMonastery(ScriptMgr * mgr)
{
    mgr->register_creature_script(LOKSEY, &LokseyAI::Create);
	mgr->register_creature_script(VISHAS, &VishasAI::Create);
	mgr->register_creature_script(COMMANDER_MOGRAINE, &MograineAI::Create);
	mgr->register_creature_script(WHITEMANE, &WhitemaneAI::Create);
	mgr->register_creature_script(FAIRBANKS, &FairbanksAI::Create);
	mgr->register_creature_script(HEROD, &HerodAI::Create); 
	mgr->register_creature_script(DOAN, &DoanAI::Create);
}
