/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2010 <http://www.ArcEmu.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef AIINTERFACE_H
#define AIINTERFACE_H

/* platforms that already define M_PI in math.h */
#ifdef M_PI
#undef M_PI
#endif

#define M_PI	   3.14159265358979323846
#define M_2PI		M_PI*2
#define UNIT_MOVEMENT_INTERPOLATE_INTERVAL 200// ms smoother server/client side moving vs less cpu/ less b/w
#define TARGET_UPDATE_INTERVAL 1000 // we most likely will have to kill players and only then check mobs
#define AIINTERFACE_UPDATE_INTERVAL 200
#define PLAYER_SIZE 1.5f
#define FLOAT_E 0.00001f //Epsilon, better that comparing to 0.0f
#define TO_RADS(angle) (angle * (float)M_PI/180.0f)

#define ENABLE_CREATURE_DAZE
#ifdef ENABLE_CREATURE_DAZE
	#define CREATURE_SPELL_TO_DAZE 1604
	#define CREATURE_DAZE_TRIGGER_ANGLE M_PI/2 //for the beginners this means 45 degrees 
	#define CREATURE_DAZE_MIN_LEVEL 6	// minimal level of the target player to daze, from 3.3.0
#endif

class MapMgr;
class Object;
class Creature;
class Unit;
class Player;
class WorldSession;
class SpellCastTargets;

//The almighty behavioral bits.
enum BehaviorBits
{
	//The following determine along w/ their respective states whether we perform the movement code
	BEHAVIOR_NONE = 0x0,
	BEHAVIOR_UNITISMOVING = 1 << 0, //Set when unit starts moving, cleared when we stop moving.
	BEHAVIOR_BLOCKPANICMOVEMENT = 1 << 1, //When set, it prevents fear movement indefinitely
	BEHAVIOR_BLOCKDISORIENTMOVEMENT = 1 << 2, //When set it prevents disorient movement indefinitely
	BEHAVIOR_CANSUMMONGUARDS = 1 << 3,
	BEHAVIOR_ISPROCESSINGAGENT = 1 << 4,	//used by Mob::get/setProcessingAgent()
	BEHAVIOR_UPDATEASSISTARGETS = 1 << 5,	//set when when it's time to update assist targets.
	BEHAVIOR_UPDATETARGETS = 1 << 6,	//set when it's time to update our threat list entries
	BEHAVIOR_BLOCKSPELLSPAMMING = 1 << 7,	//set to prevent spell spamming.
	BEHAVIOR_PANIC_EFFECT = 1 << 8,	//set when we are feared.
	BEHAVIOR_SKYWALKER = 1 << 9,	//used by some creatures spawning on gameobjects so as to send proper update flags.
	BEHAVIOR_ISMOVINGTOWAYPOINT  = 1 << 10,	//set when unit is moving to a waypoint, cleared when we reach a waypoint.

	//TO DO : More bits to define custom functionality

	/*
	This is set when we are processing an agent.
	However, if for example we are trying to move around so we can meet distance requirements
	and the target is making this impossible(by moving around). We can override the processing agent by clearing this.
	We can then pick another agent and avoid an endless loop
	*/
	BEHAVIOR_PROCAGENTMASK = 0xE00000,
	BEHAVIOR_PROCAGENTSHIFTCNT = 29,
	

};

class SERVER_DECL AIInterface
{
protected:
	//Holds the current moving move, either walk/run/fly/sprint
	uint8 m_moveMode;

	/*	Flag that controls which feature is enabled or disabled.
		When set:
			Bit 0 = disables combat 
			Bit 1 = disables melee
			Bit 2 = disables ranged 
			Bit 3 = disables spells
			Bit 4 = disables targetting.
	*/
	uint8 m_disableFlag;

	/*	Flag that controls panic feature.
		Bits 0-4 Hold Flee Health Pct(Max is a 100 so we don't need any more than 5 bits to hold 100.
		Bit 5 - holds the status of whether we've fled or not.
	*/
	uint8 m_panicFlag; 
	/*
		Duration to flee in seconds */
	uint8 m_panicDuration;
	/*
		Bit 0-4 call for help hp.
		Bit 5 - have we called for help.
	*/
	uint8 m_callhelpFlag;
	
	//Stores the current ai state
	uint8 m_AIState;
	//Stores the current interface type
	uint8 m_type;
	//Stores the current creature state
	uint8 m_creatureState;

	/* The mask manipulated by Behavior_%s functions. */
	uint32 m_behaviorMask;

	/* Movement related data */
	void _updateMovement(uint32 p_time);

	float m_walkSpeed;
	float m_runSpeed;
	float m_flySpeed;

	//This holds the location to where the unit is moving to.
	LocationVector m_destVector;
	//Set when we enter combat, and we return here on leave combat.
	LocationVector m_returnToVector;

	//Set when we are moving to a new location, holds how much time it woudl take to get there based on the current move mode.
	uint32 m_timeToMove;
	//Records how much time we've spent moving to the new location.
	uint32 m_timeMoved;
	//This blocks movement while it's greater than 0, it serves as to optimize unit movement updates but also to make the unit wait at it's current location for some time.
	int32 m_moveTimer;
	uint32 m_totalMoveTime;

	Unit * m_Self;
	int32 m_UpdateTime;

	struct FORMATION_DATA
	{
		uint64 formationHead;
		uint32 formationHeadSqlId;
		float formationAngle;
		uint32 formationDistance;
	} formation;
	
	/* External unit guids */
	uint64 m_UnitToFear;
	uint64 m_ForcedTarget;
	uint64 m_soulLinker;

public:
	AIInterface(Unit * self);
	virtual ~AIInterface() {}
	
	Unit * getUnit() { return m_Self; }
	Unit * findClosestTarget( bool usecombatrange = false);
	Unit * findRandomTarget();

	bool isMoving() { return (m_creatureState == MOVING); }

	/* owner methods */
	virtual Unit * getOwner() { return NULL; }
	virtual void setOwner() {}

	/* Formation head methods */
	void clearFormationData() {
		memset(&formation,0,sizeof(FORMATION_DATA) );
	}

	void setUnitToFollow(uint64 guid)
	{ 
		formation.formationHead = guid;
	}
	void setFormationHeadSqlId(uint32 sqlid) {
		formation.formationHeadSqlId = sqlid;
	}
	uint32 getFormationHeadSqlId() { return formation.formationHeadSqlId; }

	Unit * getUnitToFollow();

	/* Formation distance */
	void setFollowDistance(uint32 dist) { formation.formationDistance = dist; }
	uint32 getFollowDistance() { return formation.formationDistance; }

	/* Formation angle */
	void setFollowAngle(float angle) { formation.formationAngle = angle; }
	float getFollowAngle() { return formation.formationAngle; }

	/* unit to fear */
	Unit * getUnitToFear();
	void setUnitToFear(Unit* un)
	{ 
		if(un == NULL)
			m_UnitToFear = 0;
		else
			m_UnitToFear = un->GetGUID(); 
	}

	/* forced target (ex : taunting)*/
	void setForcedTarget(Unit * target, bool override = false) { 
		//Only set a new target if our old target is 0.(To prevent overriding.)
		if(target == NULL)
			m_ForcedTarget = 0;
		else if(!m_ForcedTarget || override)
			m_ForcedTarget = target->GetGUID();
	}
	Unit * getForcedTarget();
	bool hasForcedTarget() { return m_ForcedTarget != 0; }

	/* panic methods */
	uint32 getPanicHp() { return uint32(m_panicFlag & AIPANICK_HPMASK); }
	void setPanicHp(uint32 hp) {
		if(hp && hp <= 100)
		{
			//first clear the previous hp
			m_panicFlag &= ~ AIPANICK_HPMASK;
			//then set
			m_panicFlag |= (hp & AIPANICK_HPMASK);
		}
	}
	/*
		Returns the panic duration in SECONDS */
	uint32 getPanicDuration() { return (uint32)m_panicDuration; }
	void setPanicDuration(uint32 newDuration) { m_panicDuration = (uint8)newDuration; }
	bool hasPanicked() { return (m_panicFlag & AIPANIC_HASPANICKED) != 0; }
	void setHasPanicked(bool set = true) {
		if(set)
			m_panicFlag |= AIPANIC_HASPANICKED;
		else
			m_panicFlag &= ~AIPANIC_HASPANICKED;
	}

	/* call for help methods */
	uint32 getCallHelpHp() { return uint32(m_callhelpFlag & AICALLHELP_HPMASK); }
	void setCallHelpHp(uint32 hp) {
		if(hp && hp <= 100)
		{
			m_callhelpFlag &= ~AICALLHELP_HPMASK;
			m_callhelpFlag |= (hp & AICALLHELP_HPMASK);
		}
	}
	bool hasCalledHelp() { return ((m_callhelpFlag & AICALLHELP_HASCALLED) != 0); }
	void setHasCalledHelp(bool set = true) {
		if(set)
			m_callhelpFlag |= AICALLHELP_HASCALLED;
		else
			m_callhelpFlag &= ~AICALLHELP_HASCALLED;
	}
	/* disable features method */
	bool getCombatDisable() { return (m_disableFlag & DISABLE_COMBAT); }
	void disableCombat(bool set = true) {
		if(set)
			m_disableFlag |= DISABLE_COMBAT;
		else
			m_disableFlag &= ~DISABLE_COMBAT;
	}
	bool getMeleeDisable() { return (m_disableFlag  & DISABLE_MELEE) != 0; }
	void disableMelee(bool set = true) {
		if(set)
			m_disableFlag |= DISABLE_MELEE;
		else
			m_disableFlag &= ~DISABLE_MELEE;
	}
	bool getRangedDisable() { return (m_disableFlag & DISABLE_RANGED) != 0; }
	void disableRanged(bool set = true) {
		if(set)
			m_disableFlag |= DISABLE_RANGED;
		else
			m_disableFlag &= ~DISABLE_RANGED;
	}
	bool getSpellDisable() { return (m_disableFlag & DISABLE_SPELLS) != 0; } 
	void disableSpells(bool set = true) {
		if(set)
			m_disableFlag |= DISABLE_SPELLS;
		else
			m_disableFlag &= ~DISABLE_SPELLS;
	}
	bool getTargettingDisable() { return (m_disableFlag & DISABLE_TARGETTING) != 0; }
	void disableTargetting(bool set = true) {
		if(set)
			m_disableFlag |= DISABLE_TARGETTING;
		else
			m_disableFlag &= ~DISABLE_TARGETTING;
	}

	/* soul link methods */
	//************************************
	// Purpose:	Retrieves the unit that is soul linked to this unit or NULL if none.
	//************************************
	Unit * getSoulLinker();

	//************************************
	// Purpose:	Returns true if we are soul linked.
	//************************************
	bool isSoulLinked() { return (m_soulLinker != 0); }
	//************************************
	// Purpose:	Sets a new soul link, this unit will only be able to damage and take damage from this soul link only.
	// Parameter: uint64 guid - guid of a soul link
	//************************************
	void setSoulLinker(uint64 guid) { m_soulLinker = guid; }

	/* ai state methods */
	//************************************
	// Purpose:	Retrieves the current ai state.
	//************************************
	uint8 getAIState() { return m_AIState; }
	//************************************
	// Purpose:	Sets a new ai state, the creature behaves differently with each new state. States are also automatically set internally when needed.
	//			e.g when a unit leaves combat, it sets it's state to evading, during that time, it doesn't do anything else apart from return to spawn.
	//			During :
	//					STATE_IDLE - unit is open to enter combat and also to wander around.
	//					STATE_FOLLOWING - unit is constantly following it's formation head, it is also open to enter combat.
	//					STATE_FLEEING - unit is running around in fear.
	//					STATE_SCRIPTMOVE - the unit can use it's waypoints but can't enter into combat, and incoming dmg doesn't provoke it.
	//					STATE_SCRIPTIDLE - the unit becomes a dummy, doesn't react to dmg and doesn't use waypoints, but it can still move.
	//			
	// Parameter: AI_State nState - new state to set.
	//************************************
	void setAIState(AI_State nState) { m_AIState = (uint8)nState; }

	//************************************
	// Purpose:	Retrieves the Interface type, this is different for interfaces that inherit from this.
	//************************************
	uint8 getAIType() { return m_type; }

	/* Violates OO principles? 
	bool AIType_isCritter() { return m_type == AITYPE_CRITTER; }
	bool AIType_isPet() { return m_type == AITYPE_PET; }
	bool AIType_isGuardian() { return m_type == AITYPE_GUARDIAN; }
	bool AIType_isTotem() { return m_type == AITYPE_TOTEM; }
	bool AIType_isMob() { return m_type == AITYPE_MOB; } */

	/******************** SPELL METHODS *******************/

	//************************************
	// Purpose:	This is called to reserve any storage due to most container types being vectors.
	// Parameter: uint32 - number of elements to be stored.
	//************************************
	virtual void Spell_reserveStorage(uint32) {}

	//************************************
	// Purpose:	Adds the specified spell into an internal spell storage.
	// Parameter: AI_Spell * - an aispell to add.
	//************************************
	virtual void Spell_add(AI_Spell*) {}
	//************************************
	// Purpose:	Removes the specified spell from an internal spell storage.
	// Parameter: AI_Spell * - an aispell to remove
	//************************************
	virtual void Spell_remove(AI_Spell*) {}
	//************************************
	// Purpose:	Returns true if the specified spell is found inside the internal spell storage, false if otherwise.
	// Parameter: uint32 spell_id - a spell to search for.
	//************************************
	virtual bool Spell_has(uint32 spell_id) { return false; }
	//************************************
	// Purpose:	Returns the next spell to cast.
	//************************************
	virtual AI_Spell * Spell_getnext() { return NULL; }
	//************************************
	// Purpose:	Sets the next spell to cast, usually it inserts the spell into some spell queue.
	// Parameter: AI_Spell * - an aispell to cast next.
	//************************************
	virtual void Spell_setnext(AI_Spell*) {}
	//************************************
	// Purpose:	Returns an expected valid target based on the specified spell. Usually used to retrieve a valid target to cast the spell on or null if we can't.
	// Parameter: AI_Spell * - an aispell to retrieve a target based on the internal fields.
	//************************************
	virtual Unit * Spell_getarget(AI_Spell *) { return NULL; }
	//************************************
	// Purpose:	Performs a spell cast given the spell and the target. It also does internal calculations for special ai spell types.
	// Parameter: AI_Spell * spell - an aispell type to cast.
	// Parameter: Unit * target - a target to cast the spell on.
	//************************************
	bool handleSpellCast( AI_Spell * spell, Unit * target );
	//************************************
	// Purpose:	Retrieves spells contained inside the spell store if any. Stores them inside the vector reference specified.
	// Parameter: std::vector<AI_Spell * > & - a reference to a vector container to store the spells.
	//************************************
	virtual const std::vector<AI_Spell*> * getSpellStore() { return NULL;}
	//************************************
	// Purpose:	Returns the number of spell elements inside a a spell store.
	//************************************
	virtual uint32 getSpellStoreSize() { return 0; }
	//************************************
	// Purpose:	Called when an AiEvent happens, used to cast spells that should be cast during that event.
	// Parameter: AiEvents - the event type that is occurring.
	//************************************
	virtual void Spell_handlevent(AiEvents) {}
	// Update

	//************************************
	// Purpose:	Calls OnUpdate() every 200ms, called directly from Unit::Update(0
	// Parameter: uint32 p_time
	//************************************
	void Update(uint32 p_time);
	//************************************
	// Purpose:	A fixed update frequency called every 200ms, is handled differently by different interfaces.
	//************************************
	virtual void OnUpdate() = 0;


	/************************ MOVEMENT METHODS **********************************/
	//////////////////////////////////////////////////////////////////////////////

	//************************************
	// Purpose:	Sends an SMSG_MONSTER_MOVE using the given parameters. Usually sent once when a monster's position changes.
	// Parameter: float & toX - a X coordinate
	// Parameter: float & toY - a Y coordinate
	// Parameter: float & toZ - a Z coordinate
	// Parameter: float & toO - an orientation - optional.
	// Parameter: uint32 time - the time it takes to move to the specified location.
	// Parameter: uint32 MoveFlags - movement flags sent based on the current move mode. Walk/Run use 0, Fly use 0x2000.
	//************************************
	void SendMoveToPacket(float &toX, float &toY, float &toZ, float &toO, uint32 time, uint32 MoveFlags);

	//************************************
	// Purpose:	Used to make a unit move to a new location specified by the coordinates, also responsible for calling SendMoveToPacket.
	// Parameter: float x
	// Parameter: float y
	// Parameter: float z
	// Parameter: float o
	//************************************
	void MoveTo(float x, float y, float z, float o);

	//************************************
	// Purpose:	Calculates and returns the time, in millisecs, it would take for the unit to reach the specified location using the current move mode.
	// Parameter: float & x
	// Parameter: float & y
	// Parameter: float & z
	//************************************
	uint32 calcMoveTimeToLocation(float & x, float & y, float & z);

	//************************************
	// Purpose: Not sure what this method is for
	// Parameter: Player * plyr
	//************************************
	void SendCurrentMove(Player* plyr);
	//************************************
	// Purpose:	This stops the unit from moving, also sends a stopping packet.
	// Parameter: uint32 time - How long, in millisecs, to wait before moving.
	//************************************
	void StopMovement(uint32 time);
	//************************************
	// Purpose:	Called when the unit stops moving, performs some actions like timed emotes or resolves which waypoint to move to next
	//************************************
	void OnStopMovement();

	//************************************
	// Purpose:	Called when we are moving or when we have stopped moving, it's responsible for calling OnStopMovement()
	//************************************
	void handleMovement();
	//************************************
	// Purpose:	Called to handle the movement while the unit is fleeing.
	//************************************
	void handleFearMovement();
	//************************************
	// Purpose:	Called to handle the movement while the unit is confused.
	//************************************
	void handleWanderMovement();
	//************************************
	// Purpose:	Called when we are following the head formation unit, performs distance checks and updates the speed if needed.
	//************************************
	void handleFollowUnitMovement();

	virtual bool hasWaypoints() { return false; }

	/************************** BEHAVIOR BITS MUTATOR *******************\\
	// BEHAVIOR BITS : An internal 32bit dword value in which each bit represents some behavior defined by enum BehaviorBits.
	// More bits can be defined and used within the code to allow for custom functionality, might use the left over bits to extend ai agents.*/
	bool Behavior_has(BehaviorBits bit);
	void Behavior_set(BehaviorBits bit);
	void Behavior_clear(BehaviorBits bit);
	void Behavior_setandclear(BehaviorBits bit, uint32 time);
	void Behavior_clearandset(BehaviorBits bit, uint32 time);
	void Behavior_eventset(BehaviorBits bit, uint32 delay);
	void Behavior_eventclear(BehaviorBits bit, uint32 delay);

	/************* AI MOVEMENT MUTATORS ******************/

	//************************************
	// Purpose:	Returns the current moving mode (walk, run,fly, sprint).
	//************************************
	AIMovementFlags Movement_getmovemode();
	//************************************
	// Purpose:	Changes the moving mode.
	// Parameter: AIMovementFlags flag - new moving mode to use.
	//************************************
	void Movement_setmovemode(AIMovementFlags flag);
	//************************************
	// Purpose:	Enables or disables movement based on the specified boolean.
	// Parameter: bool allow - true to enable movement, false to disable.
	//************************************
	void Movement_allowmovement(bool allow = true);
	bool Movement_canmove();
	bool Movement_isflying();

	float calcAggroRange(Unit* target);
	void calcDestinationAndMove(Unit *target, float dist);
	float calcCombatRange(Unit* target, bool ranged);
	uint32 calcThreat(uint32 damage, SpellEntry * sp, Unit* Attacker);
	
	void SetAllowedToEnterCombat(bool val) { disableCombat(!val); }
	bool GetAllowedToEnterCombat() { return !getCombatDisable(); }

	void AddStopTime(uint32 Time) { m_moveTimer += Time; }
	virtual Unit * getNextTarget() { return NULL; }
	virtual void setNextTarget (Unit * nextTarget) {}

	virtual void HandleEvent(uint32 event, Unit* pUnit, uint32 misc1) {}
	virtual void AttackReaction(Unit *pUnit, uint32 damage_dealt, uint32 spellId = 0) {}
	virtual void HealReaction(Unit* caster, Unit* victim, SpellEntry* sp, uint32 amount) {}
};
#endif
