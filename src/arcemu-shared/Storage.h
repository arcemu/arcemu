/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008 <http://www.ArcEmu.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef STORAGE_H_
#define STORAGE_H_

#ifdef WIN32
#pragma warning(disable:4312)
#endif

// pooled allocations
//#define STORAGE_ALLOCATION_POOLS 1

#define STORAGE_ARRAY_MAX 200000

#ifdef STORAGE_ALLOCATION_POOLS
template<class T>
class SERVER_DECL StorageAllocationPool
{
	T* _pool;
	uint32 _count;
	uint32 _max;
public:
	void Init(uint32 count)
	{
		_pool = new T[count+100];
		_count = 0;
		_max = count+100;
	}

	T * Get()
	{
		if( _count >= _max )
		{
			printf("StorageAllocationPool Get() failed!\n");
			return NULL;
		}

		return &_pool[_count++];
	}

	void Free()
	{
		delete [] _pool;
	}
};
#endif

/** Base iterator class, returned by MakeIterator() functions.
 */
template<class T>
class SERVER_DECL StorageContainerIterator
{
protected:
	/** Currently referenced object
	 */
	T * Pointer;
public:
	virtual ~StorageContainerIterator() {}

	/** Returns the currently stored object
	 */
	ARCEMU_INLINE T * Get() { return Pointer; }

	/** Sets the current object to P
	 */
	ARCEMU_INLINE void Set(T * P) { Pointer = P; }

	/** Are we at the end of the storage container?
	 */
	ARCEMU_INLINE bool AtEnd() { return (Pointer == 0); }

	/** Virtual function to increment to the next element
	 */
	virtual bool Inc() = 0;

	/** Virtual function to destroy the iterator
	 */
	virtual void Destruct() = 0;
};

template<class T>
class SERVER_DECL ArrayStorageContainer
{
public:
#ifdef STORAGE_ALLOCATION_POOLS
	StorageAllocationPool<T> _pool;
	void InitPool(uint32 cnt) { _pool.Init( cnt ); }
#endif

	/** This is where the magic happens :P
	 */
	T ** _array;

	/** Maximum possible entry 
	 */
	uint32 _max;

	/** Returns an iterator currently referencing the start of the container
	 */
	StorageContainerIterator<T> * MakeIterator();

	/** Do we need to get the max?
	*/
	bool NeedsMax()
	{
		return true;
	}

	/** Creates the array with specified maximum
	 */
	void Setup(uint32 Max)
	{
		_array = new T*[Max];
		_max = Max;
		memset(_array, 0, sizeof(T*) * Max);
	}
	
	/** Sets up the array with a different maximum
	 */
	void Resetup(uint32 Max)
	{
		if(Max < _max)
			return;		// no need to realloc

        T ** a = new T*[Max];
		memset(a,0,sizeof(T*)*Max);
		memcpy(a, _array, sizeof(T*) * _max);
		delete [] _array;
		_array = a;
		_max = Max;
	}

	/** Frees the container array and all elements inside it
	 */
	~ArrayStorageContainer()
	{
#ifndef STORAGE_ALLOCATION_POOLS
		for(uint32 i = 0; i < _max; ++i)
			if(_array[i] != 0)
				delete _array[i];
#else
		_pool.Free();
#endif
		delete [] _array;
	}

	/** Allocates entry Entry in the array and sets the pointer, and returns
	 * the allocated memory.
	 */
	T * AllocateEntry(uint32 Entry)
	{
		if(Entry >= _max || _array[Entry] != 0)
			return reinterpret_cast<T*>(0);

#ifndef STORAGE_ALLOCATION_POOLS
		_array[Entry] = new T;
#else
		_array[Entry] = _pool.Get();
#endif
		return _array[Entry];
	}

	/** Deallocates the entry Entry in the array and sets the pointer to null.
	 */
	bool DeallocateEntry(uint32 Entry)
	{
		if(Entry >= _max || _array[Entry] == 0)
			return false;

#ifndef STORAGE_ALLOCATION_POOLS
		delete _array[Entry];
#endif
		_array[Entry] = 0;
		return true;
	}

	/** Looks up entry Entry and returns the pointer if it is existant, otherwise null.
	 */
	T * LookupEntry(uint32 Entry)
	{
		if(Entry >= _max)
			return reinterpret_cast<T*>(0);
		else
			return _array[Entry];
	}

	/** Sets the pointer to entry Entry to Pointer, and if it already exists frees the existing
	 * element.
	 */
	bool SetEntry(uint32 Entry, T * Pointer)
	{
		if(Entry > _max)
			return false;

#ifndef STORAGE_ALLOCATION_POOLS
		if(_array[Entry] != 0)
			delete _array[Entry];
#endif

		_array[Entry] = Pointer;
		return true;
	}

	/** Returns the current pointer if it exists, otherwise allocates it.
	 */
	T * LookupEntryAllocate(uint32 Entry)
	{
		T * ret = LookupEntry(Entry);
		if(!ret)
			ret = AllocateEntry(Entry);
		return ret;
	}

	/** Deletes all entries in the container.
	 */
	void Clear()
	{
		for(uint32 i = 0; i < _max; ++i)
		{
#ifndef STORAGE_ALLOCATION_POOLS
			if(_array[i] != 0)
			{
				delete _array[i];
			}
#endif
			_array[i] = 0;
		}
	}
};

template<class T>
class SERVER_DECL HashMapStorageContainer
{
public:
#ifdef STORAGE_ALLOCATION_POOLS
	StorageAllocationPool<T> _pool;
	void InitPool(uint32 cnt) { _pool.Init( cnt ); }
#endif

	typename HM_NAMESPACE::hash_map<uint32, T*> _map;

	/** Returns an iterator currently referencing the start of the container
	 */
	StorageContainerIterator<T> * MakeIterator();

	/** Frees the container array and all elements inside it
	 */
	~HashMapStorageContainer()
	{
		for(typename HM_NAMESPACE::hash_map<uint32, T*>::iterator itr = _map.begin(); itr != _map.end(); ++itr)
			delete itr->second;
	}

	/** Do we need to get the max?
	 */
	bool NeedsMax()
	{
		return false;
	}

	/** Creates the array with specified maximum
	 */
	void Setup(uint32 Max)
	{

	}

	void Resetup(uint32 Max)
	{

	}

	/** Allocates entry Entry in the array and sets the pointer, and returns
	 * the allocated memory.
	 */
	T * AllocateEntry(uint32 Entry)
	{
		if(_map.find(Entry) != _map.end())
			return reinterpret_cast<T*>(0);
#ifdef STORAGE_ALLOCATION_POOLS
		T * n = _pool.Get();
#else
		T * n = new T;
#endif
		_map.insert( make_pair( Entry, n ) );
		return n;
	}

	/** Deallocates the entry Entry in the array and sets the pointer to null.
	 */
	bool DeallocateEntry(uint32 Entry)
	{
		typename HM_NAMESPACE::hash_map<uint32, T*>::iterator itr = _map.find(Entry);
		if(itr == _map.end())
			return false;

#ifndef STORAGE_ALLOCATION_POOLS
		delete itr->second;
#endif
		_map.erase(itr);
		return true;
	}


	T * LookupEntry(uint32 Entry)
	{
		typename HM_NAMESPACE::hash_map<uint32, T*>::iterator itr = _map.find(Entry);
		if(itr == _map.end())
			return reinterpret_cast<T*>(0);
		return itr->second;
	}

	/** Sets the pointer to entry Entry to Pointer, and if it already exists frees the existing
	 * element.
	 */
	bool SetEntry(uint32 Entry, T * Pointer)
	{
		typename HM_NAMESPACE::hash_map<uint32, T*>::iterator itr = _map.find(Entry);
		if(itr == _map.end())
		{
			_map.insert( make_pair( Entry, Pointer ) );
			return true;
		}

#ifndef STORAGE_ALLOCATION_POOLS
		delete itr->second;
		itr->second = Pointer;
#endif
		return true;
	}

	/** Returns the current pointer if it exists, otherwise allocates it.
	 */
	T * LookupEntryAllocate(uint32 Entry)
	{
		T * ret = LookupEntry(Entry);
		if(!ret)
			ret = AllocateEntry(Entry);
		return ret;
	}

	/** Deletes all entries in the container.
	 */
	void Clear()
	{
		typename HM_NAMESPACE::hash_map<uint32, T*>::iterator itr = _map.begin();
		for(; itr != _map.end(); ++itr)
			delete itr->second;
		_map.clear();
	}
};

template<class T>
class SERVER_DECL ArrayStorageIterator : public StorageContainerIterator<T>
{
	ArrayStorageContainer<T> * Source;
	uint32 MyIndex;
public:

	/** Increments the iterator
	*/
	bool Inc()
	{
		GetNextElement();
		if(StorageContainerIterator<T>::Pointer != 0)
			return true;
		else
			return false;
	}

	/** Frees the memory occupied by this iterator
	*/
	void Destruct()
	{
		delete this;
	}

	/** Constructor
	*/
	ArrayStorageIterator(ArrayStorageContainer<T> * S) : StorageContainerIterator<T>(), Source(S), MyIndex(0)
	{
		GetNextElement();
	}

	/** Sets the next element pointer, or to 0 if we reached the end
	*/
	void GetNextElement()
	{
		while(MyIndex < Source->_max)
		{
			if(Source->_array[MyIndex] != 0)
			{
				StorageContainerIterator<T>::Set(Source->_array[MyIndex]);
				++MyIndex;
				return;
			}
			++MyIndex;
		}
		// reached the end of the array
		StorageContainerIterator<T>::Set(reinterpret_cast<T*>(0));
	}
};

template<class T>
class SERVER_DECL HashMapStorageIterator : public StorageContainerIterator<T>
{
	HashMapStorageContainer<T> * Source;
	typename HM_NAMESPACE::hash_map<uint32, T*>::iterator itr;
public:

	/** Constructor
	*/
	HashMapStorageIterator(HashMapStorageContainer<T> * S) : StorageContainerIterator<T>(), Source(S)
	{
		itr = S->_map.begin();
		if(itr == S->_map.end())
			StorageContainerIterator<T>::Set(0);
		else
			StorageContainerIterator<T>::Set(itr->second);
	}

	/** Gets the next element, or if we reached the end sets it to 0
	*/
	void GetNextElement()
	{
		++itr;
		if(itr == Source->_map.end())
			StorageContainerIterator<T>::Set(0);
		else
			StorageContainerIterator<T>::Set(itr->second);
	}

	/** Returns true if we're not at the end, otherwise false.
	*/
	bool Inc()
	{
		GetNextElement();
		if(StorageContainerIterator<T>::Pointer != 0)
			return true;
		else
			return false;
	}

	/** Frees the memory occupied by this iterator
	*/
	void Destruct()
	{
		delete this;
	}
};

#ifndef SCRIPTLIB
template<class T>
StorageContainerIterator<T> * ArrayStorageContainer<T>::MakeIterator()
{
	return new ArrayStorageIterator<T>(this);
}

template<class T>
StorageContainerIterator<T> * HashMapStorageContainer<T>::MakeIterator()
{
	return new HashMapStorageIterator<T>(this);
}
#endif

template<class T, class StorageType>
class SERVER_DECL Storage
{
protected:
	StorageType _storage;
	char * _indexName;
	char * _formatString;
public:
	
	ARCEMU_INLINE char * GetIndexName() { return _indexName; }
	ARCEMU_INLINE char * GetFormatString() { return _formatString; }

	/** False constructor to fool compiler
	 */
	Storage() {}
	virtual ~Storage() {}

	/** Makes an iterator, w00t!
	 */
	StorageContainerIterator<T> * MakeIterator()
	{
		return _storage.MakeIterator();
	}

	/** Calls the storage container lookup function.
	 */
	T * LookupEntry(uint32 Entry)
	{
		return _storage.LookupEntry(Entry);
	}

	/** Reloads the content in this container.
	 */
    	virtual void Reload() = 0;

	/** Loads the container using the specified name and format string
	 */
	virtual void Load(const char * IndexName, const char * FormatString)
	{
		_indexName = strdup(IndexName);
		_formatString = strdup(FormatString);
	}

	/** Frees the duplicated strings and all entries inside the storage container
	 */
	virtual void Cleanup()
	{
		printf("Deleting database cache of `%s`...\n", _indexName);
        StorageContainerIterator<T> * itr = _storage.MakeIterator();
		while(!itr->AtEnd())
		{
			FreeBlock(itr->Get());
			if(!itr->Inc())
				break;
		}
		itr->Destruct();

		_storage.Clear();
		free(_indexName);
		free(_formatString);
	}

	/** Frees any string elements inside blocks. 
	 */
	void FreeBlock(T * Allocated)
	{
		char * p = _formatString;
		char * structpointer = (char*)Allocated;
		for(; *p != 0; ++p)
		{
			switch(*p)
			{
			case 's':		// string is the only one we have to actually do anything for here
				free((*(char**)structpointer));
				structpointer += sizeof(char*);
				break;

			case 'u':
			case 'i':
			case 'f':
				structpointer += sizeof(uint32);
				break;
				
			case 'h':
				structpointer += sizeof(uint16);
				break;

			case 'c':
				structpointer += sizeof(uint8);
				break;
			}
		}
	}
};

template<class T, class StorageType>
class SERVER_DECL SQLStorage : public Storage<T, StorageType>
{
public:
	SQLStorage() : Storage<T, StorageType>() {}
	~SQLStorage() {}

	/** Loads the block using the format string.
	 */
	ARCEMU_INLINE void LoadBlock(Field * fields, T * Allocated)
	{
		char * p = Storage<T, StorageType>::_formatString;
		char * structpointer = (char*)Allocated;
		uint32 offset = 0;
		Field * f = fields;
		for(; *p != 0; ++p, ++f)
		{
            switch(*p)
			{
			case 'u':	// Unsigned integer
				*(uint32*)&structpointer[offset] = f->GetUInt32();
				offset += sizeof(uint32);
				break;

			case 'i':	// Signed integer
				*(int32*)&structpointer[offset] = f->GetInt32();
				offset += sizeof(int32);
				break;

			case 's':	// Null-terminated string
				*(char**)&structpointer[offset] = strdup(f->GetString());
				offset += sizeof(char*);
				break;

			case 'x':	// Skip
				break;

			case 'f':	// Float
				*(float*)&structpointer[offset] = f->GetFloat();
				offset += sizeof(float);
				break;

			case 'c':	// Char
				*(uint8*)&structpointer[offset] = f->GetUInt8();
				offset += sizeof(uint8);
				break;

			case 'h':	// Short
				*(uint16*)&structpointer[offset] = f->GetUInt16();
				offset += sizeof(uint16);
				break;

			default:	// unknown
				printf("Unknown field type in string: `%c`\n", *p);
				break;
			}
		}
	}

	/** Loads from the table.
	 */
	void Load(const char * IndexName, const char * FormatString)
	{
		//printf("Loading database cache from `%s`...\n", IndexName);
		Storage<T, StorageType>::Load(IndexName, FormatString);
		QueryResult * result;
		if(Storage<T, StorageType>::_storage.NeedsMax())
		{
			result = WorldDatabase.Query("SELECT MAX(entry) FROM %s", IndexName);
			uint32 Max = STORAGE_ARRAY_MAX;
			if(result)
			{
				Max = result->Fetch()[0].GetUInt32() + 1;
				if(Max > STORAGE_ARRAY_MAX)
				{
					Log.Error("Storage", "The table, '%s', has a maximum entry of %u, which is less %u. Any items higher than %u will not be loaded.",
						IndexName, Max, STORAGE_ARRAY_MAX, STORAGE_ARRAY_MAX);

					Max = STORAGE_ARRAY_MAX;
				}
				delete result;
			}

			Storage<T, StorageType>::_storage.Setup(Max);
		}

		size_t cols = strlen(FormatString);
		result = WorldDatabase.Query("SELECT * FROM %s", IndexName);
		if (!result)
			return;
		Field * fields = result->Fetch();

		if(result->GetFieldCount() != cols)
		{
			if(result->GetFieldCount() > cols)
			{
				Log.Warning("Storage", "Invalid format in %s (%u/%u), loading anyway because we have enough data\n", IndexName, (unsigned int)cols, (unsigned int)result->GetFieldCount());
			}
			else
			{
				Log.Error("Storage", "Invalid format in %s (%u/%u), not enough data to proceed.\n", IndexName, (unsigned int)cols, (unsigned int)result->GetFieldCount());
				delete result;
				return;
			}
		}

		uint32 Entry;
		T * Allocated;
#ifdef STORAGE_ALLOCATION_POOLS
		Storage<T, StorageType>::_storage.InitPool( result->GetRowCount() );
#endif
		do 
		{
			Entry = fields[0].GetUInt32();
			Allocated = Storage<T, StorageType>::_storage.AllocateEntry(Entry);
			if(!Allocated)
				continue;

			LoadBlock(fields, Allocated);
		} while(result->NextRow());
		Log.Notice("Storage", "%u entries loaded from table %s.", result->GetRowCount(), IndexName);
		delete result;

		//Log.Success("Storage", "Loaded database cache from `%s`.", IndexName);
	}

	void LoadAdditionalData(const char * IndexName, const char * FormatString)
	{
		Storage<T, StorageType>::Load(IndexName, FormatString);
		QueryResult * result;
		if(Storage<T, StorageType>::_storage.NeedsMax())
		{
			result = WorldDatabase.Query("SELECT MAX(entry) FROM %s", IndexName);
			uint32 Max = STORAGE_ARRAY_MAX;
			if(result)
			{
				Max = result->Fetch()[0].GetUInt32() + 1;
				if(Max > STORAGE_ARRAY_MAX)
				{
					Log.Error("Storage", "The table, '%s', has a maximum entry of %u, which is less %u. Any items higher than %u will not be loaded.",
						IndexName, Max, STORAGE_ARRAY_MAX, STORAGE_ARRAY_MAX);

					Max = STORAGE_ARRAY_MAX;
				}
				delete result;
			}

			Storage<T, StorageType>::_storage.Resetup(Max);
		}
		
		size_t cols = strlen(FormatString);
		result = WorldDatabase.Query("SELECT * FROM %s", IndexName);
		if (!result)
			return;
		Field * fields = result->Fetch();

		if(result->GetFieldCount() != cols)
		{
			if(result->GetFieldCount() > cols)
			{
				Log.Warning("Storage", "Invalid format in %s (%u/%u), loading anyway because we have enough data\n", IndexName, (unsigned int)cols, (unsigned int)result->GetFieldCount());
			}
			else
			{
				Log.Error("Storage", "Invalid format in %s (%u/%u), not enough data to proceed.\n", IndexName, (unsigned int)cols, (unsigned int)result->GetFieldCount());
				delete result;
				return;
			}
		}

		uint32 Entry;
		T * Allocated;
		do 
		{
			Entry = fields[0].GetUInt32();
			Allocated = Storage<T, StorageType>::_storage.LookupEntryAllocate(Entry);
			if(!Allocated)
				continue;

			LoadBlock(fields, Allocated);
		} while(result->NextRow());
		Log.Notice("Storage", "%u entries loaded from table %s.", result->GetRowCount(), IndexName);
		delete result;
	}

	/** Reloads the storage container
	 */
	void Reload()
	{
		Log.Notice("Storage", "Reloading database cache from `%s`...\n", Storage<T, StorageType>::_indexName);
		QueryResult * result = WorldDatabase.Query("SELECT MAX(entry) FROM %s", Storage<T, StorageType>::_indexName);
		if(result == 0)
			return;

		uint32 Max = result->Fetch()[0].GetUInt32();
		delete result;
		if(!Max)
			return;

		if(Storage<T, StorageType>::_storage.NeedsMax())
		{
			if(Max > STORAGE_ARRAY_MAX)
				Max = STORAGE_ARRAY_MAX;

			Storage<T, StorageType>::_storage.Resetup(Max+1);
		}

		size_t cols = strlen(Storage<T, StorageType>::_formatString);
		result = WorldDatabase.Query("SELECT * FROM %s", Storage<T, StorageType>::_indexName);
		if (!result)
			return;
		Field * fields = result->Fetch();

		if(result->GetFieldCount() != cols)
		{
			Log.Error("Storage", "Invalid format in %s (%u/%u).", Storage<T, StorageType>::_indexName, (unsigned int)cols, (unsigned int)result->GetFieldCount());
			delete result;
			return;
		}

		uint32 Entry;
		T * Allocated;
		do 
		{
			Entry = fields[0].GetUInt32();
			Allocated = Storage<T, StorageType>::_storage.LookupEntryAllocate(Entry);
			if(Allocated)
				LoadBlock(fields, Allocated);

		} while(result->NextRow());
		delete result;
	}
};

#endif
