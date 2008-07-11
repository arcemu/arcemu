//!!!!!!!!!!!! Experimental Stage !!!!
//will remove warning after finished 
//not used by default !

#ifndef _ITEM_BUFFER_POOL
#define _ITEM_BUFFER_POOL

//hmm with 2000 players each have 50 items ...Decrease it if you need the space ;)
#define INITI_POOL_WITH_SIZE 100000
//too big values migh create lag spikes on buffer limit extension !
#define EXTEND_POOL_WITH_SIZE 1000

class Item;

class SERVER_DECL oItemBufferPool : public Singleton< oItemBufferPool > 
{
public:
	oItemBufferPool();
	~oItemBufferPool();
	Item *PooledNew();				//fetch from internal list a new Item object
	void PooledDelete(Item *);		//insert into free object list
private:
	void				InitPoolNewSection(uint32 from, uint32 to);
	void				ExtedLimitAvailLimit();
	Item				**avail_list;
	uint32				max_avails;
	uint32				next_free_avail;
	Mutex				ObjLock;
};

#define ItemPool oItemBufferPool::getSingleton()

#endif