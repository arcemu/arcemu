#include "StdAfx.h"
#include "Setup.h"

#ifdef WIN32
#pragma warning(disable:4305)		// warning C4305: 'argument' : truncation from 'double' to 'float'
#endif

/************************************************************************/
/* Raid_OnyxiasLair.cpp Script by DK                                    */
/************************************************************************/

/* 
This script covers Onyxia's mind
*/

//Creature Name
#define CN_ONYXIA 10184
#define CN_ONYXIAN_WHELP 11262

#define WALK 0
#define RUN 256
#define FLY 768
//0 = walk, 256 = run, 768 = fly 

//Phase 1,3 Spells
#define FLAME_BREATH 23461 //self
#define KNOCK_AWAY 10101 //Reduce thread script effect main target
#define WING_BUFFET 18500 // self
#define CLEAVE 30495//15579,16044,19642,29832 //target
#define TAIL_SWEEP 15847

//Phase 2 Spells
#define SCRIPTABLE_FIREBALL 30691//Not sure find true one
//Script it
#define ENTANGLING_FLAMES 20019
//Onyxia's Breath (Deep Breath)
#define DEEP_BREATH 17086

//Phase 3 Spells
#define AOE_FEAR 18431//With Activate Object

struct Coords
{
    float x;
    float y;
    float z;
    float o;
};

static Coords coords[] =
{
    { 0, 0, 0, 0 },
    { -75.945, -219.245, -83.375, 0.004947 },
    { -72.945, -219.245, -80.779, 0.004947 },
    { 42.621, -217.195, -66.056, 3.014011 },
    { 12.270, -254.694, -67.997, 2.395585 },
    { -79.020, -252.374, -68.965, 0.885179 },
    { -80.257, -174.240, -69.293, 5.695741 },
    { 27.875, -178.547, -66.041, 3.908957 },
    { -4.868, -217.171, -86.710, 3.141590 }
};

static Coords whelpCoords[] =
{
    { -30.812, -166.395, -89.000, 5.160 },
    { -30.233, -264.158, -89.896, 1.129 },
    { -35.813, -169.427, -90.000, 5.384 },
    { -36.104, -260.961, -90.600, 1.111 },
    { -34.643, -164.080, -90.000, 5.364 },
    { -35.377, -267.320, -91.000, 1.111 }
};

class OnyxiaAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(OnyxiaAI);
    OnyxiaAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
        m_phase = 1;
        m_entry = pCreature->GetEntry();
        m_useSpell = true;
        m_eFlamesCooldown = 1;
        m_whelpCooldown = 7;
        m_aoeFearCooldown = 30;
        m_fCastCount = 5;
		m_breathCooldown=0;
        _unit->GetAIInterface()->setMoveType(4);
        _unit->GetAIInterface()->addWayPoint(CreateWaypoint(1, 2000, RUN));
        _unit->GetAIInterface()->addWayPoint(CreateWaypoint(2, 0, FLY));
        _unit->GetAIInterface()->addWayPoint(CreateWaypoint(3, 0, FLY));
        _unit->GetAIInterface()->addWayPoint(CreateWaypoint(4, 0, FLY));
        _unit->GetAIInterface()->addWayPoint(CreateWaypoint(5, 0, FLY));
        _unit->GetAIInterface()->addWayPoint(CreateWaypoint(6, 0, FLY));
        _unit->GetAIInterface()->addWayPoint(CreateWaypoint(7, 0, FLY));
        _unit->GetAIInterface()->addWayPoint(CreateWaypoint(8, 0, FLY));

        infoFear = dbcSpell.LookupEntry(AOE_FEAR);
        infoCleave = dbcSpell.LookupEntry(CLEAVE);
        infoFBreath = dbcSpell.LookupEntry(FLAME_BREATH);
        infoKAway = dbcSpell.LookupEntry(KNOCK_AWAY);
        infoSFireball = dbcSpell.LookupEntry(SCRIPTABLE_FIREBALL);
        infoWBuffet = dbcSpell.LookupEntry(WING_BUFFET);
        infoDeepBreath = dbcSpell.LookupEntry(DEEP_BREATH);

        if(!infoFear || !infoCleave || !infoFBreath
            || !infoKAway || !infoSFireball || !infoWBuffet || !infoDeepBreath)
            m_useSpell = false;

        _unit->GetAIInterface()->setOutOfCombatRange(200000);

        m_fBreath = false;
        m_kAway = false;
        m_wBuffet = false;
        m_Cleave = false;
    }
    
    void OnCombatStart(Unit* mTarget)
    {
        m_phase = 1;
        m_eFlamesCooldown = 1;
        m_whelpCooldown = 7;
        _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
        _unit->SetStandState(0);
        _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "How fortuitous, usually I must leave my lair to feed!");
        if(m_useSpell)
            RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));

        m_fBreath = false;
        m_kAway = false;
        m_wBuffet = false;
        m_Cleave = false;
		_unit->GetAIInterface()->skip_reset_hp=true;
    }

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setMoveType(0);
        _unit->GetAIInterface()->setWaypointToMove(0);
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        _unit->GetAIInterface()->SetAllowedToEnterCombat(true);
        _unit->GetAIInterface()->m_moveFly = false;
        _unit->GetAIInterface()->m_canMove = true;
        _unit->SetStandState(STANDSTATE_SLEEP);
        /*if(_unit->m_pacified > 0)
            _unit->m_pacified--;*/
        if(m_useSpell)
            RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
        if(m_useSpell)
            RemoveAIUpdateEvent();

        m_phase = 1;
        m_eFlamesCooldown = 1;
        m_whelpCooldown = 7;
        
        /*Add Loot?
        mKiller->
        */
    }

    void OnReachWP(uint32 iWaypointId, bool bForwards)
    {
        switch(iWaypointId)
        {
        case 1:
            {
                _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
                _unit->GetAIInterface()->setWaypointToMove(2);
                Fly();
            }break;
        case 2:
            {
                _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
                _unit->GetAIInterface()->setWaypointToMove(3);
            }break;
        case 3:
            {
                _unit->GetAIInterface()->m_canMove = false;
                _unit->GetAIInterface()->SetAllowedToEnterCombat(true);
                _unit->GetAIInterface()->setCurrentAgent(AGENT_SPELL);
                //_unit->m_pacified--;
                _unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
                _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
                _unit->GetAIInterface()->setWaypointToMove(0);
                WorldPacket data(SMSG_MOVE_SET_HOVER, 13);
                data << _unit->GetNewGUID();
                data << uint32(0);
                _unit->SendMessageToSet(&data, false);
                m_currentWP = 3;
            }break;
        case 8:
            {
                _unit->GetAIInterface()->SetAllowedToEnterCombat(true);
                _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
                _unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
                _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
                _unit->GetAIInterface()->setWaypointToMove(0);
                /*_unit->m_pacified--;
                if(_unit->m_pacified > 0)
                    _unit->m_pacified--;*/
                WorldPacket data(SMSG_MOVE_UNSET_HOVER, 13);
                data << _unit->GetNewGUID();
                data << uint32(0);
                _unit->SendMessageToSet(&data, false);
                Land();
            }break;
        default:
            {
                _unit->GetAIInterface()->m_canMove = false;
                _unit->GetAIInterface()->SetAllowedToEnterCombat(true);
                _unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
                _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
                _unit->GetAIInterface()->setWaypointToMove(0);
                WorldPacket data(SMSG_MOVE_SET_HOVER, 13);
                data << _unit->GetNewGUID();
                data << uint32(0);
                _unit->SendMessageToSet(&data, false);
                //_unit->m_pacified--;
            }break;
        };
    }

    void AIUpdate()
    {
        switch(m_phase)
        {
        case 1:
            {
                PhaseOne();
            }break;
        case 2:
            {
                PhaseTwo();
            }break;
        case 3:
            {
                PhaseThree();
            }break;
        default:
            {
                m_phase = 1;
            };
        };
    }

    void PhaseOne()
    {
        if(_unit->GetHealthPct() <= 65)
        {
            m_phase = 2;
            _unit->SetFloatValue(UNIT_MOD_CAST_SPEED, 0.01f);
            if(_unit->GetCurrentSpell() != NULL)
                _unit->GetCurrentSpell()->cancel();

            _unit->GetAIInterface()->SetAllowedToEnterCombat(false);
            //_unit->m_pacified++;            
            _unit->GetAIInterface()->StopMovement(0);
            _unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
            _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
            _unit->GetAIInterface()->setWaypointToMove(1);
            
            return;
        }
        uint32 val = RandomUInt(1000);
        SpellCast(val);
    }

    void PhaseTwo()
    {
        if(_unit->GetHealthPct() <= 40)
        {
            m_phase = 3;
            _unit->SetFloatValue(UNIT_MOD_CAST_SPEED, 1.0f);
            if(_unit->GetCurrentSpell() != NULL)
                _unit->GetCurrentSpell()->cancel();
            _unit->GetAIInterface()->m_canMove = true;
            _unit->GetAIInterface()->SetAllowedToEnterCombat(false);
            //_unit->m_pacified++;
            _unit->GetAIInterface()->StopMovement(0);
            _unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
            _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
            _unit->GetAIInterface()->setWaypointToMove(8);

            return;
        }
        if(_unit->GetAIInterface()->getMoveType() == MOVEMENTTYPE_WANTEDWP)
            return;
        m_eFlamesCooldown--;
        if(!m_eFlamesCooldown && _unit->GetAIInterface()->GetNextTarget())//_unit->getAttackTarget())
        {
            _unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), infoSFireball, false);//(_unit->getAttackTarget(),
            m_eFlamesCooldown = 4;
            m_fCastCount--;
        }
        if(!m_fCastCount)
        {
            uint32 val = RandomUInt(1250);
            if(val < 250)//Move left
            {
                m_currentWP++;
                if(m_currentWP >= 8)
                    m_currentWP = 3;

                _unit->GetAIInterface()->m_canMove = true;
                _unit->GetAIInterface()->SetAllowedToEnterCombat(false);
                //_unit->m_pacified++;
                _unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
                _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
                _unit->GetAIInterface()->setWaypointToMove(m_currentWP);
                m_fCastCount = 5;
            }
            else if(val > 1000)//Move right
            {
                m_currentWP--;
                if(m_currentWP < 3)
                    m_currentWP = 7;

                _unit->GetAIInterface()->m_canMove = true;
                _unit->GetAIInterface()->SetAllowedToEnterCombat(false);
                //_unit->m_pacified++;
                _unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
                _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);                
                _unit->GetAIInterface()->setWaypointToMove(m_currentWP);
                m_fCastCount = 5;
            }
            else if(val < 350)
            {
                //Deep breath
                _unit->CastSpell(_unit, infoDeepBreath, false);
                m_fCastCount = 5;
            }
            else
                m_fCastCount = 5;
        }
        m_whelpCooldown--;
        if(!m_whelpCooldown)
        {
            Creature *cre = NULL;
            for(int i = 0; i < 6; i++)
            {
                cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ONYXIAN_WHELP, 
                    whelpCoords[i].x, whelpCoords[i].y,
                    whelpCoords[i].z, whelpCoords[i].o,
                    true, false, _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), 50);
                if(cre)
                {
                    cre->GetAIInterface()->MoveTo(14.161, -177.874, -85.649, 0.23);
                    cre->GetAIInterface()->setOutOfCombatRange(100000);
                }
                cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ONYXIAN_WHELP, 
                    whelpCoords[5-i].x, whelpCoords[5-i].y,
                    whelpCoords[5-i].z, whelpCoords[5-i].o,
                    true, false, _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), 50);
                if(cre)
                {
                    cre->GetAIInterface()->MoveTo(27.133, -232.030, -84.188, 0.44);
                    cre->GetAIInterface()->setOutOfCombatRange(100000);
                }
            }
            m_whelpCooldown = 30;
        }
    }

    void PhaseThree()
    {
        if(!m_aoeFearCooldown)
        {
            _unit->CastSpell(_unit, infoFear, false);//(_unit->getAttackTarget(),
            m_aoeFearCooldown = 30;
            return;
        }
        uint32 val = RandomUInt(1000);
        SpellCast(val);
        m_whelpCooldown--;
        m_aoeFearCooldown--;
        if(!m_whelpCooldown)
        {
            Creature *cre = NULL;
            for(int i = 0; i < 6; i++)
            {
                cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ONYXIAN_WHELP, 
                    whelpCoords[i].x, whelpCoords[i].y,
                    whelpCoords[i].z, whelpCoords[i].o,
                    true, false, _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), 50);
                if(cre)
                {
                    cre->GetAIInterface()->MoveTo(14.161, -177.874, -85.649, 0.23);
                    cre->GetAIInterface()->setOutOfCombatRange(100000);
                }
                cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ONYXIAN_WHELP, 
                    whelpCoords[5-i].x, whelpCoords[5-i].y,
                    whelpCoords[5-i].z, whelpCoords[5-i].o,
                    true, false, _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), 50);
                if(cre)
                {
                    cre->GetAIInterface()->MoveTo(27.133, -232.030, -84.188, 0.44);
                    cre->GetAIInterface()->setOutOfCombatRange(100000);
                }
            }
            m_whelpCooldown = 300;
        }
    }

    inline WayPoint* CreateWaypoint(int id, uint32 waittime, uint32 flags)
    {
        //WayPoint* wp = new WayPoint;
        //WayPoint * wp = _unit->GetMapMgr()->GetInterface()->CreateWaypoint();
        //WayPoint * wp = sStructFactory.CreateWaypoint();
        WayPoint * wp = _unit->CreateWaypointStruct();
        wp->id = id;
        wp->x = coords[id].x;
        wp->y = coords[id].y;
        wp->z = coords[id].z;
        wp->o = coords[id].o;
        wp->waittime = waittime;
        wp->flags = flags;
        wp->forwardemoteoneshot = 0;
        wp->forwardemoteid = 0;
        wp->backwardemoteoneshot = 0;
        wp->backwardemoteid = 0;
        wp->forwardskinid = 0;
        wp->backwardskinid = 0;
        return wp;
    }

    void Fly()
    {
        _unit->Emote(EMOTE_ONESHOT_LIFTOFF);
        //Do we need hover really? Check it :D
        /*WorldPacket data(SMSG_MOVE_SET_HOVER, 13);
        data << _unit->GetNewGUID();
        data << uint32(0);
        _unit->SendMessageToSet(&data, false);*/
        _unit->GetAIInterface()->m_moveFly = true;
    }

    void Land()
    {
        _unit->Emote(EMOTE_ONESHOT_LAND);
        //Do we need hover really? Check it :D
        /*WorldPacket data(SMSG_MOVE_UNSET_HOVER, 13);
        data << _unit->GetNewGUID();
        data << uint32(0);
        _unit->SendMessageToSet(&data, false);*/
        _unit->GetAIInterface()->m_moveFly = false;
    }

    void SpellCast(uint32 val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())//_unit->getAttackTarget())
        {
            if(m_fBreath)
            {
				uint32 t = (uint32)time(NULL);
				if(t>m_breathCooldown)
				{
					_unit->CastSpell(_unit, infoFBreath, false);
					m_breathCooldown = t+30;
				}

                m_fBreath = false;
                return;
            }
            else if(m_kAway)
            {
                _unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), infoKAway, false);
                m_kAway = false;
                return;
            }
            else if(m_wBuffet)
            {
                _unit->CastSpell(_unit, infoWBuffet, false);
                m_wBuffet = false;
                return;
            }
            else if(m_Cleave)
            {
                _unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), infoCleave, false);
                m_Cleave = false;
                return;
            }

            if(val >= 100 && val <= 225)
            {
                _unit->setAttackTimer(6000, false);//6000
                m_fBreath = true;
                //_unit->CastSpell(_unit, infoFBreath, false);
            }
            else if(val > 225 && val <= 300)
            {
                _unit->setAttackTimer(4000, false);//2000
                m_kAway = true;
                //_unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), infoKAway, false);
            }
            else if(val > 300 && val <= 375)
            {
                _unit->setAttackTimer(4000, false);//3000
                m_wBuffet = true;
                //_unit->CastSpell(_unit, infoWBuffet, false);
            }
            else if(val > 375 && val < 450)
            {
                _unit->setAttackTimer(4000, false);//2000
                m_Cleave = true;
               // _unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), infoCleave, false);
            }
        }
    }

    inline bool HasEntry() { return (m_entry != 0) ? true : false; }

protected:

    bool m_fBreath;
    bool m_kAway;
    bool m_wBuffet;
    bool m_Cleave;
    uint32 m_entry;
    uint32 m_phase;
    bool m_useSpell;
    uint32 m_eFlamesCooldown;
    uint32 m_whelpCooldown;
    uint32 m_aoeFearCooldown;
    uint32 m_fCastCount;
    uint32 m_currentWP;
	uint32 m_breathCooldown;
    SpellEntry *infoFear, *infoWBuffet, *infoCleave, *infoFBreath, *infoKAway, *infoSFireball, *infoDeepBreath;
};

void SetupOnyxiasLair(ScriptMgr * mgr)
{
    // Onyxia
    mgr->register_creature_script(CN_ONYXIA, &OnyxiaAI::Create);
}
