/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2012 <http://www.ArcEmu.org/>
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

#if defined(ENABLE_DATABASE_MYSQL)

#include "MySQLDatabase.h"

MySQLDatabase::~MySQLDatabase()
{
	for(int32 i = 0; i < mConnectionCount; ++i)
	{
		mysql_close(((MySQLDatabaseConnection*)Connections[i])->MySql);
		delete Connections[i];
	}
	delete [] Connections;

	delete [] mHostname;
	delete [] mSocket;
	delete [] mUsername;
	delete [] mPassword;
	delete [] mDatabaseName;
	delete [] mSSLkey;
	delete [] mSSLcert;
	delete [] mSSLca;
}

MySQLDatabase::MySQLDatabase() : Database(), mHostname(NULL), mPort(0), mSocket(NULL), mUsername(NULL), mPassword(NULL), mDatabaseName(NULL), mSSLkey(NULL), mSSLcert(NULL), mSSLca(NULL), mCompress(false)
{

}

void MySQLDatabase::_BeginTransaction(DatabaseConnection* conn)
{
	_SendQuery(conn, "START TRANSACTION", false);
}

void MySQLDatabase::_EndTransaction(DatabaseConnection* conn)
{
	_SendQuery(conn, "COMMIT", false);
}

// Creates the given number of new mysql connection and stores the settings for future reconnects
// DatabaseName, ConnectionCount, BufferSize must be proper
// the others are optional in that mysql will use the defaults given in my.cnf
bool MySQLDatabase::Initialize(const char* Hostname, unsigned int port, const char *Socket, const char* Username, const char* Password, const char* DatabaseName, const char *ssl_key, const char *ssl_cert, const char *ssl_ca, bool Compress, uint32 ConnectionCount, uint32 BufferSize)
{
	// store database settings
	mConnectionCount = ConnectionCount;
	size_t len;
	if(Hostname && (len = strlen(Hostname))) {
		mHostname = new char[len+1];
		strcpy(mHostname, Hostname);
	}
	mPort = port;
	if(Socket && (len = strlen(Socket))) {
		mSocket = new char[len+1];
		strcpy(mSocket, Socket);
	}
	if(Username && (len = strlen(Username))) {
		mUsername = new char[len+1];
		strcpy(mUsername, Username);
	}
	if(Password && (len = strlen(Password))) {
		mPassword = new char[len+1];
		strcpy(mPassword, Password);
	}
	len = strlen(DatabaseName);
	mDatabaseName = new char[len+1];
	strcpy(mDatabaseName, DatabaseName);
	if(ssl_key && (len = strlen(ssl_key))) {
		mSSLkey = new char[len+1];
		strcpy(mSSLkey, ssl_key);
	}
	if(ssl_cert && (len = strlen(ssl_cert))) {
		mSSLcert = new char[len+1];
		strcpy(mSSLcert, ssl_cert);
	}
	if(ssl_ca && (len = strlen(ssl_ca))) {
		mSSLca = new char[len+1];
		strcpy(mSSLca, ssl_ca);
	}
	mCompress = Compress;

	// connect
	MySQLDatabaseConnection** conns;
	conns = new MySQLDatabaseConnection*[ConnectionCount];
	Connections = ((DatabaseConnection**)conns);
	for(uint32 i = 0; i < ConnectionCount; i++)
	{
		MYSQL *temp = _OpenConnection();
		if(temp == NULL)
			return false;

		conns[i] = new MySQLDatabaseConnection;
		conns[i]->MySql = temp;
	}

	Database::_Initialize();

	return true;
}

MYSQL *MySQLDatabase::_OpenConnection()
{
	Log.Notice("MySQLDatabase", "Connecting to `%s`, database `%s`...", mHostname, mDatabaseName);

	MYSQL *conn;
	conn = mysql_init(NULL);
	if(conn == NULL) {
		Log.Error("MySQLDatabase", "Could not initialize the MySQL object.");
		return NULL;
	}

	if(mysql_options(conn, MYSQL_SET_CHARSET_NAME, "utf8"))
		Log.Error("MySQLDatabase", "Could not set utf8 character set. Some languages may not work properly.");

	my_bool my_true = true;
	if(mysql_options(conn, MYSQL_OPT_RECONNECT, &my_true))
		Log.Error("MySQLDatabase", "MYSQL_OPT_RECONNECT could not be set, connection drops may occur but will be counteracted anyway.");

	// set up SSL
	mysql_ssl_set(conn, mSSLkey, mSSLcert, mSSLca, NULL, NULL);

	// enable compression if requested
	if(mCompress) {
		if(mysql_options(conn, MYSQL_OPT_COMPRESS, NULL))
			Log.Error("MySQLDatabase", "MYSQL_OPT_COMPRESS could not be set, this connection will not be compressed.");
	}

	if(mysql_real_connect(conn, mHostname, mUsername, mPassword, mDatabaseName, mPort, mSocket, 0) == NULL)
	{
		Log.Error("MySQLDatabase", "Connection failed due to: `%s`", mysql_error(conn));
		mysql_close(conn);
		return NULL;
	}
	
	return conn;
}

string MySQLDatabase::EscapeString(string Escape)
{
	char a2[16384] = { 0 };

	DatabaseConnection* con = GetFreeConnection();
	string ret;
	if(mysql_real_escape_string(static_cast<MySQLDatabaseConnection*>(con)->MySql, a2, Escape.c_str(), (unsigned long)Escape.length()) == 0)
		ret = Escape.c_str();
	else
		ret = a2;

	con->Busy.Release();

	return string(ret);
}

void MySQLDatabase::EscapeLongString(const char* str, uint32 len, stringstream & out)
{
	char a2[65536 * 3] = { 0 };

	DatabaseConnection* con = GetFreeConnection();
	const char* ret;
	if(mysql_real_escape_string(static_cast<MySQLDatabaseConnection*>(con)->MySql, a2, str, (unsigned long)len) == 0)
		ret = str;
	else
		ret = a2;

	out.write(a2, (std::streamsize)strlen(a2));
	con->Busy.Release();
}

string MySQLDatabase::EscapeString(const char* esc, DatabaseConnection* con)
{
	char a2[16384] = { 0 };
	const char* ret;
	if(mysql_real_escape_string(static_cast<MySQLDatabaseConnection*>(con)->MySql, a2, (char*)esc, (unsigned long)strlen(esc)) == 0)
		ret = esc;
	else
		ret = a2;

	return string(ret);
}

void MySQLDatabase::Shutdown()
{
	mysql_library_end();
}

bool MySQLDatabase::_SendQuery(DatabaseConnection* con, const char* Sql, bool Self)
{
	//dunno what it does ...leaving untouched
	int result = mysql_query(static_cast<MySQLDatabaseConnection*>(con)->MySql, Sql);
	if(result > 0)
	{
		if(Self == false && _HandleError(static_cast<MySQLDatabaseConnection*>(con), mysql_errno(static_cast<MySQLDatabaseConnection*>(con)->MySql)))
		{
			// Re-send the query, the connection was successful.
			// The true on the end will prevent an endless loop here, as it will
			// stop after sending the query twice.
			result = _SendQuery(con, Sql, true);
		}
		else
			sLog.outError("Sql query failed due to [%s], Query: [%s]", mysql_error(static_cast<MySQLDatabaseConnection*>(con)->MySql), Sql);
	}

	return (result == 0 ? true : false);
}

bool MySQLDatabase::_HandleError(MySQLDatabaseConnection* con, uint32 ErrorNumber)
{
	// Handle errors that should cause a reconnect to the Database.
	switch(ErrorNumber)
	{
		case 2006:  // Mysql server has gone away
		case 2008:  // Client ran out of memory
		case 2013:  // Lost connection to sql server during query
		case 2055:  // Lost connection to sql server - system error
			{
				// Let's instruct a reconnect to the db when we encounter these errors.
				return _Reconnect(con);
			}
			break;
	}

	return false;
}

MySQLQueryResult::MySQLQueryResult(MYSQL_RES* res, uint32 FieldCount, uint32 RowCount) : QueryResult(FieldCount, RowCount), mResult(res)
{
	mCurrentRow = new Field[FieldCount];
}

MySQLQueryResult::~MySQLQueryResult()
{
	mysql_free_result(mResult);
	delete [] mCurrentRow;
}

bool MySQLQueryResult::NextRow()
{
	MYSQL_ROW row = mysql_fetch_row(mResult);
	if(row == NULL)
		return false;

	for(uint32 i = 0; i < mFieldCount; ++i)
		mCurrentRow[i].SetValue(row[i]);

	return true;
}

QueryResult* MySQLDatabase::_StoreQueryResult(DatabaseConnection* con)
{
	MySQLQueryResult* res;
	MySQLDatabaseConnection* db = static_cast<MySQLDatabaseConnection*>(con);
	MYSQL_RES* pRes = mysql_store_result(db->MySql);
	uint32 uRows = (uint32)mysql_affected_rows(db->MySql);
	uint32 uFields = (uint32)mysql_field_count(db->MySql);

	if(uRows == 0 || uFields == 0 || pRes == 0)
	{
		if(pRes != NULL)
			mysql_free_result(pRes);

		return NULL;
	}

	res = new MySQLQueryResult(pRes, uFields, uRows);
	res->NextRow();

	return res;
}

bool MySQLDatabase::_Reconnect(MySQLDatabaseConnection* conn)
{
	// if the mysql object still exist, destroy it
	if(conn->MySql != NULL) {
		mysql_close(conn->MySql);
		conn->MySql = NULL;
	}

	// attempt to have a new connection created
	conn->MySql = _OpenConnection();
	return conn->MySql != NULL;
}

#endif
