/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008 <http://www.ArcEmu.org/>
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

/// Weather defines
enum WeatherTypes
{
     WEATHER_TYPE_NORMAL            = 0, // NORMAL
     WEATHER_TYPE_FOG               = 1, // FOG --> current value irrelant
     WEATHER_TYPE_RAIN              = 2, // RAIN
     WEATHER_TYPE_HEAVY_RAIN        = 4, // HEAVY_RAIN
     WEATHER_TYPE_SNOW              = 8, // SNOW
     WEATHER_TYPE_SANDSTORM         = 16 // SANDSTORM
};

enum WeatherSounds
{
     WEATHER_NOSOUND                = 0,
     WEATHER_RAINLIGHT              = 8533,
     WEATHER_RAINMEDIUM             = 8534,
     WEATHER_RAINHEAVY              = 8535,
     WEATHER_SNOWLIGHT              = 8536,
     WEATHER_SNOWMEDIUM             = 8537,
     WEATHER_SNOWHEAVY              = 8538,
     WEATHER_SANDSTORMLIGHT         = 8556,
     WEATHER_SANDSTORMMEDIUM        = 8557,
     WEATHER_SANDSTORMHEAVY         = 8558
};

initialiseSingleton( WeatherMgr );

void BuildWeatherPacket(WorldPacket * data, uint32 Effect, float Density )
{
	data->Initialize(SMSG_WEATHER);
	if(Effect == 0 ) // set all parameter to 0 for sunny.
		*data << uint32(0) << float(0) << uint32(0) << uint8(0);		
	else if (Effect == 1) // No sound/density for fog
		*data << Effect << float(0) << uint32(0) << uint8(0);		
	else
		*data << Effect << Density << GetSound(Effect,Density) << uint8(0) ;
//	sLog.outDebug("Send Weather Update %d, Density %f, Sound %d, unint8(0)", Effect,Density,GetSound(Effect,Density));
}

uint32 GetSound(uint32 Effect, float Density)
{
    uint32 sound;
    if(Density<=0.30f)
		return WEATHER_NOSOUND;

	switch(Effect)
    {
        case 2:                                             //rain
        case 4:                                             
            if(Density  <0.40f)
                 sound = WEATHER_RAINLIGHT;
            else if(Density  <0.70f)
                sound = WEATHER_RAINMEDIUM;
            else
                sound = WEATHER_RAINHEAVY;
            break;
        case 8:                                             //snow
            if(Density  <0.40f)
                sound = WEATHER_SNOWLIGHT;
            else if(Density  <0.70f)
                sound = WEATHER_SNOWMEDIUM;
            else
                sound = WEATHER_SNOWHEAVY;
            break;
        case 16:                                             //storm
            if(Density  <0.40f)
                sound = WEATHER_SANDSTORMLIGHT;
            else if(Density  <0.70f)
                sound = WEATHER_SANDSTORMMEDIUM;
            else
                sound = WEATHER_SANDSTORMHEAVY;
            break;
		default:											//no sound
            sound = WEATHER_NOSOUND;
            break;
    }
    return sound;
}

WeatherMgr::WeatherMgr()
{
}

WeatherMgr::~WeatherMgr()
{
	std::map<uint32, WeatherInfo*>::iterator itr;
	for(itr = m_zoneWeathers.begin(); itr != m_zoneWeathers.end(); itr++)
	{
		delete itr->second;
	}

	m_zoneWeathers.clear();
}

void WeatherMgr::LoadFromDB()
{
	//sLog.outString("  Loading Weather..."); // weather type 0= sunny / 1= fog / 2 = light_rain / 4 = rain / 8 = snow / ?? = sandstorm
	QueryResult *result = WorldDatabase.Query( "SELECT zoneId,high_chance,high_type,med_chance,med_type,low_chance,low_type FROM weather" );

	if( !result )
		return;

	do
	{
		Field *fields = result->Fetch();
		WeatherInfo *wi = new WeatherInfo;
		wi->m_zoneId = fields[0].GetUInt32();
		wi->m_effectValues[0] = fields[1].GetUInt32();  // high_chance
		wi->m_effectValues[1] = fields[2].GetUInt32();  // high_type
		wi->m_effectValues[2] = fields[3].GetUInt32();  // med_chance
		wi->m_effectValues[3] = fields[4].GetUInt32();  // med_type
		wi->m_effectValues[4] = fields[5].GetUInt32();  // low_chance
		wi->m_effectValues[5] = fields[6].GetUInt32();  // low_type
		m_zoneWeathers[wi->m_zoneId] = wi;

		wi->_GenerateWeather();
	} while( result->NextRow() );
	Log.Notice("WeatherMgr", "Loaded weather information for %u zones.", result->GetRowCount());

	delete result;
}

void WeatherMgr::SendWeather(Player *plr)  //Update weather when player has changed zone (WorldSession::HandleZoneUpdateOpcode)
{
	std::map<uint32, WeatherInfo*>::iterator itr;
	itr = m_zoneWeathers.find(plr->GetZoneId());

	if (itr == m_zoneWeathers.end())
	{
		WorldPacket data(SMSG_WEATHER, 9);
		BuildWeatherPacket(&data, 0, 0);
		plr->GetSession()->SendPacket( &data );
		plr->m_lastSeenWeather = 0;

		return;
	}
	else
	{
		itr->second->SendUpdate(plr);
	}
}

WeatherInfo::WeatherInfo()
{
	m_currentDensity = 0;
	m_currentEffect = 0;
	m_currentTime = 0;
	m_maxDensity = 0;
	m_totalTime = 0;
	m_zoneId = 0;
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
	m_maxDensity = fd+1; //1 - 2
	m_totalTime = (RandomUInt(11) + 5)*1000*120;//update approx. every 1-2 minutes

	uint32 rv = RandomUInt(100);

	std::map<uint32, uint32>::iterator itr;

	if (rv <= m_effectValues[4]) // %chance on changing weather from sunny to m_effectValues[5]
	{
		m_currentEffect = m_effectValues[5]; 
	}
	else if (rv <= m_effectValues[2]) // %chance on changing weather from sunny to m_effectValues[3]
	{
		m_currentEffect = m_effectValues[3]; 
	}
	else if (rv <= m_effectValues[0]) // %chance on changing weather from sunny to m_effectValues[1]
	{
		m_currentEffect = m_effectValues[1]; 
	}

	SendUpdate();

	sEventMgr.AddEvent(this, &WeatherInfo::BuildUp, EVENT_WEATHER_UPDATE, (uint32)(m_totalTime/ceil(m_maxDensity/WEATHER_DENSITY_UPDATE)*2), 0,0);
	Log.Debug("WeatherMgr", "Forecast for zone:%d new type:%d new interval:%d ms",m_zoneId,m_currentEffect,(uint32)(m_totalTime/ceil(m_maxDensity/WEATHER_DENSITY_UPDATE)*2));
}

void WeatherInfo::BuildUp()
{
    // Increase until 0.5, start random counter when reached   
	if (m_currentDensity >= 0.50f) 
	{
		sEventMgr.RemoveEvents(this, EVENT_WEATHER_UPDATE);
		sEventMgr.AddEvent(this, &WeatherInfo::Update, EVENT_WEATHER_UPDATE, (uint32)(m_totalTime/ceil(m_maxDensity/WEATHER_DENSITY_UPDATE)*4), 0,0);
//		sLog.outDebug("Weather starting random for zone:%d type:%d new interval:%d ms",m_zoneId,m_currentEffect,(uint32)(m_totalTime/ceil(m_maxDensity/WEATHER_DENSITY_UPDATE)*4));
	}
	else
	{
		m_currentDensity += WEATHER_DENSITY_UPDATE;
//		sLog.outDebug("Weather increased for zone:%d type:%d density:%f",m_zoneId,m_currentEffect,m_currentDensity);
		SendUpdate();
	}
}
void WeatherInfo::Update()
{
    // There will be a 66% the weather density decreases. If Sunny, use as currentDensity as countdown
	if (m_currentEffect == 0 || RandomUInt(100) < 66) 
	{
		m_currentDensity -= WEATHER_DENSITY_UPDATE;
		if (m_currentDensity < 0.30f) //0.20 is considered fog, lower values are anoying
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
		if (m_currentDensity >= m_maxDensity)
		{
			m_currentDensity = m_maxDensity;
			return;
		}
	}
	SendUpdate();
//	sLog.outDebug("Weather Updated,zoneId:%d type:%d density:%f", m_zoneId, m_currentEffect, m_currentDensity);
}

void WeatherInfo::SendUpdate()
{
	WorldPacket data(SMSG_WEATHER, 9);
	BuildWeatherPacket(&data, m_currentEffect, m_currentDensity);
	sWorld.SendZoneMessage(&data, m_zoneId, 0);
}

void WeatherInfo::SendUpdate(Player *plr) //Updates weather for player's zone-change only if new zone weather differs
{
	if(plr->m_lastSeenWeather == m_currentEffect) //return if weather is same as previous zone
		return;

	plr->m_lastSeenWeather = m_currentEffect;

	WorldPacket data(SMSG_WEATHER, 9);
	BuildWeatherPacket(&data, m_currentEffect, m_currentDensity);
	plr->GetSession()->SendPacket( &data );
}
