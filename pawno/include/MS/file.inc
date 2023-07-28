#if defined mdg_file_inc
	#endinput
#endif
#define mdg_file_inc

#include <filemanager>

#include <MS/separator>
#include <MS/map>

/*
	struct:
		iniNode:
	    	filename
	    	index of map:
	    	    key - value of ini
*/

static mdgFileStr[4096];

stock fmFileMove(file[], destination[])	return file_move(file, destination);
stock fmFileCopy(file[], destination[])	return file_copy(file, destination);
stock fmFileDelete(file[])	return file_delete(file);
stock fmFileCreate(file[])	return file_create(file);
stock fmFileExists(file[])	return file_exists(file);

stock File:fmFileOpen(file[], mode[] = "r")	return f_open(file, mode);
stock fmFileClose(File:file)	return f_close(file);
stock fmFileRead(File:file, storage[], size = sizeof(storage))	return f_read(file, storage, size);
stock fmFileWrite(File:file, string[])    return f_write(file, string);
stock fmFileReadFull(file[], storage[], size = sizeof(storage))	return file_read(file, storage, size);
stock fmFileWriteFull(file[], text[], mode[] = "a")	return file_write(file, text, mode);

stock fmDirectoryCreate(directory[])	return dir_create(directory);
stock fmDirectoryDelete(directory[])	return dir_delete(directory);
stock fmDirectoryExists(directory[])	return dir_exists(directory);
stock dir:fmDirectoryOpen(directory[])	return dir_open(directory);
stock fmDirectoryClose(dir:handle)	return dir_close(handle);
stock fmDirectoryRead(dir:handle, storage[], &type, length = sizeof(storage))	return dir_list(handle, storage, type, length);

stock ININode:iniOpen(file[], terminator[] = "=", comment[] = ";")
{
	static key[2048];
	static value[2048];

	new File:ini = fmFileOpen(file, "r");
	if(!ini)    return ININode:MEM_NULLPTR;
	
	new Map:map = mapCreate();
	new ININode:iniNode = ININode:MEM_new(2);
	
	new Pointer:temp = MEM_new_arr(file, strlen(file));
	
	MEM_set_val(Pointer:iniNode, 0, _:temp);
	MEM_set_val(Pointer:iniNode, 1, _:map);

	MEM_get_arr(temp, 0, mdgFileStr, MEM_get_size(temp));
	mdgFileStr[MEM_get_size(temp)] = '\0';

	new i = 0;
	new mark;
	while(fmFileRead(ini, mdgFileStr))
	{
		mark = strfind(mdgFileStr, comment);
		if(mark != -1)
		{
		    strmid(mdgFileStr, mdgFileStr, 0, mark);
		}
        if(!strKeyValueSeparator(mdgFileStr, key, value, terminator) || !strlen(key))	continue;
        mapAdd(map, key, MEM_new_arr(value, strlen(value)));
		i++;
	}
	fmFileClose(ini);
	return iniNode;
}

stock iniClose(ININode:ini)
{
	if(!MEM_is_valid_ptr(Pointer:ini))  return 0;
	MEM_delete(Pointer:MEM_get_val(Pointer:ini, 0));
    mapDestroy(Map:MEM_get_val(Pointer:ini, 1), true);
	return 1;
}

stock iniGetValue(ININode:ini, key[], value[], value_size = sizeof(value))
{
    if(!MEM_is_valid_ptr(Pointer:ini))  return 0;
	new Pointer:ptr;
    if(mapGet(Map:MEM_get_val(Pointer:ini, 1), key, ptr) == 0)    return 0;
    new len = min(MEM_get_size(ptr), value_size-1);
    MEM_get_arr(ptr, 0, value, len);
    value[len] = '\0';
    return 1;
}

stock iniGetString(ININode:ini, key[], string[], string_size = sizeof(string))
{
	return iniGetValue(ini, key, string, string_size);
}

stock iniGetInteger(ININode:ini, key[])
{
	if(!iniGetValue(ini, key, mdgFileStr)) return 0;
	return strval(mdgFileStr);
}

stock Float:iniGetFloat(ININode:ini, key[])
{
	if(!iniGetValue(ini, key, mdgFileStr)) return 0;
	return floatstr(mdgFileStr);
}

stock iniSetValue(ININode:ini, key[], value[])
{
    if(!MEM_is_valid_ptr(Pointer:ini))  return 0;
	return mapAdd(Map:MEM_get_val(Pointer:ini, 1), key, MEM_new_arr(value, strlen(value)), true);
}

stock iniSetString(ININode:ini, key[], value[])
{
	return iniSetValue(ini, key, value);
}

stock iniSetInteger(ININode:ini, key[], intvar)
{
	static form[32];
	format(form, sizeof(form), "%i", intvar);
	return iniSetValue(ini, key, form);
}

stock iniSetFloat(ININode:ini, key[], Float:floatvar)
{
	static form[32];
	format(form, sizeof(form), "%f", floatvar);
	return iniSetValue(ini, key, form);
}

stock iniGetName(ININode:ini, name[], size_name = sizeof(name))
{
	new Pointer:temp = Pointer:MEM_get_val(Pointer:ini, 0);
	new len = min(MEM_get_size(temp), size_name-1);
	MEM_get_arr(temp, 0, name, len);
	name[len] = '\0';
	return 0;
}

stock iniUpdate(ININode:iniNode, terminator[] = "=", bool:addspaces = true)
{
    if(!MEM_is_valid_ptr(Pointer:iniNode))  return 0;
	new Map:map = Map:MEM_get_val(Pointer:iniNode, 1);

    iniGetName(iniNode, mdgFileStr);

	new File:ini = fmFileOpen(mdgFileStr, "w");
	if(!ini)    return 0;
	
	new len;

	new key[2048];
	mapForeachPtr(key[2048], value:map)
	{
		strmid(mdgFileStr, key, 0, strlen(key), sizeof(mdgFileStr));

        if(!addspaces)
        {
            strcat(mdgFileStr, terminator);
        }
        else
        {
            len = strlen(mdgFileStr);
            mdgFileStr[len] = ' ';
            mdgFileStr[len+1] = '\0';
            strcat(mdgFileStr, terminator);
            len = strlen(mdgFileStr);
            mdgFileStr[len] = ' ';
            mdgFileStr[len+1] = '\0';
        }

        len = strlen(mdgFileStr)+MEM_get_size(value);
        MEM_get_arr(value, 0, mdgFileStr[strlen(mdgFileStr)], MEM_get_size(value));

		mdgFileStr[len] = '\n';
        mdgFileStr[len+1] = '\0';

        fmFileWrite(ini, mdgFileStr);
	}

    fmFileClose(ini);
	return 1;
}

stock iniIsKeyExist(ININode:iniNode, key[])
{
	static temp;
	return (mapGet(Map:MEM_get_val(Pointer:iniNode, 1), key, temp) == 0) ? 0 : 1;
}

stock iniReopen(&ININode:ini, terminator[] = "=", comment[] = ";")
{
    iniGetName(ini, mdgFileStr);
    iniClose(ini);
    ini = iniOpen(mdgFileStr, terminator, comment);
	return 1;
}
