#pragma once
class SERVER_DECL TotemAI : public AIInterface
{
protected:
	Unit * mOwner;
	AI_Spell * tSpells[5];
	AI_Spell * m_nextSpell;
	/*	Since we can override the totems spell with a spell allocated from somewhere else
		we create an internal copy.
	*/
	bool deleteNextSpell;
public:
	TotemAI(Unit * self, Unit * owner);
	void OnUpdate();
	void UpdateSpells(uint32);
	void CastSpell();
	void addSpell(AI_Spell * nSpell); 
	void removeSpell(AI_Spell * nSpell);
	AI_Spell * getNextSpell() { return m_nextSpell; }
	void setNextSpell(AI_Spell* n)
	{
		if(n == NULL) return;
		m_nextSpell = new AI_Spell;
		memcpy(m_nextSpell,n,sizeof(AI_Spell) );
		//Set this to true so that we delete the spell after casting it.
		deleteNextSpell = true;
	}
};