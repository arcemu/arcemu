/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008 <http://www.ArcEmu.org/>
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

#ifndef WOWSERVER_ITEM_H
#define WOWSERVER_ITEM_H

#include "Skill.h"

struct EnchantmentInstance
{
	EnchantEntry * Enchantment;
	bool BonusApplied;
	uint32 Slot;
	time_t ApplyTime;
	uint32 Duration;
	bool RemoveAtLogout;
	uint32 RandomSuffix;
};

const static ItemProf prof[22] = { {4, 2}, {4, 4}, {4, 8}, {4, 16}, {4, 64},
								   {2, 1}, {2, 2}, {2, 4}, {2, 8}, {2, 16}, {2, 32}, {2, 64}, {2, 128}, {2, 256}, {2, 1024}, {2, 8192}, {2, 32768}, {2, 65536}, {2, 131072},
								   {2, 262144}, {2, 524288}, {2, 1048576} };

const static uint32 arm_skills[7] = { 0,
									  SKILL_CLOTH,
									  SKILL_LEATHER,
									  SKILL_MAIL,
									  SKILL_PLATE_MAIL,
									  0,
									  SKILL_SHIELD };

const static uint32 weap_skills[21] = {	SKILL_AXES,
										SKILL_2H_AXES,
										SKILL_BOWS,
										SKILL_GUNS,
										SKILL_MACES,
										SKILL_2H_MACES,
										SKILL_POLEARMS,
										SKILL_SWORDS,
										SKILL_2H_SWORDS,
										0,
										SKILL_STAVES,
										0,
										0,
										SKILL_FIST_WEAPONS,
										0,//<-13
										SKILL_DAGGERS,
										SKILL_THROWN,
										SKILL_SPEARS,
										SKILL_CROSSBOWS,
										SKILL_WANDS,
										SKILL_FISHING };
                                      
const static float pricemod[9] = { 1.0f, // HATED
								   1.0f, // HOSTILE
								   1.0f, // UNFRIENDLY
								   1.0f, // NEUTRAL
								   0.95f, // FRIENDLY
								   0.90f, // HONORED
								   0.85f, // REVERED
								   0.80f }; // EXHALTED

const static double SuffixMods[NUM_INVENTORY_TYPES] = { 0.0,
														0.46,		// HEAD
														0.26,		// NECK
														0.35,		// SHOULDERS
														0.46,		// BODY
														0.46,		// CHEST
														0.35,		// WAIST
														0.46,		// LEGS
														0.34,		// FEET
														0.26,		// WRISTS
														0.35,		// HANDS
														0.26,		// FINGER
														0.0,		// TRINKET
														0.19,		// WEAPON
														0.25,		// SHEILD
														0.14,		// RANGED
														0.26,		// CLOAK
														0.46,		// 2H-WEAPON
														0.0,		// BAG
														0.0,		// TABARD
														0.46,		// ROBE
														0.19,		// MAIN-HAND WEAPON
														0.19,		// OFF-HAND WEAPON
														0.26,		// HOLDABLE
														0.0,		// AMMO
														0.26,		// THROWN
														0.14,		// RANGED
														0.0,		// QUIVER
														0.26, };	// RELIC

typedef std::map<uint32, EnchantmentInstance> EnchantmentMap;

#define APPLY true
#define REMOVE false

#define RANDOM_SUFFIX_MAGIC_CALCULATION( __suffix, __scale ) int32( float( ( float( ( __suffix ) ) * float( ( __scale ) ) ) ) / 10000.0f );

class SERVER_DECL Item : public Object
{
public:
	Item();
	void Init( uint32 high, uint32 low );
	void Virtual_Constructor();		//when using object pool contructor is not good to be called again sometimes. Use this instead
	virtual ~Item();
	void Virtual_Destructor();		//this makes sure we do not leave events on objects that are supposed to be deleted
	int32 m_bufferPoolId;

	void Create( uint32 itemid, Player* owner );

	ARCEMU_INLINE ItemPrototype* GetProto() const { return m_itemProto; }
	ARCEMU_INLINE void SetProto( ItemPrototype* pr ) { m_itemProto = pr; }

	ARCEMU_INLINE Player* GetOwner() const { return m_owner; }
	void SetOwner( Player* owner );

	ARCEMU_INLINE bool IsContainer(){ return ( m_objectTypeId == TYPEID_CONTAINER ) ? true : false; }
	
	//! DB Serialization
	void LoadFromDB( Field *fields, Player* plr, bool light );
	void SaveToDB( int8 containerslot, int8 slot, bool firstsave, QueryBuffer* buf );
	bool LoadAuctionItemFromDB( uint64 guid );
	void DeleteFromDB();
	void DeleteMe();
	
	ARCEMU_INLINE void SoulBind()
	{
		this->SetFlag( ITEM_FIELD_FLAGS, ITEM_FLAG_SOULBOUND );
	}

	ARCEMU_INLINE bool IsSoulbound()
	{
		return this->HasFlag( ITEM_FIELD_FLAGS, ITEM_FLAG_QUEST | ITEM_FLAG_SOULBOUND );
	}

	ARCEMU_INLINE uint32 GetChargesLeft()
	{
		for( uint32 x = 0; x < 5; x++ )
			if( m_itemProto->Spells[x].Id )
				return GetUInt32Value( ITEM_FIELD_SPELL_CHARGES + x );
		return 0;
	}

	ARCEMU_INLINE time_t GetEnchantmentApplytime( uint32 slot )
	{
		EnchantmentMap::iterator itr = Enchantments.find( slot );
		if( itr == Enchantments.end() )
			return 0;
		else
			return itr->second.ApplyTime;
	}

	//! Adds an enchantment to the item.
	int32 AddEnchantment( EnchantEntry* Enchantment, uint32 Duration, bool Perm = false, bool apply = true, bool RemoveAtLogout = false, uint32 Slot_ = 0, uint32 RandomSuffix = 0 );
	uint32 GetSocketsCount();

	//! Removes an enchantment from the item.
	void RemoveEnchantment( uint32 EnchantmentSlot );
	
	// Removes related temporary enchants
	void RemoveRelatedEnchants( EnchantEntry* newEnchant );

	//! Adds the bonus on an enchanted item.
	void ApplyEnchantmentBonus( uint32 Slot, bool Apply );

	//! Applies all enchantment bonuses (use on equip)
	void ApplyEnchantmentBonuses();

	//! Removes all enchantment bonuses (use on dequip)
	void RemoveEnchantmentBonuses();

	//! Event to remove an enchantment.
	void EventRemoveEnchantment( uint32 Slot );

	//! Check if we have an enchantment of this id?
	int32 HasEnchantment( uint32 Id );

	//! Check if we have an enchantment on that slot
	bool HasEnchantmentOnSlot( uint32 slot );

	//! Modify the time of an existing enchantment.
	void ModifyEnchantmentTime( uint32 Slot, uint32 Duration );

	//! Find free enchantment slot.
	int32 FindFreeEnchantSlot( EnchantEntry* Enchantment, uint32 random_type );

	//! Removes all enchantments.
	void RemoveAllEnchantments( bool OnlyTemporary );

	//! Sends SMSG_ITEM_UPDATE_ENCHANT_TIME
	void SendEnchantTimeUpdate( uint32 Slot, uint32 Duration );

	//! Applies any random properties the item has.
	void ApplyRandomProperties( bool apply );

	void RemoveProfessionEnchant();
	void RemoveSocketBonusEnchant();

	// gets the itemlink for a message to the player
	string GetItemLink(uint32 language);

	ARCEMU_INLINE void SetCount( uint32 amt ) { SetUInt32Value( ITEM_FIELD_STACK_COUNT, amt ); }
	ARCEMU_INLINE void SetDurability( uint32 Value ) { SetUInt32Value(ITEM_FIELD_DURABILITY, Value ); };
	ARCEMU_INLINE void SetDurabilityToMax() { SetUInt32Value( ITEM_FIELD_DURABILITY, GetUInt32Value( ITEM_FIELD_MAXDURABILITY ) ); }
	ARCEMU_INLINE uint32 GetDurability() { return GetUInt32Value( ITEM_FIELD_DURABILITY ); }
	ARCEMU_INLINE uint32 GetDurabilityMax() { return GetUInt32Value( ITEM_FIELD_MAXDURABILITY ); }
	ARCEMU_INLINE bool IsAmmoBag() { return (m_itemProto->Class == ITEM_CLASS_QUIVER); }

	void RemoveFromWorld();

	Loot* loot;
	bool locked;
	bool m_isDirty;

	EnchantmentInstance* GetEnchantment( uint32 slot );
	bool IsGemRelated( EnchantEntry* Enchantment );

	ARCEMU_INLINE uint32 GetItemRandomPropertyId() const { return m_uint32Values[ITEM_FIELD_RANDOM_PROPERTIES_ID]; }
	ARCEMU_INLINE uint32 GetItemRandomSuffixFactor() { return m_uint32Values[ITEM_FIELD_PROPERTY_SEED]; }
	static uint32 GenerateRandomSuffixFactor( ItemPrototype* m_itemProto );

	ARCEMU_INLINE void SetRandomProperty( uint32 id )
	{
		SetUInt32Value( ITEM_FIELD_RANDOM_PROPERTIES_ID, id );
		random_prop = id;
	}

	ARCEMU_INLINE void SetRandomSuffix( uint32 id )
	{
		int32 r_id = -(int32(id));
		uint32 v = Item::GenerateRandomSuffixFactor( m_itemProto );
		SetUInt32Value( ITEM_FIELD_RANDOM_PROPERTIES_ID, (uint32)r_id );
		SetUInt32Value( ITEM_FIELD_PROPERTY_SEED, v );
		random_suffix = id;
	}

	bool HasEnchantments() { return (Enchantments.size() > 0 ) ? true : false; }

	uint32 wrapped_item_id;

protected:

	ItemPrototype* m_itemProto;
	EnchantmentMap Enchantments;
	uint32 _fields[ITEM_END];//this mem is wasted in case of container... but this will be fixed in future
	Player* m_owner; // let's not bother the manager with unneeded requests
	uint32 random_prop;
	uint32 random_suffix;
};

uint32 GetSkillByProto( uint32, uint32 );

uint32 GetSellPriceForItem( ItemPrototype* proto, uint32 count );
uint32 GetBuyPriceForItem( ItemPrototype* proto, uint32 count, Player* plr, Creature* vendor );

uint32 GetSellPriceForItem( uint32 itemid, uint32 count);
uint32 GetBuyPriceForItem( uint32 itemid, uint32 count, Player* plr, Creature* vendor );

string GetItemLinkByProto(ItemPrototype * iProto, uint32 language);

#endif
