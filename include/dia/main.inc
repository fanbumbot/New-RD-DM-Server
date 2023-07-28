#if defined inc_dia_main
	#endinput
#endif
#define inc_dia_main

stock ShowPlayerStats(playerid, targetid)
{
	if(!IsPlayerConnected(playerid))    return 0;
	itemSetInt(itemGetByID(Player, playerid), "doplistitem", targetid);
	ShowDialog(playerid, Dialog:GMenuStats);
	return 1;
}

#include "../include/dia/reg"
#include "../include/dia/login"
#include "../include/dia/logskin"

#include "../include/dia/gmenu"
#include "../include/dia/amenu"
#include "../include/dia/gangmenu"
#include "../include/dia/other"

public OnScriptInit()
{
	itemInfoAdd(Player, "doplistitem", 0);

#if defined dia_OnScriptInit
    dia_OnScriptInit();
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
#define OnScriptInit dia_OnScriptInit
#if defined dia_OnScriptInit
forward dia_OnScriptInit();
#endif

/*
#include "../include/dia/gmenu"
#include "../include/dia/amenu"
#include "../include/dia/gangmenu"
#include "../include/dia/busmenu"
#include "../include/dia/copchase"
#include "../include/dia/dynraces"
#include "../include/dia/other"
#include "../include/dia/radio"
*/
