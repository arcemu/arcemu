function Horsey_OnFirstPassengerEntered( vehicle, passenger )
	vehicle:SendChatMessage( 12, 0, "Finally someone!" );
end

function Horsey_OnVehicleFull( vehicle )
	vehicle:SendChatMessage( 12, 0, "Now I feel full :S" );
end

function Horsey_OnLastPassengerLeft( vehicle, passenger )
	vehicle:SendChatMessage( 12, 0, "So ronery..." );
end

RegisterUnitEvent( 28605, 26, "Horsey_OnFirstPassengerEntered" );
RegisterUnitEvent( 28605, 27, "Horsey_OnVehicleFull" );
RegisterUnitEvent( 28605, 28, "Horsey_OnLastPassengerLeft" );