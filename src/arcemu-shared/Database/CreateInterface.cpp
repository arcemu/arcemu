#include "DatabaseEnv.h"
#include "../CrashHandler.h"
#include "../NGLog.h"

#if defined(ENABLE_DATABASE_MYSQL)
#include "MySQLDatabase.h"
#endif

#if defined(ENABLE_DATABASE_POSTGRES)
#include "PostgresDatabase.h"
#endif

#if defined(ENABLE_DATABASE_SQLITE)
#include "SQLiteDatabase.h"
#endif

void Database::CleanupLibs()
{
#if defined(ENABLE_DATABASE_MYSQL)
		mysql_library_end();
#endif
}


Database * Database::CreateDatabaseInterface(uint32 uType)
{
	switch(uType)
	{
#if defined(ENABLE_DATABASE_MYSQL)
	case 1:		// MYSQL
		return new MySQLDatabase();
		break;
#endif
	
#if defined(ENABLE_DATABASE_POSTGRES)
	case 2:		// POSTGRES
		return new PostgresDatabase();
		break;
#endif

#if defined(ENABLE_DATABASE_SQLITE)
	case 3:		// SQLITE
		return new SQLiteDatabase();
		break;
#endif
	}

	Log.LargeErrorMessage(LARGERRORMESSAGE_ERROR, "You have attempted to connect to a database that is unsupported or nonexistant.\nCheck your config and try again.");
	abort();
	return NULL;
}
