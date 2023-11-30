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
 
 #ifndef ARCEMU_WEATHER_PB
 #define ARCEMU_WEATHER_PB
 
 /// Builds a WorldPacket for SMSG_WEATHER
class SERVER_DECL WeatherPacketBuilder
{

public:
	uint32 type;
	float density;
	uint32 sound;
	uint8 unknown;

public:
	WeatherPacketBuilder()
	{
		type = 0;
		density = 0.0f;
		sound = 0;
		unknown = 0;
	}

	WeatherPacketBuilder( uint32 type, float density )
	{
		this->type = type;
		this->density = density;
		unknown = 0;
		
		setSound();
	}

	void buildPacket( WorldPacket &data );

private:

	/// Sets the sound id based on weather type and density
	void setSound();
};
 
 #endif
 