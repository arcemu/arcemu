function Strawman_Burning_Straw(Unit, event, miscunit, misc)
	print "Strawman Burning Straw"
	Unit:FullCastSpellOnTarget(31075,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Ho, god of sun, let's burn those peoples...")
end

function Strawman_Brain_Bash(Unit, event, miscunit, misc)
	print "Strawman Brain Bash"
	Unit:FullCastSpellOnTarget(31046,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Let's crush your brain...")
end

function Strawman(unit, event, miscunit, misc)
	print "Strawman"
	unit:RegisterEvent("Strawman_Burning_Straw",10000,0)
	unit:RegisterEvent("Strawman_Brain_Bash",17000,0)
end

RegisterUnitEvent(17543,1,"Strawman")