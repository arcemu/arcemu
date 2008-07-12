#include "StdAfx.h"
#include "Setup.h"

/************************************************************************/
/* Instance_ShadowfangKeep.cpp Script                                   */
/************************************************************************/

// Commander Springvale nameid
#define CN_SPRINGVALE 4278
//bersek - 26662
#define DEVOTION_AURA 10290
//Hammer of justice rank 2
#define HAMMER_OF_JUSTICE 32416
//Divide Protection
#define DIVINE_PROTECTION 498

// Commander Springvale AI
class SpringvaleAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(SpringvaleAI);
    SpringvaleAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		m_fdivine = true;

        infodevotion = sSpellStore.LookupEntry(DEVOTION_AURA);
		infohammer = sSpellStore.LookupEntry(HAMMER_OF_JUSTICE);
		infodivine = sSpellStore.LookupEntry(DIVINE_PROTECTION);

    }
    
    void OnCombatStart(Unit* mTarget)
    {
        RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));

		_unit->CastSpell(_unit, infodevotion, false);
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
        if(_unit->GetHealthPct() <= 20 && m_fdivine)
        {
			// cast divine
			_unit->CastSpell(_unit, infodivine, false);
            m_fdivine = false;
		}
		uint32 val = sRand.randInt(1000);
        SpellCast(val);
    }

    void SpellCast(uint32 val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())//_unit->getAttackTarget())
        {
            if(m_hammerJustice)
            {
                _unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), infohammer, false);
                m_hammerJustice = false;
                return;
            }           

            if(val >= 100 && val <= 225)
            {
                _unit->setAttackTimer(1000, false);
                m_hammerJustice = true;
            }
        }
    }

protected:

    bool m_hammerJustice;
    bool m_fdivine;
    SpellEntry *infodevotion, *infohammer, *infodivine;
};


// Howling Rage 1 - 7481 
// Howling Rage 2 - 7483
// Howling Rage 3 - 7484


#define CN_BLINDWATCHER 4279

#define HOWLRAGE1 7481
#define HOWLRAGE2 7483
#define HOWLRAGE3 7484
// Odo the Blindwatcher AI
class BlindWatcherAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(BlindWatcherAI);
    BlindWatcherAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		m_rage1 = m_rage2 = m_rage3 =true;

        infohrage1 = sSpellStore.LookupEntry(HOWLRAGE1);
		infohrage2 = sSpellStore.LookupEntry(HOWLRAGE2);
		infohrage3 = sSpellStore.LookupEntry(HOWLRAGE3);
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
        if(_unit->GetHealthPct() <= 75 && m_rage1)
        {
			// cast divine
			_unit->CastSpell(_unit, infohrage1, false);
            m_rage1 = false;
		}
		else if(_unit->GetHealthPct() <= 45 && m_rage2)
        {
            _unit->RemoveAura(HOWLRAGE1);
			// cast divine
			_unit->CastSpell(_unit, infohrage2, false);
            m_rage2 = false;
		}
		else if(_unit->GetHealthPct() <= 20 && m_rage3)
        {
            _unit->RemoveAura(HOWLRAGE2);
			// cast divine
			_unit->CastSpell(_unit, infohrage3, false);
            m_rage3 = false;
		}
    }

protected:

    bool m_rage1,m_rage2,m_rage3;
    SpellEntry *infohrage1, *infohrage2, *infohrage3;
};

#define CN_FENRUS 4274
// Fenrus the Devourer AI
class FenrusAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(FenrusAI);
    FenrusAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {

    }
    
    void OnCombatStart(Unit* mTarget)
    {

    }

    void OnCombatStop(Unit *mTarget)
    {

    }

    void OnDied(Unit * mKiller)
    {
        //must be arugal saying it need to find how
       _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Who dares interfere with the Sons of Arugal?");
       _unit->PlaySoundToSet(5791);
    }

protected:

};

#define CN_ARUGAL 4275

#define SHADOWBOLT 7124
#define MINDCONTROL 7621
#define TELEPORT 10418
//# "Who dares interfere with the Sons of Arugal?" -after killing Fenrus
//# Aggro: "You, too, shall serve!"
//# Player death: "Another falls!"
//# "Release your rage!"

// Archmage Arugal AI
class ArugalAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(ArugalAI);
    ArugalAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
        m_shadownbolt = m_mindcontrol = m_teleport = true;
        infoshadowbolt = sSpellStore.LookupEntry(SHADOWBOLT);
		infomindcontrol = sSpellStore.LookupEntry(MINDCONTROL);
		infoteleport = sSpellStore.LookupEntry(TELEPORT);

    }
    
    void OnCombatStart(Unit* mTarget)
    {
        RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));

		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You, too, shall serve!");
        _unit->PlaySoundToSet(5793);
    }
    
    void OnTargetDied(Unit* mTarget)
    {
        _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Another falls!");
        _unit->PlaySoundToSet(5795);
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
		uint32 val = sRand.randInt(1000);
        SpellCast(val);
    }

    void SpellCast(uint32 val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())//_unit->getAttackTarget())
        {
            if(m_shadownbolt)
            {
                _unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), infoshadowbolt, false);
                m_shadownbolt = false;
                return;
            }
            if(m_mindcontrol)
            {
                _unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), infomindcontrol, false);
                m_mindcontrol = false;
                _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Release your rage!");
                _unit->PlaySoundToSet(5797);
                return;
            }    
            if(m_teleport)
            {
                _unit->CastSpell(_unit, infoteleport, false);
                m_teleport = false;
                return;
            }               

            if(val >= 120 && val <= 225)
            {
                _unit->setAttackTimer(2000, false);
                m_shadownbolt = true;
            }
            else if(val > 300 && val <= 350)
            {
                _unit->setAttackTimer(4000, false);
                m_mindcontrol = true;
            }
            else if(val > 350 && val <= 500)
            {
                _unit->setAttackTimer(4000, false);
                m_teleport = true;
            }
        }
    }

protected:

   bool m_shadownbolt;
   bool m_mindcontrol;
   bool m_teleport;
   SpellEntry *infoshadowbolt, *infomindcontrol, *infoteleport;
};

void SetupShadowfangKeep(ScriptMgr * mgr)
{
    mgr->register_creature_script(CN_SPRINGVALE, &SpringvaleAI::Create);
    mgr->register_creature_script(CN_BLINDWATCHER, &BlindWatcherAI::Create);
    mgr->register_creature_script(CN_FENRUS, &FenrusAI::Create);
    mgr->register_creature_script(CN_ARUGAL, &ArugalAI::Create);
}
