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

// polymorph spells ranks 1-4
#define SPELL_POLYMORPH_1 118
#define SPELL_POLYMORPH_2 12824
#define SPELL_POLYMORPH_3 12825
#define SPELL_POLYMORPH_4 12826

class FragmentedMagic : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(FragmentedMagic);
		FragmentedMagic(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			RegisterAIUpdateEvent(5000);
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
			// search for the one of the polymorph auras and its caster
			Player* p_caster = NULL;
			for(uint32 i = MAX_TOTAL_AURAS_START; i < MAX_TOTAL_AURAS_END; i++)
			{
				if(_unit->m_auras[i] != NULL)
				{
					if(_unit->m_auras[i]->GetSpellId() == SPELL_POLYMORPH_1 ||
					   _unit->m_auras[i]->GetSpellId() == SPELL_POLYMORPH_2 ||
					   _unit->m_auras[i]->GetSpellId() == SPELL_POLYMORPH_3 ||
					   _unit->m_auras[i]->GetSpellId() == SPELL_POLYMORPH_4)
					{
						if(p_caster = _unit->m_auras[i]->GetPlayerCaster())
						{
							// wanted aura and its caster have been found.
							break;
						}
						else
						{
							// so the caster was not a player. Should there be searched more? Does this ever happen?
							// For now just return
							return;
						}
					}
				}
			}

			if( p_caster == NULL )
				return;

			if(!p_caster->HasQuest(9364))
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
