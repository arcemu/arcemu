#ifndef _ITEM_BUFFER_POOL
#define _ITEM_BUFFER_POOL

//hmm with 2000 players each have 50 items ...Decrease it if you need the space ;)
#define INITI_POOL_WITH_SIZE 10000
//too big values migh create lag spikes on buffer limit extension !
#define EXTEND_POOL_WITH_SIZE 100
#define POOL_IS_FULL_INDEX 0

//noob protection :P
#ifdef _DEBUG
//	#define TRACK_LEAKED_ITEMS_AND_MEMORY_CORRUPTION
#endif

class Item;

class SERVER_DECL oItemBufferPool : public Singleton< oItemBufferPool > 
{
public:
	oItemBufferPool();
	~oItemBufferPool();
	Item	*PooledNew();				//fetch from internal list a new Item object
	void	PooledDelete( Item *dumped );		//insert into free object list
	void	DestroyPool();		//since some objects may depend on us and we might depend on other objects sometimes it is better to know when we destroy the pool instead of the destructor
//	inline uint32	GetUsedItemCount(){ return next_free_avail; }	//use this to check for memory leaks at server shutdown
#ifdef TRACK_LEAKED_ITEMS_AND_MEMORY_CORRUPTION
	//maybe we want to make sure at some point nothing deleted our object
	uint32	IsValidPointer( Item *objpointer );	//this is slow and only use it for debugging
#endif
#ifdef _DEBUG
	//maybe we want to see if this pointer is really expired at some point
	uint32	IsDeletedPointer( Item *objpointer ); //this is slow and only use it for debugging
#endif
private:
	void				InitPoolNewSection(uint32 from, uint32 to);
	void				ExtedLimitAvailLimit();
	Item				**avail_list;
	uint32				max_avails;
	uint32				next_free_avail;
	Mutex				ObjLock;
#ifdef TRACK_LEAKED_ITEMS_AND_MEMORY_CORRUPTION
	std::list<Item *>	used_list;
#endif
};

#define ItemPool oItemBufferPool::getSingleton()
#endif

