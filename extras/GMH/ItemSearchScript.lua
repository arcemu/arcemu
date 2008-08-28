---------------------------------------------
-- Everything below was done by Gondrup and added by Mukele. Thank you Mukele! - Maven --
---------------------------------------------

-- Below vars are used throughout the item search
item_search_results = {}
itemName = {}

itemNameReal = {}
itemLink = {}
itemRarity = {}
itemLevel = {}
itemMinLevel = {}
itemType = {}
itemSubType = {}
itemStackCount = {}
itemEquipLoc = {}
itemTexture = {}

i = 1;

-- Item colour based on rarity
function SetQuality(quality)
    if (quality == 0) then
        return "|c00A9A9A9";
    elseif (quality == 1) then
        return "|c00FFFFFF";
    elseif (quality == 2) then
        return "|c007CFC00";
    elseif (quality == 3) then
        return "|c004169E1";
    elseif (quality == 4) then
        return "|c009932CC";
    elseif (quality == 5) then
        return "|c00FF8C00";
    else
        return "|c00FFFFFF";
    end
end

-- Fired when a server message is sent to the client
function Chat_OnEvent(event, text)
	--Detects if you tried to advance a skill but didnt get it 
	if latestSkillID=="" and text=="Does not have skill line, adding." then
	elseif text=="Does not have skill line, adding." then
		outSAY(".character advancesk "..latestSkillID.." "..latestSkillLevel);
		latestSkillID="";
    elseif string.find(text, "Item") then
		-- If the detected string is an item result
        idlength, _, _, _ = string.find(text, ":");
        item_search_results[i] = string.sub(text, 6, idlength-1);  
        itemName[i] = text;
        ProcessItemSearch(item_search_results[i]);
        i = i + 1;
    -- Reset if its a new search
    elseif string.find(text, "Starting search of item ") then
        for i=1, 25 do
            getglobal("ItemFormSearchTexture"..i.."Texture"):SetTexture("Interface\\Icons\\INV_Misc_QuestionMark");
            getglobal("ItemFormSearchTexture"..i):Hide();
            getglobal("ItemFormSearchLabelItemID"..i):Hide();
            getglobal("ItemFormSearchButton"..i):Hide();
        end
        i = 1;
    end
end

-- Function to update each button when a result is recieved by the client
function ProcessItemSearch(itemid)
    getglobal("ItemFormSearchTexture"..i):Show();
    getglobal("ItemFormSearchLabelItemID"..i):Show();
    getglobal("ItemFormSearchButton"..i):Show();
    -- Update "number of results" text
    text = "Results Found: "..i;
    ItemFormSearchLabel2Label:SetText(text);
    if GetItemInfo(itemid) then
        itemNameReal[i], itemLink[i], itemRarity[i], itemLevel[i], itemMinLevel[i], itemType[i], itemSubType[i], itemStackCount[i], itemEquipLoc[i], itemTexture[i] = GetItemInfo(itemid);
        getglobal("ItemFormSearchLabelItemID"..i.."Label"):SetText(SetQuality(itemRarity[i])..itemNameReal[i]);
        getglobal("ItemFormSearchTexture"..i.."Texture"):SetTexture(itemTexture[i]);
    else
        getglobal("ItemFormSearchLabelItemID"..i.."Label"):SetText(itemName[i]);
        getglobal("ItemFormSearchTexture"..i.."Texture"):SetTexture("Interface\\Icons\\INV_Misc_QuestionMark");
    end
end

-- When a button is rolled over, show tooltip and update vars based on user cache
function ResultButton_OnEnter(button_number)
    GameTooltip:ClearLines();
    GameTooltip:SetOwner(this, "ANCHOR_RIGHT", -(this:GetWidth() / 2), 24);
    GameTooltip:SetHyperlink("item:"..item_search_results[button_number]..":0:0:0:0:0:0:0");
    if GetItemInfo(item_search_results[button_number]) then
        itemNameReal[button_number], itemLink[button_number], itemRarity[button_number], itemLevel[button_number], itemMinLevel[button_number], itemType[button_number], itemSubType[button_number], itemStackCount[button_number], itemEquipLoc[button_number], itemTexture[button_number] = GetItemInfo(item_search_results[button_number]);
        getglobal("ItemFormSearchLabelItemID"..button_number.."Label"):SetText(SetQuality(itemRarity[button_number])..itemNameReal[button_number]);
        getglobal("ItemFormSearchTexture"..button_number.."Texture"):SetTexture(itemTexture[button_number]);
	GameTooltip:AddLine("Item ID: "..item_search_results[button_number]);
	GameTooltip:AddLine("(click to add to inventory)");
    else
	GameTooltip:ClearLines();
	GameTooltip:AddLine("|c00B0E0E6"..itemName[button_number]);
	GameTooltip:AddLine("|c007CFC00- Now Cached -");
	GameTooltip:AddLine("|c007CFC00(roll over this again to get full info)");
	GameTooltip:AddLine("(click to add to inventory)");
    end
    GameTooltip:Show();
end

-- Hide the tooltip when mouse leaves a button
function ResultButton_OnLeave()
    GameTooltip:Hide();
end

-- When a button is clicked, add to inventory
function ResultButton_OnClick(button_number)
    text = ".character additem "..item_search_results[button_number];
    outSAY(text);
end

---------------------------------------------
--            End of Item search           --
---------------------------------------------