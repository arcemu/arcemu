/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2012 <http://www.ArcEmu.org/>
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

#ifndef WOWSERVER_AIINTERFACE_H
#define WOWSERVER_AIINTERFACE_H

#define UNIT_MOVEMENT_INTERPOLATE_INTERVAL 400/*750*/ // ms smoother server/client side moving vs less cpu/ less b/w
#define TARGET_UPDATE_INTERVAL_ON_PLAYER 1000 // we most likely will have to kill players and only then check mobs
#define TARGET_UPDATE_INTERVAL 5000 // this is a multiple of PLAYER_TARGET_UPDATE_INTERVAL
#define PLAYER_SIZE 1.5f

#define ENABLE_CREATURE_DAZE
#ifdef ENABLE_CREATURE_DAZE
#define CREATURE_SPELL_TO_DAZE 1604
#define CREATURE_DAZE_TRIGGER_ANGLE M_H_PI //for the beginners this means 45 degrees 
#define CREATURE_DAZE_MIN_LEVEL 6	// minimal level of the target player to daze, from 3.3.0
#endif

#define DISTANCE_TO_SMALL_TO_WALK 2.0f //this is required so creature will not try to reposition itself to obtain perfect combat range. Not using this might lead to exploits

//!!! it is in seconds and not Milliseconds
#define MOB_SPELLCAST_GLOBAL_COOLDOWN	2 //there are individual cooldown and global ones. Global cooldown stops mob from casting 1 instant spell on you per second
#define MOB_SPELLCAST_REFRESH_COOLDOWN_INTERVAL	2

//#define INHERIT_FOLLOWED_UNIT_SPEED 1

//Pathfinding stuff
#define VERTEX_SIZE 3
#define MAX_PATH_LENGTH 512
#define SMOOTH_PATH_STEP_SIZE   6.0f
#define SMOOTH_PATH_SLOP        0.4f

inline bool inRangeYZX(const float* v1, const float* v2, const float r, const float h)
{
	const float dx = v2[0] - v1[0];
	const float dy = v2[1] - v1[1]; // elevation
	const float dz = v2[2] - v1[2];
	return (dx * dx + dz * dz) < r * r && fabsf(dy) < h;
}


class Object;
class Creature;
class Unit;
class Player;
class WorldSession;
class SpellCastTargets;

enum SplineFlags
{
    SPLINEFLAG_NONE         = 0x00000000,
    SPLINEFLAG_FORWARD      = 0x00000001,
    SPLINEFLAG_BACKWARD     = 0x00000002,
    SPLINEFLAG_STRAFE_LEFT  = 0x00000004,
    SPLINEFLAG_STRAFE_RIGHT = 0x00000008,
    SPLINEFLAG_LEFT         = 0x00000010,
    SPLINEFLAG_RIGHT        = 0x00000020,
    SPLINEFLAG_PITCH_UP     = 0x00000040,
    SPLINEFLAG_PITCH_DOWN   = 0x00000080,
    SPLINEFLAG_DONE         = 0x00000100,
    SPLINEFLAG_FALLING      = 0x00000200,
    SPLINEFLAG_NO_SPLINE    = 0x00000400,
    SPLINEFLAG_TRAJECTORY   = 0x00000800,
    SPLINEFLAG_WALKMODE     = 0x00001000,
    SPLINEFLAG_FLYING       = 0x00002000,
    SPLINEFLAG_KNOCKBACK    = 0x00004000,
    SPLINEFLAG_FINALPOINT   = 0x00008000,
    SPLINEFLAG_FINALTARGET  = 0x00010000,
    SPLINEFLAG_FINALFACING  = 0x00020000,
    SPLINEFLAG_CATMULLROM   = 0x00040000,
    SPLINEFLAG_UNKNOWN1     = 0x00080000,
    SPLINEFLAG_UNKNOWN2     = 0x00100000,
    SPLINEFLAG_UNKNOWN3     = 0x00200000,
    SPLINEFLAG_UNKNOWN4     = 0x00400000,
    SPLINEFLAG_UNKNOWN5     = 0x00800000,
    SPLINEFLAG_UNKNOWN6     = 0x01000000,
    SPLINEFLAG_UNKNOWN7     = 0x02000000,
    SPLINEFLAG_UNKNOWN8     = 0x04000000,
    SPLINEFLAG_UNKNOWN9     = 0x08000000,
    SPLINEFLAG_UNKNOWN10    = 0x10000000,
    SPLINEFLAG_UNKNOWN11    = 0x20000000,
    SPLINEFLAG_UNKNOWN12    = 0x40000000
};

enum WalkMode
{
    WALKMODE_SPRINT,
    WALKMODE_RUN,
    WALKMODE_WALK,
};

enum SplinePriority
{
    SPLINE_PRIORITY_MOVEMENT,
    SPLINE_PRIORITY_REDIRECTION,
};


enum AIType
{
    AITYPE_LONER,
    AITYPE_AGRO,
    AITYPE_SOCIAL,
    AITYPE_PET,
    AITYPE_TOTEM,
    AITYPE_GUARDIAN, //we got a master but he cannot control us, we follow and battle opposite factions
    AITYPE_PASSIVE
};

enum MovementType
{
    MOVEMENTTYPE_NONE,
    MOVEMENTTYPE_RANDOMWP,
    MOVEMENTTYPE_CIRCLEWP,
    MOVEMENTTYPE_WANTEDWP,
    MOVEMENTTYPE_DONTMOVEWP,
    MOVEMENTTYPE_QUEST = 10,
    MOVEMENTTYPE_FORWARDTHANSTOP = 11,
};


/*struct AI_Target
{
	Unit* target;
	int32 threat;
};*/


enum AI_Agent
{
    AGENT_NULL,
    AGENT_MELEE,
    AGENT_RANGED,
    AGENT_FLEE,
    AGENT_SPELL,
    AGENT_CALLFORHELP
};

enum AI_SpellType
{
    STYPE_NULL,
    STYPE_ROOT,
    STYPE_HEAL,
    STYPE_STUN,
    STYPE_FEAR,
    STYPE_SILENCE,
    STYPE_CURSE,
    STYPE_AOEDAMAGE,
    STYPE_DAMAGE,
    STYPE_SUMMON,
    STYPE_BUFF,
    STYPE_DEBUFF
};

enum AI_SpellTargetType
{
    TTYPE_NULL,
    TTYPE_SINGLETARGET,
    TTYPE_DESTINATION,
    TTYPE_SOURCE,
    TTYPE_CASTER,
    TTYPE_OWNER,
};

enum AI_State
{
    STATE_IDLE,
    STATE_ATTACKING,
    STATE_CASTING,
    STATE_FLEEING,
    STATE_FOLLOWING,
    STATE_EVADE,
    STATE_MOVEWP,
    STATE_FEAR,
    STATE_UNFEARED,
    STATE_WANDER,
    STATE_STOPPED,
    STATE_SCRIPTMOVE,
    STATE_SCRIPTIDLE
};

enum MovementState
{
    MOVEMENTSTATE_MOVE,
    MOVEMENTSTATE_FOLLOW,
    MOVEMENTSTATE_STOP,
    MOVEMENTSTATE_FOLLOW_OWNER
};

enum CreatureState
{
    STOPPED,
    MOVING,
    ATTACKING
};


struct SpellEntry;
//enum MOD_TYPES;

const uint32 AISPELL_ANY_DIFFICULTY = 4;

struct AI_Spell
{
	~AI_Spell() { autocast_type = (uint32) - 1; }
	uint32 entryId;
	uint32 instance_mode;
	uint16 agent;
	uint32 procChance;
	//int32 procCount;
	//uint32 procCountDB;
	SpellEntry* spell;
	uint8 spellType;
	uint8 spelltargetType;
	uint32 cooldown;
	uint32 cooldowntime;
	uint32 procCount;
	uint32 procCounter;
	float floatMisc1;
	uint32 Misc2;
	float minrange;
	float maxrange;
	uint32 autocast_type;
};

//Assume previous point can be reached through linked list or current creature position.
struct SplinePoint
{
	G3D::Vector3 pos;
	uint32 setoff; //mstime when npc set off of this point
	uint32 arrive; //mstime the npc reaches the destination
};

/*
#if ENABLE_SHITTY_STL_HACKS == 1
typedef HM_NAMESPACE::hash_map<Unit*, int32> TargetMap;
#else
namespace HM_NAMESPACE
{
	template <>
	struct hash<Unit*>
	{
		union __vp {
			size_t s;
			Unit* p;
		};

		size_t operator()(Unit* __x) const
		{
			__vp vp;
			vp.p = __x;
			return vp.s;
		}
	};
};

typedef HM_NAMESPACE::hash_map<Unit*, int32, HM_NAMESPACE::hash<Unit*> > TargetMap;
#endif
*/
typedef HM_NAMESPACE::hash_map<uint64, int32> TargetMap;

typedef std::set<Unit*> AssistTargetSet;
typedef std::map<uint32, AI_Spell*> SpellMap;

class SERVER_DECL AIInterface
{
	public:

		AIInterface();
		virtual ~AIInterface();

		// Misc
		void Init(Unit* un, AIType at, MovementType mt);
		void Init(Unit* un, AIType at, MovementType mt, Unit* owner); // used for pets
		Unit* GetUnit() { return m_Unit; }
		Unit* GetPetOwner() { return m_PetOwner; }
		void DismissPet();
		void SetUnitToFollow(Unit* un)
		{
			if(un == NULL)
				m_UnitToFollow = 0;
			else
				m_UnitToFollow = un->GetGUID();
		};
		void SetUnitToFollow(uint64 guid) { m_UnitToFollow = guid; };
		void ResetUnitToFollow() { m_UnitToFollow = 0; };
		void SetUnitToFear(Unit* un)
		{
			if(un == NULL)
				m_UnitToFear = 0;
			else
				m_UnitToFear = un->GetGUID();
		};
		void SetUnitToFear(uint64 guid)  { m_UnitToFear = guid; };
		void ResetUnitToFear() { m_UnitToFear = 0; };
		void SetUnitToFollowBackup(Unit* un)
		{
			if(un == NULL)
				m_UnitToFollow_backup = 0;
			else
				m_UnitToFollow_backup = un->GetGUID();
		};
		void SetUnitToFollowBackup(uint64 guid) { m_UnitToFollow_backup = guid; };
		void SetFollowDistance(float dist) { FollowDistance = dist; };
		void SetUnitToFollowAngle(float angle) { m_fallowAngle = angle; }
		bool setInFront(Unit* target);
		Unit* getUnitToFollow();
		uint64 getUnitToFollowGUID() { return m_UnitToFollow; }
		Unit* getUnitToFollowBackup();
		uint64 getUnitToFollowBackupGUID() { return m_UnitToFollow_backup; }
		Unit* getUnitToFear();
		uint64 getUnitToFearGUID() { return m_UnitToFear; }
		Creature* getFormationLinkTarget();
		void setCreatureState(CreatureState state) { m_creatureState = state; }
		ARCEMU_INLINE uint8 getAIState() { return static_cast<uint8>(m_AIState); }
		ARCEMU_INLINE uint8 getAIType() { return static_cast<uint8>(m_AIType); }
		void SetAIType(AIType at) { m_AIType = at; }
		ARCEMU_INLINE uint8 getCurrentAgent() { return static_cast<uint8>(m_aiCurrentAgent); }
		void setCurrentAgent(AI_Agent agent) { m_aiCurrentAgent = agent; }
		uint32	getThreatByGUID(uint64 guid);
		uint32	getThreatByPtr(Unit* obj);
		Unit*	GetMostHated();
		Unit*	GetSecondHated();
		bool	modThreatByGUID(uint64 guid, int32 mod);
		bool	modThreatByPtr(Unit* obj, int32 mod);
		void    RemoveThreatByGUID(uint64 guid);
		void	RemoveThreatByPtr(Unit* obj);
		ARCEMU_INLINE AssistTargetSet GetAssistTargets() { return m_assistTargets; }
		ARCEMU_INLINE void LockAITargets(bool lock)
		{
			lock ? m_aiTargetsLock.Acquire() : m_aiTargetsLock.Release();
		};
		ARCEMU_INLINE TargetMap* GetAITargets() { return &m_aiTargets; }
		void addAssistTargets(Unit* Friends);
		void ClearHateList();
		void WipeHateList();
		void WipeTargetList();
		bool taunt(Unit* caster, bool apply = true);
		Unit* getTauntedBy();
		bool GetIsTaunted();
		Unit* getSoullinkedWith();
		void SetSoulLinkedWith(Unit* target);
		bool GetIsSoulLinked();
		ARCEMU_INLINE size_t getAITargetsCount() { return m_aiTargets.size(); }
		ARCEMU_INLINE uint32 getOutOfCombatRange() { return m_outOfCombatRange; }
		void setOutOfCombatRange(uint32 val) { m_outOfCombatRange = val; }

		// Spell
		void CastSpell(Unit* caster, SpellEntry* spellInfo, SpellCastTargets targets);
		SpellEntry* getSpellEntry(uint32 spellId);
		SpellCastTargets setSpellTargets(SpellEntry* spellInfo, Unit* target);
		AI_Spell* getSpell();
		void addSpellToList(AI_Spell* sp);
		//don't use this until i finish it !!
//	void CancelSpellCast();

		// Event Handler
		void HandleEvent(uint32 event, Unit* pUnit, uint32 misc1);

		void EventForceRedirected(Unit* pUnit, uint32 misc1);
		void EventHostileAction(Unit* pUnit, uint32 misc1);
		void EventUnitDied(Unit* pUnit, uint32 misc1);
		void EventUnwander(Unit* pUnit, uint32 misc1);
		void EventWander(Unit* pUnit, uint32 misc1);
		void EventUnfear(Unit* pUnit, uint32 misc1);
		void EventFear(Unit* pUnit, uint32 misc1);
		void EventFollowOwner(Unit* pUnit, uint32 misc1);
		void EventDamageTaken(Unit* pUnit, uint32 misc1);
		void EventLeaveCombat(Unit* pUnit, uint32 misc1);
		void EventEnterCombat(Unit* pUnit, uint32 misc1);

		void OnDeath(Object* pKiller);
		void AttackReaction(Unit* pUnit, uint32 damage_dealt, uint32 spellId = 0);
		void HealReaction(Unit* caster, Unit* victim, SpellEntry* sp, uint32 amount);
		void EventAiInterfaceParamsetFinish();
		void EventChangeFaction(Unit* ForceAttackersToHateThisInstead = NULL);	//we have to tell our current enemies to stop attacking us, we should also forget about our targets

		// Update
		virtual void Update(uint32 p_time);

		void SetReturnPosition();

		void _UpdateTotem(uint32 p_time);

		// Movement
		void SendMoveToPacket();
		//void SendMoveToSplinesPacket(std::list<Waypoint> wp, bool run);
		bool MoveTo(float x, float y, float z, float o);
		void UpdateSpeeds();

		//Move flag updating
		void SetSplineFlag(uint32 flags) { m_splineFlags = flags; }
		uint32 HasSplineFlag(uint32 flags) { return m_splineFlags & flags; }
		void AddSplineFlag(uint32 flags) { m_splineFlags |= flags; }
		void RemoveSplineFlag(uint32 flags) { m_splineFlags &= ~flags; }
		bool Flying() { return HasSplineFlag(SPLINEFLAG_FLYING) != 0; }
		void SetFly() { SetSplineFlag(SPLINEFLAG_FLYING); }
		void SetSprint() { if(Flying()) return; SetSplineFlag(SPLINEFLAG_WALKMODE); SetWalkMode(WALKMODE_SPRINT); UpdateSpeeds(); }
		void SetRun() { if(Flying()) return; SetSplineFlag(SPLINEFLAG_WALKMODE); SetWalkMode(WALKMODE_RUN); UpdateSpeeds(); }
		void SetWalk() { if(Flying()) return; SetSplineFlag(SPLINEFLAG_WALKMODE); SetWalkMode(WALKMODE_WALK); UpdateSpeeds(); }
		void SetWalkMode(uint32 mode) { m_walkMode = mode; }
		bool HasWalkMode(uint32 mode) { return m_walkMode == mode; }
		void StopFlying() { if(Flying()) { SetSplineFlag(0); SetWalk(); } }

		uint32 m_splineFlags;
		uint32 m_walkMode;

		void UpdateMove();
		void SendCurrentMove(Player* plyr/*uint64 guid*/);
		bool StopMovement(uint32 time);
		uint32 getCurrentWaypoint() { return m_currentWaypoint; }
		void changeWayPointID(uint32 oldwpid, uint32 newwpid);
		//Adds a WayPoint, handling possible errors occurred when adding it. Pay attention: wp may point to free'd memory after calling this,
		//use bool addWayPoint(WayPoint* wp) instead to manually handle possible errors.
		void addWayPoint(WayPoint* wp);
		//returns true if the WayPoint was added, false otherwise. The caller MUST delete wp if it wasn't added.
		bool addWayPointUnsafe(WayPoint* wp);
		bool saveWayPoints();
		bool showWayPoints(Player* pPlayer, bool Backwards);
		bool hideWayPoints(Player* pPlayer);
		WayPoint* getWayPoint(uint32 wpid);
		void deleteWayPoint(uint32 wpid);
		void deleteWaypoints();
		ARCEMU_INLINE bool hasWaypoints() { return m_waypoints != NULL; }
		ARCEMU_INLINE void setMoveType(uint32 movetype) { m_moveType = movetype; }
		ARCEMU_INLINE uint32 getMoveType() { return m_moveType; }
		void setWaypointToMove(uint32 id) { m_currentWaypoint = id; }
		bool IsFlying();

		// Calculation
		float _CalcAggroRange(Unit* target);
		void _CalcDestinationAndMove(Unit* target, float dist);
		float _CalcCombatRange(Unit* target, bool ranged);
		float _CalcDistanceFromHome();
		uint32 _CalcThreat(uint32 damage, SpellEntry* sp, Unit* Attacker);

		void SetAllowedToEnterCombat(bool val) { m_AllowedToEnterCombat = val; }
		ARCEMU_INLINE bool GetAllowedToEnterCombat(void) { return m_AllowedToEnterCombat; }

		void CheckTarget(Unit* target);
		ARCEMU_INLINE void SetAIState(AI_State newstate) { m_AIState = newstate; }

		// Movement
		bool m_canMove;
		bool m_WayPointsShowing;
		bool m_WayPointsShowBackwards;
		uint32 m_currentWaypoint;
		bool m_moveBackward;
		uint32 m_moveType;
		bool onGameobject;
		CreatureState m_creatureState;
		size_t GetWayPointsCount()
		{
			if(m_waypoints && !m_waypoints->empty())
				return m_waypoints->size() - 1;	/* ignore 0 */
			else
				return 0;
		}
		bool m_canFlee;
		bool m_canCallForHelp;
		bool m_canRangedAttack;
		float m_FleeHealth;
		uint32 m_FleeDuration;
		float m_CallForHelpHealth;
		uint32 m_totemspelltimer;
		uint32 m_totemspelltime;
		SpellEntry* totemspell;

		uint32 m_totalMoveTime;
		ARCEMU_INLINE void AddStopTime(uint32 Time) { m_moveTimer += Time; }
		ARCEMU_INLINE void SetNextSpell(AI_Spell* sp) { m_nextSpell = sp; }
		Unit* getNextTarget();
		void setNextTarget(Unit* nextTarget);
		void setNextTarget(uint64 nextTarget);
		void resetNextTarget();

		/*ARCEMU_INLINE void ResetProcCounts()
		{
			AI_Spell * sp;
			for(list<AI_Spell*>::iterator itr = m_spells.begin(); itr != m_spells.end(); ++itr)
					{
						sp = *itr;
						sp->procCount =sp->procCountDB;
					}
		}*/

		uint64 m_formationLinkTarget;
		float m_formationFollowDistance;
		float m_formationFollowAngle;
		uint32 m_formationLinkSqlId;

		void WipeReferences();
		TimedEmoteList*		timed_emotes;
		ARCEMU_INLINE void SetPetOwner(Unit* owner) { m_PetOwner = owner; }

		list<AI_Spell*> m_spells;

		bool disable_combat;

		bool disable_melee;
		bool disable_ranged;
		bool disable_spell;
		bool disable_targeting;

		bool waiting_for_cooldown;

		uint32 next_spell_time;

		void CheckNextSpell(AI_Spell* sp)
		{
			if(m_nextSpell == sp)
				m_nextSpell = 0;
		}

		void ResetProcCounts();

		//deletes the old waypoint map as default. In case m_custom_waypoint_map is used, just call SetWaypointMap(NULL): this will delete m_custom_waypoint_map too.
		void SetWaypointMap(WayPointMap* m, bool delete_old_map = true);
		ARCEMU_INLINE WayPointMap* GetWaypointMap() { return m_waypoints; }
		void LoadWaypointMapFromDB(uint32 spawnid);
		bool m_isGuard;
		bool m_isNeutralGuard;
//	bool m_fastMove;
		void setGuardTimer(uint32 timer) { m_guardTimer = timer; }
		virtual void _UpdateCombat(uint32 p_time);

	protected:
		bool	UnsafeCanOwnerAttackUnit(Unit* pUnit);		//this is designed for internal use only
		bool m_AllowedToEnterCombat;

		// Update
		void _UpdateTargets();
		void _UpdateMovement(uint32 p_time);
		void _UpdateTimer(uint32 p_time);
		void AddSpline(float x, float y, float z);
		bool Move(float & x, float & y, float & z, float o = 0);
		void OnMoveCompleted();

		void MoveEvadeReturn();

		bool CreatePath(float x, float y, float z,  bool onlytest = false);
		dtStatus findSmoothPath(const float* startPos, const float* endPos, const dtPolyRef* polyPath, const uint32 polyPathSize, float* smoothPath, int* smoothPathSize, bool & usedOffmesh, const uint32 maxSmoothPathSize, dtNavMesh* mesh, dtNavMeshQuery* query, dtQueryFilter & filter);
		bool getSteerTarget(const float* startPos, const float* endPos, const float minTargetDist, const dtPolyRef* path, const uint32 pathSize, float* steerPos, unsigned char & steerPosFlag, dtPolyRef & steerPosRef, dtNavMeshQuery* query);
		uint32 fixupCorridor(dtPolyRef* path, const uint32 npath, const uint32 maxPath,
		                     const dtPolyRef* visited, const uint32 nvisited);

		bool m_updateAssist;
		bool m_updateTargets;
		uint32 m_updateAssistTimer;
		uint32 m_updateTargetsTimer;
		uint32 m_updateTargetsTimer2;

		// Misc
		Unit* FindTarget();
		Unit* FindTargetForSpell(AI_Spell* sp);
		bool FindFriends(float dist);
		AI_Spell* m_nextSpell;
		uint64 m_nextTarget;
		uint32 m_fleeTimer;
		bool m_hasFleed;
		bool m_hasCalledForHelp;
		uint32 m_outOfCombatRange;

//	Unit *gracefull_hit_on_target;
		Unit* m_Unit;
		Unit* m_PetOwner;
		float FollowDistance;
		float FollowDistance_backup;
		float m_fallowAngle;

		//std::set<AI_Target> m_aiTargets;
		Mutex m_aiTargetsLock;
		TargetMap m_aiTargets;
		AssistTargetSet m_assistTargets;
		AIType m_AIType;
		AI_State m_AIState;
		AI_Agent m_aiCurrentAgent;

		Unit*	tauntedBy; //This mob will hit only tauntedBy mob.
		bool	isTaunted;
		Unit*	soullinkedWith; //This mob can be hit only by a soul linked unit
		bool	isSoulLinked;


		// Movement
		float m_walkSpeed;
		float m_runSpeed;
		float m_flySpeed;

		float m_last_target_x;
		float m_last_target_y;


		/*
		Splines

		Note: First element in the spline (m_currentMoveSpline[0]) is always the position the creature started moving from.
		Index is always set to 1 when movement is started, as index 0 is referenced for first move.
		*/
		std::vector<SplinePoint> m_currentMoveSpline;
		uint32 m_currentMoveSplineIndex;
		uint32 m_currentSplineUpdateCounter;
		float m_currentSplineFinalOrientation;
		float m_splinetrajectoryVertical;
		uint32 m_splinetrajectoryTime;
		uint32 m_currentSplineTotalMoveTime;
		uint32 m_splinePriority;

		//Return position after attacking a mob
		float m_returnX;
		float m_returnY;
		float m_returnZ;

		float m_combatResetX;
		float m_combatResetY;
		float m_combatResetZ;

		float m_lastFollowX;
		float m_lastFollowY;
		//typedef std::map<uint32, WayPoint*> WayPointMap;

		uint64 m_UnitToFollow;
		uint64 m_UnitToFollow_backup;//used unly when forcing creature to wander (blind spell) so when effect wears off we can follow our master again (guardian)
		uint64 m_UnitToFear;

		uint32 m_timeToMove;
		uint32 m_timeMoved;
		uint32 m_moveTimer;
		uint32 m_FearTimer;
		uint32 m_WanderTimer;

		MovementType m_MovementType;
		MovementState m_MovementState;
		uint32 m_guardTimer;
		int32 m_currentHighestThreat;
		std::list<spawn_timed_emotes*>::iterator	next_timed_emote;
		uint32										timed_emote_expire;
	private:
		//specifies if m_waypoints was loaded from DB, so shared between other AIInterface instances.
		bool m_waypointsLoadedFromDB;
		WayPointMap* m_waypoints;
	public:
		bool m_is_in_instance;
		bool skip_reset_hp;

		void WipeCurrentTarget();

		void UpdateMovementSpline();
		bool MoveDone() { return m_currentMoveSplineIndex >= m_currentMoveSpline.size(); }
		bool CanCreatePath(float x, float y, float z) { return CreatePath(x, y, z, true); }
		void MoveKnockback(float x, float y, float z, float horizontal, float vertical);
		void MoveJump(float x, float y, float z, float o = 0);
		void MoveTeleport(float x, float y, float z, float o = 0);
		bool MoveCharge(float x, float y, float z);
};
#endif
