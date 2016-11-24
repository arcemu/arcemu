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
#ifndef GO_FISHINGHOLE_HPP
#define GO_FISHINGHOLE_HPP
namespace Arcemu{
	////////////////////////////////////////////////////////////////////////
	//class GO_FishingHole
	// Class implementing Type 25 (Fishinghole) GameObject
	//
	//
	///////////////////////////////////////////////////////////////////////
	class GO_FishingHole : public GO_Lootable{
	public:
		GO_FishingHole();
		GO_FishingHole(uint64 GUID);
		~GO_FishingHole();
		void InitAI();
		bool IsLootable(){ return true; }
		bool HasLoot();
		///////////////////////////////////////////////////////////////////
		//bool CanFish()
		// Tells if the Object is still usable for fishing (still has fish)
		//
		//Parameters
		// None
		//
		//Return Value
		// Returns true if it is still usable.
		// Returns false otherwise.
		//
		//
		///////////////////////////////////////////////////////////////////
		bool CanFish();
		///////////////////////////////////////////////////////////////////
		//void CatchFish()
		// Decreases the number of times the GO is fishable
		//
		//Parameters
		// None
		//
		//Return Value
		// None
		//
		//
		///////////////////////////////////////////////////////////////////
		void CatchFish();
		///////////////////////////////////////////////////////////////////
		//void CalcFishRemaining( bool force )
		// Sets the number of times the GO can be fished
		//
		//Parameters
		// bool force - If this is true, the value will be forcably
		// regenerated.
		//
		//Return Value
		// None
		//
		//
		//////////////////////////////////////////////////////////////////
		void CalcFishRemaining(bool force);
	private:
		uint32 usage_remaining;
	};
}
#endif