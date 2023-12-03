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

#ifndef ARCEMU_HTTPRESPONSE_H
#define ARCEMU_HTTPRESPONSE_H

#include <string>

/// Represents a response to a HTTP request
class HTTPResponse
{
public:

	/// The response code of the HTTP response
	long code;

	/// The body of this HTTP response
	std::string body;

	HTTPResponse()
	{
		code = 0;
	}
};

#endif
