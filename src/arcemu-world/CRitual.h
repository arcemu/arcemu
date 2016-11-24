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

#ifndef RITUAL_HPP
#define RITUAL_HPP

namespace Arcemu{

	////////////////////////////////////////////////////////////////////////
	//class CRitual
	// Class that implements Summoning Rituals
	//
	//
	///////////////////////////////////////////////////////////////////////
	class CRitual{

	public:
		CRitual(){}


		CRitual(unsigned long MaxMembers) : Members(MaxMembers){
			this->MaxMembers = MaxMembers;
			CurrentMembers = 0;
		}


		~CRitual(){}

		/////////////////////////////////////////////////////////////////////////////////////////////
		//void Setup( unsigned long CasterGUID, unsigned long TargetGUID, unsigned long SpellID )
		// Initializes the Ritual
		//
		//Parameters
		// unsigned long CasterGUID - GUID of the caster Player
		// unsigned long TargetGUID - GUID of the target Player
		// unsigned long SpellID - ID of the channeled spell (visual)
		//
		//Return Value
		// None
		//
		//
		/////////////////////////////////////////////////////////////////////////////////////////////
		void Setup(unsigned long CasterGUID, unsigned long TargetGUID, unsigned long SpellID);

		////////////////////////////////////////////////////////////////////////
		//unsigned long GetCasterGUID()
		// Returns the GUID of the caster Player
		//
		//Parameters
		// None
		//
		//Return Value
		// Returns the GUID of the caster Player
		//
		//
		////////////////////////////////////////////////////////////////////////
		unsigned long GetCasterGUID(){
			return CasterGUID;
		}

		////////////////////////////////////////////////////////////////////////
		//unsigned long GetTargetGUID()
		// Returns the GUID of the target Player
		//
		//Parameters
		// None
		//
		//Return Value
		// Returns the GUID of the target Player
		//
		//
		////////////////////////////////////////////////////////////////////////
		unsigned long GetTargetGUID(){
			return TargetGUID;
		}

		////////////////////////////////////////////////////////////////////////
		//unsigned long GetSpellID()
		// Returns the ID of the channeled spell
		//
		//Parameters
		// None
		//
		//Return Value
		// Returns the ID of the channeled spell
		//
		//
		////////////////////////////////////////////////////////////////////////
		unsigned long GetSpellID(){
			return SpellID;
		}

		////////////////////////////////////////////////////////////////////////
		//bool AddMember( unsigned long GUID )
		// Adds a member to the Ritual
		//
		//Parameters
		// unsigned long GUID - GUID of the ritual member to add
		//
		//Return Value
		// Returns true on success.
		// Returns false on failure.
		//
		//
		////////////////////////////////////////////////////////////////////////
		bool AddMember(unsigned long GUID);

		////////////////////////////////////////////////////////////////////////
		//bool RemoveMember( unsigned long GUID )
		// Removes a member to the Ritual
		//
		//Parameters
		// unsigned long GUID - GUID of the ritual member to remove
		//
		//Return Value
		// Returns true on success.
		// Returns false on failure.
		//
		//
		////////////////////////////////////////////////////////////////////////
		bool RemoveMember(unsigned long GUID);

		////////////////////////////////////////////////////////////////////////
		//bool HasMember( unsigned long GUID )
		// Checks if the Ritual has a member with this GUID
		//
		//Parameters
		// unsigned long GUID - GUID to search for
		//
		//Return Value
		// Returns true if a member with this GUID can be found
		// Returns false otherwise
		//
		//
		/////////////////////////////////////////////////////////////////////////
		bool HasMember(unsigned long GUID);

		////////////////////////////////////////////////////////////////////////
		//unsigned long GetMemberGUIDBySlot( unsigned long Slot )
		// Returns the GUID of the Ritual member in this slot
		//
		//Parameters
		// unsigned long Slot - Slot ID
		//
		//Return Value
		// Returns the GUID of the Ritual member in this slot
		//
		//
		///////////////////////////////////////////////////////////////////////
		unsigned long GetMemberGUIDBySlot(unsigned long Slot){
			return Members[Slot];
		}

		//////////////////////////////////////////////////////////////////////
		//bool HasFreeSlots()
		// Tells is the Ritual has free slots
		//
		//Parameters
		// None
		//
		//Return Value
		// Returns true if there are still free slots
		// Returns false otherwise
		//
		//
		//////////////////////////////////////////////////////////////////////
		bool HasFreeSlots(){
			if(CurrentMembers < MaxMembers)
				return true;
			else
				return false;
		}

		/////////////////////////////////////////////////////////////////////
		//unsigned long GetMaxMembers()
		// Returns the number of members this Ritual requires
		//
		//Parameters
		// None
		//
		//Return Value
		// Returns the number of members this Ritual requires
		//
		//
		/////////////////////////////////////////////////////////////////////
		unsigned long GetMaxMembers(){
			return MaxMembers;
		}

		////////////////////////////////////////////////////////////////////
		//void Finish()
		// Finishes the ritual
		//
		//Parameters
		// None
		//
		//Return Value
		// None
		//
		//
		////////////////////////////////////////////////////////////////////
		void Finish(){
			SpellID = 0;
		}

		////////////////////////////////////////////////////////////////////
		//bool IsFinished()
		// Tells if the ritual is finished
		//
		//Parameters
		// None
		//
		//Return Value
		// Returns true if the ritual is finished
		// Returns false otherwise
		//
		//
		////////////////////////////////////////////////////////////////////
		bool IsFinished(){
			if(SpellID == 0)
				return true;
			else
				return false;
		}

	private:
		unsigned long CasterGUID;
		unsigned long TargetGUID;
		unsigned long SpellID;
		unsigned long CurrentMembers;
		unsigned long MaxMembers;

		std::vector< unsigned long > Members;
	};
}

#endif