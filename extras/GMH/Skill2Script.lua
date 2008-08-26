function LearnProf123()
     if (ProfessionComboButton:GetText() == Alchemy) then
     result=".character advancesk 171 "..SkillLevel1:GetText();
     outSAY(result);
     elseif ( ProfessionComboButton:GetText() == Blacksmithing ) then
     result=".character advancesk 164 "..SkillLevel1:GetText();
     outSAY(result);
     elseif (ProfessionComboButton:GetText() == Enchanting) then
     result=".character advancesk 333 "..SkillLevel1:GetText();
     outSAY(result);
     elseif (ProfessionComboButton:GetText() == Engineering) then
     result=".character advancesk 202 "..SkillLevel1:GetText();
     outSAY(result);
     elseif (ProfessionComboButton:GetText() == Jewelcrafting) then
     result=".character advancesk 755 "..SkillLevel1:GetText(); 
     outSAY(result);
     elseif (ProfessionComboButton:GetText() == Leatherworking) then
     result=".character advancesk 165 "..SkillLevel1:GetText(); 
     outSAY(result);
     elseif (ProfessionComboButton:GetText() == Mining) then
     result=".character advancesk 186 "..SkillLevel1:GetText();
     outSAY(result);
     elseif (ProfessionComboButton:GetText() == Skinning) then
     result=".character advancesk 393 "..SkillLevel1:GetText();
     outSAY(result);
     elseif (ProfessionComboButton:GetText() == Tailoring) then
     result=".character advancesk 197 "..SkillLevel1:GetText(); 
     outSAY(result);
     elseif (ProfessionComboButton:GetText() == Cooking) then
     result=".character advancesk 185 "..SkillLevel1:GetText();
     outSAY(result);
     elseif (ProfessionComboButton:GetText() == Fishing) then
     result=".character advancesk 356 "..SkillLevel1:GetText();
     outSAY(result);
     elseif (ProfessionComboButton:GetText() == FirstAid) then
     result=".character advancesk 129 "..SkillLevel1:GetText();
     outSAY(result);
     elseif (ProfessionComboButton:GetText() == Poisons) then
     result=".character advancesk 40 "..SkillLevel1:GetText(); 
     outSAY(result);
     elseif (ProfessionComboButton:GetText() == Herbalism ) then
     result=".character advancesk 182 "..SkillLevel1:GetText();
     outSAY(result);
     end

end
--[[Alchemy
Blacksmithing
Enchanting
Engineering
Herbalism
Jewelcrafting
Leatherworking
Mining
Skinning
Tailoring
Cooking
Fishing
FirstAid
Poisons]]