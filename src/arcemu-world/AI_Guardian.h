#pragma once

class SERVER_DECL GuardianAI : public AIInterface
{
protected:
	typedef std::deque<AI_Spell*> SpellDeck;
	Unit * m_Owner;
	uint64 m_NextTarget;
	SpellDeck m_spellDeck;
	std::list<AI_Spell*> m_spells;
	uint32 m_PetAction;
	uint32 m_PetState;
public:
	GuardianAI(Unit * self, Unit * owner);
	~GuardianAI() 
	{
		for(std::list<AI_Spell*>::iterator itr = m_spells.begin(); itr != m_spells.end(); ++itr)
			delete (*itr);
		m_spells.clear();
		m_spellDeck.clear();
	}
	virtual void OnUpdate();
	void updateMovement();
	void updateCombat();
	void updateTargets();
	void updateSpells();
	/* Call in UpdateCombat, casts mNextSpell that is set from UpdateSpells. */
	void castNextSpell();
	/* Since guardians can't be controlled, getters return the default value and setters do nothing unless overridden */
	uint32 getPetAction() { return m_PetAction; }
	uint32 getPetState() { return m_PetState; }
	virtual void setPetAction(uint32 action) {}
	virtual void setPetState(uint32 state) {}

	Unit * getOwner() { return m_Owner; }
	void setOwner(Unit * nOwner) { m_Owner = nOwner; }

	void setNextTarget(Unit * newTarget);
	Unit * getNextTarget();

	AI_Spell * getNextSpell() {
		AI_Spell * ret = m_spellDeck.front();
		return ret;
	}
	Unit * getSpellTarget(AI_Spell*);
	//Used to forcibly override spells.
	void setNextSpell(AI_Spell * nSpell);
	void addSpell(AI_Spell* nSpell);
	void removeSpell(AI_Spell* oSpell);

	void AttackReaction(Unit * pUnit, uint32 damage_dealt, uint32 spellId);
	void HandleEvent(uint32 event, Unit* pUnit, uint32 misc1);
	void wipeReferences()
	{
		m_spellDeck.clear();
		m_NextTarget = 0;
		AIInterface::wipeReferences();
	}
#define TO_AIGUARDIAN(ai) static_cast<GuardianAI*>(ai)
};