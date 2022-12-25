--[[
  ArcEmu MMORPG Server
  Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
  Copyright (C) 2008-2022 Arcemu Team <http://www.ArcEmu.org/>
 
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU Affero General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version.
 
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Affero General Public License for more details.
 
  You should have received a copy of the GNU Affero General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
]]

-- Spawns High Tinker Mekkatorque who makes a different speech for each target NPC
function PerformSpeech_DummyHandler( effect, spell )
	local target = spell:GetTarget()
	local player = spell:GetCaster()
	
	-- Target should be an NPC we used the item on
	if target ~= NIL
	then
		local npc = target				
		local name = npc:GetName()

		if name == "Milli Featherwhistle" then
			local tinker = npc:SpawnCreature( 39712, -5451.6, -619.33, 394.55, 1.7269, 875, 10000 )
			tinker:EventChat(12, 0, "They make take our lives, but they'll never take our...", 2000)
			tinker:EventChat(12, 0, "...INNOVATION!", 4000)
			npc:EventChat(12, 0, "What? I don't even know what you're talking about! That's terrible!", 6000)
			
			player:MarkQuestObjectiveAsComplete( 25283, 1 )
			
		elseif name == "Ozzie Togglevolt"
		then
			local tinker = npc:SpawnCreature( 39712, -5496.3984, -457.6682, 395.4378, 3.2318, 875, 10000 )
			tinker:EventChat(12, 0, "What I want out of each and every one of you is a hard-target search of every refueling station, residence, warehouse, farmhouse, henhouse, outhouse, and doghouse in this area", 2000)
			tinker:EventChat(12, 0, "Your fugitive's name is Mekgineer Thermaplugg.", 4000)
			tinker:EventChat(12, 0, "Go get him!", 6000)
			npc:EventChat(12, 0, "Hmm, I suppose it could work. But it could really use a little more umph!", 8000)
			
			player:MarkQuestObjectiveAsComplete( 25283, 0 )
			
		elseif name == "Tog Rustsprocket"
		then
			local tinker = npc:SpawnCreature( 39712, -5643.3579, -525.8497, 403.9703, 4.9502, 875, 10000 )
			tinker:EventChat(12, 0, "We will not go quietly into the night! We will not vanish without a fight!", 2000)
			tinker:EventChat(12, 0, "We're going to live on! We're going to survive! Today we celebrate...", 4000)
			tinker:EventChat(12, 0, "Our Autonomy Day!", 6000)
			npc:EventChat(12, 0, "Horrible! Well, all right, maybe it just needs a little cleaning up?", 8000)
			
			player:MarkQuestObjectiveAsComplete( 25283, 2 )
		else
			spell:Cancel()
		end
		
	end
end

RegisterDummySpell( 74222, "PerformSpeech_DummyHandler" );
