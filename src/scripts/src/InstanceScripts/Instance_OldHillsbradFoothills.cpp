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
/* Instance_OldHillsbradFoothills.cpp Script                            */
/************************************************************************/

#define CN_LIEUTENANT_DRAKE 17848

#define FRIGHTENING_SHOUT 33789
#define WHIRLWIND 33239
#define MORTAL_STRIKE 17547
//#define EXPLODING_SHOT 33792 Only in Heroics

class LIEUTENANTDRAKEAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(LIEUTENANTDRAKEAI);
   SP_AI_Spell spells[3];
   bool m_spellcheck[3];

     LIEUTENANTDRAKEAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
      nrspells = 3;
      for(int i=0;i<nrspells;i++)
      {
         m_spellcheck[i] = false;
      }
      
	  spells[0].info = dbcSpell.LookupEntry(FRIGHTENING_SHOUT);
      spells[0].targettype = TARGET_VARIOUS;
      spells[0].instant = true;
      spells[0].cooldown = 8;
      spells[0].perctrigger = 0.0f;
      spells[0].attackstoptimer = 1000;
	  spells[0].speech = "Run, you blasted cowards!";

      spells[1].info = dbcSpell.LookupEntry(WHIRLWIND);
      spells[1].targettype = TARGET_VARIOUS;
      spells[1].instant = true;
      spells[1].cooldown = 10;
      spells[1].perctrigger = 0.0f;
      spells[1].attackstoptimer = 1000;
	  spells[1].speech = "Time to bleed!";
	  
      spells[2].info = dbcSpell.LookupEntry(MORTAL_STRIKE);
      spells[2].targettype = TARGET_ATTACKING;
      spells[2].instant = true;
      spells[2].cooldown = 5;
      spells[2].perctrigger = 0.0f;
      spells[2].attackstoptimer = 1000;
      
      /*spells[2].info = dbcSpell.LookupEntry(EXPLODING_SHOT);
      spells[2].targettype = TARGET_ATTACKING;
      spells[2].instant = true;
      spells[2].cooldown = 15;
      spells[2].perctrigger = 0.0f;
      spells[2].attackstoptimer = 1000;*/
    }
    
    void OnCombatStart(Unit* mTarget)
    {
      CastTime();
      _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I know what you're up to, and I mean to put an end to it, permanently!");
      _unit->PlaySoundToSet(10271);
      RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

   void CastTime()
   {
      for(int i=0;i<nrspells;i++)
         spells[i].casttime = spells[i].cooldown;
   }

   void OnTargetDied(Unit* mTarget)
    {
      if(_unit->GetHealthPct() > 0)
      {
         _unit->PlaySoundToSet(10271);
         _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You will not interfere!");
      }
    }

    void OnCombatStop(Unit *mTarget)
    {
      CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
      CastTime();
      _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Thrall... must not... go free.");
      _unit->PlaySoundToSet(10271);
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
            spells[i].casttime--;
            
            if (m_spellcheck[i])
            {               
               spells[i].casttime = spells[i].cooldown;
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
               
 
                 if (spells[i].speech != "")
                 {
                    _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
                    _unit->PlaySoundToSet(spells[i].soundid); 
                 }
				  

               m_spellcheck[i] = false;
               return;
            }

            if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
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

#define CN_CAPTAIN_SKARLOC 17862

//#define CLEANSE 39078 Needs support
#define HOLY_LIGHT 43451
#define HAMMER_OF_JUSTICE 13005
#define HOLY_SHIELD 31904
#define DEVOTION_AURA 41452
//#define CONSECRATION 41541 Only in Heroics

class CAPTAINSKARLOCAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(CAPTAINSKARLOCAI);
   SP_AI_Spell spells[4];
   bool m_spellcheck[4];

     CAPTAINSKARLOCAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
      nrspells = 4;
      for(int i=0;i<nrspells;i++)
      {
         m_spellcheck[i] = false;
      }
	  
      spells[0].info = dbcSpell.LookupEntry(HOLY_LIGHT);
      spells[0].targettype = TARGET_SELF;
      spells[0].instant = false;
      spells[0].cooldown = 15;
      spells[0].perctrigger = 0.0f;
      spells[0].attackstoptimer = 1000;
	  
      spells[0].info = dbcSpell.LookupEntry(HAMMER_OF_JUSTICE);
      spells[0].targettype = TARGET_ATTACKING;
      spells[0].instant = true;
      spells[0].cooldown = 10;
      spells[0].perctrigger = 0.0f;
      spells[0].attackstoptimer = 1000;

      spells[1].info = dbcSpell.LookupEntry(HOLY_SHIELD);
      spells[1].targettype = TARGET_SELF;
      spells[1].instant = true;
      spells[1].cooldown = 5;
      spells[1].perctrigger = 0.0f;
      spells[1].attackstoptimer = 1000;
	  
	  spells[1].info = dbcSpell.LookupEntry(DEVOTION_AURA);
      spells[1].targettype = TARGET_SELF;
      spells[1].instant = true;
      spells[1].cooldown = 5;
      spells[1].perctrigger = 0.0f;
      spells[1].attackstoptimer = 1000;
	  
	  /*spells[1].info = dbcSpell.LookupEntry(CLEANSE);
      spells[1].targettype = TARGET_SELF;
      spells[1].instant = true;
      spells[1].cooldown = 5;
      spells[1].perctrigger = 0.0f;
      spells[1].attackstoptimer = 1000;
      
      spells[2].info = dbcSpell.LookupEntry(CONSECRATION);
      spells[2].targettype = TARGET_SELF;
      spells[2].instant = true;
      spells[2].cooldown = 15;
      spells[2].perctrigger = 0.0f;
      spells[2].attackstoptimer = 1000;*/
    }
    
    void OnCombatStart(Unit* mTarget)
    {
      CastTime();
      _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Thrall! You didn't really think you would escape did you? You and your allies shall answer to Blackmoore - after I've had my fun!");
      _unit->PlaySoundToSet(10271);
      RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

   void CastTime()
   {
      for(int i=0;i<nrspells;i++)
         spells[i].casttime = spells[i].cooldown;
   }

   void OnTargetDied(Unit* mTarget)
    {
      if(_unit->GetHealthPct() > 0)
      {
         uint32 sound = 0;
         char *text;
         switch(RandomUInt(1))
         {
         case 0:
            sound = 10271;
            text = "Thrall will never be free!";
            break;
         case 1:
            sound = 10271;
            text = "Did you really think you would leave here alive?";
            break;
         }
         _unit->PlaySoundToSet(sound);
         _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, text);
      }
    }

    void OnCombatStop(Unit *mTarget)
    {
      CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
      CastTime();
      _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Guards! Urgh..Guards..!");
      _unit->PlaySoundToSet(10271);
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
            spells[i].casttime--;
            
            if (m_spellcheck[i])
            {               
               spells[i].casttime = spells[i].cooldown;
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

               if (spells[i].speech != "")
               {
                  _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
                  _unit->PlaySoundToSet(spells[i].soundid); 
               }

               m_spellcheck[i] = false;
               return;
            }

            if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
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

#define CN_EPOCH_HUNTER 18096

#define SAND_BREATH 31478 
#define IMPENDING_DOOM 19702
#define KNOCKBACK 26027
#define MAGIC_DISRUPTION_AURA 33834

class EPOCHHUNTERAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(EPOCHHUNTERAI);
   SP_AI_Spell spells[4];
   bool m_spellcheck[4];

     EPOCHHUNTERAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
      nrspells = 4;
      for(int i=0;i<nrspells;i++)
      {
         m_spellcheck[i] = false;
      }
      
	  spells[0].info = dbcSpell.LookupEntry(SAND_BREATH);
      spells[0].targettype = TARGET_DESTINATION;
      spells[0].instant = true;
      spells[0].cooldown = 15;
      spells[0].perctrigger = 0.0f;
      spells[0].attackstoptimer = 1000;

      spells[1].info = dbcSpell.LookupEntry(IMPENDING_DOOM);
      spells[1].targettype = TARGET_ATTACKING;
      spells[1].instant = true;
      spells[1].cooldown = 6;
      spells[1].perctrigger = 0.0f;
      spells[1].attackstoptimer = 1000;
      
      spells[2].info = dbcSpell.LookupEntry(KNOCKBACK);
      spells[2].targettype = TARGET_VARIOUS;
      spells[2].instant = true;
      spells[2].cooldown = 10;
      spells[2].perctrigger = 0.0f;
      spells[2].attackstoptimer = 1000;
	  
      spells[3].info = dbcSpell.LookupEntry(MAGIC_DISRUPTION_AURA);
      spells[3].targettype = TARGET_DESTINATION;
      spells[3].instant = true;
      spells[3].cooldown = 8;
      spells[3].perctrigger = 0.0f;
      spells[3].attackstoptimer = 1000;
    }
    
    void OnCombatStart(Unit* mTarget)
    {
      CastTime();
	  switch (RandomUInt(1))
	  {
	  case 0:
      _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Enough! I will erase your very existence!");
      _unit->PlaySoundToSet(10271);
	  break;
	  case 1: 
      _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You cannot fight fate!");
      _unit->PlaySoundToSet(10271);
	  break;
	  }
      RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

   void CastTime()
   {
      for(int i=0;i<nrspells;i++)
         spells[i].casttime = spells[i].cooldown;
   }

   void OnTargetDied(Unit* mTarget)
    {
      if(_unit->GetHealthPct() > 0)
      {
         uint32 sound = 0;
         char *text;
         switch (RandomUInt(1))
         {
         case 0:
            sound = 10271;
            text = "You are...irrelevant.";
            break;
         case 1:
            sound = 10271;
            text = "Thrall will remain a slave. Taretha will die. You have failed.";
            break;
         }
         _unit->PlaySoundToSet(sound);
         _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, text);
      }
    }

    void OnCombatStop(Unit *mTarget)
    {
      CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
      CastTime();
      _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "No!...The master... will not... be pleased.");
      _unit->PlaySoundToSet(10271);
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
            spells[i].casttime--;
            
            if (m_spellcheck[i])
            {               
               spells[i].casttime = spells[i].cooldown;
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

			   if (spells[i].info->Id == SAND_BREATH) //Sand Breath has 2 speeches, maybe there is an easier way to do it
			   {
			     switch(RandomUInt(1))
				 {
				 case 0:
                   _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Not so fast!");
                   _unit->PlaySoundToSet(spells[i].soundid);
				 break;
				 case 1:
                   _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Struggle as much as you like!");
                   _unit->PlaySoundToSet(spells[i].soundid);
				 break;
				 }
               }
               else
               {			   
                 if (spells[i].speech != "")
                 {
                    _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
                    _unit->PlaySoundToSet(spells[i].soundid); 
                 }
				}   

               m_spellcheck[i] = false;
               return;
            }

            if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
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

void SetupOldHillsbradFoothills(ScriptMgr * mgr)
{
   mgr->register_creature_script(CN_LIEUTENANT_DRAKE, &LIEUTENANTDRAKEAI::Create);
   mgr->register_creature_script(CN_EPOCH_HUNTER, &EPOCHHUNTERAI::Create);
   mgr->register_creature_script(CN_CAPTAIN_SKARLOC, &CAPTAINSKARLOCAI::Create);
}