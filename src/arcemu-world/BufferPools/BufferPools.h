#ifndef _ITEM_BUFFER_POOL
#define _ITEM_BUFFER_POOL

//hmm with 2000 players each have 50 items ...Decrease it if you need the space ;)
#define INITI_POOL_WITH_SIZE 100000
#define INITI_CONTAINER_POOL_WITH_SIZE 5000
//too big values migh create lag spikes on buffer limit extension !
#define EXTEND_POOL_WITH_SIZE 100
//let us have a gap of pool fullness. Some auras get deleted more then once when i write this code and they are overwriting each other
#define MINIMUM_POOL_FULNESS 10
#define POOL_IS_FULL_INDEX 0
#define OBJECT_WAS_ALLOCATED_WITHIN_POOL 0x0FFFFFF0
#define OBJECT_WAS_ALLOCATED_STANDARD_WAY -1	//should add this to constructors instead of constant number :)
#define OBJECT_WAS_DEALLOCATED_WITHIN_POOL 0x0FFFFFF2

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
	tPPoolClass(){}

	~tPPoolClass(){}

	T* PooledNew()
	{
        //yes, i'm too lazy to completely get ridd of this class so im doing this.
		T* objectClass=new T();
		objectClass->Virtual_Constructor();
		return objectClass;
	}
	//insert into free object list
	void PooledDelete(T* dumped)
	{
		if( dumped ) dumped->Virtual_Destructor();
		else return; //shit happens, in too many forms sadly

	}

	void DestroyPool()
	{
		delete this;
	}
private:
	void InitPoolNewSection(uint32 from, uint32 to)
	{

	}

	void ExtedLimitAvailLimit()
	{

	}
};

/* Grey: ContainerPooling have to come with a sepperate Patch */

//#define ContainerPool tPPoolClass<Container>::getSingleton()
#define ItemPool tPPoolClass<Item>::getSingleton()
#define AuraPool tPPoolClass<Aura>::getSingleton()
#define SpellPool tPPoolClass<Spell>::getSingleton()

#endif
