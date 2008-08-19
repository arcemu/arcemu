function TargetObject()
result=".go select ";    
outSAY(result);
end

function ObjectInfo()
result=".go info "; 
outSAY(result);
end

function DeleteObject()
result=".go delete ";    
outSAY(result);
end

function PlaceObject()
if( NoSaveCheck:GetChecked() ) then
        result=".go spawn "..ObjectNumber:GetText();
    else
        result=".go spawn "..ObjectNumber:GetText().." 1";
    end
outSAY(result)
end
