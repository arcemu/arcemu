--?!MAP=555
assert( include("../dungeon.lua") ,"Failed to load dungeon.lua")
local instance_mod = require("DUNGEON_AUCHINDOUN")
assert(instance_mod)
module(instance_mod._NAME..".INSTANCE_SHADOW_LABYRINTH",package.seeall)
print("loaded shadowlabyrinth.lua")