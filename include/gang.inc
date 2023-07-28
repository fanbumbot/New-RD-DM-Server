#if defined inc_gang
	#endinput
#endif
#define inc_gang

#if !defined MAX_GANG_NAME_LEN
	#define MAX_GANG_NAME_LEN 32
#endif

new ORMTable:GangTable;
new ItemInfo:Gang;

static List:allgangs;

public OnScriptInit()
{
    GangTable = ormTableCreate(maindb, "Gangs", true,
        typeString, MAX_GANG_NAME_LEN, "Name", "",
        typeString, 64, "Date", "",
        typeInteger, "Type", 0,
        typeInteger, "Color", 0,
        typeInteger, "Score", 0,
        typeFloat, "SpawnX", 0.0,
        typeFloat, "SpawnY", 0.0,
        typeFloat, "SpawnZ", 0.0,
        typeFloat, "SpawnRZ", 0.0,
        typeInteger, "SpawnInterior", -1,
        typeInteger, "Skin0", -1,
        typeInteger, "Skin1", -1,
        typeInteger, "Skin2", -1,
        typeInteger, "Skin3", -1,
        typeInteger, "Skin4", -1,
        typeInteger, "Skin5", -1
	);
	
	Gang = itemInfoCreate();
	itemInfoAdd(Gang, "Cache");
	itemInfoAdd(Gang, "ListPlayers");
	
	allgangs = listCreate();
	
	//Загрузка банд
	new ORMResult:result = ormSelect(GangTable, "", "id");
	new num = ormResultNumRows(result);
	new id;
	new ORMCache:cache;
	new Item:gang;
	
	new string[64];
	
	for(new i = 0; i < num; i++, ormResultNext(result))
	{
	    cache = ormCacheCreate(GangTable);
	    id = ormGetFieldInt(result, 0, i);
	    format(string, sizeof(string), "id = %i", id);
	    ormCacheBind(cache, string);
	    gang = itemCreate(Gang, id);
	    listPush(allgangs, id);
	    
	    itemSetInt(gang, "Cache", _:cache);
	    itemSetList(gang, "ListPlayers", listCreate(), true);
	    
	    ormCacheLoad(cache);
	}

#if defined gang_OnScriptInit
    gang_OnScriptInit();
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
#define OnScriptInit gang_OnScriptInit
#if defined gang_OnScriptInit
forward gang_OnScriptInit();
#endif

public OnScriptExit()
{
#if defined gang_OnScriptExit
    gang_OnScriptExit();
#endif
    listDestroy(allgangs);
    ormTableDestroy(GangTable);
	itemInfoDestroy(Gang);
	return 1;
}
#if defined _ALS_OnScriptExit
	#undef OnScriptExit
#else
    #define _ALS_OnScriptExit
#endif
#if defined OnScriptExit
	#undef OnScriptExit
#endif
#define OnScriptExit gang_OnScriptExit
#if defined gang_OnScriptExit
forward gang_OnScriptExit();
#endif

stock List:GetListGangs()	return allgangs;

stock IsGangExist(gangid)
{
	return itemIsValid(itemGetByID(Gang, gangid));
}

stock IsInputGang(string[])
{
	new len = strlen(string);
	new c;
	for(new i = 0; i < len; i++)
	{
		c = string[i];
		if(!IsDigit(c) && !IsAlphabet(c) && !IsRusAlphabet(c) && c != '_' && c != '{' && c != '}' && c != ' ')	return 0;
	}
	return 1;
}

stock IsInputGangDark(string[])
{
	static string2[7];
	new mark1 = -1;
	new mark2 = -1;
	new RGB:color;
	while(mark2 == mark2)
	{
	    mark1 = strfind(string, "{", .pos = mark2+1);
	    if(mark1 == -1) break;
	    mark2 = strfind(string, "}", .pos = mark1+1);
	    if(mark2 == -1) return -1;
	    strmid(string2, string, mark1+1, mark2);
	    if(!HexToRGB(string2, color))    return -1;
		if(_:color < 0x404040)    return 1;
	}
	return 0;
}

stock CreateGang(name[], {_, RGB}:color, type)
{
	static datereg[64];

	if(strlen(name) >= MAX_GANG_NAME_LEN)	return 0;
	color &= 0xFFFFFF;

    new y, m, d, h, mi;
	getdate(y, m, d);
	gettime(h, mi);
	format(datereg, sizeof(datereg), "%02i.%02i.%i / %02i:%02i", d, m, y, h, mi);

	new ORMCache:cache = ormInsertMaster(GangTable,
		"Name", name,
		"Date", datereg,
		"Type", type,
		"Color", color
	);
	
	new id = ormCacheID(cache);
	
	ormCacheLoad(cache);
	
	new Item:gang = itemCreate(Gang, id);
	itemSetInt(gang, "Cache", _:cache);
	
	listPush(allgangs, id);
	return id;
}

stock DestroyGang(gangid)
{
	static string[64];

	new Item:gang = itemGetByID(Gang, gangid);
	if(!itemIsValid(gang))  return 0;
	
	//Удаляем всех онлайн игроков из банды (не обновляем на аккаунте)
	new List:list = List:itemGetInt(gang, "ListPlayers");
	new Item:player;
	new ORMCache:cache;
	listForeach(playerid:list)
	{
	    player = itemGetByID(Player, playerid);
	    cache = ORMCache:itemGetInt(player, "Cache");
	    ormCacheSetInt(cache, "GangID", -1);
	    ormCacheSetInt(cache, "GangLvl", 0);
	}
	
	//Обнуляем банду и уровень в банде всем аккаунтам в базе данных
	format(string, sizeof(string), "GangID = %i", gangid);
	ormUpdate(AccountTable, string, true, "GangID", -1, "GangLvl", 0);
	
	//Удаление записи из базы данных и удаление кэша
	cache = ORMCache:itemGetInt(gang, "Cache");
	ormCacheDelete(cache);
	ormCacheDestroy(cache);
	
	//Удаление item
	itemDestroy(gang);
	
	listDeleteValue(allgangs, gangid, false);
	return 1;
}

public OnPlayerConnect(playerid)
{
	new gangid = PlayerGetInt(playerid, "GangID");
	if(IsGangExist(gangid))
	{
	    new List:list = itemGetList(itemGetByID(Gang, gangid), "ListPlayers");
	    listPush(list, playerid);
	}

#if defined gang_OnPlayerConnect
    gang_OnPlayerConnect(playerid);
#endif
	return 1;
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect gang_OnPlayerConnect
#if defined gang_OnPlayerConnect
forward gang_OnPlayerConnect(playerid);
#endif

public OnPlayerDisconnect(playerid, reason)
{
#if defined gang_OnPlayerDisconnect
    gang_OnPlayerDisconnect(playerid, reason);
#endif

	new gangid = PlayerGetInt(playerid, "GangID");
	if(IsGangExist(gangid))
	{
	    new List:list = itemGetList(itemGetByID(Gang, gangid), "ListPlayers");
	    listDeleteValue(list, playerid, false);
	}

	return 1;
}
#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect gang_OnPlayerDisconnect
#if defined gang_OnPlayerDisconnect
forward gang_OnPlayerDisconnect(playerid, reason);
#endif

stock GetPlayerGang(playerid)
{
	return PlayerGetInt(playerid, "GangID");
}

stock GetPlayerGangLevel(playerid)
{
	return PlayerGetInt(playerid, "GangLvl");
}

stock PlayerEnterGang(playerid, gangid)
{
	if(!IsPlayerRegistered(playerid) || !IsGangExist(gangid))    return 0;
	
	new ORMCache:cache = ORMCache:itemGetInt(itemGetByID(Player, playerid), "Cache");
	
	ormCacheSetInt(cache, "GangID", gangid);
	ormCacheSetInt(cache, "GangLvl", 1);
	ormCacheSave(cache, "GangID", "GangLvl");
	
	new Item:gang = itemGetByID(Gang, gangid);
	
	new List:list = List:itemGetInt(gang, "ListPlayers");
	listPush(list, playerid);
	
	new color, skin;
	
	cache = ORMCache:itemGetInt(gang, "Cache");
	ormCacheGetInt(cache, "Color", color);
	ormCacheGetInt(cache, "Skin0", skin);
	
	SetPlayerGeneralColor(playerid, (color<<8) | 0xFF);
	SetPlayerGeneralSkin(playerid, skin);
	UpdatePlayerGeneral(playerid);
	return 1;
}

stock PlayerLeaveGang(playerid)
{
	if(!IsPlayerRegistered(playerid))    return 0;
	new gangid = PlayerGetInt(playerid, "GangID");
	if(!IsGangExist(gangid))    return 0;

	new ORMCache:cache = ORMCache:itemGetInt(itemGetByID(Player, playerid), "Cache");

	ormCacheSetInt(cache, "GangID", -1);
	ormCacheSetInt(cache, "GangLvl", 0);
	ormCacheSave(cache, "GangID", "GangLvl");
	
	new List:list = List:itemGetInt(itemGetByID(Gang, gangid), "ListPlayers");
	listDeleteValue(list, playerid, false);
	return 1;
}

stock PlayerSetGangLevel(playerid, level)
{
	if(!IsPlayerRegistered(playerid))    return 0;
	new gangid = PlayerGetInt(playerid, "GangID");
	if(!IsGangExist(gangid))    return 0;
	if(level <= 0)   return 0;
	new ORMCache:cache = ORMCache:itemGetInt(itemGetByID(Player, playerid), "Cache");
	ormCacheSetInt(cache, "GangLvl", level);
	ormCacheSave(cache, "GangLvl");
	
	new skin = GetGangSkin(gangid, level);
	if(skin != -1)
	{
		SetPlayerGeneralSkin(playerid, skin);
	}
	return 1;
}

stock GetGangScore(gangid)
{
	if(!IsGangExist(gangid))    return 0;
	new ORMCache:cache = ORMCache:itemGetInt(itemGetByID(Gang, gangid), "Cache");
	new score
	ormCacheGet(cache, "Score", score);
	return score;
}

stock SetGangScore(gangid, score)
{
	if(!IsGangExist(gangid))    return 0;
	new ORMCache:cache = ORMCache:itemGetInt(itemGetByID(Gang, gangid), "Cache");
	ormCacheSet(cache, "Score", score);
	ormCacheSave(cache, "Score");
	return 1;
}

stock GetGangSkin(gangid, level)
{
    static string[32];
	if(level <= 0)  return 0;
    if(!IsGangExist(gangid))    return 0;
    
    new ORMCache:cache = ORMCache:itemGetInt(itemGetByID(Gang, gangid), "Cache");
    format(string, sizeof(string), "Skin%i", level-1);
    new skin;
    if(!ormCacheGetInt(cache, string, skin))    return -1;
    return skin;
}

stock SetGangSkin(gangid, level, skin)
{
    static string[32];
	if(skin < 0 || skin > 311)  return 0;
	if(level <= 0)  return 0;
    if(!IsGangExist(gangid))    return 0;

    new ORMCache:cache = ORMCache:itemGetInt(itemGetByID(Gang, gangid), "Cache");
    format(string, sizeof(string), "Skin%i", level-1);
    if(!ormCacheSetInt(cache, string, skin))    return 0;
    
    new List:list = GetListPlayers();
	listForeach(playerid:list)
	{
	    if(GetPlayerGang(playerid) == gangid && GetPlayerGangLevel(playerid) == level)
	    {
	        SetPlayerGeneralSkin(playerid, skin);
	    }
	}
    
    return ormCacheSave(cache, string);
}

stock GetGangSpawn(gangid, &Float:x=Float:0, &Float:y=Float:0, &Float:z=Float:0, &Float:rz=Float:0, &interior=0)
{
	if(!IsGangExist(gangid))    return 0;
	new ORMCache:cache = ORMCache:itemGetInt(itemGetByID(Gang, gangid), "Cache");
	ormCacheGetFloat(cache, "SpawnX", x);
	ormCacheGetFloat(cache, "SpawnY", y);
	ormCacheGetFloat(cache, "SpawnZ", z);
	ormCacheGetFloat(cache, "SpawnRZ", rz);
	ormCacheGetInt(cache, "SpawnInterior", interior);
	return 1;
}

stock SetGangSpawn(gangid, Float:x, Float:y, Float:z, Float:rz, interior)
{
	if(!IsGangExist(gangid))    return 0;
	new ORMCache:cache = ORMCache:itemGetInt(itemGetByID(Gang, gangid), "Cache");
	ormCacheSetFloat(cache, "SpawnX", x);
	ormCacheSetFloat(cache, "SpawnY", y);
	ormCacheSetFloat(cache, "SpawnZ", z);
	ormCacheSetFloat(cache, "SpawnRZ", rz);
	ormCacheSetFloat(cache, "SpawnInterior", interior);
	ormCacheSave(cache, "SpawnX", "SpawnY", "SpawnZ", "SpawnRZ", "SpawnInterior");
	return 1;
}

stock GetGangType(gangid)
{
	if(!IsGangExist(gangid))    return 0;
	new ORMCache:cache = ORMCache:itemGetInt(itemGetByID(Gang, gangid), "Cache");
	new type;
	ormCacheGetInt(cache, "Type", type);
	return type;
}

stock SetGangType(gangid, type)
{
	if(!IsGangExist(gangid))    return 0;
	new ORMCache:cache = ORMCache:itemGetInt(itemGetByID(Gang, gangid), "Cache");
	ormCacheSetInt(cache, "Type", type);
	ormCacheSave(cache, "Type");
	return 1;
}

stock GetGangColor(gangid)
{
	if(!IsGangExist(gangid))    return 0;
	new ORMCache:cache = ORMCache:itemGetInt(itemGetByID(Gang, gangid), "Cache");
	new color;
	ormCacheGetInt(cache, "Color", color);
	return color;
}

stock SetGangColor(gangid, {_, RGBA}:color)
{
	if(!IsGangExist(gangid))    return 0;
	new ORMCache:cache = ORMCache:itemGetInt(itemGetByID(Gang, gangid), "Cache");
	ormCacheSetInt(cache, "Color", color);
	ormCacheSave(cache, "Color");
	
    new List:list = GetListPlayers();
	listForeach(playerid:list)
	{
	    if(GetPlayerGang(playerid) == gangid)
	    {
	        SetPlayerGeneralColor(playerid, (_:color<<8)|0xFF);
	    }
	}
	return 1;
}

stock GetGangName(gangid, name[], maxlenght = sizeof(name))
{
	if(!IsGangExist(gangid))    return 0;
	new ORMCache:cache = ORMCache:itemGetInt(itemGetByID(Gang, gangid), "Cache");
	ormCacheGet(cache, "Name", name, maxlenght);
	return 1;
}

stock SetGangName(gangid, name[])
{
	if(!IsGangExist(gangid))    return 0;
	new ORMCache:cache = ORMCache:itemGetInt(itemGetByID(Gang, gangid), "Cache");
	ormCacheSet(cache, "Name", name);
	ormCacheSave(cache, "Name");
	return 1;
}


