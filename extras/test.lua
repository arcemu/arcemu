function test(unit, event, miscunit)
    local sname = [[hello, my name is]]..unit:GetName()..[[xD ]]..miscunit:GetName()
	print('myname = ', sname)
	print "hello world"
	print "hello world"
end

function koboldtick(unit)
	print "kobold tick!"
	unit:SendChatMessage(11, 0, "Goatse FOR TEH WIN!")
end

function onspawnkobold(unit, event, miscunit)
	unit:RegisterEvent("koboldtick", 1000, 0)
end

RegisterUnitEvent(100, 1, "test")
RegisterUnitEvent(6, 6, "onspawnkobold")
