#if defined inc_pl_nos
	#endinput
#endif
#define inc_pl_nos

static nostimer;

public OnScriptInit()
{
    itemInfoAdd(Player, "IsNoS", 0);

#if defined pl_nos_OnScriptInit
    pl_nos_OnScriptInit();
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
#define OnScriptInit pl_nos_OnScriptInit
#if defined pl_nos_OnScriptInit
forward pl_nos_OnScriptInit();
#endif

public OnScriptExit()
{
#if defined pl_nos_OnScriptExit
    pl_nos_OnScriptExit();
#endif
	KillTimer(nostimer);
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
#define OnScriptExit pl_nos_OnScriptExit
#if defined pl_nos_OnScriptExit
forward pl_nos_OnScriptExit();
#endif

stock IsPlayerNoS(playerid)
{
	if(!IsPlayerConnected(playerid))    return 0;
	return itemGetInt(itemGetByID(Player, playerid), "IsNoS");
}

stock PlayerNoSEnable(playerid)
{
	if(!IsPlayerConnected(playerid)) return 0;
	itemSetInt(itemGetByID(Player, playerid), "IsNoS", 1);
	return 1;
}

stock PlayerNoSDisable(playerid)
{
	if(!IsPlayerConnected(playerid)) return 0;
	new Item:player;
	if(itemGetInt(player, "IsNoS"))
	{
	    new vehicleid = GetPlayerVehicleID(playerid);
	    if(IsValidVehicle(vehicleid))
	    {
	    	RemoveVehicleComponent(vehicleid, 1010);
		}
	}
	itemSetInt(player, "IsNoS", 0);
	return 1;
}

stock IsNoSModel(modelid)
{
    switch(modelid)
    {
        case 446,432,448,452,424,453,454,461,462,463,468,471,430,472,449,473,481,484,493,495,509,510,521,538,522,523,532,537,570,581,586,590,569,595,604,611: return 0;
    }
    return 1;
}

#if !defined KEY_JUST_DOWN
	#define KEY_JUST_DOWN(%0) ((newkeys & %0) && !(oldkeys & %0))
#endif

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if(itemGetInt(itemGetByID(Player, playerid), "IsNoS"))
	{
		new vehicleid = GetPlayerVehicleID(playerid);
		new modelid = GetVehicleModel(vehicleid);
		if(IsValidVehicle(vehicleid) && IsNoSModel(modelid))
		{
            if(newkeys == 1 || newkeys == 9 || newkeys == 33 && oldkeys != 1 || oldkeys != 9 || oldkeys != 33)
            {
                AddVehicleComponent(vehicleid, 1010);
            }
		}
	}

#if defined pl_nos_OnPlayerKeyStateChange
    pl_nos_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
#endif
	return 1;
}
#if defined _ALS_OnPlayerKeyStateChange
	#undef OnPlayerKeyStateChange
#else
    #define _ALS_OnPlayerKeyStateChange
#endif
#define OnPlayerKeyStateChange pl_nos_OnPlayerKeyStateChange
#if defined pl_nos_OnPlayerKeyStateChange
forward pl_nos_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
#endif

