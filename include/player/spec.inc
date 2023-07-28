#if defined inc_spec
	#endinput
#endif
#define inc_spec

forward OnPlayerSpecDisconnect(playerid, targetid, reason);

static spectimer;

public OnScriptInit()
{
    itemInfoAdd(Player, "SpecIsDeath", 0);
    itemInfoAdd(Player, "SpecExitVW");
    itemInfoAdd(Player, "SpecIsExit", 0);
    itemInfoAdd(Player, "SpecTarget", -1);
	itemInfoAdd(Player, "SpedTargeted", -1);
	itemInfoAdd(Player, "SpecState", 0);

	spectimer = SetTimer("SpecTimer", 1000, 1);

#if defined spec_OnScriptInit
    spec_OnScriptInit();
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
#define OnScriptInit spec_OnScriptInit
#if defined spec_OnScriptInit
forward spec_OnScriptInit();
#endif

public OnScriptExit()
{
#if defined spec_OnScriptExit
    spec_OnScriptExit();
#endif
	KillTimer(spectimer);
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
#define OnScriptExit spec_OnScriptExit
#if defined spec_OnScriptExit
forward spec_OnScriptExit();
#endif

stock IsPlayerSpec(playerid)
{
	if(!IsPlayerConnected(playerid) || !IsPlayerConnected(itemGetInt(itemGetByID(Player, playerid), "SpecTarget"))) return 0;
	return 1;
}

stock GetPlayerSpec(playerid)
{
    if(!IsPlayerConnected(playerid))    return 0;
    return itemGetInt(itemGetByID(Player, playerid), "SpecTarget");
}

stock IsExitSpec(playerid)
{
    if(!IsPlayerConnected(playerid))    return 0;
    return itemGetInt(itemGetByID(Player, playerid), "SpecIsExit");
}

stock SpectatePlayerEnable(playerid, targetid)
{
    if(!IsPlayerConnected(playerid) || !IsPlayerConnected(targetid) || playerid == targetid)    return 0;
	new Item:player = itemGetByID(Player, playerid);
	if(!IsPlayerSpec(playerid))
    {
		itemSetInt(player, "SpecExitVW", GetPlayerVirtualWorld(playerid));
    	TogglePlayerSpectating(playerid, 1);
	}
	
	if(GetPlayerVirtualWorld(playerid) != GetPlayerVirtualWorld(targetid))
	{
	    SetPlayerVirtualWorld(playerid, GetPlayerVirtualWorld(targetid));
	}
	if(GetPlayerInterior(playerid) != GetPlayerInterior(targetid))
	{
	    SetPlayerInterior(playerid, GetPlayerInterior(targetid));
	}
	
	new vehicleid = GetPlayerVehicleID(targetid);
	new mode;
	if(IsValidVehicle(vehicleid))
	{
	    mode = 1;
	    PlayerSpectateVehicle(playerid, vehicleid);
	}
	else
	{
	    mode = 0;
	    PlayerSpectatePlayer(playerid, targetid);
	}
	itemSetInt(player, "SpecState", mode);
	itemSetInt(player, "SpecTarget", targetid);
	itemSetInt(itemGetByID(Player, targetid), "SpecTargeted", playerid);
    return 1;
}

stock SpectatePlayerDisable(playerid)
{
    if(!IsPlayerConnected(playerid) || !IsPlayerSpec(playerid)) return 0;

	new Item:player = itemGetByID(Player, playerid);
	new targetid = itemGetInt(player, "SpecTarget");
	itemSetInt(player, "SpecTarget", -1);
	itemSetInt(itemGetByID(Player, targetid), "SpecTargeted", -1);
    
    SetPlayerVirtualWorld(playerid, itemGetInt(player, "SpecExitVW"));
    TogglePlayerSpectating(playerid, 0);
    return 1;
}

forward SpecTimer();
public SpecTimer()
{
	static List:list;
	static Item:player;
	static targetid;
	static statemode;
	static vehicleid;
	
    list = GetListPlayers();
	listForeach(playerid:list)
	{
		player = itemGetByID(Player, playerid);
	    targetid = itemGetInt(player, "SpecTarget");
		if(!IsPlayerConnected(targetid))    continue;
		
		statemode = itemGetInt(player, "SpecState");
		vehicleid = GetPlayerVehicleID(targetid);
		
		if(IsValidVehicle(vehicleid) && !statemode)
		{
		    PlayerSpectateVehicle(playerid, vehicleid);
      		statemode = 1;
		}
		if(!IsValidVehicle(vehicleid) && statemode)
		{
		    PlayerSpectatePlayer(playerid, targetid);
		    statemode = 0;
		}
		if(GetPlayerVirtualWorld(playerid) != GetPlayerVirtualWorld(targetid))
		{
		    SetPlayerVirtualWorld(playerid, GetPlayerVirtualWorld(targetid));
			if(statemode == 0)
			{
			    PlayerSpectatePlayer(playerid, targetid);
			}
			else
			{
			    PlayerSpectateVehicle(playerid, vehicleid);
			}
		}
		if(GetPlayerInterior(playerid) != GetPlayerInterior(targetid))
		{
		    SetPlayerInterior(playerid, GetPlayerInterior(targetid));
			if(statemode == 0)
			{
			    PlayerSpectatePlayer(playerid, targetid);
			}
			else
			{
			    PlayerSpectateVehicle(playerid, vehicleid);
			}
		}
		itemSetInt(player, "SpecState", statemode);
	}
	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
	new Item:target = itemGetByID(Player, playerid);
	new targetedid = itemGetInt(target, "SpecTargeted");
	if(IsPlayerConnected(targetedid))
	{
	    new result = CallLocalFunction("OnPlayerSpecDisconnect", "iii", targetedid, playerid, reason);
		if(result)
		{
            SpectatePlayerDisable(targetedid);
		}
	}
#if defined spec_OnPlayerDisconnect
    spec_OnPlayerDisconnect(playerid, reason);
#endif
	return 1;
}
#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect spec_OnPlayerDisconnect
#if defined spec_OnPlayerDisconnect
forward spec_OnPlayerDisconnect(playerid, reason);
#endif

public OnPlayerDeath(playerid, killerid, reason)
{
#if defined spec_OnPlayerDeath
    spec_OnPlayerDeath(playerid, killerid, reason);
#endif
	itemSetInt(itemGetByID(Player, playerid), "SpecIsDeath", 1);
	return 1;
}
#if defined _ALS_OnPlayerDeath
	#undef OnPlayerDeath
#else
    #define _ALS_OnPlayerDeath
#endif
#define OnPlayerDeath spec_OnPlayerDeath
#if defined spec_OnPlayerDeath
forward spec_OnPlayerDeath(playerid, killerid, reason);
#endif

public OnPlayerSpawn(playerid)
{
	new Item:player = itemGetByID(Player, playerid);
	new targetedid;

	if(itemGetInt(player, "SpecIsDeath"))
	{
	    itemSetInt(player, "SpecIsDeath", 0);
	    targetedid = itemGetInt(player, "SpecTargeted");
		if(IsPlayerConnected(targetedid))
		{
		    PlayerSpectatePlayer(targetedid, playerid);
		    itemSetInt(player, "SpecState", 0);
		}
	}
#if defined spec_OnPlayerSpawn
    spec_OnPlayerSpawn(playerid);
#endif
	if(itemGetInt(player, "SpecIsExit"))
	{
	    itemSetInt(player, "SpecIsExit", 0);
	}
	return 1;
}
#if defined _ALS_OnPlayerSpawn
	#undef OnPlayerSpawn
#else
    #define _ALS_OnPlayerSpawn
#endif
#define OnPlayerSpawn spec_OnPlayerSpawn
#if defined spec_OnPlayerSpawn
forward spec_OnPlayerSpawn(playerid);
#endif

stock spec_PutPlayerInVehicle(playerid, vehicleid, seatid)
{
	new ret = PutPlayerInVehicle(playerid, vehicleid, seatid);
	if(ret)
	{
	    new targetedid = itemGetInt(itemGetByID(Player, playerid), "SpecTargeted");
	    if(IsPlayerConnected(targetedid))
	    {
	        PlayerSpectateVehicle(targetedid, vehicleid);
	        itemSetInt(itemGetByID(Player, targetedid), "SpecState", 1);
	    }
	}
	return ret;
}
#if defined _ALS_PutPlayerInVehicle
	#undef PutPlayerInVehicle
#else
	#define _ALS_PutPlayerInVehicle
#endif
#define PutPlayerInVehicle spec_PutPlayerInVehicle










