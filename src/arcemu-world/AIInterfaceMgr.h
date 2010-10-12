#pragma  once
#include "StdAfx.h"
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
	MOVEMENTTYPE_MISC,
	MOVEMENTTYPE_RANDOMWP,
	MOVEMENTTYPE_CIRCLEWP,
	MOVEMENTTYPE_WANTEDWP,
	MOVEMENTTYPE_FORWARDTHENSTOP,
	MOVEMENTTYPE_DONTMOVEWP,
	WAYPOINT_MOVEBACKWARD = 0x08,
	WAYPOINT_ISSHOWING = 0x10,
	WAYPOINT_MOVETYPEMASK =  0x07 // [0000,0111] 
};

enum AI_State
{
	STATE_IDLE,
	STATE_ATTACKING,
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
	EVENT_NONE,
	EVENT_ENTERWORLD, // on push to world
	EVENT_ENTERCOMBAT,	//fired when we enter combat
	EVENT_INCOMBAT,	//never fired, spells with this event are cast while the unit is in combat.
	EVENT_OUTCOMBAT, //never fired, spells with this event are cast while the unit is out of combat.
	EVENT_LEAVECOMBAT,	//fired when we leave combat
	EVENT_DAMAGETAKEN,	//fired when taking damage
	EVENT_FEAR,	//fired when a fear spell is cast on us.
	EVENT_UNFEAR,	//fired when the fear spell duration is over.
	EVENT_FOLLOWOWNER,	//fired to make the unit follow it's master.
	EVENT_OWNERATTACKED,	//fired when a unit's owner is attacked.
	EVENT_WANDER,	//fired when a disorient spell is cast on us.
	EVENT_UNWANDER,	//fired when the disorient spell duration ends.
	EVENT_TAUNTSTART,	//fired when taunt spell is cast on us.
	EVENT_TAUNTEND,	//fired when taunt spell duration ends.
	EVENT_UNITDIED,	//fired when then unit dies
};

enum AIMovementFlags
{
	AIMOVESTATE_CANTMOVE = 0xF0, //if upper nibble is set to block movement.
	AIMOVESTATE_WALK = 0x01,
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
	static AIInterface * CreateInterface(Unit * self, AIType type, Unit * owner);
	static void ConstructAISpell(Field *);
	static AI_PetSpell * ConstructPetSpell(uint32);
	static void AISpell_parseAttrStr(AI_Spell* ,const char *);
	static void AICasterSpell_parseAttrToken(AI_Spell *, string &);
	static void AIRangedSpell_parseAttrToken(AI_Spell *, string &);
	static void AIMeleeSpell_parseAttrToken(AI_Spell*, string&);

};