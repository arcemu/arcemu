--[[ Global Variable Management with automagic global table cleaning, and Lua garbage collection.
*********************************
*								*
* The LUA++ Scripting Project	*
*								*
*********************************

This software is provided as free and open source by the
staff of The LUA++ Scripting Project, in accordance with 
the AGPL license. This means we provide the software we have 
created freely and it has been thoroughly tested to work for 
the developers, but NO GUARANTEE is made it will work for you 
as well. Please give credit where credit is due, if modifying,
redistributing and/or using this software. Thank you.

~~End of License Agreement
-- LUA++ staff, March 26, 2008. ]]
--module ("global_var_management", package.seeall)

_globalvars_ = {};
_globalvars_["defCleanup"] = os.time();
_globalvars_["Garbage"] = {time=os.time(), memsize=nil};

local do_garbagecollection = function(secs)
	if (type(_globalvars_.Garbage.time) ~= "number") then _globalvars_.Garbage.time = os.time(); return; end
	local crtTime = os.time();
	if ( (crtTime - _globalvars_.Garbage.time) >= secs ) then
		-- collect garbage
		print("Performing garbage collection...");
		_globalvars_.Garbage.time = crtTime;
		--_globalvars_.Garbage.memsize = collectgarbage("count"); -- shows how much memory Lua is using in Kbytes before garbage collection.
		collectgarbage("collect"); -- FULL collection cycle.
	end
	return;
end

local clean_table = function(secs)
	-- Clean up the table based on access time.
	if (table.getns(_globalvars_) >= 3) then
		local crtTime = os.time();
		local defCount = _globalvars_.defCleanup;
		if ( math.abs((crtTime-defCount)) >= secs ) then
			-- perform a clean up of stale tables
			print("Performing clean up check for stale tables...");
			_globalvars_["defCleanup"] = os.time();
			for j,k in pairs(_globalvars_) do
				if ((k.tblaccTime ~= nil) and (k.tblaccTime ~= 0)) then
					-- If the tables access time is >= %secs then clean the stale entry.
					local val = (crtTime - math.abs(k.tblaccTime));
					if (val >= secs) then
						print("cleaned stale table->_globalvars_."..tostring(j)..": was "..tostring(val).." seconds old.");
						_globalvars_[j] = nil;
					end
				end
			end
		end
	end				
	return;
end

function setvars(Unit, tblArgs)
	-- error checking; if returns false for any reason error()
	-- will fire off an error message and break further execution of the function.
	-- But will also return calling function line number.
	if (type(Unit) ~= "userdata") then
		error("setvars arg 1 expecting userdata, got "..type(Unit).."->"..tostring(Unit), 2);
		return nil;
	elseif ( (type(tblArgs) ~= "table") and (tblArgs == false) ) then
		error("setvars arg 2 expecting table or boolean value true, got "..type(tblArgs).."->"..tostring(tblArgs), 2);
		return nil;
	end

	-- New instance specific addition so global vars are not limited to specific Unit, but accessible by instance.
	-- All Units within the same instance can access the same table elements.
	local instance = tostring(Unit:GetInstanceID());
	local unit = tostring(Unit)
	
	if (instance == "nil") then
		-- If instance is nil then use per Unit.
		if (tblArgs == true) then
			-- Remove Unit from globalvars table
			if (_globalvars_[unit] ~= nil) then _globalvars_[unit] = nil; end
			return;
		end
		_globalvars_[unit] = {tblaccTime=os.time(), args=tblArgs};
	else
		-- Per instance global management
		if (tblArgs == true) then
			-- Remove instance from globalvars table
			if (_globalvars_["instance "..instance] ~= nil) then _globalvars_["instance "..instance] = nil; end
			return;
		end
		_globalvars_["instance "..instance] = {tblaccTime=os.time(), args=tblArgs};
	end
	
	clean_table(300); -- clean any stale entries. 300 seconds = 5 minutes.
	do_garbagecollection(900); -- collect garbage. 900 seconds = 15 minutes.
	
end

function modvars(Unit, arg, value)
	-- error checking; if returns false for any reason error()
	-- will fire off an error message and break further execution of the function.
	-- But will also return calling function line number.
	if (type(Unit) ~= "userdata") then
		error("modvars arg 1 expecting userdata, got "..type(Unit).."->"..tostring(Unit), 2);
		return nil;
	elseif (type(arg) == "nil") then
		error("modvars arg 2 expecting non-nil, got nil", 2);
		return nil;
	end

	-- New instance specific addition so global vars are not limited to specific Unit, but accessible by instance.
	-- All Units within the same instance can access the same table elements.
	local instance = tostring(Unit:GetInstanceID());
	local unit = tostring(Unit)
	
	if (instance == "nil") then
		-- If instance is nil then use per Unit.
		_globalvars_[unit].tblaccTime=os.time()
		_globalvars_[unit].args[arg] = value
	else
		-- Per instance global management
		_globalvars_["instance "..instance].tblaccTime=os.time()
		_globalvars_["instance "..instance].args[arg] = value
	end
	
	clean_table(300); -- clean any stale entries. 300 seconds = 5 minutes.
	do_garbagecollection(900); -- collect garbage. 900 seconds = 15 minutes.
end

function getvars(Unit)
	-- error checking; if returns false for any reason error()
	-- will fire off an error message and break further execution of the function.
	-- But will also return calling function line number.
	if (type(Unit) ~= "userdata") then
		error("getvars arg 1 expecting userdata, got "..type(Unit).."->"..tostring(Unit), 2);
		return nil;
	end

	-- New instance specific addition so global vars are not limited to specific Unit, but accessible by instance.
	-- All Units within the same instance can access the same table elements.
	local instance = tostring(Unit:GetInstanceID());
	local unit = tostring(Unit)

	if (instance == "nil") then
		if (_globalvars_[unit] == nil) then
			error(	"getvars attempting to get data from nonexistent table entry", 2);
			return nil; -- incase for some reason error doesn't terminate function
		end
		_globalvars_[unit].tblaccTime=os.time();
		return _globalvars_[unit].args;
	else
		if (_globalvars_["instance "..instance] == nil) then
			error(	"getvars attempting to get data from nonexistent table entry", 2);
			return nil; -- incase for some reason error doesn't terminate function
		end
		_globalvars_["instance "..instance].tblaccTime=os.time();
		return _globalvars_["instance "..instance].args;
	end
	
end

-- do math.randomseed here so folks don't have to add it to their scripts.
math.randomseed(os.time());
-- pop off a few randoms to make sure we get a nice random number when needed
-- *Note: This isn't a fool proof way to get a ''good'' random number. There are many ways
---- to implement a good randomseed generator but for sake of simplicity we'll do it this way.
math.random();
math.random();
math.random();
math.random();