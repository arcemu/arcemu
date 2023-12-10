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

#ifndef ARCEMU_OWD_PARSER_H
#define ARCEMU_OWD_PARSER_H

/// Parses the JSON response from the OpenWeatherMap API
class OpenWeatherDataParser
{
private:
	static const std::string BEFORE;
	static const std::string AFTER;

public:

	/// Parses the JSON response and fills in the OpenWeatherData structure
	static bool parse( std::string &s, OpenWeatherData &data );
};

#endif
