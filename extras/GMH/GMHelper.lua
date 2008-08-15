--**********************************************************************
--$Author: ssargon $
--$Date: 2007-08-06 09:25:44 -0400 (Mon, 06 Aug 2007) $
--$Revision: 15 $
--**********************************************************************


function OpenMain()
    FullForm:Hide();
    MinipForm:Hide();
    MiniForm:Hide();
    if( view == 1 ) then
        FullForm:Show();
    elseif( view == 2 ) then
        MinipForm:Show();
    elseif( view == 3 ) then
        MiniForm:Show();
    else
        view=1;
        OpenMain();
    end
end

function outSAY(text)
 SendChatMessage(text, "GUILD", nil,nil);
end

function GMHelperOnLoad()
 this:RegisterForDrag("RightButton");
JoinChannelByName("gm_sync_channel", "lhjf448gfdw279hgfw6");
OpenMain()
addonopen = "1";
end

-- Binding Variables
BINDING_HEADER_GMHelper = "GM Helper";
BINDING_NAME_TOGLEADDON = "Togles GM Helper";

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

function TicketList()
result=".gmt get";   
outSAY(result);
end

function TicketChan()
JoinChannelByName("gm_sync_channel", "lhjf448gfdw279hgfw6");
end

function ShowTicket()
result=".gmt getid "..TicketNumber:GetText();   
outSAY(result);
end

function DeleteTicket()
result=".gmt delid "..TicketNumber:GetText();    
outSAY(result);
end

function RemoveItem()
result=".removeitem "..ItemNumber:GetText();    
outSAY(result);
end

function AddItem()
result=".additem "..ItemNumber:GetText();    
outSAY(result);
end

function AddItemSet()
result=".additemset "..ItemSetNumber:GetText();    
outSAY(result);
end

function SearchItem()
result=".lookupitem "..ItemNumber:GetText();    
outSAY(result);
end

function SearchSkill()
result=".lookupskill "..SkillName:GetText();
outSAY(result);
end

function LearnSkill()
result=".advancesk "..SkillNumber:GetText().." "..SkillLvl:GetText().." "..SkillMax:GetText();        
outSAY(result);
end

function UnLearnSkill()
result=".removesk "..SkillNumber:GetText();    
outSAY(result);
end

function LearnBlackSmithing()
result=".advancesk 164 "..SkillLvl:GetText();    
outSAY(result);
end

function LearnTailoring()
result=".advancesk 197 "..SkillLvl:GetText();    
outSAY(result);
end

function LearnLeatherworking()
result=".advancesk 165 "..SkillLvl:GetText();    
outSAY(result);
end

function LearnEngineering()
result=".advancesk 202 "..SkillLvl:GetText();    
outSAY(result);
end

function LearnPoisons()
result=".advancesk 40 "..SkillLvl:GetText();    
outSAY(result);
end

function LearnEnchanting()
result=".advancesk 333 "..SkillLevel:GetText();    
outSAY(result);
end

function LearnFishing()
result=".advancesk 356 "..SkillLevel:GetText();    
outSAY(result);
end

function LearnMining()
result=".advancesk 186 "..SkillLevel:GetText();    
outSAY(result);
end

function LearnSkinning()
result=".advancesk 393 "..SkillLevel:GetText();    
outSAY(result);
end

function LearnAlchemy()
result=".advancesk 171 "..SkillLevel:GetText();    
outSAY(result);
end

function LearnHerbalism()
result=".advancesk 182 "..SkillLevel:GetText();    
outSAY(result);
end

function LearnFirstAid()
result=".advancesk 129 "..SkillLevel:GetText();    
outSAY(result);
end

function LearnCooking()
result=".advancesk 185 "..SkillLevel:GetText();    
outSAY(result);
end

function LearnRiding()
result=".advancesk 762 "..RidingLevel:GetText();    
outSAY(result);
end

function AddMoney()
result = ".modify gold " ..Gold:GetNumber()*10000+Silver:GetNumber()*100+Copper:GetNumber();
outSAY(result);
end

function Announce()
result=".announce "..AnnounceText:GetText();    
outSAY(result);
end

function RaidStyleAnnounce()
result=".wannounce "..AnnounceText:GetText();    
outSAY(result);
end

function WhisperOn()
result=".allowwhispers "..PlayerName2:GetText();
outSAY(result);
end

function WhisperOff()
result=".blockwhispers "..PlayerName2:GetText();  
outSAY(result);
end

function WorldPort()
result=".worldport "..MapID:GetText().." "..XCord:GetText().." "..YCord:GetText().." "..ZCord:GetText();
outSAY(result);
end

function LearnSpell()
result=".learn "..SpellNumber:GetText();    
outSAY(result);
end

function UnLearnSpell()
result=".unlearnspell "..SpellNumber:GetText();    
outSAY(result);
end

function LearnDualWield()
result=".advancesk 118 "..WeaponSkillLevel:GetText();    
outSAY(result);
end

function LearnJewel()
result=".advancesk 755 "..SkillLevel:GetText();    
outSAY(result);
end

function LearnStaves()
result=".advancesk 136 "..WeaponSkillLevel:GetText();    
outSAY(result);
end

function LearnUnarmed()
result=".advancesk 136 "..WeaponSkillLevel:GetText();    
outSAY(result);
end

function LearnTwoHandedAxes()
result=".advancesk 172 "..WeaponSkillLevel:GetText();    
outSAY(result);
end

function LearnDaggers()
result=".advancesk 173 "..WeaponSkillLevel:GetText();    
outSAY(result);
end


function LearnCrossbows()
result=".advancesk 226 "..WeaponSkillLevel:GetText();    
outSAY(result);
end


function LearnWands()
result=".advancesk 228 "..WeaponSkillLevel:GetText();    
outSAY(result);
end


function LearnPolarms()
result=".advancesk 229 "..WeaponSkillLevel:GetText();    
outSAY(result);
end


function LearnGuns()
result=".advancesk 46 "..WeaponSkillLevel:GetText();    
outSAY(result);
end

function LearnSwords()
result=".advancesk 43 "..WeaponSkillLevel:GetText();    
outSAY(result);
end

function LearnTwoHandedSwords()
result=".advancesk 55 "..WeaponSkillLevel:GetText();    
outSAY(result);
end

function LearnFistWeapons()
result=".advancesk 473 "..WeaponSkillLevel:GetText();    
outSAY(result);
end

function LearnTwoHandedMaces()
result=".advancesk 160 "..WeaponSkillLevel:GetText();    
outSAY(result);
end

function LearnBows()
result=".advancesk 45 "..WeaponSkillLevel:GetText();    
outSAY(result);
end

function LearnThrown()
result=".advancesk 176 "..WeaponSkillLevel:GetText();    
outSAY(result);
end

function LearnAxes()
result=".advancesk 44 "..WeaponSkillLevel:GetText();    
outSAY(result);
end

function LearnMaces()
result=".advancesk 54 "..WeaponSkillLevel:GetText();    
outSAY(result);
end



function TargetObject()
result=".gobject select ";    
outSAY(result);
end

function DeleteObject()
result=".gobject delete ";    
outSAY(result);
end

function PlaceObject()
result=".gobject spawn "..ObjectNumber:GetText();    
outSAY(result);
end

function BanPlayer()
result=".banchar "..CharName:GetText().." "..Reason:GetText();   
outSAY(result);
end

function UnBanPlayer()
result=".unbanchar "..CharName:GetText();    
outSAY(result);
end

function KickPlayer()
result=".kick "..CharName:GetText();    
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

function AddItemVendor()
result=".npc vendoradditem "..NPCItemNumber:GetText();    
outSAY(result);
end

function RemoveItemVendor()
result=".npc vendorremoveitem "..NPCItemNumber:GetText();    
outSAY(result);
end

function SpawnNPC()
result=".npc spawn "..NPCNumber:GetText();    
outSAY(result);
end

function DeleteNPCbyNumber()
result=".npc delete "..NPCNumber:GetText();    
outSAY(result);
end

function DeleteNPC()
result=".npc delete";    
outSAY(result);
end

function FirstAnnounce()
firstannounce=".announce "..SetAnnounceText:GetText();    
end

function SecondAnnounce()
secondannounce=".announce "..SetAnnounceText:GetText();    
end

function ThirdAnnounce()
thirdannounce=".announce "..SetAnnounceText:GetText();    
end

function ForthAnnounce()
forthannounce=".announce "..SetAnnounceText:GetText();    
end

function FifthAnnounce()
fifthannounce=".announce "..SetAnnounceText:GetText();    
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


function TogleAddon()
if( addonopen == "1" ) then
FullForm:Hide();
MinipForm:Hide();
MiniForm:Hide();
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
--AnnounceForm:Hide();
AnnounceForm:Hide();
PlayerForm:Hide();
SpawnsForm:Hide();
ViewForm:Hide();
WepskForm:Hide();
addonopen = "0";
else
OpenMain();
addonopen = "1";
end
end

function GmOutfit()
result=".additem 2586"
outSAY(result);
result=".additem 11508"
outSAY(result);
result=".additem 12064"
outSAY(result);
end

function CreateGuild()
result=".guild create %t"..GuildName:GetText();  
outSAY(result);
end

function LevelPlayer()
result=".mod level "..PlayerLevel:GetText();    
outSAY(result);
end

function AdvanceAll()
result=".advanceallskills "..SkillsBy:GetText();   
outSAY(result);
end

function AddSH()
result=".npc addspirit";
outSAY(result);
end

function NPCInfo()
result=".npc info";
outSAY(result);
end

-- added by gondrup...

function SpawnItemVendors()
    result="These functions are broken for now.";
    outSAY(result);
    result="These functions are broken for now.";
    outSAY(result);
end

function SpawnArmorVendors()
    result="These functions are broken for now.";
    outSAY(result);
    result="These functions are broken for now.";
    outSAY(result);
    result="These functions are broken for now.";
    outSAY(result);
    result="These functions are broken for now.";
    outSAY(result);
    result="These functions are broken for now.";
    outSAY(result);
    result="These functions are broken for now.";
    outSAY(result);
    result="These functions are broken for now.";
    outSAY(result);
    result="These functions are broken for now.";
    outSAY(result);
    result="These functions are broken for now.";
    outSAY(result);
end

function SpawnGemVendors()
    result="These functions are broken for now.";
    outSAY(result);
    result="These functions are broken for now.";
    outSAY(result);
    result="These functions are broken for now.";
    outSAY(result);
    result="These functions are broken for now.";
    outSAY(result);
    result="These functions are broken for now.";
    outSAY(result);
end

function SpawnTwoHVendors()
    result="These functions are broken for now.";
    outSAY(result);
    result="These functions are broken for now.";
    outSAY(result);
    result="These functions are broken for now.";
    outSAY(result);
    result="These functions are broken for now.";
    outSAY(result);
    result="These functions are broken for now.";
    outSAY(result);
end

function SpawnOneHVendors()
    result="These functions are broken for now.";
    outSAY(result);
    result="These functions are broken for now.";
    outSAY(result);
    result="These functions are broken for now.";
    outSAY(result);
    result="These functions are broken for now.";
    outSAY(result);
    result="These functions are broken for now.";
    outSAY(result);
end

function SpawnRangedVendors()
    result="These functions are broken for now.";
    outSAY(result);
    result="These functions are broken for now.";
    outSAY(result);
    result="These functions are broken for now.";
    outSAY(result);
    result="These functions are broken for now.";
    outSAY(result);
end

function SpawnOffHandVendors()
    result="These functions are broken for now.";
    outSAY(result);
    result="These functions are broken for now.";
    outSAY(result);
end

function NPCCome()
    result=".npc come";
    outSAY(result);
end

function SInfo()
    result=".info";
    outSAY(result);
end

function Uptime()
    result="These functions are broken for now.";
    outSAY(result);
end

function Invis()
    result=".invisible";
    outSAY(result);
end

function Invince()
    result=".invincible";
    outSAY(result);
end
