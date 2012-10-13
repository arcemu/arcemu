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

#include "StdAfx.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// float CalcHPCoefficient( MapInfo *mi, uint32 mode, bool boss )
//  Returns the HP coefficient that is suited for the map, mode, and creature
//
// Parameters:
//  MapInfo *mi		-		pointer to the mapinfo structure
//	uint32  mode	-		numeric representation of the version of the map (normal, heroic, 10-25 men, etc )
//	bool	boss	-		true if the creature is a boss, false if not
//
// Return Values:
//	Returns the hp coefficient in a float
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float CalcHPCoefficient(MapInfo* mi, uint32 mode, bool boss)
{
	float coeff = 1.0f;

	if(mi == NULL)
		return 1.0f;

	// This calculation is purely speculation as we have no way of finding out how Blizzard generates these values
	// These cases are based on simple observation of trash/boss hp values for different modes
	// If you know they are wrong AND you know a better calculation formula then DO change it.

	// raid
	if(mi->type == INSTANCE_RAID)
	{
		bool hasheroic = false;

		// check if we have heroic mode avaiable
		if(mi->HasFlag(WMI_INSTANCE_HAS_HEROIC_10MEN) && mi->HasFlag(WMI_INSTANCE_HAS_HEROIC_25MEN))
			hasheroic = true;

		// boss hp coeff calculations
		if(boss == true)
		{

			switch(mode)
			{
				case MODE_NORMAL_10MEN:
					coeff = 1.0f;
					break;

				case MODE_HEROIC_10MEN:
					coeff = 1.25f;
					break;

				case MODE_NORMAL_25MEN:
					if(hasheroic)
						coeff = 5.0f;
					else
						coeff = 3.0f;
					break;

				case MODE_HEROIC_25MEN:
					coeff = 5.0f * 1.25f;
			}

			// trash hp coeff calculation
		}
		else
		{
			switch(mode)
			{
				case MODE_NORMAL_10MEN:
					coeff = 1.0f;
					break;

				case MODE_HEROIC_10MEN:
					coeff = 1.5f;
					break;

				case MODE_NORMAL_25MEN:
					coeff = 2.0f;
					break;

				case MODE_HEROIC_25MEN:
					coeff = 2.5f;
					break;
			}
		}
	}

	// heroic dungeon
	if(mi->type == INSTANCE_MULTIMODE)
	{

		if(mode > 0)
			coeff = 1.5f;
		else
			coeff = 1.0f;
	}

	return coeff;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// float CalcDMGCoefficient( MapInfo *mi, uint32 mode )
//  Calculates the creature damage coefficient that is suitable for the map type and difficulty
//
// Parameters:
//  MapInfo *mi		-		pointer to the MapInfo structure
//  uint32 mode		-		numeric representation of the version of the map (normal, heroic, 10-25 men, etc )
//
// Return Value:
//  Returns the suitable damage coefficient in a float
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
float CalcDMGCoefficient(MapInfo* mi, uint32 mode)
{

	// This calculation is purely speculation as we have no way of finding out how Blizzard generates these values
	// These cases are based on simple observation of trash/boss damage values for different modes
	// If you know they are wrong AND you know a better calculation formula then DO change it.

	if(mi == NULL)
		return 1.0f;

	switch(mode)
	{
		case MODE_NORMAL_10MEN:
			return 1.0f;
			break;

		case MODE_NORMAL_25MEN:
			if(mi->type == INSTANCE_MULTIMODE)
				return 1.5f;
			else
				return 2.0;

			break;

		case MODE_HEROIC_10MEN:
			return 1.5f;
			break;

		case MODE_HEROIC_25MEN:
			return 2.5f;
			break;
	}

	return 1.0f;
}

Creature::Creature(uint64 guid)
{
	m_valuesCount = UNIT_END;
	m_objectTypeId = TYPEID_UNIT;
	m_uint32Values = _fields;
	memset(m_uint32Values, 0, (UNIT_END)*sizeof(uint32));
	m_updateMask.SetCount(UNIT_END);
	SetUInt32Value(OBJECT_FIELD_TYPE, TYPE_UNIT | TYPE_OBJECT);
	SetGUID(guid);
	m_wowGuid.Init(GetGUID());


	m_quests = NULL;
	proto = NULL;
	spawnid = 0;

	creature_info = NULL;
	m_H_regenTimer = 0;
	m_P_regenTimer = 0;
	m_useAI = true;
	mTaxiNode = 0;

	Skinned = false;
	m_enslaveCount = 0;
	m_enslaveSpell = 0;

	for(uint32 x = 0; x < 7; x++)
	{
		FlatResistanceMod[x] = 0;
		BaseResistanceModPct[x] = 0;
		ResistanceModPct[x] = 0;
		ModDamageDone[x] = 0;
		ModDamageDonePct[x] = 1.0;
	}

	for(uint32 x = 0; x < 5; x++)
	{
		TotalStatModPct[x] = 0;
		StatModPct[x] = 0;
		FlatStatMod[x] = 0;
	}

	m_PickPocketed = false;
	m_SellItems = NULL;
	_myScriptClass = NULL;
	myFamily = 0;

	loot.gold = 0;
	haslinkupevent = false;
	original_emotestate = 0;
	mTrainer = 0;
	m_spawn = 0;
	auctionHouse = 0;
	SetAttackPowerMultiplier(0.0f);
	SetRangedAttackPowerMultiplier(0.0f);
	m_custom_waypoint_map = 0;
	m_escorter = NULL;
	m_limbostate = false;
	m_corpseEvent = false;
	m_respawnCell = NULL;
	m_walkSpeed = 2.5f;
	m_runSpeed = MONSTER_NORMAL_RUN_SPEED;
	m_base_runSpeed = m_runSpeed;
	m_base_walkSpeed = m_walkSpeed;
	m_noRespawn = false;
	m_respawnTimeOverride = 0;
	m_canRegenerateHP = true;
	BaseAttackType = SCHOOL_NORMAL;
	m_healthfromspell = 0;
	m_speedFromHaste = 0;
	m_Creature_type = 0;
}


Creature::~Creature()
{
	sEventMgr.RemoveEvents(this);

	if(_myScriptClass != NULL)
	{
		_myScriptClass->Destroy();
		_myScriptClass = NULL;
	}

	if(m_custom_waypoint_map != NULL)
	{
		GetAIInterface()->SetWaypointMap(NULL);
		m_custom_waypoint_map = NULL;
	}
	if(m_respawnCell != NULL)
		m_respawnCell->_respawnObjects.erase(this);

	if(m_escorter != NULL)
		m_escorter = NULL;

	// Creature::PrepareForRemove() nullifies m_owner. If m_owner is not NULL then the Creature wasn't removed from world
	//but it got a reference to m_owner
}

void Creature::Update(uint32 p_time)
{
	Unit::Update(p_time);

	if(m_corpseEvent)
	{
		sEventMgr.RemoveEvents(this);
		if(this->creature_info->Rank == ELITE_WORLDBOSS)
			sEventMgr.AddEvent(this, &Creature::OnRemoveCorpse, EVENT_CREATURE_REMOVE_CORPSE, TIME_CREATURE_REMOVE_BOSSCORPSE, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		else if(this->creature_info->Rank == ELITE_RAREELITE || this->creature_info->Rank == ELITE_RARE)
			sEventMgr.AddEvent(this, &Creature::OnRemoveCorpse, EVENT_CREATURE_REMOVE_CORPSE, TIME_CREATURE_REMOVE_RARECORPSE, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		else
			sEventMgr.AddEvent(this, &Creature::OnRemoveCorpse, EVENT_CREATURE_REMOVE_CORPSE, TIME_CREATURE_REMOVE_CORPSE, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

		m_corpseEvent = false;
	}
}

void Creature::SafeDelete()
{
	sEventMgr.RemoveEvents(this);

	delete this;
}

void Creature::DeleteMe()
{
	if(IsInWorld())
		RemoveFromWorld(false, true);
	else
		SafeDelete();
}

void Creature::OnRemoveCorpse()
{
	// time to respawn!
	if(IsInWorld() && (int32)m_mapMgr->GetInstanceID() == m_instanceId)
	{

		LOG_DETAIL("Removing corpse of " I64FMT "...", GetGUID());

		setDeathState(DEAD);
		m_position = m_spawnLocation;

		if((GetMapMgr()->GetMapInfo() && GetMapMgr()->GetMapInfo()->type == INSTANCE_RAID && proto->boss) || m_noRespawn)
		{
			RemoveFromWorld(false, true);
		}
		else
		{
			if(proto->RespawnTime || m_respawnTimeOverride)
				RemoveFromWorld(true, false);
			else
				RemoveFromWorld(false, true);
		}
	}
	else
	{
		// if we got here it's pretty bad
		ARCEMU_ASSERT(false);
	}
}

void Creature::OnRespawn(MapMgr* m)
{
	if(m_noRespawn)
		return;

	InstanceBossInfoMap* bossInfoMap = objmgr.m_InstanceBossInfoMap[m->GetMapId()];
	Instance* pInstance = m->pInstance;
	if(bossInfoMap != NULL && pInstance != NULL)
	{
		bool skip = false;
		for(std::set<uint32>::iterator killedNpc = pInstance->m_killedNpcs.begin(); killedNpc != pInstance->m_killedNpcs.end(); ++killedNpc)
		{
			// Is killed boss?
			if((*killedNpc) == creature_info->Id)
			{
				skip = true;
				break;
			}
			// Is add from killed boss?
			InstanceBossInfoMap::const_iterator bossInfo = bossInfoMap->find((*killedNpc));
			if(bossInfo != bossInfoMap->end() && bossInfo->second->trash.find(this->spawnid) != bossInfo->second->trash.end())
			{
				skip = true;
				break;
			}
		}
		if(skip)
		{
			m_noRespawn = true;
			DeleteMe();
			return;
		}
	}

	LOG_DETAIL("Respawning " I64FMT "...", GetGUID());
	SetHealth(GetMaxHealth());
	SetUInt32Value(UNIT_DYNAMIC_FLAGS, 0); // not tagging shit
	if(m_spawn)
	{
		SetUInt32Value(UNIT_NPC_FLAGS, proto->NPCFLags);
		SetEmoteState(m_spawn->emote_state);

		/* creature's death state */
		if(m_spawn->death_state == CREATURE_STATE_APPEAR_DEAD)
		{
			m_limbostate = true;
			setDeathState(ALIVE);   // we are not actually dead, we just appear dead
			SetUInt32Value(UNIT_DYNAMIC_FLAGS, U_DYN_FLAG_DEAD);
		}
		else if(m_spawn->death_state == CREATURE_STATE_DEAD)
		{
			SetHealth(0);
			m_limbostate = true;
			setDeathState(CORPSE);
		}
		else
			setDeathState(ALIVE);
	}

	RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SKINNABLE);
	Skinned = false;
	Tagged = false;
	TaggerGuid = 0;

	//empty loot
	loot.items.clear();

	GetAIInterface()->StopMovement(0); // after respawn monster can move
	m_PickPocketed = false;
	PushToWorld(m);
}

void Creature::Create(const char* name, uint32 mapid, float x, float y, float z, float ang)
{
	Object::_Create(mapid, x, y, z, ang);
}

void Creature::CreateWayPoint(uint32 WayPointID, uint32 mapid, float x, float y, float z, float ang)
{
	Object::_Create(mapid, x, y, z, ang);
}

///////////
/// Looting

void Creature::generateLoot()
{

	if(isCritter())
		return;

	if(!loot.items.empty())
		return;

	if(m_mapMgr != NULL)
		lootmgr.FillCreatureLoot(&loot, GetEntry(), m_mapMgr->iInstanceMode);
	else
		lootmgr.FillCreatureLoot(&loot, GetEntry(), 0);

	loot.gold = proto->money;

	// Master Looting Ninja Checker
	if(sWorld.antiMasterLootNinja)
	{
		Player* looter = objmgr.GetPlayer((uint32)this->TaggerGuid);
		if(looter && looter->GetGroup() && looter->GetGroup()->GetMethod() == PARTY_LOOT_MASTER)
		{
			uint16 lootThreshold = looter->GetGroup()->GetThreshold();

			for(vector<__LootItem>::iterator itr = loot.items.begin(); itr != loot.items.end(); ++itr)
			{
				if(itr->item.itemproto->Quality < lootThreshold)
					continue;

				// Master Loot Stuff - Let the rest of the raid know what dropped..
				//TODO: Shouldn't we move this array to a global position? Or maybe it already exists^^ (VirtualAngel) --- I can see (dead) talking pigs...^^
				const char* itemColours[8] = { "9d9d9d", "ffffff", "1eff00", "0070dd", "a335ee", "ff8000", "e6cc80", "e6cc80" };
				char buffer[256];
				sprintf(buffer, "\174cff%s\174Hitem:%u:0:0:0:0:0:0:0\174h[%s]\174h\174r", itemColours[itr->item.itemproto->Quality], itr->item.itemproto->ItemId, itr->item.itemproto->Name1);
				this->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, buffer);
			}
		}
	}

	/*
	 * If there's an amount given, take it as an expected value and
	 * generated a corresponding random value. The random value is
	 * something similar to a normal distribution.
	 *
	 * You'd get a ``better'' distribution if you called `rand()' for each
	 * copper individually. However, if the loot was 1G we'd call `rand()'
	 * 15000 times, which is not ideal. So we use one call to `rand()' to
	 * (hopefully) get 24 random bits, which is then used to create a
	 * normal distribution over 1/24th of the difference.
	 */
	if( loot.gold >= 12 )
	{
		uint32 random_bits;
		double chunk_size;
		double gold_fp;

		/* Split up the difference into 12 chunks.. */
		chunk_size = loot.gold / 12.0;

		/* Get 24 random bits. We use the low order bits, because we're
		 * too lazy to check how many random bits the system actually
		 * returned. */
		random_bits = rand() & 0x00ffffff;

		gold_fp = 0.0;
		while(random_bits != 0)
		{
			/* If last bit is one .. */
			if((random_bits & 0x01) == 1)
				/* .. increase loot by 1/12th of expected value */
				gold_fp += chunk_size;

			/* Shift away the LSB */
			random_bits >>= 1;
		}

		/* To hide your discrete values a bit, add another random
		 * amount between -(chunk_size/2) and +(chunk_size/2). */
		gold_fp += chunk_size * (rand() / (RAND_MAX + 1.0) - 0.5);

		/*
		 * In theory we can end up with a negative amount. Give at
		 * least one chunk_size here to prevent this from happening. In
		 * case you're interested, the probability is around 2.98e-8.
		 */
		if(gold_fp < chunk_size)
			gold_fp = chunk_size;

		/* Convert the floating point gold value to an integer again
		 * and we're done. */
		loot.gold = static_cast<uint32>(0.5 + gold_fp);
	}

	loot.gold = static_cast<uint32>( loot.gold * sWorld.getRate(RATE_MONEY) );
}

void Creature::SaveToDB()
{
	if(m_spawn == NULL)
	{
		m_spawn = new CreatureSpawn;
		m_spawn->entry = GetEntry();
		m_spawn->form = 0;
		m_spawn->id = spawnid = objmgr.GenerateCreatureSpawnID();
		m_spawn->movetype = m_aiInterface->getMoveType();
		m_spawn->displayid = m_uint32Values[UNIT_FIELD_DISPLAYID];
		m_spawn->x = m_position.x;
		m_spawn->y = m_position.y;
		m_spawn->z = m_position.z;
		m_spawn->o = m_position.o;
		m_spawn->emote_state = m_uint32Values[UNIT_NPC_EMOTESTATE];
		m_spawn->flags = m_uint32Values[UNIT_FIELD_FLAGS];
		m_spawn->factionid = GetFaction();
		m_spawn->bytes0 = m_uint32Values[UNIT_FIELD_BYTES_0];
		m_spawn->bytes1 = m_uint32Values[UNIT_FIELD_BYTES_1];
		m_spawn->bytes2 = m_uint32Values[UNIT_FIELD_BYTES_2];
		m_spawn->stand_state = GetStandState();
		m_spawn->death_state = 0;
		m_spawn->channel_target_creature = 0;
		m_spawn->channel_target_go = 0;
		m_spawn->channel_spell = 0;
		m_spawn->MountedDisplayID = m_uint32Values[UNIT_FIELD_MOUNTDISPLAYID];
		m_spawn->Item1SlotDisplay = GetEquippedItem(MELEE);
		m_spawn->Item2SlotDisplay = GetEquippedItem(OFFHAND);
		m_spawn->Item3SlotDisplay = GetEquippedItem(RANGED);
		if(GetAIInterface()->Flying())
			m_spawn->CanFly = 1;
		else if(GetAIInterface()->onGameobject)
			m_spawn->CanFly = 2;
		else
			m_spawn->CanFly = 0;
		m_spawn->phase = m_phase;

		uint32 x = GetMapMgr()->GetPosX(GetPositionX());
		uint32 y = GetMapMgr()->GetPosY(GetPositionY());

		// Add spawn to map
		GetMapMgr()->GetBaseMap()->GetSpawnsListAndCreate(x, y)->CreatureSpawns.push_back(m_spawn);
	}

	std::stringstream ss;

	ss << "DELETE FROM creature_spawns WHERE id = ";
	ss << spawnid;
	ss << ";";

	WorldDatabase.Execute(ss.str().c_str());

	ss.rdbuf()->str("");

	ss << "INSERT INTO creature_spawns VALUES("
	   << spawnid << ","
	   << GetEntry() << ","
	   << GetMapId() << ","
	   << m_position.x << ","
	   << m_position.y << ","
	   << m_position.z << ","
	   << m_position.o << ","
	   << m_aiInterface->getMoveType() << ","
	   << m_uint32Values[UNIT_FIELD_DISPLAYID] << ","
	   << GetFaction() << ","
	   << m_uint32Values[UNIT_FIELD_FLAGS] << ","
	   << m_uint32Values[UNIT_FIELD_BYTES_0] << ","
	   << m_uint32Values[UNIT_FIELD_BYTES_1] << ","
	   << m_uint32Values[UNIT_FIELD_BYTES_2] << ","
	   << m_uint32Values[UNIT_NPC_EMOTESTATE] << ",0,";

	ss << m_spawn->channel_spell << "," << m_spawn->channel_target_go << "," << m_spawn->channel_target_creature << ",";

	ss << uint32(GetStandState()) << ",";

	ss << m_spawn->death_state << ",";

	ss << m_uint32Values[UNIT_FIELD_MOUNTDISPLAYID] << ","
	   << GetEquippedItem(MELEE) << ","
	   << GetEquippedItem(OFFHAND) << ","
	   << GetEquippedItem(RANGED) << ",";

	if(GetAIInterface()->Flying())
		ss << 1 << ",";
	else if(GetAIInterface()->onGameobject)
		ss << 2 << ",";
	else
		ss << 0 << ",";

	ss << m_phase << ")";

	WorldDatabase.Execute(ss.str().c_str());
}


void Creature::LoadScript()
{
	_myScriptClass = sScriptMgr.CreateAIScriptClassForEntry(this);
}

void Creature::DeleteFromDB()
{
	if(!GetSQL_id())
		return;

	WorldDatabase.Execute("DELETE FROM creature_spawns WHERE id = %u", GetSQL_id());
	WorldDatabase.Execute("DELETE FROM creature_waypoints WHERE spawnid = %u", GetSQL_id());
}


/////////////
/// Quests

void Creature::AddQuest(QuestRelation* Q)
{
	m_quests->push_back(Q);
}

void Creature::DeleteQuest(QuestRelation* Q)
{
	list<QuestRelation*>::iterator it;
	for(it = m_quests->begin(); it != m_quests->end(); ++it)
	{
		if(((*it)->type == Q->type) && ((*it)->qst == Q->qst))
		{
			delete(*it);
			m_quests->erase(it);
			break;
		}
	}
}

Quest* Creature::FindQuest(uint32 quest_id, uint8 quest_relation)
{
	list<QuestRelation*>::iterator it;
	for(it = m_quests->begin(); it != m_quests->end(); ++it)
	{
		QuestRelation* ptr = (*it);

		if((ptr->qst->id == quest_id) && (ptr->type & quest_relation))
		{
			return ptr->qst;
		}
	}
	return NULL;
}

uint16 Creature::GetQuestRelation(uint32 quest_id)
{
	uint16 quest_relation = 0;
	list<QuestRelation*>::iterator it;

	for(it = m_quests->begin(); it != m_quests->end(); ++it)
	{
		if((*it)->qst->id == quest_id)
		{
			quest_relation |= (*it)->type;
		}
	}
	return quest_relation;
}

uint32 Creature::NumOfQuests()
{
	return (uint32)m_quests->size();
}

void Creature::_LoadQuests()
{
	sQuestMgr.LoadNPCQuests(this);
}

void Creature::setDeathState(DeathState s)
{
	if(s == ALIVE)
		this->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DEAD);

	if(s == JUST_DIED)
	{

		GetAIInterface()->ResetUnitToFollow();
		m_deathState = CORPSE;
		m_corpseEvent = true;

		/*sEventMgr.AddEvent(this, &Creature::OnRemoveCorpse, EVENT_CREATURE_REMOVE_CORPSE, 180000, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);*/
		if(m_enslaveSpell)
			RemoveEnslave();

		if(m_currentSpell)
			m_currentSpell->cancel();

		// if it's not a Pet, and not a summon and it has skinningloot then we will allow skinning
		if((GetCreatedByGUID() == 0) && (GetSummonedByGUID() == 0) && lootmgr.IsSkinnable(creature_info->Id))
			SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SKINNABLE);


	}

	else m_deathState = s;
}

void Creature::AddToWorld()
{
	// force set faction
	if(m_faction == NULL || m_factionDBC == NULL)
		_setFaction();

	if(creature_info == NULL)
		creature_info = CreatureNameStorage.LookupEntry(GetEntry());

	if(creature_info == NULL)
		return;

	if(m_faction == NULL || m_factionDBC == NULL)
		return;

	Object::AddToWorld();
}

void Creature::AddToWorld(MapMgr* pMapMgr)
{
	// force set faction
	if(m_faction == NULL || m_factionDBC == NULL)
		_setFaction();

	if(creature_info == NULL)
		creature_info = CreatureNameStorage.LookupEntry(GetEntry());

	if(creature_info == NULL)
		return;

	if(m_faction == NULL || m_factionDBC == NULL)
		return;

	Object::AddToWorld(pMapMgr);
}

bool Creature::CanAddToWorld()
{
	if(m_factionDBC == NULL || m_faction == NULL)
		_setFaction();

	if(creature_info == NULL || m_faction == NULL || m_factionDBC == NULL || proto == NULL)
		return false;

	return true;
}

void Creature::RemoveFromWorld(bool addrespawnevent, bool free_guid)
{
	uint32 delay = 0;
	if(addrespawnevent && (m_respawnTimeOverride > 0 || proto->RespawnTime > 0))
		delay = m_respawnTimeOverride > 0 ? m_respawnTimeOverride : proto->RespawnTime;

	Despawn(0, delay);
}

void Creature::RemoveFromWorld(bool free_guid)
{
	PrepareForRemove();
	Unit::RemoveFromWorld(free_guid);
}

void Creature::EnslaveExpire()
{

	m_enslaveCount++;

	uint64 charmer = GetCharmedByGUID();

	Player* caster = objmgr.GetPlayer(Arcemu::Util::GUID_LOPART(charmer));
	if(caster)
	{
		caster->SetCharmedUnitGUID(0);
		caster->SetSummonedUnitGUID(0);

		WorldPacket data(SMSG_PET_SPELLS, 8);

		data << uint64(0);
		data << uint32(0);

		caster->SendPacket(&data);
	}
	SetCharmedByGUID(0);
	SetSummonedByGUID(0);

	m_walkSpeed = m_base_walkSpeed;
	m_runSpeed = m_base_runSpeed;

	switch(GetCreatureInfo()->Type)
	{
		case UNIT_TYPE_DEMON:
			SetFaction(90);
			break;
		default:
			SetFaction(954);
			break;
	};

	GetAIInterface()->Init(((Unit*)this), AITYPE_AGRO, MOVEMENTTYPE_NONE);

	UpdateOppFactionSet();
	UpdateSameFactionSet();
}

bool Creature::RemoveEnslave()
{
	return RemoveAura(m_enslaveSpell);
}

void Creature::AddInRangeObject(Object* pObj)
{
	Unit::AddInRangeObject(pObj);
}

void Creature::OnRemoveInRangeObject(Object* pObj)
{
	if(m_escorter == pObj)
	{
		// we lost our escorter, return to the spawn.
		m_aiInterface->StopMovement(10000);
		m_escorter = NULL;
		GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
		//DestroyCustomWaypointMap(); //function not needed at all, crashing on delete(*int)
		//GetAIInterface()->deleteWaypoints();//this can repleace DestroyCustomWaypointMap, but it's crashing on delete too
		Despawn(1000, 1000);
	}

	Unit::OnRemoveInRangeObject(pObj);
}

void Creature::ClearInRangeSet()
{
	Unit::ClearInRangeSet();
}

void Creature::CalcResistance(uint32 type)
{
	int32 pos = 0;
	int32 neg = 0;

	if(BaseResistanceModPct[ type ] < 0)
		neg = (BaseResistance[ type ] * abs(BaseResistanceModPct[ type ]) / 100);
	else
		pos = (BaseResistance[ type ] * BaseResistanceModPct[ type ]) / 100;

	if(IsPet() && isAlive() && IsInWorld())
	{
		Player* owner = TO_PET(this)->GetPetOwner();
		if(type == 0 && owner)
			pos += int32(0.35f * owner->GetResistance(type));
		else if(owner)
			pos += int32(0.40f * owner->GetResistance(type));
	}

	if(ResistanceModPct[ type ] < 0)
		neg += (BaseResistance[ type ] + pos - neg) * abs(ResistanceModPct[ type ]) / 100;
	else
		pos += (BaseResistance[ type ] + pos - neg) * ResistanceModPct[ type ] / 100;

	if(FlatResistanceMod[ type ] < 0)
		neg += abs(FlatResistanceMod[ type ]);
	else
		pos += FlatResistanceMod[ type ];

	SetUInt32Value(UNIT_FIELD_RESISTANCEBUFFMODSPOSITIVE + type, pos);
	SetUInt32Value(UNIT_FIELD_RESISTANCEBUFFMODSNEGATIVE + type, neg);

	int32 tot = BaseResistance[ type ] + pos - neg;

	SetResistance(type, tot > 0 ? tot : 0);
}

void Creature::CalcStat(uint32 type)
{
	int32 pos = 0;
	int32 neg = 0;

	if(StatModPct[ type ] < 0)
		neg = (BaseStats[ type ] * abs(StatModPct[ type ]) / 100);
	else
		pos = (BaseStats[ type ] * StatModPct[ type ]) / 100;

	if(IsPet())
	{
		Player* owner = TO< Pet* >(this)->GetPetOwner();
		if(type == STAT_STAMINA && owner)
			pos += int32(0.45f * owner->GetStat(STAT_STAMINA));
		else if(type == STAT_INTELLECT && owner && GetCreatedBySpell())
			pos += int32(0.30f * owner->GetStat(STAT_INTELLECT));
	}

	if(TotalStatModPct[ type ] < 0)
		neg += (BaseStats[ type ] + pos - neg) * abs(TotalStatModPct[ type ]) / 100;
	else
		pos += (BaseStats[ type ] + pos - neg) * TotalStatModPct[ type ] / 100;

	if(FlatStatMod[ type ] < 0)
		neg += abs(FlatStatMod[ type ]);
	else
		pos += FlatStatMod[ type ];

	SetUInt32Value(UNIT_FIELD_POSSTAT0 + type, pos);
	SetUInt32Value(UNIT_FIELD_NEGSTAT0 + type, neg);

	int32 tot = BaseStats[ type ] + pos - neg;
	SetStat(type, tot > 0 ? tot : 0);

	switch(type)
	{
		case STAT_STRENGTH:
			{
				//Attack Power
				if(!IsPet())  //We calculate pet's later
				{
					uint32 str = GetStat(STAT_STRENGTH);
					int32 AP = (str * 2 - 20);
					if(AP < 0) AP = 0;
					SetAttackPower(AP);
				}
				CalcDamage();
			}
			break;
		case STAT_AGILITY:
			{
				//Ranged Attack Power (Does any creature use this?)
				int32 RAP = getLevel() + GetStat(STAT_AGILITY) - 10;
				if(RAP < 0) RAP = 0;
				SetRangedAttackPower(RAP);
			}
			break;
		case STAT_STAMINA:
			{
				//Health
				uint32 hp = GetBaseHealth();
				uint32 stat_bonus = GetUInt32Value(UNIT_FIELD_POSSTAT2) - GetUInt32Value(UNIT_FIELD_NEGSTAT2);
				if(static_cast<int32>(stat_bonus) < 0) stat_bonus = 0;

				uint32 bonus = stat_bonus * 10 + m_healthfromspell;
				uint32 res = hp + bonus;

				if(res < hp) res = hp;
				SetUInt32Value(UNIT_FIELD_MAXHEALTH, res);
				if(GetUInt32Value(UNIT_FIELD_HEALTH) > GetUInt32Value(UNIT_FIELD_MAXHEALTH))
					SetHealth(GetUInt32Value(UNIT_FIELD_MAXHEALTH));
			}
			break;
		case STAT_INTELLECT:
			{
				if(GetPowerType() == POWER_TYPE_MANA)
				{
					uint32 mana = GetBaseMana();
					uint32 stat_bonus = (GetUInt32Value(UNIT_FIELD_POSSTAT3) - GetUInt32Value(UNIT_FIELD_NEGSTAT3));
					if(static_cast<int32>(stat_bonus) < 0) stat_bonus = 0;

					uint32 bonus = stat_bonus * 15;
					uint32 res = mana + bonus;

					if(res < mana) res = mana;
					SetMaxPower(POWER_TYPE_MANA, res);
				}
			}
			break;
	}
}

void Creature::RegenerateHealth()
{
	if(m_limbostate || !m_canRegenerateHP)
		return;

	uint32 cur = GetHealth();
	uint32 mh = GetMaxHealth();
	if(cur >= mh)return;

	//though creatures have their stats we use some weird formula for amt
	float amt = 0.0f;
	uint32 lvl = getLevel();

	amt = lvl * 2.0f;
	if(PctRegenModifier)
		amt += (amt * PctRegenModifier) / 100;

	if(GetCreatureInfo()->Rank == 3)
		amt *= 10000.0f;
	//Apply shit from conf file
	amt *= sWorld.getRate(RATE_HEALTH);

	if(amt <= 1.0f) //this fixes regen like 0.98
		cur++;
	else
		cur += (uint32)amt;
	SetHealth((cur >= mh) ? mh : cur);
}

void Creature::RegenerateMana()
{
	float amt;
	if(m_interruptRegen)
		return;

	uint32 cur = GetPower(POWER_TYPE_MANA);
	uint32 mm = GetMaxPower(POWER_TYPE_MANA);
	if(cur >= mm)return;
	amt = (getLevel() + 10) * PctPowerRegenModifier[POWER_TYPE_MANA];


	amt *= sWorld.getRate(RATE_POWER1);
	if(amt <= 1.0)  //this fixes regen like 0.98
		cur++;
	else
		cur += (uint32)amt;

	if(cur >= mm)
		SetPower(POWER_TYPE_MANA, mm);
	else
		SetPower(POWER_TYPE_MANA, cur);
}

void Creature::RegenerateFocus()
{
	if(m_interruptRegen)
		return;

	uint32 cur = GetPower(POWER_TYPE_FOCUS);
	uint32 mm = GetMaxPower(POWER_TYPE_FOCUS);
	if(cur >= mm)return;
	float regenrate = sWorld.getRate(RATE_POWER3);
	float amt = 25.0f * PctPowerRegenModifier[POWER_TYPE_FOCUS] * regenrate;
	cur += (uint32)amt;
	SetPower(POWER_TYPE_FOCUS, (cur >= mm) ? mm : cur);
}

void Creature::CallScriptUpdate()
{
	ARCEMU_ASSERT(_myScriptClass != NULL);
	if(!IsInWorld())
		return;

	_myScriptClass->AIUpdate();
}

void Creature::AddVendorItem(uint32 itemid, uint32 amount, ItemExtendedCostEntry* ec)
{
	CreatureItem ci;
	ci.amount = amount;
	ci.itemid = itemid;
	ci.available_amount = 0;
	ci.max_amount = 0;
	ci.incrtime = 0;
	ci.extended_cost = ec;
	if(!m_SellItems)
	{
		m_SellItems = new vector<CreatureItem>;
		objmgr.SetVendorList(GetEntry(), m_SellItems);
	}
	m_SellItems->push_back(ci);
}
void Creature::ModAvItemAmount(uint32 itemid, uint32 value)
{
	for(std::vector<CreatureItem>::iterator itr = m_SellItems->begin(); itr != m_SellItems->end(); ++itr)
	{
		if(itr->itemid == itemid)
		{
			if(itr->available_amount)
			{
				if(value > itr->available_amount)	// shouldn't happen
				{
					itr->available_amount = 0;
					return;
				}
				else
					itr->available_amount -= value;

				if(!event_HasEvent(EVENT_ITEM_UPDATE))
					sEventMgr.AddEvent(this, &Creature::UpdateItemAmount, itr->itemid, EVENT_ITEM_UPDATE, itr->incrtime, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			}
			return;
		}
	}
}
void Creature::UpdateItemAmount(uint32 itemid)
{
	for(std::vector<CreatureItem>::iterator itr = m_SellItems->begin(); itr != m_SellItems->end(); ++itr)
	{
		if(itr->itemid == itemid)
		{
			if(itr->max_amount == 0)		// shouldn't happen
				itr->available_amount = 0;
			else
			{
				itr->available_amount = itr->max_amount;
			}
			return;
		}
	}
}

void Creature::FormationLinkUp(uint32 SqlId)
{
	if(!m_mapMgr)		// shouldn't happen
		return;

	Creature* creature = m_mapMgr->GetSqlIdCreature(SqlId);
	if(creature != 0)
	{
		m_aiInterface->m_formationLinkTarget = creature->GetGUID();
		haslinkupevent = false;
		event_RemoveEvents(EVENT_CREATURE_FORMATION_LINKUP);
	}
}

void Creature::ChannelLinkUpGO(uint32 SqlId)
{
	if(!m_mapMgr)		// shouldn't happen
		return;

	GameObject* go = m_mapMgr->GetSqlIdGameObject(SqlId);
	if(go != 0)
	{
		event_RemoveEvents(EVENT_CREATURE_CHANNEL_LINKUP);
		SetChannelSpellTargetGUID(go->GetGUID());
		SetChannelSpellId(m_spawn->channel_spell);
	}
}

void Creature::ChannelLinkUpCreature(uint32 SqlId)
{
	if(!m_mapMgr)		// shouldn't happen
		return;

	Creature* go = m_mapMgr->GetSqlIdCreature(SqlId);
	if(go != 0)
	{
		event_RemoveEvents(EVENT_CREATURE_CHANNEL_LINKUP);
		SetChannelSpellTargetGUID(go->GetGUID());
		SetChannelSpellId(m_spawn->channel_spell);
	}
}

WayPoint* Creature::CreateWaypointStruct()
{
	return new WayPoint();
}
//#define SAFE_FACTIONS

bool Creature::isattackable(CreatureSpawn* spawn)
{
	if(spawn == NULL)
		return false;

	if((spawn->flags & 2) || (spawn->flags & 128) || (spawn->flags & 256) || (spawn->flags & 65536))
		return false;
	else return true;
}

uint8 get_byte(uint32 buffer, uint32 index)
{
	uint32 mask = uint32(~0ul);
	if(index > sizeof(uint32) - 1)
		return 0;

	buffer = buffer >> index * 8;
	mask   = mask   >> 3 * 8;
	buffer = buffer & mask;

	return (uint8)buffer;
}

bool Creature::Load(CreatureSpawn* spawn, uint32 mode, MapInfo* info)
{
	m_spawn = spawn;
	proto = CreatureProtoStorage.LookupEntry(spawn->entry);
	if(proto == NULL)
		return false;
	creature_info = CreatureNameStorage.LookupEntry(spawn->entry);
	if(creature_info == NULL)
		return false;

	spawnid = spawn->id;
	m_phase = spawn->phase;

	m_walkSpeed = m_base_walkSpeed = proto->walk_speed; //set speeds
	m_runSpeed = m_base_runSpeed = proto->run_speed; //set speeds
	m_flySpeed = proto->fly_speed;

	//Set fields
	SetEntry(proto->Id);
	SetScale(proto->Scale);

	//SetHealth( (mode ? long2int32(proto->Health * 1.5)  : proto->Health));
	//SetBaseHealth((mode ? long2int32(proto->Health * 1.5)  : proto->Health));
	//SetMaxHealth( (mode ? long2int32(proto->Health * 1.5)  : proto->Health));
	if(proto->MinHealth > proto->MaxHealth)
	{
		proto->MaxHealth = proto->MinHealth + 1;
		SaveToDB();
	}

	uint32 health = proto->MinHealth + RandomUInt(proto->MaxHealth - proto->MinHealth);

	// difficutly coefficient
	float diff_coeff = 1.0f;

	if(creature_info->Rank == ELITE_WORLDBOSS)
		diff_coeff = CalcHPCoefficient(info, mode, true);
	else if(creature_info->Type != UNIT_TYPE_CRITTER)
		diff_coeff = CalcHPCoefficient(info, mode, false);

	health = static_cast< uint32 >(health * diff_coeff);

	SetHealth(health);
	SetMaxHealth(health);
	SetBaseHealth(health);

	SetMaxPower(POWER_TYPE_MANA, proto->Mana);
	SetBaseMana(proto->Mana);
	SetPower(POWER_TYPE_MANA, proto->Mana);

	// Whee, thank you blizz, I love patch 2.2! Later on, we can randomize male/female mobs! xD
	// Determine gender (for voices)
	//if(spawn->displayid != creature_info->Male_DisplayID)
	//	setGender(1);   // Female

	// uint32 model = 0;
	// uint32 gender = creature_info->GenerateModelId(&model);
	// setGender(gender);

	SetDisplayId(spawn->displayid);
	SetNativeDisplayId(spawn->displayid);
	SetMount(spawn->MountedDisplayID);

	EventModelChange();

	setLevel(proto->MinLevel + (RandomUInt(proto->MaxLevel - proto->MinLevel)));

	if(mode && info)
		modLevel(min(73 - getLevel(), info->lvl_mod_a));

	for(uint32 i = 0; i < 7; ++i)
		SetResistance(i, proto->Resistances[i]);

	SetBaseAttackTime(MELEE, proto->AttackTime);

	float dmg_coeff = CalcDMGCoefficient(info, mode);

	SetMinDamage((mode ? proto->MinDamage * dmg_coeff  : proto->MinDamage));
	SetMaxDamage((mode ? proto->MaxDamage * dmg_coeff  : proto->MaxDamage));

	SetBaseAttackTime(RANGED, proto->RangedAttackTime);
	SetMinRangedDamage(proto->RangedMinDamage);
	SetMaxRangedDamage(proto->RangedMaxDamage);

	SetEquippedItem(MELEE, spawn->Item1SlotDisplay);
	SetEquippedItem(OFFHAND, spawn->Item2SlotDisplay);
	SetEquippedItem(RANGED, spawn->Item3SlotDisplay);

	SetFaction(spawn->factionid);
	SetUInt32Value(UNIT_FIELD_FLAGS, spawn->flags);
	SetEmoteState(spawn->emote_state);
	SetBoundingRadius(proto->BoundingRadius);
	SetCombatReach(proto->CombatReach);
	original_emotestate = spawn->emote_state;
	// set position
	m_position.ChangeCoords(spawn->x, spawn->y, spawn->z, spawn->o);
	m_spawnLocation.ChangeCoords(spawn->x, spawn->y, spawn->z, spawn->o);
	m_aiInterface->setMoveType(spawn->movetype);
	m_aiInterface->LoadWaypointMapFromDB(spawn->id);

	m_aiInterface->timed_emotes = objmgr.GetTimedEmoteList(spawn->id);

	// not a neutral creature
	if(!(m_factionDBC->RepListId == -1 && m_faction->HostileMask == 0 && m_faction->FriendlyMask == 0))
	{
		GetAIInterface()->m_canCallForHelp = true;
	}

	// set if creature can shoot or not.
	if(proto->CanRanged == 1)
		GetAIInterface()->m_canRangedAttack = true;
	else
		m_aiInterface->m_canRangedAttack = false;

//SETUP NPC FLAGS
	SetUInt32Value(UNIT_NPC_FLAGS, proto->NPCFLags);

	if(isVendor())
		m_SellItems = objmgr.GetVendorList(GetEntry());

	if(isQuestGiver())
		_LoadQuests();

	if(isTrainer() | isProf())
		mTrainer = objmgr.GetTrainer(GetEntry());

	if(isAuctioner())
		auctionHouse = sAuctionMgr.GetAuctionHouse(GetEntry());

	//load resistances
	for(uint8 x = 0; x < 7; x++)
		BaseResistance[x] = GetResistance(x);
	for(uint8 x = 0; x < 5; x++)
		BaseStats[x] = GetStat(x);

	BaseDamage[0] = GetMinDamage();
	BaseDamage[1] = GetMaxDamage();
	BaseOffhandDamage[0] = GetMinOffhandDamage();
	BaseOffhandDamage[1] = GetMaxOffhandDamage();
	BaseRangedDamage[0] = GetMinRangedDamage();
	BaseRangedDamage[1] = GetMaxRangedDamage();
	BaseAttackType = proto->AttackType;

	SetCastSpeedMod(1.0f);   // better set this one
	SetUInt32Value(UNIT_FIELD_BYTES_0, spawn->bytes0);
	SetUInt32Value(UNIT_FIELD_BYTES_1, spawn->bytes1);
	SetUInt32Value(UNIT_FIELD_BYTES_2, spawn->bytes2);

////////////AI

	// kek
	for(list<AI_Spell*>::iterator itr = proto->spells.begin(); itr != proto->spells.end(); ++itr)
	{
		// Load all spells that are not bound to a specific difficulty, OR mathces this maps' difficulty
		if((*itr)->instance_mode == mode || (*itr)->instance_mode == AISPELL_ANY_DIFFICULTY)
			m_aiInterface->addSpellToList(*itr);
	}

	// m_aiInterface->m_canCallForHelp = proto->m_canCallForHelp;
	// m_aiInterface->m_CallForHelpHealth = proto->m_callForHelpHealth;
	m_aiInterface->m_canFlee = proto->m_canFlee;
	m_aiInterface->m_FleeHealth = proto->m_fleeHealth;
	m_aiInterface->m_FleeDuration = proto->m_fleeDuration;

	GetAIInterface()->SetWalk();

	if(isattackable(spawn) && !(proto->isTrainingDummy) && !IsVehicle() )
	{
		GetAIInterface()->SetAllowedToEnterCombat(true);
	}
	else
	{
		GetAIInterface()->SetAllowedToEnterCombat(false);
		GetAIInterface()->SetAIType(AITYPE_PASSIVE);
	}

	// load formation data
	if(spawn->form != NULL)
	{
		m_aiInterface->m_formationLinkSqlId = spawn->form->fol;
		m_aiInterface->m_formationFollowDistance = spawn->form->dist;
		m_aiInterface->m_formationFollowAngle = spawn->form->ang;
	}
	else
	{
		m_aiInterface->m_formationLinkSqlId = 0;
		m_aiInterface->m_formationFollowDistance = 0;
		m_aiInterface->m_formationFollowAngle = 0;
	}

//////////////AI

	myFamily = dbcCreatureFamily.LookupEntry(creature_info->Family);


//HACK!
	if(m_uint32Values[UNIT_FIELD_DISPLAYID] == 17743 ||
	        m_uint32Values[UNIT_FIELD_DISPLAYID] == 20242 ||
	        m_uint32Values[UNIT_FIELD_DISPLAYID] == 15435 ||
	        (creature_info->Family == UNIT_TYPE_MISC))
	{
		m_useAI = false;
	}

	if(spawn->CanFly == 1)
		GetAIInterface()->SetFly();
	else if(spawn->CanFly == 2)
		GetAIInterface()->onGameobject = true;
	/* more hacks! */
	if(proto->Mana != 0)
		SetPowerType(POWER_TYPE_MANA);
	else
		SetPowerType(0);

	if(proto->guardtype == GUARDTYPE_CITY)
		m_aiInterface->m_isGuard = true;
	else
		m_aiInterface->m_isGuard = false;

	if(proto->guardtype == GUARDTYPE_NEUTRAL)
		m_aiInterface->m_isNeutralGuard = true;
	else
		m_aiInterface->m_isNeutralGuard = false;

	m_aiInterface->UpdateSpeeds();

	/* creature death state */
	if(spawn->death_state == CREATURE_STATE_APPEAR_DEAD)
	{
		m_limbostate = true;
		SetUInt32Value(UNIT_DYNAMIC_FLAGS, U_DYN_FLAG_DEAD);
	}
	else if(spawn->death_state == CREATURE_STATE_DEAD)
	{
		SetHealth(0);
		m_limbostate = true;
		setDeathState(CORPSE);
	}
	m_invisFlag = static_cast<uint8>(proto->invisibility_type);
	if(m_invisFlag > 0)
		m_invisible = true;
	if(spawn->stand_state)
		SetStandState((uint8)spawn->stand_state);

	m_aiInterface->EventAiInterfaceParamsetFinish();
	this->m_position.x = spawn->x;
	this->m_position.y = spawn->y;
	this->m_position.z = spawn->z;
	this->m_position.o = spawn->o;

	if( IsVehicle() ){
		AddVehicleComponent( proto->Id, proto->vehicleid );
		SetFlag( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK );
		setAItoUse( false );
	}

	if( proto->rooted != 0 )
		Root();

	return true;
}


void Creature::Load(CreatureProto* proto_, float x, float y, float z, float o)
{
	proto = proto_;

	creature_info = CreatureNameStorage.LookupEntry(proto->Id);
	if(!creature_info)
		return;

	if(proto_->isTrainingDummy == 0 && !IsVehicle() )
	{
		GetAIInterface()->SetAllowedToEnterCombat(true);
	}
	else
	{
		GetAIInterface()->SetAllowedToEnterCombat(false);
		GetAIInterface()->SetAIType(AITYPE_PASSIVE);
	}

	m_walkSpeed = m_base_walkSpeed = proto->walk_speed; //set speeds
	m_runSpeed = m_base_runSpeed = proto->run_speed; //set speeds

	//Set fields
	SetEntry(proto->Id);
	SetScale(proto->Scale);

	uint32 health = proto->MinHealth + RandomUInt(proto->MaxHealth - proto->MinHealth);

	SetHealth(health);
	SetMaxHealth(health);
	SetBaseHealth(health);

	SetMaxPower(POWER_TYPE_MANA, proto->Mana);
	SetBaseMana(proto->Mana);
	SetPower(POWER_TYPE_MANA, proto->Mana);

	uint32 model = 0;
	uint8 gender = creature_info->GenerateModelId(&model);
	setGender(gender);

	SetDisplayId(model);
	SetNativeDisplayId(model);
	SetMount(0);

	EventModelChange();

	//setLevel((mode ? proto->Level + (info ? info->lvl_mod_a : 0) : proto->Level));
	setLevel(proto->MinLevel + (RandomUInt(proto->MaxLevel - proto->MinLevel)));

	for(uint32 i = 0; i < 7; ++i)
		SetResistance(i, proto->Resistances[i]);

	SetBaseAttackTime(MELEE, proto->AttackTime);
	SetMinDamage(proto->MinDamage);
	SetMaxDamage(proto->MaxDamage);

// m_spawn is invalid here - don't use it!
// this is loading a CreatureProto, which doesn't have ItemSlotDisplays
//	SetEquippedItem(MELEE,m_spawn->Item1SlotDisplay);
//	SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID_1, m_spawn->Item2SlotDisplay);
//	SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID_2, m_spawn->Item3SlotDisplay);

	SetFaction(proto->Faction);
	SetBoundingRadius(proto->BoundingRadius);
	SetCombatReach(proto->CombatReach);
	original_emotestate = 0;
	// set position

	m_position.ChangeCoords(x, y, z, o);
	m_spawnLocation.ChangeCoords(x, y, z, o);

	// not a neutral creature
	if(!(m_factionDBC->RepListId == -1 && m_faction->HostileMask == 0 && m_faction->FriendlyMask == 0))
	{
		GetAIInterface()->m_canCallForHelp = true;
	}

	// set if creature can shoot or not.
	if(proto->CanRanged == 1)
		GetAIInterface()->m_canRangedAttack = true;
	else
		m_aiInterface->m_canRangedAttack = false;

	//SETUP NPC FLAGS
	SetUInt32Value(UNIT_NPC_FLAGS, proto->NPCFLags);

	if(isVendor())
		m_SellItems = objmgr.GetVendorList(GetEntry());

	if(isQuestGiver())
		_LoadQuests();

	if(isTrainer() | isProf())
		mTrainer = objmgr.GetTrainer(GetEntry());

	if(isAuctioner())
		auctionHouse = sAuctionMgr.GetAuctionHouse(GetEntry());

	//load resistances
	for(uint32 j = 0; j < 7; j++)
		BaseResistance[j] = GetResistance(j);
	for(uint32 j = 0; j < 5; j++)
		BaseStats[j] = GetStat(j);

	BaseDamage[0] = GetMinDamage();
	BaseDamage[1] = GetMaxDamage();
	BaseOffhandDamage[0] = GetMinOffhandDamage();
	BaseOffhandDamage[1] = GetMaxOffhandDamage();
	BaseRangedDamage[0] = GetMinRangedDamage();
	BaseRangedDamage[1] = GetMaxRangedDamage();
	BaseAttackType = proto->AttackType;

	SetCastSpeedMod(1.0f);   // better set this one

	////////////AI

	// kek
	for(list<AI_Spell*>::iterator itr = proto->spells.begin(); itr != proto->spells.end(); ++itr)
	{
		// Load all spell that are not set for a specific difficulty
		if((*itr)->instance_mode == AISPELL_ANY_DIFFICULTY)
			m_aiInterface->addSpellToList(*itr);
	}
	m_aiInterface->m_canCallForHelp = proto->m_canCallForHelp;
	m_aiInterface->m_CallForHelpHealth = proto->m_callForHelpHealth;
	m_aiInterface->m_canFlee = proto->m_canFlee;
	m_aiInterface->m_FleeHealth = proto->m_fleeHealth;
	m_aiInterface->m_FleeDuration = proto->m_fleeDuration;

	GetAIInterface()->setMoveType(0);
	GetAIInterface()->SetWalk();

	// load formation data
	m_aiInterface->m_formationLinkSqlId = 0;
	m_aiInterface->m_formationFollowDistance = 0;
	m_aiInterface->m_formationFollowAngle = 0;

	//////////////AI

	myFamily = dbcCreatureFamily.LookupEntry(creature_info->Family);


	//HACK!
	if(m_uint32Values[ UNIT_FIELD_DISPLAYID ] == 17743 ||
	        m_uint32Values[ UNIT_FIELD_DISPLAYID ] == 20242 ||
	        m_uint32Values[ UNIT_FIELD_DISPLAYID ] == 15435 ||
	        creature_info->Type == UNIT_TYPE_MISC)
	{
		m_useAI = false;
	}

	SetPowerType(POWER_TYPE_MANA);

	if(proto->guardtype == GUARDTYPE_CITY)
		m_aiInterface->m_isGuard = true;
	else
		m_aiInterface->m_isGuard = false;

	if(proto->guardtype == GUARDTYPE_NEUTRAL)
		m_aiInterface->m_isNeutralGuard = true;
	else
		m_aiInterface->m_isNeutralGuard = false;

	m_aiInterface->UpdateSpeeds();

	m_invisFlag = static_cast<uint8>(proto->invisibility_type);
	if(m_invisFlag > 0)
		m_invisible = true;

	if( IsVehicle() ){
		AddVehicleComponent( proto->Id, proto->vehicleid );
		SetFlag( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK );
		setAItoUse( false );
	}

	if( proto->rooted != 0 )
		Root();
}

void Creature::OnPushToWorld()
{
	if(proto == NULL)
	{
		LOG_ERROR("Something tried to push to world Creature ID %u with proto set to NULL.", GetEntry());
#ifdef _DEBUG
		ARCEMU_ASSERT(false);
#else
		SetCreatureProto(CreatureProtoStorage.LookupEntry(GetEntry()));
#endif
	}
	if(creature_info == NULL)
	{
		LOG_ERROR("Something tried to push to world Creature ID %u with creature_info set to NULL.", GetEntry());
#ifdef _DEBUG
		ARCEMU_ASSERT(false);
#else
		SetCreatureInfo(CreatureNameStorage.LookupEntry(GetEntry()));
#endif
	}

	set<uint32>::iterator itr = proto->start_auras.begin();
	SpellEntry* sp;
	for(; itr != proto->start_auras.end(); ++itr)
	{
		sp = dbcSpell.LookupEntryForced((*itr));
		if(sp == NULL) continue;

		CastSpell(this, sp, 0);
	}

	if(GetScript() == NULL)
	{
		LoadScript();
	}

	Unit::OnPushToWorld();

	if(_myScriptClass)
		_myScriptClass->OnLoad();

	if(m_spawn)
	{
		if(m_aiInterface->m_formationLinkSqlId)
		{
			// add event
			sEventMgr.AddEvent(this, &Creature::FormationLinkUp, m_aiInterface->m_formationLinkSqlId,
			                   EVENT_CREATURE_FORMATION_LINKUP, 1000, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			haslinkupevent = true;
		}

		if(m_spawn->channel_target_creature)
		{
			sEventMgr.AddEvent(this, &Creature::ChannelLinkUpCreature, m_spawn->channel_target_creature, EVENT_CREATURE_CHANNEL_LINKUP, 1000, 5, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);	// only 5 attempts
		}

		if(m_spawn->channel_target_go)
		{
			sEventMgr.AddEvent(this, &Creature::ChannelLinkUpGO, m_spawn->channel_target_go, EVENT_CREATURE_CHANNEL_LINKUP, 1000, 5, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);	// only 5 attempts
		}
	}

	m_aiInterface->m_is_in_instance = (m_mapMgr->GetMapInfo()->type != INSTANCE_NULL) ? true : false;
	if(this->HasItems())
	{
		for(std::vector<CreatureItem>::iterator itr2 = m_SellItems->begin(); itr2 != m_SellItems->end(); ++itr2)
		{
			if(itr2->max_amount == 0)
				itr2->available_amount = 0;
			else if(itr2->available_amount < itr2->max_amount)
				sEventMgr.AddEvent(this, &Creature::UpdateItemAmount, itr2->itemid, EVENT_ITEM_UPDATE, VENDOR_ITEMS_UPDATE_TIME, 1, 0);
		}

	}

	CALL_INSTANCE_SCRIPT_EVENT(m_mapMgr, OnCreaturePushToWorld)(this);
}

void Creature::Despawn(uint32 delay, uint32 respawntime)
{
	if(delay)
	{
		sEventMgr.AddEvent(this, &Creature::Despawn, (uint32)0, respawntime, EVENT_CREATURE_RESPAWN, delay, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		return;
	}

	PrepareForRemove();

	if(!IsInWorld())
		return;

	if(_myScriptClass != NULL)
		_myScriptClass->OnDespawn();

	if(respawntime && !m_noRespawn)
	{
		/* get the cell with our SPAWN location. if we've moved cell this might break :P */
		MapCell* pCell = m_mapMgr->GetCellByCoords(m_spawnLocation.x, m_spawnLocation.y);
		if(pCell == NULL)
			pCell = GetMapCell();

		ARCEMU_ASSERT(pCell != NULL);
		pCell->_respawnObjects.insert(this);
		sEventMgr.RemoveEvents(this);
		sEventMgr.AddEvent(m_mapMgr, &MapMgr::EventRespawnCreature, this, pCell->GetPositionX(), pCell->GetPositionY(), EVENT_CREATURE_RESPAWN, respawntime, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		Unit::RemoveFromWorld(false);
		m_position = m_spawnLocation;
		m_respawnCell = pCell;
	}
	else
	{
		Unit::RemoveFromWorld(true);
		SafeDelete();
	}
}

void Creature::TriggerScriptEvent(int fRef)
{
	if(_myScriptClass)
		_myScriptClass->StringFunctionCall(fRef);
}

void Creature::DestroyCustomWaypointMap()
{
	if(m_custom_waypoint_map)
	{
		m_aiInterface->SetWaypointMap(NULL);
		m_custom_waypoint_map = NULL;
	}
}

void Creature::RemoveLimboState(Unit* healer)
{
	if(!m_limbostate != true)
		return;

	m_limbostate = false;
	SetEmoteState(m_spawn ? m_spawn->emote_state : 0);
	SetHealth(GetMaxHealth());
	bInvincible = false;
}

// Generates 3 random waypoints around the NPC
void Creature::SetGuardWaypoints()
{
	if(!GetMapMgr()) return;

	GetAIInterface()->setMoveType(1);
	for(int i = 1; i <= 4; i++)
	{
		float ang = rand() / 100.0f;
		float ran = (rand() % (100)) / 10.0f;
		while(ran < 1)
			ran = (rand() % (100)) / 10.0f;

		WayPoint* wp = new WayPoint;
		wp->id = i;
		wp->flags = 0;
		wp->waittime = 800;  /* these guards are antsy :P */
		wp->x = GetSpawnX() + ran * sin(ang);
		wp->y = GetSpawnY() + ran * cos(ang);
		wp->z = m_mapMgr->GetLandHeight(wp->x, wp->y, m_spawnLocation.z + 2);


		wp->o = 0;
		wp->backwardemoteid = 0;
		wp->backwardemoteoneshot = false;
		wp->forwardemoteid = 0;
		wp->forwardemoteoneshot = false;
		wp->backwardskinid = m_uint32Values[UNIT_FIELD_NATIVEDISPLAYID];
		wp->forwardskinid = m_uint32Values[UNIT_FIELD_NATIVEDISPLAYID];
		GetAIInterface()->addWayPoint(wp);
	}
}

uint32 Creature::GetNpcTextId()
{
	return objmgr.GetGossipTextForNpc(this->GetEntry());
}

float Creature::GetBaseParry()
{
	// TODO what are the parry rates for mobs?
	// FACT: bosses have varying parry rates (used to tune the difficulty of boss fights)

	// for now return a base of 5%, later get from dbase?
	return 5.0f;
}

Group* Creature::GetGroup()
{
	return NULL;
}

bool Creature::HasLootForPlayer(Player* plr)
{
	if(loot.gold > 0)
		return true;

	for(vector<__LootItem>::iterator itr = loot.items.begin(); itr != loot.items.end(); ++itr)
	{
		ItemPrototype* proto = itr->item.itemproto;
		if(proto != NULL)
		{
			if(proto->Bonding == ITEM_BIND_QUEST || proto->Bonding == ITEM_BIND_QUEST2)
			{
				if(plr->HasQuestForItem(proto->ItemId))
					return true;
			}
			else if(itr->iItemsCount > 0)
				return true;
		}
	}
	return false;
}

uint32 Creature::GetRequiredLootSkill()
{
	if(GetCreatureInfo()->Flags1 & CREATURE_FLAG1_HERBLOOT)
		return SKILL_HERBALISM;     // herbalism
	else if(GetCreatureInfo()->Flags1 & CREATURE_FLAG1_MININGLOOT)
		return SKILL_MINING;        // mining
	else if(GetCreatureInfo()->Flags1 & CREATURE_FLAG1_ENGINEERLOOT)
		return SKILL_ENGINEERING;
	else
		return SKILL_SKINNING;      // skinning
};

//! Is PVP flagged?
bool Creature::IsPvPFlagged()
{
	return HasByteFlag(UNIT_FIELD_BYTES_2, 1, U_FIELD_BYTES_FLAG_PVP);
}

void Creature::SetPvPFlag()
{
	SetByteFlag(UNIT_FIELD_BYTES_2, 1, U_FIELD_BYTES_FLAG_PVP);
	summonhandler.SetPvPFlags();
}

void Creature::RemovePvPFlag()
{
	RemoveByteFlag(UNIT_FIELD_BYTES_2, 1, U_FIELD_BYTES_FLAG_PVP);
	summonhandler.RemovePvPFlags();
}

bool Creature::IsFFAPvPFlagged()
{
	return HasByteFlag(UNIT_FIELD_BYTES_2, 1, U_FIELD_BYTES_FLAG_FFA_PVP);
}

void Creature::SetFFAPvPFlag()
{
	SetByteFlag(UNIT_FIELD_BYTES_2, 1, U_FIELD_BYTES_FLAG_FFA_PVP);
	summonhandler.SetFFAPvPFlags();
}

void Creature::RemoveFFAPvPFlag()
{
	RemoveByteFlag(UNIT_FIELD_BYTES_2, 1, U_FIELD_BYTES_FLAG_FFA_PVP);
	summonhandler.RemoveFFAPvPFlags();
}

bool Creature::IsSanctuaryFlagged()
{
	return HasByteFlag(UNIT_FIELD_BYTES_2, 1, U_FIELD_BYTES_FLAG_SANCTUARY);
}

void Creature::SetSanctuaryFlag()
{
	SetByteFlag(UNIT_FIELD_BYTES_2, 1, U_FIELD_BYTES_FLAG_SANCTUARY);
	summonhandler.SetSanctuaryFlags();
}

void Creature::RemoveSanctuaryFlag()
{
	RemoveByteFlag(UNIT_FIELD_BYTES_2, 1, U_FIELD_BYTES_FLAG_SANCTUARY);
	summonhandler.RemoveSanctuaryFlags();
}

void Creature::SetSpeeds( uint8 type, float speed ){
	WorldPacket data( 50 );

	data << GetNewGUID();
	data << uint32( 0 );

	if( type == RUN )
		data << uint8( 0 );

	data << float( speed );

	switch( type ){

		case WALK:{
			data.SetOpcode( SMSG_FORCE_WALK_SPEED_CHANGE );
			m_walkSpeed = speed;
			break;}

		case RUN:{
			data.SetOpcode(SMSG_FORCE_RUN_SPEED_CHANGE);
			m_runSpeed = speed;
			break;}
		
		case RUNBACK:{
			data.SetOpcode(SMSG_FORCE_RUN_BACK_SPEED_CHANGE);
			m_backWalkSpeed = speed;
			break;}
		
		case SWIM:{
			data.SetOpcode(SMSG_FORCE_SWIM_SPEED_CHANGE);
			m_swimSpeed = speed;
			break;}
		
		case SWIMBACK:{
			data.SetOpcode(SMSG_FORCE_SWIM_BACK_SPEED_CHANGE);
			m_backSwimSpeed = speed;
			break;}

		case FLY:{
			data.SetOpcode(SMSG_FORCE_FLIGHT_SPEED_CHANGE);
			m_flySpeed = speed;
			break;}

		default:
			return;
	}

	SendMessageToSet( &data , true );
}

void Creature::PrepareForRemove()
{
	RemoveAllAuras();

	summonhandler.RemoveAllSummons();

	if(!IsInWorld())
		return;

	if(GetCreatedByGUID() != 0)
	{

		Unit* summoner = GetMapMgrUnit(GetCreatedByGUID());
		if(summoner != NULL)
		{
			if(summoner->GetSummonedCritterGUID() == GetGUID())
				summoner->SetSummonedCritterGUID(0);

			if(GetCreatedBySpell() != 0)
				summoner->RemoveAura(GetCreatedBySpell());
		}
	}

	if(GetMapMgr()->GetMapInfo() && GetMapMgr()->GetMapInfo()->type == INSTANCE_RAID)
	{
		if(GetCreatureInfo()->Rank == 3)
		{
			GetMapMgr()->RemoveCombatInProgress(GetGUID());
		}
	}
}

bool Creature::isCritter()
{
	if(creature_info->Type == UNIT_TYPE_CRITTER)
		return true;
	else
		return false;
}

void Creature::DealDamage(Unit* pVictim, uint32 damage, uint32 targetEvent, uint32 unitEvent, uint32 spellId, bool no_remove_auras)
{
	if(!pVictim || !pVictim->isAlive() || !pVictim->IsInWorld() || !IsInWorld())
		return;
	if(pVictim->IsPlayer() && TO< Player* >(pVictim)->GodModeCheat == true)
		return;
	if(pVictim->bInvincible)
		return;
	if(pVictim->IsSpiritHealer())
		return;

	if(pVictim != this)
		CombatStatus.OnDamageDealt(pVictim);

	pVictim->SetStandState(STANDSTATE_STAND);

	if(pVictim->IsPvPFlagged())
	{
		Player* p = TO< Player* >(GetPlayerOwner());

		if(p != NULL)
		{
			if(!p->IsPvPFlagged())
				p->PvPToggle();
			p->AggroPvPGuards();
		}
	}

	// Bg dmg counter
	if(pVictim != this)
	{
		Player* p = TO< Player* >(GetPlayerOwner());
		if(p != NULL)
		{
			if(p->m_bg != NULL && GetMapMgr() == pVictim->GetMapMgr())
			{
				p->m_bgScore.DamageDone += damage;
				p->m_bg->UpdatePvPData();
			}
		}
	}

	if(pVictim->GetHealth() <= damage)
	{
		if(pVictim->isTrainingDummy())
		{
			pVictim->SetHealth(1);
			return;
		}

		pVictim->Die(this, damage, spellId);
	}
	else
	{
		pVictim->TakeDamage(this, damage, spellId, no_remove_auras);
	}
}


void Creature::TakeDamage(Unit* pAttacker, uint32 damage, uint32 spellid, bool no_remove_auras)
{
	if(!no_remove_auras)
	{
		//zack 2007 04 24 : root should not remove self (and also other unknown spells)
		if(spellid)
		{
			RemoveAurasByInterruptFlagButSkip(AURA_INTERRUPT_ON_ANY_DAMAGE_TAKEN, spellid);
			if(Rand(35.0f))
				RemoveAurasByInterruptFlagButSkip(AURA_INTERRUPT_ON_UNUSED2, spellid);
		}
		else
		{
			RemoveAurasByInterruptFlag(AURA_INTERRUPT_ON_ANY_DAMAGE_TAKEN);
			if(Rand(35.0f))
				RemoveAurasByInterruptFlag(AURA_INTERRUPT_ON_UNUSED2);
		}
	}

	GetAIInterface()->AttackReaction(pAttacker, damage, spellid);

	ModHealth(-1 * static_cast< int32 >(damage));
}

void Creature::Die(Unit* pAttacker, uint32 damage, uint32 spellid)
{
	if( GetVehicleComponent() != NULL ){
		GetVehicleComponent()->RemoveAccessories();
		GetVehicleComponent()->EjectAllPassengers();
	}

	// Creature falls off vehicle on death
	if( ( currentvehicle != NULL ) )
		currentvehicle->EjectPassenger( this );

	//general hook for die
	if(!sHookInterface.OnPreUnitDie(pAttacker, this))
		return;

	// on die and an target die proc
	{
		SpellEntry* killerspell;
		if(spellid)
			killerspell = dbcSpell.LookupEntry(spellid);
		else killerspell = NULL;

		HandleProc(PROC_ON_DIE, this, killerspell);
		m_procCounter = 0;
		pAttacker->HandleProc(PROC_ON_TARGET_DIE, this, killerspell);
		pAttacker->m_procCounter = 0;
	}

	setDeathState(JUST_DIED);
	GetAIInterface()->HandleEvent(EVENT_LEAVECOMBAT, this, 0);


	if(GetChannelSpellTargetGUID() != 0)
	{

		Spell* spl = GetCurrentSpell();

		if(spl != NULL)
		{

			for(int i = 0; i < 3; i++)
			{
				if(spl->GetProto()->Effect[i] == SPELL_EFFECT_PERSISTENT_AREA_AURA)
				{
					uint64 guid = GetChannelSpellTargetGUID();
					DynamicObject* dObj = GetMapMgr()->GetDynamicObject(Arcemu::Util::GUID_LOPART(guid));
					if(!dObj)
						return;

					dObj->Remove();
				}
			}

			if(spl->GetProto()->ChannelInterruptFlags == 48140) spl->cancel();
		}
	}

	/* Stop players from casting */
	for(std::set< Object* >::iterator itr = GetInRangePlayerSetBegin() ; itr != GetInRangePlayerSetEnd() ; itr ++)
	{
		Unit* attacker = TO< Unit* >(*itr);

		if(attacker->GetCurrentSpell() != NULL)
		{
			if(attacker->GetCurrentSpell()->m_targets.m_unitTarget == GetGUID())
				attacker->GetCurrentSpell()->cancel();
		}
	}

	smsg_AttackStop(this);
	SetHealth(0);

	// Wipe our attacker set on death
	CombatStatus.Vanished();

	RemoveAllNonPersistentAuras();

	CALL_SCRIPT_EVENT(pAttacker, OnTargetDied)(this);
	pAttacker->smsg_AttackStop(this);

	/* Tell Unit that it's target has Died */
	pAttacker->addStateFlag(UF_TARGET_DIED);

	GetAIInterface()->OnDeath(pAttacker);

	SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DEAD);

	if((GetCreatedByGUID() == 0) && (GetTaggerGUID() != 0))
	{
		Unit* owner = m_mapMgr->GetUnit(GetTaggerGUID());

		if(owner != NULL)
			generateLoot();
	}

	if(GetCharmedByGUID())
	{
		//remove owner warlock soul link from caster
		Unit* owner = GetMapMgr()->GetUnit(GetCharmedByGUID());

		if(owner != NULL && owner->IsPlayer())
			TO_PLAYER(owner)->EventDismissPet();
	}

	if(GetCharmedByGUID() != 0)
	{
		Unit* charmer = m_mapMgr->GetUnit(GetCharmedByGUID());
		if(charmer != NULL)
			charmer->UnPossess();
	}

	if( m_mapMgr->m_battleground != NULL )
		m_mapMgr->m_battleground->HookOnUnitDied( this );
}

void Creature::SendChatMessage(uint8 type, uint32 lang, const char* msg, uint32 delay)
{
	if(delay)
	{
		sEventMgr.AddEvent(this, &Creature::SendChatMessage, type, lang, msg, uint32(0), EVENT_UNIT_CHAT_MSG, delay, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		return;
	}

	const char* name = GetCreatureInfo()->Name;
	size_t CreatureNameLength = strlen((char*)name) + 1;
	size_t MessageLength = strlen((char*)msg) + 1;

	WorldPacket data(SMSG_MESSAGECHAT, 35 + CreatureNameLength + MessageLength);
	data << type;
	data << lang;
	data << GetGUID();
	data << uint32(0);			// new in 2.1.0
	data << uint32(CreatureNameLength);
	data << name;
	data << uint64(0);
	data << uint32(MessageLength);
	data << msg;
	data << uint8(0x00);
	SendMessageToSet(&data, true);
}

void Creature::SendChatMessageToPlayer(uint8 type, uint32 lang, const char* msg, Player* plr)
{
	size_t UnitNameLength = 0, MessageLength = 0;
	if( plr == NULL)
		return;

	UnitNameLength = strlen((char*)GetCreatureInfo()->Name) + 1;
	MessageLength = strlen((char*)msg) + 1;

	WorldPacket data(SMSG_MESSAGECHAT, 35 + UnitNameLength + MessageLength);
	data << type;
	data << lang;
	data << GetGUID();
	data << uint32(0);			// new in 2.1.0
	data << uint32(UnitNameLength);
	data << GetCreatureInfo()->Name;
	data << uint64(0);
	data << uint32(MessageLength);
	data << msg;
	data << uint8(0x00);
	plr->GetSession()->SendPacket(&data);
}

void Creature::HandleMonsterSayEvent(MONSTER_SAY_EVENTS Event)
{
	NpcMonsterSay* ms = creature_info->MonsterSay[Event];
	if(ms == NULL)
		return;

	if(Rand(ms->Chance))
	{
		// chance successful.
		int choice = (ms->TextCount == 1) ? 0 : RandomUInt(ms->TextCount - 1);
		const char* text = ms->Texts[choice];
		// check for special variables $N=name $C=class $R=race $G=gender
		// $G is followed by male_string:female_string;
		string newText = text;
		static const char* races[12] = {"None", "Human", "Orc", "Dwarf", "Night Elf", "Undead", "Tauren", "Gnome", "Troll", "None", "Blood Elf", "Draenei"};
		static const char* classes[12] = {"None", "Warrior", "Paladin", "Hunter", "Rogue", "Priest", "Death Knight", "Shaman", "Mage", "Warlock", "None", "Druid"};
		char* test = strstr((char*)text, "$R");
		if(test == NULL)
			test = strstr((char*)text, "$r");
		if(test != NULL)
		{
			uint64 targetGUID = GetTargetGUID();
			Unit* CurrentTarget = GetMapMgr()->GetUnit(targetGUID);
			if(CurrentTarget)
			{
				ptrdiff_t testOfs = test - text;
				newText.replace(testOfs, 2, races[CurrentTarget->getRace()]);
			}
		}
		test = strstr((char*)text, "$N");
		if(test == NULL)
			test = strstr((char*)text, "$n");
		if(test != NULL)
		{
			uint64 targetGUID = GetTargetGUID();
			Unit* CurrentTarget = GetMapMgr()->GetUnit(targetGUID);
			if(CurrentTarget && CurrentTarget->IsPlayer())
			{
				ptrdiff_t testOfs = test - text;
				newText.replace(testOfs, 2, TO_PLAYER(CurrentTarget)->GetName());
			}
		}
		test = strstr((char*)text, "$C");
		if(test == NULL)
			test = strstr((char*)text, "$c");
		if(test != NULL)
		{
			uint64 targetGUID = GetTargetGUID();
			Unit* CurrentTarget = GetMapMgr()->GetUnit(targetGUID);
			if(CurrentTarget)
			{
				ptrdiff_t testOfs = test - text;
				newText.replace(testOfs, 2, classes[CurrentTarget->getClass()]);
			}
		}
		test = strstr((char*)text, "$G");
		if(test == NULL)
			test = strstr((char*)text, "$g");
		if(test != NULL)
		{
			uint64 targetGUID = GetTargetGUID();
			Unit* CurrentTarget = GetMapMgr()->GetUnit(targetGUID);
			if(CurrentTarget)
			{
				char* g0 = test + 2;
				char* g1 = strchr(g0, ':');
				if(g1)
				{
					char* gEnd = strchr(g1, ';');
					if(gEnd)
					{
						*g1 = 0x00;
						++g1;
						*gEnd = 0x00;
						++gEnd;
						*test = 0x00;
						newText = text;
						newText += (CurrentTarget->getGender() == 0) ? g0 : g1;
						newText += gEnd;
					}
				}
			}
		}

		SendChatMessage(static_cast<uint8>(ms->Type), ms->Language, newText.c_str());
	}
}

void Creature::BuildPetSpellList(WorldPacket & data)
{

	data << uint64(GetGUID());
	data << uint16( creature_info->Family );
	data << uint32(0);

	if( !IsVehicle() )
		data << uint32(0);
	else
		data << uint32( 0x8000101 );	

	std::vector< uint32 >::iterator itr = proto->castable_spells.begin();

	// Send the actionbar
	for(uint32 i = 0; i < 10; ++i)
	{
		if(itr != proto->castable_spells.end())
		{
			uint32 spell = *itr;
			data << uint32( Arcemu::Util::MAKE_UNIT_ACTION_BUTTON( spell, i + 8 ) );
			++itr;
		}
		else
		{
			data << uint16( 0 );
			data << uint8( 0 );
			data << uint8( i + 8 );
		}
	}

	data << uint8(0);
	// cooldowns
	data << uint8(0);
}

Object* Creature::GetPlayerOwner()
{
	return NULL;
}

void Creature::AddVehicleComponent( uint32 creature_entry, uint32 vehicleid ){
	if( vehicle != NULL ){
		LOG_ERROR( "Creature %u ( %s ) with GUID %u already has a vehicle component.", proto->Id, creature_info->Name, GetUIdFromGUID() );
		return;
	}

	vehicle = new Vehicle();
	vehicle->Load( this, creature_entry, vehicleid );
}

void Creature::RemoveVehicleComponent(){
	delete vehicle;
	vehicle = NULL;
}