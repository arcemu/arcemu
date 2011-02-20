--Make sure the thread running this code is a map thread.
if(MapMgr == nil) then return end
--Establish the auchindoun namespace
module("DUNGEON_AUCHINDOUN",package.seeall)
--Lets begin the loading of our script files.
--Establish a table that contains the map_ids and loader files for each instance in auchindoun.
local CRYPTS = 558
local TOMBS = 557
local HALLS = 556
local LABY = 555

local info = debug.getinfo(1)
local src = string.reverse(info.source)
local file_sep
if(GetPlatform() == "Win32") then
	file_sep = "\\"
else
	file_sep = '/'
end
-- Directory resolving algo
local startpos,endpos = string.find(src,file_sep),string.len(src)
--check and skip '@' character in the dir.
if(string.find(src, '@') ~= nil) then endpos = endpos -1 end
local directory = string.reverse(string.sub(src,startpos,endpos))

local scripts = {
	[CRYPTS] = directory.."AUCHENAI_CRYPTS"..file_sep.."Instance_ACrypts.loader",
	[TOMBS] = directory.."MANA_TOMBS"..file_sep.."Instance_ManaTombs.loader",
	[HALLS] = directory.."SETHEKK_HALLS"..file_sep.."Instance_SethekkHalls.loader",
	[LABY] = directory.."SHADOW_LABYRINTH"..file_sep.."Instance_ShadowLabyrinth.loader"
}
--Retrieve the map id of the thread executing this lua code
local id = MapMgr:GetMapID()
--Now we simply look up the id and load the file associated with it.
if( scripts[id] ~= nil) then
	--Load it
	print("Auchindoun, loading file %"..scripts[id].."%.")
	dofile( scripts[id] )
end