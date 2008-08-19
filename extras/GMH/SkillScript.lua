function SearchSkill()
result=".lookup skill "..SkillName:GetText();
outSAY(result);
end

function LearnSkill()
result=".character advancesk "..SkillNumber:GetText().." "..SkillLvl:GetText().." "..SkillMax:GetText();        
outSAY(result);
end

function UnLearnSkill()
result=".character removesk "..SkillNumber:GetText();    
outSAY(result);
end

function AdvanceAll()
result=".character advanceallskills "..SkillsBy:GetText();   
outSAY(result);
end
