function CreateGuild()
result=".guild create %t"..GuildName:GetText();  
outSAY(result);
end

function LevelPlayer()
result=".mod level "..PlayerLevel:GetText();    
outSAY(result);
end
