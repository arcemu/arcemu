local instance_mod = require("DUNGEON_AUCHINDOUN")
if(type(instance_mod) ~= "table") then error("Auchindoun Module is missing!",1) end
module(instance_mod._NAME..".INSTANCE_ACRYPTS",package.seeall)
local mod = getfenv(1)
assert(type(mod) == "table")
local info = debug.getinfo(1)
local src = string.reverse(string.sub(info.source,2,string.len(info.source)))
local file_sep;
if(GetPlatform() == "Win32") then
	file_sep = "\\"
else
	file_sep = "/"
end
-- Directory resolving algo
local startpos,endpos = string.find(src,file_sep),string.len(src)
local directory = string.reverse(string.sub(src,startpos,endpos))
--now we construct our modules based on the resolved directory.
local modules = { directory.."shirrak.script",directory.."maladaar.script"}
for _,v in pairs(modules) do
	local loader_function,errormsg= loadfile(v)
	if(loader_function == nil) then
		print(errormsg)
	else
		setfenv(loader_function,mod)
		local ret,errormsg = pcall(loader_function)
		if(ret == false) then
			print(errormsg)
		end
	end
end