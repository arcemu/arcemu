#include "StdAfx.h"

PetAI::PetAI(Unit * self, Unit * owner) : GuardianAI(self, owner)
{
	m_type = AITYPE_PET;
	m_PetState = PET_STATE_DEFENSIVE;
}
void PetAI::OnUpdate()
{
	updateMovement();
	if(!getTargettingDisable() )
		updateTargets();
	if(!getCombatDisable() )
		updateCombat();
	if(!getSpellDisable() )
		Spell_cast();
}
void PetAI::Spell_handlevent(AiEvents evt)
{
	uint32 now = getMSTime();
	//validate event type, cooldown and whether it is autocast.
	for(SpellMap::iterator itr = m_autocastSpells.begin(); itr != m_autocastSpells.end(); ++itr)
	{
		if(itr->second != AUTOCAST_SPELL_STATE || itr->first->isQueued || itr->first->event_type != (uint8)evt || itr->first->interval != 0xFFFFFFFF && now < itr->first->cooldown )
			continue;
		m_spellDeck.push_back( (AI_Spell*)itr->first );
		itr->first->isQueued = true;
	}
}
void PetAI::Pet_setaction(uint32 newAction, Unit * target)
{
	m_PetAction = newAction;
	if(newAction == PET_ACTION_ATTACK)
	{
		//can't attack nothing
		if(target == NULL)
			TO_PET(m_Self)->SendActionFeedback(PET_FEEDBACK_NOTHING_TO_ATTACK);
		//can't attack non hostiles..
		else if( !isAttackable(m_Self, target ) )
			TO_PET(m_Self)->SendActionFeedback(PET_FEEDBACK_CANT_ATTACK_TARGET);
		else
			//change the target.
			AttackReaction(target,0,0);
	}
	else if(newAction == PET_ACTION_FOLLOW)
		GuardianAI::HandleEvent(EVENT_FOLLOWOWNER, NULL, 0);
	else if(newAction == PET_ACTION_STAY)
	{
		//sit mode.
		StopMovement(0);
		setAIState(STATE_IDLE);
		if(m_Self->GetEmoteState() != EMOTE_STATE_SIT)
			m_Self->SetEmoteState(EMOTE_STATE_SIT);
	}
}
void PetAI::Spell_cast()
{
	if(m_spellDeck.size() && !m_Self->IsCasting() && !Behavior_has(BEHAVIOR_ISPROCESSINGAGENT) )
	{
		AI_PetSpell * top = (AI_PetSpell*)m_spellDeck.front();
		Unit * target = getNextTarget();
		if(target != NULL)
		{
			//check if we are inrange to cast it.
			float dist = m_Self->CalcDistance(target);
			float max_castrange = GetMaxRange( dbcSpellRange.LookupEntryForced( top->proto->rangeIndex) );
			if(dist > max_castrange)
			{
				calcDestinationAndMove(target, dist-max_castrange);
				uint32 time = (uint32)float2int32( abs(dist-max_castrange)/m_runSpeed);
				Behavior_setandclear(BEHAVIOR_ISPROCESSINGAGENT, time);
			}
			else
			{
				m_Self->CastSpell(target, top->proto->Id, false);
				if(top->interval != 0x0)
					top->cooldown = getMSTime() + top->interval;
				top->isQueued = false;
				m_spellDeck.pop_front();
			}
		}
	}
}
