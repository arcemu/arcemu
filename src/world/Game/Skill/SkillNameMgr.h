/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2012 <http://www.ArcEmu.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef SKILL_NAME_MGR_H
#define SKILL_NAME_MGR_H

#include "StdAfx.h"

enum SpellTreeName
{

    SPELLTREE_MAGE_ARCANE						= 237,
    SPELLTREE_MAGE_FIRE							= 8,
    SPELLTREE_MAGE_FROST 						= 6,

    SPELLTREE_ROGUE_ASSASSINATION				= 253,
    SPELLTREE_ROGUE_COMBAT						= 38,
    SPELLTREE_ROGUE_SUBTLETY					= 39,

    SPELLTREE_WARLOCK_AFFLICTION				= 355,
    SPELLTREE_WARLOCK_DEMONOLOGY				= 354,
    SPELLTREE_WARLOCK_DESTRUCTION				= 593,

    SPELLTREE_WARRIOR_ARMS						= 26,
    SPELLTREE_WARRIOR_FURY						= 256,
    SPELLTREE_WARRIOR_PROTECTION				= 257,

    SPELLTREE_SHAMAN_ELEMENTAL					= 375,
    SPELLTREE_SHAMAN_ENHANCEMENT				= 373,
    SPELLTREE_SHAMAN_RESTORATION				= 374,

    SPELLTREE_PALADIN_HOLY						= 594,
    SPELLTREE_PALADIN_PROTECTION				= 267,
    SPELLTREE_PALADIN_RETRIBUTION 				= 184,

    SPELLTREE_DEATHKNIGHT_BLOOD					= 770,
    SPELLTREE_DEATHKNIGHT_FROST					= 771,
    SPELLTREE_DEATHKNIGHT_UNHOLY				= 772,

    SPELLTREE_PRIEST_DISCIPLINE					= 613,
    SPELLTREE_PRIEST_HOLY						= 56,
    SPELLTREE_PRIEST_SHADOW						= 78,

    SPELLTREE_HUNTER_BEASTMASTERY				= 50,
    SPELLTREE_HUNTER_MARKSMANSHIP				= 163,
    SPELLTREE_HUNTER_SURVIVAL					= 51,

    SPELLTREE_DRUID_BALANCE						= 574,
    SPELLTREE_DRUID_FERAL_COMBAT				= 134,
    SPELLTREE_DRUID_RESTORATION					= 573,
};

class SkillNameMgr
{
	public:
		char** SkillNames;
		uint32 maxskill;

		SkillNameMgr()
		{
			DBCStorage< skilllineentry >::iterator itr = dbcSkillLine.end();
			--itr;
			skilllineentry* skillline = *itr;

			//This will become the size of the skill name lookup table
			maxskill = skillline->id;

			//SkillNames = (char **) malloc(maxskill * sizeof(char *));
			SkillNames = new char * [maxskill + 1]; //(+1, arrays count from 0.. not 1.)
			memset(SkillNames, 0, (maxskill + 1) * sizeof(char*));

			for(itr = dbcSkillLine.begin(); itr != dbcSkillLine.end(); ++itr)
			{
				skillline = *itr;

				unsigned int SkillID = skillline->id;
				const char* SkillName = skillline->Name;

				SkillNames[SkillID] = new char [strlen(SkillName) + 1];
				//When the DBCFile gets cleaned up, so does the record data, so make a copy of it..
				memcpy(SkillNames[SkillID], SkillName, strlen(SkillName) + 1);
			}
		}
		~SkillNameMgr()
		{
			for(uint32 i = 0; i <= maxskill; i++)
			{
				if(SkillNames[i] != 0)
					delete[] SkillNames[i];
			}
			delete[] SkillNames;
		}
};
#endif

