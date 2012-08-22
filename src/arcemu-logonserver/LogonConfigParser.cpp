#include "LogonConfigParser.h"
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

LogonConfigParser::LogonConfigParser()
{
}

LogonConfigParser::~LogonConfigParser()
{
}

bool LogonConfigParser::parseFile( const std::string &name )
{
	xmlDocPtr document = xmlParseFile( name.c_str() );
	if( document == NULL )
		return false;

	xmlNodePtr root = xmlDocGetRootElement( document );
	if( root == NULL )
	{
		xmlFreeDoc( document );
		return false;
	}

	if( !isConfig( root ) )
	{
		xmlFreeDoc( document );
		return false;
	}

	bool ok = true;

	xmlNodePtr node = root->children;
	while( node != NULL )
	{
		if( xmlStrcmp( node->name, BAD_CAST "logondatabase" ) == 0 )
			ok = parseDBPart( node );
		else
		if( xmlStrcmp( node->name, BAD_CAST "host" ) == 0 )
			ok = parseHostPart( node );
		else
		if( xmlStrcmp( node->name, BAD_CAST "log" ) == 0 )
			ok = parseLogPart( node );
		else
		if( xmlStrcmp( node->name, BAD_CAST "rates" ) == 0 )
			ok = parseRatesPart( node );
		else
		if( xmlStrcmp( node->name, BAD_CAST "security" ) == 0 )
			ok = parseSecurityPart( node );

		if( !ok )
			break;

		node = node->next;
	}

	xmlFreeDoc( document );
	
	if( !ok )
		return false;
	else
		return true;
}

bool LogonConfigParser::isConfig( _xmlNode *node )
{
	if( xmlStrcmp( node->name, BAD_CAST "config" ) != 0 )
		return false;

	xmlChar *prop = NULL;
	
	prop = xmlGetProp( node, BAD_CAST "type" );
	if( prop == NULL )
		return false;
	if( xmlStrcmp( prop, BAD_CAST "logon" ) != 0 )
		return false;

	prop = xmlGetProp( node, BAD_CAST "version" );
	if( prop == NULL )
		return false;
	if( xmlStrcmp( prop, BAD_CAST "1" ) )
		return false;

	return true;
}

bool LogonConfigParser::parseDBPart( _xmlNode *node )
{
	xmlChar *prop = NULL;

	prop = xmlGetProp( node, BAD_CAST "hostname" );
	if( prop == NULL )
		return false;
	data.logondb.host = reinterpret_cast< char* >( prop );

	prop = xmlGetProp( node, BAD_CAST "username" );
	if( prop == NULL )
		return false;
	data.logondb.username = reinterpret_cast< char* >( prop );

	prop = xmlGetProp( node, BAD_CAST "password" );
	if( prop == NULL )
		return false;
	data.logondb.password = reinterpret_cast< char* >( prop );

	prop = xmlGetProp( node, BAD_CAST "database" );
	if( prop == NULL )
		return false;
	data.logondb.database = reinterpret_cast< char* >( prop );

	prop = xmlGetProp( node, BAD_CAST "port" );
	if( prop == NULL )
		data.logondb.port = 3306u;
	else
		data.logondb.port = static_cast< unsigned int >( atoi( reinterpret_cast< char* >( prop ) ) );
		
	return true;
}

bool LogonConfigParser::parseHostPart( _xmlNode *node )
{
	xmlChar *prop = NULL;

	prop = xmlGetProp( node, BAD_CAST "logon_address" );
	if( prop == NULL )
		return false;
	data.host.logon_address = reinterpret_cast< char* >( prop );

	prop = xmlGetProp( node, BAD_CAST "logon_port" );
	if( prop == NULL )
		return false;
	data.host.logon_port = static_cast< unsigned int >( atoi( reinterpret_cast< char* >( prop ) ) );

	prop = xmlGetProp( node, BAD_CAST "is_address" );
	if( prop == NULL )
		return false;
	data.host.is_address = reinterpret_cast< char* >( prop );

	prop = xmlGetProp( node, BAD_CAST "is_port" );
	if( prop == NULL )
		return false;
	data.host.is_port = static_cast< unsigned int >( atoi( reinterpret_cast< char* >( prop ) ) );

	return true;
}

bool LogonConfigParser::parseLogPart( _xmlNode *node )
{
	xmlChar *prop = xmlGetProp( node, BAD_CAST "level" );
	if( prop == NULL )
		data.log.level = 0;
	else
		data.log.level = static_cast< unsigned int >( atoi( reinterpret_cast< char* >( prop ) ) );

	return true;
}

bool LogonConfigParser::parseRatesPart( _xmlNode *node )
{
	xmlChar *prop = xmlGetProp( node, BAD_CAST "account_refresh" );
	if( prop == NULL )
		data.rates.account_refresh = 600u;
	else
		data.rates.account_refresh = static_cast< unsigned int >( atoi( reinterpret_cast< char* >( prop ) ) );

	return true;
}

bool LogonConfigParser::parseSecurityPart( _xmlNode *node )
{
	xmlChar *prop = NULL;

	prop = xmlGetProp( node, BAD_CAST "remote_password" );
	if( prop == NULL )
		return false;
	data.security.remote_password = reinterpret_cast< char* >( prop );

	prop = xmlGetProp( node, BAD_CAST "allowed_ip_ranges" );
	if( prop == NULL )
		return false;
	data.security.allowed_ip_ranges = reinterpret_cast< char* >( prop );

	prop = xmlGetProp( node, BAD_CAST "allowed_mod_ip_ranges" );
	if( prop == NULL )
		return false;
	data.security.allowed_mod_ip_ranges = reinterpret_cast< char* >( prop );

	return true;
}

