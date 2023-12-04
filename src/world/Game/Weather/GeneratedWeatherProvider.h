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

#ifndef ARCEMU_GENERATEDWEATHERPROVIDER_H
#define ARCEMU_GENERATEDWEATHERPROVIDER_H

/// Generated weather using parameters loaded from the database
class GeneratedWeatherProvider : public WeatherProvider
{
public:
	GeneratedWeatherProvider();
	~GeneratedWeatherProvider();
	void onStartup();
	void sendWeather( Player *player );
	void setEnableGeneratedWeather( uint32 zone, bool enable );
	void setWeather( uint32 zone, uint32 type, float density );

	static WeatherProvider* create(){ return new GeneratedWeatherProvider(); }

private:
	/// Load data for weather generation
	void loadFromDB();
	
	/// Weather data for the zones
	std::map< uint32, WeatherInfo* > m_zoneWeathers;
};

#endif
