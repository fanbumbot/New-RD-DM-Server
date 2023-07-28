#if defined inc_pl_arepair
	#endinput
#endif
#define inc_pl_arepair

static autorepairtimer;

public OnScriptInit()
{
    itemInfoAdd(Player, "AutoRepair", 0);
    autorepairtimer = SetTimer("AutoRepairTimer", 200, 1);

#if defined pl_arepair_OnScriptInit
    pl_arepair_OnScriptInit();
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
#define OnScriptInit pl_arepair_OnScriptInit
#if defined pl_arepair_OnScriptInit
forward pl_arepair_OnScriptInit();
#endif

public OnScriptExit()
{
#if defined pl_arepair_OnScriptExit
    pl_arepair_OnScriptExit();
#endif
    KillTimer(autorepairtimer);
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
#define OnScriptExit pl_arepair_OnScriptExit
#if defined pl_arepair_OnScriptExit
forward pl_arepair_OnScriptExit();
#endif

stock IsPlayerAutoRepair(playerid)
{
	if(!IsPlayerConnected(playerid))    return 0;
	return itemGetInt(itemGetByID(Player, playerid), "AutoRepair");
}

stock PlayerAutoRepairEnable(playerid)
{
	if(!IsPlayerConnected(playerid))    return 0;
	itemSetInt(itemGetByID(Player, playerid), "AutoRepair", 1);
	
	new vehicleid = GetPlayerVehicleID(playerid);
	if(IsValidVehicle(playerid))
	{
	    SetVehicleHealth(vehicleid, 1000.0);
		RepairVehicle(vehicleid);
	}
	return 1;
}

stock PlayerAutoRepairDisable(playerid)
{
	if(!IsPlayerConnected(playerid) || !IsPlayerAutoRepair(playerid))   return 0;
	itemSetInt(itemGetByID(Player, playerid), "AutoRepair", 0);
	return 1;
}

forward AutoRepairTimer();
public AutoRepairTimer()
{
	new List:list = GetListPlayers();
	listForeach(playerid:list)
	{
		if(itemGetInt(itemGetByID(Player, playerid), "AutoRepair"))
		{
			new vehicleid = GetPlayerVehicleID(playerid);
			if(IsValidVehicle(vehicleid))
			{
			    SetVehicleHealth(vehicleid, 1000.0);
				RepairVehicle(vehicleid);
			}
		}
	}
	return 1;
}

