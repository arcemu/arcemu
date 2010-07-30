#pragma once

enum PET_ACTION
{
	PET_ACTION_STAY,
	PET_ACTION_FOLLOW,
	PET_ACTION_ATTACK,
	PET_ACTION_DISMISS
};

enum PET_STATE
{
	PET_STATE_PASSIVE,
	PET_STATE_DEFENSIVE,
	PET_STATE_AGGRESSIVE
};

enum PET_ACTIONBARSPELL
{
	PET_SPELL_PASSIVE = 0x06000000,
	PET_SPELL_DEFENSIVE,
	PET_SPELL_AGRESSIVE,
	PET_SPELL_STAY = 0x07000000,
	PET_SPELL_FOLLOW,
	PET_SPELL_ATTACK
};

#define DEFAULT_SPELL_STATE 0x8100
#define AUTOCAST_SPELL_STATE 0xC100

class SERVER_DECL PetAI : public GuardianAI
{
protected:
	typedef std::map<AI_PetSpell*, uint16> SpellMap;
	typedef std::set<uint32> PassiveSpellMap;
	SpellMap m_autocastSpells;
	PassiveSpellMap m_passiveAuras;

	void Spell_cast();
public:
	PetAI(Unit * self, Unit * owner);
	~PetAI() {
		m_autocastSpells.clear();
	}
	
	/*	Fixed update frequency function */
	void OnUpdate();

	/*	Sets the pet action resulting in behavior defined by the action */
	void Pet_setaction(uint32 , Unit *);
	
	/*	Adds the specified ai spell into the spell list, this does not check if we alrdy have it. */
	void Spell_add(AI_Spell * newspell)
	{
		Arcemu::Util::ARCEMU_ASSERT(newspell != NULL && newspell->proto != NULL);
		m_autocastSpells.insert( make_pair( (AI_PetSpell*)newspell, DEFAULT_SPELL_STATE) );
	}

	/*	Adds the specified spell with the autocast either on or off, this doesn't check if we alrdy have it. */
	void Spell_add(AI_PetSpell * newspell, bool autocast = false)
	{
		if(newspell != NULL)
		{
			Arcemu::Util::ARCEMU_ASSERT(newspell->proto != NULL);
			if(!autocast)
				m_autocastSpells.insert( make_pair( (AI_PetSpell*)newspell, DEFAULT_SPELL_STATE) );
			//gota have a valid event to set to autocast.
			else if(newspell->event_type > 0)
				m_autocastSpells.insert( make_pair( (AI_PetSpell*)newspell, AUTOCAST_SPELL_STATE) );
		}
	}
	void Spell_addPassive(uint32 spell)
	{
		m_passiveAuras.insert(spell);
	}
	void Spell_removePassive(uint32 spell)
	{
		m_passiveAuras.erase(spell);
	}
	bool Spell_hasPassive(uint32 spell)
	{
		return (m_passiveAuras.find(spell) != m_passiveAuras.end() );
	}

	/*	Sets the pet state resulting in behavior defined by the state */
	void Pet_setstate(uint32 newState) 
	{
		m_PetState = newState;
	}
	void Spell_setautocast(AI_PetSpell * spell, bool on = true)
	{
		if(spell->event_type > 0 )
		{
			SpellMap::iterator itr = m_autocastSpells.find( (AI_PetSpell*)spell);
			if(itr != m_autocastSpells.end() )
			{
				if(on && itr->second != AUTOCAST_SPELL_STATE)
					itr->second = AUTOCAST_SPELL_STATE;
				else if(!on && itr->second != DEFAULT_SPELL_STATE)
					itr->second = DEFAULT_SPELL_STATE;
			}
		}
	}

	/*	Is the specified spell on autocast? */
	bool Spell_isautocast(AI_PetSpell * spell)
	{
		bool result = false;
		SpellMap::iterator itr = m_autocastSpells.begin();
		for(; itr != m_autocastSpells.end(); ++itr)
		{
			if( itr->first == (AI_PetSpell*)spell && itr->second == AUTOCAST_SPELL_STATE)
			{
				result = true;
				break;
			}
		}
		return result;
	}

	bool Spell_has(AI_Spell * spell)
	{
		return( m_autocastSpells.find( (AI_PetSpell*)spell) != m_autocastSpells.end() );
	}
	
	AI_Spell * Spell_findbyId(uint32 spell_id)
	{
		AI_Spell * ret = NULL;
		for(SpellMap::iterator itr = m_autocastSpells.begin(); itr != m_autocastSpells.end(); ++itr)
		{
			if( itr->first->proto->Id == spell_id)
			{
				ret = (AI_Spell *)itr->first;
				break;
			}
		}
		return ret;
	}
	
	/*	Spell retrievers */
	uint32 getSpellMapSize() { return (uint32)m_autocastSpells.size(); }
	SpellMap & getSpellMap() { return m_autocastSpells; }
	SpellMap::iterator getSpellMapStart() { return m_autocastSpells.begin(); }
	SpellMap::iterator getSpellMapEnd() { return m_autocastSpells.end(); }

	/*	Spell rank mutator */

	/*	Searches for the highest rank succeeding spell that we can have. */
	uint32 Spell_gethigherRank(uint32 spell)
	{
		SpellEntry * proto = dbcSpell.LookupEntryForced(spell);
		uint32 hRankSpell = 0;
		if(proto != NULL)
		{
			uint32 hRankThreshold = proto->baseLevel;
			uint32 mylvl = m_Self->getLevel();
			for(SpellMap::iterator itr = m_autocastSpells.begin(); itr != m_autocastSpells.end(); ++itr)
			{
				if(itr->first->proto->NameHash == proto->NameHash && itr->first->proto->baseLevel <= mylvl && itr->first->proto->baseLevel > hRankThreshold)
				{
					hRankThreshold = itr->first->proto->baseLevel;
					hRankSpell = itr->first->proto->Id;
				}
			}
		}
		return hRankSpell;
	}

	/*	Searches for the next preceding rank spell. */
	uint32 Spell_getlowerRank(uint32 spell)
	{
		SpellEntry * proto = dbcSpell.LookupEntryForced(spell);
		uint32 lowerRankSpell = 0;
		if( proto != NULL)
		{
			uint32 lowerrankthreshold = 1;
			uint32 mylvl = m_Self->getLevel();
			for(SpellMap::iterator itr = m_autocastSpells.begin(); itr != m_autocastSpells.end(); ++itr)
			{
				if(itr->first->proto->NameHash == proto->NameHash &&  itr->first->proto->baseLevel < mylvl && itr->first->proto->baseLevel <= lowerrankthreshold)
				{
					lowerRankSpell = itr->first->proto->Id;
					lowerrankthreshold = itr->first->proto->baseLevel;
				}
			}
		}
		return lowerRankSpell;
	}

	void Spell_handlevent(AiEvents);

	void HandleEvent(uint32 event, Unit* pUnit, uint32 misc1)
	{
		if(event != EVENT_ENTERWORLD)
			GuardianAI::HandleEvent(event, pUnit, misc1);
	}
#define AIType_isPet(aii) (aii->getAIType() == AITYPE_PET)
#define TO_AIPET(ai) static_cast<PetAI*>(ai)
};