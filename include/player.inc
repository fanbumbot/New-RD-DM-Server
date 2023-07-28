#if defined inc_player
	#endinput
#endif
#define inc_player

#if !defined MIN_PASSWORD_LEN
	#define MIN_PASSWORD_LEN 6
#endif

#if !defined MAX_PASSWORD_LEN
	#define MAX_PASSWORD_LEN 32
#endif

forward OnPlayerEnterLobby(playerid, World:world, Lobby:lobby);
forward OnPlayerExitLobby(playerid, World:world, Lobby:lobby, reason);

new ORMTable:AccountTable;

static namebuffer[MAX_PLAYER_NAME+7];
static namebuffer2[MAX_PLAYER_NAME];
static datereg[64];
static ipbuffer[16];
static gpcibuffer[42];

static pltimer;

public OnScriptInit()
{
    AccountTable = ormTableCreate(maindb, "Players", true,
		typeString, MAX_PLAYER_NAME, "Name", "",
		typeString, MAX_PASSWORD_LEN, "Password", "",
		typeString, 64, "DateReg", "",
		typeString, 64, "LastConnect", "",
		typeInteger, "FirstIP", _:IP_NONE,
		typeInteger, "LastIP", _:IP_NONE,
		typeString, 42, "FirstGPCI", "",
		typeString, 42, "LastGPCI", "",
		typeInteger, "Lang", _:langGetDefault(),
		typeInteger, "Admin", 0,
		typeInteger, "VIP", 0,
		typeInteger, "PlayTime", 0,
		typeInteger, "Prison", 0,
		typeInteger, "PrisonSec", 0,
		typeInteger, "Mute", 0,
		typeInteger, "MuteSec", 0,
		typeInteger, "Skin", 0,
		typeInteger, "Money", 0,
		typeInteger, "Death", 0,
		typeInteger, "Deaths", 0,
		typeInteger, "Kills", 0,
		typeInteger, "AdminShad", 0,
		typeInteger, "AdminLive", 0,
		typeInteger, "GangID", -1,
		typeInteger, "GangLvl", 0,
	    //typeArray, 13, "Weap",
	    //typeArray, 13, "Ammo",
		typeInteger, "GangInvite", 1,
		typeInteger, "MenuButton", 0
	);
	
	itemInfoAdd(Player, "Cache");
	itemInfoAdd(Player, "Registered");
	itemInfoAdd(Player, "Login");
	itemInfoAdd(Player, "PlayTimeSecond", 0);
	
	pltimer = SetTimer("PlayerOneSecond", 1000, 1);

#if defined pl_OnScriptInit
    pl_OnScriptInit();
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
#define OnScriptInit pl_OnScriptInit
#if defined pl_OnScriptInit
forward pl_OnScriptInit();
#endif

public OnScriptExit()
{
#if defined pl_OnScriptExit
    pl_OnScriptExit();
#endif
	ormTableDestroy(AccountTable);
	
	KillTimer(pltimer);
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
#define OnScriptExit pl_OnScriptExit
#if defined pl_OnScriptExit
forward pl_OnScriptExit();
#endif

forward PlayerOneSecond();
public PlayerOneSecond()
{
	new money;
	new second;
	new List:list = GetListPlayers();
	new Item:player;
	listForeach(playerid:list)
    {
        player = itemGetByID(Player, playerid);
    
        ResetPlayerMoney(playerid);
		money = PlayerGetInt(playerid, "Money");
        GivePlayerMoney(playerid, money);
        SetPlayerScore(playerid, money);
        
        second = itemGetInt(player, "PlayTimeSecond")+1;
        itemSetInt(player, "PlayTimeSecond", second);
        if(second == 60)
        {
            itemSetInt(player, "PlayTimeSecond", 0);
            PlayerSetInt(playerid, "PlayTime", PlayerGetInt(playerid, "PlayTime")+1);
        }
    }
	return 1;
}

stock PlayerEnterLobby(playerid, Lobby:lobby)
{
	if(!IsPlayerConnected(playerid) || !lobbyIsExist(lobby))    return 0;
	new Item:player = itemGetByID(Player, playerid);
    lobbyEnterItem(lobby, "Player", Item:player);
    new vw = lobbyGetVirtualWorld(lobby);
    SetPlayerVirtualWorld(playerid, vw);
	return 1;
}

stock SetPlayerWorld(playerid, world[], index)
{
    if(!IsPlayerConnected(playerid) || !strlen(world) || index < 0)    return 0;
    new World:worlditem = worldGetByName(world);
    if(!worldIsExist(worlditem))    return 0;
    PlayerEnterLobby(playerid, lobbyGetByID(worlditem, index));
    return 1;
}

stock World:GetPlayerWorld(playerid)
{
    if(!IsPlayerConnected(playerid))    return World:0;
    new Item:player = itemGetByID(Player, playerid);
    new Lobby:lobby = Lobby:itemGetInt(player, "Lobby");
    if(!lobbyIsExist(lobby))    return World:0;
    new World:world = lobbyGetWorld(lobby);
    if(!worldIsExist(world))    return World:0;
    return world;
}

stock Lobby:GetPlayerLobby(playerid)
{
    if(!IsPlayerConnected(playerid))    return Lobby:0;
    new Item:player = itemGetByID(Player, playerid);
    new Lobby:lobby = Lobby:itemGetInt(player, "Lobby");
    if(!lobbyIsExist(lobby))    return Lobby:0;
    return lobby;
}

public OnPlayerRequestClass(playerid, classid)
{
    SetSpawnInfo(playerid, NO_TEAM, 0, 0.0, 0.0, 3.0, 0.0, 0, 0, 0, 0, 0, 0);
    SpawnPlayer(playerid);
#if defined pl_OnPlayerRequestClass
    return pl_OnPlayerRequestClass(playerid, classid);
#else
	return 1;
#endif
}
#if defined _ALS_OnPlayerRequestClass
	#undef OnPlayerRequestClass
#else
    #define _ALS_OnPlayerRequestClass
#endif
#define OnPlayerRequestClass pl_OnPlayerRequestClass
#if defined pl_OnPlayerRequestClass
forward pl_OnPlayerRequestClass(playerid, classid);
#endif

public OnPlayerConnect(playerid)
{
    new Item:player = itemGetByID(Player, playerid);
	//Проверка на регистрацию аккаунта
	new ORMCache:cache = ormCacheCreate(AccountTable);

	GetPlayerName(playerid, namebuffer2, sizeof(namebuffer2));
	format(namebuffer, sizeof(namebuffer), "Name = '%s'", namebuffer2);

	if(ormCacheBind(cache, namebuffer))
	{
	    ormCacheLoad(cache);
	    itemSetInt(player, "Registered", 1);
	}
	else
	{
	    itemSetInt(player, "Registered", 0);
	}
	itemSetInt(player, "Login", 0);
	itemSetInt(player, "Cache", _:cache);

	new World:world = worldGetByName("Reglog");
	new len = worldLobbySize(world);
	
	if(len <= playerid)
	{
	    lobbyCreateArray(World:world, playerid-len+1);
	}
	
 	new Lobby:lobby = lobbyGetByID(world, playerid);
 	SetTimerEx("PlayerConnect", 100, 0, "ii", playerid, _:lobby);
 	
 	SendMessageToAll("mNewPlayer", PlayerName(playerid), playerid);
	
#if defined pl_OnPlayerConnect
    pl_OnPlayerConnect(playerid);
#endif
	return 1;
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect pl_OnPlayerConnect
#if defined pl_OnPlayerConnect
forward pl_OnPlayerConnect(playerid);
#endif

public OnPlayerDisconnect(playerid, reason)
{
#if defined pl_OnPlayerDisconnect
    pl_OnPlayerDisconnect(playerid, reason);
#endif
	CallLocalFunction("OnPlayerExitLobby", "iii", playerid, _:GetPlayerWorld(playerid), _:GetPlayerLobby(playerid), reason+1);

	new ORMCache:cache = ORMCache:itemGetInt(itemGetByID(Player, playerid), "Cache");
	ormCacheSave(cache);
    ormCacheDestroy(cache);
    
    switch(reason)
    {
        case 0: SendMessageToAll("mCrashed", PlayerName(playerid), playerid);
        case 1: SendMessageToAll("mExit", PlayerName(playerid), playerid);
        case 2: SendMessageToAll("mKick", PlayerName(playerid), playerid);
    }
	return 1;
}
#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect pl_OnPlayerDisconnect
#if defined pl_OnPlayerDisconnect
forward pl_OnPlayerDisconnect(playerid, reason);
#endif

public OnItemEnterLobby(Item:item, World:world, Lobby:lobby)
{
	if(itemGetItemInfo(item) == Player)
	{
		CallLocalFunction("OnPlayerEnterLobby", "iii", itemGetID(item), _:world, _:lobby);
	}
#if defined wreglog_OnItemEnterLobby
    wreglog_OnItemEnterLobby(item, world, lobby);
#endif
	return 1;
}
#if defined _ALS_OnItemEnterLobby
	#undef OnItemEnterLobby
#else
    #define _ALS_OnItemEnterLobby
#endif
#define OnItemEnterLobby wreglog_OnItemEnterLobby
#if defined wreglog_OnItemEnterLobby
forward wreglog_OnItemEnterLobby(Item:item, World:world, Lobby:lobby);
#endif

public OnItemExitLobby(Item:item, World:world, Lobby:lobby)
{
	if(itemGetItemInfo(item) == Player)
	{
		CallLocalFunction("OnPlayerExitLobby", "iii", itemGetID(item), _:world, _:lobby, 0);
	}
#if defined wreglog_OnItemExitLobby
    wreglog_OnItemExitLobby(item, world, lobby);
#endif
	return 1;
}
#if defined _ALS_OnItemExitLobby
	#undef OnItemExitLobby
#else
    #define _ALS_OnItemExitLobby
#endif
#define OnItemExitLobby wreglog_OnItemExitLobby
#if defined wreglog_OnItemExitLobby
forward wreglog_OnItemExitLobby(Item:item, World:world, Lobby:lobby);
#endif

forward PlayerConnect(playerid, Lobby:lobby);
public PlayerConnect(playerid, Lobby:lobby)
{
    PlayerEnterLobby(playerid, lobby);
	return 1;
}

stock FindPlayer(name[])
{
	if(!strlen(name))   return -1;
	new List:list = GetListPlayers();
	listForeach(playerid:list)
	{
		GetPlayerName(playerid, namebuffer2, sizeof(namebuffer2));
		if(strlen(namebuffer2) && !strcmp(name, namebuffer2))  return playerid;
	}
	return -1;
}

stock GetPlayerServerMoney(playerid)
{
	return PlayerGetInt(playerid, "Money");
}

stock SetPlayerServerMoney(playerid, money)
{
	return PlayerSetInt(playerid, "Money", money);
}

stock GivePlayerServerMoney(playerid, money)
{
	return PlayerSetInt(playerid, "Money", PlayerGetInt(playerid, "Money")+money);
}

stock PlayerName(playerid)
{
	GetPlayerName(playerid, namebuffer2, sizeof(namebuffer2));
	return namebuffer2;
}

stock PasswordControl(password[])
{
	new len = strlen(password);
	new c;
	for(new i = 0; i < len; i++)
	{
	    c = password[i];
		if(!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')|| c == '_'))	return 0;
	}
	return 1;
}

stock IsPlayerRegistered(playerid)
{
	if(!IsPlayerConnected(playerid))    return 0;
	return itemGetInt(itemGetByID(Player, playerid), "Registered");
}

stock IsPlayerLogin(playerid)
{
	if(!IsPlayerConnected(playerid))    return 0;
	return itemGetInt(itemGetByID(Player, playerid), "Login");
}

stock IsAccountRegistered(name[])
{
    new reg = 0;

	format(namebuffer, sizeof(namebuffer), "Name = '%s'", name);
	new ORMResult:result = ormSelect(AccountTable, namebuffer, "id");
	if(ormResultNumRows(result) != 0)
	{
	    reg = 1;
	}
	ormResultClear(result);
	ormResultDelete(result);
	return reg;
}

stock ORMCache:AccountRegister(name[], password[], IP:ip, gpci[])
{
	if(IsAccountRegistered(name))	return ORMCache:0;
	if(strlen(name) >= MAX_PLAYER_NAME)	return ORMCache:0;
	if(strlen(password) >= MAX_PASSWORD_LEN)	return ORMCache:0;
	if(strlen(gpci) >= 42) return ORMCache:0;
	
    new y, m, d, h, mi;
	getdate(y, m, d);
	gettime(h, mi);
	format(datereg, sizeof(datereg), "%02i.%02i.%i / %02i:%02i", d, m, y, h, mi);

	return ormInsertMaster(AccountTable,
		"Name", name,
		"Password", password,
		"DateReg", datereg,
		"LastConnect", datereg,
		"FirstIP", _:ip,
		"LastIP", _:ip,
		"FirstGPCI", gpci,
		"LastGPCI", gpci
	);
}

stock PlayerRegister(playerid, password[])
{
    if(!IsPlayerConnected(playerid))    return 0;
    if(IsPlayerRegistered(playerid))    return 0;
    GetPlayerName(playerid, namebuffer2, sizeof(namebuffer2));
    GetPlayerIp(playerid, ipbuffer, sizeof(ipbuffer));
    new IP:ip;
    IPFromString(ip, ipbuffer);
	gpci(playerid, gpcibuffer, sizeof(gpcibuffer));
    new ORMCache:cache = AccountRegister(namebuffer2, password, ip, gpcibuffer);
    if(!ormCacheIsValid(cache)) return 0;
    
	new Item:player = itemGetByID(Player, playerid);
    itemSetInt(player, "Cache", _:cache);
    itemSetInt(player, "Registered", 1);
    
    ormCacheLoad(cache);
    return 1;
}

stock PlayerLogin(playerid)
{
	if(!IsPlayerConnected(playerid))    return 0;
	new Item:player = itemGetByID(Player, playerid);
	if(!itemGetInt(player, "Registered"))   return 0;
	itemSetInt(player, "Login", 1);
	return 1;
}

stock AccountGet(account[], name[], output[], maxlength = sizeof(output))
{
	format(namebuffer, sizeof(namebuffer), "Name = '%s'", account);
    new ORMResult:result = ormSelect(AccountTable, namebuffer, name);
    if(ormResultNumRows(result) == 0)
	{
	    ormResultClear(result);
	    ormResultDelete(result);
		return 0;
	}
	ormGetField(result, 0, output, maxlength);
    ormResultClear(result);
    ormResultDelete(result);
	return 1;
}

stock AccountGetInt(account[], name[])
{
	format(namebuffer, sizeof(namebuffer), "Name = '%s'", account);
    new ORMResult:result = ormSelect(AccountTable, namebuffer, name);
    if(ormResultNumRows(result) == 0)
 	{
	    ormResultClear(result);
	    ormResultDelete(result);
		return 0;
	}
    new temp = ormGetFieldInt(result, 0);
    ormResultClear(result);
    ormResultDelete(result);
	return temp;
}

stock Float:AccountGetFloat(account[], name[])
{
	format(namebuffer, sizeof(namebuffer), "Name = '%s'", account);
    new ORMResult:result = ormSelect(AccountTable, namebuffer, name);
    if(ormResultNumRows(result) == 0)
	{
	    ormResultClear(result);
	    ormResultDelete(result);
		return Float:0;
	}
    new Float:temp = ormGetFieldFloat(result, 0);
    ormResultClear(result);
    ormResultDelete(result);
	return temp;
}

stock AccountSet(account[], name[], input[])
{
	format(namebuffer, sizeof(namebuffer), "Name = '%s'", account);
    new ORMResult:result = ormSelect(AccountTable, namebuffer, name);
    if(ormResultNumRows(result) == 0)
	{
	    ormResultClear(result);
	    ormResultDelete(result);
		return 0;
	}
    ormResultClear(result);
    ormResultDelete(result);
	ormUpdate(AccountTable, namebuffer, true, name, input);
	return 1;
}

stock AccountSetInt(account[], name[], input)
{
	format(namebuffer, sizeof(namebuffer), "Name = '%s'", account);
    new ORMResult:result = ormSelect(AccountTable, namebuffer, name);
    if(ormResultNumRows(result) == 0)
	{
	    ormResultClear(result);
	    ormResultDelete(result);
		return 0;
	}
    ormResultClear(result);
    ormResultDelete(result);
	ormUpdate(AccountTable, namebuffer, true, name, input);
	return 1;
}

stock AccountSetFloat(account[], name[], Float:input)
{
	format(namebuffer, sizeof(namebuffer), "Name = '%s'", account);
    new ORMResult:result = ormSelect(AccountTable, namebuffer, name);
    if(ormResultNumRows(result) == 0)
	{
	    ormResultClear(result);
	    ormResultDelete(result);
		return 0;
	}
    ormResultClear(result);
    ormResultDelete(result);
	ormUpdate(AccountTable, namebuffer, true, name, input);
	return 1;
}

stock PlayerGet(playerid, name[], output[], maxlength = sizeof(output))
{
	if(!IsPlayerConnected(playerid))    return 0;
	
	new Item:player = itemGetByID(Player, playerid);
	new ORMCache:cache = ORMCache:itemGetInt(player, "Cache");
	return ormCacheGet(cache, name, output, maxlength);
}

stock PlayerGetInt(playerid, name[])
{
	if(!IsPlayerConnected(playerid))    return 0;

	new temp;
	new Item:player = itemGetByID(Player, playerid);
	new ORMCache:cache = ORMCache:itemGetInt(player, "Cache");
	if(!ormCacheGetInt(cache, name, temp))  return 0;
	return temp;
}

stock Float:PlayerGetFloat(playerid, name[])
{
	if(!IsPlayerConnected(playerid))    return Float:0;

	new Float:temp;
	new Item:player = itemGetByID(Player, playerid);
	new ORMCache:cache = ORMCache:itemGetInt(player, "Cache");
	if(!ormCacheGetFloat(cache, name, temp))    return Float:0;
	return temp;
}

stock PlayerSet(playerid, name[], input[])
{
	if(!IsPlayerConnected(playerid))    return 0;

	new Item:player = itemGetByID(Player, playerid);
	new ORMCache:cache = ORMCache:itemGetInt(player, "Cache");
	return ormCacheSet(cache, name, input);
}

stock PlayerSetInt(playerid, name[], input)
{
	if(!IsPlayerConnected(playerid))    return 0;

	new Item:player = itemGetByID(Player, playerid);
	new ORMCache:cache = ORMCache:itemGetInt(player, "Cache");
	return ormCacheSetInt(cache, name, input);
}

stock PlayerSetFloat(playerid, name[], Float:input)
{
	if(!IsPlayerConnected(playerid))    return 0;

	new Item:player = itemGetByID(Player, playerid);
	new ORMCache:cache = ORMCache:itemGetInt(player, "Cache");
	return ormCacheSetFloat(cache, name, input);
}



