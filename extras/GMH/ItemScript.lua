function RemoveItem()
result=".character removeitem "..ItemNumber:GetText();    
outSAY(result);
end

function AddItem()
result=".character additem "..ItemNumber:GetText();    
outSAY(result);
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
result = ".modify gold " ..Gold:GetNumber()*10000+Silver:GetNumber()*100+Copper:GetNumber();
outSAY(result);
end
