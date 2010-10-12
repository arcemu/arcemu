/*
 * Circular Queue
 * Template C++ class that can be used to store a set number of elements,
 * replacing old ones as new ones are added.
 * Copyright (C) 2005-2007 Burlex <burlex@gmail.com>
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

#ifndef __CIRCULARQUEUE_H
#define __CIRCULARQUEUE_H

template<class T, int ELEMENTCOUNT>
class CircularQueue
{
	T m_elements[ELEMENTCOUNT];
	int m_pos;
public:
	CircularQueue() : m_pos(0)
	{
		memset(m_elements, 0, sizeof(T) * ELEMENTCOUNT);
	}
	~CircularQueue() {}

	void push(T& val)
	{
		m_elements[++m_pos] = val;
		if(m_pos >= ELEMENTCOUNT)
			m_pos = 0;
	}

	inline T * get()
	{
		return m_elements;
	}

	/* assumes we're int-based */
    void print()
	{
		printf("Elements of CircularQueue[%u]: ", ELEMENTCOUNT);
		for(int i = 0; i < ELEMENTCOUNT; ++i)
			printf("%u ", (int)m_elements[i]);
		printf("\n");
	}
};

#endif		// _CIRCULARQUEUE_H
