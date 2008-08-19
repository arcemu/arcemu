--**********************************************************************
--$Author: ssargon $
--$Date: 2007-08-06 09:25:44 -0400 (Mon, 06 Aug 2007) $
--$Revision: 15 $
--**********************************************************************


function OpenMain()
    if( view == 1 ) then
        FullForm:Show();
    elseif( view == 2 ) then
        MinipForm:Show();
    elseif( view == 3 ) then
        MiniForm:Show();
    else
        FullForm:Show();
    end
end

function TogleAddon()
if( addonopen == 1 ) then
FullForm:Hide();
MinipForm:Hide();
MiniForm:Hide();
Skill2Form:Hide();
CommForm:Hide();
ItemForm:Hide();
MiscForm:Hide();
ObjectForm:Hide();
TicketForm:Hide();
TeleForm:Hide();
ProfessionsForm:Hide();
RidingForm:Hide();
SkillForm:Hide();
SpellForm:Hide();
BanForm:Hide();
NPCForm:Hide();
AnnounceForm:Hide();
PlayerForm:Hide();
ViewForm:Hide();
WepskForm:Hide();
OverridesForm:Hide();
ModifyForm:Hide();
QuickItemForm:Hide();
WaypointsForm:Hide();
PlaySound("INTERFACESOUND_CHARWINDOWCLOSE");
addonopen = 0;
else
OpenMain();
PlaySound("INTERFACESOUND_CHARWINDOWOPEN");
addonopen = 1;
end
end

function outSAY(text)
 --SendChatMessage("."..text, "GUILD", nil,nil);--functional type2
 --SendChatMessage(""..text, "GUILD", nil,nil);--test type2
 SendChatMessage(""..text, "SAY", nil,nil);--test type2noguild
 --SendChatMessage(text, "GUILD", nil,nil);--old
end

function GMHelperOnLoad()
 this:RegisterForDrag("RightButton");
JoinChannelByName("gm_sync_channel", "lhjf448gfdw279hgfw6");
end

function GMHelper_Loaded()
UIErrorsFrame:AddMessage("GM Helper v2.0.0 loaded!", 0.0, 1.0, 0.0, 53, 2);
--OpenMain();
--addonopen = 1;
PSoundF("Interface\\Addons\\GMH\\Sounds\\Omega.wav");
end

function PSoundF(file)
    PlaySoundFile(file);
end

-- Binding Variables
BINDING_HEADER_GMHelper = "GM Helper";
BINDING_NAME_TOGLEADDON = "Togles GM Helper";

