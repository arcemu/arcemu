#include "StdAfx.h"
#include "Setup.h"

// Rhakzor - Sends sound and talk message on enter combat, opens door on death.

class RhahkZorAI : public CreatureAIScript
{
public:
    RhahkZorAI(Creature* pCreature) : CreatureAIScript(pCreature) {}

    void OnDied(Unit * mKiller)
    {
        // Find "Factory Door"
        // GetGameObjectNearestCoords works here too.

        GameObject *pDoor = _unit->GetMapMgr()->GetInterface()->GetObjectNearestCoords<GameObject, TYPEID_GAMEOBJECT>(
            13965, -190.860092f, -456.332184f, 54.496822f);
        if(pDoor == 0)
            return;

        // Open the door
        pDoor->SetUInt32Value(GAMEOBJECT_FLAGS, 33);
        pDoor->SetUInt32Value(GAMEOBJECT_STATE, 0);
    }

    void OnCombatStart(Unit* mTarget)
    {
        _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Van Cleef pay big for your heads!");
        _unit->PlaySoundToSet(5774);
    }

    void Destroy()
    {
        delete (RhahkZorAI*)this;
    }

    static CreatureAIScript *Create(Creature * c) { return new RhahkZorAI(c); }
};

CreatureAIScript * create_rhakzor(Creature * c) { return new RhahkZorAI(c); }

// Sneeds Shredder -> Opens Mast Room door on death.

class SneedsShredderAI : public CreatureAIScript
{
public:
    SneedsShredderAI(Creature* pCreature) : CreatureAIScript(pCreature) {}
    void OnDied(Unit * mKiller)
    {
        // Find "Mast Room Door"
        // GetGameObjectNearestCoords works here too.

        GameObject *pDoor = _unit->GetMapMgr()->GetInterface()->GetObjectNearestCoords<GameObject, TYPEID_GAMEOBJECT>(
            16400, -289.691650f, -535.988953f, 49.440678f);
        if(pDoor == 0)
            return;

        // Open the door
        pDoor->SetUInt32Value(GAMEOBJECT_FLAGS, 33);
        pDoor->SetUInt32Value(GAMEOBJECT_STATE, 0);
    }

    void Destroy()
    {
        delete (SneedsShredderAI*)this;
    }

    static CreatureAIScript * Create(Creature * c) { return new SneedsShredderAI(c); }
};

// Gilnid -> Opens Foundary door on death.

class GilnidAI : public CreatureAIScript
{
public:
    GilnidAI(Creature* pCreature) : CreatureAIScript(pCreature) {}
    void OnDied(Unit * mKiller)
    {
        // Find "Foundry Door"
        // GetGameObjectNearestCoords works here too.

        GameObject *pDoor = _unit->GetMapMgr()->GetInterface()->GetObjectNearestCoords<GameObject, TYPEID_GAMEOBJECT>(
            16399, -176.569f, -577.640991f, 19.311600f);
        if(pDoor == 0)
            return;

        // Open the door
        pDoor->SetUInt32Value(GAMEOBJECT_FLAGS, 33);
        pDoor->SetUInt32Value(GAMEOBJECT_STATE, 0);
    }

    void Destroy()
    {
        delete (GilnidAI*)this;
    }

    static CreatureAIScript * Create(Creature * c) { return new GilnidAI(c); }
};

class VanCleefAI : public CreatureAIScript
{
public:
    VanCleefAI(Creature *pCreature) : CreatureAIScript(pCreature)
    {
        mPhase = 0;
    }

    void OnCombatStart(Unit* mTarget)
    {
        _unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "None may challenge the brotherhood.");
        _unit->PlaySoundToSet(5780);    // EdwinVanCleefAggro01.wav
    }

    void OnTargetDied(Unit* mTarget)
    {
        char msg[200];
        if(mTarget->GetTypeId() == TYPEID_PLAYER)
            sprintf(msg, "And stay down, %s.", static_cast<Player*>(mTarget)->GetName());
        else if (mTarget->IsPet())
            sprintf(msg, "And stay down, %s.", static_cast<Pet*>(mTarget)->GetName().c_str());

        _unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, msg);
        _unit->PlaySoundToSet(5781);    // EdwinVanCleefSlay01.wav
    }

    void OnDamageTaken(Unit* mAttacker, float fAmount)
    {
        if(fAmount < 5) return;

        // <100% hp -> We go to phase 1
        if(_unit->GetHealthPct() <= 100 && mPhase == 0) {
            ChangeToPhase1();
        }

        // <67% hp -> We go to phase 2
        if(_unit->GetHealthPct() <= 67 && mPhase == 1) {
            ChangeToPhase2();
        }

        // <34% hp -> We go to phase 3
        if(_unit->GetHealthPct() <= 34 && mPhase == 2) {
            ChangeToPhase3();
        }
    }

    void ChangeToPhase1()
    {
        // Set phase var
        mPhase = 1;
        
        // Play the sound, and send text.
        _unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Lap dogs, all of you.");
        _unit->PlaySoundToSet(5782);    // EdwinVanCleefHealth01.wav
    }

    void ChangeToPhase2()
    {
        // Set phase var
        mPhase = 2;

        // Play sound, and send text.
        _unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Fools! Our cause is righteous.");
        _unit->PlaySoundToSet(5783);    // EdwinVanCleefHealth02.wav
    }

    void ChangeToPhase3()
    {
        // Set phase var
        mPhase = 3;

        // Play sound, and send text.
        _unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "The brotherhood shall remain.");
        _unit->PlaySoundToSet(5784);    // EdwinVanCleefHealth03.wav
    }

    void Destroy()
    {
        delete (VanCleefAI*)this;
    }

    static CreatureAIScript * Create(Creature * c) { return new VanCleefAI(c); }

protected:
    uint32 mPhase;  // NPC has 3 phases
};

class MrSmiteAI : public CreatureAIScript
{
public:
    MrSmiteAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
        mPhase = 0;
    }

    void OnCombatStart(Unit* mTarget)
    {
        // This guy has 2 messages he can say upon entering combat. Let's
        // make a 50/50 chance of using one.

        bool altmsg = false;
        if(Rand(50.0f))
            altmsg = true;
        char msg[200];
        uint32 soundid;

        if(altmsg) {
            sprintf(msg, "You there! Check out that noise.");
            soundid = 5775;
        } else {
            sprintf(msg, "We're under attack! Repel the invaders!");
            soundid = 5777;
        }

        _unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, msg);
        _unit->PlaySoundToSet(soundid);
    }
    
    // 2 phases, someone needs to script the combat changes
    void OnDamageTaken(Unit* mAttacker, float fAmount)
    {
        if(fAmount < 5) return;

        // <100% hp -> We go to phase 1
        if(_unit->GetHealthPct() <= 100 && mPhase == 0) {
            ChangeToPhase1();
        }

        // <50% hp -> We go to phase 2
        if(_unit->GetHealthPct() <= 50 && mPhase == 1) {
            ChangeToPhase2();
        }
    }

    void ChangeToPhase1()
    {
        mPhase = 1;
        _unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You land lovers are tougher than I thought! I'll have to improvise.");
        _unit->PlaySoundToSet(5778);
    }

    void ChangeToPhase2()
    {
        mPhase = 2;
        _unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Now you're making me angry!");
        _unit->PlaySoundToSet(5779);
    }

    ADD_CREATURE_FACTORY_FUNCTION(MrSmiteAI);

protected:
    uint32 mPhase;
};

void SetupDeadmines(ScriptMgr * mgr)
{
    mgr->register_creature_script(644, &RhahkZorAI::Create);
    mgr->register_creature_script(642, &SneedsShredderAI::Create);
    mgr->register_creature_script(1763, &GilnidAI::Create);
    mgr->register_creature_script(639, &VanCleefAI::Create);
    mgr->register_creature_script(646, &MrSmiteAI::Create);
}
