/*
* ArcEmu MMORPG Server
* Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
* Copyright (C) 2008-2014 <http://www.ArcEmu.org/>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Affero General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Affero General Public License for more details.
*
* You should have received a copy of the GNU Affero General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
*/
#ifndef GO_FISHINGNODE_HPP
#define GO_FISHINGNODE_HPP
namespace Arcemu{
	////////////////////////////////////////////////////////////////////////
	//class GO_FishingNode
	// Implements Type 17 (FISHINGNODE) GameObjects
	//
	////////////////////////////////////////////////////////////////////////
	class GO_FishingNode : public GO_Lootable{
	public:
		GO_FishingNode();
		GO_FishingNode(uint64 GUID);
		~GO_FishingNode();
		///////////////////////////////////////////////////////////////////////
		//void UseFishingNode( Player *player )
		// Handles the click on the bobber, if there is a fish hooked
		// fills the loot, otherwise ends the fishing session
		//
		//Parameters
		// Player *player - The fisherman
		//
		//Return Value
		// None
		//
		//
		///////////////////////////////////////////////////////////////////////
		void UseFishingNode(Player *player);
		///////////////////////////////////////////////////////////////////////
		//void EndFishing( Player *player, bool abort )
		// Ends the fishing session. Despawns the bobber.
		//
		//Parameters
		// Player *player - The fisherman
		// bool abort - Indicates whether the fishing was a success or not
		//
		//
		////////////////////////////////////////////////////////////////////////
		void EndFishing(Player *player, bool abort);
		///////////////////////////////////////////////////////////////////////
		//void EventFishHooked()
		// Shows the splash effect of the fishing bobber and makes it lootable
		//
		//Paramaters
		// None
		//
		//Return Value
		// None
		//
		//
		///////////////////////////////////////////////////////////////////////
		void EventFishHooked();

		bool IsLootable(){ return true; }

		bool HasLoot();
	};
}
#endif