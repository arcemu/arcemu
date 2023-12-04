/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2012 <http://www.ArcEmu.org/>
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

initialiseSingleton( WeatherMgr );

WeatherMgr::WeatherMgr( const std::string providerName )
{
	WeatherProviderFactory factory;

	provider = factory.createProvider( providerName );
	Arcemu::Util::ArcemuAssert( provider != NULL );
}

WeatherMgr::~WeatherMgr()
{
	delete provider;
	provider = NULL;
}

void WeatherMgr::onStartup()
{
	provider->onStartup();
}

void WeatherMgr::SendWeather( Player* plr )
{
	provider->sendWeather( plr );
}

void WeatherMgr::setEnableGeneratedWeather( uint32 zone, bool enable )
{
	provider->setEnableGeneratedWeather( zone, enable );
}

void WeatherMgr::setWeather( uint32 zone, uint32 type, float density )
{
	provider->setWeather( zone, type, density );
}
