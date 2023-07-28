#if defined inc_database
	#endinput
#endif
#define inc_database

new ORMDB:maindb;

public OnScriptInit()
{
	maindb = ormDB(db_open("main.db"), dbSQLite);
	
#if defined db_OnScriptInit
    db_OnScriptInit();
#endif
	return 1;
}
#if defined _ALS_OnScriptInit
	#undef OnScriptInit
#else
    #define _ALS_OnScriptInit
#endif
#if defined OnScriptInit
	#undef OnScriptInit
#endif
#define OnScriptInit db_OnScriptInit
#if defined db_OnScriptInit
forward db_OnScriptInit();
#endif
