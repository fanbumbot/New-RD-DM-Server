#if defined mdg_orm_inc
	#endinput
#endif
#define mdg_orm_inc

#include <memory>
#include <a_mysql>

#include <MS/extargs>
#include <MS/list>
#include <MS/map>

/*
	ORM

	Table:
	    struct:
			array of column (by id):
			    node of namecolumn:
				    name of column
		            node of column:
		                id of column (equal id in array)
		                type
		                len (if type - string or array)
						default value
	        map of columns:
	            node of column (equal in array of column)
	        name of table
	        orm database
	        list of connected caches

	 Cache:
	    struct:
	        table
	        id
	        array of data

*/

#define MAX_ORM_KEY_SIZE 1024
#define MAX_ORM_BUFFER_SIZE 4096

#define ALLDB {_, DB, MySQL}
#define ALLDB_RESULT {_, DBResult, Cache}

enum DBType
{
	dbSQLite,
	dbMySQL
}

enum ORMColumnType
{
	typeInteger,
	typeFloat,
	typeString,
	typeArray
}

enum
{
	isConst,
	isVariable,
	isMemory
}

static orm_key[MAX_ORM_KEY_SIZE];
static orm_buffer[MAX_ORM_BUFFER_SIZE];

forward orm_SysreqFix();
public orm_SysreqFix()
{
	new temp[2];
	MEM_delete(MEM_new_arr(temp));
	return 1;
}

stock ORMDB:ormDB(ALLDB:database, DBType:type)
{
	new ORMDB:db = ORMDB:MEM_new(2);
	MEM_set_val(Pointer:db, 0, _:database);
	MEM_set_val(Pointer:db, 1, _:type);
	return db;
}

stock ormDBDelete(ORMDB:db)
{
	return MEM_delete(Pointer:db);
}

stock ORMResult:ormResult(ALLDB_RESULT:result, DBType:type)
{
	new ORMResult:res = ORMResult:MEM_new(2);
	MEM_set_val(Pointer:res, 0, _:result);
	MEM_set_val(Pointer:res, 1, _:type);
	return res;
}

stock ormResultDelete(ORMResult:result)
{
	return MEM_delete(Pointer:result);
}

stock ormResultClear(ORMResult:result)
{
    new res = MEM_get_val(Pointer:result, 0);
    new DBType:type = DBType:MEM_get_val(Pointer:result, 1);
    switch(type)
    {
        case dbSQLite:	return db_free_result(DBResult:res);
        case dbMySQL:		return cache_delete(Cache:res);
    }
    return 0;
}

stock ORMResult:ormQuery(ORMDB:database, bool:autodelete, text[])
{
    new db = MEM_get_val(Pointer:database, 0);
	new DBType:type = DBType:MEM_get_val(Pointer:database, 1);

    new result;
	switch(type)
	{
	    case dbSQLite:
	    {
	        result = _:db_query(DB:db, text);
	    }
	    case dbMySQL:
	    {
	        result = _:mysql_query(MySQL:db, text, !autodelete);
	    }
	}
	new ORMResult:res = ormResult(result, type);
	if(autodelete)
	{
	    ormResultClear(res);
	    ormResultDelete(res);
	    res = ORMResult:0;
	}
	return res;
}

stock ormResultNumRows(ORMResult:result)
{
    new res = MEM_get_val(Pointer:result, 0);
    new DBType:type = DBType:MEM_get_val(Pointer:result, 1);

	new ret;
	switch(type)
	{
	    case dbSQLite:
		{
			ret = db_num_rows(DBResult:res);
		}
	    case dbMySQL:
		{
		    cache_set_active(Cache:res);
			ret = cache_num_rows();
		}
	}
	return ret;
}

stock ormResultNumFields(ORMResult:result)
{
    new res = MEM_get_val(Pointer:result, 0);
    new DBType:type = DBType:MEM_get_val(Pointer:result, 1);

	new ret;
	switch(type)
	{
	    case dbSQLite:
		{
			ret = db_num_fields(DBResult:res);
		}
	    case dbMySQL:
		{
		    cache_set_active(Cache:res);
			ret = cache_num_fields();
		}
	}
	return ret;
}

stock ormResultGetFieldName(ORMResult:result, field, destination[], maxlength = sizeof(destination))
{
    new res = MEM_get_val(Pointer:result, 0);
    new DBType:type = DBType:MEM_get_val(Pointer:result, 1);

	new ret;
	switch(type)
	{
	    case dbSQLite:
		{
			ret = db_field_name(DBResult:res, field, destination, maxlength);
		}
	    case dbMySQL:
		{
		    cache_set_active(Cache:res);
			ret = cache_get_field_name(field, destination, maxlength);
		}
	}
	return ret;
}

stock ormResultNext(ORMResult:result)
{
    new res = MEM_get_val(Pointer:result, 0);
    new DBType:type = DBType:MEM_get_val(Pointer:result, 1);
    
	switch(type)
	{
	    case dbSQLite:		return db_next_row(DBResult:res);
	}
	return 0;
}

stock ormGetFieldAssoc(ORMResult:result, field[], dest[], dest_s = sizeof(dest), row = 0)
{
    new value = MEM_get_val(Pointer:result, 0);
    new DBType:type = DBType:MEM_get_val(Pointer:result, 1);

	new ret;
	switch(type)
	{
	    case dbSQLite:
		{
			ret = db_get_field_assoc(DBResult:value, field, dest, dest_s);
		}
	    case dbMySQL:
	    {
		    cache_set_active(Cache:value);
			ret = cache_get_value_name(row, field, dest, dest_s);
	    }
	}
	return ret;
}

stock ormGetFieldAssocInt(ORMResult:result, field[], row = 0)
{
    new value = MEM_get_val(Pointer:result, 0);
    new DBType:type = DBType:MEM_get_val(Pointer:result, 1);

	new ret;
	switch(type)
	{
	    case dbSQLite:		return db_get_field_assoc_int(DBResult:value, field);
	    case dbMySQL:
	    {
		    cache_set_active(Cache:value);
			cache_get_value_name_int(row, field, ret);
			return ret;
	    }
	}
	return 0;
}

stock Float:ormGetFieldAssocFloat(ORMResult:result, field[], row = 0)
{
    new value = MEM_get_val(Pointer:result, 0);
    new DBType:type = DBType:MEM_get_val(Pointer:result, 1);

	new Float:ret;
	switch(type)
	{
	    case dbSQLite:		return db_get_field_assoc_float(DBResult:value, field);
	    case dbMySQL:
	    {
		    cache_set_active(Cache:value);
			cache_get_value_name_float(row, field, ret);
			return ret;
	    }
	}
	return 0;
}

stock ormGetField(ORMResult:result, id, dest[], dest_s = sizeof(dest), row = 0)
{
    new value = MEM_get_val(Pointer:result, 0);
    new DBType:type = DBType:MEM_get_val(Pointer:result, 1);

	new ret;
	switch(type)
	{
	    case dbSQLite:		return db_get_field(DBResult:value, id, dest, dest_s);
	    case dbMySQL:
	    {
		    cache_set_active(Cache:value);
			ret = cache_get_value_index(row, id, dest, dest_s);
			return ret;
	    }
	}
	return 0;
}

stock ormGetFieldInt(ORMResult:result, id, row = 0)
{
    new value = MEM_get_val(Pointer:result, 0);
    new DBType:type = DBType:MEM_get_val(Pointer:result, 1);

	new ret;
	switch(type)
	{
	    case dbSQLite:		return db_get_field_int(DBResult:value, id);
	    case dbMySQL:
	    {
		    cache_set_active(Cache:value);
			cache_get_value_index_int(row, id, ret);
			return ret;
	    }
	}
	return 0;
}

stock Float:ormGetFieldFloat(ORMResult:result, id, row = 0)
{
    new value = MEM_get_val(Pointer:result, 0);
    new DBType:type = DBType:MEM_get_val(Pointer:result, 1);

	new Float:ret;
	switch(type)
	{
	    case dbSQLite:		return db_get_field_float(DBResult:value, id);
	    case dbMySQL:
	    {
		    cache_set_active(Cache:value);
			cache_get_value_index_float(row, id, ret);
			return ret;
	    }
	}
	return Float:0;
}

stock ormTableIsValid(ORMTable:table)
{
	return MEM_is_valid_ptr(Pointer:table);
}

//Table and table query

stock ORMTable:ormTableCreate(ORMDB:database, nametable[], bool:forced, {_, Float, ORMColumnType}:...)
{
    argInit(3);
    
    new DBType:dbtype = DBType:MEM_get_val(Pointer:database, 1);

	//init
	new Map:map = mapCreate();
	new ORMTable:table = ORMTable:MEM_new(5);
	MEM_set_val(Pointer:table, 1, _:map);
	MEM_set_val(Pointer:table, 2, _:MEM_new_arr(nametable, strlen(nametable)));
	MEM_set_val(Pointer:table, 3, _:database);
	MEM_set_val(Pointer:table, 4, _:listCreate());
	
	//is table create?
	
    format(orm_buffer, sizeof(orm_buffer), "SELECT name FROM sqlite_master WHERE name='%s'", nametable);
	new ORMResult:result = ormQuery(database, false, orm_buffer);
	new num = ormResultNumRows(result);
	ormResultClear(result);
	
	new Pointer:node;
	new ORMColumnType:type;
	
	new stringlen;
	
	new List:listname = listCreate();
	new Pointer:namecolumnNode;

	if(!num)
	{
	    new len = argGetNum();
	    if(len == 0)	return ORMTable:0;
	    //create query for create table

		if(dbtype == dbSQLite)
		{
			format(orm_buffer, sizeof(orm_buffer), "CREATE TABLE %s (id INTEGER PRIMARY KEY AUTOINCREMENT,", nametable);
		}
		else if(dbtype == dbMySQL)
		{
			format(orm_buffer, sizeof(orm_buffer), "CREATE TABLE %s (id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,", nametable);
		}
	    for(new curarg = 0, i = 0; curarg < len; curarg++, i++)
	    {
	        node = MEM_new(4);
	        MEM_set_val(node, 2, 0);

	        type = ORMColumnType:argGetRef(curarg);
	        MEM_set_val(node, 1, _:type);
	        if(type == typeString)
	        {
                curarg++;
        		stringlen = argGetRef(curarg);
        		MEM_set_val(node, 2, stringlen);
	        }
	        curarg++;
	        argGetString(curarg, orm_key);
	        mapAdd(map, orm_key, node, true);

	        namecolumnNode = MEM_new(2);
	        MEM_set_val(namecolumnNode, 0, _:MEM_new_arr(orm_key, strlen(orm_key)));
	        MEM_set_val(namecolumnNode, 1, _:node);
	        listPush(listname, namecolumnNode);

	        if(dbtype == dbSQLite)
	        {
		        switch(type)
		        {
		            case typeInteger:   format(orm_buffer, sizeof(orm_buffer), "%s%s INTEGER", orm_buffer, orm_key);
		            case typeFloat:   	format(orm_buffer, sizeof(orm_buffer), "%s%s FLOAT", orm_buffer, orm_key);
		            case typeString:	format(orm_buffer, sizeof(orm_buffer), "%s%s TEXT", orm_buffer, orm_key);
		        }
			}
			else if(dbtype == dbMySQL)
			{
		        switch(type)
		        {
		            case typeInteger:   format(orm_buffer, sizeof(orm_buffer), "%s%s INT", orm_buffer, orm_key);
		            case typeFloat:   	format(orm_buffer, sizeof(orm_buffer), "%s%s FLOAT", orm_buffer, orm_key);
		            case typeString:
					{
		        		if(1 <= stringlen <= 65536)
		        		{
		        		    format(orm_buffer, sizeof(orm_buffer), "%s%s VARCHAR(%i)", orm_buffer, orm_key, stringlen);
		        		}
		        		else
		        		{
		        		    format(orm_buffer, sizeof(orm_buffer), "%s%s MEDIUMTEXT", orm_buffer, orm_key, stringlen);
		        		}
					}
		        }
			}
	        curarg++;
	        switch(type)
	        {
	            case typeInteger:
				{
					MEM_set_val(node, 3, argGetRef(curarg));
					format(orm_buffer, sizeof(orm_buffer), "%s DEFAULT %i,", orm_buffer, argGetRef(curarg));
				}
				case typeFloat:
				{
					MEM_set_val(node, 3, argGetRef(curarg));
					format(orm_buffer, sizeof(orm_buffer), "%s DEFAULT %f,", orm_buffer, Float:argGetRef(curarg));
				}
				case typeString:
				{
				    argGetString(curarg, orm_key);
				    MEM_set_val(node, 3, _:MEM_new_arr(orm_key, strlen(orm_key)));
				    if(strlen(orm_key))
				    {
				        if(dbtype == dbSQLite)
				        {
							format(orm_buffer, sizeof(orm_buffer), "%s DEFAULT %s,", orm_buffer, orm_key);
						}
                        else if(dbtype == dbMySQL)
                        {
                            format(orm_buffer, sizeof(orm_buffer), "%s DEFAULT '%s',", orm_buffer, orm_key);
                        }
					}
					else
					{
					    format(orm_buffer, sizeof(orm_buffer), "%s,", orm_buffer);
					}
				}
			}
		}
	    if(!num)
		{
			orm_buffer[strlen(orm_buffer)-1] = ')';
//	    	print(orm_buffer);
	    	ormQuery(database, true, orm_buffer);
		}
	}
 	else
	{
		//check columns

		new columnName[64];
		new mark;

		if(dbtype == dbSQLite)
		{
		    format(orm_buffer, sizeof(orm_buffer), "PRAGMA table_info('%s')", nametable);
		    strmid(columnName, "name", 0, 4);
		}
		else if(dbtype == dbMySQL)
		{
		    format(orm_buffer, sizeof(orm_buffer), "DESC %s", nametable);
		    strmid(columnName, "Field", 0, 5);
		}
	    result = ormQuery(database, false, orm_buffer);
		num = ormResultNumRows(result);

        ormResultNext(result);//without id
	    for(new i = 1; i < num; i++, ormResultNext(result))
	    {
	        ormGetFieldAssoc(result, columnName, orm_key, sizeof(orm_key), i);

	        node = MEM_new(4);
	        MEM_set_val(node, 2, 0);

	        namecolumnNode = MEM_new(2);
	        MEM_set_val(namecolumnNode, 0, _:MEM_new_arr(orm_key, strlen(orm_key)));
	        MEM_set_val(namecolumnNode, 1, _:node);
	        listPush(listname, namecolumnNode);

			ormGetFieldAssoc(result, "type", orm_buffer, sizeof(orm_buffer), i);
			if(dbtype == dbSQLite)
			{
				if(!strcmp(orm_buffer, "INTEGER"))
				{
				    type = typeInteger;
				}
				else if(!strcmp(orm_buffer, "FLOAT"))
				{
				    type = typeFloat;
				}
				else if(!strcmp(orm_buffer, "TEXT"))
				{
				    type = typeString;
				}
			}
			else if(dbtype == dbMySQL)
			{
				if(!strcmp(orm_buffer, "int(11)"))
				{
				    type = typeInteger;
				}
				if(!strcmp(orm_buffer, "float"))
				{
				    type = typeFloat;
				}
				else if(!strcmp(orm_buffer, "tinytext"))
				{
				    type = typeString;
	                MEM_set_val(node, 2, 256);
				}
				else if(!strcmp(orm_buffer, "text"))
				{
				    type = typeString;
				    MEM_set_val(node, 2, 65536);
				}
				else if(!strcmp(orm_buffer, "mediumtext"))
				{
				    type = typeString;
				    MEM_set_val(node, 2, 16777216);
				}
				else if(!strcmp(orm_buffer, "longtext"))
				{
				    type = typeString;
				    MEM_set_val(node, 2, -1);
				}
				else
				{
					mark = strfind(orm_buffer, "(");
					strmid(columnName, orm_buffer, mark, strlen(orm_buffer));
					strmid(orm_buffer, orm_buffer, 0, mark);
					if(!strcmp(orm_buffer, "varchar"))
					{
					    type = typeString;
					    MEM_set_val(node, 2, strval(columnName));
					}
				}
			}
			MEM_set_val(node, 1, _:type);
			ormGetFieldAssoc(result, "dflt_value", orm_buffer, sizeof(orm_buffer), i);
			if(strlen(orm_buffer))
			{
				switch(type)
				{
				    case typeInteger: MEM_set_val(node, 3, strval(orm_buffer));
				    case typeFloat: MEM_set_val(node, 3, _:floatstr(orm_buffer));
				    case typeString: MEM_set_val(node, 3, _:MEM_new_arr(orm_buffer, strlen(orm_buffer)));
				}
			}
			else
			{
			    MEM_set_val(node, 3, _:MEM_new_zero(1));
			}
			mapAdd(map, orm_key, node, true);
	    }
	    if(forced)
	    {
		    new len = argGetNum();
		    //create query for create table

		    for(new curarg = 0, i = 0; curarg < len; curarg++, i++)
		    {
		        node = MEM_new(4);
		        MEM_set_val(node, 2, 0);

		        type = ORMColumnType:argGetRef(curarg);
		        MEM_set_val(node, 1, _:type);
		        if(type == typeString)
		        {
	                curarg++;
	        		stringlen = argGetRef(curarg);
	        		MEM_set_val(node, 2, stringlen);
		        }
		        curarg++;
		        argGetString(curarg, orm_key);
		        
		        if(mapIsKeyExist(map, orm_key))
				{
				    curarg++;
					continue;
		        }
		        
		        format(orm_buffer, sizeof(orm_buffer), "ALTER TABLE %s ADD COLUMN ", nametable);
		        
		        mapAdd(map, orm_key, node, true);

		        namecolumnNode = MEM_new(2);
		        MEM_set_val(namecolumnNode, 0, _:MEM_new_arr(orm_key, strlen(orm_key)));
		        MEM_set_val(namecolumnNode, 1, _:node);
		        listPush(listname, namecolumnNode);

		        if(dbtype == dbSQLite)
		        {
			        switch(type)
			        {
			            case typeInteger:   format(orm_buffer, sizeof(orm_buffer), "%s%s INTEGER", orm_buffer, orm_key);
			            case typeFloat:   	format(orm_buffer, sizeof(orm_buffer), "%s%s FLOAT", orm_buffer, orm_key);
			            case typeString:	format(orm_buffer, sizeof(orm_buffer), "%s%s TEXT", orm_buffer, orm_key);
			        }
				}
				else if(dbtype == dbMySQL)
				{
			        switch(type)
			        {
			            case typeInteger:   format(orm_buffer, sizeof(orm_buffer), "%s%s INT", orm_buffer, orm_key);
			            case typeFloat:   	format(orm_buffer, sizeof(orm_buffer), "%s%s FLOAT", orm_buffer, orm_key);
			            case typeString:
						{
			        		if(1 <= stringlen <= 65536)
			        		{
			        		    format(orm_buffer, sizeof(orm_buffer), "%s%s VARCHAR(%i)", orm_buffer, orm_key, stringlen);
			        		}
			        		else
			        		{
			        		    format(orm_buffer, sizeof(orm_buffer), "%s%s MEDIUMTEXT", orm_buffer, orm_key, stringlen);
			        		}
						}
			        }
				}
		        curarg++;
		        switch(type)
		        {
		            case typeInteger:
					{
						MEM_set_val(node, 3, argGetRef(curarg));
						format(orm_buffer, sizeof(orm_buffer), "%s DEFAULT %i,", orm_buffer, argGetRef(curarg));
					}
					case typeFloat:
					{
						MEM_set_val(node, 3, argGetRef(curarg));
						format(orm_buffer, sizeof(orm_buffer), "%s DEFAULT %f,", orm_buffer, Float:argGetRef(curarg));
					}
					case typeString:
					{
					    argGetString(curarg, orm_key);
					    MEM_set_val(node, 3, _:MEM_new_arr(orm_key, strlen(orm_key)));
					    if(strlen(orm_key))
					    {
					        if(dbtype == dbSQLite)
					        {
								format(orm_buffer, sizeof(orm_buffer), "%s DEFAULT %s,", orm_buffer, orm_key);
							}
	                        else if(dbtype == dbMySQL)
	                        {
	                            format(orm_buffer, sizeof(orm_buffer), "%s DEFAULT '%s',", orm_buffer, orm_key);
	                        }
						}
						else
						{
						    format(orm_buffer, sizeof(orm_buffer), "%s,", orm_buffer);
						}
					}
				}
				orm_buffer[strlen(orm_buffer)-1] = '\0';
//				print(orm_buffer);
				ormQuery(database, true, orm_buffer);
			}
	    }
	}
    new Pointer:data = MEM_new(listLen(listname));
    MEM_set_val(Pointer:table, 0, _:data);

	new index = 0;
	listForeachPtr(nod:listname)
	{
		MEM_set_val(data, index, _:nod);
		MEM_set_val(Pointer:MEM_get_val(nod, 1), 0, index);
	    index++;
	}
	listDestroy(listname);
	return table;
}

stock ormTableDestroy(ORMTable:table)
{
	if(!ormTableIsValid(table)) return 0;
	
	new Pointer:data = Pointer:MEM_get_val(Pointer:table, 0);
	new len = MEM_get_size(data);
	new Pointer:namecolumnNode;
//	new Pointer:node;
	for(new i = 0; i < len; i++)
	{
	    namecolumnNode = Pointer:MEM_get_val(data, i);
	    MEM_delete(Pointer:MEM_get_val(namecolumnNode, 0));
	    MEM_delete(Pointer:MEM_get_val(namecolumnNode, 1));
	}
	MEM_delete(data);
	
	mapDestroy(Map:MEM_get_val(Pointer:table, 1));
	MEM_delete(Pointer:MEM_get_val(Pointer:table, 2));
	MEM_delete(Pointer:MEM_get_val(Pointer:table, 3));
	listDestroy(List:MEM_get_val(Pointer:table, 4));
	
	MEM_delete(Pointer:table);
	return 1;
}

stock ORMResult:ormInsert(ORMTable:table, bool:autodelete, {_, Float}:...)
{
	if(!ormTableIsValid(table)) return ORMResult:0;
    argInit(2);

	new len = argGetNum();
	new ORMColumnType:type;
    
    //Узнаём map таблицу таблицы
    new Map:map = Map:MEM_get_val(Pointer:table, 1);
	//Узнаём базу данных
	new ORMDB:db = ORMDB:MEM_get_val(Pointer:table, 3);
	
	new List:sequence = listCreate();
	
	new Pointer:tempNode;
	new Pointer:stringPtr;
	//Построение очереди
	//Добавление в очередь данных, которые мы занесли в параметрах функции
	for(new curarg = 0, Pointer:node; curarg < len; curarg++)
	{
        argGetString(curarg, orm_key);
        
        //Ищем узел колонки через map таблицу
		if(!mapGet(map, orm_key, node))
		{
		    curarg++;
			continue;
		}
		type = ORMColumnType:MEM_get_val(node, 1);

        tempNode = MEM_new(3);
        MEM_set_val(tempNode, 0, _:MEM_new_arr(orm_key, strlen(orm_key)));
        MEM_set_val(tempNode, 1, _:type);
        listPush(sequence, tempNode);

	    curarg++;
	    switch(type)
	    {
	        case typeInteger, typeFloat:
			{
			    MEM_set_val(tempNode, 2, argGetRef(curarg));
			}
			case typeString:
			{
				argGetStringLen(curarg);
			    #emit PUSH.pri
			    argGet(curarg);
			    #emit PUSH.pri
			    #emit PUSH.C 8
			    #emit SYSREQ.C MEM_new_arr
			    #emit STACK 12
			    #emit STOR.S.pri stringPtr
			    MEM_set_val(tempNode, 2, _:stringPtr);
			}
     	}
	}
	
	//Узнаём название таблицы
    new Pointer:tablename = Pointer:MEM_get_val(Pointer:table, 2);
	MEM_get_arr(tablename, 0, orm_buffer, MEM_get_size(tablename));
	orm_buffer[MEM_get_size(tablename)] = '\0';
	
	//Начинаем составлять запрос
	format(orm_buffer, sizeof(orm_buffer), "INSERT INTO %s(", orm_buffer);
	//Добавление названий колонок
	listForeachPtr(node:sequence)
	{
	    tempNode = Pointer:MEM_get_val(node, 0);
	    MEM_get_arr(tempNode, 0, orm_key, MEM_get_size(tempNode));
		orm_key[MEM_get_size(tempNode)] = '\0';
 		format(orm_buffer, sizeof(orm_buffer), "%s%s,", orm_buffer, orm_key);
	}
    orm_buffer[strlen(orm_buffer)-1] = ')';
	format(orm_buffer, sizeof(orm_buffer), "%s VALUES (", orm_buffer);
	//Добавление значений
	listForeachPtr(node:sequence)
	{
	    type = ORMColumnType:MEM_get_val(node, 1);
	    switch(type)
	    {
			case typeInteger:	format(orm_buffer, sizeof(orm_buffer), "%s%i,", orm_buffer, MEM_get_val(node, 2));
			case typeFloat:		format(orm_buffer, sizeof(orm_buffer), "%s%f,", orm_buffer, Float:MEM_get_val(node, 2));
			case typeString:
			{
			    tempNode = Pointer:MEM_get_val(node, 2);
			    MEM_get_arr(tempNode, 0, orm_key, MEM_get_size(tempNode));
				orm_key[MEM_get_size(tempNode)] = '\0';
		 		format(orm_buffer, sizeof(orm_buffer), "%s'%q',", orm_buffer, orm_key);
		 		MEM_delete(tempNode);
			}
		}
	}
	orm_buffer[strlen(orm_buffer)-1] = ')';
	listDestroyPtr(sequence);
//	print(orm_buffer);
    return ormQuery(db, autodelete, orm_buffer);
}

stock ORMResult:ormUpdate(ORMTable:table, expression[], bool:autodelete, {_, Float}:...)
{
    if(!ormTableIsValid(table)) return ORMResult:0;
    
    argInit(3);

	new len = argGetNum();
	new ORMColumnType:type;

    //Узнаём map таблицу таблицы
    new Map:map = Map:MEM_get_val(Pointer:table, 1);
	//Узнаём базу данных
	new ORMDB:db = ORMDB:MEM_get_val(Pointer:table, 3);
    
	//Узнаём название таблицы
    new Pointer:tablename = Pointer:MEM_get_val(Pointer:table, 2);
	MEM_get_arr(tablename, 0, orm_buffer, MEM_get_size(tablename));
	orm_buffer[MEM_get_size(tablename)] = '\0';
	
	//Начинаем составлять запрос
    format(orm_buffer, sizeof(orm_buffer), "UPDATE %s SET ", orm_buffer);
    
	for(new curarg = 0, Pointer:node; curarg < len; curarg++)
	{
        argGetString(curarg, orm_key);

        //Ищем узел колонки через map таблицу
		if(!mapGet(map, orm_key, node))
		{
		    curarg++;
			continue;
		}
		type = ORMColumnType:MEM_get_val(node, 1);

	    curarg++;
	    switch(type)
	    {
	        case typeInteger:
			{
			    format(orm_buffer, sizeof(orm_buffer), "%s%s=%i,", orm_buffer, orm_key, argGetRef(curarg));
			}
			case typeFloat:
			{
			    format(orm_buffer, sizeof(orm_buffer), "%s%s=%f,", orm_buffer, orm_key, Float:argGetRef(curarg));
			}
			case typeString:
			{
				format(orm_buffer, sizeof(orm_buffer), "%s%s=", orm_buffer, orm_key);
				argGetString(curarg, orm_key);
				format(orm_buffer, sizeof(orm_buffer), "%s'%q'", orm_buffer, orm_key);
			}
     	}
	}
	orm_buffer[strlen(orm_buffer)-1] = '\0';
    format(orm_buffer, sizeof(orm_buffer), "%s WHERE %s", orm_buffer, expression);
//    print(orm_buffer);
	return ormQuery(db, autodelete, orm_buffer);
}

stock ORMResult:ormDelete(ORMTable:table, expression[], bool:autodelete)
{
    if(!ormTableIsValid(table)) return ORMResult:0;
	//Узнаём название таблицы
    new Pointer:tablename = Pointer:MEM_get_val(Pointer:table, 2);
	MEM_get_arr(tablename, 0, orm_buffer, MEM_get_size(tablename));
	orm_buffer[MEM_get_size(tablename)] = '\0';
    format(orm_buffer, sizeof(orm_buffer), "DELETE FROM %s WHERE %s", orm_buffer, expression);
//    print(orm_buffer);
	return ormQuery(ORMDB:MEM_get_val(Pointer:table, 3), autodelete, orm_buffer);
}

stock ORMResult:ormSelect(ORMTable:table, expression[], ...)
{
    if(!ormTableIsValid(table)) return ORMResult:0;

    argInit(2);
    
	//Узнаём базу данных
	new ORMDB:db = ORMDB:MEM_get_val(Pointer:table, 3);
    
    new len = argGetNum();
    if(len != 0)
    {
        format(orm_buffer, sizeof(orm_buffer), "SELECT ");
		for(new curarg = 0; curarg < len; curarg++)
		{
	        argGetString(curarg, orm_key);
	        format(orm_buffer, sizeof(orm_buffer), "%s%s,", orm_buffer, orm_key);
	    }
	    orm_buffer[strlen(orm_buffer)-1] = '\0';
    
		//Узнаём название таблицы
	    new Pointer:tablename = Pointer:MEM_get_val(Pointer:table, 2);
		MEM_get_arr(tablename, 0, orm_key, MEM_get_size(tablename));
		orm_key[MEM_get_size(tablename)] = '\0';

	    format(orm_buffer, sizeof(orm_buffer), "%s FROM %s", orm_buffer, orm_key);
	    if(strlen(expression))
	    {
	        format(orm_buffer, sizeof(orm_buffer), "%s WHERE %s", orm_buffer, expression);
	    }
	}
	else
	{
		//Узнаём название таблицы
	    new Pointer:tablename = Pointer:MEM_get_val(Pointer:table, 2);
		MEM_get_arr(tablename, 0, orm_buffer, MEM_get_size(tablename));
		orm_buffer[MEM_get_size(tablename)] = '\0';

	    format(orm_buffer, sizeof(orm_buffer), "SELECT * FROM %s", orm_buffer);
	    if(strlen(expression))
	    {
	        format(orm_buffer, sizeof(orm_buffer), "%s WHERE %s", orm_buffer, expression);
	    }
	}
//	print(orm_buffer);
	return ormQuery(db, false, orm_buffer);
}

//Cache

stock ormCacheIsValid(ORMCache:cache)
{
	return MEM_is_valid_ptr(Pointer:cache);
}

stock ORMCache:ormCacheCreate(ORMTable:table = ORMTable:-1)
{
	//init
	new ORMCache:cache = ORMCache:MEM_new(3);
	MEM_set_val(Pointer:cache, 0, _:table);
	MEM_set_val(Pointer:cache, 1, -1);
	MEM_set_val(Pointer:cache, 2, 0);
	
	if(ormTableIsValid(table))
	{
	    listPush((List:MEM_get_val(Pointer:table, 4)), _:cache);
	}
	return cache;
}

stock ormCacheIsBind(ORMCache:cache)
{
    if(!ormCacheIsValid(cache)) return 0;
	new ORMTable:table = ORMTable:MEM_get_val(Pointer:cache, 0);
    return ormTableIsValid(table);
}

stock ormCacheBind(ORMCache:cache, expression[])
{
	if(!ormCacheIsValid(cache)) return 0;
	
	new ORMResult:result = ormSelect(ORMTable:MEM_get_val(Pointer:cache, 0), expression);
	if(!ormResultNumRows(result))
	{
		ormResultClear(result);
		ormResultDelete(result);
		return 0;
	}
	new id = ormGetFieldAssocInt(result, "id");
	MEM_set_val(Pointer:cache, 1, id);
	ormResultClear(result);
	ormResultDelete(result);
	return 1;
}

stock ormCacheBindResult(ORMCache:cache, ORMResult:result, bool:autodelete)
{
	if(!ormCacheIsValid(cache)) return 0;

	new id = ormGetFieldAssocInt(result, "id");
	MEM_set_val(Pointer:cache, 1, id);
	if(autodelete)
	{
		ormResultClear(result);
		ormResultDelete(result);
	}
	return 1;
}

stock ormCacheUnbind(ORMCache:cache)
{
    if(!ormCacheIsValid(cache) || !ormCacheIsBind(cache)) return 0;
    
    new List:list = List:MEM_get_val(Pointer:MEM_get_val(Pointer:cache, 0), 4);
    
    new id = listFind(list, Pointer:cache);
    if(id != -1)
    {
        listDelete(list, id, .deleteptr = false);
    }
    
    MEM_set_val(Pointer:cache, 1, -1);
    return 1;
}

stock ormCacheDestroy(ORMCache:cache)
{
	if(!ormCacheIsValid(cache)) return 0;
	ormCacheUnbind(cache);
	listDestroyPtr(List:MEM_get_val(Pointer:cache, 2));
    MEM_delete(Pointer:cache);
	return 1;
}

stock ORMTable:ormCacheTable(ORMCache:cache)
{
	return ORMTable:MEM_get_val(Pointer:cache, 0);
}

stock ormCacheID(ORMCache:cache)
{
	return MEM_get_val(Pointer:cache, 1);
}

stock List:ormCacheData(ORMCache:cache)
{
	return List:MEM_get_val(Pointer:cache, 2);
}

stock ormCacheLoad(ORMCache:cache, {_, Float}:...)
{
    if(!ormCacheIsValid(cache) || !ormCacheIsBind(cache)) return 0;
    
    format(orm_key, sizeof(orm_key), "id = %i", MEM_get_val(Pointer:cache, 1));
    new ORMResult:result = ormSelect(ORMTable:MEM_get_val(Pointer:cache, 0), orm_key);
    
    new ORMTable:table = ORMTable:MEM_get_val(Pointer:cache, 0);
	if(!ormTableIsValid(table)) return 0;
    
 	new Map:map = Map:MEM_get_val(Pointer:table, 1);
	new ORMColumnType:type;
	
    new Pointer:data = Pointer:MEM_get_val(Pointer:cache, 2);
    new datamark = 0;
    if(MEM_is_valid_ptr(data))
    {
		MEM_delete(data);
    }
    data = MEM_new(mapGetLen(map));
    MEM_set_val(Pointer:cache, 2, _:data);

	new Pointer:dat = Pointer:MEM_get_val(Pointer:table, 0);
	new len = MEM_get_size(dat);
	new Pointer:namecolumnNode;
	new Pointer:node;
	new index;
	
	argInit(1);
	new argnum = argGetNum();
	if(argnum == 0)
	{
		for(new i = 0; i < len; i++)
		{
		    namecolumnNode = Pointer:MEM_get_val(Pointer:dat, i);
			node = Pointer:MEM_get_val(namecolumnNode, 0);

			MEM_get_arr(node, 0, orm_key, MEM_get_size(node));
			orm_key[MEM_get_size(node)] = '\0';

			node = Pointer:MEM_get_val(namecolumnNode, 1);

		    if(ormGetFieldAssoc(result, orm_key, orm_buffer))
		    {
		        type = ORMColumnType:MEM_get_val(node, 1);
				switch(type)
				{
				    case typeInteger:   MEM_set_val(data, datamark, strval(orm_buffer));
				    case typeFloat:   	MEM_set_val(data, datamark, _:floatstr(orm_buffer));
				    case typeString:   	MEM_set_val(data, datamark, _:MEM_new_arr(orm_buffer, strlen(orm_buffer)));
				}
				datamark++;
			}
		}
	}
	else
    {
        for(new curarg = 0; curarg < argnum; curarg++)
        {
            argGetString(curarg, orm_key);
        	if(ormGetFieldAssoc(result, orm_key, orm_buffer))
        	{
			    mapGet(map, orm_key, node);
			    index = MEM_get_val(node, 0);
		        type = ORMColumnType:MEM_get_val(node, 1);
				switch(type)
				{
				    case typeInteger:   MEM_set_val(data, index, strval(orm_buffer));
				    case typeFloat:   	MEM_set_val(data, index, _:floatstr(orm_buffer));
				    case typeString:   	MEM_set_val(data, index, _:MEM_new_arr(orm_buffer, strlen(orm_buffer)));
				}
			}
		}
    }
    return 1;
}

stock ormCacheSave(ORMCache:cache, {_, Float}:...)
{
    if(!ormCacheIsValid(cache) || !ormCacheIsBind(cache)) return 0;
    
    argInit(1);

    new ORMTable:table = ORMTable:MEM_get_val(Pointer:cache, 0);
	if(!ormTableIsValid(table)) return 0;

 	new Map:map = Map:MEM_get_val(Pointer:table, 1);
	//Узнаём базу данных
	new ORMDB:db = ORMDB:MEM_get_val(Pointer:table, 3);
	
    new Pointer:data = Pointer:MEM_get_val(Pointer:cache, 2);
    new datamark = 0;
    if(!MEM_is_valid_ptr(data)) return 0;
    if(mapGetLen(map) != MEM_get_size(data))    return 0;
    
	//Узнаём название таблицы
    new Pointer:tablename = Pointer:MEM_get_val(Pointer:table, 2);
	MEM_get_arr(tablename, 0, orm_buffer, MEM_get_size(tablename));
	orm_buffer[MEM_get_size(tablename)] = '\0';
    
    new ORMColumnType:type;
    new Pointer:tempNode;
    
	//Начинаем составлять запрос
    format(orm_buffer, sizeof(orm_buffer), "UPDATE %s SET ", orm_buffer);
    
 	new Pointer:dat = Pointer:MEM_get_val(Pointer:table, 0);
	new len = MEM_get_size(dat);
	new Pointer:namecolumnNode;
	new Pointer:node;
    new index;
    
    new argnum = argGetNum();
    if(argnum == 0)
    {
		for(new i = 0; i < len; i++)
		{
		    namecolumnNode = Pointer:MEM_get_val(Pointer:dat, i);
			node = Pointer:MEM_get_val(namecolumnNode, 0);

			MEM_get_arr(node, 0, orm_key, MEM_get_size(node));
			orm_key[MEM_get_size(node)] = '\0';

			node = Pointer:MEM_get_val(namecolumnNode, 1);

	 		type = ORMColumnType:MEM_get_val(node, 1);
			switch(type)
			{
			    case typeInteger:   format(orm_buffer, sizeof(orm_buffer), "%s%s=%i,", orm_buffer, orm_key, MEM_get_val(data, datamark));
			    case typeFloat:   	format(orm_buffer, sizeof(orm_buffer), "%s%s=%f,", orm_buffer, orm_key, Float:MEM_get_val(data, datamark));
			    case typeString:
			    {
			        format(orm_buffer, sizeof(orm_buffer), "%s%s='", orm_buffer, orm_key);
			        tempNode = Pointer:MEM_get_val(data, datamark);
			        MEM_get_arr(tempNode, 0, orm_key, MEM_get_size(tempNode));
			        orm_key[MEM_get_size(tempNode)] = '\0';
			        format(orm_buffer, sizeof(orm_buffer), "%s%q',", orm_buffer, orm_key);
			    }
			}
			datamark++;
		}
	}
	else
	{
        for(new curarg = 0; curarg < argnum; curarg++)
        {
            argGetString(curarg, orm_key);

		    mapGet(map, orm_key, node);
		    index = MEM_get_val(node, 0);
	        type = ORMColumnType:MEM_get_val(node, 1);
			switch(type)
			{
			    case typeInteger:   format(orm_buffer, sizeof(orm_buffer), "%s%s=%i,", orm_buffer, orm_key, MEM_get_val(data, index));
			    case typeFloat:   	format(orm_buffer, sizeof(orm_buffer), "%s%s=%f,", orm_buffer, orm_key, Float:MEM_get_val(data, index));
			    case typeString:
			    {
			        format(orm_buffer, sizeof(orm_buffer), "%s%s='", orm_buffer, orm_key);
			        tempNode = Pointer:MEM_get_val(data, index);
			        MEM_get_arr(tempNode, 0, orm_key, MEM_get_size(tempNode));
			        orm_key[MEM_get_size(tempNode)] = '\0';
			        format(orm_buffer, sizeof(orm_buffer), "%s%q',", orm_buffer, orm_key);
			    }
			}
		}
	}
	orm_buffer[strlen(orm_buffer)-1] = '\0';
    format(orm_buffer, sizeof(orm_buffer), "%s WHERE id = %i", orm_buffer, MEM_get_val(Pointer:cache, 1));
//    print(orm_buffer);
	ormQuery(db, true, orm_buffer);
    return 1;
}

stock ormCacheGet(ORMCache:cache, column[], output[], maxlength = sizeof(output))
{
    if(!ormCacheIsValid(cache)) return 0;
    new ORMTable:table = ORMTable:MEM_get_val(Pointer:cache, 0);
	if(!ormTableIsValid(table)) return 0;
	new Map:map = Map:MEM_get_val(Pointer:table, 1);
	
	new Pointer:node;
    mapGet(map, column, node);
	new index = MEM_get_val(node, 0);
	new ORMColumnType:type = ORMColumnType:MEM_get_val(node, 1);
	
	new Pointer:data = Pointer:MEM_get_val(Pointer:cache, 2);
	node = Pointer:MEM_get_val(data, index);

    switch(type)
    {
        case typeInteger:	format(output, maxlength, "%i", _:node);
        case typeFloat:		format(output, maxlength, "%f", Float:node);
        case typeString:
        {
			new len = min(MEM_get_size(node), maxlength-1);
			MEM_get_arr(node, 0, output, len);
			output[len] = '\0';
		}
	}
    return 1;
}

stock ormCacheGetInt(ORMCache:cache, column[], &output)
{
    if(!ormCacheIsValid(cache)) return 0;

    new ORMTable:table = ORMTable:MEM_get_val(Pointer:cache, 0);
	if(!ormTableIsValid(table)) return 0;

	new Map:map = Map:MEM_get_val(Pointer:table, 1);

	new Pointer:node;
    mapGet(map, column, node);
	new index = MEM_get_val(node, 0);
	new ORMColumnType:type = ORMColumnType:MEM_get_val(node, 1);

	new Pointer:data = Pointer:MEM_get_val(Pointer:cache, 2);
	node = Pointer:MEM_get_val(data, index);

    switch(type)
    {
        case typeInteger:	output = _:node;
        case typeFloat:		output = floatround(Float:node);
        case typeString:
        {
			new len = min(MEM_get_size(node), sizeof(orm_buffer)-1);
			MEM_get_arr(node, 0, orm_buffer, len);
			orm_buffer[len] = '\0';
			output = strval(orm_buffer);
		}
	}
    return 1;
}

stock ormCacheGetFloat(ORMCache:cache, column[], &Float:output)
{
    if(!ormCacheIsValid(cache)) return 0;

    new ORMTable:table = ORMTable:MEM_get_val(Pointer:cache, 0);
	if(!ormTableIsValid(table)) return 0;

	new Map:map = Map:MEM_get_val(Pointer:table, 1);

	new Pointer:node;
    mapGet(map, column, node);
	new index = MEM_get_val(node, 0);
	new ORMColumnType:type = ORMColumnType:MEM_get_val(node, 1);

	new Pointer:data = Pointer:MEM_get_val(Pointer:cache, 2);
	node = Pointer:MEM_get_val(data, index);

    switch(type)
    {
        case typeInteger:	output = float(_:node);
        case typeFloat:		output = Float:node;
        case typeString:
        {
			new len = min(MEM_get_size(node), sizeof(orm_buffer)-1);
			MEM_get_arr(node, 0, orm_buffer, len);
			orm_buffer[len] = '\0';
			output = floatstr(orm_buffer);
		}
	}
    return 1;
}

stock ormCacheSet(ORMCache:cache, column[], input[])
{
    if(!ormCacheIsValid(cache)) return 0;

    new ORMTable:table = ORMTable:MEM_get_val(Pointer:cache, 0);
	if(!ormTableIsValid(table)) return 0;

	new Map:map = Map:MEM_get_val(Pointer:table, 1);

	new Pointer:node;
    mapGet(map, column, node);
	new index = MEM_get_val(node, 0);
	new ORMColumnType:type = ORMColumnType:MEM_get_val(node, 1);

	new Pointer:data = Pointer:MEM_get_val(Pointer:cache, 2);
	node = Pointer:MEM_get_val(data, index);

    switch(type)
    {
        case typeInteger:	MEM_set_val(data, index, strval(input));
        case typeFloat:		MEM_set_val(data, index, _:floatstr(input));
        case typeString:
		{
			if(MEM_is_valid_ptr(Pointer:MEM_get_val(data, index)))  MEM_delete(Pointer:MEM_get_val(data, index));
			MEM_set_val(data, index, _:MEM_new_arr(input, strlen(input)));
		}
	}
	return 1;
}

stock ormCacheSetInt(ORMCache:cache, column[], input)
{
    if(!ormCacheIsValid(cache)) return 0;

    new ORMTable:table = ORMTable:MEM_get_val(Pointer:cache, 0);
	if(!ormTableIsValid(table)) return 0;

	new Map:map = Map:MEM_get_val(Pointer:table, 1);

	new Pointer:node;
    mapGet(map, column, node);
	new index = MEM_get_val(node, 0);
	new ORMColumnType:type = ORMColumnType:MEM_get_val(node, 1);

	new Pointer:data = Pointer:MEM_get_val(Pointer:cache, 2);
	node = Pointer:MEM_get_val(data, index);

    switch(type)
    {
        case typeInteger:	MEM_set_val(data, index, input);
        case typeFloat:		MEM_set_val(data, index, _:float(input));
        case typeString:
		{
			if(MEM_is_valid_ptr(Pointer:MEM_get_val(data, index)))  MEM_delete(Pointer:MEM_get_val(data, index));
			format(orm_buffer, sizeof(orm_buffer), "%i", input);
			MEM_set_val(data, index, _:MEM_new_arr(orm_buffer, strlen(orm_buffer)));
		}
	}
	return 1;
}

stock ormCacheSetFloat(ORMCache:cache, column[], Float:input)
{
    if(!ormCacheIsValid(cache)) return 0;

    new ORMTable:table = ORMTable:MEM_get_val(Pointer:cache, 0);
	if(!ormTableIsValid(table)) return 0;

	new Map:map = Map:MEM_get_val(Pointer:table, 1);

	new Pointer:node;
    mapGet(map, column, node);
	new index = MEM_get_val(node, 0);
	new ORMColumnType:type = ORMColumnType:MEM_get_val(node, 1);

	new Pointer:data = Pointer:MEM_get_val(Pointer:cache, 2);
	node = Pointer:MEM_get_val(data, index);

    switch(type)
    {
        case typeInteger:	MEM_set_val(data, index, floatround(input));
        case typeFloat:		MEM_set_val(data, index, _:input);
        case typeString:
		{
			if(MEM_is_valid_ptr(Pointer:MEM_get_val(data, index)))  MEM_delete(Pointer:MEM_get_val(data, index));
			format(orm_buffer, sizeof(orm_buffer), "%f", input);
			MEM_set_val(data, index, _:MEM_new_arr(orm_buffer, strlen(orm_buffer)));
		}
	}
	return 1;
}

stock ormCacheDelete(ORMCache:cache)
{
    if(!ormCacheIsValid(cache) || !ormCacheIsBind(cache)) return 0;
	format(orm_buffer, sizeof(orm_buffer), "id = %i", MEM_get_val(Pointer:cache, 1));
	ormDelete(ORMTable:MEM_get_val(Pointer:cache, 0), orm_buffer, true);
	ormCacheUnbind(cache);
	return 1;
}

stock ORMCache:ormInsertMaster(ORMTable:table, {_, Float}:...)
{
	argInit(1);

	new numarg = (argGetNum()+2)<<2;
	
	argPush();
	#emit PUSH.C 0
	#emit PUSH.S table
	#emit PUSH.S numarg
	
	#emit CONST.pri ormInsert
	#emit PUSH.pri
	AddressCall();
	
	new ORMDB:database = ORMDB:MEM_get_val(Pointer:table, 3);
	new ORMColumnType:type = ORMColumnType:MEM_get_val(Pointer:database, 1);

	new ORMResult:result;
	switch(type)
	{
	    case dbSQLite:	result = ormQuery(database, false, "SELECT last_insert_rowid()");
		case dbMySQL:	result = ormQuery(database, false, "SELECT LAST_INSERT_ID()");
	}
	
	new ORMCache:cache = ormCacheCreate(table);
	MEM_set_val(Pointer:cache, 1, ormGetFieldInt(result, 0));
	ormResultClear(result);
	ormResultDelete(result);
	return cache;
}
