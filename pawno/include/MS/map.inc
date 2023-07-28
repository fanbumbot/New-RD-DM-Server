#if defined mdg_map_inc
	#endinput
#endif
#define mdg_map_inc

#include <memory>
#include <gvar>

static map_temp[4096];
static map_key[4096];
static mdg_map_iter;

stock mapForeachFunc(Map:map, &i, key[], size, &value)
{
	static len;
	
	key[0] = '\0';
	
    format(map_temp, sizeof(map_temp), "mapI%i", _:map);
 	len = GetGVarInt(map_temp, 0);
	GetGVarString(map_temp, key, size, i+2);
	format(map_temp, sizeof(map_temp), "map%i_%s", _:map, key);
	value = GetGVarInt(map_temp);

    i++;
	if(i == len)
	{
	    return 0;
	}
 	return 1;
}

#define mapForeach(%2[%3],%0:%1) \
	for(new %0, %0_i = 0; mapForeachFunc(%1, %0_i, %2, %3, %0);)

#define mapForeachPtr(%2[%3],%0:%1) \
	for(new Pointer:%0, %0_i = 0; mapForeachFunc(%1, %0_i, %2, %3, _:%0);)


stock mapIsExist(Map:map)
{
    format(map_temp, sizeof(map_temp), "mapI%i", _:map);
    return GetGVarType(map_temp) == GLOBAL_VARTYPE_NONE ? 0 : 1;
}

stock Map:mapCreate()
{
    format(map_temp, sizeof(map_temp), "mapI%i", _:mdg_map_iter);
    SetGVarInt(map_temp, 1, 0);
    mdg_map_iter++;
    return Map:(mdg_map_iter-1);
}

stock mapDestroy(Map:map, bool:isvaluestring = false)
{
    if(!mapIsExist(map))    return 0;
	new num = GetGVarInt(map_temp, 0)+1;

    format(map_key, sizeof(map_key), "mapI%i", _:map);
	for(new i = 1; i < num; i++)
	{
	    map_temp[0] = '\0';
		GetGVarString(map_key, map_temp, i);
		DeleteGVar(map_key, i);
		format(map_temp, sizeof(map_temp), "map%i_%s", _:map, map_temp);
		if(isvaluestring)
		{
			MEM_delete(Pointer:GetGVarInt(map_temp));
		}
		DeleteGVar(map_temp);
	}
	DeleteGVar(map_key, 0);
	return 1;
}

stock mapAdd(Map:map, key[], {_, Pointer}:value, bool:forced = true)
{
    if(!mapIsExist(map))    return 0;
    format(map_temp, sizeof(map_temp), "map%i_%s", _:map, key);
    if(GetGVarType(map_temp) != GLOBAL_VARTYPE_NONE)
    {
        if(!forced)
        {
        	return 0;
		}
		else
		{
            SetGVarInt(map_temp, _:value);
		}
	}
	else
	{
	    SetGVarInt(map_temp, _:value);
	    format(map_temp, sizeof(map_temp), "mapI%i", _:map);
	    new num = GetGVarInt(map_temp, 0);
	    SetGVarInt(map_temp, num+1, 0);
	    SetGVarString(map_temp, key, num+1);
	}
	return 1;
}

stock mapGet(Map:map, key[], &{_, Pointer}:value)
{
    if(!mapIsExist(map))    return 0;
    format(map_temp, sizeof(map_temp), "map%i_%s", _:map, key);
    new temp = GetGVarType(map_temp);
    if(temp != GLOBAL_VARTYPE_INT)
    {
        return 0;
    }
	value = GetGVarInt(map_temp);
	return 1;
}

stock mapGetLen(Map:map)
{
	if(!mapIsExist(map))    return 0;
    format(map_temp, sizeof(map_temp), "mapI%i", _:map);
 	return GetGVarInt(map_temp, 0)-1;
}

stock mapIsKeyExist(Map:map, key[])
{
	if(!mapIsExist(map))    return 0;
	format(map_temp, sizeof(map_temp), "map%i_%s", _:map, key);
	return (GetGVarType(map_temp) != GLOBAL_VARTYPE_NONE);
}