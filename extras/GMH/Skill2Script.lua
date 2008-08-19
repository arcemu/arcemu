function LearnProf()
     if ( ProfessionComboButton:GetText() == Alchemy ) then
     result=".character advancesk 171 "..SkillLvl:GetText();
     elseif ( ProfessionComboButton:GetText() == Blacksmithing ) then
     result=".character advancesk 164 "..SkillLvl:GetText();
     elseif ( ProfessionComboButton:GetText() == Enchanting ) then
     result=".character advancesk 333 "..SkillLvl:GetText();
     elseif ( ProfessionComboButton:GetText() == Engineering ) then
     result=".character advancesk 202 "..SkillLvl:GetText();
     elseif ( ProfessionComboButton:GetText() == Herbalism ) then
     result=".character advancesk 182 "..SkillLvl:GetText();
     elseif ( ProfessionComboButton:GetText() == Jewelcrafting ) then
     result=".character advancesk 755 "..SkillLvl:GetText(); 
     elseif ( ProfessionComboButton:GetText() == Leatherworking ) then
     result=".character advancesk 165 "..SkillLvl:GetText(); 
     elseif ( ProfessionComboButton:GetText() == Mining ) then
     result=".character advancesk 186 "..SkillLvl:GetText();
     elseif ( ProfessionComboButton:GetText() == Skinning ) then
     result=".character advancesk 393 "..SkillLvl:GetText();
     elseif ( ProfessionComboButton:GetText() == Tailoring ) then
     result=".character advancesk 197 "..SkillLvl:GetText(); 
     elseif ( ProfessionComboButton:GetText() == Cooking ) then
     result=".character advancesk 185 "..SkillLvl:GetText();  
     elseif ( ProfessionComboButton:GetText() == Fishing ) then
     result=".character advancesk 356 "..SkillLvl:GetText();
     elseif ( ProfessionComboButton:GetText() == FirstAid ) then
     result=".character advancesk 129 "..SkillLvl:GetText();
     elseif ( ProfessionComboButton:GetText() == Poisons ) then
     result=".character advancesk 40 "..SkillLvl:GetText(); 
     else
     result="bloody error...";
     end
     outSAY(result);
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