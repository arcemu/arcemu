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

#ifndef ARCMEU_OWD_H
#define ARCEMU_OWD_H

/// Represents the weather data retrieved from OpenWeatherMap
struct OpenWeatherData
{
	struct CurrentWeather
	{
		/// Weather phenomena codes
		std::vector< uint32 > codes;
	};

	CurrentWeather currentWeather;

	/// Attempts to map OpenWeatherMap API weather data to WoW weather
	/// Cannot be mapped properly, so we approximate
	/// https://openweathermap.org/weather-conditions#Weather-Condition-Codes-2
	void buildWeatherData( WeatherData &data );
};

#endif
