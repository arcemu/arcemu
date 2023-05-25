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
import arcemu
from arcemu import Unit

GNOMEREGAN_PRIDE_SPELLID = 75531

def GnomereganPride_handleAura( i, aura, apply ):
	target = aura.getTarget()
	
	if apply:
		target.setDisplayId( 31654 )
		print( "Applying gp aura" )
	else:
		target.setDisplayId( target.getNativeDisplayId() )
		print( "Removing gp aura" )
		
arcemu.RegisterDummyAuraHandler( GNOMEREGAN_PRIDE_SPELLID, GnomereganPride_handleAura )
