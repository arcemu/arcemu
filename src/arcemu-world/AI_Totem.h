#pragma once
class SERVER_DECL TotemAI : public AIInterface
{
protected:
	Unit * mOwner;
	typedef std::vector<AI_Spell*> SpellStore;
	SpellStore m_spells;
	std::deque<AI_Spell*> m_spellQue;

public:
	TotemAI(Unit * self, Unit * owner);

	void OnUpdate();

	/*	Simply checks the cooldown  of all our spells and pushes them to the spell que if they have expired. */
	void UpdateSpells()
	{
		uint32 now = getMSTime();
		for(SpellStore::iterator itr = m_spells.begin(); itr != m_spells.end(); ++itr)
		{
			if( (*itr)->cooldown >= now)
				m_spellQue.push_back( (*itr) );
		}
	}

	/*	Stores the given AI_Spell to the internal spell store only if it's not alrdy there. */
	void Spell_add(AI_Spell * nSpell)
	{
		bool found = false;
		for(SpellStore::iterator itr = m_spells.begin(); itr != m_spells.end(); ++itr)
		{
			if( (*itr) == nSpell)
			{
				found = true;
				break;
			}
		}
		if(!found)
			m_spells.push_back(nSpell);
	}

	/*	Removes the given AI_Spell from the internal spell store. */
	void Spell_remove(AI_Spell * nSpell)
	{
		for(SpellStore::iterator itr = m_spells.begin(); itr != m_spells.end(); ++itr)
		{
			if( (*itr) == nSpell)
			{
				m_spells.erase(itr);
				break;
			}
		}
	}

	/*	Returns the at the top of our spell que. */
	AI_Spell * Spell_getnext() { return m_spellQue.front(); }
	/*	Pushes the given AI_Spell to the front of our spell que. */
	void Spell_setnext(AI_Spell* next)
	{
		if(next != NULL)
			m_spellQue.push_front(next);
	}

	/*	Fills the specified list with the spells we currently have */
	void getSpellStore(std::vector<AI_Spell*> & out) 
	{
		out = m_spells;
	}

	uint32 getSpellStoreSize() { return (uint32)m_spells.size(); }
	SpellStore::iterator getSpellStoreStart() { return m_spells.begin(); }
	SpellStore::iterator getSpellStoreEnd() { return m_spells.end(); }

	/* Reserve storage for spells prior to adding  */
	void Spell_reserveStorage(uint32 spellcnt)
	{
		m_spells.reserve(spellcnt);
	}
	void Spell_handlevent(AiEvents evt)
	{
		uint32 now = getMSTime();
		for(SpellStore::iterator itr = m_spells.begin(); itr != m_spells.end(); ++itr)
		{
			if( (*itr)->event_type != (uint8)evt || (*itr)->isQueued || now < (*itr)->cooldown || (*itr)->maxcastcount && (*itr)->currentcastcount >= (*itr)->maxcastcount)
				continue;
			m_spellQue.push_back( (*itr) );
			(*itr)->isQueued = true;
		}
	}

#define AIType_isTotem(aii) (aii->getAIType() == AITYPE_TOTEM)
};