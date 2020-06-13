#ifndef ITEMITERATOR_H_
#define ITEMITERATOR_H_

class ItemIterator
{
		bool m_atEnd;
		bool m_searchInProgress;
		uint32 m_slot;
		uint32 m_containerSlot;
		Container* m_container;
		Item* m_currentItem;
		ItemInterface* m_target;
	public:
		ItemIterator(ItemInterface* target) : m_atEnd(false), m_searchInProgress(false), m_slot(0), m_containerSlot(0), m_container(NULL), m_target(target) {}
		~ItemIterator() { if(m_searchInProgress) { EndSearch(); } }

		void BeginSearch()
		{
			// iteminterface doesn't use mutexes, maybe it should :P
			ARCEMU_ASSERT(!m_searchInProgress);
			m_atEnd = false;
			m_searchInProgress = true;
			m_container = NULL;
			m_currentItem = NULL;
			m_slot = 0;
			Increment();
		}

		void EndSearch()
		{
			// nothing here either
			ARCEMU_ASSERT(m_searchInProgress);
			m_atEnd = true;
			m_searchInProgress = false;
		}

		Item* operator*() const
		{
			return m_currentItem;
		}

		Item* operator->() const
		{
			return m_currentItem;
		}

		void Increment()
		{
			if(!m_searchInProgress)
				BeginSearch();

			// are we currently inside a container?
			if(m_container != NULL)
			{
				// loop the container.
				for(; m_containerSlot < m_container->GetProto()->ContainerSlots; ++m_containerSlot)
				{
					m_currentItem = m_container->GetItem(static_cast<int16>(m_containerSlot));
					if(m_currentItem != NULL)
					{
						// increment the counter so we don't get the same item again
						++m_containerSlot;

						// exit
						return;
					}
				}

				// unset this
				m_container = NULL;
			}

			for(; m_slot < MAX_INVENTORY_SLOT; ++m_slot)
			{
				if(m_target->m_pItems[m_slot])
				{
					if(m_target->m_pItems[m_slot]->IsContainer())
					{
						// we are a container :O lets look inside the box!
						m_container = TO< Container* >(m_target->m_pItems[m_slot]);
						m_containerSlot = 0;

						// clear the pointer up. so we can tell if we found an item or not
						m_currentItem = NULL;

						// increment m_slot so we don't search this container again
						++m_slot;

						// call increment() recursively. this will search the container.
						Increment();

						// jump out so we're not wasting cycles and skipping items
						return;
					}

					// we're not a container, just a regular item
					// set the pointer
					m_currentItem = m_target->m_pItems[m_slot];

					// increment the slot counter so we don't do the same item again
					++m_slot;

					// jump out
					return;
				}
			}

			// if we're here we've searched all items.
			m_atEnd = true;
			m_currentItem = NULL;
		}

		ARCEMU_INLINE Item* Grab() { return m_currentItem; }
		ARCEMU_INLINE bool End() { return m_atEnd; }
};

#endif
