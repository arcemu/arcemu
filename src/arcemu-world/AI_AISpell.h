#pragma once
#include "Chat.h"

struct SpellEntry;
enum Field_AIAGENT
{
	AIAGENTFIELD_IDFIELD,
	AIAGENTFIELD_OBJECTENTRY,
	AIAGENTFIELD_DIFFICULTY,
	AIAGENTFIELD_SPELLAGENT,
	AIAGENTFIELD_EVENTYPE,
	AIAGENTFIELD_PROCHANCE,
	AIAGENTFIELD_CASTCOUNT,
	AIAGENTFIELD_SPELLID,
	AIAGENTFIELD_SPELLTYPE,
	AIAGENTFIELD_SPELLTTYPE,
	AIAGENTFIELD_COOLDOWN,
	AIAGENTFIELD_ATTRIBUTES
};

enum AI_SpellTargetType
{
	TTYPE_NULL,
	TTYPE_SELF,	//ourselves.
	TTYPE_OWNER, //our owner
	TTYPE_CURRENTTARGET,	//our current target
	TTYPE_RANDOMTARGET,		//random enemy target
	TTYPE_RANDOM_ASSISTTARGET,	//random assist target	
	TTYPE_RANDOMFRIENDLY,	//random friendly target
	TTYPE_FORMATION_HEAD,	//our unit to follow
	TTYPE_SELF_DESTINATION,	//our current location vector.
	TTYPE_CURRENTTARGET_DESTINATION, //the current target's location vector.
	TTYPE_RANDOMTARGET_DESTINATION,	//a random enemy target's location vector.
	TTYPE_RANDOMFRIENDLY_DESTINATION,	// a random friendly target's location vector.
	TTYPE_RANDOM_ASSISTTARGET_DESTINATION,	// a random assist target's location vector.
	TTYPE_FORMATION_HEAD_DESTINATION	//our unit to follow's location vector.
};


//Internal types used for down casting.
enum AI_StructType
{
	AISPELLTYPE_SINGLETARGET,
	AISPELLTYPE_AOE,
	AISPELLTYPE_SUMMON,
	AISPELLTYPE_SUMMON_DEST,
	AISPELLTYPE_HEAL,
	AISPELLTYPE_AOE_HEAL,
	AISPELLTYPE_DISPEL,
	AISPELLTYPE_BUFF,
	AISPELLTYPE_DEBUFF,
};

//structure that stores any random fields spells might have.
struct AISpell_RandomData
{
	uint32 interval[2];	//random cooldown intervals
	uint16 facing[2];	//for aoe spells that want to use random orientations, in degrees not radians.
	uint16 radius[2];	//for aoe spells or summoning spells that use a radius value but may be random.
	AISpell_RandomData() {
		memset(this,0,sizeof(AISpell_RandomData) );
	}
};
//structure that stores interact data.
struct AISpell_InteractionData
{
	uint32 soundId;	//sound id of the file to play
	uint16 chat_type;	// the chat type
	uint16 lang_type;	// language id
	const char * sendtxt;	// text literal that will get printed. TODO- implement tokens.
	AISpell_InteractionData()
	{
		soundId = 0;
		//default chat options.
		chat_type = CHAT_MSG_MONSTER_SAY;
		lang_type = LANG_UNIVERSAL;
		sendtxt = NULL;
	}
	~AISpell_InteractionData() {
		if(sendtxt != NULL)
			free( (void*)sendtxt );
	}
};
#define AISPELL_ANY_DIFFICULTY -1
//structure that stores ai spell data.
struct AI_Spell
{
	SpellEntry * proto;	//the spell entry of our spell..
	uint32 cooldown;	//holds the time to the next cast, it holds ( getMSTime() + interval) for instance.
	uint32 interval;	//holds the cooldown btwn each cast, in millisecs
	uint8 event_type;	//stores an enum AiEvents when this spell will supposedly get cast.
	int8 difficulty;	//the instance type this spell will be used(normal,heroic,etc....)
	uint8 spell_agent;	//our spell agent either, melee/ranged/spell.. might implement more later for much more functionality.
	uint8 type;		//the spell type, uses enum AI_StructType
	uint8 spellTargeType;	//the target type of this spell uses enum AI_SpellTargetType
	uint8 procChance;	//chance this spell get cast, default and max is 100.
	uint8 maxcastcount;	// how many times the spell will get cast, -1 is unlimited max being 127.
	uint8 currentcastcount;	//this holds how many times the spell has been cast so far.
	bool isNotProtoSpell;	//Spells that were allocated somewhere and added, since proto doesn't keep reference to it, we need to deallocate when destruct.
	bool isQueued;	//Since, mobs now use a queue to store spells, a spell may get queued multiple times before the mob can cast it, this is to preven that.
	uint8 pad;	//some padding bytes to make the member offsets modulo 4.
	float minrange;	//min casting range of this spell.
	float maxrange;	//max casting range of this spell.
	AISpell_RandomData * rdata;	//a pointer to random data structure, null by default
	AISpell_InteractionData * interact;	// a pointer to interact data structure, null by default.
	AI_Spell()
	{
		isNotProtoSpell = false;
		isQueued = false;
	}
	virtual void Construct() 
	{
		memset(this,0,sizeof(AI_Spell));
		type = AISPELLTYPE_SINGLETARGET;
	}
	~AI_Spell() 
	{
		if(rdata != NULL)
		{
			delete rdata;
			rdata = NULL;
		}
		if(interact != NULL)
		{
			delete interact;
			interact = NULL;
		}
	}
};
//structure that holds pet spells, very simple.
struct AI_PetSpell
{
	void * bogusvtablePtr;	//structure compatibility w/ AI_Spell, needed since AI_Spell has a virtual method.
	SpellEntry * proto;	//the spell entry
	uint32 cooldown;	//holds the time to next cast.
	uint32 interval;	//holds how much time to wait btwn each cast.
	uint8 event_type;	//the enum AiEvent type.
	uint8 acquire_type;
	bool isQueued;
	uint8 pad;	//structure size is modulo 4.
	AI_PetSpell() {
		isQueued = false;
		memset(this, 0, sizeof(AI_PetSpell) );
	}
};

/* AI Heal Spell Flags, un-used atm. */
#define AI_HEALFLAG_EXTRATARGETS_FRIENDLY 0x1
#define AI_AOEHEALFLAG_ENFORCE_THRESHOLD 0x4 

//heal spell structure
struct AI_HealSpell : public AI_Spell
{
	uint32 heal_hp;	//the healing pct when to heal. Max is 100, can't be 0(otherwise don't use this structure)
	virtual void Construct() 
	{
		memset(this,0,sizeof(AI_HealSpell) );
		type = AISPELLTYPE_HEAL;
	}
	
};
struct AI_AOETemplate
{
	uint16 radius;
	uint16 facing;	//in degrees;
};
//aoe healing spell structure
struct AI_AOEHealSpell : public AI_HealSpell, public AI_AOETemplate
{
	void Construct()
	{
		memset(this,0,sizeof(AI_AOEHealSpell));
		type = AISPELLTYPE_AOE_HEAL;
	}
};
/* AI Summon Flags */
#define AI_SUMMONTYPE_ISSPELL 0x1	//when the spell itself does the actual summoning(ie. SpellEffectSummon)
#define AI_SUMMONTYPE_CREATURE 0x2	// summon a creature.
#define AI_SUMMONFLAG_GAMEOBJECT 0x3	// summon a gameobject.
struct AI_SummonSpell : public AI_Spell, public AI_AOETemplate
{
	uint32 summon_entry;	//Can store the id of the creature/go OR it can be a summoning spell id.
	uint8 summon_type;		//Specifies the summon type.
	uint8 summon_count;		//How many times to summon.
	uint8 summon_faction;	//The faction to set our summon, 0 for default.
	uint8 delta_angle;		//Starting with our starting_angle, for 1...summon_count, we add delta_angle, and spawn the creature at that orientation.
	virtual void Construct()
	{
		memset(this,0,sizeof(AI_SummonSpell) );
		type = AISPELLTYPE_SUMMON;
		summon_count = 1;
	}
};
//structure holding summon spells on a specific coordinate.
struct AI_SummonSpellDest : public AI_SummonSpell
{
	size_t vectorCount;	//How many locations do we have.
	LocationVector * dest;	//Pointer to our vectors.
	void Construct()
	{
		memset(this,0,sizeof(AI_SummonSpellDest) );
		type = AISPELLTYPE_SUMMON_DEST;
		summon_count = 1;
	}
	~AI_SummonSpellDest() 
	{
		if(dest != NULL)
			delete dest;
		dest = NULL;
	}
};
//structure holding dispel spell types
struct AI_DispelSpell : public AI_Spell
{
	int dispel_Type;	//holds the dispel type of the spell. the unit checks for auras with this dispel type before casting.
	void Construct()
	{
		memset(this,0,sizeof(AI_DispelSpell) );
		type = AISPELLTYPE_DISPEL;
	}	
};
//structure that holds aoe spell types.
struct AI_AOESpell : public AI_Spell, public AI_AOETemplate
{
	void Construct()
	{
		memset(this,0,sizeof(AI_AOESpell) );
		type = AISPELLTYPE_AOE;
	}
};