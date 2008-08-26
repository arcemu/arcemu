function SetAnnouncementChecked()
if (AnnounceCheck:GetChecked() or ScreenCheck:GetChecked() or GMAnnounceCheck:GetChecked() ) then
Announce();
else
UIErrorsFrame:AddMessage("Please choose where to Announce!", 1.0, 0.0, 0.0, 53, 2);
end
end

function FirstAnnounce()
if (AnnounceSetCheck:GetChecked() or ScreenAnnounceSetCheck:GetChecked() or GMAnnounceSetCheck:GetChecked() ) then
if AnnounceSetCheck:GetChecked() then
firstannounce=".announce "..SetAnnounceText:GetText();    
end
if ScreenAnnounceSetCheck:GetChecked() then
firstannounce=".wannounce "..SetAnnounceText:GetText();    
end
if GMAnnounceSetCheck:GetChecked() then
firstannounce=".gmannounce "..SetAnnounceText:GetText();    
end
else
UIErrorsFrame:AddMessage("Please choose where to Announce!", 1.0, 0.0, 0.0, 53, 2);
end
end

function SecondAnnounce()
if (AnnounceSetCheck:GetChecked() or ScreenAnnounceSetCheck:GetChecked() or GMAnnounceSetCheck:GetChecked() ) then
if AnnounceSetCheck:GetChecked() then
secondannounce=".announce "..SetAnnounceText:GetText();    
end
if ScreenAnnounceSetCheck:GetChecked() then
secondannounce=".wannounce "..SetAnnounceText:GetText();    
end
if GMAnnounceSetCheck:GetChecked() then
secondannounce=".gmannounce "..SetAnnounceText:GetText();    
end
else
UIErrorsFrame:AddMessage("Please choose where to Announce!", 1.0, 0.0, 0.0, 53, 2);
end  
end

function ThirdAnnounce()
if (AnnounceSetCheck:GetChecked() or ScreenAnnounceSetCheck:GetChecked() or GMAnnounceSetCheck:GetChecked() ) then
if AnnounceSetCheck:GetChecked() then
thirdannounce=".announce "..SetAnnounceText:GetText();    
end
if ScreenAnnounceSetCheck:GetChecked() then
thirdannounce=".wannounce "..SetAnnounceText:GetText();    
end
if GMAnnounceSetCheck:GetChecked() then
thirdannounce=".gmannounce "..SetAnnounceText:GetText();    
end
else
UIErrorsFrame:AddMessage("Please choose where to Announce!", 1.0, 0.0, 0.0, 53, 2);
end     
end

function ForthAnnounce()
if (AnnounceSetCheck:GetChecked() or ScreenAnnounceSetCheck:GetChecked() or GMAnnounceSetCheck:GetChecked() ) then
if AnnounceSetCheck:GetChecked() then
forthannounce=".announce "..SetAnnounceText:GetText();    
end
if ScreenAnnounceSetCheck:GetChecked() then
forthannounce=".wannounce "..SetAnnounceText:GetText();    
end
if GMAnnounceSetCheck:GetChecked() then
forthannounce=".gmannounce "..SetAnnounceText:GetText();    
end
else
UIErrorsFrame:AddMessage("Please choose where to Announce!", 1.0, 0.0, 0.0, 53, 2);
end     
end

function FifthAnnounce()
if (AnnounceSetCheck:GetChecked() or ScreenAnnounceSetCheck:GetChecked() or GMAnnounceSetCheck:GetChecked() ) then
if AnnounceSetCheck:GetChecked() then
fifthannounce=".announce "..SetAnnounceText:GetText();    
end
if ScreenAnnounceSetCheck:GetChecked() then
fifthannounce=".wannounce "..SetAnnounceText:GetText();    
end
if GMAnnounceSetCheck:GetChecked() then
fifthannounce=".gmannounce "..SetAnnounceText:GetText();    
end
else
UIErrorsFrame:AddMessage("Please choose where to Announce!", 1.0, 0.0, 0.0, 53, 2);
end     
end
