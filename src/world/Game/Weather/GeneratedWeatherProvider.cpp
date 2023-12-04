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

GeneratedWeatherProvider::GeneratedWeatherProvider()
{
}

GeneratedWeatherProvider::~GeneratedWeatherProvider()
{
	std::map< uint32, WeatherInfo* >::iterator itr;
	for(itr = m_zoneWeathers.begin(); itr != m_zoneWeathers.end(); itr++)
	{
		delete itr->second;
	}

	m_zoneWeathers.clear();
}

void GeneratedWeatherProvider::onStartup()
{
	loadFromDB();
}

void GeneratedWeatherProvider::sendWeather( Player *player )
{
	std::map< uint32, WeatherInfo* >::iterator itr
		= m_zoneWeathers.find( player->GetZoneId() );

	if( itr == m_zoneWeathers.end() )
	{
		WorldPacket data( SMSG_WEATHER, 9 );
		WeatherPacketBuilder builder( 0, 0 );
		builder.buildPacket( data );
		player->GetSession()->SendPacket( &data );
		player->m_lastSeenWeather = 0;

		return;
	}
	else
	{
		itr->second->sendUpdate( player );
	}
}


void GeneratedWeatherProvider::setEnableGeneratedWeather( uint32 zone, bool enable )
{
	std::map< uint32, WeatherInfo* >::const_iterator itr
		= m_zoneWeathers.find( zone );

	if( itr == m_zoneWeathers.end() )
		return;

	itr->second->setEnableGeneratedWeather( enable );
}

void GeneratedWeatherProvider::setWeather( uint32 zone, uint32 type, float density )
{
	std::map< uint32, WeatherInfo* >::const_iterator itr
		= m_zoneWeathers.find( zone );

	if( itr == m_zoneWeathers.end() )
		return;

	itr->second->setWeather( type, density );
}

void GeneratedWeatherProvider::loadFromDB()
{
	Log.Notice( "WeatherMgr", "Loading weather data..." );

	QueryResult* result
		= WorldDatabase.Query( "SELECT zoneId,high_chance,high_type,med_chance,med_type,low_chance,low_type FROM weather" );

	if( result == NULL )
		return;

	do
	{
		Field* fields = result->Fetch();
		WeatherInfo* wi = new WeatherInfo;
		wi->m_zoneId = fields[0].GetUInt32();
		wi->weatherProbabilities.highProbabilityWeather.probability = fields[ 1 ].GetUInt32();
		wi->weatherProbabilities.highProbabilityWeather.type        = fields[ 2 ].GetUInt32();
		wi->weatherProbabilities.medProbabilityWeather.probability  = fields[ 3 ].GetUInt32();
		wi->weatherProbabilities.medProbabilityWeather.type         = fields[ 4 ].GetUInt32();
		wi->weatherProbabilities.lowProbabilityWeather.probability  = fields[ 5 ].GetUInt32();
		wi->weatherProbabilities.lowProbabilityWeather.type         = fields[ 6 ].GetUInt32();

		m_zoneWeathers[wi->m_zoneId] = wi;

		wi->generateWeather();
	}
	while( result->NextRow() );

	Log.Notice("WeatherMgr", "Loaded weather data for %u zones.", result->GetRowCount() );

	delete result;
}
