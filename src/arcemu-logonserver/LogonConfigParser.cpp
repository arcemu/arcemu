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

	xmlFreeDoc( document );

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
	return true;
}

bool LogonConfigParser::parseHostPart( _xmlNode *node )
{
	return true;
}

bool LogonConfigParser::parseLogPart( _xmlNode *node )
{
	return true;
}

bool LogonConfigParser::parseRatesPart( _xmlNode *node )
{
	return true;
}

bool LogonConfigParser::parseSecurityPart( _xmlNode *node )
{
	return true;
}

