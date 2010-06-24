#pragma once

class SERVER_DECL PetAI : public GuardianAI
{
public:
	PetAI(Unit * self, Unit * owner) : GuardianAI(self, owner)
	{
		m_type = AITYPE_PET;
	}
	void OnUpdate();
	void setPetAction(uint32 newAction);
	void setPetState(uint32 newState);
};