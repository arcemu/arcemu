function GMHAnnounce()
result=".announce "..AnnounceText:GetText();    
outSAY(result);
end


function RaidStyleAnnounce()
result=".wannounce "..AnnounceText:GetText();    
outSAY(result);
end

function GMAnnounce()
result=".gmannounce "..AnnounceText:GetText();    
outSAY(result);
end

function WideBroadcast()
result=".wannounce "..AnnounceText:GetText();
outSAY(result);
result=".announce "..AnnounceText:GetText();
outSAY(result);
end

function WhisperOn()
result=".gm allowwhispers "..PlayerName2:GetText();
outSAY(result);
end

function WhisperOff()
result=".gm blockwhispers "..PlayerName2:GetText();  
outSAY(result);
end

function SayFirstAnnounce()
outSAY(firstannounce);
end

function SaySecondAnnounce()
outSAY(secondannounce);
end

function SayThirdAnnounce()
outSAY(thirdannounce);
end

function SayForthAnnounce()
outSAY(forthannounce);
end

function SayFifthAnnounce()
outSAY(fifthannounce);
end