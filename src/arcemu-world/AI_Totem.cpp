
#include "StdAfx.h"
TotemAI::TotemAI(Unit * self, Unit * owner) : AIInterface(self)
{
	mOwner = owner;
	Movement_allowmovement(false);
	m_type = AITYPE_TOTEM;
}

void TotemAI::OnUpdate()
{
	UpdateSpells();
	if(m_spellQue.size() && !Behavior_has(BEHAVIOR_BLOCKSPELLSPAMMING) )
	{
		//Retrieve next spell to cast.
		AI_Spell * nextSpell = m_spellQue.front();
		//Retrieve a target based on thats spell.
		Unit * target = Spell_getarget(nextSpell);
		// Cast the spell on the target.
		if( handleSpellCast(nextSpell, target) )
		{
			if(nextSpell->maxcastcount)
				++nextSpell->currentcastcount;
			nextSpell->isQueued = false;
			//Handle random intervals.
			if(nextSpell->rdata != NULL)
			{
				uint32 range1 = nextSpell->rdata->interval[0];
				uint32 range2 = nextSpell->rdata->interval[1];
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
					nextSpell->cooldown = getMSTime() + newCd;
				}
			}
			else
				nextSpell->cooldown = getMSTime() + nextSpell->interval;
		}
		// Setup our spell spam block behavior.
		Behavior_setandclear(BEHAVIOR_BLOCKSPELLSPAMMING,500);
		// Remove spell from list.
		m_spellQue.pop_front();
	}
}