 --[[
   ******************************************************************
   *	 _____              ___                           _         *
   *	(  _  )            (  _`\               _        ( )_       *
   *	| (_) | _ __   ___ | (_(_)   ___  _ __ (_) _ _   | ,_)      *
   *	|  _  |( '__)/'___)`\__ \  /'___)( '__)| |( '_`\ | |        *
   *	| | | || |  ( (___ ( )_) |( (___ | |   | || (_) )| |_       *
   *	(_) (_)(_)  `\____)`\____)`\____)(_)   (_)| ,__/'`\__)      *
   *	                                          | |               *
   *	                                          (_)               *
   *	                                                            *
   *	               OpenSource Scripting Team                    *
   *	                <http://www.arcemu.org>                     *
   *	                                                            *
   ******************************************************************
  
   This software is provided as free and open source by the
staff of The ArcScript Project, in accordance with 
the GPL license. This means we provide the software we have 
created freely and it has been thoroughly tested to work for 
the developers, but NO GUARANTEE is made it will work for you 
as well. Please give credit where credit is due, if modifying,
redistributing and/or using this software. Thank you.

Staff of ArcScript Project, Feb 2008
~~End of License Agreement

#############################################################

-- ]]

function Lards_Picnic_Basket_onUse (pUnit, Event)
	pUnit:SpawnCreature (14748, 401.681061, -4825.897949, 9.381310, 3.16951, 95, 540000)
	pUnit:SpawnCreature (14748, 403.259125, -4830.528320, 8.991310, 3.16951, 95, 540000)
	pUnit:SpawnCreature (14748, 404.802185, -4820.998535, 10.790082, 3.16951, 95, 540000)
end
RegisterGameObjectEvent (179910, 2, "Lards_Picnic_Basket_onUse")