#include "StdAfx.h"

GuardianAI::GuardianAI(Unit * self, Unit * owner) : AIInterface(self)
{
	m_Owner = owner;
	m_NextTarget = 0;
	m_PetAction = PET_ACTION_FOLLOW;
	m_PetState = PET_STATE_AGGRESSIVE;
	m_type = AITYPE_GUARDIAN;
}
void GuardianAI::OnUpdate()
{
	updateMovement();
	if(!getTargettingDisable() )
		updateTargets();
	if(!getCombatDisable() )
		updateCombat();
	//Cast any pending spells if we are allowed to cast.
	if(!getSpellDisable() )
		castNextSpell();
}
void GuardianAI::updateMovement()
{
	//Only handle unit to follow movement.
	_UpdateMovement(AIINTERFACE_UPDATE_INTERVAL);
	handleFollowUnitMovement();
}
void GuardianAI::updateCombat()
{
	bool attackable = false;
	Unit * target = getNextTarget();
	attackable = isAttackable(m_Self,target);
	if(target != NULL && target->isAlive() && attackable && getAIState() == STATE_ATTACKING)
	{
		/*If we have disabled melee, then we simply stand there facing our target, maybe we have spells to cast */
		if(!getMeleeDisable() ) 
		{
			float dist = m_Self->CalcDistance(target);
			float reach = calcCombatRange(target,false);
			if(dist <= reach )
			{
				if(m_Self->isReadyToAttack() ) 
				{
					if(!m_Self->isInFront(target) )
						m_Self->setInFront(target);
					m_Self->Strike(target,MELEE,NULL,0,0,0,false,false);
					m_Self->setAttackTimer();
				}
			}
			else
				calcDestinationAndMove(target,reach);	
		}
		//Only update spells while in combat and if we don't have spell casting disabled.
		if(!getSpellDisable() )
			updateSpells();
	}
	else
	{
		uint8 state = getAIState();
		if(!attackable && state == STATE_ATTACKING)
			HandleEvent(EVENT_FOLLOWOWNER,NULL,0);
		if(target == NULL)
		{
			switch(state)
			{
			case STATE_FOLLOWING:
			case STATE_FEAR:
			case STATE_WANDER:
				break;
			default:
				//Pets that are ordered to sit.
				if(getPetAction() != PET_ACTION_STAY )
					HandleEvent(EVENT_FOLLOWOWNER,NULL,0);
				else
					StopMovement(0);
			}
			
		}

	}
}
void GuardianAI::updateTargets()
{
	Unit * target = getNextTarget();
	/* Search for new targets when we are following our master and we are on aggressive mode, but only if we don't have a target already */
	if(target == NULL && getPetState() == PET_STATE_AGGRESSIVE && getAIState() == STATE_FOLLOWING)
	{
		/*Simply get the closest target to aggro. */
		target = findClosestTarget( true);
		if(target != NULL)
			AttackReaction(target,0,0);
	}
}
void GuardianAI::updateSpells()
{
	AI_Spell * curSpell = NULL;
	for(std::list<AI_Spell*>::iterator itr = m_spells.begin(); itr != m_spells.end(); ++itr)
	{
		curSpell = (*itr);
		if(curSpell == NULL ) continue;
		if(curSpell->maxcastcount != -1 && curSpell->currentcastcount >= curSpell->maxcastcount) continue;
		if(curSpell->cooldown >= getMSTime() )
			m_spellDeck.push_back(curSpell);
	}
}
void GuardianAI::castNextSpell()
{
	uint8 state = getAIState();
	if(state == STATE_WANDER || state == STATE_FEAR || m_spellDeck.size() == 0)
		return;

	AI_Spell * nSpell = m_spellDeck.front();
	//Get the spell target.
	Unit * target = getSpellTarget(nSpell);
	//Then perform the spell.
	handleSpellCast(nSpell,target);
	m_spellDeck.pop_front();
}
void GuardianAI::setNextTarget(Unit * newTarget) 
{
	if(newTarget) 
	{
		m_Self->SetTargetGUID(newTarget->GetGUID());
		m_NextTarget = newTarget->GetGUID();
	}
	else
	{
		m_NextTarget = 0;
		m_Self->SetTargetGUID(0);
	}
}
Unit * GuardianAI::getNextTarget() 
{
	Unit * target = m_Self->GetMapMgr()->GetUnit(m_ForcedTarget);
	if(target == NULL)
		target = m_Self->GetMapMgr()->GetUnit(m_NextTarget);
	return target;
}
void GuardianAI::AttackReaction(Unit * pUnit, uint32 damage_dealt, uint32 spellId)
{
	if(pUnit != NULL && pUnit->isAlive() && isAttackable(m_Self,pUnit,true) )
	{
		setAIState(STATE_ATTACKING);
		setNextTarget(pUnit);
		if(pUnit->IsPlayer()  )
		{
			WorldPacket data(SMSG_AI_REACTION,12);
			data << m_Self->GetGUID();
			data << uint32(2);
			TO_PLAYER(pUnit)->SendPacket(&data);
		}
	}
}
void GuardianAI::HandleEvent(uint32 event, Unit* pUnit, uint32 misc1)
{
	switch(event)
	{
	case EVENT_FEAR:
		if(pUnit != NULL)
		{
			setUnitToFear(pUnit);
			setNextTarget(pUnit); //We can attack this guy after being unfeared.
			setAIState(STATE_FEAR);
			StopMovement(0);
			disableSpells();
		}
		break;
	case EVENT_UNFEAR:
		setAIState(STATE_IDLE);
		StopMovement(0);
		AttackReaction(getNextTarget(),0,0);
		disableSpells(false);
		break;
	case EVENT_WANDER:
		if(pUnit != NULL)
		{
			setAIState(STATE_WANDER);
			setNextTarget(pUnit);
			StopMovement(0);
			disableSpells();
		}
		break;
	case EVENT_UNWANDER:
		setAIState(STATE_IDLE);
		StopMovement(0);
		AttackReaction(getNextTarget(),0,0);
		disableSpells(false);
		break;
	case EVENT_OWNERATTACKED:
		if(getAIState() == STATE_FOLLOWING && getPetState() == PET_STATE_DEFENSIVE)
			AttackReaction(pUnit,0,0);
		break;
	case EVENT_FOLLOWOWNER:
		setAIState(STATE_FOLLOWING);
		setNextTarget(NULL);
		setUnitToFollow(m_Owner->GetGUID() );
		setFollowAngle( float(M_PI));
		setFollowDistance(3);
		break;
	case EVENT_TAUNTSTART:
		setForcedTarget(pUnit);
		break;
	case EVENT_TAUNTEND:
		setForcedTarget(NULL);
		break;
	}
}

void GuardianAI::addSpell( AI_Spell* nSpell )
{
	if(nSpell != NULL)
	{
		bool found = false;
		std::list<AI_Spell*>::iterator itr = m_spells.begin();
		for(; itr != m_spells.end(); ++itr)
		{
			if( (*itr)->sEntry == nSpell->sEntry)
			{
				found = true;
				break;
			}
		}
		if(!found)
		{
			AI_Spell * copy = new AI_Spell;
			memcpy(copy,nSpell,sizeof(AI_Spell) );
			m_spells.push_back(copy);
		}
	}
}

void GuardianAI::removeSpell( AI_Spell* oSpell )
{
	if(oSpell != NULL)
	{
		std::list<AI_Spell*>::iterator itr = m_spells.begin();
		for(; itr != m_spells.end(); ++itr)
		{
			if( (*itr)->sEntry == oSpell->sEntry)
			{
				m_spells.erase(itr);
				break;
			}
		}
	}
}

void GuardianAI::setNextSpell( AI_Spell * nSpell )
{
	if(nSpell != NULL && nSpell->sEntry != NULL)
	{
		AI_Spell * spell = NULL;
		for(std::list<AI_Spell*>::iterator itr = m_spells.begin(); itr != m_spells.end(); ++itr)
		{
			spell = (*itr);
			if(spell->sEntry == nSpell->sEntry)
			{
				m_spellDeck.push_back(spell);
				break;
			}
		}
	}
}
Unit * GuardianAI::getSpellTarget(AI_Spell* nSpell)
{
	Unit * target = getNextTarget();
	if(nSpell->type == AISPELLTYPE_HEAL)
	{
		AI_HealSpell * hspell = static_cast<AI_HealSpell*>(nSpell);
		if(target == NULL && isHostile(m_Self,target) )
		{
			//When we are attacking, targetting ourself by default, else our master.
			if(getAIState() == STATE_ATTACKING)
				target = m_Self;
			else
				target = m_Owner;
		}
		float dist = m_Self->CalcDistance(target);
		/* If we are too far, we simply move to the max range area and try to cast. */
		if(nSpell->minrange > 0 && dist < nSpell->minrange ||
			nSpell->maxrange > 0 && dist > nSpell->maxrange ||
			target->GetHealthPct() > hspell->heal_hp )
		{	// Check health requirements, if we fail the hp requirements, try again in 2secs.
			if(target->GetHealthPct() > hspell->heal_hp)
			{
				hspell->cooldown = getMSTime() + 2000;
				target = NULL;
			}
		}
	}
	//Dispel types.
	else if(nSpell->type == AISPELLTYPE_DISPEL)
	{
		AI_DispelSpell * dspell = static_cast<AI_DispelSpell*>(nSpell);
		//If we have a specific dispel type, we can prevent casting of this spell if we don't have any of the debuff.s
		if(dspell->dispel_Type != DISPEL_NULL)
		{
			uint32 start,end = 0;
			bool aura_found = false;
			//If the target is friendly, then we check for negative auras.
			if(isFriendly(m_Self,target) )
			{
				start = MAX_NEGATIVE_AURAS_EXTEDED_START;
				end = MAX_NEGATIVE_AURAS_EXTEDED_END;
			}
			else
			{
				start = MAX_POSITIVE_AURAS_EXTEDED_START;
				end = MAX_POSITIVE_AURAS_EXTEDED_END;
			}
			if(dspell->dispel_Type == DISPEL_ALL)
			{
				for(uint32 i = start; i < end; ++i)
				{
					if(target->m_auras[i] != NULL && target->m_auras[i]->GetSpellProto()->DispelType != DISPEL_NULL)
					{
						aura_found = true;
						break;
					}
				}
			}
			else
			{
				for(uint32 i = start; i < end; ++i)
				{
					if(target->m_auras[i] != NULL && target->m_auras[i]->GetSpellProto()->DispelType == dspell->dispel_Type)
					{
						aura_found = true;
						break;
					}
				}
			}
			if(!aura_found)
			{
				//No aura w/ dispel type? add a 5 second cd and continue;
				dspell->cooldown = getMSTime() + 5000;
				target = NULL;
			}
		}
	}
	else
	{
		switch(nSpell->spellTargeType)
		{
		case TTYPE_SELF:
		case TTYPE_SELF_DESTINATION:
			target = m_Self;
			break;
		case TTYPE_OWNER:
			target = m_Owner;
			break;
		case TTYPE_CURRENTTARGET:
		case TTYPE_CURRENTTARGET_DESTINATION:
			//Set to current target but our target is null.
			if(target == NULL)
				target = findClosestTarget();
			break;
		case TTYPE_RANDOMTARGET:
		case TTYPE_RANDOMTARGET_DESTINATION:
			target = findRandomTarget();
			break;
		}
		if(target != NULL) 
		{
			float dist = m_Self->CalcDistance(target);
			if(nSpell->maxrange > 0 && dist > nSpell->maxrange)
			{
				calcDestinationAndMove(target,nSpell->maxrange);
				target = NULL;
			}
		}
	}
	return target;
}