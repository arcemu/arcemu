function Mohawk_OnTalk( unit, event, player )
		unit:GossipCreateMenu( 1, player, 0 );
		unit:GossipMenuAddItem( 0, "Give me a vehicle!", 1, 0 );
		unit:GossipMenuAddItem( 0, "Dismiss my vehicle!", 2, 0 );
		unit:GossipMenuAddItem( 0, "Am I on a vehicle?", 3, 0 );
		unit:GossipMenuAddItem( 0, "Add me a passenger!", 4, 0 );
		unit:GossipMenuAddItem( 0, "Shut up woman and get on my vehicle!", 5, 0 );
		unit:GossipMenuAddItem( 0, "Get off my vehicle!", 6, 0 );
		unit:GossipMenuAddItem( 0, "Speed up my vehicle!", 7, 0 );
		unit:GossipSendMenu( player );

end


function Mohawk_OnSelect( unit, event, player, id, selection, code )
		if( selection == 1 )
		then
				if( player:IsOnVehicle() )
				then
					unit:SendChatMessage( 12, 0, "You already have a vehicle! Are you blind?!" );
				else
					unit:SendChatMessage( 12, 0, "Here it is!" );
					player:SpawnAndEnterVehicle( 28605, 1000 );
				end
		elseif( selection == 2 )
		then
				if( player:IsOnVehicle() == false )
				then
					unit:SendChatMessage( 12, 0, "You don't have a vehicle to dismiss! Are you blind?!" );	
				else
					unit:SendChatMessage( 12, 0, "Go away vehicle! Go away!" );
					player:DismissVehicle();
				end
		elseif( selection == 3 )
		then
				if( player:IsOnVehicle() )
				then
					unit:SendChatMessage( 12, 0, "Ofc, you are on a vehicle! Are you blind?" );
				else
					unit:SendChatMessage( 12, 0, "Ofc, you are NOT on a vehicle! Are you blind?" );
				end
		elseif( selection == 4 )
		then
				if( player:IsOnVehicle() == false )
				then
					unit:SendChatMessage( 12, 0, "You don't have a vehicle to add a passeger to! Are you blind?!" );	
				else
					if( player:HasEmptyVehicleSeat() == false )
					then
						unit:SendChatMessage( 12, 0, "You don't have free seats! Are you blind?!" );
					else
						unit:SendChatMessage( 12, 0, "OK!" );
						player:AddVehiclePassenger( 31111 );
					end
				end
		elseif( selection == 5 )
		then
				if( player:IsOnVehicle() == false )
				then
					unit:SendChatMessage( 12, 0, "You are NOT on a vehicle! Are you blind?" );
				else
					if( player:HasEmptyVehicleSeat() == false )
					then
						unit:SendChatMessage( 12, 0, "You don't have any more free seats! Are you blind?" );
					else
						if( unit:IsOnVehicle() )
						then
							unit:SendChatMessage( 12, 0, "I am already on a vehicle!" );
						else
							local u = player:GetVehicleBase();
							unit:EnterVehicle( u:GetGUID(), 0 );
						end
					end
				end
		elseif( selection == 6 )
		then
			if( unit:IsOnVehicle() == false )
			then
				unit:SendChatMessage( 12, 0, "I am NOT on a vehicle!" );
			else
				unit:ExitVehicle();
			end
		elseif( selection == 7 )
		then
			if( player:IsOnVehicle() == false )
			then
				unit:SendChatMessage( 12, 0, "You are NOT on a vehicle!" );
			else
				u = player:GetVehicleBase();
				u:ModifyWalkSpeed( 100.0 );
				u:ModifyRunSpeed( 100.0 );
				u:ModifyFlySpeed( 100.0 );
			end
		end

		player:GossipComplete();
end

function Mohawk_OnEnterVehicle( unit )
    unit:SendChatMessage( 12, 0, "Naice! I'm on a vehicle now!" );
end

function Mohawk_OnExitVehicle( unit )
    unit:SendChatMessage( 12, 0, "Aw I got booted :(" );
end

RegisterUnitGossipEvent( 31111, 1, "Mohawk_OnTalk" );
RegisterUnitGossipEvent( 31111, 2, "Mohawk_OnSelect" );
RegisterUnitEvent( 31111, 24, "Mohawk_OnEnterVehicle" );
RegisterUnitEvent( 31111, 25, "Mohawk_OnExitVehicle" );
