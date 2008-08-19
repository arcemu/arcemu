function BanPlayer()
result=".ban char "..CharName:GetText().." "..BanReason:GetText();   
outSAY(result);
end

function UnBanPlayer()
result=".unban char "..CharName:GetText();    
outSAY(result);
end

function AddIPBan()
result=".ban ip "..IPAddress:GetText().." "..Duration:GetText();    
outSAY(result);
end

function DelIPBan()
result=".unban ip "..IPAddress:GetText();
outSAY(result);
end

function KickPlayer()
result=".kick "..CharName:GetText().." "..BanReason:GetText();    
outSAY(result);
end

function ParPlayer()
result=".paralyze "..CharName:GetText();    
outSAY(result);
end

function UnParPlayer()
result=".unparalyze "..CharName:GetText();    
outSAY(result);
end

function PInfo()
result=".playerinfo "..CharName:GetText();    
outSAY(result);
end

