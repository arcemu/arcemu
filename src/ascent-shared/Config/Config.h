/*
 * ArcEmu MMORPG Server
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

#if !defined (CONFIG_H)
#define CONFIG_H
#include "Common.h"

using namespace std;
struct SERVER_DECL ConfigSetting
{
	string AsString;
	bool AsBool;
	int AsInt;
	float AsFloat;
};

typedef map<uint32, ConfigSetting> ConfigBlock;

class SERVER_DECL ConfigFile
{
public:
	ConfigFile();
	~ConfigFile();

	bool SetSource(const char *file, bool ignorecase = true);
	ConfigSetting * GetSetting(const char * Block, const char * Setting);

	bool GetString(const char * block, const char* name, std::string *value);
	std::string GetStringDefault(const char * block, const char* name, const char* def);
	std::string GetStringVA(const char * block, const char* def, const char * name, ...);
	bool GetString(const char * block, char * buffer, const char * name, const char * def, uint32 len);

	bool GetBool(const char * block, const char* name, bool *value);
	bool GetBoolDefault(const char * block, const char* name, const bool def);

	bool GetInt(const char * block, const char* name, int *value);
	int GetIntDefault(const char * block, const char* name, const int def);
	int GetIntVA(const char * block, int def, const char* name, ...);

	bool GetFloat(const char * block, const char* name, float *value);
	float GetFloatDefault(const char * block, const char* name, const float def);
	float GetFloatVA(const char * block, float def, const char* name, ...);

private:
	map<uint32, ConfigBlock> m_settings;
};


class SERVER_DECL ConfigMgr
{
	// Mainly used for WS, others will probably only have one.
public:
	ConfigFile MainConfig;
	ConfigFile RealmConfig;
	ConfigFile ClusterConfig;
	ConfigFile OptionalConfig;
};

extern SERVER_DECL ConfigMgr Config;

#endif
