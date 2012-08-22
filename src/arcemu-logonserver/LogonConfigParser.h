#ifndef LOGON_CONFIG_PARSER_H
#define LOGON_CONFIG_PARSER_H

#include "LogonConfig.h"

struct _xmlNode;

class LogonConfigParser
{
public:
	LogonConfigParser();
	~LogonConfigParser();

	bool parseFile( const std::string &name );
	const LogonConfigData& getData() const{ return data; }

private:
	bool isConfig( _xmlNode *node );
	bool parseDBPart( _xmlNode *node );
	bool parseHostPart( _xmlNode *node );
	bool parseLogPart( _xmlNode *node );
	bool parseRatesPart( _xmlNode *node );
	bool parseSecurityPart( _xmlNode *node );

	LogonConfigData data;
};

#endif

