#include "StdAfx.h"
#include "Setup.h"

/************************************************************************/
/* Instance_BlackTemple.cpp Script		                                */
/************************************************************************/

/*****************************/
/*                           */
/*         Boss AIs          */
/*                           */
/*****************************/
/*
// SupremusAI

#define CN_SUPREMUS 22898	

#define 

class SupremusAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(SupremusAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    SupremusAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 5;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = sSpellStore.LookupEntry();
		spells[0].targettype = TARGET_;
		spells[0].instant = false;
		spells[0].cooldown = -1;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Bear witness to the agent of your demise!");	// used when he kills Warden Mellichar
		_unit->PlaySoundToSet(11123);
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void OnTargetDied(Unit* mTarget)
    {
		if (_unit->GetHealthPct() > 0)
		{
			int RandomSpeach;
			RandomSpeach=rand()%2;
			switch (RandomSpeach)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Your fate is written!");	// this one needs verification
				_unit->PlaySoundToSet(11124);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The chaos I have sown here is but a taste....");
				_unit->PlaySoundToSet(11125);
				break;
			}
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
       RemoveAIUpdateEvent();
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I am merely one of... infinite multitudes.");
		_unit->PlaySoundToSet(11126);
    }

    void AIUpdate()
	{
		if (_unit->GetHealthPct() <= 66 && !IllusionCount)
		{
			IllusionCount = 1;
			_unit->CastSpell(_unit, spells[3].info, spells[3].instant);
			//_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "We span the universe, as countless as the stars!");
			_unit->PlaySoundToSet(11131);	// Idk if those texts shouldn't be told by clones and by org. so disabled MSG to make it harder to detect =P
		}

		if (_unit->GetHealthPct() <= 33 && IllusionCount == 1)
		{
			IllusionCount = 2;
			_unit->CastSpell(_unit, spells[4].info, spells[4].instant);
			//_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "We span the universe, as countless as the stars!");
			_unit->PlaySoundToSet(11131);
		}
		
		else
		{
			float val = sRand.rand(100.0f);
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

	uint32 m_phase;
	int nrspells;
};
*/

/**********************************/
/*                                */
/*       Illidan and stuff        */
/*           related to him.      */
/*                                */
/**********************************/


// Additional stuff
uint32 m_phase;
uint32 AKAMA_DIALOG;
uint32 FoADeaths;
uint32 MAIEV_DIALOG2;

#define WALK 0
#define RUN 256
#define FLY 768

// Flame of AzzinothAI

#define CN_FLAME_OF_AZZINOTH 22997	

class FlameOfAzzinothAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(FlameOfAzzinothAI);
	//SP_AI_Spell spells[0];	// Not used yet so disabled to prevent illegal zero array error (disabled all spell cast related functions)
	//bool m_spellcheck[0];

    FlameOfAzzinothAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
/*
		nrspells = 0;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = sSpellStore.LookupEntry(BOMB);
		spells[0].targettype = TARGET_DESTINATION;
		spells[0].instant = false;
		spells[0].cooldown = 10;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;
*/
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		//CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }
/*
	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}
*/
	void OnTargetDied(Unit* mTarget)
    {
    }

    void OnCombatStop(Unit *mTarget)
    {
		//CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		//CastTime();
       RemoveAIUpdateEvent();
	   FoADeaths--;
    }

    void AIUpdate()
	{
		/*float val = sRand.rand(100.0f);
		SpellCast(val);*/
    }
/*
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
*/
protected:

	int nrspells;
};

// AkamaAI

#define CN_AKAMA 22990 //21700	// Should be: 22990
// faction 1858 (All Ashtongue Deathsworn factions in DBC: 1820, 1858, 1866)

#define HEALING_POTION 40535
#define BLESSING_OF_KINGS 20217

struct Coords
{
    float x;
    float y;
    float z;
    float o;
};

static Coords toillidan[] = 
{
	{ 0, 0, 0, 0 },
	{ 755.243591, 304.847565, 312.156586, 0.024107 },
	{ 771.880676, 305.175659, 313.610596, 0.002112 },
	{ 779.961548, 305.039795, 319.706055, 6.266372 },
	{ 793.287415, 290.741547, 319.774963, 5.280694 },
    { 792.858276, 278.565430, 328.446716, 4.679865 },
	{ 792.382019, 261.826965, 341.463715, 4.687721 },
	{ 782.984314, 249.260162, 341.709167, 4.055477 },
	{ 773.819031, 245.568298, 347.780304, 3.509625 },
	{ 764.204895, 241.937164, 353.667877, 3.454647 },
	{ 742.981079, 252.640411, 352.996094, 1.444028 },
	{ 753.753479, 304.675110, 352.997223, 3.092202 },
	{ 751.941528, 304.626221, 352.996124, 3.128243 }
	//{ 753.107300, 304.686584, 352.996094, 3.101546 }
};

class AkamaAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(AkamaAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    AkamaAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		m_entry = pCreature->GetEntry();

		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(1, 8000, WALK));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(2, 0, WALK));
        _unit->GetAIInterface()->addWayPoint(CreateWaypoint(3, 0, WALK));
        _unit->GetAIInterface()->addWayPoint(CreateWaypoint(4, 0, WALK));
        _unit->GetAIInterface()->addWayPoint(CreateWaypoint(5, 0, WALK));
        _unit->GetAIInterface()->addWayPoint(CreateWaypoint(6, 0, WALK));
        _unit->GetAIInterface()->addWayPoint(CreateWaypoint(7, 0, WALK));
        _unit->GetAIInterface()->addWayPoint(CreateWaypoint(8, 0, WALK));
        _unit->GetAIInterface()->addWayPoint(CreateWaypoint(9, 0, WALK));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(10, 0, WALK));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(11, 0, WALK));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(12, 0, WALK));
/*							// don't know why, but when spells are initialized server crashes =O
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
*/
        spells[0].info = sSpellStore.LookupEntry(HEALING_POTION);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = true;
		spells[0].cooldown = -1;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = sSpellStore.LookupEntry(BLESSING_OF_KINGS);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = true;
		spells[1].cooldown = -1;
		spells[1].perctrigger = 15.0f;
		spells[1].attackstoptimer = 1000;

		WAY_START = 0;
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, U_FIELD_FLAG_UNIT_UNTACKABLE_SELECT);
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		//CastTime();
		//RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }
/*
	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}
*/
    void OnCombatStop(Unit *mTarget)
    {
		if (m_phase > 0)
			_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        //RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
       //RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		if (m_phase == 0 && !WAY_START)
		{
			_unit->GetAIInterface()->StopMovement(0);
			_unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
			_unit->GetAIInterface()->setWaypointToMove(1);
			WAY_START = 1;
		}
/*
		if (_unit->GetHealthPct() < 15)
			_unit->CastSpell(_unit, spells[0].info, spells[0].instant);
*/
		if (AKAMA_DIALOG)
		{
			switch(AKAMA_DIALOG)
			{
			case 8:
			    {
			        _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "We've come to end your reign, Illidan. My people and all of Outland shall be free!");
					_unit->PlaySoundToSet(11389);
					_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 378);
			    }break;
			case 13:
				{
					_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
				}break;
			case 17:	// (16.5) must check voiding time changing possibility!
			    {
			        _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The time has come! The moment is at hand!");
					_unit->PlaySoundToSet(11380);
					_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 378);
			    }break;
			case 19:	// changed from 18, because I don't have weapon model ids yet to add them to function
				{
					_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
					_unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
					_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
					_unit->GetAIInterface()->m_canMove = true;
					AKAMA_DIALOG = 1;
					//_unit->GetAIInterface()->m_moveRun = true;
					//_unit->GetAIInterface()->MoveTo(714.594788, 305.476044, 353.309204, 3.170653);	// it's just hack to prvent Akama to stand in place (as I have no idea now how to target and attack Illidan =S)
					//Unit * target = _unit->GetMapMgr()->GetInterface()->GetObjectNearestCoords<Creature, TYPEID_UNIT>(22917, 704.539001f, 305.282013f, 353.919006f);	// no idea if this works as I don't have right Akama (hostile with Illidan)
					//Unit * target = _unit->GetMapMgr()->GetCreature(355093);
					//_unit->setAttackTarget(target);
					
				}break;
			};
		}
/*
		if (m_phase)
		{
			float val = sRand.rand(100.0f);
			SpellCast(val);
		}*/
    }
/*
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
					if (i == 1 && !(_unit->HasActiveAura(20217)) *//*_unit->!HasAura(137)*/ //)	// added separated case to not cast spell when aura effect is still on target
					/*{
						_unit->setAttackTimer(spells[i].attackstoptimer, false);
						m_spellcheck[i] = true;
					}

					else
					{
						_unit->setAttackTimer(spells[i].attackstoptimer, false);
						m_spellcheck[i] = true;
					}
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}*/

    void OnReachWP(uint32 iWaypointId, bool bForwards)
    {
		switch(iWaypointId)
		{
		case 1:
			{
				_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
				_unit->GetAIInterface()->setWaypointToMove(2);
			}break;
		case 2:
			{
				_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
				_unit->GetAIInterface()->setWaypointToMove(3);
			}break;
		case 3:
		    {
		        _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
		        _unit->GetAIInterface()->setWaypointToMove(4);
		    }break;
		case 4:
		    {
		        _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
		        _unit->GetAIInterface()->setWaypointToMove(5);
		    }break;
		case 5:
		    {
		        _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
		        _unit->GetAIInterface()->setWaypointToMove(6);
		    }break;
		case 6:
		    {
		        _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
		        _unit->GetAIInterface()->setWaypointToMove(7);
		    }break;
		case 7:
		    {
		        _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
		        _unit->GetAIInterface()->setWaypointToMove(8);
		    }break;
		case 8:
			{
		        _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
		        _unit->GetAIInterface()->setWaypointToMove(9);
		    }break;
		case 9:
		    {
		        _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
		        _unit->GetAIInterface()->setWaypointToMove(10);
		    }break;
		case 10:
		    {
				_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
		        _unit->GetAIInterface()->setWaypointToMove(11);
			}break;
		case 11:
		    {
				_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
		        _unit->GetAIInterface()->setWaypointToMove(12);
			}break;
		case 12:
			{
				AKAMA_DIALOG = 1;
				//_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
				//_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);	// one of those messes Akama =S
				_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
				_unit->GetAIInterface()->setWaypointToMove(0);
			}break;
		}
    }

    inline WayPoint* CreateWaypoint(int id, uint32 waittime, uint32 flags)
    {
        WayPoint * wp = _unit->CreateWaypointStruct();
        wp->id = id;
        wp->x = toillidan[id].x;
        wp->y = toillidan[id].y;
        wp->z = toillidan[id].z;
        wp->o = toillidan[id].o;
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

protected:

	uint32 WAY_START;
    uint32 m_entry;
    uint32 m_currentWP;
	int nrspells;
};

// Illidan StormrageAI

#define CN_ILLIDAN_STORMRAGE 22917 //22083 // should be 22917
// Faction: 1825 (All Demon factions in DBC: 90, 954, 1768, 1769, 1786, 1825)

// Normal form spells
#define SHEAR 41032				// +
#define DRAW_SOUL 40904			// +
#define FLAME_CRASH 40832		// +
#define MORTAL_STRIKE 37335		// +
#define ENRAGE 34971			// +	<-- Phase 5 only!

// Demon form spells
#define SHADOW_BLAST 41078	// +
#define SUMMON_DEMON 41117	// lack of core support
// One more spell with throwing balls in radius?

// Other spells
#define SHADOW_PRISON 40647

// Missing Spells (how is it possible with so much spell added already? :O
// All spells from Phase 2 without Summon Tear of Azzinoth 39855

// Phase 2 spells
#define FIREBALL 40598	// I took it as id was close to what we hae here and it has blue icon (Illidan used spells with blue fire =P)

// TRANSFORM 40115
// DEMON TRANSFORM 40511, 40398, 40510
// Others Maiev/Akama
// DEMON FORM 40506
// Models: Illidan 21135, 21526, 21137; Dark Illidan 21322
#define SKULL_INTRO 39656
#define PERM_ILLUSION_ILLIDAN 39704

// Test Demon				// SHOULD BE EMOTES! :D
#define DEMON_FORM1 40511
#define DEMON_FORM2 40398
#define DEMON_FORM3 40510

// TEAR OF AZZINOTH as summon spell for phase 2 ? YES IT IS! Summon Tear of Azzinoth 39855 summons flame of azzinoth 22997

// NOTES: CHECK FLAMES' TEXT AS HE CAN BE HEARD DURING TRANSFORMATIONS SOMETIMES;
//		  CHECK BLASTS AS THEY ARE USED LITTLE TOO EARLY BEFORE FULL TRANSED;

// OTHERS: Summon Parasitic Shadowfiends 41915; Parasitic Shadowfiend 41914 [41913]

class IllidanStormrageAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(IllidanStormrageAI);
	SP_AI_Spell spells[11];
	bool m_spellcheck[11];

    IllidanStormrageAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 8;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
        spells[0].info = sSpellStore.LookupEntry(SHEAR);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].cooldown = -1;
		spells[0].perctrigger = 12.0f;
		spells[0].attackstoptimer = 1000;

        spells[1].info = sSpellStore.LookupEntry(DRAW_SOUL);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = false;
		spells[1].cooldown = -1;
		spells[1].perctrigger = 6.0f;
		spells[1].attackstoptimer = 1000;

        spells[2].info = sSpellStore.LookupEntry(FLAME_CRASH);
		spells[2].targettype = TARGET_DESTINATION;
		spells[2].instant = false;
		spells[2].cooldown = -1;
		spells[2].perctrigger = 12.0f;
		spells[2].attackstoptimer = 1000;
		spells[2].speech = "Behold the flames of Azzinoth!";	// ?
		spells[2].soundid = 11480;

        spells[3].info = sSpellStore.LookupEntry(MORTAL_STRIKE);
		spells[3].targettype = TARGET_ATTACKING;
		spells[3].instant = true;
		spells[3].cooldown = -1;
		spells[3].perctrigger = 8.0f;
		spells[3].attackstoptimer = 1000;

        spells[4].info = sSpellStore.LookupEntry(ENRAGE);
		spells[4].targettype = TARGET_SELF;
		spells[4].instant = true;
		spells[4].cooldown = -1;
		spells[4].perctrigger = 0.0f;
		spells[4].attackstoptimer = 1000;
		spells[4].speech = "You've wasted too much time mortals, now you shall fall!";
		spells[4].soundid = 11474;

        spells[5].info = sSpellStore.LookupEntry(SHADOW_BLAST);
		spells[5].targettype = TARGET_DESTINATION;
		spells[5].instant = false;
		spells[5].cooldown = -1;
		spells[5].perctrigger = 0.0f;
		spells[5].attackstoptimer = 1000;

        spells[6].info = sSpellStore.LookupEntry(SUMMON_DEMON);
		spells[6].targettype = TARGET_SELF;	// ?
		spells[6].instant = false;
		spells[6].cooldown = -1;
		spells[6].perctrigger = 0.0f;
		spells[6].attackstoptimer = 1000;

        spells[7].info = sSpellStore.LookupEntry(SHADOW_PRISON);
		spells[7].targettype = TARGET_VARIOUS;
		spells[7].instant = true;
		spells[7].cooldown = -1;
		spells[7].perctrigger = 0.0f;
		spells[7].attackstoptimer = 1000;

		spells[8].info = sSpellStore.LookupEntry(DEMON_FORM1);
		spells[8].instant = true;
		spells[8].cooldown = -1;
		spells[8].perctrigger = 0.0f;
		spells[8].attackstoptimer = 1000;

		spells[9].info = sSpellStore.LookupEntry(DEMON_FORM2);
		spells[9].instant = true;
		spells[9].cooldown = -1;
		spells[9].perctrigger = 0.0f;

		spells[10].info = sSpellStore.LookupEntry(DEMON_FORM3);
		spells[10].instant = true;
		spells[10].cooldown = -1;
		spells[10].perctrigger = 0.0f;

		spells[11].info = sSpellStore.LookupEntry(FIREBALL);
		spells[11].instant = false;
		spells[11].cooldown = -1;
		spells[11].perctrigger = 0.0f;

		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, U_FIELD_FLAG_UNIT_UNTACKABLE_SELECT);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		_unit->GetAIInterface()->m_canMove = false;
		DemonPhaseTimer = 30;
		DemonPhase = 0;
		m_phase = 0;	// must be 0 to use it with some other stuff.
    }

    void OnCombatStart(Unit* mTarget)
    {
		DemonPhaseTimer = 30;
		DemonPhase = 0;
		m_phase = 1;
		_unit->GetAIInterface()->m_canMove = true;
		CastTime();
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You are not prepared!");	// only this one?
		_unit->PlaySoundToSet(11466);
		//RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void OnTargetDied(Unit* mTarget)
    {
		if (_unit->GetHealthPct() > 0)	// Hack to prevent double yelling (OnDied and OnTargetDied when creature is dying)
		{
			int RandomSpeach;
			sRand.randInt(1000);
			RandomSpeach=rand()%2;
			switch (RandomSpeach)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Who shall be next to taste my blades?!");
				_unit->PlaySoundToSet(11473);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "This is too easy!");
				_unit->PlaySoundToSet(11472);
				break;
			}
		}
    }

    void OnCombatStop(Unit *mTarget)
    {
		DemonPhaseTimer = 30;
		DemonPhase = 0;
		m_phase = 0;
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        //RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		DemonPhaseTimer = 30;
		DemonPhase = 0;
		m_phase = 0;
		CastTime();
		RemoveAIUpdateEvent();
	   	_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You have won... Maiev. But the huntress... is nothing without the hunt. You... are nothing... without me.");
		_unit->PlaySoundToSet(11478);
    }

	void AIUpdate()	//Unit *p_time
	{
		if (AKAMA_DIALOG)
		{
			switch(AKAMA_DIALOG)
			{
			case 1:
			    {
			        _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Akama... your duplicity is hardly surprising. I should have slaughtered you and your malformed brethren long ago.");
					_unit->PlaySoundToSet(11463);
					_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 378);	// EMOTE_STATE_TALK
			    }break;

			case 7:		// 8
				{
					_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
				}break;

			case 13:	// (12.5) I would give it 4.5 diff, but must check if this is possible to manipulate time between two AIUpdate voiding
			    {
			        _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Boldly said. But I remain unconvinced.");
					_unit->PlaySoundToSet(11464);
					_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 378);
			    }break;

			case 16:	//17
				{
					_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
				}break;

			case 18:
				{
					_unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
					_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
					_unit->GetAIInterface()->m_canMove = true;
				}break;
			};
			AKAMA_DIALOG++;
		}

		if (_unit->isInCombat())
		{
			if (DemonPhase > 0 && DemonPhase < 9)
			{
				DemonTrans();
			}
	
			if (DemonPhase > 9 && DemonPhase < 17)	
			{
				TransRevert();
			}
	
			else
			{
	
				if (m_phase == 1)	// First Phase (without demon form)
				{
					PhaseOne();
				}
			
				if (m_phase == 2)	// Second Phase (Fly phase)
				{
					PhaseTwo();
				}
		
				if (m_phase == 3)	// Third Phase normal form with ability to change into Demon
				{
					PhaseThree();
				}
		
				if (m_phase == 4)	// Fourth Phase Demon Form
				{
					PhaseFour();
				}
	
				if (m_phase == 5)	// Fifth Phase with ability to change into Demon and Enrage
				{
					PhaseFive();
				}
			}
		}
	}

	void PhaseOne()
	{
		if (_unit->GetHealthPct() <= 65)
		{
			//_unit->GetAIInterface()->WipeTargetList();
			//_unit->GetAIInterface()->WipeHateList();
			if(_unit->GetCurrentSpell() != NULL)
                _unit->GetCurrentSpell()->cancel();
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, U_FIELD_FLAG_UNIT_UNTACKABLE_SELECT);
			_unit->Emote(EMOTE_ONESHOT_LIFTOFF);
			_unit->GetAIInterface()->m_moveFly = true;

			_unit->GetAIInterface()->SetAllowedToEnterCombat(false);	// he can attack and this is main problem of this phase =S
			_unit->GetAIInterface()->m_canMove = false;
				
			_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
            _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
            _unit->GetAIInterface()->setWaypointToMove(0);
            WorldPacket data(SMSG_MOVE_SET_HOVER, 13);
            data << _unit->GetNewGUID();
            data << uint32(0);
            _unit->SendMessageToSet(&data, false);

			m_phase = 2;

			FoASummon();
			FoADeaths = 2;
		}

		else
		{
			float val = sRand.rand(100.0f);
			SpellCast(val);
		}
	}

	void FoASummon()	// little hacky way to spawn them by pushing into world, but proper spell won't work now
	{
		_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_FLAME_OF_AZZINOTH, 672.039246, 326.748322,
		354.206390, 0.207343, true, false, 0, 0);
		_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_FLAME_OF_AZZINOTH, 673.008667, 283.813660,
		354.267548, 6.203853, true, false, 0, 0);
	}

	void PhaseTwo()
	{
		if (_unit->GetAIInterface()->GetNextTarget())
		{
			Unit *target = NULL;
			target = _unit->GetAIInterface()->GetNextTarget();
			//_unit->CastSpell(target, spells[11].info, spells[11].instant);	// UNTESTED YET!	DISABLED BOTH AS THOSE CAN CAUSE CRASHES!
			//_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[11].info, spells[11].instant);
		}

		if (!FoADeaths)
		{
			if(_unit->GetCurrentSpell() != NULL)
                _unit->GetCurrentSpell()->cancel();
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
			_unit->Emote(EMOTE_ONESHOT_LAND);
			_unit->GetAIInterface()->m_moveFly = false;

			_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
			_unit->GetAIInterface()->m_canMove = true;
/*	// this is for future, because Idk how I will do fire walls (as I don't even know correct spell name =()
			_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
            _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
            _unit->GetAIInterface()->setWaypointToMove(0);
*/
			WorldPacket data(SMSG_MOVE_UNSET_HOVER, 13);
			data << _unit->GetNewGUID();
			data << uint32(0);
			_unit->SendMessageToSet(&data, false);
			m_phase = 3;
			DemonPhaseTimer = 30;
		}
	}

	void PhaseThree()
	{
		if (_unit->GetHealthPct() <= 30)	// add more stuff with maiev in future!
		{
			Maiev = true;
			//sRand.randInt(1000);
			//EnrageTimer=rand()%10+1;
			EnrageTimer = 10;	// UNTESTED!
			m_phase = 5;
		}
		
		else
		{
			DemonPhaseTimer--;
			if (!DemonPhaseTimer)
			{
				DemonPhase = 1;
			}
	
			else
			{
				float val = sRand.rand(100.0f);
				SpellCast(val);
			}
		}
	}

	void DemonTrans()
	{
		if(_unit->GetCurrentSpell() != NULL)
                _unit->GetCurrentSpell()->cancel();
		if (DemonPhase == 1)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Behold the power... of the demon within!");
			_unit->PlaySoundToSet(11475);
			_unit->CastSpell(_unit, spells[8].info, spells[8].instant);
		}

		if (DemonPhase == 2) //3
			_unit->CastSpell(_unit, spells[9].info, spells[9].instant);

		if (DemonPhase == 5)
		{
			_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID , 21322);	// hack, because I couldn't find right spell for now
			_unit->SetFloatValue(OBJECT_FIELD_SCALE_X , 5.0f);
		}

		if (DemonPhase == 6) // 6
			_unit->CastSpell(_unit, spells[10].info, spells[10].instant);

		if (DemonPhase == 8)
			m_phase = 4;

		DemonPhase++;
	}

	void TransRevert()
	{
		if(_unit->GetCurrentSpell() != NULL)
                _unit->GetCurrentSpell()->cancel();
		if (DemonPhase == 10)	// 1 diff is too long?
			//_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 403);	// changed to spells as I can't control timer in a way I would like to and without that it doesn't look good
			_unit->CastSpell(_unit, spells[8].info, spells[8].instant);

		if (DemonPhase == 11)
			//_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 404);
			_unit->CastSpell(_unit, spells[9].info, spells[9].instant);

		if (DemonPhase == 14)
		{
			_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID , 21137);	// hack, because I couldn't find right spell for now
			_unit->SetFloatValue(OBJECT_FIELD_SCALE_X , 1.5f);
		}

		if (DemonPhase == 15)
			//_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 405);
			_unit->CastSpell(_unit, spells[10].info, spells[10].instant);

		if (DemonPhase == 16)
		{
			//_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
			if (_unit->GetHealthPct() > 30)	// also can be if (Maiev == false)
				m_phase = 3;
			else							// if (Maiev == true)
				m_phase = 5;
		}

		DemonPhase++;
	}

	void PhaseFour()
	{
		DemonPhaseTimer++;
		if (DemonPhaseTimer == 30)
		{
			DemonPhase = 10;
		}

		if (_unit->GetHealthPct() <= 30 && Maiev == false)
		{
			DemonPhase = 10; // or DemonPhase = 10; or ... ?
		}

		else
		{
			if (_unit->GetAIInterface()->GetNextTarget())
			{
				Unit *target = NULL;
				target = _unit->GetAIInterface()->GetNextTarget();
				if (_unit->GetDistance2dSq(target) >= 0.0f && _unit->GetDistance2dSq(target) <= 10000.0f)
				{
					_unit->GetAIInterface()->m_canMove = false;
					int Spell;
					sRand.randInt(1000);
					Spell=rand()%100;
					if (Spell >= 0 && Spell <= 80)
						_unit->CastSpell(target, spells[5].info, spells[5].instant);
					if (Spell > 80 && Spell <= 90)
						_unit->CastSpell(target, spells[6].info, spells[6].instant);
					if (Spell > 90 && Spell <= 100)
						_unit->CastSpell(target, spells[1].info, spells[1].instant);
				}
				else
					_unit->GetAIInterface()->m_canMove = true;
			}

			else return;
		}
	}

	void PhaseFive()
	{
		DemonPhaseTimer--;
		EnrageTimer--;
		if (!DemonPhaseTimer)
		{
			DemonPhase = 1;
		}

		if (Enraged = false && !EnrageTimer)
		{
			_unit->CastSpell(_unit, spells[4].info, spells[4].instant);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You've wasted too much time mortals, now you shall fall!");
			_unit->PlaySoundToSet(11474);
			Enraged = true;
		}

		else
		{
			float val = sRand.rand(100.0f);
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

	bool Maiev;
	bool Enraged;
	int EnrageTimer;
	int DemonPhaseTimer;	// I know I could use one var, but it's easier in this way
	int DemonPhase;
	int nrspells;
};

void SetupBlackTemple(ScriptMgr * mgr)
{
	//mgr->register_creature_script(CN_SUPREMUS, &SupremusAI::Create);
	//mgr->register_creature_script(CN_FLAME_OF_AZZINOTH, &FlameOfAzzinothAI::Create);
	//mgr->register_creature_script(CN_AKAMA, &AkamaAI::Create);
	//mgr->register_creature_script(CN_ILLIDAN_STORMRAGE, &IllidanStormrageAI::Create);
}