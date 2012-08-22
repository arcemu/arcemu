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
	struct logondb
	{
		std::string host;
		std::string username;
		std::string password;
		std::string database;
		unsigned int port;
	};

	struct host
	{
		std::string logon_address;
		unsigned int logon_port;
		std::string is_address;
		unsigned int is_port;
	};

	struct log
	{
		unsigned short level;
	};

	struct rates
	{
		unsigned int account_refresh;
	};

	struct security
	{
		std::string remote_password;
		std::string allowed_ip_ranges;
		std::string allowed_mod_ip_ranges;
	};
};

#endif

