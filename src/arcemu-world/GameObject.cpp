/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2014 <http://www.ArcEmu.org/>
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

GameObject::GameObject(){
	sLog.outError("GameObject being instantiated with default constructor.");

	ARCEMU_ASSERT(false);
}

GameObject::GameObject(uint64 guid)
{
	m_objectTypeId = TYPEID_GAMEOBJECT;
	m_valuesCount = GAMEOBJECT_END;
	m_uint32Values = _fields;
	memset(m_uint32Values, 0, (GAMEOBJECT_END)*sizeof(uint32));
	m_updateMask.SetCount(GAMEOBJECT_END);
	SetUInt32Value(OBJECT_FIELD_TYPE, TYPE_GAMEOBJECT | TYPE_OBJECT);
	SetGUID(guid);
	SetAnimProgress(100);
	m_wowGuid.Init(GetGUID());
	SetScale(1);  //info->Size  );
	bannerslot = bannerauraslot = -1;
	m_summonedGo = false;
	invisible = false;
	invisibilityFlag = INVIS_FLAG_NORMAL;
	m_summoner = NULL;
	charges = -1;
	m_ritualcaster = 0;
	m_ritualtarget = 0;
	m_ritualmembers = NULL;
	m_ritualspell = 0;
	m_quests = NULL;
	pInfo = NULL;
	myScript = NULL;
	m_spawn = 0;
	loot.gold = 0;
	m_deleted = false;
	usage_remaining = 1;
	m_respawnCell = NULL;
	m_rotation = 0;
	m_overrides = 0;
	hitpoints = 0;
	maxhitpoints = 0;
}

GameObject::~GameObject()
{
	sEventMgr.RemoveEvents(this);
	if(m_ritualmembers)
	{
		delete[] m_ritualmembers;
		m_ritualmembers = NULL;
	}

	if(myScript != NULL)
	{
		myScript->Destroy();
		myScript = NULL;
	}

	uint32 guid = GetUInt32Value(OBJECT_FIELD_CREATED_BY);
	if(guid)
	{
		Player* plr = objmgr.GetPlayer(guid);
		if(plr && plr->GetSummonedObject() == this)
			plr->SetSummonedObject(NULL);

		if(plr == m_summoner)
			m_summoner = 0;
	}

	if(m_respawnCell != NULL)
		m_respawnCell->_respawnObjects.erase(this);

	if(m_summonedGo && m_summoner)
		for(int i = 0; i < 4; i++)
			if(m_summoner->m_ObjectSlots[i] == GetLowGUID())
				m_summoner->m_ObjectSlots[i] = 0;
}

bool GameObject::CreateFromProto(uint32 entry, uint32 mapid, float x, float y, float z, float ang, float r0, float r1, float r2, float r3, uint32 overrides)
{
	Object::_Create(mapid, x, y, z, ang);
	SetEntry(entry);

	m_overrides = overrides;

	SetPosition(x, y, z, ang);
	SetParentRotation(0, r0);
	SetParentRotation(1, r1);
	SetParentRotation(2, r2);
	SetParentRotation(3, r3);
	UpdateRotation();
	SetByte(GAMEOBJECT_BYTES_1, 3, 0);
	SetByte(GAMEOBJECT_BYTES_1, 0, 1);
	SetDisplayId(pInfo->DisplayID);
	SetType(static_cast<uint8>(pInfo->Type));

	InitAI();
	_LoadQuests();

	return true;
}

/* Need to be checked if it is unused...
void GameObject::EventCastSpell(uint32 guid, uint32 sp, bool triggered)
{
	Spell* spp = sSpellFactoryMgr.NewSpell(this, dbcSpell.LookupEntry(sp), false, NULL);
	SpellCastTargets tars(guid);
	spp->prepare(&tars);
}

void GameObject::TrapSearchTarget()
{
	Update(100);
}*/

void GameObject::Update(unsigned long time_passed)
{
	if(m_event_Instanceid != m_instanceId)
	{
		event_Relocate();
		return;
	}

	if(!IsInWorld())
		return;

	if(m_deleted)
		return;
}

void GameObject::Spawn(MapMgr* m)
{
	PushToWorld(m);
}

void GameObject::Despawn(uint32 delay, uint32 respawntime)
{
	if(delay)
	{
		sEventMgr.AddEvent(this, &GameObject::Despawn, (uint32)0, respawntime, EVENT_GAMEOBJECT_EXPIRE, delay, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		return;
	}

	if(!IsInWorld())
		return;

	loot.items.clear();

	//This is for go get deleted while looting
	if(m_spawn)
	{
		SetState(static_cast<uint8>(m_spawn->state));
		SetFlags(m_spawn->flags);
	}

	CALL_GO_SCRIPT_EVENT(this, OnDespawn)();

	if(respawntime)
	{
		/* Get our originating mapcell */
		MapCell* pCell = GetMapCell();
		ARCEMU_ASSERT(pCell != NULL);
		pCell->_respawnObjects.insert(this);
		sEventMgr.RemoveEvents(this);
		sEventMgr.AddEvent(m_mapMgr, &MapMgr::EventRespawnGameObject, this, pCell->GetPositionX(), pCell->GetPositionY(), EVENT_GAMEOBJECT_ITEM_SPAWN, respawntime, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		Object::RemoveFromWorld(false);
		m_respawnCell = pCell;
	}
	else
	{
		Object::RemoveFromWorld(true);
		ExpireAndDelete();
	}
}

void GameObject::SaveToDB()
{
	if(m_spawn == NULL)
	{
		// Create spawn instance
		m_spawn = new GOSpawn;
		m_spawn->entry = GetEntry();
		m_spawn->facing = GetOrientation();
		m_spawn->faction = GetFaction();
		m_spawn->flags = GetUInt32Value(GAMEOBJECT_FLAGS);
		m_spawn->id = objmgr.GenerateGameObjectSpawnID();
		m_spawn->o1 = GetParentRotation(0);
		m_spawn->o2 = GetParentRotation(2);
		m_spawn->o3 = GetParentRotation(3);
		m_spawn->scale = GetScale();
		m_spawn->x = GetPositionX();
		m_spawn->y = GetPositionY();
		m_spawn->z = GetPositionZ();
		m_spawn->o = 0.0f;
		m_spawn->state = GetByte(GAMEOBJECT_BYTES_1, 0);
		m_spawn->phase = GetPhase();
		m_spawn->overrides = m_overrides;

		uint32 cx = GetMapMgr()->GetPosX(GetPositionX());
		uint32 cy = GetMapMgr()->GetPosY(GetPositionY());

		GetMapMgr()->GetBaseMap()->GetSpawnsListAndCreate(cx, cy)->GOSpawns.push_back(m_spawn);
	}
	std::stringstream ss;

	ss << "DELETE FROM gameobject_spawns WHERE id = ";
	ss << m_spawn->id;
	ss << ";";

	WorldDatabase.ExecuteNA(ss.str().c_str());

	ss.rdbuf()->str("");

	ss << "INSERT INTO gameobject_spawns VALUES("
	   << m_spawn->id << ","
	   << GetEntry() << ","
	   << GetMapId() << ","
	   << GetPositionX() << ","
	   << GetPositionY() << ","
	   << GetPositionZ() << ","
	   << GetOrientation() << ","
//		<< GetUInt64Value(GAMEOBJECT_ROTATION) << ","
	   << uint64(0) << ","
	   << GetParentRotation(0) << ","
	   << GetParentRotation(2) << ","
	   << GetParentRotation(3) << ","
	   << uint32(GetState()) << ","
	   << GetFlags() << ","
	   << GetFaction() << ","
	   << GetScale() << ","
	   << "0,"
	   << m_phase << ","
	   << m_overrides << ")";
	WorldDatabase.Execute(ss.str().c_str());
}

void GameObject::InitAI()
{
	if(!pInfo)
		return;

	if( pInfo->Type == GAMEOBJECT_TYPE_DESTRUCTIBLE_BUILDING )
		Rebuild();

	if(pInfo->Type == GAMEOBJECT_TYPE_RITUAL)
	{
		m_ritualmembers = new uint32[pInfo->raw.sound0];
		memset(m_ritualmembers, 0, sizeof(uint32)*pInfo->raw.sound0);
	}

	else if(pInfo->Type == GAMEOBJECT_TYPE_FISHINGHOLE)
	{
		CalcFishRemaining(true);
	}

	if(myScript == NULL)
		myScript = sScriptMgr.CreateAIScriptClassForGameObject(GetEntry(), this);
}

bool GameObject::Load(GOSpawn* spawn)
{
	if(!CreateFromProto(spawn->entry, 0, spawn->x, spawn->y, spawn->z, spawn->facing, spawn->o, spawn->o1, spawn->o2, spawn->o3, spawn->overrides))
		return false;

	m_spawn = spawn;
	m_phase = spawn->phase;
	//SetRotation(spawn->o);
	SetFlags(spawn->flags);
//	SetLevel(spawn->level);
	SetByte(GAMEOBJECT_BYTES_1, 0, static_cast<uint8>(spawn->state));
	if(spawn->faction)
	{
		SetFaction(spawn->faction);
	}
	SetScale(spawn->scale);

	return true;
}

void GameObject::DeleteFromDB()
{
	if(m_spawn != NULL)
		WorldDatabase.Execute("DELETE FROM gameobject_spawns WHERE id=%u", m_spawn->id);
}

void GameObject::EventCloseDoor()
{
	// gameobject_flags +1 closedoor animate restore the pointer flag.
	// by cebernic
	SetState(1);
	SetFlags(GetFlags() & ~1);
}

void GameObject::UseFishingNode(Player* player)
{
	sEventMgr.RemoveEvents(this);
	if (GetFlags() != 32)     // Clicking on the bobber before something is hooked
	{
		player->GetSession()->OutPacket(SMSG_FISH_NOT_HOOKED);
		EndFishing(player, true);
		return;
	}

	/* Unused code: sAreaStore.LookupEntry(GetMapMgr()->GetAreaID(GetPositionX(),GetPositionY()))->ZoneId*/
	uint32 zone = player->GetAreaID();
	if(zone == 0)   // If the player's area ID is 0, use the zone ID instead
		zone = player->GetZoneId();

	FishingZoneEntry* entry = FishingZoneStorage.LookupEntry(zone);
	if(entry == NULL)   // No fishing information found for area or zone, log an error, and end fishing
	{
		LOG_ERROR("ERROR: Fishing zone information for zone %d not found!", zone);
		EndFishing(player, true);
		return;
	}
	uint32 maxskill = entry->MaxSkill;
	uint32 minskill = entry->MinSkill;

	if(player->_GetSkillLineCurrent(SKILL_FISHING, false) < maxskill)
		player->_AdvanceSkillLine(SKILL_FISHING, float2int32(1.0f * sWorld.getRate(RATE_SKILLRATE)));

	GameObject* school = NULL;
	for(InRangeSet::iterator it = GetInRangeSetBegin(); it != GetInRangeSetEnd(); ++it)
	{
		if((*it) == NULL || !(*it)->IsGameObject() || TO_GAMEOBJECT(*it)->GetType() != GAMEOBJECT_TYPE_FISHINGHOLE)
			continue;
		school = TO< GameObject* >(*it);
		if(!isInRange(school, (float)school->GetInfo()->raw.sound1))
		{
			school = NULL;
			continue;
		}
		else
			break;
	}

	if(school != NULL)    // open school loot if school exists
	{

		if(school->GetMapMgr() != NULL)
			lootmgr.FillGOLoot(&school->loot, school->GetInfo()->raw.sound1, school->GetMapMgr()->iInstanceMode);
		else
			lootmgr.FillGOLoot(&school->loot, school->GetInfo()->raw.sound1, 0);

		player->SendLoot(school->GetGUID(), LOOT_FISHING, school->GetMapId());
		EndFishing(player, false);
		school->CatchFish();

		if(!school->CanFish())
			sEventMgr.AddEvent(school, &GameObject::Despawn, (uint32)0, (1800000 + RandomUInt(3600000)), EVENT_GAMEOBJECT_EXPIRE, 10000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);       // respawn in 30 - 90 minutes
	}
	else if(Rand(((player->_GetSkillLineCurrent(SKILL_FISHING, true) - minskill) * 100) / maxskill))           // Open loot on success, otherwise FISH_ESCAPED.
	{
		lootmgr.FillFishingLoot(&loot, zone);
		player->SendLoot(GetGUID(), LOOT_FISHING, GetMapId());
		EndFishing(player, false);
	}
	else // Failed
	{
		player->GetSession()->OutPacket(SMSG_FISH_ESCAPED);
		EndFishing(player, true);
	}

}

void GameObject::EndFishing(Player* player, bool abort)
{
	Spell* spell = player->GetCurrentSpell();

	if(spell)
	{
		if(abort)   // abort because of a reason
		{
			//FIX ME: here 'failed' should appear over progress bar
			spell->SendChannelUpdate(0);
			//spell->cancel();
			spell->finish(false);
		}
		else		// spell ended
		{
			spell->SendChannelUpdate(0);
			spell->finish();
		}
	}

	if(!abort)
		sEventMgr.AddEvent(this, &GameObject::ExpireAndDelete, EVENT_GAMEOBJECT_EXPIRE, 10000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	else
		ExpireAndDelete();
}

void GameObject::FishHooked(Player* player)
{
	WorldPacket  data(12);
	data.Initialize(SMSG_GAMEOBJECT_CUSTOM_ANIM);
	data << uint64(GetGUID());
	data << uint32(0); // value < 4
	player->GetSession()->SendPacket(&data);
	//SetByte(GAMEOBJECT_BYTES_1, 0, 0);
	//BuildFieldUpdatePacket(player, GAMEOBJECT_FLAGS, 32);
	SetFlags(32);
}

/////////////
/// Quests

void GameObject::AddQuest(QuestRelation* Q)
{
	m_quests->push_back(Q);
}

void GameObject::DeleteQuest(QuestRelation* Q)
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

Quest* GameObject::FindQuest(uint32 quest_id, uint8 quest_relation)
{
	list< QuestRelation* >::iterator it;
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

uint16 GameObject::GetQuestRelation(uint32 quest_id)
{
	uint16 quest_relation = 0;
	list< QuestRelation* >::iterator it;
	for(it = m_quests->begin(); it != m_quests->end(); ++it)
	{
		if((*it) != NULL && (*it)->qst->id == quest_id)
		{
			quest_relation |= (*it)->type;
		}
	}
	return quest_relation;
}

uint32 GameObject::NumOfQuests()
{
	return (uint32)m_quests->size();
}

void GameObject::_LoadQuests()
{
	sQuestMgr.LoadGOQuests(this);
}

void GameObject::_Expire()
{
	sEventMgr.RemoveEvents(this);

	if(IsInWorld())
		RemoveFromWorld(true);

	delete this;
}

void GameObject::ExpireAndDelete()
{
	if(m_deleted)
		return;

	m_deleted = true;

	//! remove any events
	sEventMgr.RemoveEvents(this);
	if(IsInWorld())
		sEventMgr.AddEvent(this, &GameObject::_Expire, EVENT_GAMEOBJECT_EXPIRE, 1, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	else
		delete this;
}

void GameObject::CallScriptUpdate()
{
	ARCEMU_ASSERT(myScript != NULL);
	myScript->AIUpdate();
}

void GameObject::OnPushToWorld()
{
	Object::OnPushToWorld();
	CALL_GO_SCRIPT_EVENT(this, OnCreate)();
	CALL_GO_SCRIPT_EVENT(this, OnSpawn)();
	CALL_INSTANCE_SCRIPT_EVENT(m_mapMgr, OnGameObjectPushToWorld)(this);

	// We have a field supposedly for this, but it's pointless to waste CPU time for this
	// unless it's longer than a minute ( since usually then it's much longer )
	if( ( pInfo->Type == GAMEOBJECT_TYPE_CHEST ) && ( pInfo->raw.sound3 == 0 ) ){
		time_t restockTime = 60 * 1000;
		if( pInfo->raw.sound2 > 60 )
			restockTime = pInfo->raw.sound2 * 1000;

		EventMgr::getSingleton().AddEvent( this, &GameObject::ReStock, EVENT_GO_CHEST_RESTOCK, restockTime, 0, 0 );
	}
}

void GameObject::OnRemoveInRangeObject(Object* pObj)
{
	Object::OnRemoveInRangeObject(pObj);
	if(m_summonedGo && m_summoner == pObj)
	{
		for(int i = 0; i < 4; i++)
			if(m_summoner->m_ObjectSlots[i] == GetLowGUID())
				m_summoner->m_ObjectSlots[i] = 0;

		m_summoner = 0;
		ExpireAndDelete();
	}
}
//! Remove gameobject from world, using their despawn animation.
void GameObject::RemoveFromWorld(bool free_guid)
{
	WorldPacket data(SMSG_GAMEOBJECT_DESPAWN_ANIM, 8);
	data << GetGUID();
	SendMessageToSet(&data, true);

	sEventMgr.RemoveEvents( this );
	Object::RemoveFromWorld(free_guid);
}

//! Gameobject contains loot ex. chest
bool GameObject::HasLoot()
{
	if(loot.gold > 0)
		return true;

	for(vector<__LootItem>::iterator itr = loot.items.begin(); itr != loot.items.end(); ++itr)
	{
		if(itr->item.itemproto->Bonding == ITEM_BIND_QUEST || itr->item.itemproto->Bonding == ITEM_BIND_QUEST2)
			continue;

		if(itr->iItemsCount > 0)
			return true;
	}
	return false;
}

uint32 GameObject::GetGOReqSkill()
{
	//! Here we check the SpellFocus table against the dbcs
	Lock *lock = dbcLock.LookupEntryForced(GetInfo()->raw.sound0);
	if(!lock) return 0;
	for(uint32 i = 0; i < LOCK_NUM_CASES; i++)
	{
		if(lock->locktype[i] == 2 && lock->minlockskill[i])
			return lock->minlockskill[i];
	}
	return 0;
}
//! Set GameObject rotational value
void GameObject::SetRotation(float rad)
{
	if(rad > M_PI_FLOAT)
		rad -= 2 * M_PI_FLOAT;
	else if(rad < -M_PI_FLOAT)
		rad += 2 * M_PI_FLOAT;
	float sin = sinf(rad / 2.f);

	if(sin >= 0)
		rad = 1.f + 0.125f * sin;
	else
		rad = 1.25f + 0.125f * sin;
}

void GameObject::UpdateRotation()
{
	static double const atan_pow = atan(pow(2.0f, -20.0f));

	double f_rot1 = sin(GetOrientation() / 2.0f);
	double f_rot2 = cos(GetOrientation() / 2.0f);

	int64 i_rot1 = int64(f_rot1 / atan_pow * (f_rot2 >= 0 ? 1.0f : -1.0f));
	int64 rotation = (i_rot1 << 43 >> 43) & 0x00000000001FFFFF;

	m_rotation = rotation;

	float r2 = GetParentRotation(2);
	float r3 = GetParentRotation(3);
	if(r2 == 0.0f && r3 == 0.0f && !(m_overrides & GAMEOBJECT_OVERRIDE_PARENTROT))
	{
		r2 = (float)f_rot1;
		r3 = (float)f_rot2;
		SetParentRotation(2, r2);
		SetParentRotation(3, r3);
	}
}

void GameObject::Damage( uint32 damage, uint64 AttackerGUID, uint64 ControllerGUID, uint32 SpellID ){
	// If we are already destroyed there's nothing to damage!
	if( hitpoints == 0 )
		return;
	
	if( damage >= hitpoints ){
		// Instant destruction
		hitpoints = 0;
		
		SetFlags(GAMEOBJECT_FLAG_DESTROYED);
		SetFlags(GetFlags() & ~GAMEOBJECT_FLAG_DAMAGED);
		SetDisplayId(pInfo->raw.sound9); // destroyed display id

		CALL_GO_SCRIPT_EVENT( this, OnDestroyed)();
	
	}else{
		// Simply damaging
		hitpoints -= damage;
		
		if( !HasFlags( GAMEOBJECT_FLAG_DAMAGED ) ){
			// Intact  ->  Damaged
			
			// Are we below the intact-damaged transition treshold?
			if (hitpoints <= (maxhitpoints - pInfo->raw.sound0)){
				SetFlags( GAMEOBJECT_FLAG_DAMAGED );
				SetDisplayId(pInfo->raw.sound4); // damaged display id
			}
		}

		CALL_GO_SCRIPT_EVENT( this, OnDamaged )( damage );
	}
	
	uint8 animprogress = static_cast< uint8 >( Arcemu::round( hitpoints/ float( maxhitpoints ) ) * 255 );
	SetAnimProgress( animprogress );
	SendDamagePacket( damage, AttackerGUID, ControllerGUID, SpellID );
}

void GameObject::SendDamagePacket( uint32 damage, uint64 AttackerGUID, uint64 ControllerGUID, uint32 SpellID ){
	WorldPacket data( SMSG_DESTRUCTIBLE_BUILDING_DAMAGE, 29 );
	
	data << WoWGuid( GetNewGUID() );
	data << WoWGuid( AttackerGUID );
	data << WoWGuid( ControllerGUID );
	data << uint32( damage );
	data << uint32( SpellID );
	SendMessageToSet( &data, false, false );
}

void GameObject::Rebuild(){
	SetFlags( GetFlags() & uint32( ~( GAMEOBJECT_FLAG_DAMAGED | GAMEOBJECT_FLAG_DESTROYED ) ) );
	SetDisplayId( pInfo->DisplayID );
	maxhitpoints = pInfo->raw.sound0 + pInfo->raw.sound5;
	hitpoints = maxhitpoints;
}

void GameObject::ReStock(){
	// this hasn't been looted yet so we don't want to restock
	if( loot.items.empty() )
		return;

	if( !loot.looters.empty() )
		return;

	if( loot.HasRoll() )
		return;

	lootmgr.FillGOLoot(&loot, pInfo->raw.sound1, m_mapMgr->iInstanceMode);
}

