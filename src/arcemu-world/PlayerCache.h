#ifndef __PLAYERCACHE_H
#define __PLAYERCACHE_H

enum FourByteFields
{
	CACHE_MAPID,
	CACHE_INSTANCEID,
	CACHE_PLAYER_LOWGUID,
	CACHE_PLAYER_FLAGS,
	CACHE_PLAYER_INITIALTEAM,
	NUM_FOURBYTE_CACHE_FIELDS
};

enum StringFields
{
	CACHE_PLAYER_NAME,
	CACHE_AFK_DND_REASON,
	NUM_STRING_CACHE_FIELDS,
};

enum Set64Fields
{
	CACHE_SOCIAL_FRIENDLIST,
	CACHE_SOCIAL_IGNORELIST,
	CACHE_GM_TARGETS,
	NUM_SET64_CACHE_FIELDS
};

union CacheField
{
	uint32 u;
	float f;
	int32 i;
};

typedef std::set<uint64> PlayerCacheSet;

class Player;
class PlayerCache
{
public:
	PlayerCache()
	{
		for (uint32 i = 0; i < NUM_FOURBYTE_CACHE_FIELDS; ++i)
			m_fields[i].u = 0;
		_refs = 1;

		//default values
		SetInt32Value(CACHE_MAPID, -1);
		SetInt32Value(CACHE_INSTANCEID, -1);
	}

	~PlayerCache()
	{
		WorldPacket* pending_packets = m_pendingPackets.pop();
		while (pending_packets != NULL)
		{
			delete pending_packets;
			pending_packets = m_pendingPackets.pop();
		}
	}

	//String cache
	FastMutex m_stringlock; //Protects strings during realloc possibilities
	std::string m_stringfields[NUM_STRING_CACHE_FIELDS];

	//Four byte cache (ints, floats)
	CacheField m_fields[NUM_FOURBYTE_CACHE_FIELDS];

	//Set uint64 cache (valid gm talk targets, ignore lists, friend lists)
	FastMutex m_set64lock; //Protects dick
	PlayerCacheSet m_set64fields[NUM_SET64_CACHE_FIELDS];

	FQueue<WorldPacket*> m_pendingPackets; //used for sending packets to another context

	void SetStringValue(uint32 field, std::string & val) { m_stringlock.Acquire(); m_stringfields[field] = val; m_stringlock.Release(); }
	void SetStringValue(uint32 field, const char* val) { m_stringlock.Acquire(); m_stringfields[field] = val; m_stringlock.Release(); }
	std::string GetStringValue(uint32 field) { m_stringlock.Acquire(); std::string ret = m_stringfields[field]; m_stringlock.Release(); return ret; }
	void GetStringValue(uint32 field, std::string & val) { m_stringlock.Acquire(); val = m_stringfields[field]; m_stringlock.Release(); }
	void SetUInt32Value(uint32 field, uint32 val) { m_fields[field].u = val; }
	void SetInt32Value(uint32 field, int32 val) { m_fields[field].i = val; }
	void SetFloatValue(uint32 field, float val) { m_fields[field].f = val; }
	uint32 GetUInt32Value(uint32 field) { return m_fields[field].u; }
	int32 GetInt32Value(uint32 field) { return m_fields[field].i; }
	float GetFloatValue(uint32 field) { return m_fields[field].f; }
	uint32 HasFlag(uint32 field, uint32 flag) { return m_fields[field].u & flag; }

	//Set64
	void Insert64Value(uint32 field, uint64 value) { m_set64lock.Acquire(); m_set64fields[field].insert(value); m_set64lock.Release(); }
	void Remove64Vaue(uint32 field, uint64 value) { m_set64lock.Acquire(); m_set64fields[field].erase(value); m_set64lock.Release(); }
	size_t Count64Value(uint32 field, uint64 value) { m_set64lock.Acquire(); size_t ret = m_set64fields[field].count(value); m_set64lock.Release(); return ret; }
	size_t GetSet64Size(uint32 field) { m_set64lock.Acquire(); size_t ret = m_set64fields[field].size(); m_set64lock.Release(); return ret; }
	//Set64 locks
	//These functions request the field you're going to use, so we can turn them into an array of mutexes if needed. Scalability testing needs done first :P
	void AcquireSetLock(uint32 field) { m_set64lock.Acquire(); }
	void ReleaseSetLock(uint32 field) { m_set64lock.Release(); }
	//Set64 iterators, you must have the lock before using these!
	PlayerCacheSet::iterator Begin64(uint32 field) { return m_set64fields[field].begin(); }
	PlayerCacheSet::iterator End64(uint32 field) { return m_set64fields[field].end(); }

	//Queues a packet to send to the player. The packet must be created using the new operator, and SendPacket takes ownership of the packet.
	void SendPacket(WorldPacket* p)
	{
		m_pendingPackets.push(p);
	}

	//Queues a packet to send to the player.
	void SendPacket(WorldPacket & p)
	{
		WorldPacket* data = new WorldPacket(p);
		m_pendingPackets.push(data);
	}

	//BEGINNING OF WRAPPERS
	uint64 GetGUID() { return (uint64(HIGHGUID_TYPE_PLAYER) << 32) | GetUInt32Value(CACHE_PLAYER_LOWGUID); }

	//END OF WRAPPERS

	//ref counters (this lets us exist in multiple threads without holding locks)
	volatile long _refs;
	void AddRef() { Sync_Add(&_refs); }
	void DecRef() { if (Sync_Sub(&_refs) == 0) delete this; }
};

#endif
