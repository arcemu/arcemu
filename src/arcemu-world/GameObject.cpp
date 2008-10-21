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

#include "StdAfx.h"
GameObject::GameObject(uint64 guid)
{
	m_objectTypeId = TYPEID_GAMEOBJECT;
	m_valuesCount = GAMEOBJECT_END;
	m_uint32Values = _fields;
	memset(m_uint32Values, 0,(GAMEOBJECT_END)*sizeof(uint32));
	m_updateMask.SetCount(GAMEOBJECT_END);
	SetUInt32Value( OBJECT_FIELD_TYPE,TYPE_GAMEOBJECT|TYPE_OBJECT);
	SetUInt64Value( OBJECT_FIELD_GUID,guid);
	SetUInt32Value(GAMEOBJECT_ANIMPROGRESS, 100);
	m_wowGuid.Init(GetGUID());
 
	SetFloatValue( OBJECT_FIELD_SCALE_X, 1);//info->Size  );

	counter=0;//not needed at all but to prevent errors that var was not inited, can be removed in release

	bannerslot = bannerauraslot = -1;

	m_summonedGo = false;
	invisible = false;
	invisibilityFlag = INVIS_FLAG_NORMAL;
	spell = 0;
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
	m_respawnCell=NULL;
	m_battleground = NULL;
}

GameObject::~GameObject()
{
	sEventMgr.RemoveEvents(this);
	if(m_ritualmembers) {
		delete[] m_ritualmembers;
		m_ritualmembers = NULL;
	}

	uint32 guid = GetUInt32Value(OBJECT_FIELD_CREATED_BY);
	if(guid)
	{
		Player *plr = objmgr.GetPlayer(guid);
		if(plr && plr->GetSummonedObject() == this)
			plr->SetSummonedObject(NULL);

		if(plr == m_summoner)
			m_summoner = 0;
	}

	if(m_respawnCell!=NULL)
		m_respawnCell->_respawnObjects.erase(this);

	if (m_summonedGo && m_summoner)
		for(int i = 0; i < 4; i++)
			if (m_summoner->m_ObjectSlots[i] == GetLowGUID())
				m_summoner->m_ObjectSlots[i] = 0;

	if( m_battleground != NULL && m_battleground->GetType() == BATTLEGROUND_ARATHI_BASIN )
	{
		if( bannerslot >= 0 && static_cast<ArathiBasin*>(m_battleground)->m_controlPoints[bannerslot] == this )
			static_cast<ArathiBasin*>(m_battleground)->m_controlPoints[bannerslot] = NULL;

		if( bannerauraslot >= 0 && static_cast<ArathiBasin*>(m_battleground)->m_controlPointAuras[bannerauraslot] == this )
			static_cast<ArathiBasin*>(m_battleground)->m_controlPointAuras[bannerauraslot] = NULL;
	}
}

bool GameObject::CreateFromProto(uint32 entry,uint32 mapid, float x, float y, float z, float ang)
{
	pInfo= GameObjectNameStorage.LookupEntry(entry);
	if(!pInfo)return false;

	Object::_Create( mapid, x, y, z, ang );
	SetUInt32Value( OBJECT_FIELD_ENTRY, entry );
	
	SetFloatValue( GAMEOBJECT_POS_X, x );
	SetFloatValue( GAMEOBJECT_POS_Y, y );
	SetFloatValue( GAMEOBJECT_POS_Z, z );
	SetFloatValue( GAMEOBJECT_FACING, ang );
	 
	//SetUInt32Value( GAMEOBJECT_TIMESTAMP, (uint32)UNIXTIME);
 //   SetUInt32Value( GAMEOBJECT_ARTKIT, 0 );		   //these must be from wdb somewhere i guess
   // SetUInt32Value( GAMEOBJECT_ANIMPROGRESS, 0 );
	SetUInt32Value( GAMEOBJECT_STATE, 1 );
	SetUInt32Value( GAMEOBJECT_DISPLAYID, pInfo->DisplayID );
	SetUInt32Value( GAMEOBJECT_TYPE_ID, pInfo->Type );
   
	InitAI();

	 return true;
	/*
	original_flags = m_uint32Values[GAMEOBJECT_FLAGS];
	original_state = m_uint32Values[GAMEOBJECT_STATE];
	*/
}
/*
void GameObject::Create(uint32 mapid, float x, float y, float z, float ang)
{
	Object::_Create( mapid, x, y, z, ang);

	SetFloatValue( GAMEOBJECT_POS_X, x);
	SetFloatValue( GAMEOBJECT_POS_Y, y );
	SetFloatValue( GAMEOBJECT_POS_Z, z );
	SetFloatValue( GAMEOBJECT_FACING, ang );
	//SetUInt32Value( GAMEOBJECT_TIMESTAMP, (uint32)time(NULL));
}

void GameObject::Create( uint32 guidlow, uint32 guidhigh,uint32 displayid, uint8 state, uint32 entryid, float scale,uint32 typeId, uint32 type,uint32 flags, uint32 mapid, float x, float y, float z, float ang )
{
	Object::_Create( mapid, x, y, z, ang);

	SetUInt32Value( OBJECT_FIELD_ENTRY, entryid );
	SetFloatValue( OBJECT_FIELD_SCALE_X, scale );
	SetUInt32Value( GAMEOBJECT_DISPLAYID, displayid );
	SetUInt32Value( GAMEOBJECT_STATE, state  );
	SetUInt32Value( GAMEOBJECT_TYPE_ID, typeId  );
	SetUInt32Value( GAMEOBJECT_FLAGS, flags );
}*/

void GameObject::TrapSearchTarget()
{
	Update(100);
}

void GameObject::Update(uint32 p_time)
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

	if(spell && (GetUInt32Value(GAMEOBJECT_STATE) == 1))
	{
		if(checkrate > 1)
		{
			if(counter++%checkrate)
				return;
		}
		ObjectSet::iterator itr = GetInRangeSetBegin();
		ObjectSet::iterator it2 = itr;
		ObjectSet::iterator iend = GetInRangeSetEnd();
		Unit * pUnit;
		float dist;
		this->AquireInrangeLock(); //make sure to release lock before exit function !
		for(; it2 != iend;)
		{
			itr = it2;
			++it2;
			dist = GetDistanceSq((*itr));
			if( (*itr) != m_summoner && (*itr)->IsUnit() && dist <= range)
			{
				pUnit = static_cast<Unit*>(*itr);

				if(m_summonedGo)
				{
					if(!m_summoner)
					{
						ExpireAndDelete();
						return;
					}
					if(!isAttackable(m_summoner,pUnit))continue;
				}
				
				Spell * sp=SpellPool.PooledNew();
				sp->Init((Object*)this,spell,true,NULL);
				SpellCastTargets tgt((*itr)->GetGUID());
				tgt.m_destX = GetPositionX();
				tgt.m_destY = GetPositionY();
				tgt.m_destZ = GetPositionZ();
				sp->prepare(&tgt);

				// proc on trap trigger
				if( pInfo->Type == GAMEOBJECT_TYPE_TRAP )
				{
					if( m_summoner != NULL )
						m_summoner->HandleProc( PROC_ON_TRAP_TRIGGER, pUnit, spell );
				} 

				if(m_summonedGo)
				{
					ExpireAndDelete();
					this->ReleaseInrangeLock();
					return;
				}

				if(spell->EffectImplicitTargetA[0] == 16 ||
					spell->EffectImplicitTargetB[0] == 16)
				{
					this->ReleaseInrangeLock();
					return;	 // on area dont continue.
				}
			}
		}
		this->ReleaseInrangeLock();
	}
}

void GameObject::Spawn(MapMgr * m)
{
	PushToWorld(m);	
	CALL_GO_SCRIPT_EVENT(this, OnSpawn)();
}

void GameObject::Despawn(uint32 time)
{
	if(!IsInWorld())
		return;

	loot.items.clear();

	//This is for go get deleted while looting
	if(m_spawn)
	{
		SetUInt32Value(GAMEOBJECT_STATE, m_spawn->state);
		SetUInt32Value(GAMEOBJECT_FLAGS, m_spawn->flags);
	}

	CALL_GO_SCRIPT_EVENT(this, OnDespawn)();

	if(time)
	{
		/* Get our originiating mapcell */
		MapCell * pCell = m_mapCell;
		ASSERT(pCell);
		pCell->_respawnObjects.insert( ((Object*)this) );
		sEventMgr.RemoveEvents(this);
		sEventMgr.AddEvent(m_mapMgr, &MapMgr::EventRespawnGameObject, this, pCell, EVENT_GAMEOBJECT_ITEM_SPAWN, time, 1, 0);
		Object::RemoveFromWorld(false);
		m_respawnCell=pCell;
	}
	else
	{
		Object::RemoveFromWorld(true);
		ExpireAndDelete();
	}
}

void GameObject::SaveToDB()
{
	std::stringstream ss;
	ss << "REPLACE INTO gameobject_spawns VALUES("
		<< ((m_spawn == NULL) ? 0 : m_spawn->id) << ","
		<< GetEntry() << ","
		<< GetMapId() << ","
		<< GetPositionX() << ","
		<< GetPositionY() << ","
		<< GetPositionZ() << ","
		<< GetOrientation() << ","
		<< GetFloatValue(GAMEOBJECT_ROTATION) << ","
		<< GetFloatValue(GAMEOBJECT_ROTATION_01) << ","
		<< GetFloatValue(GAMEOBJECT_ROTATION_02) << ","
		<< GetFloatValue(GAMEOBJECT_ROTATION_03) << ","
		<< GetUInt32Value(GAMEOBJECT_STATE) << ","
		<< GetUInt32Value(GAMEOBJECT_FLAGS) << ","
		<< GetUInt32Value(GAMEOBJECT_FACTION) << ","
		<< GetFloatValue(OBJECT_FIELD_SCALE_X) << ","
		<< "0)";
	WorldDatabase.Execute(ss.str().c_str());

  /*  std::stringstream ss;
	if (!m_sqlid)
		m_sqlid = objmgr.GenerateLowGuid(HIGHGUID_GAMEOBJECT);

	ss << "DELETE FROM gameobjects WHERE id=" << m_sqlid;
	sDatabase.Execute(ss.str().c_str());

	ss.rdbuf()->str("");
	ss << "INSERT INTO gameobjects VALUES ( "
		<< m_sqlid << ", "
		<< m_position.x << ", "
		<< m_position.y << ", "
		<< m_position.z << ", "
		<< m_position.o << ", "
		<< GetZoneId() << ", "
		<< GetMapId() << ", '";

	for( uint32 index = 0; index < m_valuesCount; index ++ )
		ss << GetUInt32Value(index) << " ";

	ss << "', ";
	ss << GetEntry() << ", 0, 0)"; 

	sDatabase.Execute( ss.str( ).c_str( ) );*/
}

void GameObject::SaveToFile(std::stringstream & name)
{

	std::stringstream ss;

	ss << "REPLACE INTO gameobject_spawns VALUES("
		<< ((m_spawn == NULL) ? 0 : m_spawn->id) << ","
		<< GetEntry() << ","
		<< GetMapId() << ","
		<< GetPositionX() << ","
		<< GetPositionY() << ","
		<< GetPositionZ() << ","
		<< GetOrientation() << ","
		<< GetFloatValue(GAMEOBJECT_ROTATION) << ","
		<< GetFloatValue(GAMEOBJECT_ROTATION_01) << ","
		<< GetFloatValue(GAMEOBJECT_ROTATION_02) << ","
		<< GetFloatValue(GAMEOBJECT_ROTATION_03) << ","
		<< GetUInt32Value(GAMEOBJECT_STATE) << ","
		<< GetUInt32Value(GAMEOBJECT_FLAGS) << ","
		<< GetUInt32Value(GAMEOBJECT_FACTION) << ","
		<< GetFloatValue(OBJECT_FIELD_SCALE_X) << ","
		<< "0)";

	FILE * OutFile;

	OutFile = fopen(name.str().c_str(), "wb");
	if (!OutFile) return;
	fwrite(ss.str().c_str(),1,ss.str().size(),OutFile);
	fclose(OutFile);

}

void GameObject::InitAI()
{
	
	if(!pInfo)
		return;
	
	// this fixes those fuckers in booty bay
	if(pInfo->SpellFocus == 0 &&
		pInfo->sound1 == 0 &&
		pInfo->sound2 == 0 &&
		pInfo->sound3 != 0 &&
		pInfo->sound5 != 3 &&
		pInfo->sound9 == 1)
		return;

	if(pInfo->DisplayID == 1027)//Shaman Shrine
	{
		if(pInfo->ID != 177964 && pInfo->ID != 153556)
		{
			//Deactivate
			//SetUInt32Value(GAMEOBJECT_DYN_FLAGS, 0);
		}
	}


	uint32 spellid = 0;
	if(pInfo->Type==GAMEOBJECT_TYPE_TRAP)
	{	
		spellid = pInfo->sound3;
	}
	else if(pInfo->Type == GAMEOBJECT_TYPE_SPELL_FOCUS)
	{
		// get spellid from attached gameobject - by sound2 field
		if( pInfo->sound2 == 0 )
			return;
		if( GameObjectNameStorage.LookupEntry( pInfo->sound2 ) == NULL )
			return;
		spellid = GameObjectNameStorage.LookupEntry( pInfo->sound2 )->sound3;
	}
	else if(pInfo->Type == GAMEOBJECT_TYPE_RITUAL)
	{	
		m_ritualmembers = new uint32[pInfo->SpellFocus];
		memset(m_ritualmembers,0,sizeof(uint32)*pInfo->SpellFocus);
	}
    else if(pInfo->Type == GAMEOBJECT_TYPE_CHEST)
    {
        Lock *pLock = dbcLock.LookupEntry(GetInfo()->SpellFocus);
        if(pLock)
        {
            for(uint32 i=0; i < 5; i++)
            {
                if(pLock->locktype[i])
                {
                    if(pLock->locktype[i] == 2) //locktype;
                    {
                        //herbalism and mining;
                        if(pLock->lockmisc[i] == LOCKTYPE_MINING || pLock->lockmisc[i] == LOCKTYPE_HERBALISM)
                        {
							CalcMineRemaining(true);
                        }
                    }
                }
            }
        }

    }
	else if ( pInfo->Type == GAMEOBJECT_TYPE_FISHINGHOLE )
	{
		CalcFishRemaining( true );
	}

	myScript = sScriptMgr.CreateAIScriptClassForGameObject(GetEntry(), this);

	// hackfix for bad spell in BWL
	if(!spellid || spellid == 22247)
		return;

	SpellEntry *sp= dbcSpell.LookupEntry(spellid);
	if(!sp)
	{
		spell = NULL;
		return;
	}
	else
	{
		spell = sp;
	}
	//ok got valid spell that will be casted on target when it comes close enough
	//get the range for that 
	
	float r = 0;

	for(uint32 i=0;i<3;i++)
	{
		if(sp->Effect[i])
		{
			float t = GetRadius(dbcSpellRadius.LookupEntry(sp->EffectRadiusIndex[i]));
			if(t > r)
				r = t;
		}
	}

	if(r < 0.1)//no range
		r = GetMaxRange(dbcSpellRange.LookupEntry(sp->rangeIndex));

	range = r*r;//square to make code faster
	checkrate = 20;//once in 2 seconds
	
}

bool GameObject::Load(GOSpawn *spawn)
{
	if(!CreateFromProto(spawn->entry,0,spawn->x,spawn->y,spawn->z,spawn->facing))
		return false;

	m_spawn = spawn;
	SetFloatValue(GAMEOBJECT_ROTATION,spawn->o);
	SetFloatValue(GAMEOBJECT_ROTATION_01 ,spawn->o1);
	SetFloatValue(GAMEOBJECT_ROTATION_02 ,spawn->o2);
	SetFloatValue(GAMEOBJECT_ROTATION_03 ,spawn->o3);
	SetUInt32Value(GAMEOBJECT_FLAGS,spawn->flags);
//	SetUInt32Value(GAMEOBJECT_LEVEL,spawn->level);
	SetUInt32Value(GAMEOBJECT_STATE,spawn->state);	
	if(spawn->faction)
	{
		SetUInt32Value(GAMEOBJECT_FACTION,spawn->faction);
		m_faction = dbcFactionTemplate.LookupEntry(spawn->faction);
		if(m_faction)
			m_factionDBC = dbcFaction.LookupEntry(m_faction->Faction);
	}
	SetFloatValue(OBJECT_FIELD_SCALE_X,spawn->scale);
	_LoadQuests();
	CALL_GO_SCRIPT_EVENT(this, OnCreate)();
	CALL_GO_SCRIPT_EVENT(this, OnSpawn)();

	InitAI();

	_LoadQuests();
	return true;
}

void GameObject::DeleteFromDB()
{
	if( m_spawn != NULL )
		WorldDatabase.Execute("DELETE FROM gameobject_spawns WHERE id=%u", m_spawn->id);
}

void GameObject::EventCloseDoor()
{
// gameobject_flags +1 closedoor animate restore the pointer flag.
// by cebernic
	SetUInt32Value(GAMEOBJECT_STATE, 1);
  SetUInt32Value(GAMEOBJECT_FLAGS, GetUInt32Value( GAMEOBJECT_FLAGS ) & ~1);
}

void GameObject::UseFishingNode(Player *player)
{
	sEventMgr.RemoveEvents( this );
	if( GetUInt32Value( GAMEOBJECT_FLAGS ) != 32 ) // Clicking on the bobber before something is hooked
	{
		player->GetSession()->OutPacket( SMSG_FISH_NOT_HOOKED );
		EndFishing( player, true );
		return;
	}
	
	/* Unused code: sAreaStore.LookupEntry(GetMapMgr()->GetAreaID(GetPositionX(),GetPositionY()))->ZoneId*/
	uint32 zone = player->GetAreaID();
	if( zone == 0 ) // If the player's area ID is 0, use the zone ID instead
		zone = player->GetZoneId();

	FishingZoneEntry *entry = FishingZoneStorage.LookupEntry( zone );
	if( entry == NULL ) // No fishing information found for area or zone, log an error, and end fishing
	{
		sLog.outError( "ERROR: Fishing zone information for zone %d not found!", zone );
		EndFishing( player, true );
		return;
	}
	uint32 maxskill = entry->MaxSkill;
	uint32 minskill = entry->MinSkill;

	if( player->_GetSkillLineCurrent( SKILL_FISHING, false ) < maxskill )	
		player->_AdvanceSkillLine( SKILL_FISHING, float2int32( 1.0f * sWorld.getRate( RATE_SKILLRATE ) ) );

	GameObject * school = NULL;
	this->AquireInrangeLock(); //make sure to release lock before exit function !
	for ( InRangeSet::iterator it = GetInRangeSetBegin(); it != GetInRangeSetEnd(); ++it )
	{
		if ( (*it) == NULL || (*it)->GetTypeId() != TYPEID_GAMEOBJECT || (*it)->GetUInt32Value(GAMEOBJECT_TYPE_ID) != GAMEOBJECT_TYPE_FISHINGHOLE)
			continue;
		school = static_cast<GameObject *>( *it );
		if ( !isInRange( school, (float)school->GetInfo()->sound1 ) )
		{
			school = NULL;
			continue;
		}
		else
			break;
	}
	this->ReleaseInrangeLock();

	if ( school != NULL ) // open school loot if school exists
	{
		lootmgr.FillGOLoot( &school->loot, school->GetEntry(), school->GetMapMgr() ? ( school->GetMapMgr()->iInstanceMode ? true : false ) : false );
		player->SendLoot( school->GetGUID(), LOOT_FISHING );
		EndFishing( player, false );
		school->CatchFish();
		if ( !school->CanFish() )
			sEventMgr.AddEvent( school, &GameObject::Despawn, ( 1800000 + RandomUInt( 3600000 ) ), EVENT_GAMEOBJECT_EXPIRE, 10000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT ); // respawn in 30 - 90 minutes
	}
	else if( Rand( ( ( player->_GetSkillLineCurrent( SKILL_FISHING, true ) - minskill ) * 100 ) / maxskill ) ) // Open loot on success, otherwise FISH_ESCAPED.
	{
		lootmgr.FillFishingLoot( &loot, zone );
		player->SendLoot( GetGUID(), LOOT_FISHING );
		EndFishing( player, false );
	}
	else // Failed
	{
		player->GetSession()->OutPacket( SMSG_FISH_ESCAPED );
		EndFishing( player, true );
	}

}

void GameObject::EndFishing(Player* player, bool abort )
{
	Spell * spell = player->GetCurrentSpell();
	
	if(spell)
	{
		if(abort)   // abort becouse of a reason
		{
			//FIXME: here 'failed' should appear over progress bar
			spell->SendChannelUpdate(0);
			//spell->cancel();
			spell->finish();
		}
		else		// spell ended
		{
			spell->SendChannelUpdate(0);
			spell->finish();
		}
	}

	if(!abort)
		sEventMgr.AddEvent(this, &GameObject::ExpireAndDelete, EVENT_GAMEOBJECT_EXPIRE, 10000, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	else
		ExpireAndDelete();
}

void GameObject::FishHooked(Player * player)
{
	WorldPacket  data(12);
	data.Initialize(SMSG_GAMEOBJECT_CUSTOM_ANIM); 
	data << GetGUID();
	data << (uint32)(0); // value < 4
	player->GetSession()->SendPacket(&data);
	//SetUInt32Value(GAMEOBJECT_STATE, 0);
	//BuildFieldUpdatePacket(player, GAMEOBJECT_FLAGS, 32);
	SetUInt32Value(GAMEOBJECT_FLAGS, 32);
 }

/////////////
/// Quests

void GameObject::AddQuest(QuestRelation *Q)
{
	m_quests->push_back(Q);
}

void GameObject::DeleteQuest(QuestRelation *Q)
{
	list<QuestRelation *>::iterator it;
	for( it = m_quests->begin(); it != m_quests->end(); ++it )
	{
		if( ( (*it)->type == Q->type ) && ( (*it)->qst == Q->qst ) )
		{
			delete (*it);
			m_quests->erase(it);
			break;
		}
	}
}

Quest* GameObject::FindQuest(uint32 quest_id, uint8 quest_relation)
{   
	list< QuestRelation* >::iterator it;
	for( it = m_quests->begin(); it != m_quests->end(); ++it )
	{
		QuestRelation* ptr = (*it);
		if( ( ptr->qst->id == quest_id ) && ( ptr->type & quest_relation ) )
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
	for( it = m_quests->begin(); it != m_quests->end(); ++it )
	{
		if( (*it) != NULL && (*it)->qst->id == quest_id )
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

/////////////////
// Summoned Go's

void GameObject::_Expire()
{
	sEventMgr.RemoveEvents(this);
	if(IsInWorld())
		RemoveFromWorld(true);

	//sEventMgr.AddEvent(World::getSingletonPtr(), &World::DeleteObject, ((Object*)this), EVENT_DELETE_TIMER, 1000, 1);
	delete this;
}

void GameObject::ExpireAndDelete()
{
	if(m_deleted)
		return;

	m_deleted = true;
	
	/* remove any events */
	sEventMgr.RemoveEvents(this);
	sEventMgr.AddEvent(this, &GameObject::_Expire, EVENT_GAMEOBJECT_EXPIRE, 1, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
}

void GameObject::Deactivate()
{
	SetUInt32Value(GAMEOBJECT_DYN_FLAGS, 0);
}

void GameObject::CallScriptUpdate()
{
	ASSERT(myScript);
	myScript->AIUpdate();
}

void GameObject::OnPushToWorld()
{
	Object::OnPushToWorld();
}

void GameObject::OnRemoveInRangeObject(Object* pObj)
{
	Object::OnRemoveInRangeObject(pObj);
	if(m_summonedGo && m_summoner == pObj)
	{
		for(int i = 0; i < 4; i++)
			if (m_summoner->m_ObjectSlots[i] == GetLowGUID())
				m_summoner->m_ObjectSlots[i] = 0;

		m_summoner = 0;
		ExpireAndDelete();
	}
}

void GameObject::RemoveFromWorld(bool free_guid)
{
	WorldPacket data(SMSG_GAMEOBJECT_DESPAWN_ANIM, 8);
	data << GetGUID();
	SendMessageToSet(&data,true);

	sEventMgr.RemoveEvents(this, EVENT_GAMEOBJECT_TRAP_SEARCH_TARGET);
	Object::RemoveFromWorld(free_guid);
}

bool GameObject::HasLoot()
{
    int count=0;
    for(vector<__LootItem>::iterator itr = loot.items.begin(); itr != loot.items.end(); ++itr)
	{
		if( itr->item.itemproto->Bonding == ITEM_BIND_QUEST || itr->item.itemproto->Bonding == ITEM_BIND_QUEST2 )
			continue;

		count += (itr)->iItemsCount;
	}

    return (count>0);

}

uint32 GameObject::GetGOReqSkill()  
{
	if(GetEntry() == 180215) return 300;

	if(GetInfo() == NULL)
		return 0;

	Lock *lock = dbcLock.LookupEntry( GetInfo()->SpellFocus );
	if(!lock) return 0;
	for(uint32 i=0;i<5;i++)
		if(lock->locktype[i] == 2 && lock->minlockskill[i])
		{
			return lock->minlockskill[i];
		}
	return 0;
}



