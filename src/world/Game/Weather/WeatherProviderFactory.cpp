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

WeatherProviderFactory::WeatherProviderFactory()
{
	factoryMethods[ "GeneratedWeatherProvider" ] = GeneratedWeatherProvider::create;
	factoryMethods[ "NullWeatherProvider" ] = NullWeatherProvider::create;
	factoryMethods[ "OpenWeatherProvider" ] = OpenWeatherProvider::create;
}

WeatherProviderFactory::~WeatherProviderFactory()
{
	factoryMethods.clear();
}

WeatherProvider* WeatherProviderFactory::createProvider( std::string providerName )
{
	std::map< std::string, WeatherProviderFactoryFct >::const_iterator itr
		= factoryMethods.find( providerName );

	if( itr == factoryMethods.end() )
	{
		Log.Notice( "WeatherMgr", "Couldn't find provider '%s'. Using NullWeatherProvider instead.", providerName.c_str() );
		return new NullWeatherProvider();
	}

	return itr->second();
}
