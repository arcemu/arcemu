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

///////////////////////////////////////////////
//  Waypoint Commands
//

#include "StdAfx.h"

bool ChatHandler::HandleWPAddCommand(const char* args, WorldSession *m_session)
{
	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid == 0)
		SystemMessage(m_session, "No Selection");
	else
	{
		Creature * pCreature = pCreature = m_session->GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
		if( pCreature == NULL )
			SystemMessage(m_session, "You should select a creature.");
		else
		{
			MobAI * ai = AIType_isMob(pCreature->GetAIInterface() ) ? TO_AIMOB(pCreature->GetAIInterface() ) : NULL;
			if(ai == NULL)
				SystemMessage(m_session," The selected creature doesn't support waypoints.");
			else
			{
				char* pWaitTime = strtok((char*)args, " ");
				uint32 WaitTime = (pWaitTime)? atoi(pWaitTime) : 0;
				char* pFlags = strtok(NULL, " ");
				uint32 Flags = (pFlags)? atoi(pFlags) : 0 ;
				char* pForwardEmoteId = strtok(NULL, " ");
				uint32 ForwardEmoteId = (pForwardEmoteId)? atoi(pForwardEmoteId) : 0;
				char* pBackwardEmoteId = strtok(NULL, " ");
				uint32 BackwardEmoteId = (pBackwardEmoteId)? atoi(pBackwardEmoteId) : 0;
				char* pForwardSkinId = strtok(NULL, " ");
				uint32 ForwardSkinId = (pForwardSkinId)? atoi(pForwardSkinId) : pCreature->GetNativeDisplayId();
				char* pBackwardSkinId = strtok(NULL, " ");
				uint32 BackwardSkinId = (pBackwardSkinId)? atoi(pBackwardSkinId) : pCreature->GetNativeDisplayId();
				char* pForwardEmoteOneShot = strtok(NULL, " ");
				uint32 ForwardEmoteOneShot = (pForwardEmoteOneShot)? atoi(pForwardEmoteOneShot) : 1;
				char* pBackwardEmoteOneShot = strtok(NULL, " ");
				uint32 BackwardEmoteOneShot = (pBackwardEmoteOneShot)? atoi(pBackwardEmoteOneShot) : 1;

				WayPoint* wp = new WayPoint;
				wp->id = (uint8)TO_AIMOB(ai)->Waypoint_getcount()+1;
				LocationVector loc = m_session->GetPlayer()->GetPosition();
				wp->x = loc.x;
				wp->y = loc.y;
				wp->z = loc.z;
				wp->waittime = WaitTime;
				wp->flags = (uint16)Flags;
				wp->forwardemoteoneshot = (ForwardEmoteOneShot>0)?true:false;
				wp->forwardemoteid = ForwardEmoteId;
				wp->backwardemoteoneshot = (BackwardEmoteOneShot>0)?true:false;
				wp->backwardemoteid = BackwardEmoteId;
				wp->forwardskinid = ForwardSkinId;
				wp->backwardskinid = BackwardSkinId;
				TO_AIMOB(ai)->Waypoint_add(wp);
				//TO_AIMOB(ai)->Waypoint_saveall(); We can always save later when needed.
				SystemMessage(m_session, "Waypoint %u added.", wp->id);
			}
		}
	}
	return true;
}

bool ChatHandler::HandleWPMoveTypeCommand(const char* args, WorldSession *m_session)
{
	if(!*args)
		SystemMessage(m_session, "Please specify a move type option.");
	else
	{
		uint32 option = atoi((char*)args);
		if (option > 2)
		{
			std::stringstream ss;
			ss << "Incorrect value." << endl;
			ss << "0 is Move from WP 1 ->  10 then 10 -> 1." << endl;
			ss << "1 is Move from WP to a random WP." << endl;
			ss << "2 is Move from WP 1 -> 10 then 1 -> 10." << endl;
			SendMultilineMessage(m_session, ss.str().c_str());
		}
		else
		{
			uint64 guid = m_session->GetPlayer()->GetSelection();
			if (guid == 0)
				SystemMessage(m_session, "No selection.");
			else
			{
				Creature * pCreature = m_session->GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
				if(pCreature == NULL)
					SystemMessage(m_session, "You should select a creature.");
				else
				{
					MobAI * ai = AIType_isMob( pCreature->GetAIInterface() ) ? TO_AIMOB(pCreature->GetAIInterface() ) : NULL;
					if(ai != NULL)
					{
						MovementType type(MOVEMENTTYPE_DONTMOVEWP);
						switch(option)
						{
						case 0:
							type = MOVEMENTTYPE_MISC;
							break;
						case 1:
							type = MOVEMENTTYPE_RANDOMWP;
							break;
						case 2:
							type = MOVEMENTTYPE_CIRCLEWP;
							break;
						}
						ai->Waypoint_setmovetype(type);
						char sql[512];
						snprintf(sql, 512, "UPDATE creature_spawns SET movetype = '%u' WHERE id = '%u'", (uint32)type, (unsigned int)Arcemu::Util::GUID_LOPART( guid ));
						WorldDatabase.Execute( sql );
						SystemMessage(m_session, "Value saved to database.");
					}
					else
						SystemMessage(m_session, "The selected creature does not support waypoints. ");
				}
			}
		}
	}

	return true;
}


bool ChatHandler::HandleWPShowCommand(const char* args, WorldSession *m_session)
{
	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid == 0)
		SystemMessage(m_session, "No selection.");
	else
	{
		Creature * pCreature = m_session->GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
		if(pCreature == NULL)
			SystemMessage(m_session, "You should select a Creature.");
		else
		{
			if(AIType_isMob(pCreature->GetAIInterface() ) )
			{
				char* pBackwards = strtok((char*)args, " ");
				bool Backwards = (pBackwards)? ((atoi(pBackwards)>0)?true:false) : false;
				MobAI * ai = TO_AIMOB( pCreature->GetAIInterface() );
				Player* pPlayer = m_session->GetPlayer();
				if(ai->Waypoint_isShowing() )
					SystemMessage(m_session, "Waypoints for this creature are already showing.");
				else
				{
					ai->Waypoint_show(pPlayer, Backwards);
					SystemMessage(m_session, "Showing waypoints for creature %u", pCreature->GetSQL_id());
				}
			}
			else
				SystemMessage(m_session, "The selected creature does not support waypoints. ");
		}
	}
	return true;
}

bool ChatHandler::HandleWPDeleteCommand(const char* args, WorldSession *m_session)
{
	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid == 0)
		SystemMessage(m_session, "No selection.");
	else
	{
		if(GET_TYPE_FROM_GUID(guid) != HIGHGUID_TYPE_WAYPOINT)
			SystemMessage(m_session, "Please select a waypoint.");
		else
		{
			Player * plr = m_session->GetPlayer();
			//Retrieve the packed wp id.
			uint32 wpid = Arcemu::Util::GUID_HIPART(guid) & 0x000FFFFF;
			//Retrieve the packed owning creature's low guid.
			uint32 owner_creature_guid = GET_LOWGUID_PART(guid);
			Creature * owner_creature = plr->GetMapMgr()->GetCreature(owner_creature_guid);
			if(wpid && owner_creature != NULL)
			{
				MobAI * ai = AIType_isMob(owner_creature->GetAIInterface() ) ? TO_AIMOB(owner_creature->GetAIInterface() ) : NULL;
				if(ai != NULL)
				{
					ai->Waypoint_delete(wpid);
					if(ai->Waypoint_isShowing(wpid) )
						ai->Waypoint_hide(plr, wpid);
					SystemMessage(m_session, "Waypoint %u deleted.", wpid);
				}
				else
					SystemMessage(m_session, "The selected creature doesn't support waypoints.");
			}
			else
				SystemMessage(m_session, "Invalid waypoint selected.");
		}
	}
	return true;
}

bool ChatHandler::HandleWPChangeNoCommand(const char* args, WorldSession *m_session)
{
	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid == 0)
		SystemMessage(m_session, "No selection.");
	else
	{
		if(GET_TYPE_FROM_GUID(guid) != HIGHGUID_TYPE_WAYPOINT)
			SystemMessage(m_session, "You should select a Waypoint.");
		else
		{
			Player * plr = m_session->GetPlayer();

			//get newid
			char* pNewID = strtok((char*)args, " ");
			uint32 NewID = (pNewID)? atoi(pNewID) : 0;

			uint32 wpid = Arcemu::Util::GUID_HIPART(guid) & 0x000FFFFF;
			uint32 owner_creature_guid = GET_LOWGUID_PART(guid);
			Creature * owner = plr->GetMapMgr()->GetCreature(owner_creature_guid);
			if(owner != NULL && NewID > 0 && NewID != wpid)
			{
				MobAI * ai = AIType_isMob(owner->GetAIInterface() ) ? TO_AIMOB(owner->GetAIInterface() ) : NULL;
				if(ai != NULL)
				{
					ai->Waypoint_getWP(wpid)->id = (uint8)NewID;
					if(ai->Waypoint_isShowing() )
					{
						ai->Waypoint_hide(plr, NewID);
						ai->Waypoint_show(plr, NewID);
					}
					SystemMessage(m_session, "Waypoint id %u has been changed to %u",wpid, NewID);
				}
				else
					SystemMessage(m_session, "The selected creature does not support waypoints.");
			}
		}
	}
	return true;
}

bool ChatHandler::HandleWPFlagsCommand(const char* args, WorldSession *m_session)
{
	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid == 0)
		SystemMessage(m_session, "No selection.");
	else
	{
		if(GET_TYPE_FROM_GUID(guid) != HIGHGUID_TYPE_WAYPOINT)
			SystemMessage(m_session, "You should select a Waypoint.");
		else
		{

			Player * plr = m_session->GetPlayer();
			//retrieve packed wpid
			uint32 wpid = Arcemu::Util::GUID_HIPART(guid) & 0x000FFFFF;
			//retrieve packed owning creature low guid
			uint32 owner_creature_guid = GET_LOWGUID_PART(guid);
			Creature * owner_creature = plr->GetMapMgr()->GetCreature(owner_creature_guid);
			if(owner_creature != NULL && wpid)
			{
				MobAI * ai = AIType_isMob(owner_creature->GetAIInterface() ) ? TO_AIMOB(owner_creature->GetAIInterface() ) : NULL;
				if(ai != NULL)
				{
					char* pNewFlags = strtok((char*)args, " ");
					uint32 NewFlags = (pNewFlags)? atoi(pNewFlags) : 0;
					WayPoint * wp = ai->Waypoint_getWP(wpid);
					if(wp != NULL)
					{
						uint32 oldflags = wp->flags;
						wp->flags = (uint16)NewFlags;
						SystemMessage(m_session, "Waypoint %u replaced flags %u with %u",wpid, oldflags, NewFlags);
					}
				}
				else
					SystemMessage(m_session, "The selected creature does not support waypoints.");
			}
			else
				SystemMessage(m_session, "Invalid waypoint selected.");
		}
	}
	return true;
}

bool ChatHandler::HandleWPMoveHereCommand(const char* args, WorldSession *m_session)
{
	return true;
	/*
	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid == 0)
	{
		SystemMessage(m_session, "No selection.");
		return true;
	}

	if(GET_TYPE_FROM_GUID(guid) != HIGHGUID_TYPE_WAYPOINT)
	{
		SystemMessage(m_session, "You should select a Waypoint.");
		return true;
	}

	Player* pPlayer = m_session->GetPlayer();
	AIInterface* ai = pPlayer->waypointunit;
	if(!ai || !ai->getUnit())
	{
		SystemMessage(m_session, "Invalid Creature, please select another one.");
		return true;
	}
	std::stringstream ss;

	uint32 wpid = Arcemu::Util::GUID_LOPART( guid );
	if(wpid)
	{
		WayPoint* wp = ai->getWayPoint(wpid);
		if(wp)
		{
			wp->x = pPlayer->GetPositionX();
			wp->y = pPlayer->GetPositionY();
			wp->z = pPlayer->GetPositionZ();

			//save wp
			ai->saveWayPoints();
		}
		//Refresh client
		if(ai->m_WayPointsShowing == true)
		{
			ai->hideWayPoints(pPlayer);
			ai->showWayPoints(pPlayer,ai->m_WayPointsShowBackwards);
		}

		ss << "Waypoint " << wpid << " has been moved.";
		SystemMessage(m_session, ss.str().c_str());
	}
	else
	{
		SystemMessage(m_session, "Invalid Waypoint.");
		return true;
	}
	return true;*/
}

bool ChatHandler::HandleWPWaitCommand(const char* args, WorldSession *m_session)
{
	/*
	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid == 0)
	{
		SystemMessage(m_session, "No selection.");
		return true;
	}

	if(GET_TYPE_FROM_GUID(guid) != HIGHGUID_TYPE_WAYPOINT)
	{
		SystemMessage(m_session, "You should select a Waypoint.");
		return true;
	}

	Player* pPlayer = m_session->GetPlayer();
	AIInterface* ai = pPlayer->waypointunit;
	if(!ai || !ai->getUnit())
	{
		SystemMessage(m_session, "Invalid Creature, please select another one.");
		return true;
	}

	uint32 Wait = 10000;
	std::stringstream ss;

	uint32 wpid = Arcemu::Util::GUID_LOPART( guid );
	if(wpid)
	{
		WayPoint* wp = ai->getWayPoint(wpid);
		if(wp)
		{
			char* pWait = strtok((char*)args, " ");
			Wait = (pWait)? atoi(pWait) : 10000;

			if(Wait < 5000)
			{
				SystemMessage(m_session, "A Wait Time of less then 5000ms can cause lag, consider extending it.");
			}
			wp->waittime = Wait;

			//save wp
			ai->saveWayPoints();
		}

		ss << "Wait Time for Waypoint " << wpid << " is now " << Wait << "ms.";
		SystemMessage(m_session, ss.str().c_str());
	}
	else
	{
		SystemMessage(m_session, "Invalid Waypoint.");
		return true;
	}*/
	return true;
}

bool ChatHandler::HandleWPEmoteCommand(const char* args, WorldSession *m_session)
{/*
	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid == 0)
	{
		SystemMessage(m_session, "No selection.");
		return true;
	}

	if(GET_TYPE_FROM_GUID(guid) != HIGHGUID_TYPE_WAYPOINT)
	{
		SystemMessage(m_session, "You should select a Waypoint.");
		return true;
	}

	Player* pPlayer = m_session->GetPlayer();
	AIInterface* ai = pPlayer->waypointunit;
	if(!ai || !ai->getUnit())
	{
		SystemMessage(m_session, "Invalid Creature, please select another one.");
		return true;
	}
	uint32 EmoteId = 0;
	bool OneShot = true;
	std::stringstream ss;

	uint32 wpid = Arcemu::Util::GUID_LOPART( guid );
	if(wpid)
	{
		WayPoint* wp = ai->getWayPoint(wpid);
		if(wp)
		{
			char* pBackwards = strtok((char*)args, " ");
			uint32 Backwards = (pBackwards)? atoi(pBackwards) : 0;
			char* pEmoteId = strtok(NULL, " ");
			EmoteId = (pEmoteId)? atoi(pEmoteId) : 0;
			char* pOneShot = strtok(NULL, " ");
			OneShot = (pOneShot)? ((atoi(pOneShot)>0)?true:false) : 1;
			if(Backwards)
			{
				wp->backwardemoteid = EmoteId;
				wp->backwardemoteoneshot = OneShot;
			}
			else
			{
				wp->forwardemoteid = EmoteId;
				wp->forwardemoteoneshot = OneShot;
			}

			//save wp
			ai->saveWayPoints();
		}

		ss << "EmoteID for Waypoint " << wpid << " is now " << EmoteId << " and oneshot is " << ((OneShot == true)? "Enabled." : "Disabled.");
		SystemMessage(m_session,  ss.str().c_str());
	}
	else
	{
		SystemMessage(m_session, "Invalid Waypoint.");
		return true;
	}*/
	return true;
}

bool ChatHandler::HandleWPSkinCommand(const char* args, WorldSession *m_session)
{
	/*uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid == 0)
	{
		SystemMessage(m_session,  "No selection.");
		return true;
	}

	if(GET_TYPE_FROM_GUID(guid) != HIGHGUID_TYPE_WAYPOINT)
	{
		SystemMessage(m_session,  "You should select a Waypoint.");
		return true;
	}

	Player* pPlayer = m_session->GetPlayer();
	AIInterface* ai = pPlayer->waypointunit;
	if(!ai || !ai->getUnit())
	{
		SystemMessage(m_session, "Invalid Creature, please select another one.");
		return true;
	}
	uint32 SkinId = 0;
	std::stringstream ss;

	uint32 wpid = Arcemu::Util::GUID_LOPART( guid );
	if(wpid)
	{
		WayPoint* wp = ai->getWayPoint(wpid);
		if(wp)
		{
			char* pBackwards = strtok((char*)args, " ");
			uint32 Backwards = (pBackwards)? atoi(pBackwards) : 0;
			char* pSkinId = strtok(NULL, " ");
			SkinId = (pSkinId)? atoi(pSkinId) : 0;
			if(Backwards)
			{
				wp->backwardskinid = SkinId;
			}
			else
			{
				wp->forwardskinid = SkinId;
			}

			//save wp
			ai->saveWayPoints();
		}

		ss << "SkinID for Waypoint " << wpid << " is now " << SkinId;
		SystemMessage(m_session, ss.str().c_str());
	}
	else
	{
		SystemMessage(m_session, "Invalid Waypoint.");
		return true;
	}*/
	return true;
}

bool ChatHandler::HandleWPInfoCommand(const char* args, WorldSession *m_session)
{
	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid == 0)
		SystemMessage(m_session, "No selection.");
	else

	{
		if(GET_TYPE_FROM_GUID(guid) != HIGHGUID_TYPE_WAYPOINT)
			SystemMessage(m_session, "You should select a Waypoint.");
		else
		{
			Player * plr = m_session->GetPlayer();
			uint32 wpid = Arcemu::Util::GUID_HIPART( guid ) & 0x000FFFFF;
			uint32 owner_creature_guid = GET_LOWGUID_PART(guid);
			Creature * owner_creature = plr->GetMapMgr()->GetCreature(owner_creature_guid);
			if(wpid > 0 && owner_creature != NULL)
			{
				MobAI * ai = AIType_isMob(owner_creature->GetAIInterface() ) ? TO_AIMOB(owner_creature->GetAIInterface() ) : NULL;
				if(ai != NULL)
				{
					WayPoint * wp = ai->Waypoint_getWP(wpid);
					if(wp != NULL)
					{
						std::stringstream ss;
						ss << "Waypoint Id " << wp->id << ":\n";
						ss << "WaitTime: " << wp->waittime << "\n";
						ss << "Flags: " << wp->flags;
						if(wp->flags == 768)
							ss << " (Fly)\n";
						else if(wp->flags == 256)
							ss << " (Run)\n";
						else
							ss << " (Walk)\n";
						ss << "Backwards[ emoteid:" << wp->backwardemoteid << " skinid :" << wp->backwardskinid << " oneshot : " << ((wp->backwardemoteoneshot == 1)? "Yes" : "No") << "]\n";
						ss << "Forwards[ emoteid:" << wp->forwardemoteid << " skinid :" << wp->forwardskinid << " oneshot : " << ((wp->forwardemoteoneshot == 1)? "Yes" : "No") << "]\n";
						SendMultilineMessage(m_session, ss.str().c_str());
					}
				}
				else
					SystemMessage(m_session, "The selected creature does not support waypoints.");
			}
			else
				SystemMessage(m_session, "Invalid waypoint selected.");
		}
	}
	return true;
}

bool ChatHandler::HandleWPHideCommand(const char* args, WorldSession *m_session)
{
	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid == 0)
		SystemMessage(m_session, "No selection.");
	else
	{
		if(GET_TYPE_FROM_GUID(guid) == HIGHGUID_TYPE_WAYPOINT || GET_TYPE_FROM_GUID(guid) == HIGHGUID_TYPE_UNIT )
		{
			Creature * pCreature = m_session->GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
			if(pCreature == NULL)
				SystemMessage(m_session, "You should select a Creature/Waypoint.");
			else
			{
				if(AIType_isMob(pCreature->GetAIInterface() ) )
				{
					Player* plr = m_session->GetPlayer();
					if(GET_TYPE_FROM_GUID(guid) == HIGHGUID_TYPE_WAYPOINT)
					{
						uint32 wp_id = Arcemu::Util::GUID_HIPART(guid) & 0x000FFFFF;
						TO_AIMOB(pCreature->GetAIInterface() )->Waypoint_hide(plr, wp_id);
						SystemMessage(m_session, "Hiding waypoint id %u",wp_id);
					}
					else if(TO_AIMOB(pCreature->GetAIInterface() )->Waypoint_isShowing() )
					{
						TO_AIMOB(pCreature->GetAIInterface() )->Waypoint_hide(plr);
						SystemMessage(m_session, "Hiding waypoints for Creature %u (%s) ",pCreature->GetEntry(), pCreature->GetCreatureInfo()->Name);
					}
					else
						SystemMessage(m_session, "Waypoints for this creature are already hidden.");
				}
				else
					SystemMessage(m_session, "The selected creature does not support waypoints.");
			}
		}
	}
	return true;
}

bool ChatHandler::HandleGenerateWaypoints(const char* args, WorldSession * m_session)
{
	/*
	Creature * cr = 
		m_session->GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(m_session->GetPlayer()->GetSelection()));
	if(!cr)
	{
		SystemMessage(m_session, "You should select a creature.");
		return true;
	}
	if(cr->GetAIInterface()->GetWayPointsCount())//ALREADY HAVE WAYPOINTS
	{	
		SystemMessage(m_session, "The creature already has waypoints");
		return false;
	}
	if(m_session->GetPlayer()->waypointunit != NULL)
	{
		SystemMessage(m_session, "You are already showing waypoints, hide them first.");
		return true;
	}

	if(!cr->GetSQL_id())
		return false;
	char* pR = strtok((char*)args, " ");
	if(!pR)
	{
		SystemMessage(m_session, "Randomly generate wps params: range count");
		return true;
	}
	int r = atoi(pR);
	char *pC=strtok(NULL, " ");
	if(!pC)
	{
		SystemMessage(m_session, "Randomly generate wps params: range count");
		return true;
	}
	int n = atol(pC);

	for(int i= 0;i<n;i++)
	{
		float ang = rand()/100.0f;
		float ran = (rand()%(r*10))/10.0f;
		while(ran<1)
		{
			ran = (rand()%(r*10))/10.0f;
		}

		float x = cr->GetPositionX()+ran*sin(ang);
		float y = cr->GetPositionY()+ran*cos(ang);
		float z = cr->GetMapMgr()->GetBaseMap()->GetLandHeight(x,y);
		 
		WayPoint* wp = new WayPoint;
		wp->id = (uint32)cr->GetAIInterface()->GetWayPointsCount()+1;
		wp->x = x;
		wp->y = y;
		wp->z = z;
		wp->waittime = 5000;
		wp->flags = 0;
		wp->forwardemoteoneshot = 0;
		wp->forwardemoteid = 0;
		wp->backwardemoteoneshot = 0;
		wp->backwardemoteid = 0;
		wp->forwardskinid = 0;
		wp->backwardskinid = 0;
		
		cr->GetAIInterface()->addWayPoint(wp);
	}

	if(cr->m_spawn && cr->m_spawn->movetype != 1)		/* move random 
	{
		cr->m_spawn->movetype = 1;
		cr->GetAIInterface()->m_moveType = 1;
		WorldDatabase.Execute("UPDATE creature_spawns SET movetype = 1 WHERE id = %u", cr->GetSQL_id());
	}
	m_session->GetPlayer()->waypointunit = cr->GetAIInterface();
	cr->GetAIInterface()->showWayPoints(m_session->GetPlayer(),cr->GetAIInterface()->m_WayPointsShowBackwards);
	*/
	return true;
}

bool ChatHandler::HandleSaveWaypoints(const char* args, WorldSession * m_session)
{
	Creature * cr = m_session->GetPlayer()->GetMapMgr()->GetCreature( GET_LOWGUID_PART( m_session->GetPlayer()->GetSelection() ) );
	if ( cr == NULL || !cr->GetSQL_id() )
		return false;
	if(AIType_isMob(cr->GetAIInterface() ) )
		TO_AIMOB(cr->GetAIInterface() )->Waypoint_saveall();
	return true;
}


bool ChatHandler::HandleDeleteWaypoints(const char* args, WorldSession * m_session)
{
	Creature * cr = m_session->GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(m_session->GetPlayer()->GetSelection()));
	if(cr != NULL && cr->GetSQL_id() > 0 && AIType_isMob(cr->GetAIInterface() ) )
	{
		bool fromDB = (args != NULL);
		MobAI * ai = TO_AIMOB(cr->GetAIInterface() );
		if(fromDB)
			WorldDatabase.Execute("DELETE FROM creature_waypoints WHERE spawnid=%u",cr->GetSQL_id());
		//delete them
		ai->Waypoint_deleteall();
		//delete them visually
		if(ai->Waypoint_isShowing() )
			ai->Waypoint_hide(m_session->GetPlayer() );
		SystemMessage(m_session, "Deleted waypoints for Creature: %s entry: %u", cr->GetCreatureInfo()->Name, cr->GetSQL_id() );
	}
	else
		SystemMessage(m_session, "Invalid creature selected or that creature does not support waypoints.");

	return true;
}

bool ChatHandler::HandleWaypointAddFlyCommand(const char * args, WorldSession * m_session)
{
	/*uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid == 0)
	{
		SystemMessage(m_session, "No Selection");
		return true;
	}
	AIInterface* ai = NULL;
	Creature * pCreature = NULL;
	Player* p = m_session->GetPlayer();
	if(p->waypointunit != NULL)
	{
		SystemMessage(m_session, "Using Previous Unit.");
		ai = p->waypointunit;
		if(!ai)
		{
			SystemMessage(m_session, "Invalid Creature, please select another one.");
			return true;
		}

		pCreature = (Creature*)ai->getUnit();
		if ( !pCreature || pCreature->IsPet() )
		{
			SystemMessage(m_session, "Invalid Creature, please select another one.");
			return true;
		}
	}
	else
	{
		pCreature = m_session->GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
		if ( !pCreature || pCreature->IsPet() )
		{
			SystemMessage(m_session, "You should select a creature.");
			return true;
		}
		ai = pCreature->GetAIInterface();
	}

	char* pWaitTime = strtok((char*)args, " ");
	uint32 WaitTime = (pWaitTime)? atoi(pWaitTime) : 10000;
	char* pForwardEmoteId = strtok(NULL, " ");
	uint32 ForwardEmoteId = (pForwardEmoteId)? atoi(pForwardEmoteId) : 0;
	char* pBackwardEmoteId = strtok(NULL, " ");
	uint32 BackwardEmoteId = (pBackwardEmoteId)? atoi(pBackwardEmoteId) : 0;
	char* pForwardSkinId = strtok(NULL, " ");
	uint32 ForwardSkinId = (pForwardSkinId)? atoi(pForwardSkinId) : pCreature->GetNativeDisplayId();
	char* pBackwardSkinId = strtok(NULL, " ");
	uint32 BackwardSkinId = (pBackwardSkinId)? atoi(pBackwardSkinId) : pCreature->GetNativeDisplayId();
	char* pForwardEmoteOneShot = strtok(NULL, " ");
	uint32 ForwardEmoteOneShot = (pForwardEmoteOneShot)? atoi(pForwardEmoteOneShot) : 1;
	char* pBackwardEmoteOneShot = strtok(NULL, " ");
	uint32 BackwardEmoteOneShot = (pBackwardEmoteOneShot)? atoi(pBackwardEmoteOneShot) : 1;

	WayPoint* wp = new WayPoint;
	bool showing = ai->m_WayPointsShowing;
	wp->id = (uint32)ai->GetWayPointsCount()+1;
	wp->x = p->GetPositionX();
	wp->y = p->GetPositionY();
	wp->z = p->GetPositionZ();
	wp->waittime = WaitTime;
	wp->flags = 768;
	wp->forwardemoteoneshot = (ForwardEmoteOneShot>0)?true:false;
	wp->forwardemoteid = ForwardEmoteId;
	wp->backwardemoteoneshot = (BackwardEmoteOneShot>0)?true:false;
	wp->backwardemoteid = BackwardEmoteId;
	wp->forwardskinid = ForwardSkinId;
	wp->backwardskinid = BackwardSkinId;

	if(showing)
		ai->hideWayPoints(p);

	ai->addWayPoint(wp);
	ai->saveWayPoints();

	if(showing)
		ai->showWayPoints(p,ai->m_WayPointsShowBackwards);

	SystemMessage(m_session, "Waypoint %u added.", wp->id);*/
	return true;
}

bool ChatHandler::HandleNpcSelectCommand(const char * args, WorldSession * m_session)
{
	Creature * un = NULL;
	float dist = 999999.0f;
	float dist2;
	Player * plr = m_session->GetPlayer();
	set<Object*>::iterator itr;
	for(itr = plr->GetInRangeSetBegin(); itr != plr->GetInRangeSetEnd(); ++itr)
	{
		if( (dist2 = plr->GetDistance2dSq(*itr)) < dist && (*itr)->GetTypeId() == TYPEID_UNIT )
		{
			un = TO_CREATURE(*itr);
			dist = dist2;
		}
	}

	if(!un)
	{
		SystemMessage(m_session, "No inrange creatures found.");
		return true;
	}

	plr->SetSelection(un->GetGUID());
	SystemMessage(m_session, "Set selection to "I64FMT" (%s)", un->GetGUID(), un->GetCreatureInfo()->Name);
	return true;
}
