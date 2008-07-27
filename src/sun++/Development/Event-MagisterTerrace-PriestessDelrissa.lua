--[[
********************************
*                              *
*      The Moon Project        *
*                              *
********************************

This software is provided as free and open source by the
staff of The Moon Project, in accordance with
the GPL license. This means we provide the software we have
created freely and it has been thoroughly tested to work for
the developers, but NO GUARANTEE is made it will work for you
as well. Please give credit where credit is due, if modifying,
redistributing and/or using this software. Thank you.

Staff of Moon Project, Feb 2008
~~End of License Agreement
--Moon 2008]]


--[[Priestess Delrissa spawn function maybe like this??  ]]


function Priestess_Delrissa_Spawn(Unit, Event)
minionone = math.rand(1, 8)
miniontwo = math.rand(1, 8)
minionthree = math.rand (1, 8)
minionfour = math.rand(1, 8)

if minionone == 1 and Minion1used ~= 1 then
Unit:SpawnCreature(xyz, x, y, z, o, 1010101)
elseif minionone == 2 and Minion2used ~= 1 then
Unit:SpawnCreature(asdasd)


end
end




RegisterUnitEvent(24560, 10, "Priestess_Delrissa_Spawn")
