
#include "StdAfx.h"

TotemAI::TotemAI(Unit * self, Unit * owner) : AIInterface(self)
{
	mOwner = owner;
	memset(tSpells,NULL,sizeof(AI_Spell*)*5);
	setAIMoveFlags(AIMOVESTATE_CANTMOVE);
	m_type = AITYPE_TOTEM;
}

void TotemAI::OnUpdate()
{
	UpdateSpells(AIINTERFACE_UPDATE_INTERVAL);
	if(m_nextSpell != NULL)
		CastSpell();
}
void TotemAI::CastSpell()
{
	Unit * target = NULL;
	/* Cast buff spells immediately */
	if(m_nextSpell->type == AISPELLTYPE_BUFF)
	{
		/*Make the owner our default target */
		Unit * target = mOwner;
		if(m_nextSpell->spellTargeType == TTYPE_SELF || m_nextSpell->spellTargeType == TTYPE_SELF_DESTINATION)
			target = m_Self;
		if(target != NULL && !target->HasAura(m_nextSpell->sEntry->Id) )
			m_Self->CastSpell(target,m_nextSpell->sEntry,false);
	}
	else
	{
		switch(m_nextSpell->spellTargeType)
		{
		case TTYPE_SELF_DESTINATION:
		case TTYPE_SELF:
			target = m_Self;
			break;
		case TTYPE_OWNER:
			target = mOwner;
			break;
		case TTYPE_RANDOMTARGET:
		case TTYPE_RANDOMTARGET_DESTINATION:
			target = findRandomTarget();
			break;
		case TTYPE_CURRENTTARGET:
		case TTYPE_CURRENTTARGET_DESTINATION:
			target = findClosestTarget();
			break;
		}
		if(target != NULL)
		{
			if(m_nextSpell->spellTargeType == TTYPE_SELF_DESTINATION || m_nextSpell->spellTargeType == TTYPE_CURRENTTARGET_DESTINATION 
				|| m_nextSpell->spellTargeType == TTYPE_RANDOMTARGET_DESTINATION )
				m_Self->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(),m_nextSpell->sEntry,false);
			else
				m_Self->CastSpell(target,m_nextSpell->sEntry,false);
			//If it is a temporary spell, then simply  cast it once and forget it.
			if(deleteNextSpell )
			{
				delete m_nextSpell;
				deleteNextSpell = false;
			}
			else
			{
				/* After casting, we update the values and clear our spell to allow for new */
				if(m_nextSpell->maxcastcount != -1)
					++m_nextSpell->currentcastcount;
				m_nextSpell->cooldown = m_nextSpell->interval;
			}
			m_nextSpell = NULL;
		}
	}
}

void TotemAI::addSpell( AI_Spell * nSpell )
{
	for(int i = 0; i < 5; ++i)
	{
		if(tSpells[i] == NULL)
		{
			tSpells[i] = new AI_Spell;
			memcpy(tSpells[i],nSpell,sizeof(AI_Spell) );
			break;
		}
		//Spell is in the list already.
		else if(nSpell->sEntry == tSpells[i]->sEntry)
			break;
	}
}

void TotemAI::removeSpell( AI_Spell * nSpell )
{
	if(nSpell != NULL)
	{
		for(int i = 0; i < 5; ++i)
		{
			if(tSpells[i] != NULL && tSpells[i]->sEntry == nSpell->sEntry)
			{
				//De-allocate memory and free up that slot.
				delete tSpells[i];
				tSpells[i] = NULL;
				break;
			}
		}
	}
}

void TotemAI::UpdateSpells( uint32 timeDiff)
{
	AI_Spell * spell = NULL;
	for(int i = 0; i < 5; ++i)
	{
		spell = tSpells[i];
		if(spell != NULL)
		{
			if(spell->maxcastcount != -1 && spell->maxcastcount == spell->currentcastcount) continue;
			if(spell->interval )
			{
				if(timeDiff > spell->cooldown)
					spell->cooldown = 0;
				else
					spell->cooldown -= timeDiff;
				if(!spell->cooldown && m_nextSpell == NULL)
					m_nextSpell = spell;
			}
		}
	}
}