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

function NPCCome()
    result=".npc come";
    outSAY(result);
end

function NPCPos()
    result=".npc possess";
    outSAY(result);
end

function NPCInfo()
result=".npc info";
outSAY(result);
end

