function LearnSpell()
result=".character learn "..SpellNumber:GetText();    
outSAY(result);
end

function UnlearnSpell()
result=".character unlearn "..SpellNumber:GetText();    
outSAY(result);
end
