#pragma  once
enum AIType
{
	AITYPE_MOB,
	AITYPE_PET,
	AITYPE_CRITTER,
	AITYPE_TOTEM,
	AITYPE_GUARDIAN, //we got a master but he cannot control us, we follow and battle opposite factions
};

enum MovementType
{
	MOVEMENTTYPE_NONE,
	MOVEMENTTYPE_DONTMOVEWP,
	MOVEMENTTYPE_WANTEDWP,
	MOVEMENTTYPE_RANDOMWP,
	MOVEMENTTYPE_CIRCLEWP,
	MOVEMENTTYPE_FORWARDTHENSTOP,
	WAYPOINT_MOVEBACKWARD = 0x08,
	WAYPOINT_SHOWFORWARD = 0x10,
	WAYPOINT_SHOWBACKWARD = 0x20,
	WAYPOINT_MOVETYPEMASK =  0x07 // [0000,0111] 
};

enum AI_State
{
	STATE_IDLE,
	STATE_ATTACKING,
	STATE_CASTING,
	STATE_FLEEING,
	STATE_FOLLOWING,
	STATE_EVADE,
	STATE_FEAR,
	STATE_WANDER,
	STATE_STOPPED,
	STATE_SCRIPTMOVE,
	STATE_SCRIPTIDLE
};

enum CreatureState
{
	STOPPED,
	MOVING,
	ATTACKING
};

enum AiEvents
{
	EVENT_ENTERCOMBAT,
	EVENT_LEAVECOMBAT,
	EVENT_DAMAGETAKEN,
	EVENT_FEAR,
	EVENT_UNFEAR,
	EVENT_FOLLOWOWNER,
	EVENT_OWNERATTACKED,
	EVENT_WANDER,
	EVENT_UNWANDER,
	EVENT_TAUNTSTART,
	EVENT_TAUNTEND,
	EVENT_UNITDIED,
};
enum AIMovementFlags
{
	AIMOVESTATE_CANTMOVE,
	AIMOVESTATE_WALK,
	AIMOVESTATE_RUN,
	AIMOVESTATE_FLY,
	AIMOVESTATE_SPRINT
};

#define	DISABLE_COMBAT 0x01
#define DISABLE_MELEE 0x02
#define DISABLE_RANGED 0x04
#define DISABLE_SPELLS 0x08
#define DISABLE_TARGETTING 0x10

#define AIPANICK_HPMASK 0x1F // [0001,1111]
#define AIPANIC_HASPANICKED 0x20
#define AICALLHELP_HPMASK 0x1F
#define AICALLHELP_HASCALLED 0x20

class AIInterface_Mgr
{
public:
	static AIInterface * CreateAIInterface(Unit * self, AIType type, Unit * owner);
	static AI_Spell * ConstructAISpell(Field *);
	static void AISpell_parseAttrStr(AI_Spell* ,const char *);
	static void AISpell_parseAttrToken(AI_Spell *, string &);
	static void AIRangedSpell_parseAttrToken(AI_Spell *, string &);
	static void AIMeleeSpell_parseAttrToken(AI_Spell*, string&);
};