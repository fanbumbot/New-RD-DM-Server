#if defined mdg_pl_inc
	#endinput
#endif
#define mdg_pl_inc

new ItemInfo:Player;

static List:allplayers;

public OnScriptInit()
{
    Player = itemInfoCreate();
    itemInfoAdd(Player, "IP");
    allplayers = listCreate();
#if defined mdg_pl_OnScriptInit
    mdg_pl_OnScriptInit();
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
#define OnScriptInit mdg_pl_OnScriptInit
#if defined mdg_pl_OnScriptInit
forward mdg_pl_OnScriptInit();
#endif

public OnScriptExit()
{
#if defined mdg_pl_OnScriptExit
    mdg_pl_OnScriptExit();
#endif
    itemInfoDestroy(Player);
    listDestroy(allplayers);
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
#define OnScriptExit mdg_pl_OnScriptExit
#if defined mdg_pl_OnScriptExit
forward mdg_pl_OnScriptExit();
#endif

public OnIncomingConnection(playerid, ip_address[], port)
{
	new Item:player = itemCreate(Player, playerid);
	new IP:ip;
	IPFromString(ip, ip_address);
	itemSetInt(player, "IP", _:ip);

#if defined mdg_pl_OnIncomingConnection
    mdg_pl_OnIncomingConnection(playerid, ip_address, port);
#endif
	return 1;
}
#if defined _ALS_OnIncomingConnection
	#undef OnIncomingConnection
#else
    #define _ALS_OnIncomingConnection
#endif
#define OnIncomingConnection mdg_pl_OnIncomingConnection
#if defined mdg_pl_OnIncomingConnection
forward mdg_pl_OnIncomingConnection(playerid, ip_address[], port);
#endif

public OnPlayerConnect(playerid)
{
    listPush(allplayers, playerid);
#if defined mdg_pl_OnPlayerConnect
    mdg_pl_OnPlayerConnect(playerid);
#endif
	return 1;
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect mdg_pl_OnPlayerConnect
#if defined mdg_pl_OnPlayerConnect
forward mdg_pl_OnPlayerConnect(playerid);
#endif

public OnPlayerDisconnect(playerid, reason)
{
#if defined mdg_pl_OnPlayerDisconnect
    mdg_pl_OnPlayerDisconnect(playerid, reason);
#endif
	itemDestroyByID(Player, playerid);
	listDeleteValue(allplayers, playerid, false);
	return 1;
}
#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect mdg_pl_OnPlayerDisconnect
#if defined mdg_pl_OnPlayerDisconnect
forward mdg_pl_OnPlayerDisconnect(playerid, reason);
#endif

stock List:GetListPlayers()   return allplayers;



