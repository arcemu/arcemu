#ifndef _SPELL_BUFFER_POOL
#define _SPELL_BUFFER_POOL

//hmm with 2000 players each have 50 items ...Decrease it if you need the space ;)
#define INITI_POOL_WITH_SIZE 10000
//too big values migh create lag spikes on buffer limit extension !
#define EXTEND_POOL_WITH_SIZE 100
#define POOL_IS_FULL_INDEX 0

//noob protection :P
#ifdef _DEBUG
//	#define TRACK_LEAKED_SpellS_AND_MEMORY_CORRUPTION
#endif

class Spell;

class SERVER_DECL oSpellBufferPool : public Singleton< oSpellBufferPool > 
{
public:
	oSpellBufferPool();
	~oSpellBufferPool();
	Spell	*PooledNew();				//fetch from internal list a new Spell object
	void	PooledDelete( Spell *dumped );		//insert into free object list
	void	DestroyPool();		//since some objects may depend on us and we might depend on other objects sometimes it is better to know when we destroy the pool instead of the destructor
//	inline uint32	GetUsedSpellCount(){ return next_free_avail; }	//use this to check for memory leaks at server shutdown
#ifdef TRACK_LEAKED_ITEMS_AND_MEMORY_CORRUPTION
	//maybe we want to make sure at some point nothing deleted our object
	uint32	IsValidPointer( Spell *objpointer );	//this is slow and only use it for debugging
#endif
#ifdef _DEBUG
	//maybe we want to see if this pointer is really expired at some point
	uint32	IsDeletedPointer( Spell *objpointer ); //this is slow and only use it for debugging
#endif
private:
	void				InitPoolNewSection(uint32 from, uint32 to);
	void				ExtedLimitAvailLimit();
	Spell				**avail_list;
	uint32				max_avails;
	uint32				next_free_avail;
	Mutex				ObjLock;
#ifdef TRACK_LEAKED_SpellS_AND_MEMORY_CORRUPTION
	std::list<Spell *>	used_list;
#endif
};

#define SpellPool oSpellBufferPool::getSingleton()
#endif

