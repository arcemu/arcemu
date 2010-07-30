#include "StdAfx.h"

CritterAI::CritterAI(Unit * self) : AIInterface(self)
{
	panicTime = 0;
	m_type = AITYPE_CRITTER;
}
void CritterAI::OnUpdate() 
{
	if(m_Self->isAlive() )
	{
		_updateMovement(AIINTERFACE_UPDATE_INTERVAL);
		handleMovement();
		Unit * target = getNextTarget();
		if(target != NULL)
		{
			if( getMSTime() >= panicTime || m_Self->CalcDistance(target) > 30.0f )
			{
				HandleEvent(EVENT_UNFEAR,NULL,0);
				MoveTo(m_Self->GetSpawnX(),m_Self->GetSpawnY(),m_Self->GetSpawnZ(),m_Self->GetSpawnO());
				panicTime = 0;
				setNextTarget(NULL);
			}
		}
	}
}
void CritterAI::HandleEvent(uint32 event, Unit* pUnit, uint32 misc1)
{
	switch(event)
	{
	case EVENT_FEAR:
		if(pUnit != NULL)
		{
			//Flag our unit to fear.
			setUnitToFear(pUnit);
			// Set the state so that it will start automatically running away.
			setAIState(STATE_FEAR);
			// Reset movement bit so that fear movement code will register.
			StopMovement(0);
			//Set our panic time to the panic duration from DB.
			panicTime = getMSTime() + getPanicDuration();
		}
		break;
	case EVENT_UNFEAR:
		setAIState(STATE_IDLE);
		setUnitToFear(NULL);
		StopMovement(0);
		break;
	case EVENT_WANDER:
		setAIState(STATE_WANDER);
		StopMovement(0);
		break;
	case EVENT_UNWANDER:
		setAIState(STATE_IDLE);
		StopMovement(0);
		break;
	}
}
Unit * CritterAI::getNextTarget() 
{
	return m_Self->GetMapMgr()->GetUnit(m_Self->GetTargetGUID());
}
void CritterAI::setNextTarget(Unit * target) 
{
	if(target)
		m_Self->SetTargetGUID(target->GetGUID());
	else
		m_Self->SetTargetGUID(0);
}