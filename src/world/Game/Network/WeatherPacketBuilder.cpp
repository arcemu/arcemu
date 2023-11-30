/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2023 <http://www.ArcEmu.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "StdAfx.h"

void WeatherPacketBuilder::buildPacket( WorldPacket &data )
{
	data << uint32( type );
	data << float( density );
	data << uint32( sound );
	data << uint8( unknown );
}

void WeatherPacketBuilder::setSound()
{
	if( type == 0 || density <= 0.3f )
	{
		sound = WEATHER_NOSOUND;
		return;
	}
	
	switch( type )
	{
		case WEATHER_TYPE_RAIN:
		case WEATHER_TYPE_HEAVY_RAIN:
			if( density  < 0.40f )
				sound = WEATHER_RAINLIGHT;
			else
			if( density  < 0.70f )
				sound = WEATHER_RAINMEDIUM;
			else
				sound = WEATHER_RAINHEAVY;
			
			break;
		
		case WEATHER_TYPE_SNOW:
			if( density  < 0.40f )
				sound = WEATHER_SNOWLIGHT;
			else
				if( density  < 0.70f )
					sound = WEATHER_SNOWMEDIUM;
				else
					sound = WEATHER_SNOWHEAVY;
			
			break;
		
		case WEATHER_TYPE_SANDSTORM:
			if( density  < 0.40f )
				sound = WEATHER_SANDSTORMLIGHT;
			else
				if( density  < 0.70f )
					sound = WEATHER_SANDSTORMMEDIUM;
				else
					sound = WEATHER_SANDSTORMHEAVY;
			break;
	}
}
