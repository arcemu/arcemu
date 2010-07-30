#include "StdAfx.h"

GuardianAI::GuardianAI(Unit * self, Unit * owner) : AIInterface(self)
{
	m_Owner = owner;
	m_NextTarget = 0;
	m_PetAction = PET_ACTION_FOLLOW;
	m_PetState = PET_STATE_AGGRESSIVE;
	m_type = AITYPE_GUARDIAN;
	setUnitToFollow(owner->GetGUID() );
	setFollowDistance(3); // 3 yards
	setFollowAngle( float(M_PI) );
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
		Spell_cast();
}
void GuardianAI::updateMovement()
{
	//Only handle unit to follow movement.
	_updateMovement(AIINTERFACE_UPDATE_INTERVAL);
	handleMovement();
	if(getUnitToFollow() != NULL && Pet_getaction() != PET_ACTION_STAY)
		handleFollowUnitMovement();
}
void GuardianAI::updateCombat()
{
	bool attackable = false;
	Unit * target = getNextTarget();
	attackable = isAttackable(m_Self,target);
	if(target != NULL && target->isAlive() && attackable && Pet_getstate() != PET_STATE_PASSIVE && getAIState() == STATE_ATTACKING)
	{
		//check and queue any spells that are cast on combat.
		Spell_handlevent(EVENT_INCOMBAT);
		/*If we have disabled melee, then we simply stand there facing our target, maybe we have spells to cast */
		if(!getMeleeDisable() ) 
		{
			//Position ourself correctly on the target.
			float dist = m_Self->CalcDistance(target);
			float combatReach[2];
			combatReach[0] = m_Self->GetModelHalfSize();
			combatReach[1] = calcCombatRange(target,false);
			if(dist < combatReach[0] || dist > combatReach[1] )
			{
				float dX = 0.5f;
				if( dist < combatReach[0] )
					dX = dist+combatReach[0];
				else
					dX = dist- combatReach[1];
				if(dX < 0.5f)
					dX = 0.5f;
				calcDestinationAndMove(target,dX);
			}
			else
			{
				if(m_Self->isReadyToAttack() ) 
				{
					if(!m_Self->isInFront(target) )
						m_Self->setInFront(target);
					m_Self->Strike(target,MELEE,NULL,0,0,0,false,false);
					m_Self->setAttackTimer();
				}
			}
					
		}
	}
	//If we are in an attacking state but we either killed off our target, we dont' have one, or he's not attackable...
	else if(getAIState() == STATE_ATTACKING)
	{
		//Let pets that are not ordered to sit follow the master.
		if(Pet_getaction() != PET_ACTION_STAY )
			HandleEvent(EVENT_FOLLOWOWNER,NULL,0);
		/* So we have been ordered to sit but only stop moving if we are moving in the 1st place. */
		else if(isMoving() )
		{
			StopMovement(0);
			//Make him sit visually xd
			if(m_Self->GetEmoteState() != EMOTE_STATE_SIT)
				m_Self->SetEmoteState(EMOTE_STATE_SIT);
			//reset state since we are now out of combat.
			setAIState(STATE_IDLE);
		}
		//we are leaving combat so update our spells.
		Spell_handlevent(EVENT_LEAVECOMBAT);
	}
	else
		//check and queue spells that require out of combat event.
		Spell_handlevent(EVENT_OUTCOMBAT);
}
void GuardianAI::updateTargets()
{
	Unit * target = getNextTarget();
	/* Search for new targets when we are following our master and we are on aggressive mode, but only if we don't have a target already */
	if(target == NULL && Pet_getstate() == PET_STATE_AGGRESSIVE && ( getAIState() == STATE_FOLLOWING || getAIState() == STATE_IDLE) )
	{
		/*Simply get the closest target to aggro. */
		target = findClosestTarget( true);
		if(target != NULL)
			AttackReaction(target,0,0);
	}
}
void GuardianAI::Spell_cast()
{
	uint8 state = getAIState();
	if(state == STATE_WANDER || state == STATE_FEAR || m_spellDeck.size() == 0 || Behavior_has(BEHAVIOR_BLOCKSPELLSPAMMING) || m_Self->IsCasting() )
		return;

	AI_Spell * nSpell = m_spellDeck.front();
	//Get the spell target.
	Unit * target = Spell_getarget(nSpell);
	//Then perform the spell.
	if( handleSpellCast(nSpell,target) )
	{
		if( nSpell->maxcastcount)
			++nSpell->currentcastcount;
		nSpell->isQueued = false;
		//Handle random intervals.
		if(nSpell->rdata != NULL)
		{
			uint32 range1 = nSpell->rdata->interval[0];
			uint32 range2 = nSpell->rdata->interval[1];
			if(range1 > 0 && range2 > 0 && range1 < range2)
			{
				//Lets deal in seconds.
				if(range1 >= 1000)
					range1 /= 1000;
				if(range2 >= 1000)
					range2 /= 1000;
				uint32 dT = range2-range1;
				//Get a random between (range1-range2)
				uint32 newCd = rand() % dT + range1;
				newCd *= 1000;
				nSpell->cooldown = getMSTime() + newCd;
			}
		}
		else
			nSpell->cooldown = getMSTime() + nSpell->interval;
	}
	Behavior_setandclear(BEHAVIOR_BLOCKSPELLSPAMMING, 1000);
	
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
		//set our state
		setAIState(STATE_ATTACKING);
		//enter combat would be here.
		Spell_handlevent(EVENT_ENTERCOMBAT);
		//face our target.
		setNextTarget(pUnit);
		//visual update
		m_Self->SetEmoteState(0);
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
		//We have to load our spells on world push since we have to be in world just incase we load passive spells.
	case EVENT_ENTERWORLD:
		{
			//For creatures we load spells from our proto.
			if(m_Self->IsCreature() )
			{
				CreatureProto * cn = TO_CREATURE(m_Self)->GetProto();
				Arcemu::Util::ARCEMU_ASSERT(cn != NULL);
				//Reserve storage space to prevent re-allocations.
				Spell_reserveStorage( (uint32)cn->spells.size() );
				//Add spells from our proto.
				for(std::list<AI_Spell*>::iterator itr = cn->spells.begin(); itr != cn->spells.end(); ++itr)
				{
					//For passive spells, simply cast them right away.
					if( (*itr)->proto->Attributes & ATTRIBUTES_PASSIVE)
						m_Self->CastSpell(m_Self, (*itr)->proto, false);
					else
						Spell_add( (*itr) );
				}
				Spell_handlevent(EVENT_ENTERWORLD);
			}
		}break;
	case EVENT_FEAR:
		if(pUnit != NULL)
		{
			setUnitToFear(pUnit);	//causes movement code to act accordingly.
			setNextTarget(pUnit); //We hate this guy now, so after unfear we switch to him.
			setAIState(STATE_FEAR);
			if(isMoving() )
				StopMovement(0);	//reset our movement bit on to begin the fear movement code
			disableSpells();	//disable any spell casting(even manually set ones)
		}
		break;
	case EVENT_UNFEAR:
		StopMovement(0);	//reset our movement bit
		AttackReaction(getNextTarget(),0,0);	//aggro the fear caster.
		disableSpells(false);	//reallow spells.
		break;
	case EVENT_WANDER:
		if(pUnit != NULL)
		{
			setAIState(STATE_WANDER);
			setNextTarget(pUnit);	//we hate this guy now
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
		//if we are in a defensive state, only then will we react to our owner being attacked but only if we are following or sitting.
		if( ( getAIState() == STATE_FOLLOWING || getAIState() == STATE_IDLE ) && Pet_getstate() == PET_STATE_DEFENSIVE)
			AttackReaction(pUnit,0,0);
		// cast the event spells.
		Spell_handlevent( EVENT_OWNERATTACKED);
		break;
	case EVENT_FOLLOWOWNER:
		//if we were attacking and we got forced to return, we fire the event leave combat
		if( getAIState() == STATE_ATTACKING)
			HandleEvent(EVENT_LEAVECOMBAT, m_Self, 0);
		setAIState(STATE_FOLLOWING);
		setNextTarget(NULL);
		setUnitToFollow(m_Owner->GetGUID() );
		setFollowAngle( float(M_PI));
		setFollowDistance(3);
		Spell_handlevent( EVENT_FOLLOWOWNER);
		break;
	case EVENT_TAUNTSTART:
		setForcedTarget(pUnit);
		Spell_handlevent( EVENT_TAUNTSTART);
		break;
	case EVENT_TAUNTEND:
		setForcedTarget(NULL);
		break;
	}
}

void GuardianAI::Spell_add( AI_Spell* nSpell )
{
	if(nSpell != NULL)
	{
		bool found = false;
		std::vector<AI_Spell*>::iterator itr = m_spells.begin();
		for(; itr != m_spells.end(); ++itr)
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
		
}

void GuardianAI::Spell_remove( AI_Spell* oSpell )
{
	if(oSpell != NULL)
	{
		std::vector<AI_Spell*>::iterator itr = m_spells.begin();
		for(; itr != m_spells.end(); ++itr)
		{
			if( (*itr) == oSpell)
			{
				m_spells.erase(itr);
				break;
			}
		}
	}
}
Unit * GuardianAI::Spell_getarget(AI_Spell* nSpell)
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
			(uint32)target->GetHealthPct() > hspell->heal_hp )
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
					if(target->m_auras[i] != NULL && (int)target->m_auras[i]->GetSpellProto()->DispelType == dspell->dispel_Type)
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