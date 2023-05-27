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

/////////////////////////////////////////////////
//  Debug Chat Commands
//

#include "StdAfx.h"
#include "Messenger.h"
#include "PlayerMessenger.h"

bool ChatHandler::HandleDebugInFrontCommand(const char* args, WorldSession* m_session)
{
	Object* obj;

	uint64 guid = m_session->GetPlayer()->GetSelection();
	if(guid != 0)
	{
		if((obj = m_session->GetPlayer()->GetMapMgr()->GetUnit(guid)) == 0)
		{
			SystemMessage(m_session, "You should select a character or a creature.");
			return true;
		}
	}
	else
		obj = m_session->GetPlayer();

	char buf[256];
	snprintf((char*)buf, 256, "%d", m_session->GetPlayer()->isInFront((Unit*)obj));

	SystemMessage(m_session, buf);

	return true;
}

bool ChatHandler::HandleShowReactionCommand(const char* args, WorldSession* m_session)
{
	Object* obj = NULL;

	uint64 guid = m_session->GetPlayer()->GetSelection();
	if(guid != 0)
	{
		obj = m_session->GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	}

	if(!obj)
	{
		SystemMessage(m_session, "You should select a creature.");
		return true;
	}


	char* pReaction = strtok((char*)args, " ");
	if(!pReaction)
		return false;

	uint32 Reaction  = atoi(pReaction);

	Messenger::SendAIReaction(obj,Reaction);

	std::stringstream sstext;
	sstext << "Sent Reaction of " << Reaction << " to " << obj->GetUIdFromGUID() << '\0';

	SystemMessage(m_session,  sstext.str().c_str());
	return true;
}

bool ChatHandler::HandleDistanceCommand(const char* args, WorldSession* m_session)
{
	Object* obj;

	uint64 guid = m_session->GetPlayer()->GetSelection();
	if(guid != 0)
	{
		if((obj = m_session->GetPlayer()->GetMapMgr()->GetUnit(guid)) == 0)
		{
			SystemMessage(m_session, "You should select a character or a creature.");
			return true;
		}
	}
	else
		obj = m_session->GetPlayer();

	float dist = m_session->GetPlayer()->CalcDistance(obj);
	std::stringstream sstext;
	sstext << "Distance is: " << dist << '\0';

	SystemMessage(m_session, sstext.str().c_str());
	return true;
}

bool ChatHandler::HandleMoveInfoCommand(const char* args, WorldSession* m_session)
{
	Object* obj;

	uint64 guid = m_session->GetPlayer()->GetSelection();
	if((obj = m_session->GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid))) == 0)
	{
		SystemMessage(m_session, "You should select a character or a creature.");
		return true;
	}

	float dist = m_session->GetPlayer()->CalcDistance(obj);
	bool minfront = obj->isInFront(m_session->GetPlayer());
	bool pinfront = m_session->GetPlayer()->isInFront(obj);
	uint32 movetype = TO< Creature* >(obj)->GetAIInterface()->getMoveType();
	bool run = TO< Creature* >(obj)->GetAIInterface()->HasWalkMode(WALKMODE_RUN) || TO< Creature* >(obj)->GetAIInterface()->HasWalkMode(WALKMODE_SPRINT);
	uint32 attackerscount = (uint32)TO< Creature* >(obj)->GetAIInterface()->getAITargetsCount();
	uint32 creatureState = TO< Creature* >(obj)->GetAIInterface()->m_creatureState;
	uint32 curwp = TO< Creature* >(obj)->GetAIInterface()->getCurrentWaypoint();
//	Unit* unitToFollow = ((Creature *)obj)->GetAIInterface()->getUnitToFollow();
	uint32 aistate = TO< Creature* >(obj)->GetAIInterface()->getAIState();
	uint32 aitype = TO< Creature* >(obj)->GetAIInterface()->getAIType();
	uint32 aiagent = TO< Creature* >(obj)->GetAIInterface()->getCurrentAgent();
	uint32 lowfollow = 0;
	uint32 highfollow = 0;
	/*if(unitToFollow == NULL)
	{
		lowfollow = 0;
		highfollow = 0;
	}
	else
	{
		lowfollow = unitToFollow->GetGUIDLow();
		highfollow = unitToFollow->GetGUIDHigh();;
	}*/

	std::stringstream sstext;
	sstext << "Following Unit: Low: " << lowfollow << " High: " << highfollow << "\n";
	sstext << "Distance is: " << dist << "\n";
	sstext << "Mob Facing Player: " << minfront << " Player Facing Mob: " << pinfront << "\n";
	sstext << "Attackers Count: " << attackerscount << "\n";
	sstext << "Creature State: " << creatureState << " Run: " << run << "\n";
	sstext << "AIState: " << aistate << " AIType: " << aitype << " AIAgent: " << aiagent << "\n";
	sstext << "MoveType: " << movetype << " Current Waypoint: " << curwp << "\n";

	SendMultilineMessage(m_session, sstext.str().c_str());
	//FillSystemMessageData(&data, sstext.str().c_str());
	//m_session->SendPacket( &data );

	return true;
}

bool ChatHandler::HandleAIMoveCommand(const char* args, WorldSession* m_session)
{
	Object* obj = NULL;

	uint64 guid = m_session->GetPlayer()->GetSelection();
	if(guid != 0)
	{
		obj = m_session->GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	}

	if(obj == NULL)
	{
		SystemMessage(m_session, "You should select a creature.");
		return true;
	}

	//m_session->GetPlayer()->GetOrientation();

	uint32 Move  = 1;
	uint32 Run  = 0;
	uint32 Time = 0;
	uint32 Meth = 0;

	char* pMove = strtok((char*)args, " ");
	if(pMove)
		Move  = atoi(pMove);

	char* pRun = strtok(NULL, " ");
	if(pRun)
		Run  = atoi(pRun);

	char* pTime = strtok(NULL, " ");
	if(pTime)
		Time  = atoi(pTime);

	char* pMeth = strtok(NULL, " ");
	if(pMeth)
		Meth  = atoi(pMeth);

	float x = m_session->GetPlayer()->GetPositionX();
	float y = m_session->GetPlayer()->GetPositionY();
	float z = m_session->GetPlayer()->GetPositionZ();
	float o = m_session->GetPlayer()->GetOrientation();

	if(Run)
		TO< Creature* >(obj)->GetAIInterface()->SetRun();
	else
		TO< Creature* >(obj)->GetAIInterface()->SetWalk();
	float distance = TO< Creature* >(obj)->CalcDistance(x, y, z);
	if(Move == 1)
	{
		if(Meth == 1)
		{
			float q = distance - 0.5f;
			x = (TO< Creature* >(obj)->GetPositionX() + x * q) / (1 + q);
			y = (TO< Creature* >(obj)->GetPositionY() + y * q) / (1 + q);
			z = (TO< Creature* >(obj)->GetPositionZ() + z * q) / (1 + q);
		}
		else if(Meth == 2)
		{
			float q = distance - 1;
			x = (TO< Creature* >(obj)->GetPositionX() + x * q) / (1 + q);
			y = (TO< Creature* >(obj)->GetPositionY() + y * q) / (1 + q);
			z = (TO< Creature* >(obj)->GetPositionZ() + z * q) / (1 + q);
		}
		else if(Meth == 3)
		{
			float q = distance - 2;
			x = (TO< Creature* >(obj)->GetPositionX() + x * q) / (1 + q);
			y = (TO< Creature* >(obj)->GetPositionY() + y * q) / (1 + q);
			z = (TO< Creature* >(obj)->GetPositionZ() + z * q) / (1 + q);
		}
		else if(Meth == 4)
		{
			float q = distance - 2.5f;
			x = (TO< Creature* >(obj)->GetPositionX() + x * q) / (1 + q);
			y = (TO< Creature* >(obj)->GetPositionY() + y * q) / (1 + q);
			z = (TO< Creature* >(obj)->GetPositionZ() + z * q) / (1 + q);
		}
		else if(Meth == 5)
		{
			float q = distance - 3;
			x = (TO< Creature* >(obj)->GetPositionX() + x * q) / (1 + q);
			y = (TO< Creature* >(obj)->GetPositionY() + y * q) / (1 + q);
			z = (TO< Creature* >(obj)->GetPositionZ() + z * q) / (1 + q);
		}
		else if(Meth == 6)
		{
			float q = distance - 3.5f;
			x = (TO< Creature* >(obj)->GetPositionX() + x * q) / (1 + q);
			y = (TO< Creature* >(obj)->GetPositionY() + y * q) / (1 + q);
			z = (TO< Creature* >(obj)->GetPositionZ() + z * q) / (1 + q);
		}
		else
		{
			float q = distance - 4;
			x = (TO< Creature* >(obj)->GetPositionX() + x * q) / (1 + q);
			y = (TO< Creature* >(obj)->GetPositionY() + y * q) / (1 + q);
			z = (TO< Creature* >(obj)->GetPositionZ() + z * q) / (1 + q);
		}
		TO< Creature* >(obj)->GetAIInterface()->MoveTo(x, y, z, 0);
	}
	else
		TO_CREATURE(obj)->GetAIInterface()->MoveTo(x, y, z, o);
	return true;
}

bool ChatHandler::HandleFaceCommand(const char* args, WorldSession* m_session)
{
	Player *player = m_session->GetPlayer();
	Creature* creature = NULL;

	uint64 guid = player->GetSelection();
	if(guid != 0)
	{
		creature = player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	}

	if(creature == NULL)
	{
		SystemMessage(m_session,  "You should select a creature.");
		return true;
	}

	float o = 0;
	char* pOrentation = strtok((char*)args, " ");
	if( pOrentation != NULL )
	{
		o  = atof(pOrentation);
	}
	else
	{
		o = Math::calcRadAngle( creature->GetPositionX(), creature->GetPositionY(), player->GetPositionX(), player->GetPositionY() );
	}

	creature->SetFacing( o );

	return true;
}
/*

bool ChatHandler::HandleAIMoveCommand(const char* args)
{
WorldPacket data;
Object *obj = NULL;

uint64 guid = m_session->GetPlayer()->GetSelection();
if (guid != 0)
{
obj = (Object*)objmgr.GetCreature(guid);
}

if(obj == NULL)
{
FillSystemMessageData(&data, "You should select a creature.");
m_session->SendPacket( &data );
return true;
}

uint8 Value1  = 0;
bool Run  = 0;
uint32 Value2 = 1;
uint32 Value3 = 0;
bool ToFrom = 0;

char* pValue1 = strtok((char*)args, " ");
if (pValue1)
Value1  = static_cast<uint8>(atol(pValue1));

char* pRun = strtok(NULL, " ");
if (pRun)
Run  = atoi(pRun);

char* pValue2 = strtok(NULL, " ");
if (pValue2)
Value2  = atoi(pValue2);

char* pValue3 = strtok(NULL, " ");
if (pValue3)
Value3  = atoi(pValue3);

char* pToFrom = strtok(NULL, " ");
if (pToFrom)
ToFrom  = atoi(pToFrom);

float fromX = ((Creature *)obj)->GetPositionX();
float fromY = ((Creature *)obj)->GetPositionY();
float fromZ = ((Creature *)obj)->GetPositionZ();
float fromO = ((Creature *)obj)->GetOrientation();
float toX = m_session->GetPlayer()->GetPositionX();
float toY = m_session->GetPlayer()->GetPositionY();
float toZ = m_session->GetPlayer()->GetPositionZ();
float toO = m_session->GetPlayer()->GetOrientation();

float distance = ((Creature *)obj)->CalcDistance((Object *)m_session->GetPlayer());
uint32 moveSpeed = 0;
if(!Run)
{
moveSpeed = 2.5f*0.001f;
}
else
{
moveSpeed = 7.0f*0.001f;
}
uint32 moveTime = (uint32) (distance / moveSpeed);

data.Initialize( SMSG_MONSTER_MOVE );
data << guid;
if(ToFrom)
{
data << toX << toY << toZ << toO;
}
else
{
data << fromX << fromY << fromZ << fromO;
}
data << uint8(Value1);
if(Value1 != 1)
{
data << uint32(Run ? 0x00000100 : 0x00000000);
data << moveTime;
data << Value2;
if(ToFrom)
{
data << fromX << fromY << fromZ;
if(Value2 > 1)
{
data << fromO;
}
}
else
{
data << toX << toY << toZ;
if(Value2 > 1)
{
data << toO;
}

}
if(Value2 > 2)
{
data << Value3;
}
}
//((Creature *)obj)->m_m_timeToMove = moveTime;
//m_moveTimer =  UNIT_MOVEMENT_INTERPOLATE_INTERVAL; // update every few msecs

//	m_creatureState = MOVING;
((Creature *)obj)->SendMessageToSet( &data, false );
return true;
}
*/
bool ChatHandler::HandleSetBytesCommand(const char* args, WorldSession* m_session)
{
	Object* obj;

	uint64 guid = m_session->GetPlayer()->GetSelection();
	if(guid != 0)
	{
		if((obj = m_session->GetPlayer()->GetMapMgr()->GetUnit(guid)) == 0)
		{
			SystemMessage(m_session, "You should select a character or a creature.");
			return true;
		}
	}
	else
		obj = m_session->GetPlayer();

	char* pBytesIndex = strtok((char*)args, " ");
	if(!pBytesIndex)
		return false;

	uint32 BytesIndex  = atoi(pBytesIndex);

	char* pValue1 = strtok(NULL, " ");
	if(!pValue1)
		return false;

	uint8 Value1  = static_cast<uint8>(atol(pValue1));

	char* pValue2 = strtok(NULL, " ");
	if(!pValue2)
		return false;

	uint8 Value2  = static_cast<uint8>(atol(pValue2));

	char* pValue3 = strtok(NULL, " ");
	if(!pValue3)
		return false;

	uint8 Value3  = static_cast<uint8>(atol(pValue3));

	char* pValue4 = strtok(NULL, " ");
	if(!pValue4)
		return false;

	uint8 Value4  = static_cast<uint8>(atol(pValue4));

	std::stringstream sstext;
	sstext << "Set Field " << BytesIndex << " bytes to " << uint16((uint8)Value1) << " " << uint16((uint8)Value2) << " " << uint16((uint8)Value3) << " " << uint16((uint8)Value4) << '\0';
	obj->SetUInt32Value(BytesIndex, ((Value1) | (Value2 << 8) | (Value3 << 16) | (Value4 << 24)));
	SystemMessage(m_session, sstext.str().c_str());
	return true;
}

bool ChatHandler::HandleGetBytesCommand(const char* args, WorldSession* m_session)
{
	Object* obj;

	uint64 guid = m_session->GetPlayer()->GetSelection();
	if(guid != 0)
	{
		if((obj = m_session->GetPlayer()->GetMapMgr()->GetUnit(guid)) == 0)
		{
			SystemMessage(m_session, "You should select a character or a creature.");
			return true;
		}
	}
	else
		obj = m_session->GetPlayer();

	char* pBytesIndex = strtok((char*)args, " ");
	if(!pBytesIndex)
		return false;

	uint32 BytesIndex  = atoi(pBytesIndex);
	uint32 theBytes = obj->GetUInt32Value(BytesIndex);
	/*
	switch (Bytes)
	{
	case 0:
	theBytes = obj->GetUInt32Value(UNIT_FIELD_BYTES_0);
	break;
	case 1:
	theBytes = obj->GetUInt32Value(UNIT_FIELD_BYTES_1);
	break;
	case 2:
	theBytes = obj->GetUInt32Value(UNIT_FIELD_BYTES_2);
	break;
	}
	*/
	std::stringstream sstext;
	sstext << "bytes for Field " << BytesIndex << " are " << uint16((uint8)theBytes & 0xFF) << " " << uint16((uint8)(theBytes >> 8) & 0xFF) << " ";
	sstext << uint16((uint8)(theBytes >> 16) & 0xFF) << " " << uint16((uint8)(theBytes >> 24) & 0xFF) << '\0';

	SystemMessage(m_session, sstext.str().c_str());
	return true;
}
bool ChatHandler::HandleDebugLandWalk(const char* args, WorldSession* m_session)
{
	Player* chr = getSelectedChar(m_session);
	char buf[256];

	if(chr == NULL)  // Ignatich: what should NOT happen but just in case...
	{
		SystemMessage(m_session, "No character selected.");
		return false;
	}
	chr->SetMovement(MOVE_LAND_WALK, 8);
	snprintf((char*)buf, 256, "Land Walk Test Ran.");
	SystemMessage(m_session, buf);
	return true;
}

bool ChatHandler::HandleDebugWaterWalk(const char* args, WorldSession* m_session)
{
	Player* chr = getSelectedChar(m_session);
	char buf[256];

	if(chr == NULL)  // Ignatich: what should NOT happen but just in case...
	{
		SystemMessage(m_session, "No character selected.");
		return false;
	}
	chr->SetMovement(MOVE_WATER_WALK, 4);
	snprintf((char*)buf, 256, "Water Walk Test Ran.");
	SystemMessage(m_session,  buf);
	return true;
}

bool ChatHandler::HandleDebugUnroot(const char* args, WorldSession* m_session)
{
	if( m_session->GetPlayer()->GetTargetGUID() == 0 ){
		RedSystemMessage( m_session, "You need to select a unit!" );
		return true;
	}

	Unit *u = m_session->GetPlayer()->GetMapMgr()->GetUnit( m_session->GetPlayer()->GetTargetGUID() );
	if( u == NULL ){
		RedSystemMessage( m_session, "You need to select a unit!" );
		return true;
	}

	u->Unroot();

	SystemMessage( m_session, "Unit unrooted!" );

	return true;
}

bool ChatHandler::HandleDebugRoot(const char* args, WorldSession* m_session)
{
	if( m_session->GetPlayer()->GetTargetGUID() == 0 ){
		RedSystemMessage( m_session, "You need to select a unit!" );
		return true;
	}

	Unit *u = m_session->GetPlayer()->GetMapMgr()->GetUnit( m_session->GetPlayer()->GetTargetGUID() );
	if( u == NULL ){
		RedSystemMessage( m_session, "You need to select a unit!" );
		return true;
	}

	u->Root();

	SystemMessage( m_session, "Unit rooted!" );

	return true;
}

bool ChatHandler::HandleAggroRangeCommand(const char* args, WorldSession* m_session)
{
	Unit* obj = NULL;

	uint64 guid = m_session->GetPlayer()->GetSelection();
	if(guid != 0)
	{
		if((obj = m_session->GetPlayer()->GetMapMgr()->GetUnit(guid)) == 0)
		{
			SystemMessage(m_session,  "You should select a character or a creature.");
			return true;
		}
	}
	else
	{
		SystemMessage(m_session, "You should select a character or a creature.");
		return true;
	}

	float aggroRange = obj->GetAIInterface()->_CalcAggroRange(m_session->GetPlayer());
	std::stringstream sstext;
	sstext << "Aggrorange is: " << sqrtf(aggroRange) << '\0';

	SystemMessage(m_session, sstext.str().c_str());

	return true;
}

bool ChatHandler::HandleLeapCommand(const char* args, WorldSession* m_session)
{
	Player *player = m_session->GetPlayer();

	float horizontal;
	float vertical;
	uint32 direction = 0;

	int read = sscanf( args, "%f %f %u", &horizontal, &vertical, &direction );
	if( read < 2 || read > 3 )
	{
		return false;
	}

	bool flip = false;
	if( direction == -1 )
		flip = true;

	Messenger::SendMoveKnockback( player, horizontal, vertical, flip );

	return true;
}

bool ChatHandler::HandleFadeCommand(const char* args, WorldSession* m_session)
{
	Unit* target = m_session->GetPlayer()->GetMapMgr()->GetUnit(m_session->GetPlayer()->GetSelection());
	if(!target)
		target = m_session->GetPlayer();
	char* v = strtok((char*)args, " ");
	if(!v)
		return false;

	target->ModThreatModifyer(atoi(v));

	std::stringstream sstext;
	sstext << "threat is now reduced by: " << target->GetThreatModifyer() << '\0';

	SystemMessage(m_session, sstext.str().c_str());
	return true;
}

bool ChatHandler::HandleThreatModCommand(const char* args, WorldSession* m_session)
{
	Unit* target = m_session->GetPlayer()->GetMapMgr()->GetUnit(m_session->GetPlayer()->GetSelection());
	if(!target)
		target = m_session->GetPlayer();
	char* v = strtok((char*)args, " ");
	if(!v)
		return false;

	target->ModGeneratedThreatModifyer(0, atoi(v));

	std::stringstream sstext;
	sstext << "new threat caused is now reduced by: " << target->GetGeneratedThreatModifyer(0) << "%" << '\0';

	SystemMessage(m_session, sstext.str().c_str());
	return true;
}

bool ChatHandler::HandleCalcThreatCommand(const char* args, WorldSession* m_session)
{
	Unit* target = m_session->GetPlayer()->GetMapMgr()->GetUnit(m_session->GetPlayer()->GetSelection());
	if(!target)
	{
		SystemMessage(m_session, "You should select a creature.");
		return true;
	}
	char* dmg = strtok((char*)args, " ");
	if(!dmg)
		return false;
	char* spellId = strtok(NULL, " ");
	if(!spellId)
		return false;

	uint32 threat = target->GetAIInterface()->_CalcThreat(atol(dmg), dbcSpell.LookupEntry(atoi(spellId)), m_session->GetPlayer());

	std::stringstream sstext;
	sstext << "generated threat is: " << threat << '\0';

	SystemMessage(m_session, sstext.str().c_str());
	return true;
}

bool ChatHandler::HandleThreatListCommand(const char* args, WorldSession* m_session)
{
	Unit* target = NULL;
	target = m_session->GetPlayer()->GetMapMgr()->GetUnit(m_session->GetPlayer()->GetSelection());
	if(!target)
	{
		SystemMessage(m_session, "You should select a creature.");
		return true;
	}

	std::stringstream sstext;
	sstext << "threatlist of creature: " << Arcemu::Util::GUID_LOPART(m_session->GetPlayer()->GetSelection()) << " " << Arcemu::Util::GUID_HIPART(m_session->GetPlayer()->GetSelection()) << '\n';
	TargetMap::iterator itr;
	for(itr = target->GetAIInterface()->GetAITargets()->begin(); itr != target->GetAIInterface()->GetAITargets()->end();)
	{
		Unit* ai_t = target->GetMapMgr()->GetUnit(itr->first);
		if(!ai_t || !itr->second)
		{
			++itr;
			continue;
		}
		sstext << "guid: " << itr->first << " | threat: " << itr->second << "| threat after mod: " << (itr->second + ai_t->GetThreatModifyer()) << "\n";
		++itr;
	}

	SendMultilineMessage(m_session, sstext.str().c_str());
	return true;
}
bool ChatHandler::HandleSendItemPushResult(const char* args, WorldSession* m_session)
{
	uint32 uint_args[7];
	char* arg = const_cast<char*>(args);
	char* token = strtok(arg, " ");

	uint8 i = 0;
	while(token != NULL && i < 7)
	{
		uint_args[i] = atol(token);
		token = strtok(NULL, " ");
		i++;
	}
	for(; i < 7; i++)
		uint_args[i] = 0;

	if(uint_args[0] == 0)   // null itemid
		return false;

	// lookup item
//	ItemPrototype *proto = ItemPrototypeStorage.LookupEntry(itemid);

	WorldPacket data;
	data.SetOpcode(SMSG_ITEM_PUSH_RESULT);
	data << m_session->GetPlayer()->GetGUID();	// recivee_guid
	data << uint_args[2];	// type
	data << uint32(1);		// unk
	data << uint_args[1];	// count
	data << uint8(0xFF);	// uint8 unk const 0xFF
	data << uint_args[3];	// unk1
	data << uint_args[0];	// item id
	data << uint_args[4];	// unk2
	data << uint_args[5];	// random prop
	data << uint_args[6];	// unk3
	m_session->SendPacket(&data);
	return true;
	//data << ((proto != NULL) ? proto->Quality : uint32(0)); // quality
}

bool ChatHandler::HandleModifyBitCommand(const char* args, WorldSession* m_session)
{

	Object* obj;

	uint64 guid = m_session->GetPlayer()->GetSelection();
	if(guid != 0)
	{
		if((obj = m_session->GetPlayer()->GetMapMgr()->GetUnit(guid)) == 0)
		{
			SystemMessage(m_session, "You should select a character or a creature.");
			return true;
		}
	}
	else
		obj = m_session->GetPlayer();

	char* pField = strtok((char*)args, " ");
	if(!pField)
		return false;

	char* pBit = strtok(NULL, " ");
	if(!pBit)
		return false;

	uint16 field = static_cast<uint16>(atoi(pField));
	uint32 bit   = atoi(pBit);

	if(field < 1 || field >= PLAYER_END)
	{
		SystemMessage(m_session, "Incorrect values.");
		return true;
	}

	if(bit < 1 || bit > 32)
	{
		SystemMessage(m_session, "Incorrect values.");
		return true;
	}

	char buf[256];

	if(obj->HasFlag(field, (1 << (bit - 1))))
	{
		obj->RemoveFlag(field, (1 << (bit - 1)));
		snprintf((char*)buf, 256, "Removed bit %i in field %i.", (unsigned int)bit, (unsigned int)field);
	}
	else
	{
		obj->SetFlag(field, (1 << (bit - 1)));
		snprintf((char*)buf, 256, "Set bit %i in field %i.", (unsigned int)bit, (unsigned int)field);
	}

	SystemMessage(m_session, buf);
	return true;
}

bool ChatHandler::HandleModifyValueCommand(const char* args,  WorldSession* m_session)
{
	Object* obj;

	uint64 guid = m_session->GetPlayer()->GetSelection();
	if(guid != 0)
	{
		if((obj = m_session->GetPlayer()->GetMapMgr()->GetUnit(guid)) == 0)
		{
			SystemMessage(m_session, "You should select a character or a creature.");
			return true;
		}
	}
	else
		obj = m_session->GetPlayer();

	char* pField = strtok((char*)args, " ");
	if(!pField)
		return false;

	char* pValue = strtok(NULL, " ");
	if(!pValue)
		return false;

	uint16 field = static_cast<uint16>(atoi(pField));
	uint32 value   = atoi(pValue);

	if(field < 1 || field >= PLAYER_END)
	{
		SystemMessage(m_session, "Incorrect Field.");
		return true;
	}
	/*
		if (value > sizeof(uint32))
		{
			FillSystemMessageData(&data, m_session, "Incorrect Value.");
			m_session->SendPacket( &data );
			return true;
		}
	*/
	char buf[256];
	uint32 oldValue = obj->GetUInt32Value(field);
	obj->SetUInt32Value(field, value);

	snprintf((char*)buf, 256, "Set Field %i from %i to %i.", (unsigned int)field, (unsigned int)oldValue, (unsigned int)value);

	if(obj->IsPlayer())
		TO< Player* >(obj)->UpdateChances();

	SystemMessage(m_session, buf);

	return true;
}

bool ChatHandler::HandleDebugDumpCoordsCommmand(const char* args, WorldSession* m_session)
{
	Player* p = m_session->GetPlayer();
	//char buffer[200] = {0};
	FILE* f = fopen("C:\\script_dump.txt", "a");
	if(!f) return true;

	fprintf(f, "mob.CreateWaypoint(%f, %f, %f, %f, 0, 0, 0);\n", p->GetPositionX(), p->GetPositionY(), p->GetPositionZ(),
	        p->GetOrientation());
	fclose(f);
	return true;
}

//As requested by WaRxHeAd for database development.
//This should really only be available in special cases and NEVER on real servers... -DGM

//#define _ONLY_FOOLS_TRY_THIS_

bool ChatHandler::HandleSQLQueryCommand(const char* args, WorldSession* m_session)
{
#ifdef _ONLY_FOOLS_TRY_THIS_
	if(!*args)
	{
		RedSystemMessage(m_session, "No query given.");
		return false;
	}

	bool isok = false;
	//SQL query lenght is seems to be limited to 16384 characters, thus the check
	if(strlen(args) > 16384)
	{
		RedSystemMessage(m_session, "Query is longer than 16384 chars!");
		//Now let the user now what are we talking about
		GreenSystemMessage(m_session, args);
	}
	else
	{
		//First send back what we got. Feedback to the user of the command.
		GreenSystemMessage(m_session, args);
		//Sending the query, but this time getting the result back
		QueryResult* qResult = WorldDatabase.Query(args);
		if(qResult != NULL)
		{
			Field* field;
			//Creating the line (instancing)
			std::string line = "";
			do
			{
				field = qResult->Fetch();
				for(uint32 i = 0; i < (qResult->GetFieldCount()); i++)
				{
					//Constructing the line
					line += field[i].GetString();
				}
				//Sending the line as ingame message
				BlueSystemMessage(m_session, line.data());
				//Clear the line
				line.clear();
			}
			while(qResult->NextRow());
			delete field;
		}
		else
		{
			RedSystemMessage(m_session, "Invalid query, or the Databse might be busy.");
			isok = false;
		}
		//delete qResult anyway, not to cause some leak!
		delete qResult;
		isok = true;
	}

	if(isok)
		GreenSystemMessage(m_session, "Query was executed successfully.");
	else
		RedSystemMessage(m_session, "Query failed to execute.");

#endif

	return true;
}

bool ChatHandler::HandleSendpacket(const char* args, WorldSession* m_session)
{
#ifdef _ONLY_FOOLS_TRY_THIS_

	uint32 arg_len = strlen(args);
	char* xstring = new char [arg_len];
	memcpy(xstring, args, arg_len);

	for(uint32 i = 0; i < arg_len; i++)
	{
		if(xstring[i] == ' ')
		{
			xstring[i] = '\0';
		}
	}

	// we receive our packet as hex, that means we get it like ff ff ff ff
	// the opcode consists of 2 bytes

	if(!xstring)
	{
		LOG_DEBUG("[Debug][Sendpacket] Packet is invalid");
		return false;
	}

	WorldPacket data(arg_len);

	uint32 loop = 0;
	uint16 opcodex = 0;
	uint16 opcodez = 0;

	// get the opcode
	sscanf(xstring , "%x", &opcodex);

	// should be around here
	sscanf(&xstring[3] , "%x", &opcodez);

	opcodex =  opcodex << 8;
	opcodex |= opcodez;
	data.Initialize(opcodex);


	int j = 3;
	int x = 0;
	do
	{
		if(xstring[j] == '\0')
		{
			uint32 HexValue;
			sscanf(&xstring[j + 1], "%x", &HexValue);
			if(HexValue > 0xFF)
			{
				LOG_DEBUG("[Debug][Sendpacket] Packet is invalid");
				return false;
			}
			data << uint8(HexValue);
			//j++;
		}
		j++;
	}
	while(j < arg_len);

	data.hexlike();

	m_session->SendPacket(&data);

	LOG_DEBUG("[Debug][Sendpacket] Packet was send");
#endif
	return true;
}

bool ChatHandler::HandleDebugSpawnWarCommand(const char* args, WorldSession* m_session)
{
	uint32 count, npcid;
	uint32 health = 0;

	// takes 2 or 3 arguments: npcid, count, (health)
	if(sscanf(args, "%u %u %u", &npcid, &count, &health) != 3)
		if(sscanf(args, "%u %u", &count, &npcid) != 2)
			return false;

	if(!count || !npcid)
		return false;

	CreatureProto* cp = CreatureProtoStorage.LookupEntry(npcid);
	CreatureInfo* ci = CreatureNameStorage.LookupEntry(npcid);
	if(cp == NULL || ci == NULL)
		return false;

	MapMgr* m = m_session->GetPlayer()->GetMapMgr();

	// if we have selected unit, use its position
	Unit* unit = m->GetUnit(m_session->GetPlayer()->GetSelection());
	if(unit == NULL)
		unit = m_session->GetPlayer(); // otherwise ours

	float bx = unit->GetPositionX();
	float by = unit->GetPositionY();
	float x, y, z;

	float angle = 1;
	float r = 3; // starting radius
	for(; count > 0; --count)
	{
		// spawn in spiral
		x = r * sinf(angle);
		y = r * cosf(angle);
		z = m->GetLandHeight(bx + x, by + y, unit->GetPositionZ() + 2);

		Creature* c = m->CreateCreature(npcid);
		c->Load(cp, bx + x, by + y, z, 0.0f);
		if(health != 0)
		{
			c->SetUInt32Value(UNIT_FIELD_MAXHEALTH, health);
			c->SetUInt32Value(UNIT_FIELD_HEALTH, health);
		}
		c->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, (count % 2) ? 1 : 2);
		c->_setFaction();
		c->PushToWorld(m);

		r += 0.5;
		angle += 8 / r;
	}
	return true;
}

bool ChatHandler::HandleUpdateWorldStateCommand( const char *args, WorldSession *session ){
	if( *args == '\0' ){
		RedSystemMessage( session, "You need to specify the worldstate field and the new value." );
		return true;
	}

	uint32 field = 0;
	uint32 state = 0;

	std::stringstream ss( args );

	ss >> field;
	if( ss.fail() ){
		RedSystemMessage( session, "You need to specify the worldstate field and the new value." );
		return true;
	}

	ss >> state;
	if( ss.fail() ){
		RedSystemMessage( session, "You need to specify the worldstate field and the new value." );
		return true;
	}

	Messenger::SendWorldStateUpdate( session->GetPlayer(), field, state );

	GreenSystemMessage( session, "Set worldstate field %u to %u", field, state );

	return true;
}

bool ChatHandler::HandleInitWorldStatesCommand(const char *args, WorldSession *session){
	Player *p = session->GetPlayer();

	uint32 zone = p->GetZoneId();
	if( zone == 0 )
		zone = p->GetAreaID();

	BlueSystemMessage( session, "Sending initial worldstates for zone %u", zone );

	p->SendInitialWorldstates();	

	return true;
}


bool ChatHandler::HandleClearWorldStatesCommand( const char *args, WorldSession *session ){
	Player *p = session->GetPlayer();

	uint32 zone = p->GetZoneId();
	if( zone == 0 )
		zone = p->GetAreaID();

	BlueSystemMessage( session, "Clearing worldstates for zone %u", zone );

	WorldPacket data( SMSG_INIT_WORLD_STATES, 16 );
	
	data << uint32( p->GetMapId() );
	data << uint32( p->GetZoneId() );
	data << uint32( p->GetAreaID() );
	data << uint16( 0 );

	p->SendPacket( &data );

	return true;
}

bool ChatHandler::HandleAddWorldStateCommand( const char *args, WorldSession *session ){
	if( *args == '\0' ){
		RedSystemMessage( session, "You need to specify the worldstate field and the value." );
		return true;
	}

	uint32 field = 0;
	uint32 state = 0;

	std::stringstream ss( args );

	ss >> field;
	if( ss.fail() ){
		RedSystemMessage( session, "You need to specify the worldstate field and the value." );
		return true;
	}

	ss >> state;
	if( ss.fail() ){
		RedSystemMessage( session, "You need to specify the worldstate field and the value." );
		return true;
	}

	WorldState ws;
	ws.field = field;
	ws.value = state;

	Player *player = session->GetPlayer();
	MapMgr *mapMgr = player->GetMapMgr();
	WorldStatesHandler &worldStatesHandler = mapMgr->GetWorldStatesHandler();

	worldStatesHandler.addWorldState( player->GetZoneId(), ws );

	GreenSystemMessage( session, "Added worldstate field %u with initial value %u", field, state );

	return true;
}

bool ChatHandler::HandleRemoveWorldStateCommand( const char *args, WorldSession *session ){
	if( *args == '\0' ){
		RedSystemMessage( session, "You need to specify the worldstate field you want removed." );
		return true;
	}

	uint32 field = 0;

	std::stringstream ss( args );

	ss >> field;
	if( ss.fail() ){
		RedSystemMessage( session, "You need to specify the worldstate field and the value." );
		return true;
	}

	Player *player = session->GetPlayer();
	MapMgr *mapMgr = player->GetMapMgr();
	WorldStatesHandler &worldStatesHandler = mapMgr->GetWorldStatesHandler();

	mapMgr->onWorldStateUpdate( player->GetZoneId(), field, 0 );
	
	worldStatesHandler.removeWorldState( player->GetZoneId(), field );

	GreenSystemMessage( session, "Removed worldstate field %u.", field );

	return true;
}

bool ChatHandler::HandleDebugPlaySoundCommand(const char *args, WorldSession *session)
{
	Player *player = session->GetPlayer();

	uint32 sound = atol( args );
	if( sound == 0 )
	{
		RedSystemMessage( session, "You need to specify a sound id. Example: 5154" );
		return true;
	}

	Messenger::PlaySoundToSet( player, sound );

	return true;
}

bool ChatHandler::HandleDebugPlaySpellVisualCommand( const char *args, WorldSession *session )
{
	Player *player = session->GetPlayer();

	MapMgr* mapMgr = player->GetMapMgr();
	Unit* selectedUnit = mapMgr->GetUnit( player->GetSelection() );
	if( selectedUnit == NULL )
	{
		RedSystemMessage( session, "You need to select a unit." );
		return true;
	}

	uint32 spellVisual = atol( args );
	if( spellVisual == 0 )
	{
		RedSystemMessage( session, "You need to specify a spell visual id. Example: 150 or 179" );
		return true;
	}

	Messenger::SendPlaySpellVisualToSet( selectedUnit, spellVisual );

	return true;
}

bool ChatHandler::HandleDebugShowEmoteCommand(const char *args, WorldSession *session)
{
	Player *player = session->GetPlayer();

	MapMgr* mapMgr = player->GetMapMgr();
	Unit* selectedUnit = mapMgr->GetUnit( player->GetSelection() );
	if( selectedUnit == NULL )
	{
		RedSystemMessage( session, "You need to select a unit." );
		return true;
	}

	uint32 emote = atol( args );
	if( emote == 0 )
	{
		RedSystemMessage( session, "You need to specify an emote id." );
		return true;
	}

	SystemMessageToPlr( player, "Showing emote %u", emote );

	Messenger::SendEmoteMessageToSet( selectedUnit, emote );

	return true;
}

bool ChatHandler::HandleDebugSendMOTDCommand( const char *args, WorldSession *session )
{
	Player *player = session->GetPlayer();
	uint32 value;
	char text[ 500 ];
	if( sscanf( args, "%u %s", &value, text ) != 2 )
	{
		RedSystemMessage( session, "Usage: sendmotd <value> <text>" );
		return true;
	}

	Messenger::SendMOTD( player, value, text );

	return true;
}

bool ChatHandler::HandleDebugSendLFGJoinResultCommand( const char *args, WorldSession *session )
{
	Player *player = session->GetPlayer();
	uint32 result;
	uint32 state;

	if( sscanf( args, "%u %u", &result, &state ) != 2 )
	{
		RedSystemMessage( session, "Usage: sendlfgjoinresult <result> <state>" );
		return true;
	}

	PacketBuffer buffer;
	Arcemu::GamePackets::LFG::SLFGJoinResult packet;
	packet.result = result;
	packet.state = state;
	packet.serialize( buffer );
	player->SendPacket( &buffer );

	return true;
}

bool ChatHandler::HandleDebugSendLFGPlayerUpdateCommand( const char *args, WorldSession *session )
{
	Player *player = session->GetPlayer();
	uint32 type = 0;
	uint32 queued = 0;
	uint32 dungeon = 0;
	char comment[ 500 ];

	int count = sscanf( args, "%u %u %u %s", &type, &queued, &dungeon, comment );
	if( count < 2 )
	{
		RedSystemMessage( session, "Usage: sendlfgplayerupdate <type> <queued> <dungeon> <comment>" );
		RedSystemMessage( session, "type and queued parameters are mandatory." );
		return true;
	}

	PacketBuffer buffer;
	Arcemu::GamePackets::LFG::SLFGUpdatePlayer packet;
	packet.updateType = static_cast< uint8 >( type );
	packet.queued = static_cast< uint8 >( queued );

	if( count >= 3 )
	{
		packet.dungeons.push_back( dungeon );
	}

	if( count == 4 )
	{
		packet.comment.assign( comment );
	}

	packet.serialize( buffer );
	player->SendPacket( &buffer );

	return true;
}

bool ChatHandler::HandleDebugSendLFGPartyUpdateCommand( const char *args, WorldSession *session )
{
	Player *player = session->GetPlayer();
	uint32 type = 0;
	uint32 queued = 0;
	uint32 joined = 0;
	uint32 dungeon = 0;
	char comment[ 500 ];

	int count = sscanf( args, "%u %u %u %u %s", &type, &queued, &joined, &dungeon, comment );
	if( count < 3 )
	{
		RedSystemMessage( session, "Usage: sendlfgpartyupdate <type> <queued> <joined> <dungeon> <comment>" );
		RedSystemMessage( session, "type, queued, joined parameters are mandatory." );
		return true;
	}

	PacketBuffer buffer;
	Arcemu::GamePackets::LFG::SLFGUpdateParty packet;
	packet.updateType = static_cast< uint8 >( type );
	packet.queued = static_cast< uint8 >( queued );
	packet.joined = static_cast< uint8 >( joined );

	if( count >= 4 )
	{
		packet.dungeons.push_back( dungeon );
	}

	if( count == 5 )
	{
		packet.comment.assign( comment );
	}

	packet.serialize( buffer );
	player->SendPacket( &buffer );

	return true;
}

bool ChatHandler::HandleDebugSendLFGProposalUpdateCommand( const char *args, WorldSession *session )
{
	Player *player = session->GetPlayer();

	uint32 dungeon = 0;
	uint32 state = 0;
	uint32 encounters = 0;
	uint32 silent = 0;

	int count = sscanf( args, "%u %u %u %u", &dungeon, &state, &encounters, &silent );
	if( count != 4 )
	{
		RedSystemMessage( session, "Usage: sendlfgproposalupdate <dungeon> <state> <encounters> <silent>" );
		return true;
	}

	PacketBuffer buffer( SMSG_LFG_PROPOSAL_UPDATE, 512 );

	Arcemu::GamePackets::LFG::SLFGProposalUpdate packet;
	packet.dungeonId = dungeon;
	packet.proposalState = state;
	packet.proposalId = 123;
	packet.encountersFinishedMask = encounters;
	packet.silent = silent;

	Arcemu::GamePackets::LFG::SLFGProposalUpdate::LFGProposalEntry entry;
	entry.roleMask = Arcemu::GamePackets::LFG::SLFGProposalUpdate::LFG_ROLE_LEADER | Arcemu::GamePackets::LFG::SLFGProposalUpdate::LFG_ROLE_TANK;
	entry.isCurrentPlayer = 1;
	entry.inDungeon = 0;
	entry.inSameGroup = 0;
	entry.hasAnswered = 0;
	entry.hasAccepted = 0;
	packet.entries.push_back( entry );

	entry.roleMask = Arcemu::GamePackets::LFG::SLFGProposalUpdate::LFG_ROLE_HEALER;
	entry.isCurrentPlayer = 0;
	entry.inDungeon = 0;
	entry.inSameGroup = 0;
	entry.hasAnswered = 1;
	entry.hasAccepted = 1;
	packet.entries.push_back( entry );

	entry.roleMask = Arcemu::GamePackets::LFG::SLFGProposalUpdate::LFG_ROLE_DPS;
	entry.isCurrentPlayer = 0;
	entry.inDungeon = 0;
	entry.inSameGroup = 0;
	entry.hasAnswered = 1;
	entry.hasAccepted = 1;
	packet.entries.push_back( entry );

	entry.roleMask = Arcemu::GamePackets::LFG::SLFGProposalUpdate::LFG_ROLE_DPS;
	entry.isCurrentPlayer = 0;
	entry.inDungeon = 0;
	entry.inSameGroup = 0;
	entry.hasAnswered = 1;
	entry.hasAccepted = 1;
	packet.entries.push_back( entry );

	entry.roleMask = Arcemu::GamePackets::LFG::SLFGProposalUpdate::LFG_ROLE_DPS;
	entry.isCurrentPlayer = 0;
	entry.inDungeon = 0;
	entry.inSameGroup = 0;
	entry.hasAnswered = 1;
	entry.hasAccepted = 1;
	packet.entries.push_back( entry );

	packet.serialize( buffer );
	player->SendPacket( &buffer );

	return true;
}

bool ChatHandler::HandleDebugLFGUpdateCommand( const char *args, WorldSession *session )
{
	Player *player = session->GetPlayer();

	uint32 force = 0;

	sscanf( args, "%u", &force );

	if( force == 1 )
	{
		BlueSystemMessage( session, "Triggering an LFG update with force" );
		sLfgMgr.update(true);
	}
	else
	{
		BlueSystemMessage( session, "Triggering an LFG update" );
		sLfgMgr.update(false);
	}	

	BlueSystemMessage( session, "Done" );

	return true;
}

bool ChatHandler::HandleDebugSendLFGQueueStatusCommand( const char *args, WorldSession *session )
{
	Player *player = session->GetPlayer();

	uint32 dungeon;
	int32 waitTimeAvg;
	uint32 tanksNeeded;
	uint32 healersNeeded;
	uint32 dpsNeeded;

	if( sscanf( args, "%u %i %u %u %u", &dungeon, &waitTimeAvg, &tanksNeeded, &healersNeeded, &dpsNeeded ) != 5 )
	{
		RedSystemMessage( session, "usage:" );
		RedSystemMessage( session, "sendlfgqueuestatus [dungeon] [avg waittime] [tanks needed] [healers needed] [dps needed]" );
		return true;
	}

	Arcemu::GamePackets::LFG::SLFGQueueStatus status;
	status.dungeon = dungeon;
	status.avgWaitTime = waitTimeAvg;
	status.waitTime = 0;
	status.waitTimeTank = 90;
	status.waitTimeHealer = 150;
	status.waitTimeDps = 210;
	status.tanksNeeded = static_cast< uint8 >( tanksNeeded );
	status.healersNeeded = static_cast< uint8 >( healersNeeded );
	status.dpsNeeded = static_cast< uint8 >( dpsNeeded );
	status.queueTime = 65;

	PacketBuffer buffer;
	status.serialize( buffer );
	player->SendPacket( &buffer );

	return true;
}

bool ChatHandler::HandleDebugDamageUnitCommand( const char *args, WorldSession *session )
{
	uint32 damage;
	
	if( sscanf( args, "%u", &damage ) != 1 )
	{
		RedSystemMessage( session, "usage:" );
		RedSystemMessage( session, "damageunit [damage]" );
		return true;
	}

	Player *player = session->GetPlayer();
	uint64 guid = player->GetSelection();

	if( guid == 0 )
	{
		RedSystemMessage( session, "You need to select a target unit" );
		return true;
	}

	Unit *target = player->GetMapMgr()->GetUnit( guid );

	/// Shouldn't happen
	if( target == NULL )
	{
		return true;
	}

	player->Strike( target, MELEE, NULL, 0, 0, damage, true, true, false );

	return true;
}

