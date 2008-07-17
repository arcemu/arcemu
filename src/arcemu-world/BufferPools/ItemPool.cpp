#include "StdAfx.h"
#include "ItemPool.h"

//put some objects in pool
oItemBufferPool::oItemBufferPool()
{
	max_avails = INITI_POOL_WITH_SIZE;
	avail_list = (Item **)malloc(sizeof(Item*) * max_avails); //list that contains pointers to objects
	if( !avail_list )
		sLog.outError("Item Pool failed to alocate more memory = %u bytes",sizeof(Item*) * max_avails);
	next_free_avail = 0;
	InitPoolNewSection(0,max_avails);
}

//get rid of all objects
oItemBufferPool::~oItemBufferPool()
{
}

void oItemBufferPool::DestroyPool()
{

#ifdef TRACK_LEAKED_ITEMS_AND_MEMORY_CORRUPTION
	if( next_free_avail != 0 )
	{
		sLog.outDebug("Memory leak detected in Object Pool for items having %u leaked items. Listing pointers: \n",next_free_avail);
		std::list<Item *>::iterator itr=used_list.begin();
		uint32 foundit = 0;
		for(itr=used_list.begin();itr!=used_list.end();itr++ )
			sLog.outDebug("Leaked item p = %u with owner %u \n",*itr,(*itr)->GetOwner() );
	}
#endif

	for(uint32 i=next_free_avail;i<max_avails;i++)
		delete avail_list[i];

	free( avail_list );
	avail_list = NULL;
}

//new pool must be filled with item objects
void oItemBufferPool::InitPoolNewSection(uint32 from, uint32 to)
{
	for(uint32 i=from;i<to;i++)
	{
		avail_list[i] = new Item;
		ASSERT( avail_list[i] );
	}
}

//we increase our pool size if we run out of it
void oItemBufferPool::ExtedLimitAvailLimit()
{
	uint32 prev_max = max_avails;
	max_avails += EXTEND_POOL_WITH_SIZE;
	avail_list = (Item **)realloc( avail_list, sizeof(Item*) * max_avails );
	if( !avail_list )
		sLog.outError("Item Pool failed to alocate more memory = %u bytes",sizeof(Item*) * max_avails);
	InitPoolNewSection( prev_max, max_avails );
}

Item *oItemBufferPool::PooledNew()
{
	ObjLock.Acquire();

	//recycle the item object
	avail_list[ next_free_avail ]->Virtual_Constructor();

	//move our index to next free object
	uint32 free_index = next_free_avail;
	next_free_avail++;

	//out of buffer ? get new ones !
	if( next_free_avail == max_avails )
		ExtedLimitAvailLimit();

#ifdef TRACK_LEAKED_ITEMS_AND_MEMORY_CORRUPTION
	used_list.push_back( avail_list[ free_index ] );
#endif
	ObjLock.Release();

	return avail_list[ free_index ];
}

void oItemBufferPool::PooledDelete( Item *dumped )
{
	//containers are items and not stored in this pool atm
	if( dumped->IsContainer() )
	{
		delete dumped;
		return;
	}

	//remove events and remove object from world ...
	dumped->Virtual_Destructor();

	ObjLock.Acquire();

#ifdef _DEBUG
	if( next_free_avail == POOL_IS_FULL_INDEX )
	{
		sLog.outDebug("Pool is full and we still tried to add more items to it. Maybe item was alocated not from pool ? \n" );
		delete dumped;
		return; // OMG, We made more deletes then inserts ? This should not happen !
	}
	if( dumped->GetTypeId() != TYPEID_ITEM )
	{
		sLog.outDebug("We tried to add to the item pool an non item object ! \n" );
		return;
	}
#endif

#ifdef TRACK_LEAKED_ITEMS_AND_MEMORY_CORRUPTION
	//check if item is in list
	std::list<Item *>::iterator itr=used_list.begin();
	uint32 foundit = 0;
	for(itr=used_list.begin();itr!=used_list.end();itr++ )
		if( *itr == dumped )
		{
			foundit = 1;
			break;
		}
	if( foundit == 0 )
	{
		sLog.outDebug("Possible memory corruption while trying to delete object %u \n",dumped );
		return;
	}
#endif

	//We do not care about used up guids only available ones. Note that with this overwrite used guid list is not valid anymore
	next_free_avail--;
	avail_list[ next_free_avail ] = dumped;

#ifdef TRACK_LEAKED_ITEMS_AND_MEMORY_CORRUPTION
	used_list.remove( dumped );
#endif
	ObjLock.Release();
}

#ifdef TRACK_LEAKED_ITEMS_AND_MEMORY_CORRUPTION
uint32 oItemBufferPool::IsValidPointer( Item *objpointer )
{
	ObjLock.Acquire();
	std::list<Item *>::iterator itr=used_list.begin();
	uint32 Object_is_in_use = 0;
	for(itr=used_list.begin();itr!=used_list.end();itr++ )
		if( *itr == objpointer )
		{
			Object_is_in_use = 1;
			break;
		}
	ObjLock.Release();
	return Object_is_in_use;
}
#endif
#ifdef _DEBUG
//maybe we want to see if this pointer is really expired at some point
uint32	oItemBufferPool::IsDeletedPointer( Item *objpointer )
{
	ObjLock.Acquire();
	uint32 object_is_deleted = 0;
	for(uint32 i=next_free_avail;i<max_avails;i++)
		if( objpointer == avail_list[ i ] )
		{
			object_is_deleted = 1;
			break;
		}
	ObjLock.Release();
	return object_is_deleted;
}
#endif
createFileSingleton( oItemBufferPool );
