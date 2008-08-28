function RemoveItem()
result=".character removeitem "..ItemNumber:GetText();    
outSAY(result);
end

function AddItem()
if ItemNumber:GetText() == "" then
UIErrorsFrame:AddMessage("Specify an Item Number or Name!", 1.0, 0.0, 0.0, 53, 2);
else
result=".character additem "..ItemNumber:GetText().." "..ItemQuantity1:GetText();    
outSAY(result);
end
end

function AddItemSet()
result=".character additemset "..ItemSetNumber:GetText();    
outSAY(result);
end

function SearchItem()
result=".lookup item "..ItemNumber:GetText();    
outSAY(result);
end

function AddMoney()
IntGold = Gold:GetNumber();
IntSilver = Silver:GetNumber();
IntCopper = Copper:GetNumber();
-- 214748g 35s 47c
--[[while IntGold >= 214748 do
	 IntGold = IntGold-100;
end
while IntSilver >= 100 do
	 IntGold = IntGold-100;
end
while IntGold >= 100 do
	 IntGold = IntGold-100;
end]]
result = ".modify gold " ..IntGold*10000+IntSilver*100+IntCopper;
outSAY(result);
end

---------------------------------------------
--          End of Advance Command         --
---------------------------------------------