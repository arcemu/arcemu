function AnnounceChecked()
if (AnnounceCheck:GetChecked() or ScreenCheck:GetChecked() or GMAnnounceCheck:GetChecked() ) then
Announce();
else
UIErrorsFrame:AddMessage("Please choose where to Announce!", 1.0, 0.0, 0.0, 53, 2);
end
end


function Announce()
if ( AnnounceCheck:GetChecked() ) then--Announce
	result=".announce "..AnnounceText:GetText();
	outSAY(result);
else
    result="";
	outSAY(result);
end
if ( ScreenCheck:GetChecked() ) then--Announce
	result=".wannounce "..AnnounceText:GetText();
	outSAY(result);
else
    result="";
	outSAY(result);
end
if ( GMAnnounceCheck:GetChecked() ) then--Announce
	result=".gmannounce "..AnnounceText:GetText();
	outSAY(result);
else
    result="";
	outSAY(result);
end
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