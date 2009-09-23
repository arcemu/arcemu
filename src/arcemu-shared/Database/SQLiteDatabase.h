/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2009 <http://www.ArcEmu.org/>
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

#ifndef __SQLITEDATABASE_H
#define __SQLITEDATABASE_H

#include <sqlite/sqlite3.h>

struct SQLiteDatabaseConnection : public DatabaseConnection
{
	sqlite3 * handle;

	char ** tabledata;
	int rows;
	int cols;
};

class SERVER_DECL SQLiteDatabase : public Database
{
	friend class QueryThread;
	friend class AsyncQuery;
public:
	SQLiteDatabase();
	~SQLiteDatabase();

	bool Initialize(const char* Hostname, unsigned int port,
		const char* Username, const char* Password, const char* DatabaseName,
		uint32 ConnectionCount, uint32 BufferSize);

	void Shutdown();

	string EscapeString(string Escape);
	void EscapeLongString(const char * str, uint32 len, stringstream& out);
	string EscapeString(const char * esc, DatabaseConnection * con);
	
protected:

	bool _SendQuery(DatabaseConnection *con, const char* Sql, bool Self = false);

	void _BeginTransaction(DatabaseConnection * conn);
	void _EndTransaction(DatabaseConnection * conn);

	QueryResult * _StoreQueryResult(DatabaseConnection * con);

	string mConnectString;
};
/*
class SERVER_DECL PostgresQueryResult : public QueryResult
{
public:
	PostgresQueryResult(PGresult * res, uint32 FieldCount, uint32 RowCount);
	~PostgresQueryResult();

	bool NextRow();

protected:
	PGresult * mResult;
	uint32 mRow;
};
*/
#endif		// __POSTGRESDATABASE_H
