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

enum PartyUpdateFlags
{
	GROUP_UPDATE_FLAG_NONE						= 0,		// 0x00000000
	GROUP_UPDATE_FLAG_ONLINE					= 1,		// 0x00000001  uint8
	GROUP_UPDATE_FLAG_HEALTH					= 2,		// 0x00000002  uint16
	GROUP_UPDATE_FLAG_MAXHEALTH					= 4,		// 0x00000004  uint16
	GROUP_UPDATE_FLAG_POWER_TYPE				= 8,		// 0x00000008  uint16
	GROUP_UPDATE_FLAG_POWER						= 16,		// 0x00000010  uint16
	GROUP_UPDATE_FLAG_MAXPOWER					= 32,		// 0x00000020  uint16
	GROUP_UPDATE_FLAG_LEVEL						= 64,		// 0x00000040  uint16
	GROUP_UPDATE_FLAG_ZONEID					= 128,		// 0x00000080  uint16
	GROUP_UPDATE_FLAG_POSITION					= 256,		// 0x00000100  uint16, uint16
	GROUP_UPDATE_FLAG_PLAYER_AURAS				= 512,		// 0x00000200  uint64, uint16 for each uint64
	GROUP_UPDATE_FLAG_PET_GUID					= 1024,		// 0x00000400  uint64
	GROUP_UPDATE_FLAG_PET_NAME					= 2048,		// 0x00000800  string
	GROUP_UPDATE_FLAG_PET_DISPLAYID				= 4096,		// 0x00001000  uint16
	GROUP_UPDATE_FLAG_PET_HEALTH				= 8192,		// 0x00002000  uint16
	GROUP_UPDATE_FLAG_PET_MAXHEALTH				= 16384,	// 0x00004000  uint16
	GROUP_UPDATE_FLAG_PET_POWER_TYPE			= 32768,	// 0x00008000  uint8
	GROUP_UPDATE_FLAG_PET_POWER					= 65535,	// 0x00010000  uint16
	GROUP_UPDATE_FLAG_PET_MAXPOWER				= 131070,	// 0x00020000  uint16
	GROUP_UPDATE_FLAG_PET_AURAS					= 262144,	// 0x00040000  uint64, uint16 for each uint64
};

enum PartyUpdateFlagGroups
{
	GROUP_UPDATE_TYPE_FULL_CREATE				=	GROUP_UPDATE_FLAG_ONLINE | GROUP_UPDATE_FLAG_HEALTH | GROUP_UPDATE_FLAG_MAXHEALTH |
													GROUP_UPDATE_FLAG_POWER | GROUP_UPDATE_FLAG_LEVEL |
													GROUP_UPDATE_FLAG_ZONEID | GROUP_UPDATE_FLAG_MAXPOWER | GROUP_UPDATE_FLAG_POSITION,
	GROUP_UPDATE_TYPE_FULL_REQUEST_REPLY		=   0x7FFC0BFF,
};

Group::Group(bool Assign)
{
	m_GroupType = GROUP_TYPE_PARTY;	 // Always init as party

	// Create initial subgroup
    memset(m_SubGroups,0, sizeof(SubGroup*)*8);
	m_SubGroups[0] = new SubGroup(this, 0);

	memset(m_instanceIds, 0, sizeof(uint32) * NUM_MAPS * NUM_INSTANCE_MODES);

	m_Leader = NULL;
	m_Looter = NULL;
	m_LootMethod = PARTY_LOOT_GROUP;
	m_LootThreshold = 2;
	m_SubGroupCount = 1;
	m_MemberCount = 0;

	if( Assign )
	{
		m_Id = objmgr.GenerateGroupId();
		ObjectMgr::getSingleton().AddGroup(this);
	}

	m_dirty=false;
	m_updateblock=false;
	m_disbandOnNoMembers = true;
	memset(m_targetIcons, 0, sizeof(uint64) * 8);
	m_isqueued=false;
	m_difficulty=0;
	m_assistantLeader=m_mainAssist=m_mainTank=NULL;
#ifdef VOICE_CHAT
	m_voiceChannelRequested = false;
	m_voiceChannelId = 0;
	m_voiceMemberCount = 0;
	memset(m_voiceMembersList, 0, sizeof(Player*)*41);
#endif
}

Group::~Group()
{
	ObjectMgr::getSingleton().RemoveGroup(this);
}

SubGroup::~SubGroup()
{

}

void SubGroup::RemovePlayer(PlayerInfo * info)
{
	m_GroupMembers.erase(info);
	info->subGroup=-1;
}

bool SubGroup::AddPlayer(PlayerInfo * info)
{
	if(IsFull())
		return false;

	info->subGroup=(int8)GetID();
	m_GroupMembers.insert(info);
	return true;
}

bool SubGroup::HasMember(uint32 guid)
{
	for( GroupMembersSet::iterator itr = m_GroupMembers.begin(); itr != m_GroupMembers.end(); ++itr )
		if( (*itr) != NULL )
			if( (*itr)->guid == guid )
				return true;

	return false;
}

SubGroup * Group::FindFreeSubGroup()
{
	for(uint32 i = 0; i < m_SubGroupCount; i++)
		if(!m_SubGroups[i]->IsFull())
			return m_SubGroups[i];

	return NULL;
}

bool Group::AddMember(PlayerInfo * info, int32 subgroupid/* =-1 */)
{
	m_groupLock.Acquire();
	Player * pPlayer = info->m_loggedInPlayer;

	if(m_isqueued)
	{
		m_isqueued=false;
		BattlegroundManager.RemoveGroupFromQueues(this);
	}

	if(!IsFull())
	{
		SubGroup* subgroup = (subgroupid>0) ? m_SubGroups[subgroupid] : FindFreeSubGroup();
		if(subgroup == NULL)
		{
			m_groupLock.Release();
			return false;
		}

		if(subgroup->AddPlayer(info))
		{
			if(pPlayer)
				sEventMgr.AddEvent(pPlayer,&Player::EventGroupFullUpdate,EVENT_PLAYER_UPDATE,1500,1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
            
			m_dirty=true;
			++m_MemberCount;
			Update();	// Send group update
			if(info->m_Group && info->m_Group != this)
				info->m_Group->RemovePlayer(info);

			if(m_Leader==NULL && info->m_loggedInPlayer)
				m_Leader=info;

			info->m_Group=this;
			info->subGroup = (int8)subgroup->GetID();

			m_groupLock.Release();
			return true;
		}
		else
		{
			m_groupLock.Release();
			info->m_Group=NULL;
			info->subGroup=-1;
			return false;
		}

	}
	else
	{
		info->m_Group = NULL;
		info->subGroup = -1;
		m_groupLock.Release();
		return false;
	}
}

void Group::SetLeader(Player* pPlayer, bool silent)
{
	if( pPlayer != NULL )
	{
		m_Leader = pPlayer->m_playerInfo;
		m_dirty = true;
		if( !silent )
		{
			WorldPacket data( SMSG_GROUP_SET_LEADER, pPlayer->GetNameString()->size() + 1 );
			data << pPlayer->GetName();
			SendPacketToAll( &data );
		}
	}
	Update();
}

void Group::Update()
{
	if( m_updateblock )
		return;

	Player* pNewLeader = NULL;

	if( m_Leader == NULL || ( m_Leader != NULL && m_Leader->m_loggedInPlayer == NULL ) )
	{
		pNewLeader = FindFirstPlayer();
		if( pNewLeader != NULL )
			m_Leader = pNewLeader->m_playerInfo;
	}

	if( m_Looter != NULL && m_Looter->m_loggedInPlayer == NULL )
	{
		if( pNewLeader == NULL )
			pNewLeader = FindFirstPlayer();
		if( pNewLeader != NULL )
			m_Looter = pNewLeader->m_playerInfo;
	}

	WorldPacket data( 50 + ( m_MemberCount * 20 ) );
	GroupMembersSet::iterator itr1, itr2;

	uint32 i = 0, j = 0;
	uint8 flags;
	SubGroup *sg1 = NULL;
	SubGroup *sg2 = NULL;
	m_groupLock.Acquire();

	for( i = 0; i < m_SubGroupCount; i++ )
	{
		sg1 = m_SubGroups[i];

		if( sg1 != NULL)
		{
			for( itr1 = sg1->GetGroupMembersBegin(); itr1 != sg1->GetGroupMembersEnd(); ++itr1 )
			{
				// should never happen but just in case
				if( (*itr1) == NULL )
					continue;

				/* skip offline players */
				if( (*itr1)->m_loggedInPlayer == NULL )
				{
#ifdef VOICE_CHAT
					if( (*itr1)->groupVoiceId >= 0 )
					{
						// remove from voice members since that player is now offline
						RemoveVoiceMember( (*itr1) );
					}

					continue;
				}

				if( (*itr1)->groupVoiceId < 0 && sVoiceChatHandler.CanUseVoiceChat() )
				{
					(*itr1)->groupVoiceId = 0;
					AddVoiceMember( (*itr1) );
				}
#else
					continue;
				}
#endif

				data.Initialize(SMSG_GROUP_LIST);
				data << uint8(m_GroupType);	//0=party,1=raid
				data << uint8(0);   // unk
				data << uint8(sg1->GetID());
				data << uint8(0);	// unk2
				//data << uint64(0);	// unk3
				data << uint64(0x500000000004BC0CULL);
				data << uint32(m_MemberCount-1);	// we don't include self

				for( j = 0; j < m_SubGroupCount; j++ )
				{
					sg2 = m_SubGroups[j];

					if( sg2 != NULL)
					{
						for( itr2 = sg2->GetGroupMembersBegin(); itr2 != sg2->GetGroupMembersEnd(); ++itr2 )
						{
							if( (*itr1) == (*itr2) )
								continue;

							// should never happen but just in case
							if( (*itr2) == NULL )
								continue;

							data << (*itr2)->name << (*itr2)->guid << uint32(0);	// highguid
							
							if( (*itr2)->m_loggedInPlayer != NULL )
								data << uint8( 1 );
							else
								data << uint8( 0 );

							data << uint8( sg2->GetID() );
							
							flags = 0;

							if( (*itr2) == m_assistantLeader )
								flags |= 1;
							if( (*itr2) == m_mainTank )
								flags |= 2;
							if( (*itr2) == m_mainAssist )
								flags |= 4;

							data << flags;
						}
					}
				}

				if( m_Leader != NULL )
					data << m_Leader->guid << uint32( 0 );
				else
					data << uint64( 0 );

				data << uint8( m_LootMethod );

				if( m_Looter != NULL )
					data << m_Looter->guid << uint32( 0 );
				else
					data << uint64( 0 );

				data << uint8( m_LootThreshold );
				data << uint8( m_difficulty );

				if( !(*itr1)->m_loggedInPlayer->IsInWorld() )
					(*itr1)->m_loggedInPlayer->CopyAndSendDelayedPacket( &data );
				else
					(*itr1)->m_loggedInPlayer->GetSession()->SendPacket( &data );
			}		
		}
	}

	if( m_dirty )
	{
		m_dirty = false;
		SaveToDB();
	}

	m_groupLock.Release();
}

void Group::Disband()
{
	m_groupLock.Acquire();
	m_updateblock=true;

	if(m_isqueued)
	{
		m_isqueued=false;
		WorldPacket * data = sChatHandler.FillSystemMessageData("A change was made to your group. Removing the arena queue.");
		SendPacketToAll(data);
		delete data;

		BattlegroundManager.RemoveGroupFromQueues(this);
	}

	uint32 i = 0;
	for(i = 0; i < m_SubGroupCount; i++)
	{
		SubGroup *sg = m_SubGroups[i];
		sg->Disband();
	}

	m_groupLock.Release();
	CharacterDatabase.Execute("DELETE FROM groups WHERE group_id = %u", m_Id);
	sInstanceMgr.OnGroupDestruction(this);
	delete this;	// destroy ourselves, the destructor removes from eventmgr and objectmgr.
}

void SubGroup::Disband()
{
	WorldPacket data(SMSG_GROUP_DESTROYED, 1);
	WorldPacket data2(SMSG_PARTY_COMMAND_RESULT, 12);
	data2 << uint32(2) << uint8(0) << uint32(m_Parent == NULL ? 0 : m_Parent->m_difficulty);	// you leave the group

	GroupMembersSet::iterator itr = m_GroupMembers.begin();
	GroupMembersSet::iterator it2;
	for(; itr != m_GroupMembers.end();)
	{
		if( (*itr) != NULL )
		{
			if( (*itr)->m_loggedInPlayer )
			{
				if( (*itr)->m_loggedInPlayer->GetSession() != NULL )
				{
					data2.put(5, uint32((*itr)->m_loggedInPlayer->iInstanceType));
					(*itr)->m_loggedInPlayer->GetSession()->SendPacket(&data2);
					(*itr)->m_loggedInPlayer->GetSession()->SendPacket(&data);
          (*itr)->m_Group->SendNullUpdate( (*itr)->m_loggedInPlayer ); // cebernic: panel refresh.
					
				}
			}

			(*itr)->m_Group = NULL;
			(*itr)->subGroup = -1;
		}

		m_Parent->m_MemberCount--;
		it2 = itr;
		++itr;

		m_GroupMembers.erase(it2);
	}

	m_Parent->m_SubGroups[m_Id] = NULL;
	delete this;
}

Player* Group::FindFirstPlayer()
{
	GroupMembersSet::iterator itr;
	m_groupLock.Acquire();

	for( uint32 i = 0; i < m_SubGroupCount; i++ )
	{
		if( m_SubGroups[i] != NULL )
		{
			for( itr = m_SubGroups[i]->GetGroupMembersBegin(); itr != m_SubGroups[i]->GetGroupMembersEnd(); ++itr )
			{
				if( (*itr) != NULL )
				{
					if( (*itr)->m_loggedInPlayer != NULL )
					{
						m_groupLock.Release();
						return (*itr)->m_loggedInPlayer;
					}
				}
			}
		}
	}

	m_groupLock.Release();
	return NULL;
}

void Group::RemovePlayer(PlayerInfo * info)
{
	WorldPacket data(50);
	Player * pPlayer = info->m_loggedInPlayer;

	m_groupLock.Acquire();
	if(m_isqueued)
	{
		m_isqueued=false;
		BattlegroundManager.RemoveGroupFromQueues(this);
	}
	
	SubGroup *sg=NULL;
	if(info->subGroup >= 0 && info->subGroup <= 8)
		sg = m_SubGroups[info->subGroup];

	if(sg == NULL || sg->m_GroupMembers.find(info) == sg->m_GroupMembers.end())
	{
		for(uint32 i = 0; i < m_SubGroupCount; ++i)
		{
			if(m_SubGroups[i] != NULL)
			{
				if(m_SubGroups[i]->m_GroupMembers.find(info) != m_SubGroups[i]->m_GroupMembers.end())
				{
					sg = m_SubGroups[i];
					break;
				}
			}
		}
	}

	info->m_Group=NULL;
	info->subGroup=-1;
#ifdef VOICE_CHAT
	if( info->groupVoiceId <= 0 )
		RemoveVoiceMember(info);
#endif

	if(sg==NULL)
	{
		m_groupLock.Release();
		return;
	}

	m_dirty=true;
	sg->RemovePlayer(info);
	--m_MemberCount;

	if( info->m_loggedInPlayer != NULL )
		sInstanceMgr.PlayerLeftGroup( this, info->m_loggedInPlayer );

	if( pPlayer != NULL )
	{
		if( pPlayer->GetSession() != NULL )
		{
			SendNullUpdate( pPlayer );

			data.SetOpcode( SMSG_GROUP_DESTROYED );
			pPlayer->GetSession()->SendPacket( &data );

			data.Initialize( SMSG_PARTY_COMMAND_RESULT );
			data << uint32(2) << uint8(0) << uint32(0);  // you leave the group
			pPlayer->GetSession()->SendPacket( &data );
		}

		//Remove some party auras.
		for (uint32 i=MAX_POSITIVE_AURAS_EXTEDED_START;i<MAX_POSITIVE_AURAS_EXTEDED_END;i++)
		{
			if (pPlayer->m_auras[i] && 
				pPlayer->m_auras[i]->m_areaAura && 
				pPlayer->m_auras[i]->GetUnitCaster() &&
				(!pPlayer->m_auras[i]->GetUnitCaster() ||(pPlayer->m_auras[i]->GetUnitCaster()->IsPlayer() && pPlayer!=pPlayer->m_auras[i]->GetUnitCaster())))
				pPlayer->m_auras[i]->Remove();
		}
	}

	if(m_MemberCount < 2)
	{
		if(m_disbandOnNoMembers)
		{
			m_groupLock.Release();
			Disband();
			return;
		}
	}

	/* eek! ;P */
	Player *newPlayer = NULL;
	if(m_Looter == info)
	{
		newPlayer = FindFirstPlayer();
		if( newPlayer != NULL )
            m_Looter = newPlayer->m_playerInfo;
		else
			m_Looter = NULL;
	}

	if(m_Leader == info)
	{
		if( newPlayer==NULL )
			newPlayer=FindFirstPlayer();

		if( newPlayer != NULL )
			SetLeader(newPlayer, false);
		else
			m_Leader = NULL;
	}

	Update();
	m_groupLock.Release();
}

void Group::ExpandToRaid()
{
	if(m_isqueued)
	{
		m_isqueued=false;
		WorldPacket * data = sChatHandler.FillSystemMessageData("A change was made to your group. Removing the arena queue.");
		SendPacketToAll(data);
		delete data;

		BattlegroundManager.RemoveGroupFromQueues(this);
	}
	// Very simple ;)

	uint32 i = 1;
	m_groupLock.Acquire();
	m_SubGroupCount = 8;

	for(; i < m_SubGroupCount; i++)
		m_SubGroups[i] = new SubGroup(this, i);

	m_GroupType = GROUP_TYPE_RAID;
	m_dirty=true;
	Update();
	m_groupLock.Release();
}

void Group::SetLooter(Player *pPlayer, uint8 method, uint16 threshold)
{ 
	if( pPlayer != NULL )
	{
		m_LootMethod = method;
		m_Looter = pPlayer->m_playerInfo;
		m_LootThreshold  = threshold;
		m_dirty = true;
	}
	Update();
}

void Group::SendPacketToAllButOne(WorldPacket *packet, Player *pSkipTarget)
{
	GroupMembersSet::iterator itr;
	uint32 i = 0;
	m_groupLock.Acquire();
	for(; i < m_SubGroupCount; i++)
	{
		for(itr = m_SubGroups[i]->GetGroupMembersBegin(); itr != m_SubGroups[i]->GetGroupMembersEnd(); ++itr)
		{
			if((*itr)->m_loggedInPlayer != NULL && (*itr)->m_loggedInPlayer != pSkipTarget && (*itr)->m_loggedInPlayer->GetSession())
				(*itr)->m_loggedInPlayer->GetSession()->SendPacket(packet);
		}
	}
	
	m_groupLock.Release();
}

void Group::OutPacketToAllButOne(uint16 op, uint16 len, const void* data, Player *pSkipTarget)
{
	GroupMembersSet::iterator itr;
	uint32 i = 0;
	m_groupLock.Acquire();
	for(; i < m_SubGroupCount; i++)
	{
		for(itr = m_SubGroups[i]->GetGroupMembersBegin(); itr != m_SubGroups[i]->GetGroupMembersEnd(); ++itr)
		{
			if((*itr)->m_loggedInPlayer != NULL && (*itr)->m_loggedInPlayer != pSkipTarget)
				(*itr)->m_loggedInPlayer->GetSession()->OutPacket( op, len, data );
		}
	}

	m_groupLock.Release();
}

bool Group::HasMember(Player * pPlayer)
{
	if( !pPlayer )
		return false;

	GroupMembersSet::iterator itr;
	m_groupLock.Acquire();

	for( uint32 i = 0; i < m_SubGroupCount; i++ )
	{
		if( m_SubGroups[i] != NULL )
		{
			if( m_SubGroups[i]->m_GroupMembers.find( pPlayer->m_playerInfo ) != m_SubGroups[i]->m_GroupMembers.end() )
			{
				m_groupLock.Release();
				return true;
			}
		}
	}

	m_groupLock.Release();
	return false;
}

bool Group::HasMember(PlayerInfo * info)
{
	GroupMembersSet::iterator itr;
	uint32 i = 0;

	m_groupLock.Acquire();

	for(; i < m_SubGroupCount; i++)
	{
		if(m_SubGroups[i]->m_GroupMembers.find(info) != m_SubGroups[i]->m_GroupMembers.end())
		{
			m_groupLock.Release();
			return true;
		}
	}

	m_groupLock.Release();
	return false;
}

void Group::MovePlayer(PlayerInfo *info, uint8 subgroup)
{
	if( subgroup >= m_SubGroupCount )
		return;

	if(m_SubGroups[subgroup]->IsFull())
		return;

	m_groupLock.Acquire();
	SubGroup *sg=NULL;

	if(info->subGroup > 0 && info->subGroup <= 8)
		sg = m_SubGroups[info->subGroup];

	if(sg == NULL || sg->m_GroupMembers.find(info) == sg->m_GroupMembers.end())
	{
		for(uint32 i = 0; i < m_SubGroupCount; ++i)
		{
			if(m_SubGroups[i] != NULL)
			{
				if(m_SubGroups[i]->m_GroupMembers.find(info) != m_SubGroups[i]->m_GroupMembers.end())
				{
					sg = m_SubGroups[i];
					break;
				}
			}
		}
	}

	if(!sg)
	{
		m_groupLock.Release();
		return;
	}
	
	sg->RemovePlayer(info);
    
	// Grab the new group, and insert
	sg = m_SubGroups[subgroup];
	if(!sg->AddPlayer(info))
	{
		RemovePlayer(info);
		info->m_Group=NULL;
	}
	else
	{
		info->subGroup=(int8)sg->GetID();
		info->m_Group=this;
	}

	Update();
	m_groupLock.Release();
}

void Group::SendNullUpdate( Player *pPlayer )
{
	// this packet is 24 bytes long.		// AS OF 2.1.0
	uint8 buffer[24];
	memset(buffer, 0, 24);
	pPlayer->GetSession()->OutPacket( SMSG_GROUP_LIST, 24, buffer );
}

// player is object class becouse its called from unit class
void Group::SendPartyKillLog( Object * player, Object * Unit )
{
	if( !player || !Unit || !HasMember( static_cast< Player* >( player ) ) )
		return;

	WorldPacket data( SMSG_PARTYKILLLOG, 16 );
	data << player->GetGUID();
	data << Unit->GetGUID();
	SendPacketToAll( &data );
}

void Group::LoadFromDB(Field *fields)
{
#define LOAD_ASSISTANT(__i, __d) g = fields[__i].GetUInt32(); if(g != 0) { __d = objmgr.GetPlayerInfo(g); }

	uint32 g;
	m_updateblock=true;
	m_Id = fields[0].GetUInt32();

	ObjectMgr::getSingleton().AddGroup( this );

	m_GroupType = fields[1].GetUInt8();
	m_SubGroupCount = fields[2].GetUInt8();
	m_LootMethod = fields[3].GetUInt8();
	m_LootThreshold = fields[4].GetUInt8();
	m_difficulty = fields[5].GetUInt8();

	LOAD_ASSISTANT(6, m_assistantLeader);
	LOAD_ASSISTANT(7, m_mainTank);
	LOAD_ASSISTANT(8, m_mainAssist);

	// create groups
	for(int i = 1; i < m_SubGroupCount; ++i)
		m_SubGroups[i] = new SubGroup(this, i);

	// assign players into groups
	for(int i = 0; i < m_SubGroupCount; ++i)
	{
		for(int j = 0; j < 5; ++j)
		{
			uint32 guid = fields[9 + (i*5) + j].GetUInt32();
			if( guid == 0 )
				continue;

			PlayerInfo * inf = objmgr.GetPlayerInfo(guid);
			if(inf == NULL)
				continue;

			AddMember(inf);
			m_dirty=false;
		}
	}

	char *ids = strdup(fields[50].GetString());
	char *q = ids;
	char *p = strchr(q, ' ');
	while(p)
	{
		char *r = strchr(q, ':');
		if(r == NULL || r > p)
			continue;
		*p = 0;
		*r = 0;
		char *s = strchr(r+1, ':');
		if(s == NULL || s > p)
			continue;
		*s = 0;
		uint32 mapId = atoi(q);
		uint32 mode = atoi(r+1);
		uint32 instanceId = atoi(s+1);

		if(mapId >= NUM_MAPS)
			continue;

		m_instanceIds[mapId][mode] = instanceId;

		q = p+1;
		p = strchr(q, ' ');
	}
	free(ids);

	m_updateblock=false;
}

void Group::SaveToDB()
{
	if(!m_disbandOnNoMembers)	/* don't save bg groups */
		return;

	std::stringstream ss;
	//uint32 i = 0;
	uint32 fillers = 8 - m_SubGroupCount;

	ss << "REPLACE INTO groups VALUES("
		<< m_Id << ","
		<< uint32(m_GroupType) << ","
		<< uint32(m_SubGroupCount) << ","
		<< uint32(m_LootMethod) << ","
		<< uint32(m_LootThreshold) << ","
		<< uint32(m_difficulty) << ",";

	if(m_assistantLeader)
		ss << m_assistantLeader->guid << ",";
	else
		ss << "0,";
	
	if(m_mainTank)
		ss << m_mainTank->guid << ",";
	else
		ss << "0,";

	if(m_mainAssist)
		ss << m_mainAssist->guid << ",";
	else
		ss << "0,";

	for(uint32 i = 0; i < m_SubGroupCount; ++i)
	{
		uint32 j = 0;
		for(GroupMembersSet::iterator itr = m_SubGroups[i]->GetGroupMembersBegin(); j<5 && itr != m_SubGroups[i]->GetGroupMembersEnd(); ++j, ++itr)
		{
			ss << (*itr)->guid << ",";
		}

		for(; j < 5; ++j)
			ss << "0,";
	}

	for(uint32 i = 0; i < fillers; ++i)
		ss << "0,0,0,0,0,";

	ss << (uint32)UNIXTIME << ",'";
	for(int i=0; i<NUM_MAPS; i++)
	{
		for(int j=0; j<NUM_INSTANCE_MODES; j++)
		{
			if(m_instanceIds[i][j] > 0)
			{
				ss << i << ":" << j << ":" << m_instanceIds[i][j] << " ";
			}
		}
	}
	ss << "')";
	/*printf("==%s==\n", ss.str().c_str());*/
	CharacterDatabase.Execute(ss.str().c_str());
}

void Group::UpdateOutOfRangePlayer(Player * pPlayer, uint32 Flags, bool Distribute, WorldPacket * Packet)
{
	uint8 member_flags = 0x01;
	WorldPacket * data = Packet;
	if(!Packet)
		data = new WorldPacket(SMSG_PARTY_MEMBER_STATS, 500);

	if(pPlayer->GetPowerType() != POWER_TYPE_MANA)
		Flags |= GROUP_UPDATE_FLAG_POWER_TYPE;

	/*Flags |= GROUP_UPDATE_FLAG_PET_NAME;
	Flags |= GROUP_UPDATE_FLAG_PET_UNK_1;*/

	data->Initialize(SMSG_PARTY_MEMBER_STATS);
	*data << pPlayer->GetNewGUID();
	*data << Flags;

	if(Flags & GROUP_UPDATE_FLAG_ONLINE)
	{
		if(pPlayer->IsPvPFlagged())
			member_flags |= 0x02;
		if(pPlayer->getDeathState() == CORPSE)
			member_flags |= 0x08;
		else if(pPlayer->isDead())
			member_flags |= 0x10;

		*data << member_flags << uint8(0);
	}

	if(Flags & GROUP_UPDATE_FLAG_HEALTH)
		*data << uint16(pPlayer->GetUInt32Value(UNIT_FIELD_HEALTH));

	if(Flags & GROUP_UPDATE_FLAG_MAXHEALTH)
		*data << uint16(pPlayer->GetUInt32Value(UNIT_FIELD_MAXHEALTH));

	if(Flags & GROUP_UPDATE_FLAG_POWER_TYPE)
		*data << uint8(pPlayer->GetPowerType());

	if(Flags & GROUP_UPDATE_FLAG_POWER)
		*data << uint16(pPlayer->GetUInt32Value(UNIT_FIELD_POWER1 + pPlayer->GetPowerType()));

	if(Flags & GROUP_UPDATE_FLAG_MAXPOWER)
		*data << uint16(pPlayer->GetUInt32Value(UNIT_FIELD_MAXPOWER1 + pPlayer->GetPowerType()));

	if(Flags & GROUP_UPDATE_FLAG_LEVEL)
		*data << uint16(pPlayer->getLevel());

	if(Flags & GROUP_UPDATE_FLAG_ZONEID) {
		*data << uint16(pPlayer->GetAreaID());
    }

	if(Flags & GROUP_UPDATE_FLAG_POSITION)
	{
		*data << int16(pPlayer->GetPositionX()) << int16(pPlayer->GetPositionY());			// wtf packed floats? O.o
		pPlayer->m_last_group_position = pPlayer->GetPosition();
	}

	if(Flags & GROUP_UPDATE_TYPE_FULL_REQUEST_REPLY)
	{
		*data << uint64(0xFF00000000000000ULL);
		*data << uint8(0);
		*data << uint64(0xFF00000000000000ULL);
	}

	if(Distribute&&pPlayer->IsInWorld())
	{
		Player * plr;
		float dist = pPlayer->GetMapMgr()->m_UpdateDistance;
		m_groupLock.Acquire();
		for(uint32 i = 0; i < m_SubGroupCount; ++i)
		{
			if(m_SubGroups[i]==NULL)
				continue;

			for(GroupMembersSet::iterator itr = m_SubGroups[i]->GetGroupMembersBegin(); itr != m_SubGroups[i]->GetGroupMembersEnd();)
			{
				plr = (*itr)->m_loggedInPlayer;
				++itr;

				if(plr && plr != pPlayer)
				{
					if(plr->GetDistance2dSq(pPlayer) > dist)
						plr->GetSession()->SendPacket(data);
				}
			}
		}
		m_groupLock.Release();
	}

	if(!Packet)
		delete data;
}

void Group::UpdateAllOutOfRangePlayersFor(Player * pPlayer)
{
	WorldPacket data(150);
	WorldPacket data2(150);

	if(m_SubGroupCount>8)
		return;

	/* tell the other players about us */
	UpdateOutOfRangePlayer(pPlayer, GROUP_UPDATE_TYPE_FULL_CREATE, true, &data2);

	/* tell us any other players we don't know about */
	Player * plr;
	bool u1, u2;
	UpdateMask myMask;
	myMask.SetCount(PLAYER_END);
	UpdateMask hisMask;
	hisMask.SetCount(PLAYER_END);

	m_groupLock.Acquire();
	for(uint32 i = 0; i < m_SubGroupCount; ++i)
	{
		if(m_SubGroups[i]==NULL)
			continue;

		for(GroupMembersSet::iterator itr = m_SubGroups[i]->GetGroupMembersBegin(); itr != m_SubGroups[i]->GetGroupMembersEnd(); ++itr)
		{
			plr = (*itr)->m_loggedInPlayer;
			if(!plr || plr == pPlayer) continue;

			if(!plr->IsVisible(pPlayer))
			{
				UpdateOutOfRangePlayer(plr, GROUP_UPDATE_TYPE_FULL_CREATE, false, &data);
				pPlayer->GetSession()->SendPacket(&data);
			}
			else
			{
				if(pPlayer->GetSubGroup() == plr->GetSubGroup())
				{
					/* distribute quest fields to other players */
					hisMask.Clear();
					myMask.Clear();
					u1 = u2 = false;
					for(uint32 i = PLAYER_QUEST_LOG_1_1; i < PLAYER_QUEST_LOG_25_1; ++i)
					{
						if(plr->GetUInt32Value(i))
						{
							hisMask.SetBit(i);
							u1 = true;
						}

						if(pPlayer->GetUInt32Value(i))
						{
							u2 = true;
							myMask.SetBit(i);
						}
					}

					if(u1)
					{
						data.clear();
                        plr->BuildValuesUpdateBlockForPlayer(&data, &hisMask);
						pPlayer->PushUpdateData(&data, 1);
					}

					if(u2)
					{
						data.clear();
						pPlayer->BuildValuesUpdateBlockForPlayer(&data, &myMask);
						plr->PushUpdateData(&data, 1);
					}
				}
			}
		}
	}

	m_groupLock.Release();
}

void Group::HandleUpdateFieldChange(uint32 Index, Player * pPlayer)
{
	uint32 Flags = 0;
	m_groupLock.Acquire();
	switch(Index)
	{
	case UNIT_FIELD_HEALTH:
		Flags = GROUP_UPDATE_FLAG_HEALTH;
		break;
		
	case UNIT_FIELD_MAXHEALTH:
		Flags = GROUP_UPDATE_FLAG_MAXHEALTH;
		break;

	case UNIT_FIELD_POWER1:
	case UNIT_FIELD_POWER2:
	case UNIT_FIELD_POWER3:
	case UNIT_FIELD_POWER4:
		Flags = GROUP_UPDATE_FLAG_POWER;
		break;

	case UNIT_FIELD_MAXPOWER1:
	case UNIT_FIELD_MAXPOWER2:
	case UNIT_FIELD_MAXPOWER3:
	case UNIT_FIELD_MAXPOWER4:
		Flags = GROUP_UPDATE_FLAG_MAXPOWER;
		break;

	case UNIT_FIELD_LEVEL:
		Flags = GROUP_UPDATE_FLAG_LEVEL;
		break;
	default:
		break;
	}

	if( Flags != 0 )
		UpdateOutOfRangePlayer( pPlayer, Flags, true, 0 );

	m_groupLock.Release();
}

void Group::HandlePartialChange(uint32 Type, Player * pPlayer)
{
	uint32 Flags = 0;
	m_groupLock.Acquire();

	switch(Type)
	{
	case PARTY_UPDATE_FLAG_POSITION:
		Flags = GROUP_UPDATE_FLAG_POSITION;
		break;

	case PARTY_UPDATE_FLAG_ZONEID:
		Flags = GROUP_UPDATE_FLAG_ZONEID;
		break;
	}

	if(Flags)
		UpdateOutOfRangePlayer(pPlayer, Flags, true, 0);

	m_groupLock.Release();
}

void WorldSession::HandlePartyMemberStatsOpcode(WorldPacket & recv_data)
{
	if(!_player->IsInWorld())
		return;

	uint64 guid;
	recv_data >> guid;

	Player * plr = _player->GetMapMgr()->GetPlayer((uint32)guid);

	if(!_player->GetGroup() || !plr)
		return;

	WorldPacket data(200);
	if(!_player->GetGroup()->HasMember(plr))
		return;			// invalid player

	if(_player->IsVisible(plr))
		return;

	_player->GetGroup()->UpdateOutOfRangePlayer(plr, GROUP_UPDATE_TYPE_FULL_CREATE | GROUP_UPDATE_TYPE_FULL_REQUEST_REPLY, false, &data);
	data.SetOpcode(SMSG_PARTY_MEMBER_STATS_FULL);
	SendPacket(&data);
}

Group* Group::Create()
{
	return new Group(true);
}

void Group::SetMainAssist(PlayerInfo * pMember)
{
	if(m_mainAssist==pMember)
		return;

	m_mainAssist = pMember;
	m_dirty = true;
	Update();
}

void Group::SetMainTank(PlayerInfo * pMember)
{
	if(m_mainTank==pMember)
		return;

	m_mainTank=pMember;
	m_dirty = true;
	Update();
}

void Group::SetAssistantLeader(PlayerInfo * pMember)
{
	if(m_assistantLeader == pMember)
		return;

	m_assistantLeader = pMember;
	m_dirty = true;
	Update();
}


/************************************************************************/
/* Voicechat                                                            */
/************************************************************************/
#ifdef VOICE_CHAT

void Group::CreateVoiceSession()
{
	sVoiceChatHandler.CreateGroupChannel( this );
}

void Group::VoiceChannelCreated(uint16 id)
{
	Log.Debug("Group", "voicechannelcreated: id %u", (uint32)id);
	m_voiceChannelId = id;
	if( id != 0 )
	{
		// so we just got a channel. we better activate the slots that are in use so people can talk
		uint32 i;
		for( i = 0; i <= 40; ++i )
		{
			if( m_voiceMembersList[i] != NULL )
				sVoiceChatHandler.ActivateChannelSlot(id, (uint8)i );
		}

		SendVoiceUpdate();
	}
}

void Group::AddVoiceMember(PlayerInfo * pPlayer)
{
	m_groupLock.Acquire();
	Log.Debug("Group", "adding voice member %u to group %u", pPlayer->guid, GetID());
	uint32 i;

	// find him an id
	for( i = 1; i <= 40; ++i )
	{
		if( m_voiceMembersList[i] == NULL )
			break;
	}

	if( i == 41 )
	{
		// no free slots
		Log.Error("Group", "could not add voice member, no slots!");
		return;
	}

	pPlayer->groupVoiceId = i;
	m_voiceMembersList[i] = pPlayer;
	++m_voiceMemberCount;

	if( !m_voiceChannelRequested )
	{
		/*uint32 count = 0;
		for( i = 0; i < 41; ++i )
		{
			if( m_voiceMembersList[i] != NULL )
			{
				++count;
				if( count >= 2 )
					break;
			}
		}*/

		if( m_voiceMemberCount >= 2 )		// Don't create channels with only one member
		{
			CreateVoiceSession();
			m_voiceChannelRequested = true;
		}
	}
	else
	{
		if( m_voiceChannelId != 0 )
		{
			// activate his slot
			sVoiceChatHandler.ActivateChannelSlot(m_voiceChannelId, (uint8)i);
			SendVoiceUpdate();
		}
	}

	m_groupLock.Release();
}

void Group::RemoveVoiceMember(PlayerInfo * pPlayer)
{
	if( pPlayer->groupVoiceId <= 0 )
		return;

	Log.Debug("Group", "removing voice member %u from group %u", pPlayer->guid, GetID());

	m_groupLock.Acquire();
	if( m_voiceMembersList[pPlayer->groupVoiceId] == pPlayer )
	{
		--m_voiceMemberCount;
		m_voiceMembersList[pPlayer->groupVoiceId] = NULL;

		if( m_voiceChannelId != 0 )
		{
			// turn off the slot
			sVoiceChatHandler.DeactivateChannelSlot(m_voiceChannelId, pPlayer->groupVoiceId);
			SendVoiceUpdate();
		}

		if( m_voiceMemberCount < 2 )
		{
			// destroy the channel
			sVoiceChatHandler.DestroyGroupChannel(this);
			m_voiceChannelId = 0;
			m_voiceChannelRequested = false;
		}
	}

	pPlayer->groupVoiceId = -1;
	m_groupLock.Release();
}

void Group::SendVoiceUpdate()
{
	m_groupLock.Acquire();
	Log.Debug("Group", "sendgroupupdate id %u", (uint32)m_voiceChannelId);

	//static uint8 EncryptionKey[16] = { 0x14, 0x60, 0xcf, 0xaf, 0x9e, 0xa2, 0x78, 0x38, 0xce, 0xc7, 0xaf, 0x0b, 0x3a, 0x23, 0x61, 0x44 };
	static uint8 EncryptionKey[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

	uint8 counter = 1;
	size_t pos;
	uint32 i,j;
	Player * pl;

	WorldPacket data(SMSG_VOICE_SESSION, 100);
	data << uint32( 0x00000E9D );
	data << uint32( 0xE2500000 );		// this appears to be constant :S

	data << uint16( m_voiceChannelId );		// voice channel id, used in udp packet
	data << uint8( 2 );						// party voice channel
	data << uint8( 0 );						// for channels this is name
	data.append( EncryptionKey, 16 );		// encryption key

	// IP
	// these dont appear to be in network byte order.. gg
	data << uint32(htonl(sVoiceChatHandler.GetVoiceServerIP()));
	data << uint16(sVoiceChatHandler.GetVoiceServerPort());
	
	data << uint8( m_voiceMemberCount );
	pos = data.wpos();

	for( i = 0; i <= 40; ++i )
	{
		if( m_voiceMembersList[i] == NULL )
			continue;

		if( m_voiceMembersList[i]->m_loggedInPlayer == NULL )
		{
			// shouldnt happen
			RemoveVoiceMember(m_voiceMembersList[i]);
			continue;
		}

		pl = m_voiceMembersList[i]->m_loggedInPlayer;

		// Append ourself first, always.
		data << uint64 ( pl->GetGUID() );
		data << uint8( i );

		if( pl->m_playerInfo == m_Leader )
		{
			data << uint8(0x06);
		}
		else
		{
			data << uint8(0x46);
		}

		for( j = 0; j <= 40; ++j )
		{
			if( i == j || m_voiceMembersList[j] == NULL )
				continue;

			if( m_voiceMembersList[j]->m_loggedInPlayer == NULL )
			{
				// shouldnt happen
				RemoveVoiceMember(m_voiceMembersList[j]);
				continue;
			}

			data << uint64( m_voiceMembersList[j]->guid );
			data << uint8( j );

			if( m_voiceMembersList[j] == m_Leader )
			{
				data << uint8(0x80) << uint8(0x47);
			}
			else
			{
				data << uint8(0xC8) << uint8(0x47);
			}
		}

		//data << uint8( 0x47 );

		pl->GetSession()->SendPacket( &data );
		data.wpos( pos );
	}

	m_groupLock.Release();
}

void Group::VoiceSessionDropped()
{
	Log.Debug("Group", "Voice session dropped");
	m_groupLock.Acquire();
	for(uint32 i = 0; i < 41; ++i)
	{
		if( m_voiceMembersList[i] != NULL )
		{
			m_voiceMembersList[i]->groupVoiceId = -1;
			m_voiceMembersList[i] = NULL;
		}
	}
	m_voiceChannelRequested = false;
	m_voiceChannelId = 0;
	m_voiceMemberCount = 0;
	m_groupLock.Release();
}

void Group::VoiceSessionReconnected()
{
	if( !sVoiceChatHandler.CanUseVoiceChat() )
		return;

	// try to recreate a group if one is needed
	GroupMembersSet::iterator itr1, itr2;
	Log.Debug("Group", "Attempting to recreate voice session for group %u", GetID());

	uint32 i = 0, j = 0;
	SubGroup *sg1 = NULL;
	SubGroup *sg2 = NULL;
	m_groupLock.Acquire();

	for( i = 0; i < m_SubGroupCount; i++ )
	{
		sg1 = m_SubGroups[i];

		if( sg1 != NULL)
		{
			for( itr1 = sg1->GetGroupMembersBegin(); itr1 != sg1->GetGroupMembersEnd(); ++itr1 )
			{
				// should never happen but just in case
				if( (*itr1) == NULL )
					continue;

				/* skip offline players */
				if( (*itr1)->m_loggedInPlayer == NULL )
				{
					if( (*itr1)->groupVoiceId >= 0 )
					{
						// remove from voice members since that player is now offline
						RemoveVoiceMember( (*itr1) );
					}

					continue;
				}

				if( (*itr1)->groupVoiceId < 0 )
				{
					(*itr1)->groupVoiceId = 0;
					AddVoiceMember( (*itr1) );
				}
			}
		}
	}

	m_groupLock.Release();
}
#endif
