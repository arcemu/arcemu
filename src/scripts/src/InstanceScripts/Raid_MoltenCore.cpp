#include "StdAfx.h"
#include "Setup.h"

/************************************************************************/
/* Raid_MoltenCore.cpp Script											*/
/************************************************************************/

#define CN_FIREWALKER 11666

#define MELT_ARMOR 19631 // reduces armor by 1000
#define INCITE_FLAME 19635 // reduces fire resistance
// Firewalker AI
class FirewalkerAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(FirewalkerAI);

    FirewalkerAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		m_meltarmor = m_inciteflame = true;

        infomeltarmor = dbcSpell.LookupEntry(MELT_ARMOR);
		infoinciteflame = dbcSpell.LookupEntry(INCITE_FLAME);
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
		uint32 val = RandomUInt(1000);
        SpellCast(val);
    }

    void SpellCast(uint32 val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())//_unit->getAttackTarget())
        {
            if(m_meltarmor)
            {
                _unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), infomeltarmor, false);
                m_meltarmor = false;
                return;
            }
            
            if(m_inciteflame)
            {
                _unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), infoinciteflame, false);
                m_inciteflame = false;
                return;
            }           

            if(val >= 100 && val <= 160)
            {
                _unit->setAttackTimer(1000, false);
                m_meltarmor = true;
            }
            if(val > 160 && val <= 240)
            {
                _unit->setAttackTimer(1000, false);
                m_inciteflame = true;
            }
        }
    }

protected:

    bool m_meltarmor;
    bool m_inciteflame;
    SpellEntry *infomeltarmor, *infoinciteflame;
};


#define CN_FLAMEGUARD 11667

#define FLAMETHROWER 25027
#define FIRE_SHIELD 19627
#define FLAMES 19628
// Flameguard AI
class FlameguardAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(FlameguardAI);

    FlameguardAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		m_flamethrower = m_fireshield = m_flames =true;

        infoflamethrower = dbcSpell.LookupEntry(FLAMETHROWER);
		infofireshield = dbcSpell.LookupEntry(FIRE_SHIELD);
		infoflames = dbcSpell.LookupEntry(FLAMES);
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
        if(_unit->GetHealthPct() <= 10 && m_flames)
        {
			// cast divine
			_unit->CastSpell(_unit, infoflames, false);
            m_flames = false;
		}
		
	    uint32 val = RandomUInt(1000);
        SpellCast(val);
    }

    void SpellCast(uint32 val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())//_unit->getAttackTarget())
        {
            if(m_flamethrower)
            {
                _unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), infoflamethrower, false);
                m_flamethrower = false;
                return;
            }
            
            if(m_fireshield)
            {
                _unit->CastSpell(_unit, infofireshield, false);
                m_fireshield = false;
                return;
            }           

            if(val >= 100 && val <= 160)
            {
                _unit->setAttackTimer(1000, false);
                m_flamethrower = true;
            }
            if(val > 160 && val <= 240)
            {
                _unit->setAttackTimer(1000, false);
                m_fireshield = true;
            }
        }
    }

protected:

    bool m_flamethrower,m_fireshield,m_flames;
    SpellEntry *infoflamethrower, *infofireshield, *infoflames;
};

#define CN_LAVAELEMENTAL 12076

#define STRIKE 31754
#define PYROCLAST_BARRAGE 19641
// Lava Elemental AI
class LavaelementalAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(LavaelementalAI);

    LavaelementalAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		m_strike = m_pyroclast = true;

        infostrike = dbcSpell.LookupEntry(STRIKE);
		infopyroclast = dbcSpell.LookupEntry(PYROCLAST_BARRAGE);
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
	    uint32 val = RandomUInt(1000);
        SpellCast(val);
    }

    void SpellCast(uint32 val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())//_unit->getAttackTarget())
        {
            if(m_strike)
            {
                _unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), infostrike, false);
                m_strike = false;
                return;
            }
            
            if(m_pyroclast)
            {
                _unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), infopyroclast, false);
                m_pyroclast = false;
                return;
            }           

            if(val >= 100 && val <= 160)
            {
                _unit->setAttackTimer(1000, false);
                m_strike = true;
            }
            if(val > 160 && val <= 240)
            {
                _unit->setAttackTimer(1000, false);
                m_pyroclast = true;
            }
        }
    }

protected:

    bool m_strike,m_pyroclast;
    SpellEntry *infostrike, *infopyroclast;
};


#define CN_LAVAREAVER 12100

#define CLEAVE 31345
// Lave Reaver AI
class LavaReaverAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(LavaReaverAI);

    LavaReaverAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		m_cleave =true;

        infocleave = dbcSpell.LookupEntry(CLEAVE);
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
	
	    uint32 val = RandomUInt(1000);
        SpellCast(val);
    }

    void SpellCast(uint32 val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())//_unit->getAttackTarget())
        {
            if(m_cleave)
            {
                _unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), infocleave, false);
                m_cleave = false;
                return;
            }          

            if(val >= 100 && val <= 300)
            {
                _unit->setAttackTimer(1000, false);
                m_cleave = true;
            }
        }
    }

protected:

    bool m_cleave;
    SpellEntry *infocleave;
};


#define CN_FLAMEIMP 11669

#define FIRENOVA 25537
// Flame Imp AI
class FlameImpAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(FlameImpAI);
    FlameImpAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		m_firenova =true;

        infofirenova = dbcSpell.LookupEntry(FIRENOVA);
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
	
	    uint32 val = RandomUInt(1000);
        SpellCast(val);
    }

    void SpellCast(uint32 val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())//_unit->getAttackTarget())
        {
            if(m_firenova)
            {
                _unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), infofirenova, true);
                m_firenova = false;
                return;
            }          

            if(val >= 100 && val <= 350)
            {
                _unit->setAttackTimer(3000, false);
                m_firenova = true;
            }
        }
    }

protected:

    bool m_firenova;
    SpellEntry *infofirenova;
};


#define CN_LAVASURGER 12101

#define SURGE 25787
// Lava Surger AI
class LavaSurgerAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(LavaSurgerAI);
    LavaSurgerAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		m_surge =true;

        infosurge = dbcSpell.LookupEntry(SURGE);
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
	
	    uint32 val = RandomUInt(1000);
        SpellCast(val);
    }

    void SpellCast(uint32 val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())//_unit->getAttackTarget())
        {
            if(m_surge)
            {
                _unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), infosurge, false);
                m_surge = false;
                return;
            }          

            if(val >= 100 && val <= 225)
            {
                _unit->setAttackTimer(1000, false);
                m_surge = true;
            }
        }
    }

protected:

    bool m_surge;
    SpellEntry *infosurge;
};


#define CN_FIRELORD 11668 

#define SUMMON_LAVA_SPAWN 19392 // TODO: this spell needs to be done on core
#define SOUL_BURN 19393
// FireLord AI
class FireLordAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(FireLordAI);
    FireLordAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		m_lavaspawn = m_soulburn = true;

        infolavaspawn = dbcSpell.LookupEntry(SUMMON_LAVA_SPAWN);
		infosoulburn = dbcSpell.LookupEntry(SOUL_BURN);
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
	    uint32 val = RandomUInt(1000);
        SpellCast(val);
    }

    void SpellCast(uint32 val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())//_unit->getAttackTarget())
        {
            if(m_lavaspawn)
            {
                _unit->CastSpell(_unit, infolavaspawn, false);
                m_lavaspawn = false;
                return;
            }
            
            if(m_soulburn)
            {
                _unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), infosoulburn, false);
                m_soulburn = false;
                return;
            }           

            if(val >= 100 && val <= 160)
            {
                _unit->setAttackTimer(1000, false);
                m_lavaspawn = true;
            }
            if(val > 160 && val <= 240)
            {
                _unit->setAttackTimer(1000, false);
                m_soulburn = true;
            }
        }
    }

protected:

    bool m_lavaspawn,m_soulburn;
    SpellEntry *infolavaspawn, *infosoulburn;
};


#define CN_MOLTENDESTROYER 11659

#define KNOCKDOWN 5164 

// Molten Destroyer AI
class MoltenDestroyerAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(MoltenDestroyerAI);
    MoltenDestroyerAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		m_knockdown = true;

        infoknockdown = dbcSpell.LookupEntry(KNOCKDOWN);
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
	    uint32 val = RandomUInt(1000);
        SpellCast(val);
    }

    void SpellCast(uint32 val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())//_unit->getAttackTarget())
        {           
            if(m_knockdown)
            {
                _unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), infoknockdown, false);
                m_knockdown = false;
                return;
            }           

            if(val >= 100 && val <= 225)
            {
                _unit->setAttackTimer(1000, false);
                m_knockdown = true;
            }
        }
    }

protected:

    bool m_knockdown;
    SpellEntry *infoknockdown;
};


#define CN_MOLTENGIANT 11658

#define STOMP 31900
#define KNOCKBACK 30056

// Molten Giant AI
class MoltenGiantAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(MoltenGiantAI);
    MoltenGiantAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		m_stomp = m_knockback = true;

        infostomp = dbcSpell.LookupEntry(STOMP);
		infoknockback = dbcSpell.LookupEntry(KNOCKBACK);
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
	    uint32 val = RandomUInt(1000);
        SpellCast(val);
    }

    void SpellCast(uint32 val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())//_unit->getAttackTarget())
        {
            if(m_stomp)
            {
                _unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), infostomp, false);
                m_stomp = false;
                return;
            }
            
            if(m_knockback)
            {
                _unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), infoknockback, false);
                m_knockback = false;
                return;
            }           

            if(val >= 100 && val <= 200)
            {
                _unit->setAttackTimer(1000, false);
                m_stomp = true;
            }
            if(val > 200 && val <= 275)
            {
                _unit->setAttackTimer(1000, false);
                m_knockback = true;
            }
        }
    }

protected:

    bool m_stomp,m_knockback;
    SpellEntry *infostomp, *infoknockback;
};

// ------ TIME TO DO THE BOSS'S :D --------

#define CN_LUCIFRON 12118

#define IMPEDING_DOOM 19702
#define LUCIFRON_CURSE 19703
#define SHADOW_SHOCK 20603

// Lucifron AI
class LucifronAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(LucifronAI);
    LucifronAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		m_impedingdoom = m_lucifroncurse = m_shadowshock = true;

        infoimpedingdoom = dbcSpell.LookupEntry(IMPEDING_DOOM);
		infolucifroncurse = dbcSpell.LookupEntry(LUCIFRON_CURSE);
		infoshadowshock =  dbcSpell.LookupEntry(SHADOW_SHOCK);
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
	    uint32 val = RandomUInt(1000);
        SpellCast(val);
    }

    void SpellCast(uint32 val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())//_unit->getAttackTarget())
        {
            if(m_impedingdoom)
            {
                _unit->CastSpell(_unit, infoimpedingdoom, false);
                m_impedingdoom = false;
                return;
            }
            
            if(m_lucifroncurse)
            {
                _unit->CastSpell(_unit, infolucifroncurse, false);
                m_lucifroncurse = false;
                return;
            }

			if(m_shadowshock)
            {
                _unit->CastSpell(_unit, infoshadowshock, false);
                m_shadowshock = false;
                return;
            }  

            if(val >= 100 && val <= 180)
            {
                _unit->setAttackTimer(1000, false);
                m_impedingdoom = true;
            }
            if(val > 180 && val <= 260)
            {
                _unit->setAttackTimer(1000, false);
                m_lucifroncurse = true;
            }
			if(val > 260 && val <= 340)
            {
                _unit->setAttackTimer(1000, false);
                m_shadowshock = true;
            }
        }
    }

protected:

    bool m_impedingdoom,m_lucifroncurse,m_shadowshock;
    SpellEntry *infoimpedingdoom, *infolucifroncurse,*infoshadowshock;
};

#define CN_MAGMADAR 11982

#define MAGMASPIT 19450
#define LAVABREATH 19272
#define PANIC 19408
#define LAVABOMB 19411 // TODO: spell to be dummy scripted
// Magmadar AI
class MagmadarAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(MagmadarAI);
    MagmadarAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		m_magmaspit = m_lavabreath = m_panic = true;

        infomagmaspit = dbcSpell.LookupEntry(MAGMASPIT);
		infolavabreath = dbcSpell.LookupEntry(LAVABREATH);
		infopanic =  dbcSpell.LookupEntry(PANIC);
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
	    uint32 val = RandomUInt(1000);
        SpellCast(val);
    }

    void SpellCast(uint32 val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())//_unit->getAttackTarget())
        {
            if(m_magmaspit)
            {
                _unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), infomagmaspit, false);
                m_magmaspit = false;
                return;
            }
            
            if(m_lavabreath)
            {
                _unit->CastSpell(_unit, infolavabreath, false);
                m_lavabreath = false;
                return;
            }

			if(m_panic)
            {
                _unit->CastSpell(_unit, infopanic, false);
                m_panic = false;
                return;
            }  

            if(val >= 100 && val <= 180)
            {
                _unit->setAttackTimer(1000, false);
                m_magmaspit = true;
            }
            if(val > 180 && val <= 260)
            {
                _unit->setAttackTimer(1000, false);
                m_lavabreath = true;
            }
			if(val > 260 && val <= 340)
            {
                _unit->setAttackTimer(1000, false);
                m_panic = true;
            }
        }
    }

protected:

    bool m_magmaspit,m_lavabreath,m_panic;
    SpellEntry *infomagmaspit, *infolavabreath,*infopanic;
};


#define CN_GEHENNAS 12259

#define SHADOW_BOLT 29317
#define GEHENNAS_CURSE 19716
#define RAIN_OF_FIRE 19717

// Gehennas AI
class GehennasAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(GehennasAI);
    GehennasAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		m_shadowbolt = m_gehennascurse = m_rainoffire = true;

        infoshadowbolt = dbcSpell.LookupEntry(SHADOW_BOLT);
		infogehennascurse = dbcSpell.LookupEntry(GEHENNAS_CURSE);
		inforainoffire =  dbcSpell.LookupEntry(RAIN_OF_FIRE);
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
	    uint32 val = RandomUInt(1000);
        SpellCast(val);
    }

    void SpellCast(uint32 val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())//_unit->getAttackTarget())
        {
			Unit *target = _unit->GetAIInterface()->GetNextTarget();
            if(m_shadowbolt)
            {
                _unit->CastSpell(target, infoshadowbolt, true);
                m_shadowbolt = false;
                return;
            }
            
            if(m_gehennascurse)
            {
                _unit->CastSpell(_unit, infogehennascurse, false);
                m_gehennascurse = false;
                return;
            }

			if(m_rainoffire)
            {
                _unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), inforainoffire, false);
                m_rainoffire = false;
                return;
            }  

            if(val >= 100 && val <= 180)
            {
                _unit->setAttackTimer(3000, false);
                m_shadowbolt = true;
            }
            if(val > 180 && val <= 260)
            {
                _unit->setAttackTimer(1000, false);
                m_gehennascurse = true;
            }
			if(val > 260 && val <= 300)
            {
                _unit->setAttackTimer(1000, false);
                m_rainoffire = true;
            }
        }
    }

protected:

    bool m_shadowbolt,m_gehennascurse,m_rainoffire;
    SpellEntry *infoshadowbolt, *infogehennascurse,*inforainoffire;
};


#define CN_FLAMEWAKER 11661

#define SUNDERARMOR 25051
#define FISTOFRAGNAROS 20277
#define FSTRIKE 11998

// Flamewaker AI
class FlamewakerAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(FlamewakerAI);
    FlamewakerAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		m_sunderarmor = m_fistofragnaros = m_strike = true;

        infosunderarmor = dbcSpell.LookupEntry(SUNDERARMOR);
		infofistofragnaros = dbcSpell.LookupEntry(FISTOFRAGNAROS);
		infostrike =  dbcSpell.LookupEntry(FSTRIKE);
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
	    uint32 val = RandomUInt(1000);
        SpellCast(val);
    }

    void SpellCast(uint32 val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())//_unit->getAttackTarget())
        {
			Unit *target = _unit->GetAIInterface()->GetNextTarget();
            if(m_sunderarmor)
            {
                _unit->CastSpell(target, infosunderarmor, false);
                m_sunderarmor = false;
                return;
            }
            
            if(m_fistofragnaros)
            {
                _unit->CastSpell(_unit, infofistofragnaros, false);
                m_fistofragnaros = false;
                return;
            }

			if(m_strike)
            {
                _unit->CastSpell(target, infostrike, false);
                m_strike = false;
                return;
            }  

            if(val >= 100 && val <= 180)
            {
                _unit->setAttackTimer(1000, false);
                m_sunderarmor = true;
            }
            if(val > 180 && val <= 260)
            {
                _unit->setAttackTimer(1000, false);
                m_fistofragnaros = true;
            }
			if(val > 260 && val <= 400)
            {
                _unit->setAttackTimer(1000, false);
                m_strike = true;
            }
        }
    }

protected:

    bool m_sunderarmor,m_fistofragnaros,m_strike;
    SpellEntry *infosunderarmor, *infofistofragnaros,*infostrike;
};


#define CN_GARR 12057

#define ANTIMAGIC_PULSE 19492 // various targets
#define MAGMA_SHACKES 19496 // various targets

// Garr AI
class GarrAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(GarrAI);
    GarrAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		m_antimagic = m_magmashackes = true;

        infoantimagic = dbcSpell.LookupEntry(ANTIMAGIC_PULSE);
		infomagmashackes = dbcSpell.LookupEntry(MAGMA_SHACKES);
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
	    uint32 val = RandomUInt(1000);
        SpellCast(val);
    }

    void SpellCast(uint32 val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())//_unit->getAttackTarget())
        {
			//Unit *target = _unit->GetAIInterface()->GetNextTarget();
            if(m_antimagic)
            {
                _unit->CastSpell(_unit, infoantimagic, false);
                m_antimagic = false;
                return;
            }
            
            if(m_magmashackes)
            {
                _unit->CastSpell(_unit, infomagmashackes, false);
                m_magmashackes = false;
                return;
            }

            if(val >= 100 && val <= 200)
            {
                _unit->setAttackTimer(1000, false);
                m_antimagic = true;
            }
            if(val > 200 && val <= 300)
            {
                _unit->setAttackTimer(1000, false);
                m_magmashackes = true;
            }
        }
    }

protected:

    bool m_antimagic,m_magmashackes;
    SpellEntry *infoantimagic, *infomagmashackes;
};


#define CN_FIRESWORN 12099

#define IMMOLATE 20294 // 1 target
#define ERUPTION 20527 // check

// FireSworn AI
class FireSwornAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(FireSwornAI);
    FireSwornAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		m_immolate = m_eruption = true;

        infoimmolate = dbcSpell.LookupEntry(IMMOLATE);
		infoeruption = dbcSpell.LookupEntry(ERUPTION);
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
	    uint32 val = RandomUInt(1000);
        SpellCast(val);
    }

    void SpellCast(uint32 val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())//_unit->getAttackTarget())
        {
			//Unit *target = _unit->GetAIInterface()->GetNextTarget();
            if(m_immolate)
            {
                _unit->CastSpell(_unit, infoimmolate, false);
                m_immolate = false;
                return;
            }
            
            if(m_eruption)
            {
                //_unit->CastSpell(_unit, infoeruption, false);
                m_eruption = false;
                return;
            }

            if(val >= 100 && val <= 200)
            {
                _unit->setAttackTimer(1000, false);
                m_immolate = true;
            }
            if(val > 200 && val <= 300)
            {
                _unit->setAttackTimer(1000, false);
                m_eruption = true;
            }
        }
    }

protected:

    bool m_immolate,m_eruption;
    SpellEntry *infoimmolate, *infoeruption;
};


#define CN_BARON_GEDDON 12056

#define INFERNO 35268 // various targets
#define IGNITE_MANA 19659 // various targets
#define LIVING_BOMB 20475 // 1 target

// BaronGeddon AI
class BaronGeddonAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(BaronGeddonAI);
    BaronGeddonAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		m_inferno = m_ignitemana = m_livingbomb = true;

        infoinferno = dbcSpell.LookupEntry(INFERNO);
		infoignitemana = dbcSpell.LookupEntry(IGNITE_MANA);
		infolivingbomb = dbcSpell.LookupEntry(LIVING_BOMB);
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
	    uint32 val = RandomUInt(1000);
        SpellCast(val);
    }

    void SpellCast(uint32 val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())//_unit->getAttackTarget())
        {
			Unit *target = _unit->GetAIInterface()->GetNextTarget();
            if(m_inferno)
            {
                _unit->CastSpell(_unit, infoinferno, false);
                m_inferno = false;
                return;
            }
            
            if(m_ignitemana)
            {
                _unit->CastSpell(_unit, infoignitemana, false);
                m_ignitemana = false;
                return;
            }
			if(m_livingbomb)
            {
                _unit->CastSpell(target, infolivingbomb, true);
                m_livingbomb = false;
                return;
            }

            if(val >= 100 && val <= 180)
            {
                _unit->setAttackTimer(9000, false);
                m_inferno = true;
            }
            if(val > 180 && val <= 260)
            {
                _unit->setAttackTimer(1000, false);
                m_ignitemana = true;
            }
			if(val > 260 && val <= 340)
            {
                _unit->setAttackTimer(1000, false);
                m_livingbomb = true;
            }
        }
    }

protected:

    bool m_inferno,m_ignitemana,m_livingbomb;
    SpellEntry *infoinferno, *infoignitemana, *infolivingbomb;
};

#define CN_SHAZZRAH 12264

#define ARCANE_EXPLOSION 19712 // various targets
#define SHAZZRAH_CURSE 19713 // various targets
#define DEADEN_MAGIC 19714 // self
#define COUNTERSPELL 19715 // various targets
#define BLINK 29883 // TODO: choose a target to blink, is dummy, "bad boy, bad boy what you gona do, what you gona do when i come for you!" :D

// Shazzrah AI
class ShazzrahAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(ShazzrahAI);
    ShazzrahAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		m_arcaneexplosion = m_shazzrahcurse = m_deadenmagic = m_counterspell = m_blink = true;

        info_arcaneexplosion = dbcSpell.LookupEntry(ARCANE_EXPLOSION);
		info_shazzrahcurse = dbcSpell.LookupEntry(SHAZZRAH_CURSE);
		info_deadenmagic = dbcSpell.LookupEntry(DEADEN_MAGIC);
		info_counterspell = dbcSpell.LookupEntry(COUNTERSPELL);
		info_blink = dbcSpell.LookupEntry(BLINK);
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
	    uint32 val = RandomUInt(1000);
        SpellCast(val);
    }

    void SpellCast(uint32 val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())//_unit->getAttackTarget())
        {
			//Unit *target = _unit->GetAIInterface()->GetNextTarget();
            if(m_arcaneexplosion)
            {
                _unit->CastSpell(_unit, info_arcaneexplosion, false);
                m_arcaneexplosion = false;
                return;
            }
            
            if(m_shazzrahcurse)
            {
                _unit->CastSpell(_unit, info_shazzrahcurse, false);
                m_shazzrahcurse = false;
                return;
            }
            
            if(m_deadenmagic)
            {
                _unit->CastSpell(_unit, info_deadenmagic, false);
                m_deadenmagic = false;
                return;
            }
            
            if(m_counterspell)
            {
                _unit->CastSpell(_unit, info_counterspell, false);
                m_counterspell = false;
                return;
            }
            
            if(m_blink)
            {
                _unit->CastSpell(_unit, info_blink, false);
                m_blink = false;
                return;
            }
            

            if(val >= 100 && val <= 160)
            {
                _unit->setAttackTimer(1000, false);
                m_arcaneexplosion = true;
            }
            if(val > 160 && val <= 240)
            {
                _unit->setAttackTimer(1000, false);
                m_shazzrahcurse = true;
            }
            if(val > 240 && val <= 300)
            {
                _unit->setAttackTimer(1000, false);
                m_deadenmagic = true;
            }
            if(val > 300 && val <= 360)
            {
                _unit->setAttackTimer(1000, false);
                m_counterspell = true;
            }
            if(val > 320 && val <= 380)
            {
                _unit->setAttackTimer(1000, false);
                m_blink = true;
            }
        }
    }

protected:

    bool m_arcaneexplosion,m_shazzrahcurse,m_deadenmagic,m_counterspell,m_blink;
    SpellEntry *info_arcaneexplosion, *info_shazzrahcurse,*info_deadenmagic,*info_counterspell,*info_blink;
};



#define CN_GOLEMAGG 11988

#define GOLEMAGG_TRUST 20556 // self
#define MAGMA_SPLASH 13880 // 1 target

// Golemagg AI
class GolemaggAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(GolemaggAI);
    GolemaggAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		m_golemaggtrust = m_magmasplash = true;

        info_golemaggtrust = dbcSpell.LookupEntry(GOLEMAGG_TRUST);
		info_magmasplash = dbcSpell.LookupEntry(MAGMA_SPLASH);
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		_unit->CastSpell(_unit, info_golemaggtrust, false);
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
	    uint32 val = RandomUInt(1000);
        SpellCast(val);
    }

    void SpellCast(uint32 val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())//_unit->getAttackTarget())
        {
			Unit *target = _unit->GetAIInterface()->GetNextTarget();
           
            if(m_magmasplash)
            {
                _unit->CastSpell(target, info_magmasplash, false);
                m_magmasplash = false;
                return;
            }
            if(val >= 180 && val <= 260)
            {
                _unit->setAttackTimer(1000, false);
                m_magmasplash = true;
            }
        }
    }

protected:

    bool m_golemaggtrust,m_magmasplash;
    SpellEntry *info_golemaggtrust, *info_magmasplash;
};

#define CN_CORERAGER 11672

#define MANGLE 19820 // 1 target
// put full HP if less 50% and golemagg is still alive

// Golemagg AI
class CoreRagerAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(GolemaggAI);
    CoreRagerAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		m_mangle = true;

        info_mangle = dbcSpell.LookupEntry(MANGLE);
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
	    uint32 val = RandomUInt(1000);
        SpellCast(val);
    }

    void SpellCast(uint32 val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())//_unit->getAttackTarget())
        {
			//Unit *target = _unit->GetAIInterface()->GetNextTarget();
            if(m_mangle)
            {
                _unit->CastSpell(_unit, info_mangle, false);
                m_mangle = false;
                return;
            }

            if(val >= 100 && val <= 220)
            {
                _unit->setAttackTimer(9000, false);
                m_mangle = true;
            }
        }
    }

protected:

    bool m_mangle;
    SpellEntry *info_mangle;
};

#define CN_SULFURON_HARBRINGER 12098

#define DEMORALIZING_SHOUT 19778
#define INSPIRE 19779
#define FLAME_SPEAR 19781
// needs a aoe knockback 19780?

// Sulfuron Harbringer AI
class SulfuronAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(SulfuronAI);
    SulfuronAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		m_demoralizingshout = m_inspire = m_flamespear = true;

        info_demoralizingshout = dbcSpell.LookupEntry(DEMORALIZING_SHOUT);
		info_inspire = dbcSpell.LookupEntry(INSPIRE);
		info_flamespear = dbcSpell.LookupEntry(FLAME_SPEAR);
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
	    uint32 val = RandomUInt(1000);
        SpellCast(val);
    }

    void SpellCast(uint32 val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())//_unit->getAttackTarget())
        {
			//Unit *target = _unit->GetAIInterface()->GetNextTarget();
                      
            if(m_demoralizingshout)
            {
                _unit->CastSpell(_unit, info_demoralizingshout, false);
                m_demoralizingshout = false;
                return;
            }
            
            if(m_inspire)
            {
                _unit->CastSpell(_unit, info_inspire, false);
                m_inspire = false;
                return;
            }
            
            if(m_flamespear)
            {
                _unit->CastSpell(_unit, info_flamespear, false);
                m_flamespear = false;
                return;
            }
            
            if(val >= 100 && val <= 180)
            {
                _unit->setAttackTimer(1000, false);
                m_inspire = true;
            }
            
            if(val > 180 && val <= 260)
            {
                _unit->setAttackTimer(1000, false);
                m_demoralizingshout = true;
            }
            
            if(val > 260 && val <= 320)
            {
                _unit->setAttackTimer(1000, false);
                m_flamespear = true;
            }
        }
    }

protected:

    bool m_demoralizingshout,m_inspire,m_flamespear;
    SpellEntry *info_demoralizingshout, *info_inspire, *info_flamespear;
};



// Woot DOING RAGNAROS Tha BosS

/*    * Ragnaros Summoning talk:
          o Majordomo Executus: Behold Ragnaros, the Firelord! He who was ancient when this world was young! Bow before him, mortals! Bow before your ending!
          o Ragnaros: TOO SOON! YOU HAVE AWAKENED ME TOO SOON, EXECUTUS! WHAT IS THE MEANING OF THIS INTRUSION?
          o Majordomo Executus: These mortal infidels, my lord! They have invaded your sanctum, and seek to steal your secrets!
          o Ragnaros: FOOL! YOU ALLOWED THESE INSECTS TO RUN RAMPANT THROUGH THE HALLOWED CORE, AND NOW YOU LEAD THEM TO MY VERY LAIR? YOU HAVE FAILED ME, EXECUTUS! JUSTICE SHALL BE MET, INDEED!
          o Ragnaros: NOW FOR YOU, INSECTS. BOLDLY YOU SOUGHT THE POWER OF RAGNAROS! NOW YOU SHALL SEE IT FIRSTHAND! 
    * DIE, INSECT! (When he kills the player he has aggro on)
    * BY FIRE BE PURGED! (Ranged knockback)
    * TASTE THE FLAMES OF SULFURON! (Melee knockback)
    * COME FORTH, MY SERVANTS! DEFEND YOUR MASTER! (Summoning Sons of Flame) */

#define CN_RAGNAROS 11502

#define ELEMENTAL_FIRE 20563 // 1 target
#define MAGMA_BLAST 20565 // various targets on not attacked. -> sound -> 8048 ?
#define WRATH_OF_RAGNAROS 20566  // Fly bitches fly! quote -> "Taste the Flames of Sulfuron!" -> sound 8047
#define HAMMER_OF_RAGNAROS 19780 // quote -> "By fire be purged!" -> sound 8046
#define MELT_WEAPON 21387 // 1 target
#define SUMMON_SONS_OF_FLAMES 21108 // TODO: DUMMY :P summon the sons of flames. entry = 12143 -> sound 8049,8050


//Ragnaros Submerge Visual -> 20567
//Ragnaros Emerge -> 20568

// Ragnaros AI
class RagnarosAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(RagnarosAI);
    RagnarosAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		m_elementalfire = m_wrath = m_hammer = m_meltweapon = m_summonsons = true;

        info_elementalfire = dbcSpell.LookupEntry(ELEMENTAL_FIRE);
		info_wrath = dbcSpell.LookupEntry(WRATH_OF_RAGNAROS);
		info_hammer = dbcSpell.LookupEntry(HAMMER_OF_RAGNAROS);
		info_meltweapon = dbcSpell.LookupEntry(MELT_WEAPON);
		info_summonsons = dbcSpell.LookupEntry(SUMMON_SONS_OF_FLAMES);
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		_unit->GetAIInterface()->skip_reset_hp=true;
    }

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
       RemoveAIUpdateEvent();
    }
    
    void OnTargetDied(Unit* mTarget)
    {
        _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "DIE, INSECT!");
        _unit->PlaySoundToSet(8051);
    }

    void OnDied(Unit * mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {	
	    uint32 val = RandomUInt(1000);
        SpellCast(val);
    }

    void SpellCast(uint32 val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())//_unit->getAttackTarget())
        {
			Unit *target = _unit->GetAIInterface()->GetNextTarget();
			                      
            if(m_elementalfire)
            {
                _unit->CastSpell(target, info_elementalfire, false);
                m_elementalfire = false;
                return;
            }
            
            if(m_wrath)
            {
                _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Taste the Flames of Sulfuron!");
		        _unit->PlaySoundToSet(8047);
                _unit->CastSpell(_unit, info_wrath, false);
                m_wrath = false;
                return;
            }
            
            if(m_hammer)
            {
                _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "By fire be purged!");
		        _unit->PlaySoundToSet(8046);
                _unit->CastSpell(_unit, info_hammer, false);
                m_hammer = false;
                return;
            }
            
            if(m_meltweapon)
            {
                _unit->CastSpell(target, info_meltweapon, false);
                m_meltweapon = false;
                return;
            }
            
            if(val >= 100 && val <= 160)
            {
                _unit->setAttackTimer(1000, false);
                m_elementalfire = true;
            }
            
            if(val > 160 && val <= 220)
            {
                _unit->setAttackTimer(1000, false);
                m_wrath = true;
            }
            
            if(val > 220 && val <= 280)
            {
                _unit->setAttackTimer(1000, false);
                m_hammer = true;
            }
            if(val > 340 && val <= 400)
            {
                _unit->setAttackTimer(1000, false);
                m_meltweapon = true;
            }
            
        }
    }

protected:

    bool m_elementalfire,m_wrath,m_hammer,m_meltweapon,m_summonsons;
    SpellEntry *info_elementalfire, *info_wrath, *info_hammer, *info_meltweapon,*info_summonsons;
};

void SetupMoltenCore(ScriptMgr * mgr)
{
    mgr->register_creature_script(CN_FIREWALKER, &FirewalkerAI::Create);
    mgr->register_creature_script(CN_FLAMEGUARD, &FlameguardAI::Create);
    mgr->register_creature_script(CN_LAVAREAVER, &LavaReaverAI::Create);
    mgr->register_creature_script(CN_LAVAELEMENTAL, &LavaelementalAI::Create);
    mgr->register_creature_script(CN_FLAMEIMP, &FlameImpAI::Create);
    mgr->register_creature_script(CN_LAVASURGER, &LavaSurgerAI::Create);
    mgr->register_creature_script(CN_FIRELORD, &FireLordAI::Create);
    mgr->register_creature_script(CN_MOLTENDESTROYER, &MoltenDestroyerAI::Create);
    mgr->register_creature_script(CN_MOLTENGIANT, &MoltenGiantAI::Create);
    mgr->register_creature_script(CN_LUCIFRON, &LucifronAI::Create);
    mgr->register_creature_script(CN_MAGMADAR, &MagmadarAI::Create);
    mgr->register_creature_script(CN_GEHENNAS, &GehennasAI::Create);
    mgr->register_creature_script(CN_FLAMEWAKER, &FlamewakerAI::Create);
    mgr->register_creature_script(CN_GARR, &GarrAI::Create);
    mgr->register_creature_script(CN_FIRESWORN, &FireSwornAI::Create);
    mgr->register_creature_script(CN_BARON_GEDDON, &BaronGeddonAI::Create);
    mgr->register_creature_script(CN_SHAZZRAH, &ShazzrahAI::Create);
    mgr->register_creature_script(CN_GOLEMAGG, &GolemaggAI::Create);
    mgr->register_creature_script(CN_CORERAGER, &CoreRagerAI::Create);
    mgr->register_creature_script(CN_SULFURON_HARBRINGER, &SulfuronAI::Create);
    mgr->register_creature_script(CN_RAGNAROS, &RagnarosAI::Create);
}
