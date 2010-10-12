/*
 * Copyright (c) 2001 Jani Kajala
 *
 * Permission to use, copy, modify, distribute and sell this
 * software and its documentation for any purpose is hereby
 * granted without fee, provided that the above copyright notice
 * appear in all copies and that both that copyright notice and
 * this permission notice appear in supporting documentation.
 * Jani Kajala makes no representations about the suitability 
 * of this software for any purpose. It is provided "as is" 
 * without express or implied warranty.
 */
 
#ifndef _DEV_ARRAY_H
#define _DEV_ARRAY_H

/** Very simple dynamic array. */
template <class T> class Array
{
public:
	/** Creates an empty array. */
	Array() :
		m_data(0), m_len(0), m_cap(0)
	{
	}

	/** Creates an array of specified size. */
	explicit Array( int size ) :
		m_data(0), m_len(0), m_cap(0)
	{
		setSize( size );
	}

	///
	~Array()
	{
		delete[] m_data;
	}

	/** Appends an item at the end of the array. */
	void add( const T& item )
	{
		if ( m_len+1 > m_cap )
			setCapacity( m_len + 1 );
		m_data[m_len++] = item;
	}

	/** Resizes the array. */
	void setSize( int size )
	{
		if ( size > m_cap )
			setCapacity( size );
		m_len = size;
	}

	/** Returns ith item. */
	T& operator[]( int i )
	{
		return m_data[i];
	}

	/** Returns pointer to the first element in the vector. */
	T* begin() 
	{
		return m_data;
	}

	/** Returns pointer to one beyond the last element in the vector. */
	T* end() 
	{
		return m_data + m_len;
	}

	/** Returns number of items in the array. */
	int size() const
	{
		return m_len;
	}

	/** Returns ith item. */
	const T& operator[]( int i ) const
	{
		return m_data[i];
	}

	/** Returns pointer to the first element in the vector. */
	const T* begin() const														
	{
		return m_data;
	}

	/** Returns pointer to one beyond the last element in the vector. */
	const T* end() const															
	{
		return m_data + m_len;
	}

private:
	T*		m_data;
	int		m_len;
	int		m_cap;

	void setCapacity( int cap )
	{
		++cap;
		if ( cap < 8 )
			cap = 8;
		else if ( cap < m_cap*2 )
			cap = m_cap*2;
		m_cap = cap;

		T* data = new T[cap];
		for ( int i = 0 ; i < m_len ; ++i )
			data[i] = m_data[i];
		delete[] m_data;
		m_data = data;
	}
};

#endif // _DEV_ARRAY_H
