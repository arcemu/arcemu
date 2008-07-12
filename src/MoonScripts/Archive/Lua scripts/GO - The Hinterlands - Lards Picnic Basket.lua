

function Lards_Picnic_Basket_onUse (pUnit, Event)
	pUnit:SpawnCreature (14748, 401.681061, -4825.897949, 9.381310, 3.16951, 95, 540000)
	pUnit:SpawnCreature (14748, 403.259125, -4830.528320, 8.991310, 3.16951, 95, 540000)
	pUnit:SpawnCreature (14748, 404.802185, -4820.998535, 10.790082, 3.16951, 95, 540000)
end
RegisterGameObjectEvent (179910, 2, "Lards_Picnic_Basket_onUse")