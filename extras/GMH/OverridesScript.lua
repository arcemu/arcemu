
function CheatStatus()
	result=".cheat status";
	outSAY(result);
end

function CheatUpdate()
if ( FlyCheck:GetChecked() ) then--fly
	result=".cheat fly on";
	outSAY(result);
else
	result=".cheat fly off";
	outSAY(result);
end
if ( GodCheck:GetChecked() ) then--god
	result=".cheat god on";
	outSAY(result);
else
	result=".cheat god off";
	outSAY(result);
end
if ( NCDCheck:GetChecked() ) then--cooldown
	result=".cheat cooldown on";
	outSAY(result);
else
	result=".cheat cooldown off";
	outSAY(result);
end
if ( NCTCheck:GetChecked() ) then--casttime
	result=".cheat casttime on";
	outSAY(result);
else
	result=".cheat casttime off";
	outSAY(result);
end
if ( PowCheck:GetChecked() ) then--power
	result=".cheat power on";
	outSAY(result);
else
	result=".cheat power off";
	outSAY(result);
end
if ( AuraCheck:GetChecked() ) then--stack
	result=".cheat stack on";
	outSAY(result);
else
	result=".cheat stack off";
	outSAY(result);
end
if ( TrigCheck:GetChecked() ) then--triggers
	result=".cheat triggerpass on";
	outSAY(result);
else
	result=".cheat triggerpass off";
	outSAY(result);
end
end

function FlySpeed()
result=".cheat flyspeed "..FlyEntry:GetText();
outSAY(result);
end

function FlightPath()
if ( TaxiCheck:GetChecked() ) then --Taxi
    result=".cheat taxi on";
    outSAY(result);
else
    result=".cheat taxi off";
    outSAY(result);
end
end
