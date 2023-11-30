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

#ifndef ARCEMU_WEATHERINFO_H
#define ARCEMU_WEATHERINFO_H

struct WeatherTypeProbability
{
public:
	uint32 probability;
	uint32 type;

	WeatherTypeProbability()
	{
		probability = 0;
		type = 0;
	}
};

struct WeatherProbabilities
{
public:
	WeatherTypeProbability lowProbabilityWeather;
	WeatherTypeProbability medProbabilityWeather;
	WeatherTypeProbability highProbabilityWeather;
};

class WeatherInfo : public EventableObject
{
		friend class WeatherMgr;
	public:
		WeatherInfo();
		~WeatherInfo();

		void buildUp();
		void update();
		void sendUpdate();
		void sendUpdate(Player* plr);

	protected:
		void generateWeather();

		uint32 m_zoneId;

		uint32 m_totalTime;
		uint32 m_currentTime;

		float m_maxDensity;
		float m_currentDensity;

		uint32 m_currentEffect;

		WeatherProbabilities weatherProbabilities;
};

#endif
