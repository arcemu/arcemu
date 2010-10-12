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

#ifndef _PET_H
#define _PET_H

/* Taken from ItemPetFood.dbc
 * Each value is equal to a flag
 * so 1 << PET_FOOD_BREAD for example
 * will result in a usable value.
 */
enum PET_FOOD
{
	PET_FOOD_NONE,
	PET_FOOD_MEAT,
	PET_FOOD_FISH,
	PET_FOOD_CHEESE,
	PET_FOOD_BREAD,
	PET_FOOD_FUNGUS,
	PET_FOOD_FRUIT,
	PET_FOOD_RAW_MEAT, // not used in pet diet
	PET_FOOD_RAW_FISH  // not used in pet diet
};

enum PetActionFeedback
{
	PET_FEEDBACK_NONE,
	PET_FEEDBACK_PET_DEAD,
	PET_FEEDBACK_NOTHING_TO_ATTACK,
	PET_FEEDBACK_CANT_ATTACK_TARGET
};

enum PET_RENAME
{
	PET_RENAME_NOT_ALLOWED	= 0x02,
	PET_RENAME_ALLOWED		= 0x03
};

enum StableState
{
	STABLE_STATE_ACTIVE		= 1,
	STABLE_STATE_PASSIVE	= 2
};
enum HappinessState
{
	UNHAPPY		= 1,
	CONTENT		= 2,
	HAPPY		= 3
};

#define PET_TALENT_TREE_START	409	// Tenacity
#define PET_TALENT_TREE_END		411	// Cunning

#define PET_DELAYED_REMOVAL_TIME 60000  // 1 min




enum PetType
{
	HUNTERPET = 1,
	WARLOCKPET = 2,
};

typedef map<SpellEntry*, uint16> PetSpellMap;
struct PlayerPet;

class SERVER_DECL Pet : public Creature
{
	friend class Player;
	friend class Creature;
	friend class WorldSession;

public:
	Pet(uint64 guid);
	~Pet();

	// Override superclass method that returns false
    bool IsPet() { return true; }

	void LoadFromDB(Player* owner, PlayerPet * pi);

	bool CreateAsSummon(uint32 entry, CreatureInfo *ci, Creature *created_from_creature, Player* owner, SpellEntry *created_by_spell, uint32 type, uint32 expiretime, LocationVector* Vec = NULL, bool dismiss_old_pet = true);

	virtual void Update(uint32 time);
	void OnPushToWorld();

	void InitializeMe(bool first);
	void SendSpellsToOwner();
	void SendNullSpellsToOwner();
	void SendCastFailed( uint32 spellid, uint8 fail );
	void SendActionFeedback( PetActionFeedback value  );

	ARCEMU_INLINE void SetPetDiet(uint32 diet) { m_Diet = diet; }
	ARCEMU_INLINE void SetPetDiet() { m_Diet = myFamily->petdietflags; }
	ARCEMU_INLINE uint32 GetPetDiet(void) { return m_Diet; }

	void UpdatePetInfo(bool bSetToOffline);
	void Remove( bool bUpdate, bool bSetOffline );
	void Dismiss();
	void setDeathState(DeathState s);

	void PrepareForRemove(bool bUpdate, bool bSetOffline);
	void RemoveFromWorld(bool free_guid);
	void OnRemoveFromWorld();
	void DelayedRemove(bool bTime, bool dismiss = false, uint32 delay = PET_DELAYED_REMOVAL_TIME);
	void Despawn(uint32 delay, uint32 respawntime);
	void SafeDelete();

	ARCEMU_INLINE Player* GetPetOwner() { return m_Owner; }
	ARCEMU_INLINE void ClearPetOwner() { m_Owner = NULL; }
	
	/*	Returns the current pet exp. */
	ARCEMU_INLINE uint32 getPetXP() { return GetUInt32Value(UNIT_FIELD_PETEXPERIENCE); }
	/*	Sets our current exp */
	ARCEMU_INLINE void setPetXP(uint32 xp) { SetUInt32Value(UNIT_FIELD_PETEXPERIENCE, xp); }
	/*	Gets the exp to the next level. */
	ARCEMU_INLINE uint32 getPetNextLvlXP() { return GetUInt32Value(UNIT_FIELD_PETNEXTLEVELEXP); }
	/*	Sets exp to the next level. */
	ARCEMU_INLINE void setPetNextLvlXP(uint32 xp) { SetUInt32Value(UNIT_FIELD_PETNEXTLEVELEXP, xp); }

	bool CanGainXP();
	void GiveXP( uint32 xp );
	uint32 calcNextLvlXP(uint32 currentlevel);
	void ApplyStatsForLevel();
	void ApplySummonLevelAbilities();
	void ApplyPetLevelAbilities();
	void UpdateAP();
	void LoadPetAuras(int32 id);
	void setDefaultActionbar();
	void SetActionBarSlot(uint32 slot, uint32 spell){ ActionBar[ slot ] = spell; }
	
	//////////////////////////////////////////////////////////////////////////
	//-----------------PET SPELL MUTATORS-------------------//
	//////////////////////////////////////////////////////////////////////////

	//************************************
	// Purpose:	Adds the specified ai pet spell to the ai autocast list.
	// Parameter: AI_PetSpell *	spell - a pointer to a ai pet spell to add.
	// Parameter: bool learning - when true if this spell is a successer to a previous action bar spell or the action bar has extar slots, it gets placed there.
	// Parameter: bool showLearnSpell - when true, it sends a pet learnt spell packet.
	//************************************
	void Spell_add(AI_PetSpell* spell, bool learning, bool showLearnSpell = true);

	//************************************
	// Purpose:	Returns true if we have the pet spell specified in either autocast list or talent list, false otherwise
	// Parameter: uint32 SpellID - a valid spell id
	//************************************
	bool Spell_has( uint32 SpellID );

	//************************************
	// Purpose:	Removes the specified spell_id from either the talent list or autocast list. If it's a talent, then it also removes the aura from the unit.
	// Parameter: uint32 spell_id - a valid spell id.
	// Parameter: bool showUnlearnSpell - if true, it sends a pet unlearnt packet.
	//************************************
	void Spell_remove(uint32 spell_id, bool showUnlearnSpell = true);

	//////////////////////////////////////////////////////////////////////////
	//-----------------PET TALENT MUTATORS-------------------//
	//////////////////////////////////////////////////////////////////////////

	//************************************
	// Purpose: Adds a new talent to our list. If the spell is a passive, it casts on itself, 
	//			otherwise it adds an AI_PetSpell represented by this id to the pets ai auto cast list.
	// Parameter: uint32 spell - a valid spell id to add.
	// Parameter: bool show - if this is true, the pet owner sends an smsg pet learnt spell packet to show the learnt spell.
	//************************************/
	void Talent_add(uint32 spell, bool show = false);

	//************************************
	// Purpose:	Removes the id from the internal list.If  the spell is a passive spell, it removes the aura from the unit,
	//			otherwise it removes the spell from the ai autocast list.
	// Parameter: uint32 spell - a valid spell id to remove.
	// Parameter: bool show	- if this is true, it sends a pet unlearned packet.
	//************************************
	void Talent_remove(uint32 spell, bool show = false);

	//************************************
	// Purpose:	Returns true if we have the specified spell in our talent list.
	// Parameter: uint32 spell - a valid spell id to check.
	//************************************
	bool Talent_has(uint32 spell);

	//************************************
	// Purpose:	Clears our talent list of all entries. For every entry, if it is a passive spell, we remove the aura from the unit,
	//			otherwise, we remove the spell from our ai autocast list.
	//************************************
	void Talent_wipe();

	//************************************
	// Purpose:	Returns a reference to the internal talent list.
	//************************************
	std::set<uint32> & Talent_getAll() { return m_talentSpells; }
	
	uint32 getUntrainCost();
	ARCEMU_INLINE bool IsSummon() { return Summon; }

	void Rename(string NewName);
	ARCEMU_INLINE string& GetName() { return m_name; }
	void UpdateSpellList( bool showLearnSpells = true );

	// talents
	void SendTalentsToOwner();		// Send talentpoints and talent spells to owner
	ARCEMU_INLINE uint8 GetTPsForLevel( uint32 level ) { return ( level >= 20 ) ? uint8( level - 16 ) >> 2 : 0; }	// pet gain first talent point at lvl 20, then every 4 lvls another point
	ARCEMU_INLINE void SetTPs( uint8 TP ) { SetByte( UNIT_FIELD_BYTES_1, 1, TP ); }			// sets talent points
	ARCEMU_INLINE uint8 GetTPs() { return GetByte( UNIT_FIELD_BYTES_1, 1 ); }				// returns available talent points
	ARCEMU_INLINE uint8 GetSpentTPs() { return GetTPsForLevel( getLevel() ) - GetTPs(); }	// returns amount of spent talent points

	float GetHappinessDmgMod() { return 0.25f * GetHappinessState() + 0.5f; };
	bool IsBeingDeleted(){ return ScheduledForDeletion; }

	virtual Group *GetGroup();

	void DealDamage(Unit *pVictim, uint32 damage, uint32 targetEvent, uint32 unitEvent, uint32 spellId, bool no_remove_auras = false);
	void TakeDamage(Unit *pAttacker, uint32 damage, uint32 spellid, bool no_remove_auras = false );
	void Die( Unit *pAttacker, uint32 damage, uint32 spellid );

protected:
	
	Player * m_Owner;	//our owner, only players can be pet owners, creature owners have guardians.
	PlayerPet * mPi;
	uint32 ActionBar[10];   // 10 slots
	std::set<uint32> m_talentSpells;	//any talent auras we store them here.

	uint32 m_HappinessTimer;
	uint32 m_PetNumber;
	uint32 m_ExpireTime;
	uint32 m_Diet;
	time_t reset_time;
	uint32 reset_cost;
	bool bExpires;
	bool Summon;
	bool ScheduledForDeletion;
	string m_name;
	HappinessState GetHappinessState();
	void SetNameForEntry( uint32 entry );
};

#define PET_HAPPINESS_UPDATE_VALUE 333000
#define PET_HAPPINESS_UPDATE_TIMER 7500

#define PET_ACTION_ACTION   0x700
#define PET_ACTION_STATE	0x600

//TODO: grep see the way pet spells contain the same flag?
#define PET_ACTION_SPELL	0xC100
#define PET_ACTION_SPELL_1  0x8100
#define PET_ACTION_SPELL_2  0x0100
#endif
