

--Object Globals
local Names = {};
local Types = {};
local Views = {};
local Prev;
local Next;

--Cache vars
local Tickets = {};
local TicketNames = {};
local LastIndex= -1;

--Other
local CurrentPage = 0;
TicketChannel = "gm_sync_channel";
TT_Get = false;
TT_Name = nil;
local PageCount;
local Alert = false;

--Misc.
local TicketTypes = {};
TicketTypes[0]="Stuck"; --135681
TicketTypes[1]="Harassment"; --135682
TicketTypes[2]="Guild"; --135683
TicketTypes[3]="Item"; --135684
TicketTypes[4]="Environment"; --135685
TicketTypes[5]="NPC"; --135686
TicketTypes[6]="Quest"; --135687
TicketTypes[7]="--Unused--"; --Unused
TicketTypes[8]="--Unused--"; --Unused
TicketTypes[9]="Character"; --135690 !!!

local function Clear()
    for i=0,14 do
        Names[i]:Clear();
        Types[i]:Clear();
        Views[i]:Hide();
    end
end

local function GotoPage(id)
    begin = id*15;
    finish = begin+14;
    Clear();
    for i=begin,finish do
        if Tickets[i] ~= nil then
            Names[i-begin]:AddMessage(Tickets[i].Name);
            Types[i-begin]:AddMessage(Tickets[i].Category);
            Views[i-begin]:Show();
        end
    end
    Prev:Enable();
    Next:Enable();
    if begin <= 0 then
        Prev:Disable();
    end
    if finish > LastIndex then
        Next:Disable();
    end
    CurrentPage = id;
    TicketTrackerPanel1Bar:Clear();
    TicketTrackerPanel1Bar:AddMessage(""..(CurrentPage+1).." / "..PageCount);
end



local function Refresh()
    Tickets={};
    TicketNames={};
    LastIndex=-1;
    Clear();
end



local function Reticket()
    SendChatMessage("!gmTicket get","GUILD");
end

function TT_Refresh()
    Refresh();
    Reticket();
end

local function AddTicket(name,category)
    if TicketNames[name] == nil then
        LastIndex = LastIndex+1;
        Tickets[LastIndex] = {Name=name,Category=category};
        TicketNames[name] = LastIndex;
        PageCount = math.ceil(LastIndex/15);
    else
    end
end

function TT_Delete(name)
    SendChatMessage("!gmTicket del "..name,"GUILD");
    Refresh();
    Reticket();
end

--Set up to receive ticket info.
local ORIG_ChatFrame_MessageEventHandler = ChatFrame_MessageEventHandler;
function ChatFrame_MessageEventHandler(event)
    if event == "CHAT_MSG_CHANNEL" and arg9 == TicketChannel then
        junk,msg = strsplit(" ",arg1);
        args = {strsplit(",",msg)};
        if args[1] == "2" and arg2 == PlayerName:GetText() then
            Refresh();
        elseif args[1] == "5" then
            if Alert == false then
                UIErrorsFrame:AddMessage("New Ticket");
                Alert = true;
            end
            Refresh();
            Reticket();
        elseif args[1] == "0" and args[5] ~= "0" and arg2 == PlayerName:GetText() then
            AddTicket(args[2],TicketTypes[args[4]-135681],0);
            GotoPage(CurrentPage);
        elseif args[1] == "3" and arg2 == PlayerName:GetText() and TT_Get == true then
            TT_Get = false;
            TT_Name = args[2];
            len = string.len(args[2])+13;
            TicketViewPanel1Message:AddMessage("|Hplayer:"..args[2].."|h["..args[2].."]|h: "..string.sub(arg1,len));
        end
    else
        ORIG_ChatFrame_MessageEventHandler(event);
    end
end

function TT_Page(up)
    if up==true then
        GotoPage(CurrentPage+1);
    else
        GotoPage(CurrentPage-1);
    end
    PlaySound("INTERFACESOUND_CHARWINDOWTAB");
end

function TT_Click(index)
    TT_Get = true;
    TicketViewPanel1Message:Clear();
    SendChatMessage("!gmTicket getid "..Tickets[(CurrentPage*15)+index].Name);
    SendChatMessage("!gm allowwhispers "..Tickets[(CurrentPage*15)+index].Name);
    TicketTracker:Hide();
    TicketView:Show();
    PlaySound("QUESTLOGOPEN");
end

local function Setup()
    for i=0,14 do
        Names[i] = getglobal("TicketTrackerPanel1Component"..i);
    end
    for i=15,29 do
        Types[i-15] = getglobal("TicketTrackerPanel1Component"..i);
    end
    for i=30,44 do
        Views[i-30] = getglobal("TicketTrackerPanel1Component"..i);
        Views[i-30]:Hide();
    end
    Prev = TicketTrackerPanel1Prev;
    Next = TicketTrackerPanel1Next;
    Prev:Disable();
    Next:Disable();

end

function TT_JoinChannel()
--[[    JoinChannelByName(TicketChannel,"",ChatFrame1:GetID());
    TicketTrackerEdit:SetText(TicketChannel);
    Refresh();
    Reticket();]]
end

function TT_LeaveChannel()
    LeaveChannelByName(TicketChannel);
end

function TT_Setup(event)
    if event == "VARIABLES_LOADED" then
        Setup();
    elseif event == "PLAYER_LEAVING_WORLD" or event == "PLAYER_LOGOUT" then
        --TT_LeaveChannel();
    elseif event == "PLAYER_LOGIN" then
        TT_JoinChannel();
    end
end

function TT_Show()
    TicketTracker:Show();
    PlaySound("INTERFACESOUND_CHARWINDOWOPEN");
    Alert = false;
end

function TT_SetChannel(msg)
    TT_LeaveChannel();
    TicketChannel=msg;
    ReloadUI();
end


TicketTrackerPanel1Bar:AddMessage("1 / 1");
SlashCmdList["TICKETTRACKER"] = TT_Show;
SLASH_TICKETTRACKER1="/tt";
SlashCmdList["TICKETCHANNEL"] = TT_SetChannel;
SLASH_TICKETCHANNEL1="/ticketchannel";
TicketTracker:RegisterEvent("VARIABLES_LOADED");
TicketTracker:RegisterEvent("PLAYER_LEAVING_WORLD");
TicketTracker:RegisterEvent("PLAYER_LOGOUT");
TicketTracker:RegisterEvent("PLAYER_LOGIN");
TicketTracker:Hide();

