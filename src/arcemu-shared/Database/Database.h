/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008 <http://www.ArcEmu.org/>
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

#ifndef _DATABASE_H
#define _DATABASE_H

#include <string>
#include "../Threading/Queue.h"
#include "../CallBack.h"

using namespace std;
class QueryResult;
class QueryThread;
class Database;

struct DatabaseConnection
{
	FastMutex Busy;
};

struct SERVER_DECL AsyncQueryResult
{
	QueryResult * result;
	char * query;
};

class SERVER_DECL AsyncQuery
{
	friend class Database;
	SQLCallbackBase * func;
	vector<AsyncQueryResult> queries;
	Database * db;
public:
	AsyncQuery(SQLCallbackBase * f) : func(f) {}
	~AsyncQuery();
	void AddQuery(const char * format, ...);
	void Perform();
	ARCEMU_INLINE void SetDB(Database * dbb) { db = dbb; }
};

class SERVER_DECL QueryBuffer
{
	vector<char*> queries;
public:
	friend class Database;
	void AddQuery( const char * format, ... );
	void AddQueryNA( const char * str );
	void AddQueryStr(const string& str);
};

class SERVER_DECL Database : public CThread
{
	friend class QueryThread;
	friend class AsyncQuery;
public:
	Database();
	virtual ~Database();

	/************************************************************************/
	/* Thread Stuff                                                         */
	/************************************************************************/
	bool run();

	/************************************************************************/
	/* Virtual Functions                                                    */
	/************************************************************************/
	virtual bool Initialize(const char* Hostname, unsigned int port,
		const char* Username, const char* Password, const char* DatabaseName,
		uint32 ConnectionCount, uint32 BufferSize) = 0;
	
	virtual void Shutdown() = 0;

	virtual QueryResult* Query(const char* QueryString, ...);
	virtual QueryResult* QueryNA(const char* QueryString);
	virtual QueryResult * FQuery(const char * QueryString, DatabaseConnection * con);
	virtual void FWaitExecute(const char * QueryString, DatabaseConnection * con);
	virtual bool WaitExecute(const char* QueryString, ...);//Wait For Request Completion
	virtual bool WaitExecuteNA(const char* QueryString);//Wait For Request Completion
	virtual bool Execute(const char* QueryString, ...);
	virtual bool ExecuteNA(const char* QueryString);

	bool ThreadRunning;

	ARCEMU_INLINE const string& GetHostName() { return mHostname; }
	ARCEMU_INLINE const string& GetDatabaseName() { return mDatabaseName; }
	ARCEMU_INLINE const uint32 GetQueueSize() { return queries_queue.get_size(); }

	virtual string EscapeString(string Escape) = 0;
	virtual void EscapeLongString(const char * str, uint32 len, stringstream& out) = 0;
	virtual string EscapeString(const char * esc, DatabaseConnection * con) = 0;
	
	void QueueAsyncQuery(AsyncQuery * query);
	void EndThreads();
	
	void thread_proc_query();
	void FreeQueryResult(QueryResult * p);

	DatabaseConnection * GetFreeConnection();

	void PerformQueryBuffer(QueryBuffer * b, DatabaseConnection * ccon);
	void AddQueryBuffer(QueryBuffer * b);

	static Database * CreateDatabaseInterface(uint32 uType);
	static void CleanupLibs();

	virtual bool SupportsReplaceInto() = 0;
	virtual bool SupportsTableLocking() = 0;

protected:

	// spawn threads and shizzle
	void _Initialize();

	virtual void _BeginTransaction(DatabaseConnection * conn) = 0;
	virtual void _EndTransaction(DatabaseConnection * conn) = 0;

	// actual query function
	virtual bool _SendQuery(DatabaseConnection *con, const char* Sql, bool Self) = 0;
	virtual QueryResult * _StoreQueryResult(DatabaseConnection * con) = 0;

	////////////////////////////////
	FQueue<QueryBuffer*> query_buffer;

	////////////////////////////////
	FQueue<char*> queries_queue;
	DatabaseConnection ** Connections;
	
	uint32 _counter;
	///////////////////////////////

	int32 mConnectionCount;

	// For reconnecting a broken connection
	string mHostname;
	string mUsername;
	string mPassword;
	string mDatabaseName;
	uint32 mPort;

	QueryThread * qt;
};

class SERVER_DECL QueryResult
{
public:
	QueryResult(uint32 fields, uint32 rows) : mFieldCount(fields), mRowCount(rows), mCurrentRow(NULL) {}
	virtual ~QueryResult() {}

	virtual bool NextRow() = 0;
	void Delete() { delete this; }

	ARCEMU_INLINE Field* Fetch() { return mCurrentRow; }
	ARCEMU_INLINE uint32 GetFieldCount() const { return mFieldCount; }
	ARCEMU_INLINE uint32 GetRowCount() const { return mRowCount; }

protected:
	uint32 mFieldCount;
	uint32 mRowCount;
        Field *mCurrentRow;
};

class SERVER_DECL QueryThread : public CThread
{
	friend class Database;
	Database * db;
public:
	QueryThread(Database * d) : CThread(), db(d) {}
	~QueryThread();
	bool run();
};

#endif
