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

#ifndef ARCEMU_HTTP_REQUEST_H
#define ARCEMU_HTTP_REQUEST_H

#include <string>

/// Represents a HTTP request
class HTTPRequest
{
public:

	/// Supported HTTP request methods
	enum HTTPRequestMethod{
		HTTP_GET
	};

	HTTPRequest( std::string url, const HTTPRequestMethod method )
	{
		this->url = url;
		this->method = method;
	}

	/// Returns the URL of this HTTP request
	std::string getUrl() const
	{
		return url;
	}


	/// Returns the method of this HTTP request
	HTTPRequestMethod getMethod() const
	{
		return method;
	}

private:
	/// The URL of this HTTP request
	std::string url;

	/// The method of this HTTP request
	HTTPRequestMethod method;
};

#endif
