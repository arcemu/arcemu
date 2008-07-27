

function BlizzconBand_Spawn_On_MobDied (Unit)

  Unit:SendChatMessage(12, 0, "ARE YOU READY TO ROCK N ROOOOOLLLLL!!!");

  ---Spawn in band formation in front of Gurubashi Arena.  
  Unit:SpawnCreature(23624, -13245.6, 210.251, 30.8405, 3.53443, 1741, 120000) --- Rhythm Guitar Player
  Unit:SpawnCreature(23623, -13250.8, 211.854, 30.855, -0.78326, 1741, 120000) --- Drummer
  Unit:SpawnCreature(23626, -13251.1, 207.387, 30.8936, 5.54681, 1741, 120000) --- Bass Player
  Unit:SpawnCreature(23625, -13248.8, 205.170, 30.8996, 0.723238, 1741, 120000) --- Singer
  Unit:SpawnCreature(23619, -13254.2, 206.523, 30.9156, 5.31983, 1741, 120000) --- Lead Guitar Player

  Unit:PlaySoundToSet(11803)
  
  Unit:Despawn(120000, 0);
  
  Unit:RemoveEvents()
  
  
  

end

RegisterUnitEvent(66666, 4, "BlizzconBand_Spawn_On_MobDied")









