#include "StdAfx.h"

CritterAI::CritterAI(Unit * self) : AIInterface(self)
{
	maxtimetoflee = 0;
	m_type = AITYPE_CRITTER;
}
void CritterAI::OnUpdate() 
{
	if(!m_Self->isAlive() ) return;
	_UpdateMovement(AIINTERFACE_UPDATE_INTERVAL);
	handleMovement();
	Unit * target = getNextTarget();
	if(target != NULL)
	{
		maxtimetoflee += AIINTERFACE_UPDATE_INTERVAL;
		float dist = m_Self->CalcDistance(target);
		if(dist > 20.0f || maxtimetoflee >= 10000)
		{
			HandleEvent(EVENT_UNFEAR,NULL,0);
			MoveTo(m_Self->GetSpawnX(),m_Self->GetSpawnY(),m_Self->GetSpawnZ(),m_Self->GetSpawnO());
			maxtimetoflee = 0;
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
			setUnitToFear(pUnit);
			setAIState(STATE_FEAR);
			StopMovement(0);
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