#ifndef _ITEM_BUFFER_POOL
#define _ITEM_BUFFER_POOL

//hmm with 2000 players each have 50 items ...Decrease it if you need the space ;)
//@Zack 2000 x 50 = 10000? :P
#define INITI_POOL_WITH_SIZE 10000
#define INITI_CONTAINER_POOL_WITH_SIZE 5000
//too big values migh create lag spikes on buffer limit extension !
#define EXTEND_POOL_WITH_SIZE 100
#define POOL_IS_FULL_INDEX 0

//noob protection :P
#ifdef _DEBUG
//	#define TRACK_LEAKED_OBJECTS_AND_MEMORY_CORRUPTION
#endif

class Item;
class Container;
class Aura;
class Spell;

/* Maybe we should use template< class T, int InitPoolSize > but for now
all Pools use the same val so we don't need */

template< class T > class SERVER_DECL tPPoolClass : public Singleton< tPPoolClass< T > >
{
private:
	typedef tPPoolClass< T > tBaseType;

	T**	avail_list;
	uint32 max_avails;
	Mutex ObjLock;

	std::list<uint32> avail_indexes;

public:
	tPPoolClass()
		//lol i get compile error here. Is it my old VS 2k3 ?
		: Singleton()
	{
		max_avails = INITI_POOL_WITH_SIZE;

		avail_list = (T **)malloc( sizeof(T*) * max_avails );

		if( !avail_list )
			Log.Error( "PointerPool", "Pool failed to alocate more memory = %u bytes", sizeof(T*) * max_avails );

		InitPoolNewSection( 0, max_avails );
	}

	~tPPoolClass()
	{
		//? DestroyPool? :P
	};

	//fetch from internal list a new Item object
	T* PooledNew()
	{
		ObjLock.Acquire();

		//out of buffer ? get new ones !
		if( !avail_indexes.size() )
			ExtedLimitAvailLimit();

		//Get a Element from the availpool
		uint32 free_index = (*avail_indexes.begin());
		avail_indexes.pop_front();
		avail_list[ free_index ]->Virtual_Constructor();
		avail_list[ free_index ]->m_bufferPoolId = free_index;

		ObjLock.Release();

		return avail_list[ free_index ];
	}
	//insert into free object list
	void PooledDelete(T* dumped)
	{
		//non pooled object requested or invalid
		if( dumped->m_bufferPoolId < 0 || dumped->m_bufferPoolId >= (int32)max_avails ) {
			delete dumped;
			return;
		}

		//get our PoolId
		uint32 myPoolId = (uint32)dumped->m_bufferPoolId;

		//remove events and remove object from world ...
		dumped->Virtual_Destructor();

		ObjLock.Acquire();

		//We do not care about used up guids only available ones. Note that with this overwrite used guid list is not valid anymore
		avail_list[ myPoolId ] = dumped;
		avail_indexes.push_back( myPoolId );

		ObjLock.Release();
	}
	//since some objects may depend on us and we might depend on other objects sometimes it is better to know when we destroy the pool instead of the destructor
	void DestroyPool()
	{
		for( uint32 i = 0; i < max_avails; i++ )
			delete avail_list[i];

		free( avail_list );
		avail_list = NULL;
		avail_indexes.clear();
	}
private:
	void InitPoolNewSection(uint32 from, uint32 to)
	{
		for( uint32 i = from; i < to;i++ )
		{
			avail_list[i] = new T;
			ASSERT( avail_list[i] );

			//all avail on creating new
			avail_indexes.push_back( i );
		}
	}

	void ExtedLimitAvailLimit()
	{
		uint32 prev_max = max_avails;
		max_avails += EXTEND_POOL_WITH_SIZE;
		avail_list = (T**)realloc( avail_list, sizeof(T*) * max_avails );
		
		if( !avail_list )
			Log.Error( "PointerPool", "Pool failed to alocate more memory = %u bytes", sizeof(T*) * max_avails );

		InitPoolNewSection( prev_max, max_avails );
	}
};

/* Grey: ContainerPooling have to come with a sepperate Patch */

//#define ContainerPool tPPoolClass<Container>::getSingleton()
#define ItemPool tPPoolClass<Item>::getSingleton()
#define AuraPool tPPoolClass<Aura>::getSingleton()
#define SpellPool tPPoolClass<Spell>::getSingleton()

#endif

