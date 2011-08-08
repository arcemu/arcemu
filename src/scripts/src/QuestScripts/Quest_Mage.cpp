/*
 * ArcScripts for ArcEmu MMORPG Server
 * Copyright (C) 2009 ArcEmu Team <http://www.arcemu.org/>
 * Copyright (C) 2008-2009 Sun++ Team <http://www.sunscripting.com/>
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2007-2008 Moon++ Team <http://www.moonplusplus.info/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Setup.h"

class FragmentedMagic : public CreatureAIScript
{
	private:
		uint32 current_aura;
	public:
		ADD_CREATURE_FACTORY_FUNCTION(FragmentedMagic);
		FragmentedMagic(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			RegisterAIUpdateEvent(5000);
			current_aura = 0;
		}

		void SetWander(Creature* m_target, Player* p_caster)
		{
			m_target->m_special_state |= UNIT_STATE_CONFUSE;
			m_target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_CONFUSED);

			m_target->EnableAI();
			m_target->GetAIInterface()->HandleEvent(EVENT_WANDER, p_caster, 0);
		}

		void AIUpdate()
		{
			bool auraOk = false;
			const uint32 auras[] = {118, 12824, 12825, 12826}; // Polymorph rank 1,2,3,4

			for(int i = 0; i < 4; i++)
			{
				if(_unit->HasAura(auras[i]))
				{
					current_aura = auras[i];
					auraOk = true;

					break;
				}
			}

			if(!auraOk)
				return;

			bool casterOk = false;
			Player* p_caster;

			for(int i = 0; i < 100; i++) //random number - azolex(should be max aura fields, no idea and dont care)
			{
				if(_unit->m_auras[i] == NULL)
					continue;

				if(_unit->m_auras[i]->GetSpellId() == current_aura)
				{
					if(!_unit->m_auras[i]->GetCaster()->IsPlayer())
						break;

					p_caster = TO_PLAYER(_unit->m_auras[i]->GetCaster());

					if(p_caster == NULL)
						break;

					casterOk = true;

					break;
				}
			}

			if(!casterOk)
				return;

			QuestLogEntry* qle = p_caster->GetQuestLogForEntry(9364);
			if(qle == NULL)
				return;

			_unit->Despawn(1, 1 * 60 * 1000);

			uint8 num = RandomUInt(5);
			for(int i = 0; i < num; i++)
			{
				Creature* cr = sEAS.SpawnCreature(p_caster, 16479, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 0, 1 * 60 * 1000);
				SetWander(cr, p_caster);
			}
		}
};

void SetupMage(ScriptMgr* mgr)
{
	mgr->register_creature_script(6193, &FragmentedMagic::Create);
	mgr->register_creature_script(6194, &FragmentedMagic::Create);
	mgr->register_creature_script(6190, &FragmentedMagic::Create);
	mgr->register_creature_script(6196, &FragmentedMagic::Create);
}
