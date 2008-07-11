#include "Stdafx.h"
#include "ItemPool.h"

oItemBufferPool::oItemBufferPool()
{
	max_avails = INITI_POOL_WITH_SIZE;
	avail_list = (Item **)malloc(sizeof(Item*) * max_avails); //list that contains pointers to objects
	next_free_avail = 0;
	InitPoolNewSection(0,max_avails);
}

oItemBufferPool::~oItemBufferPool()
{
	ObjLock.Acquire();

	for(uint32 i=0;i<max_avails;i++)
		delete avail_list[i];
	free( avail_list );

	ObjLock.Release();
}

inline void oItemBufferPool::InitPoolNewSection(uint32 from, uint32 to)
{
	for(uint32 i=from;i<to;i++)
		avail_list[i] = new Item;
}

inline void oItemBufferPool::ExtedLimitAvailLimit()
{
	uint32 prev_max = max_avails;
	max_avails += EXTEND_POOL_WITH_SIZE;
	avail_list = (Item **)realloc( avail_list, sizeof(Item*) * max_avails );
	InitPoolNewSection( prev_max, max_avails );
}

Item *oItemBufferPool::PooledNew()
{
	ObjLock.Acquire();

	avail_list[ next_free_avail ]->Item_Recycle();
	uint32 free_index = next_free_avail;
	next_free_avail++;

	//out of buffer ? get new ones !
	if( next_free_avail == max_avails )
		ExtedLimitAvailLimit();

	ObjLock.Release();

	return avail_list[ free_index ];
}

void oItemBufferPool::PooledDelete(Item *)
{
	ObjLock.Acquire();

#ifdef _DEBUG
	if( next_free_avail == 0 )
		return; // OMG, We made more deletes then inserts ? This should not happen !
#endif

	next_free_avail--;

	ObjLock.Release();
}

createFileSingleton( oItemBufferPool );
