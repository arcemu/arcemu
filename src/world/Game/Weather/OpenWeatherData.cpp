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
#include "Game/Weather/OpenWeatherData.h"

void OpenWeatherData::buildWeatherData( WeatherData &data )
{
	/// OpenWeatherMap returns more than one phenomena, the first one seems to be the dominant
	uint32 primaryCode = currentWeather.codes[ 0 ];

	/// First we set the type of the weather

	if( primaryCode >= 200 && primaryCode <= 232 )
	{
		data.type = WEATHER_TYPE_THUNDERSTORM;
	}
	else
	if( primaryCode >= 300 && primaryCode <= 321 )
	{
		data.type = WEATHER_TYPE_LIGHT_RAIN;
	}
	else
	if( ( primaryCode >= 701 && primaryCode <= 781 ) || ( primaryCode >= 802 && primaryCode <= 804 ) )
	{
		data.type = WEATHER_TYPE_FOG;
	}
	else
	if( primaryCode >= 800 && primaryCode <= 801 )
	{
		data.type = WEATHER_TYPE_NORMAL;
	}
	else
	{
		switch( primaryCode )
		{
			case 500: data.type = WEATHER_TYPE_LIGHT_RAIN; break;
			case 501: data.type = WEATHER_TYPE_MEDIUM_RAIN; break;
			case 502: data.type = WEATHER_TYPE_HEAVY_RAIN; break;
			case 503: data.type = WEATHER_TYPE_VERY_HEAVY_RAIN; break;
			case 504: data.type = WEATHER_TYPE_VERY_HEAVY_RAIN; break;
			case 511: data.type = WEATHER_TYPE_LIGHT_RAIN; break;
			case 520: data.type = WEATHER_TYPE_LIGHT_RAIN; break;
			case 521: data.type = WEATHER_TYPE_MEDIUM_RAIN; break;
			case 522: data.type = WEATHER_TYPE_HEAVY_RAIN; break;
			case 531: data.type = WEATHER_TYPE_LIGHT_RAIN; break;
				
			case 600: data.type = WEATHER_TYPE_LIGHT_SNOW; break;
			case 601: data.type = WEATHER_TYPE_MEDIUM_SNOW; break;
			case 602: data.type = WEATHER_TYPE_HEAVY_SNOW; break;
			case 611: data.type = WEATHER_TYPE_MEDIUM_SNOW; break;
			case 612: data.type = WEATHER_TYPE_LIGHT_SNOW; break;
			case 613: data.type = WEATHER_TYPE_MEDIUM_SNOW; break;
			case 615: data.type = WEATHER_TYPE_LIGHT_SNOW; break;
			case 616: data.type = WEATHER_TYPE_MEDIUM_SNOW; break;
			case 620: data.type = WEATHER_TYPE_LIGHT_SNOW; break;
			case 621: data.type = WEATHER_TYPE_MEDIUM_SNOW; break;
			case 622: data.type = WEATHER_TYPE_LIGHT_SNOW; break;
		}
	}

	/// Then we set the density

	/// Let's set a default density
	if( data.type == WEATHER_TYPE_NORMAL )
	{
		data.density = 0.0f;
	}
	else
	{
		data.density = WEATHER_DENSITY_MIN;
	}

	switch( primaryCode )
	{
		case 200: data.density = WEATHER_DENSITY_MIN; break;
		case 201: data.density = WEATHER_DENSITY_MID; break;
		case 202: data.density = WEATHER_DENSITY_MAX; break;
		case 210: data.density = WEATHER_DENSITY_MIN; break;
		case 211: data.density = WEATHER_DENSITY_MID; break;
		case 212: data.density = WEATHER_DENSITY_MAX; break;
		case 221: data.density = WEATHER_DENSITY_MIN; break;
		case 230: data.density = WEATHER_DENSITY_MIN; break;
		case 231: data.density = WEATHER_DENSITY_MID; break;
		case 232: data.density = WEATHER_DENSITY_MAX; break;

		case 300: data.density = WEATHER_DENSITY_MIN; break;
		case 301: data.density = WEATHER_DENSITY_MID; break;
		case 302: data.density = WEATHER_DENSITY_MAX; break;
		case 310: data.density = WEATHER_DENSITY_MIN; break;
		case 311: data.density = WEATHER_DENSITY_MID; break;
		case 312: data.density = WEATHER_DENSITY_MAX; break;
		case 313: data.density = WEATHER_DENSITY_MID; break;
		case 314: data.density = WEATHER_DENSITY_MAX; break;
		case 321: data.density = WEATHER_DENSITY_MIN; break;

		case 500: data.density = WEATHER_DENSITY_MID; break;
		case 501: data.density = WEATHER_DENSITY_MID; break;
		case 502: data.density = WEATHER_DENSITY_MAX; break;
		case 503: data.density = WEATHER_DENSITY_MAX; break;
		case 504: data.density = WEATHER_DENSITY_MAX; break;
		case 511: data.density = WEATHER_DENSITY_MID; break;
		case 520: data.density = WEATHER_DENSITY_MID; break;
		case 521: data.density = WEATHER_DENSITY_MID; break;
		case 522: data.density = WEATHER_DENSITY_MAX; break;
		case 531: data.density = WEATHER_DENSITY_MIN; break;

		case 600: data.density = WEATHER_DENSITY_MID; break;
		case 601: data.density = WEATHER_DENSITY_MID; break;
		case 602: data.density = WEATHER_DENSITY_MAX; break;
		case 611: data.density = WEATHER_DENSITY_MID; break;
		case 612: data.density = WEATHER_DENSITY_MIN; break;
		case 613: data.density = WEATHER_DENSITY_MID; break;
		case 615: data.density = WEATHER_DENSITY_MID; break;
		case 616: data.density = WEATHER_DENSITY_MAX; break;
		case 620: data.density = WEATHER_DENSITY_MIN; break;
		case 621: data.density = WEATHER_DENSITY_MID; break;
		case 622: data.density = WEATHER_DENSITY_MAX; break;
	}
}
