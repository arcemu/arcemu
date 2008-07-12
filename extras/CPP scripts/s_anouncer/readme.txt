Copy this as is your "scripts" folder.
open announcer.cpp and edit :
	MSG_TO_ANNOUNCE to say the desired text 
	ANNOUNCE_INTERVAL for the interval to say it
	ANNOUNCE_RANGE_SQR the range in what the npc will send message around him
	MSG_TYPE - default is set to wisper (players will wisper themselfs)
	UNIT_ENTRY_FOR_SPAWN_AS_ANNOUNCER - to adjust the announcer look. Mob will be made untouchable after spawned
compile it and it will create a dll in scripts_bin directory. On server load this will be loaded

I recommand using a static spawn for this npc in case announce range is bigger then default visibility range.