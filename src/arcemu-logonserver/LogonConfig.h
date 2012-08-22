#ifndef LOGON_CONFIG_H
#define LOGON_CONFIG_H

#include <string>

struct AllowedIP
{
	unsigned int IP;
	unsigned char Bytes;
};


struct LogonConfigData
{
	struct LogonDB
	{
		std::string host;
		std::string username;
		std::string password;
		std::string database;
		unsigned int port;
	}logondb;

	struct Host
	{
		std::string logon_address;
		unsigned int logon_port;
		std::string is_address;
		unsigned int is_port;
	}host;

	struct LogSettings
	{
		unsigned short level;
	}log;

	struct Rates
	{
		unsigned int account_refresh;
	}rates;

	struct Security
	{
		std::string remote_password;
		std::string allowed_ip_ranges;
		std::string allowed_mod_ip_ranges;
	}security;

};

#endif

