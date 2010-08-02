#pragma once

class SERVER_DECL GuardianAI : public AIInterface
{
protected:
	typedef std::deque<AI_Spell*> SpellDeck;
	typedef std::vector<AI_Spell*> SpellStore;
	Unit * m_Owner;
	uint64 m_NextTarget;
	SpellDeck m_spellDeck;
	SpellStore m_spells;
	uint32 m_PetAction;
	uint32 m_PetState;

	void updateMovement();
	void updateCombat();
	void updateTargets();

	/* Called in UpdateCombat, casts the spell that is currently at the top of our queue */
	virtual void Spell_cast();

	/*	Retrieves an appropriate target based on the given spell, returns NULL if there is no suitable target. */
	Unit * Spell_getarget(AI_Spell*);
public:
	GuardianAI(Unit * self, Unit * owner);
	~GuardianAI() 
	{
		m_spells.clear();
		m_spellDeck.clear();
	}
	/* Static update frequency function. */
	virtual void OnUpdate();
	
	/* Since guardians can't be controlled, getters return the default value and setters do nothing unless overridden */
	uint32 Pet_getaction() { return m_PetAction; }
	uint32 Pet_getstate() { return m_PetState; }
	virtual void Pet_setaction(uint32 action, Unit * obj) {}
	virtual void Pet_setstate(uint32 state) {}

	/*	Returns our current owner */
	Unit * getOwner() { return m_Owner; }
	/*	Sets our new owner */
	void setOwner(Unit * nOwner) { m_Owner = nOwner; }

	/*	Sets the target to focus on */
	void setNextTarget(Unit * newTarget);
	/*	Returns the target we are currently focusing on */
	Unit * getNextTarget();

	/*	Fills the specified list with our spells */
	void getSpellStore(std::vector<AI_Spell*> & spellstore)
	{
		spellstore = m_spells;
	}
	uint32 getSpellStoreSize() { return (uint32)m_spells.size(); }
	/*	More efficient to pass iterators when we want to access the spells */
	SpellStore::iterator getSpellStoreStart() { return m_spells.begin(); }
	SpellStore::iterator getSpellStoreEnd() { return m_spells.end(); }

	/*	Simply returns the spell at the top of our spell que. */
	AI_Spell * Spell_getnext() 
	{
		AI_Spell * ret = m_spellDeck.front();
		return ret;
	}
	/*	Checks if we have the given spell/spell_id in our spell storage. */
	virtual bool Spell_has(AI_Spell * spell)
	{
		bool result = false;
		if(spell != NULL)
		{
			for(SpellStore::iterator itr = m_spells.begin(); itr != m_spells.end(); ++itr)
			{
				if( (*itr) == spell)
				{
					result = true;
					break;
				}
			}
		}
		return result;
	}
	virtual bool Spell_has(uint32 spell_id)
	{
		bool result = false;
		if(spell_id)
		{
			for(SpellStore::iterator itr = m_spells.begin(); itr != m_spells.end(); ++itr)
			{
				if( (*itr)->proto->Id == spell_id)
				{
					result = true;
					break;
				}
			}
		}
		return result;
	}
	/* Places the specified spell on the top of the spell queue */
	void Spell_setnext(AI_Spell * spell)
	{
		//validate if we have it and also if it's cooldown is past.
		if( (Spell_has(spell) && spell->interval == 0x0) || getMSTime() >= spell->cooldown)
			m_spellDeck.push_back(spell);
	}
	/*	Adds the given spell to our spell store */
	void Spell_add(AI_Spell*);
	/*	Removes the given spell from teh spell store. */
	void Spell_remove(AI_Spell* oSpell);
	/*	Use this prior to adding spells to prevent re-allocations that vector's do. */
	void Spell_reserveStorage(uint32 cntSpells)
	{
		m_spells.reserve( cntSpells);
	}

	/*	Queues any spells in our spell store by that event. Pets handle their spells differently */
	virtual void Spell_handlevent(AiEvents evt)
	{
		uint32 now = getMSTime();
		for(SpellStore::iterator itr = m_spells.begin(); itr != m_spells.end(); ++itr)
		{
			//Validate the event type, cast count and cooldown.
			if( (*itr)->isQueued || (*itr)->event_type != (uint8)evt || ( (*itr)->maxcastcount && (*itr)->currentcastcount >= (*itr)->maxcastcount) || now < (*itr)->cooldown)
				continue;
			m_spellDeck.push_back( (*itr) );
			(*itr)->isQueued = true;
		}
	}

	virtual AI_Spell * Spell_findbyId(uint32 spell_id)
	{
		AI_Spell * result = NULL;
		SpellStore::iterator itr = m_spells.begin();
		for(; itr != m_spells.end(); ++itr)
		{
			if( (*itr)->proto->Id == spell_id)
			{
				result = (*itr);
				break;

			}
		}
		return result;
	}

	void AttackReaction(Unit * pUnit, uint32 damage_dealt, uint32 spellId);
	virtual void HandleEvent(uint32 event, Unit* pUnit, uint32 misc1);

#define TO_AIGUARDIAN(ai) static_cast<GuardianAI*>(ai)
#define AIType_isGuardian(aii) (aii->getAIType() == AITYPE_GUARDIAN)
};