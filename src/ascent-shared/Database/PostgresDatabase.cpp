
/*
* Ascent MMORPG Server
* Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
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

#include "DatabaseEnv.h"
#include "../CrashHandler.h"
#include "../NGLog.h"

#if defined(ENABLE_DATABASE_POSTGRES)

#include "PostgresDatabase.h"
#pragma comment(lib, "libpq.lib")

PostgresDatabase::~PostgresDatabase()
{
	delete [] Connections;
}

PostgresDatabase::PostgresDatabase() : Database()
{

}

void PostgresDatabase::_BeginTransaction(DatabaseConnection * conn)
{
	_SendQuery( conn, "BEGIN TRANSACTION", false );
}

void PostgresDatabase::_EndTransaction(DatabaseConnection * conn)
{
	_SendQuery( conn, "COMMIT TRANSACTION", false );
}

bool PostgresDatabase::Initialize(const char* Hostname, unsigned int port, const char* Username, const char* Password, const char* DatabaseName, uint32 ConnectionCount, uint32 BufferSize)
{
	uint32 i;
	PostgresDatabaseConnection * con;

	mHostname = string(Hostname);
	mConnectionCount = ConnectionCount;
	mUsername = string(Username);
	mPassword = string(Password);

	// Build the connection string
	stringstream ss;
	ss << "host = " << Hostname << " port = " << port << " ";
	ss << "user = '" << Username << "' ";
	if(strlen(Password) > 0)
		ss << "password = '" << Password << "' ";

	ss << "dbname = '" << DatabaseName << "'";
	mConnectString = ss.str();

	Log.Notice("PostgresDatabase", "Connecting to `%s`, database: `%s`...", Hostname, DatabaseName);
	Connections = new DatabaseConnection*[ConnectionCount];
	for( i = 0; i < ConnectionCount; ++i )
	{
		Connections[i] = new PostgresDatabaseConnection;
		con = static_cast<PostgresDatabaseConnection*>(Connections[i]);
		con->PgSql = PQconnectdb( mConnectString.c_str() );
		con->Result = NULL;
		if( con->PgSql == NULL )
		{
			Log.Error("PostgresDatabase", "Connection failed due to: `%s`", PQerrorMessage( con->PgSql ) );
			return false;
		}
	}

	Database::_Initialize();
	return true;
}

string PostgresDatabase::EscapeString(string Escape)
{
	char a2[16384] = {0};

	DatabaseConnection * con = GetFreeConnection();
	const char * ret;
	int err;
	if(PQescapeStringConn(static_cast<PostgresDatabaseConnection*>(con)->PgSql, a2, Escape.c_str(), (unsigned long)Escape.length(), &err) == 0)
		ret = Escape.c_str();
	else
		ret = a2;

	con->Busy.Release();
	return string(ret);
}

void PostgresDatabase::EscapeLongString(const char * str, uint32 len, stringstream& out)
{
	char a2[65536*3] = {0};

	DatabaseConnection * con = GetFreeConnection();
	const char * ret;
	int err;
	if(PQescapeStringConn(static_cast<PostgresDatabaseConnection*>(con)->PgSql, a2, str, (unsigned long)len, &err) == 0)
		ret = str;
	else
		ret = a2;

	out.write(a2, (std::streamsize)strlen(a2));
	con->Busy.Release();
}

string PostgresDatabase::EscapeString(const char * esc, DatabaseConnection * con)
{
	char a2[16384] = {0};
	const char * ret;
	int err;
	if(PQescapeStringConn(static_cast<PostgresDatabaseConnection*>(con)->PgSql, a2, (char*)esc, (unsigned long)strlen(esc), &err) == 0)
		ret = esc;
	else
		ret = a2;

	return string(ret);
}

void PostgresDatabase::Shutdown()
{
	// TODO
}

bool PostgresDatabase::_SendQuery(DatabaseConnection *con, const char* Sql, bool Self)
{
	PGresult * res = PQexec( static_cast<PostgresDatabaseConnection*>(con)->PgSql, Sql );
	if( res == NULL )
	{
		printf("Error: %s\n", PQerrorMessage( static_cast<PostgresDatabaseConnection*>(con)->PgSql ) );
		return false;
	}

	ExecStatusType result = PQresultStatus(res);
	if( result != PGRES_TUPLES_OK && result != PGRES_COMMAND_OK )
	{
		Log.Error("PostgreDatabase", "Query failed: %s", PQresultErrorMessage(res));
		PQclear( res );
		static_cast<PostgresDatabaseConnection*>(con)->Result = NULL;
		return false;
	}

	static_cast<PostgresDatabaseConnection*>(con)->Result = res;
	return true;
}

bool PostgresDatabase::_HandleError(PostgresDatabaseConnection * con, uint32 ErrorNumber)
{
	// Handle errors that should cause a reconnect to the Database.
	/*switch(ErrorNumber)
	{
	case 2006:  // Mysql server has gone away
	case 2008:  // Client ran out of memory
	case 2013:  // Lost connection to sql server during query
	case 2055:  // Lost connection to sql server - system error
		{
			// Let's instruct a reconnect to the db when we encounter these errors.
			return _Reconnect( con );
		}break;
	}*/

	return false;
}

PostgresQueryResult::PostgresQueryResult(PGresult* res, uint32 FieldCount, uint32 RowCount) : QueryResult(FieldCount, RowCount), mResult( res ), mRow(0)
{
	mCurrentRow = new Field[FieldCount];
}

PostgresQueryResult::~PostgresQueryResult()
{
	PQclear(mResult);
	delete [] mCurrentRow;
}

bool PostgresQueryResult::NextRow()
{
	char * pValue;
	uint32 i;

	if( mRow == mRowCount )
		return false;
	
	for( i = 0; i < mFieldCount; ++i )
	{
		pValue = PQgetvalue( mResult, mRow, i );
		if( pValue == NULL )
		{
			printf("NULL field in PostgresResult!!!\n");
			return false;
		}

		mCurrentRow[i].SetValue( pValue );
	}

	++mRow;
	return true;
}

QueryResult * PostgresDatabase::_StoreQueryResult(DatabaseConnection * con)
{
	PostgresQueryResult * res;
	PostgresDatabaseConnection * db = static_cast<PostgresDatabaseConnection*>(con);

	if( db->Result == NULL )
		return NULL;

	uint32 uRows = (uint32)PQntuples( db->Result );
	uint32 uFields = (uint32)PQnfields( db->Result );

	if( uRows == 0 || uFields == 0 )
	{
		PQclear( db->Result );
		db->Result = NULL;

		return NULL;
	}

	res = new PostgresQueryResult( db->Result, uFields, uRows );
	res->NextRow();

	db->Result = NULL;
	return res;
}

bool PostgresDatabase::_Reconnect(PostgresDatabaseConnection * conn)
{
	return false;
}

#endif
