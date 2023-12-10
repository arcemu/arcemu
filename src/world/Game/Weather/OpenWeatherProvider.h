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

#ifndef ARCEMU_OPENWEATHERPROVIDER_H
#define ARCEMU_OPENWEATHERPROVIDER_H

struct WeatherData;

/// A weather provider that retrieves weather data from OpenWeather
class OpenWeatherProvider : public WeatherProvider, public EventableObject
{
public:
	OpenWeatherProvider();
	~OpenWeatherProvider();
	void onStartup();
	void sendWeather( Player *player );
	void setEnableGeneratedWeather( uint32 zone, bool enabled );
	void setWeather( uint32 zone, uint32 type, float density );
	void updateWeather();

	static WeatherProvider* create(){ return new OpenWeatherProvider(); }

private:
	std::string apiURL;
	WeatherData* weatherData;
};

#endif
