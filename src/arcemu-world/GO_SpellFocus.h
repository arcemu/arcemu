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
namespace Arcemu{
	/////////////////////////////////////////////////////////////////////
	//class GO_SpellFocus
	// Class implementing Type 8 (SPELL_FOCUS) GameObjects
	//
	////////////////////////////////////////////////////////////////////
	class GO_SpellFocus : public GameObject{
	public:
		GO_SpellFocus();
		GO_SpellFocus(uint64 GUID);
		~GO_SpellFocus();
		void InitAI();
		void Update(unsigned long time_passed);
		///////////////////////////////////////////////////////////////////
		//void CastSpell( uint64 TargetGUID, uint32 SpellID )
		// Makes the Trap try to cast a spell on a target
		//
		//Parameters
		// uint64 TargetGUID - GUID of the Target Object
		// uint32 SpellId - Identifier of the spell to cast
		//
		//Return Value
		// None
		//
		//
		//////////////////////////////////////////////////////////////////
		void CastSpell(uint64 TargetGUID, uint32 SpellID);
	private:
		SpellEntry *spell;
		float range;
	};
}