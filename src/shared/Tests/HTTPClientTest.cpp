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

#include "../../testing/testmacros.h"
#include "Network/HTTPClient.h"

class HTTPClientTest
{
public:
	bool testSimpleGET();

	bool run();
};


TESTCASE_REGISTRY_FOR( HTTPClientTest )
	TESTCASE( HTTPClientTest, testSimpleGET, "Test that a simple GET request can be performed" )
TESTCASE_REGISTRY_END()

bool HTTPClientTest::testSimpleGET()
{
	HTTPClient client;
	HTTPRequest request( "https://wwww.google.com/ncr", HTTPRequest::HTTP_GET );

	client.setFollowLocation( true );

	if( !client.performRequest( request ) )
	{
		std::cout << "Error: " << client.getLastErrorText() << std::endl;
		
		TEST_EQ( false, client.hasError() );
	}

	HTTPResponse response = client.getResponse();

	std::cout << "Error text: " << client.getLastErrorText() << std::endl;

	std::cout << "Response code: " << response.code << std::endl;

	TEST_EQ( 200L, response.code );

	std::cout << "Response body:" << std::endl;
	std::cout << response.body << std::endl;

	TESTCASE_END();
}

bool HTTPClientTest::run()
{
	RUN_TESTS();
}

int main( int arc, char **argv )
{
	RUN_TESTCLASS( HTTPClientTest );
}
