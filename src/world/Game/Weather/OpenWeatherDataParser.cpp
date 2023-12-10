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
#include "Game/Weather/OpenWeatherDataParser.h"

const std::string OpenWeatherDataParser::BEFORE = "{\"id\":";
const std::string OpenWeatherDataParser::AFTER  = ",\"main\":";

bool OpenWeatherDataParser::parse( std::string &s, OpenWeatherData &data )
{
	/// Example response
	/*
		{"lat":47.498,"lon":19.0399,"timezone":"Europe/Budapest","timezone_offset":3600,
		"current":{"dt":1701964356,"sunrise":1701929832,"sunset":1701960801,"temp":0.97,"feels_like":-0.71,"pressure":1017,"humidity":95,"dew_point":0.26,"uvi":0,"clouds":100,"visibility":2600,"wind_speed":1.54,"wind_deg":310,
		"weather":[{"id":500,"main":"Rain","description":"light rain","icon":"10n"},{"id":600,"main":"Snow","description":"light snow","icon":"13n"},{"id":701,"main":"Mist","description":"mist","icon":"50n"}],"rain":{"1h":0.76}},"minutely":
	*/

	/// Would have been prettier to use a JSON parser here, but then we'd have to add a new dependency

	/// Find the beginning of the current weather block
	size_t startPos = s.find( "\"current\":{", 0 );
	if( startPos == std::string::npos )
	{
		return false;
	}
		
	/// Find the beginning of the weather data block
	startPos = s.find( "\"weather\":[", startPos );
	if( startPos == std::string::npos )
	{
		return false;
	}
		
	/// Find the end of the current weather block
	size_t endPos = s.find( "\"minutely\":", startPos );
	if( endPos == std::string::npos )
	{
		return false;
	}
		
	std::vector< uint32 > &codes = data.currentWeather.codes;

	/// There is always at least one entry in the current weather block
	/// We extract the weather phenomenon id/code from the entries
	size_t phraseStart = s.find( BEFORE, startPos );
	size_t phraseEnd   = s.find( AFTER, phraseStart );
		
	do
	{
		/// The first character of the code is actually after the colon
		phraseStart = phraseStart + BEFORE.size();

		/// Extract the code
		std::string phrase = s.substr( phraseStart, phraseEnd - phraseStart);
		uint32 code = strtoul( phrase.c_str(), NULL, 0 );
		codes.push_back( code );

		/// Find the next code if there's any
		phraseStart = s.find( BEFORE, phraseEnd );
		phraseEnd   = s.find( AFTER, phraseStart );
	}
	while( ( phraseStart != std::string::npos ) && ( phraseStart < endPos ) );

	return true;
}
