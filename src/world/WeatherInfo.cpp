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

#define WEATHER_DENSITY_UPDATE 0.05f

WeatherInfo::WeatherInfo()
{
	m_currentDensity = 0;
	m_currentEffect = 0;
	m_currentTime = 0;
	m_maxDensity = 0;
	m_totalTime = 0;
	m_zoneId = 0;

	// Weather updates run in WorldRunnable thread
	m_holder = sEventMgr.GetEventHolder(WORLD_INSTANCE);
}

WeatherInfo::~WeatherInfo()
{

}

void WeatherInfo::_GenerateWeather()
{
	m_currentTime = 0;
	m_currentEffect = 0;
	m_currentDensity = 0.30f;//Starting Offset (don't go below, it's annoying fog)
	float fd = RandomFloat();
	m_maxDensity = fd + 1; //1 - 2
	m_totalTime = (RandomUInt(11) + 5) * 1000 * 120; //update approx. every 1-2 minutes

	uint32 rv = RandomUInt(100);

	std::map<uint32, uint32>::iterator itr;

	if(rv <= m_effectValues[4])  // %chance on changing weather from sunny to m_effectValues[5]
	{
		m_currentEffect = m_effectValues[5];
	}
	else if(rv <= m_effectValues[2])  // %chance on changing weather from sunny to m_effectValues[3]
	{
		m_currentEffect = m_effectValues[3];
	}
	else if(rv <= m_effectValues[0])  // %chance on changing weather from sunny to m_effectValues[1]
	{
		m_currentEffect = m_effectValues[1];
	}

	SendUpdate();

	sEventMgr.AddEvent(this, &WeatherInfo::BuildUp, EVENT_WEATHER_UPDATE, (uint32)(m_totalTime / ceil(m_maxDensity / WEATHER_DENSITY_UPDATE) * 2), 0, 0);
	Log.Debug("WeatherMgr", "Forecast for zone:%d new type:%d new interval:%d ms", m_zoneId, m_currentEffect, (uint32)(m_totalTime / ceil(m_maxDensity / WEATHER_DENSITY_UPDATE) * 2));
}

void WeatherInfo::BuildUp()
{
	// Increase until 0.5, start random counter when reached
	if(m_currentDensity >= 0.50f)
	{
		sEventMgr.RemoveEvents(this, EVENT_WEATHER_UPDATE);
		sEventMgr.AddEvent(this, &WeatherInfo::Update, EVENT_WEATHER_UPDATE, (uint32)(m_totalTime / ceil(m_maxDensity / WEATHER_DENSITY_UPDATE) * 4), 0, 0);
//		LOG_DEBUG("Weather starting random for zone:%d type:%d new interval:%d ms",m_zoneId,m_currentEffect,(uint32)(m_totalTime/ceil(m_maxDensity/WEATHER_DENSITY_UPDATE)*4));
	}
	else
	{
		m_currentDensity += WEATHER_DENSITY_UPDATE;
//		LOG_DEBUG("Weather increased for zone:%d type:%d density:%f",m_zoneId,m_currentEffect,m_currentDensity);
		SendUpdate();
	}
}
void WeatherInfo::Update()
{
	// There will be a 66% the weather density decreases. If Sunny, use as currentDensity as countdown
	if(m_currentEffect == 0 || RandomUInt(100) < 66)
	{
		m_currentDensity -= WEATHER_DENSITY_UPDATE;
		if(m_currentDensity < 0.30f)  //0.20 is considered fog, lower values are annoying
		{
			m_currentDensity = 0.0f;
			m_currentEffect = 0;
			sEventMgr.RemoveEvents(this, EVENT_WEATHER_UPDATE);
			_GenerateWeather();
			return;
		}
	}
	else
	{
		m_currentDensity += WEATHER_DENSITY_UPDATE;
		if(m_currentDensity >= m_maxDensity)
		{
			m_currentDensity = m_maxDensity;
			return;
		}
	}
	SendUpdate();
//	LOG_DEBUG("Weather Updated,zoneId:%d type:%d density:%f", m_zoneId, m_currentEffect, m_currentDensity);
}

void WeatherInfo::SendUpdate()
{
	WorldPacket data(SMSG_WEATHER, 9);
	WeatherPacketBuilder builder( m_currentEffect, m_currentDensity );
	builder.buildPacket( data );
	sWorld.SendZoneMessage(&data, m_zoneId, 0);
}

void WeatherInfo::SendUpdate(Player* plr) //Updates weather for player's zone-change only if new zone weather differs
{
	if(plr->m_lastSeenWeather == m_currentEffect) //return if weather is same as previous zone
		return;

	plr->m_lastSeenWeather = m_currentEffect;

	WorldPacket data(SMSG_WEATHER, 9);
	WeatherPacketBuilder builder( m_currentEffect, m_currentDensity );
	builder.buildPacket( data );
	plr->GetSession()->SendPacket(&data);
}
