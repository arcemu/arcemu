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

#ifndef __WEATHERMGR_H
#define __WEATHERMGR_H

class WeatherMgr :  public Singleton < WeatherMgr >
{
	public:
		WeatherMgr();
		~WeatherMgr();

		void LoadFromDB();
		void SendWeather(Player* plr);

		/// Enable or disable automatically generated weather for the zone
		void setEnableGeneratedWeather( uint32 zone, bool enabled );

		/// Set weather for the zone
		void setWeather( uint32 zone, uint32 type, float density );

	private:
		std::map<uint32, WeatherInfo*> m_zoneWeathers;
};

#define sWeatherMgr WeatherMgr::getSingleton()

#endif
