/*
 * shared_ptr.hpp - Copyright (C) 2007 by Nathan Reed
 * Implementation of shared_ptr class template.
 */
 
// Declaration of container for the refcounts
//#include "StdAfx.h"
#ifdef WIN32
	typedef stdext::hash_map<const void *, int> refcounts_t;
#else
	struct ptr_hash
	{
		size_t operator () (const void * const v) const
		{
			static __gnu_cxx::hash<unsigned int> H;
			return H((unsigned int)v);
		}
	};
	typedef __gnu_cxx::hash_map<const void *, int, ptr_hash> refcounts_t;
#endif
extern refcounts_t refcounts_;

/*
 * shared_ptr <T> implementation
 */
template <typename T>
cshared_ptr<T>::cshared_ptr (T* ptr_ ): ptr(ptr_)
{
	++refcounts_[ptr];
}

template <typename T>
cshared_ptr<T>::cshared_ptr (const cshared_ptr<T>& rhs): u.ptr(rhs.get() )
{
	++refcounts_[ptr];
}

template <typename T>
template <typename U>
cshared_ptr<T>::cshared_ptr (const cshared_ptr<U>& rhs): ptr(rhs.get() )
{
	++refcounts_[ptr];
}

template <typename T>
template <typename U>
cshared_ptr<T>& cshared_ptr<T>::operator = (const cshared_ptr<U>& rhs)
{
	reset();
	ptr = static_cast<T*>(rhs.get());
	++refcounts_[ptr];
	return *this;
}
template <typename T>
T * cshared_ptr<T>::get()
{
	return ptr;
}

template <typename T>
void cshared_ptr<T>::set(T * ptr)
{
	ptr = ptr;
}

template <typename T>
T* cshared_ptr<T>::operator -> ()
{
	return ptr;
}

template <typename T>
long cshared_ptr<T>::use_count () const
{
	return refcounts_[ptr];
}
template <typename T>
void cshared_ptr<T>::reset ()
{
	if (!ptr) return;
	--refcounts_[ptr];
	ptr = 0;
}
template <typename T>
cshared_ptr<T>::~cshared_ptr ()
{
	reset();
}
