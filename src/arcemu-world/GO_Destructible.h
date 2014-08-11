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

#ifndef GO_DESTRUCTIBLE_HPP
#define GO_DESTRUCTIBLE_HPP

namespace Arcemu{
	///////////////////////////////////////////////////////////////////////
	//class GO_Destructible
	// Implements Type 33 ( Destructible ) GameObjects
	//
	///////////////////////////////////////////////////////////////////////
	class GO_Destructible : public GameObject{

	public:
		GO_Destructible();


		GO_Destructible(uint64 GUID);


		~GO_Destructible();


		void InitAI();

		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		//void Damage( uint32 damage, uint64 AttackerGUID, uint64 ControllerGUID, uint32 SpellID )
		// Damages the destructible GameObject with a spell
		//
		//Parameters
		// uint32 damage - The hit points that the GO will lose
		// uint64 AttackerGUID - GUID of the caster of the damaging spell
		// uint64 ControllerGUID - GUID of the controller of the caster of the damaging spell
		// uint32 SpellID - ID of the damaging spell
		//
		//Return Value
		// None
		//
		//
		////////////////////////////////////////////////////////////////////////////////////////////////////////
		void Damage(uint32 damage, uint64 AttackerGUID, uint64 ControllerGUID, uint32 SpellID);


		////////////////////////////////////////////////////////////////////////////////////////////////////////
		//void Rebuild()
		// Rebuilds the damaged/destroyed GameObject.
		//
		//Parameters
		// None
		//
		//Return Value
		// None
		//
		//
		////////////////////////////////////////////////////////////////////////////////////////////////////////
		void Rebuild();


		////////////////////////////////////////////////////////////////////////////////////////////////////////
		//uint32 GetHP()
		// Returns the current hitpoints of the GameObject
		//
		//Parameters
		// None
		//
		//Return Value
		// Returns the current hitpoints of the GameObject
		//
		//
		////////////////////////////////////////////////////////////////////////////////////////////////////////
		uint32 GetHP(){ return hitpoints; }

	private:

		///////////////////////////////////////////////////////////////////////////////////////////////////////
		//void SendDamagePacket( uint32 damage, uint64 AttackerGUID, uint64 ControllerGUID, uint32 SpellID )
		// Notifies the surrounding clients about the GameObject taking damage
		//
		//Parameters
		// uint32 damage - The hit points that the GO will lose
		// uint64 AttackerGUID - GUID of the caster of the damaging spell
		// uint64 ControllerGUID - GUID of the controller of the caster of the damaging spell
		// uint32 SpellID - ID of the damaging spell
		//
		//Return Value
		// None
		//
		//
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		void SendDamagePacket(uint32 damage, uint64 AttackerGUID, uint64 ControllerGUID, uint32 SpellID);

		uint32 hitpoints;
		uint32 maxhitpoints;
	};
}

#endif