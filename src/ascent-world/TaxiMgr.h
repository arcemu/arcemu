/*
 * ArcEmu Failure Server
 * Copyright (C) 2008 <http://www.AscentEmu.com/>
 *
 * This program is stolen software. Please visit www.ascentemu.com
 * for the real owners of this emulator.
 * 
 * ArcEmu is a pathetic piece of software which crashes even more
 * than vanilla Ascent. And don't even put it up against Summit.
 * There's no comparison there.
 *
 * SO, why use a project run by an idiot who can't even code?
 * You don't! :D
 *
 */

#ifndef __TAXIMGR_H
#define __TAXIMGR_H

#define TAXI_TRAVEL_SPEED 32

class Player;

struct TaxiNode {
	uint32 id;
	float x,y,z;
	uint32 mapid;
	uint32 horde_mount;
	uint32 alliance_mount;
};

struct TaxiPathNode 
{
	float x,y,z;
	uint32 mapid;
};

class TaxiPath {
	friend class TaxiMgr;

public:
	TaxiPath() 
	{ 
	}

	~TaxiPath() 
	{
		while(m_pathNodes.size())
		{
			TaxiPathNode *pn = m_pathNodes.begin()->second;
			m_pathNodes.erase(m_pathNodes.begin());
			delete pn;
		}
	}

	void ComputeLen();
	void SetPosForTime(float &x, float &y, float &z, uint32 time, uint32* lastnode, uint32 mapid);
	ASCENT_INLINE uint32 GetID() { return id; }
	void SendMoveForTime(Player *riding, Player *to, uint32 time);
	void AddPathNode(uint32 index, TaxiPathNode* pn) { m_pathNodes[index] = pn; }
	ASCENT_INLINE size_t GetNodeCount() { return m_pathNodes.size(); }
	TaxiPathNode* GetPathNode(uint32 i);
	ASCENT_INLINE uint32 GetPrice() { return price; }
	ASCENT_INLINE uint32 GetSourceNode() { return from; }

protected:

	std::map<uint32, TaxiPathNode*> m_pathNodes;

	float m_length1;
	uint32 m_map1;

	float m_length2;
	uint32 m_map2;
	uint32 id, to, from, price;
};


class SERVER_DECL TaxiMgr :  public Singleton < TaxiMgr >
{
public:
	TaxiMgr() 
	{
		_LoadTaxiNodes();
		_LoadTaxiPaths();
	}

	~TaxiMgr() 
	{ 
		while(m_taxiPaths.size())
		{
			TaxiPath *p = m_taxiPaths.begin()->second;
			m_taxiPaths.erase(m_taxiPaths.begin());
			delete p;
		}
		while(m_taxiNodes.size())
		{
			TaxiNode *n = m_taxiNodes.begin()->second;
			m_taxiNodes.erase(m_taxiNodes.begin());
			delete n;
		}
	}

	TaxiPath *GetTaxiPath(uint32 path);
	TaxiPath *GetTaxiPath(uint32 from, uint32 to);
	TaxiNode *GetTaxiNode(uint32 node);

	uint32 GetNearestTaxiNode( float x, float y, float z, uint32 mapid );
	bool GetGlobalTaxiNodeMask( uint32 curloc, uint32 *Mask );


private:
	void _LoadTaxiNodes();
	void _LoadTaxiPaths();

	HM_NAMESPACE::hash_map<uint32, TaxiNode*> m_taxiNodes;
	HM_NAMESPACE::hash_map<uint32, TaxiPath*> m_taxiPaths;
};

#define sTaxiMgr TaxiMgr::getSingleton()

#endif
