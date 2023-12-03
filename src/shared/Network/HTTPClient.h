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

#ifndef ARCEMU_HTTPCLIENT_H
#define ARCEMU_HTTPCLIENT_H

#include "curl/curl.h"

#include <string>

#include "Network/HTTPRequest.h"
#include "Network/HTTPResponse.h"

/// Represents a HTTP client that performs HTTP requests and returns the result
class HTTPClient
{
private:
	
	/// Internal CURL handle
	CURL *handle;

	/// The response of the last request performed
	HTTPResponse response;

	/// The error code of the last request performed
	CURLcode lastError;

	/// Fills the HTTP body request as data is available
	static size_t _writeCallback( void *data, size_t size, size_t nmemb, void *clientp );

public:

	HTTPClient();

	~HTTPClient();

	/// Tells if we had an error while performing
	bool hasError() const;

	/// Returns the error message of the last error
	std::string getLastErrorText() const;

	/// Tells the client whether to verify the host's certificate or not
	void setVerifyPeer( bool verify );

	/// Tells the client whether to follow HTTP redirects or not
	void setFollowLocation( bool follow );

	/// Performs the specified HTTP request. Returns false on failure.
	bool performRequest( HTTPRequest &request );

	/// Returns the response of the last performed HTTP request
	HTTPResponse getResponse() const
	{
		return response;
	}
};

#endif
