#ifndef __POSTGRESDATABASE_H
#define __POSTGRESDATABASE_H

#include <postgre/libpq-fe.h>

struct PostgresDatabaseConnection : public DatabaseConnection
{
	PGconn * PgSql;
	PGresult * Result;
};

class SERVER_DECL PostgresDatabase : public Database
{
	friend class QueryThread;
	friend class AsyncQuery;
public:
	PostgresDatabase();
	~PostgresDatabase();

	bool Initialize(const char* Hostname, unsigned int port,
		const char* Username, const char* Password, const char* DatabaseName,
		uint32 ConnectionCount, uint32 BufferSize);

	void Shutdown();

	string EscapeString(string Escape);
	void EscapeLongString(const char * str, uint32 len, stringstream& out);
	string EscapeString(const char * esc, DatabaseConnection * con);
	
	bool SupportsReplaceInto() { return false; }
	bool SupportsTableLocking() { return false; }

protected:

	bool _HandleError(PostgresDatabaseConnection*, uint32 ErrorNumber);
	bool _SendQuery(DatabaseConnection *con, const char* Sql, bool Self = false);

	void _BeginTransaction(DatabaseConnection * conn);
	void _EndTransaction(DatabaseConnection * conn);
	bool _Reconnect(PostgresDatabaseConnection * conn);

	QueryResult * _StoreQueryResult(DatabaseConnection * con);

	string mConnectString;
};

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

#endif		// __POSTGRESDATABASE_H
