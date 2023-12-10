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

#include "Network/HTTPClient.h"

#include "Game/Weather/OpenWeatherData.h"
#include "Game/Weather/OpenWeatherDataParser.h"

OpenWeatherProvider::OpenWeatherProvider()
{
	m_holder = sEventMgr.GetEventHolder( WORLD_INSTANCE );
	weatherData = new WeatherData();

	apiURL = Config.MainConfig.GetStringDefault( "WeatherSettings", "URL", "" );
	if( apiURL.length() == 0 )
	{
		Log.Error( "OpenWeatherProvider", "You need a valid URL with a valid API key to use this provider!" );
	}
}

OpenWeatherProvider::~OpenWeatherProvider()
{
	delete weatherData;
	weatherData = NULL;
}

void OpenWeatherProvider::onStartup()
{
	/// Start periodic weather updates
	updateWeather();
}

void OpenWeatherProvider::sendWeather( Player *player )
{
	WorldPacket data( SMSG_WEATHER, 9 );
	WeatherPacketBuilder builder( weatherData->type, weatherData->density );
	builder.buildPacket( data );
	
	PlayerMessenger::sendMessage( player, data );
}

void OpenWeatherProvider::setEnableGeneratedWeather( uint32 zone, bool enabled )
{
	Log.Notice( "OpenWeatherProvider", "setEnabledGeneratedWeather is not supported in this provider." );
}

void OpenWeatherProvider::setWeather( uint32 zone, uint32 type, float density )
{
	Log.Notice( "OpenWeatherProvider", "setWeather is not supported in this provider." );
}

#define WEATHER_UPDATE_INTERVAL 5*6*1000

void OpenWeatherProvider::updateWeather()
{
	Log.Debug( "OpenWeatherProvider", "Updating weather data" );

	if( apiURL.length() == 0 )
	{
		Log.Error( "OpenWeatherProvider", "You need a valid URL with a valid API key to use this provider!" );
	}

	sEventMgr.RemoveEvents( this, EVENT_WEATHER_UPDATE );

	/// Retrieve the weather report
	HTTPClient client;
	HTTPRequest request( apiURL, HTTPRequest::HTTP_GET );
	if( client.performRequest( request ) )
	{
		HTTPResponse response = client.getResponse();
		if( response.code == 200 )
		{
			std::string &s = response.body;

			/// Parse the weather data
			OpenWeatherData openWeatherData;
			if( OpenWeatherDataParser::parse( s, openWeatherData ) )
			{
				openWeatherData.buildWeatherData( *weatherData );
				
				Log.Notice( "OpenWeatherProvider", "New weather type %u", weatherData->type );
				Log.Notice( "OpenWeatherProvider", "New weather density %f", weatherData->density );

				/// Send the new weather to players
				WorldPacket data( SMSG_WEATHER, 9 );
				WeatherPacketBuilder builder( weatherData->type, weatherData->density );
				builder.buildPacket( data );
				sWorld.SendGlobalMessage( &data, NULL );
			}
			else
			{
				Log.Error( "OpenWeatherProvider", "Error parsing OpenWeather response." );
			}

		}
		else
		{
			Log.Error( "OpenWeatherProvider", "Failed to query OpenWeather API: HTTP code: %ld", response.code );
		}
	}	
	else
	{
		Log.Error( "OpenWeatherProvider", "Failed to query OpenWeather API: %s", client.getLastErrorText() );
	}

	sEventMgr.AddEvent( this, &OpenWeatherProvider::updateWeather, EVENT_WEATHER_UPDATE, WEATHER_UPDATE_INTERVAL, 0, 0 );

	Log.Debug( "OpenWeatherProvider", "Finished updating weather data" );
}
