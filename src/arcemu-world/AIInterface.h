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
#define UNIT_MOVEMENT_INTERPOLATE_INTERVAL 400/*750*/ // ms smoother server/client side moving vs less cpu/ less b/w
#define TARGET_UPDATE_INTERVAL_ON_PLAYER 1000 // we most likely will have to kill players and only then check mobs
#define TARGET_UPDATE_INTERVAL 5000 // this is a multiple of PLAYER_TARGET_UPDATE_INTERVAL
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
#define SMALL_DISTANCE 1.0f
//!!! it is in seconds and not Milliseconds
#define MOB_SPELLCAST_GLOBAL_COOLDOWN	2 //there are individual cooldown and global ones. Global cooldown stops mob from casting 1 instant spell on you per second
#define MOB_SPELLCAST_REFRESH_COOLDOWN_INTERVAL	2

//#define INHERIT_FOLLOWED_UNIT_SPEED 1

#define HACKY_SERVER_CLIENT_POS_SYNC

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
	BEHAVIOR_ISCASTINGMOVEBLOCK = 1 << 1,
	BEHAVIOR_BLOCKPANICMOVEMENT = 1 << 2, //When set, it prevents fear movement indefinitely
	BEHAVIOR_BLOCKDISORIENTMOVEMENT = 1 << 3, //When set it prevents disorient movement indefinitely
	BEHAVIOR_CANSUMMONGUARDS = 1 << 4,
	BEHAVIOR_ISPROCESSINGAGENT = 1 << 5,
	BEHAVIOR_UPDATEASSISTARGETS = 1 << 6,
	BEHAVIOR_UPDATETARGETS = 1 << 7,
	BEHAVIOR_BLOCKSPELLSPAMMING = 1 << 8,
	BEHAVIOR_PANIC_EFFECT = 1 << 9,
	BEHAVIOR_SKYWALKER = 1 << 10,

	//TO DO : More bits to define custom functionality

	/*
	This is set when we are processing an agent.
	However, if for example we are trying to move around so we can meet distance requirements
	and the target is making this impossible. We can override the processing agent by clearing this.
	We can then pick another agent
	*/
	BEHAVIOR_PROCAGENTMASK = 0xE00000,
	BEHAVIOR_PROCAGENTSHIFTCNT = 29,
	

};

class SERVER_DECL AIInterface
{
protected:
	/* Movement data */
	uint8 m_moveType;
	uint8 m_moveMode;
	/*	Bit 0 = disable combat 
		Bit 1 = disable melee
		Bit 2 = disable ranged 
		Bit 3 = disable spells
		Bit 4 = disable targetting.
	*/
	uint8 m_disableFlag;
	/*
		Bit 0-4 Flee Health Pct(Max is a 100 so we don't need any more than 5 bits to hold 100.
		Bit 5 - have we fled
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
	uint8 m_AIState;
	uint8 m_type;
	uint8 m_creatureState;
	/* Behavior Mask  : Used to control certain internal functions */
	uint32 m_behaviorMask;

	uint32 m_updateAssistTimer;
	uint32 m_updateTargetsTimer;
	uint32 m_updateTargetsTimer2;


	/* Movement related data */
	void _UpdateMovement(uint32 p_time);
	void _UpdateTimer(uint32 p_time);

	float m_walkSpeed;
	float m_runSpeed;
	float m_flySpeed;
	//Determines where our unit will move next.
	LocationVector m_destVector;
	//Return position after attacking a mob
	LocationVector m_returnToVector;
	uint32 m_timeToMove;
	uint32 m_timeMoved;
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
	void UpdateMove();

public:
	AIInterface(Unit * self);
	virtual ~AIInterface() {}
	Unit * getUnit() { return m_Self; }
	Unit * findClosestTarget( bool usecombatrange = false);
	Unit * findRandomTarget();

	/* owner methods */
	virtual Unit * getOwner() { return NULL; }
	virtual void setOwner(Unit *) {}

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
	void setForcedTarget(Unit * target) { 
		//Only set a new target if our old target is 0.(To prevent overriding.)
		if(target == NULL)
			m_ForcedTarget = 0;
		else if(!m_ForcedTarget)
			m_ForcedTarget = target->GetGUID();
	}
	Unit * getForcedTarget();
	bool hasForcedTarget() { return m_ForcedTarget != 0; }

	/* panic methods */
	uint32 getPanicHp() { return uint32(m_panicFlag & AIPANICK_HPMASK); }
	void setPanicHp(uint32 hp) {
		if(hp && hp <= 100)
			m_panicFlag |= (hp & AIPANICK_HPMASK);
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
			m_callhelpFlag |= (hp & AICALLHELP_HPMASK);
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
	Unit * getSoulLinker();
	bool isSoulLinked() { return (m_soulLinker != 0); }
	void setSoulLinker(uint64 guid) { m_soulLinker = guid; }

	/* ai state methods */
	uint8 getAIState() { return m_AIState; }
	void setAIState(AI_State nState) { m_AIState = (uint8)nState; }
	/* ai type, used for down casting */
	uint8 getAIType() { return m_type; }
	/* easy type methods */
	bool AIType_isCritter() { return m_type == AITYPE_CRITTER; }
	bool AIType_isPet() { return m_type == AITYPE_PET; }
	bool AIType_isGuardian() { return m_type == AITYPE_GUARDIAN; }
	bool AIType_isTotem() { return m_type == AITYPE_TOTEM; }
	bool AIType_isMob() { return m_type == AITYPE_MOB; }

	/* spell mutators */
	virtual void addSpell(AI_Spell*) {}
	virtual void removeSpell(AI_Spell*) {}
	virtual bool hasSpell(uint32 spell_id) { return false; }
	virtual AI_Spell * getNextSpell() { return NULL; }
	virtual void setNextSpell(AI_Spell*) {}
	virtual Unit * getSpellTarget(AI_Spell *) { return NULL; }
	void handleSpellCast( AI_Spell * spell, Unit * target );
	virtual std::list<AI_Spell*> * getSpellStore() { return NULL; }
	// Update
	void Update(uint32 p_time);
	virtual void OnUpdate() = 0;


	/* Movement */
	void SendMoveToPacket(float &toX, float &toY, float &toZ, float &toO, uint32 time, uint32 MoveFlags);
	void MoveTo(float x, float y, float z, float o);
	uint32 calcTimeToDestination(float & x, float & y, float & z);
	uint32 getMoveFlags();
	void SendCurrentMove(Player* plyr);
	void StopMovement(uint32 time);
	void OnStopMovement();
	void handleMovement();
	void handleFearMovement();
	void handleWanderMovement();
	void handleFollowUnitMovement();

	// Behavior manipulators 
	bool hasBehavior(BehaviorBits bit) { return (m_behaviorMask & bit) != 0; }
	void setBehavior(BehaviorBits bit) { m_behaviorMask |= bit; }
	void clearBehavior(BehaviorBits bit) {
		if(m_behaviorMask & bit)
			m_behaviorMask &= ~bit;
	}
	void eventSetBehavior(BehaviorBits bit, uint32 delay) {
		if(!delay) setBehavior(bit);
		else
		{
			TimedEvent * tE = TimedEvent::Allocate(this,new CallbackP1<AIInterface,BehaviorBits>(this,&AIInterface::setBehavior,bit),EVENT_AIIBEHAVIORMODIFY,delay,1);
			m_Self->event_AddEvent(tE);
		}
	}
	void eventClearBehavior(BehaviorBits bit, uint32 delay) {
		if(!delay) clearBehavior(bit);
		else
		{
			TimedEvent * tE = TimedEvent::Allocate(this,new CallbackP1<AIInterface,BehaviorBits>(this,&AIInterface::clearBehavior,bit),EVENT_AIIBEHAVIORMODIFY,delay,1);
			m_Self->event_AddEvent(tE);
		}
	}
	virtual bool hasWaypoints() { return false; }
	void setAIMoveFlags(AIMovementFlags flag) { m_moveMode = (uint8)flag; }
	AIMovementFlags getAIMoveFlags() { return AIMovementFlags(m_moveMode); }
	bool isFlying();

	float calcAggroRange(Unit* target);
	void calcDestinationAndMove(Unit *target, float dist);
	float calcCombatRange(Unit* target, bool ranged);
	uint32 calcThreat(uint32 damage, SpellEntry * sp, Unit* Attacker);
	
	void SetAllowedToEnterCombat(bool val) { disableCombat(!val); }
	bool GetAllowedToEnterCombat() { return !getCombatDisable(); }

	void AddStopTime(uint32 Time) { m_moveTimer += Time; }
	virtual Unit * getNextTarget() { return NULL; }
	virtual void setNextTarget (Unit * nextTarget) {}


	virtual void wipeReferences();

	virtual void HandleEvent(uint32 event, Unit* pUnit, uint32 misc1) {}
	virtual void AttackReaction(Unit *pUnit, uint32 damage_dealt, uint32 spellId = 0) {}
	virtual void HealReaction(Unit* caster, Unit* victim, SpellEntry* sp, uint32 amount) {}

};
#endif
