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

/////////////////////////////////////////////////
//  Debug Chat Commands
//

#include "StdAfx.h"

bool ChatHandler::HandleDebugInFrontCommand(const char* args, WorldSession *m_session)
{
	Object *obj;

	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid != 0)
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
	snprintf((char*)buf, 256, "%d", m_session->GetPlayer()->isInFront((Unit *)obj));

	SystemMessage(m_session, buf);

	return true;
}

bool ChatHandler::HandleShowReactionCommand(const char* args, WorldSession *m_session)
{
	Object *obj = NULL;

	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid != 0)
	{
		obj = m_session->GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	}

	if(!obj)
	{
		SystemMessage(m_session, "You should select a creature.");
		return true;
	}


	char* pReaction = strtok((char*)args, " ");
	if (!pReaction)
		return false;

	uint32 Reaction  = atoi(pReaction);

	WorldPacket data(SMSG_AI_REACTION, 12);
	data << obj->GetGUID() << uint32(Reaction);
	m_session->SendPacket( &data );

	std::stringstream sstext;
	sstext << "Sent Reaction of " << Reaction << " to " << obj->GetUIdFromGUID() << '\0';

	SystemMessage(m_session,  sstext.str().c_str());
	return true;
}

bool ChatHandler::HandleDistanceCommand(const char* args, WorldSession *m_session)
{
	Object *obj;

	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid != 0)
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
	sstext << "Distance is: " << dist <<'\0';

	SystemMessage(m_session, sstext.str().c_str());
	return true;
}

bool ChatHandler::HandleMoveInfoCommand(const char* args, WorldSession *m_session)
{
	Object *obj;

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
	bool run = TO< Creature* >(obj)->GetAIInterface()->getMoveRunFlag();
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

bool ChatHandler::HandleAIMoveCommand(const char* args, WorldSession *m_session)
{
	Object *obj = NULL;

	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid != 0)
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
	if (pMove)
		Move  = atoi(pMove);

	char* pRun = strtok(NULL, " ");
	if (pRun)
		Run  = atoi(pRun);

	char* pTime = strtok(NULL, " ");
	if (pTime)
		Time  = atoi(pTime);

	char* pMeth = strtok(NULL, " ");
	if (pMeth)
		Meth  = atoi(pMeth);

	float x = m_session->GetPlayer()->GetPositionX();
	float y = m_session->GetPlayer()->GetPositionY();
	float z = m_session->GetPlayer()->GetPositionZ();
	float o = m_session->GetPlayer()->GetOrientation();
	TO< Creature* >(obj)->GetAIInterface()->setMoveRunFlag((Run>0?true:false));
	float distance = TO< Creature* >(obj)->CalcDistance(x,y,z);
	if(Move == 1)
	{
		if(Meth == 1)
		{
			float q = distance-0.5f;
			x = (TO< Creature* >(obj)->GetPositionX()+x*q)/(1+q);
			y = (TO< Creature* >(obj)->GetPositionY()+y*q)/(1+q);
			z = (TO< Creature* >(obj)->GetPositionZ()+z*q)/(1+q);
		}
		else if(Meth == 2)
		{
			float q = distance-1;
			x = (TO< Creature* >(obj)->GetPositionX()+x*q)/(1+q);
			y = (TO< Creature* >(obj)->GetPositionY()+y*q)/(1+q);
			z = (TO< Creature* >(obj)->GetPositionZ()+z*q)/(1+q);
		}
		else if(Meth == 3)
		{
			float q = distance-2;
			x = (TO< Creature* >(obj)->GetPositionX()+x*q)/(1+q);
			y = (TO< Creature* >(obj)->GetPositionY()+y*q)/(1+q);
			z = (TO< Creature* >(obj)->GetPositionZ()+z*q)/(1+q);
		}
		else if(Meth == 4)
		{
			float q = distance-2.5f;
			x = (TO< Creature* >(obj)->GetPositionX()+x*q)/(1+q);
			y = (TO< Creature* >(obj)->GetPositionY()+y*q)/(1+q);
			z = (TO< Creature* >(obj)->GetPositionZ()+z*q)/(1+q);
		}
		else if(Meth == 5)
		{
			float q = distance-3;
			x = (TO< Creature* >(obj)->GetPositionX()+x*q)/(1+q);
			y = (TO< Creature* >(obj)->GetPositionY()+y*q)/(1+q);
			z = (TO< Creature* >(obj)->GetPositionZ()+z*q)/(1+q);
		}
		else if(Meth == 6)
		{
			float q = distance-3.5f;
			x = (TO< Creature* >(obj)->GetPositionX()+x*q)/(1+q);
			y = (TO< Creature* >(obj)->GetPositionY()+y*q)/(1+q);
			z = (TO< Creature* >(obj)->GetPositionZ()+z*q)/(1+q);
		}
		else
		{
			float q = distance-4;
			x = (TO< Creature* >(obj)->GetPositionX()+x*q)/(1+q);
			y = (TO< Creature* >(obj)->GetPositionY()+y*q)/(1+q);
			z = (TO< Creature* >(obj)->GetPositionZ()+z*q)/(1+q);
		}
		TO< Creature* >(obj)->GetAIInterface()->MoveTo(x,y,z,0);
	}
	else
	{
		uint32 moveTime = 0;
		if(!Time)
		{
			//float dx = x - ((Creature *)obj)->GetPositionX();
			//float dy = y - ((Creature *)obj)->GetPositionY();
			//float dz = z - ((Creature *)obj)->GetPositionZ();

			//float distance = sqrt((dx*dx) + (dy*dy) + (dz*dz));
			if(!distance)
			{
				SystemMessage(m_session, "The Creature is already there.");
				return true;
			}

			float moveSpeed = 1.0f;
			if(!Run)
			{
				moveSpeed = 2.5f*0.001f;
			}
			else
			{
				moveSpeed = 7.0f*0.001f;
			}

			moveTime = uint32(distance / moveSpeed);
		}
		else
		{
			moveTime = Time;
		}
		//((Creature *)obj)->setMovementState(MOVING);
		TO< Creature* >(obj)->GetAIInterface()->SendMoveToPacket(x,y,z,o,moveTime,Run);
	}
	return true;
}

bool ChatHandler::HandleFaceCommand(const char* args, WorldSession *m_session)
{

	Object *obj = NULL;

	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid != 0)
	{
		obj = m_session->GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	}

	if(obj == NULL)
	{
		SystemMessage(m_session,  "You should select a creature.");
		return true;
	}

	uint32 Orentation = 0;
	char* pOrentation = strtok((char*)args, " ");
	if (pOrentation)
		Orentation  = atoi(pOrentation);

	/* Convert to Blizzards Format */
	float theOrientation = Orentation/(180.0f/float(M_PI));

	obj->SetPosition(obj->GetPositionX(), obj->GetPositionY(), obj->GetPositionZ(), theOrientation, false);

	/*
	data.Initialize( SMSG_MONSTER_MOVE );
	data << obj->GetGUID();
	data << obj->GetPositionX() << obj->GetPositionY() << obj->GetPositionZ() << obj->GetOrientation();
	data << uint8(1);
	
	data << uint32(0x100); //run
	data << uint32(0); //time
	data << uint32(2);
	data << obj->GetPositionX() << obj->GetPositionY() << obj->GetPositionZ() << theOrientation;
	*/
	//UpdateData upd;

	// update movment for others
	//obj->BuildMovementUpdateBlock(&upd,0);
	//upd.BuildPacket( &data );
	//GetSession()->SendPacket( &packet );
	//obj->BuildMovementUpdateBlock(data,0)
	//obj->SendMessageToSet(&data,false);
	sLog.outDebug("facing sent");
	return true;
	//((Creature *)obj)->AI_MoveTo(obj->GetPositionX()+0.1,obj->GetPositionY()+0.1,obj->GetPositionZ()+0.1,theOrientation);
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
bool ChatHandler::HandleSetBytesCommand(const char* args, WorldSession *m_session)
{
	Object *obj;

	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid != 0)
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
	if (!pBytesIndex)
		return false;

	uint32 BytesIndex  = atoi(pBytesIndex);

	char* pValue1 = strtok(NULL, " ");
	if (!pValue1)
		return false;

	uint8 Value1  = static_cast<uint8>(atol(pValue1));

	char* pValue2 = strtok(NULL, " ");
	if (!pValue2)
		return false;

	uint8 Value2  = static_cast<uint8>(atol(pValue2));

	char* pValue3 = strtok(NULL, " ");
	if (!pValue3)
		return false;

	uint8 Value3  = static_cast<uint8>(atol(pValue3));

	char* pValue4 = strtok(NULL, " ");
	if (!pValue4)
		return false;

	uint8 Value4  = static_cast<uint8>(atol(pValue4));

	std::stringstream sstext;
	sstext << "Set Field " << BytesIndex << " bytes to " << uint16((uint8)Value1) << " " << uint16((uint8)Value2) << " " << uint16((uint8)Value3) << " " << uint16((uint8)Value4) << '\0';
	obj->SetUInt32Value(BytesIndex, ( ( Value1 ) | ( Value2 << 8 ) | ( Value3 << 16 ) | ( Value4 << 24 ) ) );
	SystemMessage(m_session, sstext.str().c_str());
	return true;
}

bool ChatHandler::HandleGetBytesCommand(const char* args, WorldSession *m_session)
{
	Object *obj;

	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid != 0)
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
	if (!pBytesIndex)
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
bool ChatHandler::HandleDebugLandWalk(const char* args, WorldSession *m_session)
{
	Player *chr = getSelectedChar(m_session);
	char buf[256];

	if (chr == NULL) // Ignatich: what should NOT happen but just in case...
	{
		SystemMessage(m_session, "No character selected.");
		return false;
	}
	chr->SetMovement(MOVE_LAND_WALK,8);
	snprintf((char*)buf,256, "Land Walk Test Ran.");
	SystemMessage(m_session, buf);
	return true;
}

bool ChatHandler::HandleDebugWaterWalk(const char* args, WorldSession *m_session)
{
	Player *chr = getSelectedChar(m_session);
	char buf[256];

	if (chr == NULL) // Ignatich: what should NOT happen but just in case...
	{
		SystemMessage(m_session, "No character selected.");
		return false;
	}
	chr->SetMovement(MOVE_WATER_WALK, 4);
	snprintf((char*)buf,256, "Water Walk Test Ran.");
	SystemMessage(m_session,  buf);
	return true;
}

bool ChatHandler::HandleDebugUnroot(const char* args, WorldSession *m_session)
{
	Player *chr = getSelectedChar(m_session);
	char buf[256];

	if (chr == NULL) // Ignatich: what should NOT happen but just in case...
	{
		SystemMessage(m_session,  "No character selected.");
		return false;
	}

	chr->SetMovement(MOVE_UNROOT,5);

	snprintf((char*)buf,256, "UnRoot Test Ran.");
	SystemMessage(m_session, buf);
	return true;
}

bool ChatHandler::HandleDebugRoot(const char* args, WorldSession *m_session)
{
	Player *chr = getSelectedChar(m_session);
	char buf[256];

	if (chr == NULL) // Ignatich: what should NOT happen but just in case...
	{
		SystemMessage(m_session, "No character selected.");
		return true;
	}
	chr->SetMovement(MOVE_ROOT,1);
	snprintf((char*)buf,256, "Root Test Ran.");
	SystemMessage(m_session, buf);
	return true;
}

bool ChatHandler::HandleAggroRangeCommand(const char* args, WorldSession *m_session)
{
	Unit *obj = NULL;

	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid != 0)
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
	sstext << "Aggrorange is: " << sqrtf(aggroRange) <<'\0';

	SystemMessage(m_session, sstext.str().c_str());

	return true;
}

bool ChatHandler::HandleKnockBackCommand(const char* args, WorldSession *m_session)
{
	float f = args ? (float)atof(args) : 0.0f;
	if(f == 0.0f)
		f = 5.0f;

	float dx = sinf(m_session->GetPlayer()->GetOrientation());
	float dy = cosf(m_session->GetPlayer()->GetOrientation());

	float z = f*0.66f;

	WorldPacket data(SMSG_MOVE_KNOCK_BACK, 50);
	data << m_session->GetPlayer()->GetNewGUID();
	data << getMSTime();
	data << dy;
	data << dx;
	data << f;
	data << z;
	m_session->SendPacket(&data);
	return true;
}

bool ChatHandler::HandleFadeCommand(const char* args, WorldSession *m_session)
{
	Unit* target = m_session->GetPlayer()->GetMapMgr()->GetUnit(m_session->GetPlayer()->GetSelection());
	if(!target)
		target = m_session->GetPlayer();
	char* v = strtok((char*)args, " ");
	if(!v)
		return false;

	target->ModThreatModifyer(atoi(v));

	std::stringstream sstext;
	sstext << "threat is now reduced by: " << target->GetThreatModifyer() <<'\0';

	SystemMessage(m_session, sstext.str().c_str());
	return true;
}

bool ChatHandler::HandleThreatModCommand(const char* args, WorldSession *m_session)
{
	Unit* target = m_session->GetPlayer()->GetMapMgr()->GetUnit(m_session->GetPlayer()->GetSelection());
	if(!target)
		target = m_session->GetPlayer();
	char* v = strtok((char*)args, " ");
	if(!v)
		return false;

	target->ModGeneratedThreatModifyer(0, atoi(v));

	std::stringstream sstext;
	sstext << "new threat caused is now reduced by: " << target->GetGeneratedThreatModifyer(0) << "%" <<'\0';

	SystemMessage(m_session, sstext.str().c_str());
	return true;
}

bool ChatHandler::HandleCalcThreatCommand(const char* args, WorldSession *m_session)
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

	uint32 threat = target->GetAIInterface()->_CalcThreat(atol(dmg), dbcSpell.LookupEntry( atoi( spellId ) ), m_session->GetPlayer());

	std::stringstream sstext;
	sstext << "generated threat is: " << threat <<'\0';

	SystemMessage(m_session, sstext.str().c_str());
	return true;
}

bool ChatHandler::HandleThreatListCommand(const char* args, WorldSession *m_session)
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
		Unit *ai_t = target->GetMapMgr()->GetUnit( itr->first );
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
	char* arg = const_cast<char*>( args );
	char* token = strtok(arg, " ");
	
	uint8 i = 0;
	while( token != NULL && i < 7 )
	{
		uint_args[i] = atol( token );
		token = strtok(NULL," ");
		i++;
	}
	for( ; i < 7; i++)
		uint_args[i] = 0;

	if( uint_args[0] == 0 ) // null itemid
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

	Object *obj;

	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid != 0)
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
	if (!pField)
		return false;

	char* pBit = strtok(NULL, " ");
	if (!pBit)
		return false;

	uint16 field = static_cast<uint16>(atoi(pField));
	uint32 bit   = atoi(pBit);

	if (field < 1 || field >= PLAYER_END)
	{
		SystemMessage(m_session, "Incorrect values.");
		return true;
	}

	if (bit < 1 || bit > 32)
	{
		SystemMessage(m_session, "Incorrect values.");
		return true;
	}

	char buf[256];

	if ( obj->HasFlag( field, (1<<(bit-1)) ) )
	{
		obj->RemoveFlag( field, (1<<(bit-1)) );
		snprintf((char*)buf,256, "Removed bit %i in field %i.", (unsigned int)bit, (unsigned int)field);
	}
	else
	{
		obj->SetFlag( field, (1<<(bit-1)) );
		snprintf((char*)buf,256, "Set bit %i in field %i.", (unsigned int)bit, (unsigned int)field);
	}

	SystemMessage(m_session, buf);
	return true;
}

bool ChatHandler::HandleModifyValueCommand(const char* args,  WorldSession* m_session)
{
	Object *obj;

	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid != 0)
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
	if (!pField)
		return false;

	char* pValue = strtok(NULL, " ");
	if (!pValue)
		return false;

	uint16 field = static_cast<uint16>(atoi(pField));
	uint32 value   = atoi(pValue);

	if (field < 1 || field >= PLAYER_END)
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
	obj->SetUInt32Value(field,value);

	snprintf((char*)buf,256,"Set Field %i from %i to %i.", (unsigned int)field, (unsigned int)oldValue, (unsigned int)value);

	if( obj->IsPlayer() )
		TO< Player* >( obj )->UpdateChances();

	SystemMessage(m_session, buf);

	return true;
}

bool ChatHandler::HandleDebugDumpCoordsCommmand(const char * args, WorldSession * m_session)
{
	Player* p = m_session->GetPlayer();
	//char buffer[200] = {0};
	FILE * f = fopen("C:\\script_dump.txt", "a");
	if(!f) return true;

	fprintf(f, "mob.CreateWaypoint(%f, %f, %f, %f, 0, 0, 0);\n", p->GetPositionX(), p->GetPositionY(), p->GetPositionZ(),
		p->GetOrientation());
	fclose(f);
	return true;
}

//As requested by WaRxHeAd for database development.
//This should really only be available in special cases and NEVER on real servers... -DGM

//#define _ONLY_FOOLS_TRY_THIS_

bool ChatHandler::HandleSQLQueryCommand(const char* args, WorldSession *m_session)
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
			QueryResult * qResult = WorldDatabase.Query(args);
			if(qResult != NULL)
			{
				Field *field;
				//Creating the line (instancing)
				std::string line ="";
				do 
				{
					field = qResult->Fetch();
					for(uint32 i= 0;i<(qResult->GetFieldCount());i++)
					{
						//Constructing the line
						line+= field[i].GetString();
					}
					//Sending the line as ingame message
					BlueSystemMessage(m_session,line.data());
					//Clear the line
					line.clear();
				} while(qResult->NextRow());
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

bool ChatHandler::HandleSendpacket(const char * args, WorldSession * m_session)
{
#ifdef _ONLY_FOOLS_TRY_THIS_

    uint32 arg_len = strlen(args);
    char * xstring = new char [arg_len];
    memcpy(xstring, args,arg_len);

    for (uint32 i = 0; i < arg_len; i++)
    {
        if (xstring[i] == ' ')
        {
            xstring[i] = '\0';
        }
    }

    // we receive our packet as hex, that means we get it like ff ff ff ff
    // the opcode consists of 2 bytes

    if (!xstring)
    {
        sLog.outDebug("[Debug][Sendpacket] Packet is invalid");
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
        if (xstring[j] == '\0')
        {
            uint32 HexValue;
            sscanf(&xstring[j+1], "%x", &HexValue);
            if (HexValue > 0xFF)
            {
                sLog.outDebug("[Debug][Sendpacket] Packet is invalid");
                return false;
            }
            data << uint8(HexValue);
            //j++;
        }
        j++;
    } while(j < arg_len);

    data.hexlike();

    m_session->SendPacket(&data);

    sLog.outDebug("[Debug][Sendpacket] Packet was send");
#endif
    return true;
}

bool ChatHandler::HandleDebugSpawnWarCommand(const char *args, WorldSession *m_session)
{
	uint32 count, npcid;
	uint32 health = 0;

	// takes 2 or 3 arguments: npcid, count, (health)
	if( sscanf( args, "%u %u %u", &npcid, &count, &health ) != 3 )
		if( sscanf( args, "%u %u", &count, &npcid ) != 2 )
			return false;

	if( !count || !npcid )
		return false;

	CreatureProto *cp = CreatureProtoStorage.LookupEntry( npcid );
	CreatureInfo *ci = CreatureNameStorage.LookupEntry( npcid );
	if( cp == NULL || ci == NULL )
		return false;

	MapMgr *m = m_session->GetPlayer()->GetMapMgr();

	// if we have selected unit, use its position
	Unit* unit = m->GetUnit( m_session->GetPlayer()->GetSelection() );
	if( unit == NULL )
		unit = m_session->GetPlayer(); // otherwise ours
		
	float bx = unit->GetPositionX();
	float by = unit->GetPositionY();
	float x, y, z;

	float angle = 1;
	float r = 3; // starting radius
	for( ; count > 0; --count )
	{
		// spawn in spiral		
		x = r * sinf( angle );
		y = r * cosf( angle );
		z = m->GetBaseMap()->GetLandHeight( bx + x, by + y );
		
		Creature *c = m->CreateCreature( npcid );
		c->Load( cp, bx + x, by + y, z, 0.0f );
		if( health != 0 ) 
		{
			c->SetUInt32Value( UNIT_FIELD_MAXHEALTH, health );
			c->SetUInt32Value( UNIT_FIELD_HEALTH, health );
		}
		c->SetUInt32Value( UNIT_FIELD_FACTIONTEMPLATE, (count % 2) ? 1 : 2 );
		c->_setFaction();
		c->PushToWorld( m );

		r += 0.5;
		angle += 8 / r;
	}
	return true;
}
