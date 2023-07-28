#if defined mdg_afk_inc
	#endinput
#endif
#define mdg_afk_inc

#if !defined TICKS_TO_AFK
	#define TICKS_TO_AFK 2000
#endif

public OnScriptInit()
{
	itemInfoAdd(Player, "AFKLastTick");
#if defined mdg_afk_OnScriptInit
    mdg_afk_OnScriptInit();
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
#define OnScriptInit mdg_afk_OnScriptInit
#if defined mdg_afk_OnScriptInit
forward mdg_afk_OnScriptInit();
#endif

public OnPlayerConnect(playerid)
{
	itemSetInt(itemGetByID(Player, playerid), "AFKLastTick", GetTickCount());
#if defined mdg_afk_OnPlayerConnect
    mdg_afk_OnPlayerConnect(playerid);
#endif
	return 1;
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect mdg_afk_OnPlayerConnect
#if defined mdg_afk_OnPlayerConnect
forward mdg_afk_OnPlayerConnect(playerid);
#endif

public OnPlayerUpdate(playerid)
{
#if defined mdg_afk_OnPlayerUpdate
    mdg_afk_OnPlayerUpdate(playerid);
#endif
	itemSetInt(itemGetByID(Player, playerid), "AFKLastTick", GetTickCount());
	return 1;
}
#if defined _ALS_OnPlayerUpdate
	#undef OnPlayerUpdate
#else
    #define _ALS_OnPlayerUpdate
#endif
#define OnPlayerUpdate mdg_afk_OnPlayerUpdate
#if defined mdg_afk_OnPlayerUpdate
forward mdg_afk_OnPlayerUpdate(playerid);
#endif

stock GetAFKTime(playerid)//in milliseconds
{
	return ((GetTickCount()-itemGetInt(itemGetByID(Player, playerid), "AFKLastTick"))-TICKS_TO_AFK);
}

stock IsPlayerAFK(playerid)
{
    return (GetAFKTime(playerid) > 0) ? 1 : 0;
}

stock UpdateAFKTick(playerid)
{
    itemSetInt(itemGetByID(Player, playerid), "AFKLastTick", GetTickCount());
    return 1;
}
