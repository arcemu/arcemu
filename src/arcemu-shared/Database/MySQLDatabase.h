/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2012 <http://www.ArcEmu.org/>
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

#ifndef __MYSQLDATABASE_H
#define __MYSQLDATABASE_H

#if PLATFORM == PLATFORM_APPLE
#include <mysql.h>
#else
#include <mysql/mysql.h>
#endif

struct MySQLDatabaseConnection : public DatabaseConnection
{
	MYSQL* MySql;
};

class SERVER_DECL MySQLDatabase : public Database
{
		friend class QueryThread;
		friend class AsyncQuery;
	public:
		MySQLDatabase();
		~MySQLDatabase();

		bool Initialize(const char* Hostname, unsigned int port, const char *Socket,
		                        const char* Username, const char* Password, const char* DatabaseName,
		                        const char *ssl_key, const char *ssl_cert, const char *ssl_ca,
		                        bool Compress, uint32 ConnectionCount, uint32 BufferSize);

		void Shutdown();

		string EscapeString(string Escape);
		void EscapeLongString(const char* str, uint32 len, stringstream & out);
		string EscapeString(const char* esc, DatabaseConnection* con);

		bool SupportsReplaceInto() { return true; }
		bool SupportsTableLocking() { return true; }

	protected:
		MYSQL *_OpenConnection();

		bool _HandleError(MySQLDatabaseConnection*, uint32 ErrorNumber);
		bool _SendQuery(DatabaseConnection* con, const char* Sql, bool Self = false);

		void _BeginTransaction(DatabaseConnection* conn);
		void _EndTransaction(DatabaseConnection* conn);
		bool _Reconnect(MySQLDatabaseConnection* conn);

		QueryResult* _StoreQueryResult(DatabaseConnection* con);

	private:
		// Database settings used for reconnecting
		char *mHostname;
		uint32 mPort;
		char *mSocket;
		char *mUsername;
		char *mPassword;
		char *mDatabaseName;
		char *mSSLkey;
		char *mSSLcert;
		char *mSSLca;
		bool mCompress;
};

class SERVER_DECL MySQLQueryResult : public QueryResult
{
	public:
		MySQLQueryResult(MYSQL_RES* res, uint32 FieldCount, uint32 RowCount);
		~MySQLQueryResult();

		bool NextRow();

	protected:
		MYSQL_RES* mResult;
};

#endif		// __MYSQLDATABASE_H
