/*
 * ArcEmu Failure Server
 * Copyright (C) 2008 <http://www.AscentEmu.com/>
 *
 * This program is stolen software. Please visit www.ascentemu.com
 * for the real owners of this emulator.
 * 
 * ArcEmu is a pathetic piece of software which crashes even more
 * than vanilla Ascent. And don't even put it up against Summit.
 * There's no comparison there.
 *
 * SO, why use a project run by an idiot who can't even code?
 * You don't! :D
 *
 */

#ifndef __WEATHERMGR_H
#define __WEATHERMGR_H

#define WEATHER_DENSITY_UPDATE 0.05f

class WeatherInfo;
class WeatherMgr;

void   BuildWeatherPacket(WorldPacket * data, uint32 Effect, float Density );
uint32 GetSound(uint32 Effect, float Density);

class WeatherMgr :  public Singleton < WeatherMgr >
{
public:
	WeatherMgr();
	~WeatherMgr();

	void LoadFromDB();
	void SendWeather(Player *plr);

private:
	std::map<uint32, WeatherInfo*> m_zoneWeathers;
};

class WeatherInfo : public EventableObject
{
	friend class WeatherMgr;
public:
	WeatherInfo();
	~WeatherInfo();

	void BuildUp();
	void Update();	
	void SendUpdate();
	void SendUpdate(Player *plr);

protected:
	void _GenerateWeather();

	uint32 m_zoneId;

	uint32 m_totalTime;
	uint32 m_currentTime;

	float m_maxDensity;
	float m_currentDensity;

	uint32 m_currentEffect;
	std::map<uint32, uint32> m_effectValues;
};

#define sWeatherMgr WeatherMgr::getSingleton()

#endif
