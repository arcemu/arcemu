//!!!!!!!!!!!! Experimental Stage !!!!
//will remove warning after finished 
//not used by default !

#ifndef _ITEM_BUFFER_POOL
#define _ITEM_BUFFER_POOL

//hmm with 2000 players each have 50 items ...Decrease it if you need the space ;)
//#define INITI_POOL_WITH_SIZE 100000
#define INITI_POOL_WITH_SIZE 2
//too big values migh create lag spikes on buffer limit extension !
//#define EXTEND_POOL_WITH_SIZE 1000
#define EXTEND_POOL_WITH_SIZE 2

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
	Item *PooledNew();				//fetch from internal list a new Item object
	void	PooledDelete( Item *dumped );		//insert into free object list
	inline uint32	GetUsedItemCount(){ return next_free_avail; }	//use this to check for memory leaks at server shutdown
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

