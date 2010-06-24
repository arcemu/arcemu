#include "StdAfx.h"

void PetAI::OnUpdate()
{
	updateMovement();
	if(!getTargettingDisable() )
		updateTargets();
	if(!getCombatDisable() )
		updateCombat();
	AI_Spell * autoCastSpell =  TO_PET(m_Self)->HandleAutoCastEvent();
	//Handling for auto cast spells.
	if(autoCastSpell != NULL)
		m_spellDeck.push_back(autoCastSpell);
	if(!getSpellDisable() )
		castNextSpell();
}
void PetAI::setPetAction(uint32 newAction) { m_PetAction = newAction; }
void PetAI::setPetState(uint32 newState) { m_PetState = newState; }