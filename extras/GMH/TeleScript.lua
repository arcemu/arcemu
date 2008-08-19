function GoName()
result=".appear "..ToPlayerName:GetText();    
outSAY(result);
end

function NameGo()
result=".summon "..ToPlayerName:GetText();    
outSAY(result);
end

function SearchTele()
result=".recall list "   
outSAY(result);
end

function Tele()
result=".recall port "..ZoneName:GetText(); 
outSAY(result);
end

function AddPort()
result=".recall add "..ZoneName:GetText(); 
outSAY(result);
end

function DelPort()
result=".recall del "..ZoneName:GetText(); 
outSAY(result);
end

function PortPlayer()
result=".recall PortPlayer "..ToPlayerName:GetText().." " ..ZoneName:GetText(); 
outSAY(result);
end

function WorldPort()
result=".worldport "..MapID:GetText().." "..XCord:GetText().." "..YCord:GetText().." "..ZCord:GetText();
outSAY(result);
end


function GPS()
result=".gps";
outSAY(result);
end