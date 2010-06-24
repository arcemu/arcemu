#pragma once

struct SpellEntry;
enum Field_AIAGENT
{
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

enum AI_SpellType
{
	STYPE_NULL,
	STYPE_DEFAULT, //Regular spells that simply get cast on a target.
	/* Rest of these types can have special attributes */
	STYPE_AOE,
	STYPE_HEAL,
	STYPE_AOE_HEAL,
	STYPE_DISPEL,
	STYPE_SUMMON,
	STYPE_SUMMON_DEST,
	STYPE_BUFF,
};

enum AI_SpellTargetType
{
	TTYPE_NULL,
	TTYPE_SELF,
	TTYPE_OWNER,
	TTYPE_CURRENTTARGET,
	TTYPE_RANDOMTARGET,
	TTYPE_RANDOM_ASSISTTARGET,
	TTYPE_RANDOMFRIENDLY,
	TTYPE_FORMATION_HEAD,
	TTYPE_SELF_DESTINATION,
	TTYPE_CURRENTTARGET_DESTINATION,
	TTYPE_RANDOMTARGET_DESTINATION,
	TTYPE_RANDOMFRIENDLY_DESTINATION,
	TTYPE_RANDOM_ASSISTTARGET_DESTINATION,
	TTYPE_FORMATION_HEAD_DESTINATION
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
struct AISpell_RandomData
{
	uint32 interval[2];
	uint16 facing[2];
	uint16 radius[2];
	AISpell_RandomData() {
		memset(this,0,sizeof(AISpell_RandomData) );
	}
};
struct AISpell_InteractionData
{
	uint32 soundId;
	uint16 chat_type;
	uint16 lang_type;
	const char * sendtxt;
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
struct AI_Spell
{
	SpellEntry * sEntry;
	int8 difficulty;
	uint8 event_type;
	uint8 spell_agent;
	uint8 autocast_type;
	uint8 type;
	uint8 spellTargeType;
	uint8 procChance;
	int8 maxcastcount;
	int8 currentcastcount;
	uint8 pad[3];
	float minrange;
	float maxrange;
	uint32 cooldown;
	uint32 interval;
	AISpell_RandomData * rdata;
	AISpell_InteractionData * interact;
	virtual void Construct() 
	{
		memset(this,0,sizeof(AI_Spell));
		type = AISPELLTYPE_SINGLETARGET;
	}
	virtual ~AI_Spell() 
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
/* AI Heal Spell Flags */
#define AI_HEALFLAG_EXTRATARGETS_FRIENDLY 0x1
#define AI_AOEHEALFLAG_ENFORCE_THRESHOLD 0x4 

struct AI_HealSpell : public AI_Spell
{
	uint32 heal_hp;
	virtual void Construct() 
	{
		memset(this,0,sizeof(AI_HealSpell) );
		type = AISPELLTYPE_HEAL;
	}
	
};
struct AI_AOEHealSpell : public AI_HealSpell
{
	uint16 heal_radius;
	uint16 facing;
	void Construct()
	{
		memset(this,0,sizeof(AI_AOEHealSpell));
		type = AISPELLTYPE_AOE_HEAL;
	}
};
/* AI Summon Flags */
#define AI_SUMMONFLAG_SUMMONENTRY_IS_SUMMONSPELL 0x1
#define AI_SUMMONFLAG_SUMMONTYPE_CREATURE 0x2
#define AI_SUMMONFLAG_SUMMONTYPE_GAMEOBJECT 0x3
struct AI_SummonSpell : public AI_Spell
{
	uint32 summon_entry;	//Can store the id of the creature/go OR it can be a summoning spell id.
	uint8 summon_type;		//Specifies some summon flags.
	uint8 summon_count;		//How many times to summon.
	uint8 summon_radius;	
	uint8 summon_faction;	//The faction to set our summon, 0 for default.
	uint16 delta_angle;		//Starting with our starting_angle, for 1...summon_count, we add delta_angle, and spawn the creature at that orientation.
	uint16 facing;	// This is added to our current orientation to create a spawn vector.
	virtual void Construct()
	{
		memset(this,0,sizeof(AI_SummonSpell) );
		type = AISPELLTYPE_SUMMON;
	}
};
struct AI_SummonSpellDest : public AI_SummonSpell
{
	size_t vectorCount;	//How many locations do we have.
	LocationVector * dest;	//Pointer to our vectors.
	void Construct()
	{
		memset(this,0,sizeof(AI_SummonSpellDest) );
		type = AISPELLTYPE_SUMMON_DEST;
	}
	~AI_SummonSpellDest() {
		if(dest != NULL)
			delete dest;
		dest = NULL;
	}
};
struct AI_DispelSpell : public AI_Spell
{
	uint32 dispel_Type;
	void Construct()
	{
		memset(this,0,sizeof(AI_DispelSpell) );
		type = AISPELLTYPE_DISPEL;
}	
};
struct AI_AOESpell : public AI_Spell
{
	uint16 facing;
	uint16 radius;
	void Construct()
	{
		memset(this,0,sizeof(AI_AOESpell) );
		type = AISPELLTYPE_AOE;
	}
};