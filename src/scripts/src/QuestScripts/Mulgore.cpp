/*
 * WEmu Scripts for WEmu MMORPG Server
 * Copyright (C) 2008 WEmu Team
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

#include "StdAfx.h"
#include "Setup.h"
#include "../EAS/EasyFunctions.h"

bool InducingVision(uint32 i, Spell * pSpell)
{
		if(!pSpell->p_caster) return true;
		Player *mTarget = pSpell->p_caster;
		if( mTarget == NULL || mTarget->GetMapMgr() == NULL || mTarget->GetMapMgr()->GetInterface() == NULL )
			return true;
		  Creature *creat = mTarget->GetMapMgr()->GetInterface()->SpawnCreature(2983, -2238.994873f, -408.009552f, -9.424423f, 5.753043f, true, false, 0, 0);
		  creat->GetAIInterface()->setMoveType(11);

		  sEAS.CreateCustomWaypointMap(creat);

		  sEAS.WaypointCreate(creat,-2240.521729f, -407.114532f, -9.424648f, 5.753043f, 0, 256, 0);
		  sEAS.WaypointCreate(creat,-2225.764404f, -419.251404f, -9.360950f, 5.575544f, 0, 256, 0);
		  sEAS.WaypointCreate(creat,-2200.883789f, -440.999634f, -5.606637f, 5.599899f, 0, 256, 0);
		  sEAS.WaypointCreate(creat,-2143.712646f, -468.068665f, -9.401191f, 0.797177f, 0, 256, 0);
		  sEAS.WaypointCreate(creat,-2100.811279f, -420.980194f, -5.322747f, 0.918914f, 0, 256, 0);
		  sEAS.WaypointCreate(creat,-2079.469482f, -392.465576f, -10.262321f, 0.930689f, 0, 256, 0);
		  sEAS.WaypointCreate(creat,-2043.699707f, -343.802155f, -6.971242f, 0.934694f, 0, 256, 0);
		  sEAS.WaypointCreate(creat,-2001.858521f, -242.533966f, -10.763200f, 0.952265f, 0, 256, 0);
		  sEAS.WaypointCreate(creat,-1924.751343f, -119.969292f, -11.770116f, 0.754775f, 0, 256, 0);
		  sEAS.WaypointCreate(creat,-1794.804565f, -7.919222f, -9.326016f, 0.711578f, 0, 256, 0);
		  sEAS.WaypointCreate(creat,-1755.206421f, 72.430847f, 1.121445f, 1.103484f, 0, 256, 0);
		  sEAS.WaypointCreate(creat,-1734.550049f, 116.842003f, -4.337420f, 3.350100f, 0, 256, 0);
		  sEAS.WaypointCreate(creat,-1720.041870f, 125.933235f, -2.325089f, 0.903206f, 0, 256, 0);
		  sEAS.WaypointCreate(creat,-1704.406860f, 183.596405f, 12.065074f, 0.691148f, 0, 256, 0);
		  sEAS.WaypointCreate(creat,-1674.317261f, 201.597565f, 11.235887f, 0.475164f, 0, 256, 0);
		  sEAS.WaypointCreate(creat,-1624.068726f, 223.555389f, 2.074259f, 0.416259f, 0, 256, 0);
		  sEAS.WaypointCreate(creat,-1572.863403f, 234.714813f, 2.305119f, 0.781471f, 0, 256, 0);
		  sEAS.WaypointCreate(creat,-1542.866943f, 277.896759f, 20.543165f, 1.256637f, 0, 256, 0);
		  sEAS.WaypointCreate(creat,-1541.813232f, 316.415649f, 49.910889f, 1.248783f, 0, 256, 0);
		  sEAS.WaypointCreate(creat,-1526.979126f, 329.664001f, 61.835552f, 1.182024f, 0, 256, 0);
		  sEAS.WaypointCreate(creat,-1524.173584f, 335.237610f, 63.325703f, 1.173092f, 0, 256, 0);
		  sEAS.WaypointCreate(creat,-1513.968506f, 355.759338f, 63.064487f, 1.119193f, 0, 256, 0);
		  return true;
}


		
class The_Plains_Vision : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(The_Plains_Vision);
	The_Plains_Vision(Creature* pCreature) : CreatureAIScript(pCreature) {}

	void OnReachWP(uint32 iWaypointId, bool bForwards)
	{
		if(iWaypointId == 2)
			_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You follow me.");
		if(iWaypointId == 22)
		{
			sEAS.DeleteWaypoints(_unit);
			_unit->Despawn(500,0);
		}
	}
};

void SetupMulgore(ScriptMgr * mgr)
{
	mgr->register_creature_script(2983, &The_Plains_Vision::Create);
	mgr->register_dummy_spell(4981, &InducingVision);
}
