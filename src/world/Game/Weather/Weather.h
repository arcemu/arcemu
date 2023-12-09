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
 
#ifndef ARCEMU_WEATHER_H
#define ARCEMU_WEATHER_H

enum WeatherType
{
	WEATHER_TYPE_NORMAL            = 0,
	WEATHER_TYPE_FOG               = 1,
	WEATHER_TYPE_LIGHT_RAIN        = 2,
	WEATHER_TYPE_MEDIUM_RAIN       = 3,
	WEATHER_TYPE_HEAVY_RAIN        = 4,
	WEATHER_TYPE_VERY_HEAVY_RAIN   = 5,
	WEATHER_TYPE_LIGHT_SNOW        = 6,
	WEATHER_TYPE_MEDIUM_SNOW       = 7,
	WEATHER_TYPE_HEAVY_SNOW        = 8,
	
	WEATHER_TYPE_LIGHT_SANDSTORM   = 41,
	WEATHER_TYPE_SANDSTORM         = 42,
	
	WEATHER_TYPE_THUNDERSTORM      = 86,
	WEATHER_TYPE_BLACK_RAIN        = 90
};

/// Tells if the specified numeric value is a valid weather type
ARCEMU_INLINE bool isValidWeatherType( uint32 type )
{
	switch( type )
	{
	case WEATHER_TYPE_NORMAL:
	case WEATHER_TYPE_FOG:
	case WEATHER_TYPE_LIGHT_RAIN:
	case WEATHER_TYPE_MEDIUM_RAIN:
	case WEATHER_TYPE_HEAVY_RAIN:
	case WEATHER_TYPE_VERY_HEAVY_RAIN:
	case WEATHER_TYPE_LIGHT_SNOW:
	case WEATHER_TYPE_MEDIUM_SNOW:
	case WEATHER_TYPE_HEAVY_SNOW:
	case WEATHER_TYPE_LIGHT_SANDSTORM:
	case WEATHER_TYPE_SANDSTORM:
	case WEATHER_TYPE_THUNDERSTORM:
	case WEATHER_TYPE_BLACK_RAIN:
		return true;
	}

	return false;
}

enum WeatherSound
{
    WEATHER_NOSOUND                = 0,
    WEATHER_RAINLIGHT              = 8533,
    WEATHER_RAINMEDIUM             = 8534,
    WEATHER_RAINHEAVY              = 8535,
    WEATHER_SNOWLIGHT              = 8536,
    WEATHER_SNOWMEDIUM             = 8537,
    WEATHER_SNOWHEAVY              = 8538,
    WEATHER_SANDSTORMLIGHT         = 8556,
    WEATHER_SANDSTORMMEDIUM        = 8557,
    WEATHER_SANDSTORMHEAVY         = 8558
};

#define WEATHER_DENSITY_MIN 0.3f
#define WEATHER_DENSITY_MID 1.15f
#define WEATHER_DENSITY_MAX 2.0f

#endif
 