#
# ArcEmu MMORPG Server
# Copyright (C) 2008-2023 <http://www.ArcEmu.org/>
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
# 
# You should have received a copy of the GNU Affero General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
# 
#
# Dummy spell handler script for the spell 'Deploy Radiageigatron' (73082)
# for the quest Vent Horizon (25212)
# 
# The player is required to deploy (throw) a Radiageigatron to 3 Gnomeregan vents to finish the quest
# When the device lands the spell procs the dummy effect.
# The dummy effect checks whether the deployed Radiageigatron has hit a vent (based on coords) and gives quest credit accordingly
#

import arcemu
import ArcPyMath as Math

RADIAGEIGATRON_SPELL_ID = 73082
VENT_HORIZON_QUEST_ID = 25212

gnomereganVentCoords = [
[ -5018.71630859375, 545.6546630859375, 473.5934143066406 ],
[ -5125.66455078125, 594.12646484375, 459.75164794921875 ],
[ -5158.83154296875, 630.7918701171875, 465.70660400390625 ],
[ -5223.40625, 625.9254760742188, 455.8747253417969 ],
[ -5303.7109375, 664.9591064453125, 449.4031982421875 ],
[ -5347.95361328125, 646.2954711914062, 445.67156982421875 ],

[ -5300.9462890625, 555.1154174804688, 386.9750671386719 ],
[ -5300.75, 467.29205322265625, 385.9061584472656 ],
[ -5272.58203125, 469.91522216796875, 385.5768127441406 ]
]

def isDestNearVent( x, y, z ):
	hit = False
	
	for v in gnomereganVentCoords:
		dist = Math.calcDistance( x, y, z, v[ 0 ], v[ 1 ], v[ 2 ] )

		if dist <= 6.5:
			hit = True
			break
			
	return hit

def deploy_radiageigatron_handleDummyEffect( effectIndex, spell ):
	playerCaster = spell.getPlayerCaster()

	if playerCaster is None:
		return True
		
	destX = spell.getTargetDestinationX()
	destY = spell.getTargetDestinationY()
	destZ = spell.getTargetDestinationZ()
	
	hit = isDestNearVent( destX, destY, destZ )
	if hit:
		playerCaster.addQuestKill( VENT_HORIZON_QUEST_ID, 0, 0 )
	
	return True

arcemu.RegisterDummySpellHandler( RADIAGEIGATRON_SPELL_ID, deploy_radiageigatron_handleDummyEffect )
