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

initialiseSingleton( TaxiMgr );

/************************
 *	   TaxiPath	   *
 ************************/

void TaxiPath::ComputeLen()
{
	m_length1 = m_length1 = 0;
	m_map1 = m_map2 = 0;
	float * curptr = &m_length1;

	if (!m_pathNodes.size())
		return;

	std::map<uint32, TaxiPathNode*>::iterator itr;
	itr = m_pathNodes.begin();

	float x = itr->second->x;
	float y = itr->second->y;
	float z = itr->second->z;
	uint32 curmap = itr->second->mapid;
	m_map1 = curmap;

	itr++;

	while (itr != m_pathNodes.end())
	{
		if( itr->second->mapid != curmap )
		{
			curptr = &m_length2;
			m_map2 = itr->second->mapid;
			curmap = itr->second->mapid;
		}

		*curptr += sqrt((itr->second->x - x)*(itr->second->x - x) +
			(itr->second->y - y)*(itr->second->y - y) + 
			(itr->second->z - z)*(itr->second->z - z));

		x = itr->second->x;
		y = itr->second->y;
		z = itr->second->z;
		itr++;
	}
}

void TaxiPath::SetPosForTime(float &x, float &y, float &z, uint32 time, uint32 *last_node, uint32 mapid)
{
	if (!time)
		return;

	float length;
	if( mapid == m_map1 )
		length = m_length1;
	else
		length = m_length2;

	float traveled_len = (time/(length * TAXI_TRAVEL_SPEED))*length;
	uint32 len = 0;

	x = 0;
	y = 0;
	z = 0;

	if (!m_pathNodes.size())
		return;

	std::map<uint32, TaxiPathNode*>::iterator itr;
	itr = m_pathNodes.begin();

	float nx,ny,nz = 0.0f;
	bool set = false;
	uint32 nodecounter = 0;

	while (itr != m_pathNodes.end())
	{
		if( itr->second->mapid != mapid )
		{
			itr++;
			nodecounter++;
			continue;
		}

		if(!set)
		{
			nx = itr->second->x;
			ny = itr->second->y;
			nz = itr->second->z;
			set = true;
			continue;
		}

		len = (uint32)sqrt((itr->second->x - nx)*(itr->second->x - nx) +
			(itr->second->y - ny)*(itr->second->y - ny) + 
			(itr->second->z - nz)*(itr->second->z - nz));

		if (len >= traveled_len)
		{
			x = (itr->second->x - nx)*(traveled_len/len) + nx;
			y = (itr->second->y - ny)*(traveled_len/len) + ny;
			z = (itr->second->z - nz)*(traveled_len/len) + nz;
			*last_node = nodecounter;
			return;
		}
		else
		{
			traveled_len -= len;
		}

		nx = itr->second->x;
		ny = itr->second->y;
		nz = itr->second->z;
		itr++;
		nodecounter++;
	}

	x = nx;
	y = ny;
	z = nz;
}

TaxiPathNode* TaxiPath::GetPathNode(uint32 i)
{
	if (m_pathNodes.find(i) == m_pathNodes.end())
		return NULL;
	else
		return m_pathNodes.find(i)->second;
}

void TaxiPath::SendMoveForTime(Player *riding, Player *to, uint32 time)
{
	if (!time)
		return;

	float length;
	uint32 mapid = riding->GetMapId();
	if( mapid == m_map1 )
		length = m_length1;
	else
		length = m_length2;

	float traveled_len = (time/(length * TAXI_TRAVEL_SPEED))*length;
	uint32 len = 0;
	float x = 0,y = 0,z = 0;

	if (!m_pathNodes.size())
		return;

	std::map<uint32, TaxiPathNode*>::iterator itr;
	itr = m_pathNodes.begin();

	float nx,ny,nz = 0.0f;
	bool set = false;
	uint32 nodecounter = 1;

	while (itr != m_pathNodes.end())
	{
		if( itr->second->mapid != mapid )
		{
			itr++;
			nodecounter++;
			continue;
		}

		if(!set)
		{
			nx = itr->second->x;
			ny = itr->second->y;
			nz = itr->second->z;
			set = true;
			continue;
		}

		len = (uint32)sqrt((itr->second->x - nx)*(itr->second->x - nx) +
			(itr->second->y - ny)*(itr->second->y - ny) + 
			(itr->second->z - nz)*(itr->second->z - nz));

		if (len >= traveled_len)
		{
			x = (itr->second->x - nx)*(traveled_len/len) + nx;
			y = (itr->second->y - ny)*(traveled_len/len) + ny;
			z = (itr->second->z - nz)*(traveled_len/len) + nz;
			break;
		}
		else
		{
			traveled_len -= len;
		}

		nx = itr->second->x;
		ny = itr->second->y;
		nz = itr->second->z;
		itr++;
	}

	if (itr == m_pathNodes.end())
		return;

	WorldPacket * data = new WorldPacket(SMSG_MONSTER_MOVE, 2000);
	size_t pos;

	*data << riding->GetNewGUID();
	*data << riding->GetPositionX( ) << riding->GetPositionY( ) << riding->GetPositionZ( );
	*data << getMSTime();
	*data << uint8( 0 );
	*data << uint32( 0x00000300 );
	*data << uint32( uint32((length * TAXI_TRAVEL_SPEED) - time));
	*data << uint32( nodecounter );
	pos = data->wpos();
	*data << nx << ny << nz;

	while (itr != m_pathNodes.end())
	{
		TaxiPathNode *pn = itr->second;
		if( pn->mapid != mapid )
			break;

		*data << pn->x << pn->y << pn->z;
		++itr;
		++nodecounter;
	}
	
	*(uint32*)&(data->contents()[pos]) = nodecounter;
	to->delayedPackets.add(data);
/*	if (!time)
		return;

	float traveled_len = (time/(getLength() * TAXI_TRAVEL_SPEED))*getLength();;
	uint32 len = 0, count = 0;
	float x = 0,y = 0,z = 0;

	if (!m_pathNodes.size())
		return;

	std::map<uint32, TaxiPathNode*>::iterator itr;
	itr = m_pathNodes.begin();

	float nx = itr->second->x;
	float ny = itr->second->y;
	float nz = itr->second->z; 
	itr++;

	while (itr != m_pathNodes.end())
	{		
		len = (uint32)sqrt((itr->second->x - nx)*(itr->second->x - nx) +
			(itr->second->y - ny)*(itr->second->y - ny) + 
			(itr->second->z - nz)*(itr->second->z - nz));

		if (len > traveled_len)
		{
			x = (itr->second->x - nx)*(traveled_len/len) + nx;
			y = (itr->second->y - ny)*(traveled_len/len) + ny;
			z = (itr->second->z - nz)*(traveled_len/len) + nz;
			break;
		}
		else
		{
			traveled_len -= len;
		}

		nx = itr->second->x;
		ny = itr->second->y;
		nz = itr->second->z;
		itr++;
		count++;
	}

	if (itr == m_pathNodes.end())
		return;

	WorldPacket * data = new WorldPacket(SMSG_MONSTER_MOVE, 2000);

	*data << riding->GetNewGUID();
	*data << riding->GetPositionX( ) << riding->GetPositionY( ) << riding->GetPositionZ( );
	*data << getMSTime();
	*data << uint8( 0 );
	*data << uint32( 0x00000300 );
	*data << uint32( uint32((getLength() * TAXI_TRAVEL_SPEED) - time));
	*data << uint32( GetNodeCount() - count );
	*data << nx << ny << nz;
	while (itr != m_pathNodes.end())
	{
		TaxiPathNode *pn = itr->second;
		*data << pn->x << pn->y << pn->z;
		itr++;
	}
	//to->GetSession()->SendPacket(&data);
	to->delayedPackets.add(data);*/
}

/***********************
 *	   TaxiMgr	   *
 ***********************/

void TaxiMgr::_LoadTaxiNodes()
{
	uint32 i;

	for(i = 0; i < dbcTaxiNode.GetNumRows(); i++)
	{
		DBCTaxiNode *node = dbcTaxiNode.LookupRow(i);
		if (node)
		{
			TaxiNode *n = new TaxiNode;
			n->id = node->id;
			n->mapid = node->mapid;
			n->alliance_mount = node->alliance_mount;
			n->horde_mount = node->horde_mount;
			n->x = node->x;
			n->y = node->y;
			n->z = node->z;

			this->m_taxiNodes.insert(std::map<uint32, TaxiNode*>::value_type(n->id, n));
		}
	}

	//todo: load mounts
}

void TaxiMgr::_LoadTaxiPaths()
{
	uint32 i, j;

	for(i = 0; i < dbcTaxiPath.GetNumRows(); i++)
	{
		DBCTaxiPath *path = dbcTaxiPath.LookupRow(i);

		if (path)
		{
			TaxiPath *p = new TaxiPath;
			p->from = path->from;
			p->to = path->to;
			p->id = path->id;
			p->price = path->price;

			//Load Nodes
			for(j = 0; j < dbcTaxiPathNode.GetNumRows(); j++)
			{
				DBCTaxiPathNode *pathnode = dbcTaxiPathNode.LookupRow(j);

				if (pathnode)
				{
					if (pathnode->path == p->id)
					{
						TaxiPathNode *pn = new TaxiPathNode;
						pn->x = pathnode->x;
						pn->y = pathnode->y;
						pn->z = pathnode->z;
						pn->mapid = pathnode->mapid;
						p->AddPathNode(pathnode->seq, pn);
					}
				}
			}

			p->ComputeLen();
			this->m_taxiPaths.insert(std::map<uint32, TaxiPath*>::value_type(p->id, p));
		}
	}
}

TaxiPath* TaxiMgr::GetTaxiPath(uint32 path)
{
	HM_NAMESPACE::hash_map<uint32, TaxiPath*>::iterator itr;

	itr = this->m_taxiPaths.find(path);

	if (itr == m_taxiPaths.end())
		return NULL;
	else
		return itr->second;
}

TaxiPath* TaxiMgr::GetTaxiPath(uint32 from, uint32 to)
{
	HM_NAMESPACE::hash_map<uint32, TaxiPath*>::iterator itr;

	for (itr = m_taxiPaths.begin(); itr != m_taxiPaths.end(); itr++)
		if ((itr->second->to == to) && (itr->second->from == from))
			return itr->second;

	return NULL;
}

TaxiNode* TaxiMgr::GetTaxiNode(uint32 node)
{
	HM_NAMESPACE::hash_map<uint32, TaxiNode*>::iterator itr;

	itr = this->m_taxiNodes.find(node);

	if (itr == m_taxiNodes.end())
		return NULL;
	else
		return itr->second;
}

uint32 TaxiMgr::GetNearestTaxiNode( float x, float y, float z, uint32 mapid )
{
	uint32 nearest = 0;
	float distance = -1;
	float nx, ny, nz, nd;

	HM_NAMESPACE::hash_map<uint32, TaxiNode*>::iterator itr;

	for (itr = m_taxiNodes.begin(); itr != m_taxiNodes.end(); itr++)
	{
		if (itr->second->mapid == mapid)
		{
			nx = itr->second->x - x;
			ny = itr->second->y - y;
			nz = itr->second->z - z;
			nd = nx * nx + ny * ny + nz * nz;
			if( nd < distance || distance < 0 )
			{
				distance = nd;
				nearest = itr->second->id;
			}
		}
	}

	return nearest;
}

bool TaxiMgr::GetGlobalTaxiNodeMask( uint32 curloc, uint32 *Mask )
{
	HM_NAMESPACE::hash_map<uint32, TaxiPath*>::iterator itr;
	uint8 field;

	for (itr = m_taxiPaths.begin(); itr != m_taxiPaths.end(); itr++)
	{
		/*if( itr->second->from == curloc )
		{*/
			field = (uint8)((itr->second->to - 1) / 32);
			Mask[field] |= 1 << ( (itr->second->to - 1 ) % 32 );
		//}
	}

	return true;
}
