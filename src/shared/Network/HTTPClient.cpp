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

///#include <iostream>

#include "Network/HTTPRequest.h"
#include "Network/HTTPResponse.h"
#include "Network/HTTPClient.h"

/// Initializes and deinitializes CURL global state
class CURLGlobal
{
public:
	CURLGlobal()
	{
		///std::cout << "CURL global init" << std::endl;
		curl_global_init(CURL_GLOBAL_DEFAULT);
	}

	~CURLGlobal()
	{
		///std::cout << "CURL global cleanup" << std::endl;
		curl_global_cleanup();
	}
};

/// Make sure that we run the initialization and deinitialization. Yes this is very ugly, but sadly neccessary
static CURLGlobal curlGlobal;

size_t HTTPClient::_writeCallback( void *data, size_t size, size_t nmemb, void *clientp )
{
	size_t len = nmemb * size;
	
	HTTPClient *client = reinterpret_cast< HTTPClient* >( clientp );
	client->response.body.append( reinterpret_cast< char* >( data ), len );
	
	return len;
}

HTTPClient::HTTPClient()
{
	lastError = CURLE_OK;
	handle = curl_easy_init();
	
	/// Disable peer verification by default because we won't have certs with Arcemu by default
	curl_easy_setopt( handle, CURLOPT_SSL_VERIFYPEER, 0L );
}

HTTPClient::~HTTPClient()
{
	curl_easy_cleanup( handle );
}


bool HTTPClient::hasError() const
{
	if( lastError != CURLE_OK )
	{
		return true;
	}
	else
	{
		return false;
	}
}

std::string HTTPClient::getLastErrorText() const
{
	return curl_easy_strerror( lastError );
}

void HTTPClient::setVerifyPeer( bool verify )
{
	if( verify )
	{
		curl_easy_setopt( handle, CURLOPT_SSL_VERIFYPEER, 1L );
	}
	else
	{
		curl_easy_setopt( handle, CURLOPT_SSL_VERIFYPEER, 0L );
	}
}

void HTTPClient::setFollowLocation( bool follow )
{
	if( follow )
	{
		curl_easy_setopt( handle, CURLOPT_FOLLOWLOCATION, 1L );
	}
	else
	{
		curl_easy_setopt( handle, CURLOPT_FOLLOWLOCATION, 0L );
	}
}

bool HTTPClient::performRequest( HTTPRequest &request )
{
	curl_easy_setopt( handle, CURLOPT_URL, request.getUrl().c_str() );
	
	/// Write data using our private write callback method
	curl_easy_setopt( handle, CURLOPT_WRITEFUNCTION, _writeCallback );
	
	/// We pass the this pointer to the write function, so that we can reach this class instance
	curl_easy_setopt( handle, CURLOPT_WRITEDATA, this );
	
	lastError = curl_easy_perform( handle );
	if( lastError != CURLE_OK )
	{
		return false;
	}
	else
	{
		long responseCode;
		curl_easy_getinfo( handle, CURLINFO_RESPONSE_CODE, &responseCode );
		
		response.code = responseCode;
	}
	
	return true;
}
