--?!MAP=558
assert( include("../dungeon.lua") , "Failed to load dungeon.lua")
local instance_mod = require("DUNGEON_AUCHINDOUN")
if(type(instance_mod) ~= "table") then error("Auchindoun Module is missing!",1) end
module(instance_mod._NAME..".INSTANCE_ACRYPTS",package.seeall)
print("loaded acrypts.lua")