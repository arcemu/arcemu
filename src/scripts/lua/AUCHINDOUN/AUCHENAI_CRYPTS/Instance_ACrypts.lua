--?!MAP=558
local instance_mod = require("DUNGEON_AUCHINDOUN")
if(type(instance_mod) ~= "table") then error("Auchindoun Module is missing!",1) end
module(instance_mod._NAME..".INSTANCE_ACRYPTS",package.seeall)