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

#include "ConfigEnv.h"
ConfigMgr Config;

//#define _CONFIG_DEBUG

ConfigFile::ConfigFile()
{
}


ConfigFile::~ConfigFile()
{
	
}

void remove_spaces(string& str)
{
	while(str.size() && (*str.begin() == ' ' || *str.begin() == '\t'))
		str.erase(str.begin());
}

void remove_all_spaces(string& str)
{
	string::size_type off = str.find(" ");
	while(off != string::npos)
	{
		str.erase(off, 1);
		off = str.find(" ");
	}

	off = str.find("\t");
	while(off != string::npos)
	{
		str.erase(off, 1);
		off = str.find("\t");
	}
}

bool is_comment(string& str, bool * in_multiline_quote)
{
	string stemp = str;
	remove_spaces(stemp);
	if(stemp.length() == 0)
		return false;

	if(stemp[0] == '/')
	{
		if(stemp.length() < 2)
			return false;

		if(stemp[1] == '*')
		{
			*in_multiline_quote = true;
			return true;
		}
		else if(stemp[2] == '/')
		{
			return true;
		}
	}

	if(stemp[0] == '#')
		return true;

	return false;
}

void apply_setting(string & str, ConfigSetting & setting)
{
	setting.AsString = str;
	setting.AsInt = atoi(str.c_str());
	setting.AsBool = (setting.AsInt > 0);
	setting.AsFloat = (float)atof(str.c_str());

	/* check for verbal yes/no answers */
	if(str.length() > 1)
	{
		// this might be a yes/no?
		if(str.size() >= 3 && !strnicmp("yes", str.c_str(), 3))
		{
			setting.AsBool = true;
			setting.AsInt = 1;
		}
		else if(str.size() >= 2 && !strnicmp("no", str.c_str(), 2))
		{
			setting.AsBool = false;
			setting.AsInt = 0;
		}
	}
}

uint32 ahash(const char * str)
{
	register size_t len = strlen(str);
	register uint32 ret = 0;
	register size_t i = 0;
	for(; i < len; ++i)
		ret += 5 * ret + (tolower(str[i]));

	/*printf("%s : %u\n", str, ret);*/
	return ret;
}

uint32 ahash(string& str)
{
	return ahash(str.c_str());
}

bool ConfigFile::SetSource(const char *file, bool ignorecase)
{
	/* wipe any existing settings. */
	m_settings.clear();

	/* open the file */
	if(file != 0)
	{
		FILE * f = fopen(file, "r");
		char * buf;
		int length;
		if(!f)
		{
			sLog.outError("Could not open %s.", file);
			return false;
		}

		/* get the length of the file */
		fseek(f, 0, SEEK_END);
		length = ftell(f);
		buf = new char[length + 1];
		fseek(f, 0, SEEK_SET);

		fread(buf, length, 1, f);
		buf[length] = '\0';
		string buffer = string(buf);
		delete [] buf;
		
		/* close the file, it is no longer needed */
		fclose(f);

		/* let's parse it. */
		string line;
		string::size_type end;
		string::size_type offset;
		bool in_multiline_comment = false;
		bool in_multiline_quote = false;
		bool in_block = false;
		string current_setting = "";
		string current_variable = "";
		string current_block = "";
		ConfigBlock current_block_map;
		ConfigSetting current_setting_struct;

		/* oh god this is awful */
		try {

		for(;;)
		{
			/* grab a line. */
			end = buffer.find("\n");
			if(end == string::npos)
				break;

			line = buffer.substr(0, end);
			buffer.erase(0, end+1);
			goto parse;

parse:
			if(!line.size())
				continue;

			/* are we a comment? */
			if(!in_multiline_comment && is_comment(line, &in_multiline_comment))
			{
				/* our line is a comment. */
				if(!in_multiline_comment)
				{
					/* the entire line is a comment, skip it. */
					continue;
				}
			}

			/* handle our cases */
			if(in_multiline_comment)
			{
				// we need to find a "*/".
				offset = line.find("*/", 0);
				
				/* skip this entire line, eh? */
				if(offset == string::npos)
					continue;

				/* remove up to the end of the comment block. */
				line.erase(0, offset + 2);
				in_multiline_comment = false;
			}
		
			if(in_block)
			{
				/* handle settings across multiple lines */
				if(in_multiline_quote)
				{
					/* attempt to find the end of the quote block. */
					offset = line.find("\"");

					if(offset == string::npos)
					{
						/* append the whole line to the quote. */
						current_setting += line;
						current_setting += "\n";
						continue;
					}
	                
					/* only append part of the line to the setting. */
					current_setting.append(line.c_str(), offset+1);
					line.erase(0, offset + 1);
				
					/* append the setting to the config block. */
					if(current_block == "" || current_variable == "")
					{
						sLog.outError("Quote without variable.");
						return false;
					}

					/* apply the setting */
					apply_setting(current_setting, current_setting_struct);

					/* the setting is done, append it to the current block. */
                    current_block_map[ahash(current_variable)] = current_setting_struct;
#ifdef _CONFIG_DEBUG
					printf("Block: '%s', Setting: '%s', Value: '%s'\n", current_block.c_str(), current_variable.c_str(), current_setting_struct.AsString.c_str());
#endif
					/* no longer doing this setting, or in a quote. */
					current_setting = "";
					current_variable = "";
                    in_multiline_quote = false;					
				}

				/* remove any leading spaces */
				remove_spaces(line);

				if(!line.size())
					continue;

				/* our target is a *setting*. look for an '=' sign, this is our seperator. */
                offset = line.find("=");
				if(offset != string::npos)
				{
					ASSERT(current_variable == "");
					current_variable = line.substr(0, offset);

					/* remove any spaces from the end of the setting */
					remove_all_spaces(current_variable);

					/* remove the directive *and* the = from the line */
					line.erase(0, offset + 1);
				}

				/* look for the opening quote. this signifies the start of a setting. */
				offset = line.find("\"");
				if(offset != string::npos)
				{
					ASSERT(current_setting == "");
					ASSERT(current_variable != "");

					/* try and find the ending quote */
					end = line.find("\"", offset + 1);
					if(end != string::npos)
					{
						/* the closing quote is on the same line, oh goody. */
						current_setting = line.substr(offset+1, end-offset-1);

						/* erase up to the end */
						line.erase(0, end + 1);
						
						/* apply the setting */
						apply_setting(current_setting, current_setting_struct);

						/* the setting is done, append it to the current block. */
						current_block_map[ahash(current_variable)] = current_setting_struct;

#ifdef _CONFIG_DEBUG
						printf("Block: '%s', Setting: '%s', Value: '%s'\n", current_block.c_str(), current_variable.c_str(), current_setting_struct.AsString.c_str());
#endif
						/* no longer doing this setting, or in a quote. */
						current_setting = "";
						current_variable = "";
						in_multiline_quote = false;		

						/* attempt to grab more settings from the same line. */
						goto parse;
					}
					else
					{
						/* the closing quote is not on the same line. means we'll try and find it on
						   the next. */
						current_setting.append(line.c_str(), offset);

						/* skip to the next line. (after setting our condition first, of course :P */
						in_multiline_quote = true;
						continue;
					}
				}

				/* are we at the end of the block yet? */
				offset = line.find(">");
				if(offset != string::npos)
				{
					line.erase(0, offset+1);

					// freeeee!
					in_block = false;
					
					/* assign this block to the main "big" map. */
					m_settings[ahash(current_block)] = current_block_map;

					/* erase all data for this so it doesn't seep through */
					current_block_map.clear();
					current_setting = "";
					current_variable = "";
					current_block = "";
				}
			}
			else
			{
				/* we're not in a block. look for the start of one. */
				offset = line.find("<");

				if(offset != string::npos)
				{
					in_block = true;

					/* whee, a block! let's cut the string and re-parse. */
					line.erase(0, offset + 1);

					/* find the name of the block first, though. */
					offset = line.find(" ");
					if(offset != string::npos)
					{
						current_block = line.substr(0, offset);
						line.erase(0, offset + 1);
					}
					else
					{
						sLog.outError("Block without name.");
						return false;
					}

					/* skip back */
					goto parse;
				}
			}
		}

		}catch(...)
			{
				sLog.outError("Exception in config parsing.");
				return false;
			}

		/* handle any errors */
		if(in_block)
		{
			sLog.outError("Unterminated block.");
			return false;
		}

		if(in_multiline_comment)
		{
			sLog.outError("Unterminated comment.");
			return false;
		}

		if(in_multiline_quote)
		{
			sLog.outError("Unterminated quote.");
			return false;
		}

		/* we're all good :) */
		return true;
	}

	return false;
}

ConfigSetting * ConfigFile::GetSetting(const char * Block, const char * Setting)
{
	uint32 block_hash = ahash(Block);
	uint32 setting_hash = ahash(Setting);

	/* find it in the big map */
	map<uint32, ConfigBlock>::iterator itr = m_settings.find(block_hash);
	if(itr != m_settings.end())
	{
		ConfigBlock::iterator it2 = itr->second.find(setting_hash);
		if(it2 != itr->second.end())
			return &(it2->second);

		return 0;
	}

	return 0;
}

bool ConfigFile::GetString(const char * block, const char* name, std::string *value)
{
	ConfigSetting * Setting = GetSetting(block, name);
	if(Setting == 0)
		return false;

	*value = Setting->AsString;
	return true;
}


std::string ConfigFile::GetStringDefault(const char * block, const char* name, const char* def)
{
	string ret;
	return GetString(block, name, &ret) ? ret : def;
}


bool ConfigFile::GetBool(const char * block, const char* name, bool *value)
{
	ConfigSetting * Setting = GetSetting(block, name);
	if(Setting == 0)
		return false;

	*value = Setting->AsBool;
	return true;
}


bool ConfigFile::GetBoolDefault(const char * block, const char* name, const bool def /* = false */)
{
	bool val;
	return GetBool(block, name, &val) ? val : def;
}

bool ConfigFile::GetInt(const char * block, const char* name, int *value)
{
	ConfigSetting * Setting = GetSetting(block, name);
	if(Setting == 0)
		return false;

	*value = Setting->AsInt;
	return true;
}

bool ConfigFile::GetFloat(const char * block, const char* name, float *value)
{
	ConfigSetting * Setting = GetSetting(block, name);
	if(Setting == 0)
		return false;

	*value = Setting->AsFloat;
	return true;
}

int ConfigFile::GetIntDefault(const char * block, const char* name, const int def)
{
	int val;
	return GetInt(block, name, &val) ? val : def;
}

float ConfigFile::GetFloatDefault(const char * block, const char* name, const float def)
{
	float val;
	return (GetFloat(block, name, &val) ? val : def);
}

int ConfigFile::GetIntVA(const char * block, int def, const char* name, ...)
{
	va_list ap;
	va_start(ap, name);
	char str[150];
	vsnprintf(str, 150, name, ap);
	va_end(ap);
	int val;
	return GetInt(str, block, &val) ? val : def;
}

float ConfigFile::GetFloatVA(const char * block, float def, const char* name, ...)
{
	va_list ap;
	va_start(ap, name);
	char str[150];
	vsnprintf(str, 150, name, ap);
	va_end(ap);
	float val;
	return GetFloat(str, block, &val) ? val : def;
}

std::string ConfigFile::GetStringVA(const char * block, const char* def, const char * name, ...)
{
	va_list ap;
	va_start(ap, name);
	char str[150];
	vsnprintf(str, 150, name, ap);
	va_end(ap);

	return GetStringDefault(str, block, def);
}

bool ConfigFile::GetString(const char * block, char * buffer, const char * name, const char * def, uint32 len)
{
	string val = GetStringDefault(block, name, def);
	size_t blen = val.length();
	if(blen > len)
		blen = len;

	memcpy(buffer, val.c_str(), blen);
	buffer[blen] = 0;
    
	return true;
}

