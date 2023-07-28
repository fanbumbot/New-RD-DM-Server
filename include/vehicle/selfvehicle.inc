#if defined inc_sveh
	#endinput
#endif
#define inc_sveh

public OnScriptInit()
{
	itemInfoAdd(Vehicle, "SelfPlayer", -1);
	itemInfoAdd(Player, "SelfVehicle", 0);

#if defined sveh_OnScriptInit
    sveh_OnScriptInit();
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
#define OnScriptInit sveh_OnScriptInit
#if defined sveh_OnScriptInit
forward sveh_OnScriptInit();
#endif

public OnPlayerDisconnect(playerid, reason)
{
#if defined sveh_OnPlayerDisconnect
    sveh_OnPlayerDisconnect(playerid, reason);
#endif
	new Item:player = itemGetByID(Player, playerid);
	new vehicleid = itemGetInt(player, "SelfVehicle");
	if(IsValidVehicle(vehicleid))
	{
	    DestroyVehicle(vehicleid);
	    new Item:vehicle = itemGetByID(Vehicle, vehicleid);
		itemSetInt(vehicle, "SelfPlayer", -1);
	}
	itemSetInt(player, "SelfVehicle", 0);
	return 1;
}
#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect sveh_OnPlayerDisconnect
#if defined sveh_OnPlayerDisconnect
forward sveh_OnPlayerDisconnect(playerid, reason);
#endif

stock IsVehicleSelf(vehicleid)
{
    if(!IsValidVehicle(vehicleid))  return 0;
    return itemGetInt(itemGetByID(Vehicle, vehicleid), "SelfPlayer") == -1 ? 0 : 1;
}

stock GetSelfVehiclePlayer(vehicleid)
{
    if(!IsValidVehicle(vehicleid))  return -1;
    new Item:vehicle = itemGetByID(Vehicle, vehicleid);
    new playerid = itemGetInt(vehicle, "SelfPlayer");
    if(!IsPlayerConnected(playerid))    return -1;
    return playerid;
}

stock GetPlayerSelfVehicle(playerid)
{
	if(!IsPlayerConnected(playerid))    return 0;
	new Item:player = itemGetByID(Player, playerid);
	new vehicleid = itemGetInt(player, "SelfVehicle");
	if(!IsValidVehicle(vehicleid))  return 0;
	return vehicleid;
}

stock CreateSelfVehicle(playerid, model, Float:x, Float:y, Float:z, Float:rz, Lobby:lobby, interior, color1, color2)
{
	if(!IsPlayerConnected(playerid))    return 0;
	new Item:player = itemGetByID(Player, playerid);
	new vehicleid = itemGetInt(player, "SelfVehicle");
	DestroyVehicle(vehicleid);
	vehicleid = CreateVehicle(model, x, y, z, rz, color1, color2, -1);
    if(!IsValidVehicle(vehicleid))  return 0;

    new Item:vehicle = itemGetByID(Vehicle, vehicleid);
    itemSetInt(player, "SelfVehicle", vehicleid);
    itemSetInt(vehicle, "SelfPlayer", playerid);
    LinkVehicleToInterior(vehicleid, interior);
    if(lobbyIsExist(lobby))
    {
    	VehicleEnterLobby(vehicleid, lobby);
	}
	return vehicleid;
}

stock DestroySelfVehicle(playerid)
{
    if(!IsPlayerConnected(playerid))    return 0;
    new Item:player = itemGetByID(Player, playerid);
    new vehicleid = itemGetInt(player, "SelfVehicle");
    new Item:vehicle = itemGetByID(Vehicle, vehicleid);
    if(!IsValidVehicle(vehicleid))  return 0;
    DestroyVehicle(vehicleid);
    itemSetInt(player, "SelfVehicle", 0);
    itemSetInt(vehicle, "SelfPlayer", -1);
    return 1;
}

stock FastCreateSelfVehicle(playerid, model, color1, color2, Float:offsetz = 0.0)
{
	if(!IsPlayerConnected(playerid))    return 0;
	new interior, Lobby:lobby;
	new Float:x, Float:y, Float:z, Float:rz;
	new vehicleid = GetPlayerVehicleID(playerid);
	interior = GetPlayerInterior(playerid);
    lobby = GetPlayerLobby(playerid);
	if(IsValidVehicle(vehicleid))
	{
		GetVehiclePos(vehicleid, x, y, z);
		GetVehicleZAngle(vehicleid, rz);
	}
	else
	{
		GetPlayerPos(playerid, x, y, z);
		GetPlayerFacingAngle(playerid, rz);
	}
	vehicleid = CreateSelfVehicle(playerid, model, x, y, z+offsetz, rz, lobby, interior, color1, color2);
	PutPlayerInVehicle(playerid, vehicleid, 0);
	return vehicleid;
}

stock sveh_DestroyVehicle(vehicleid)
{
	if(IsValidVehicle(vehicleid))
	{
		new playerid = itemGetInt(itemGetByID(Vehicle, vehicleid), "SelfPlayer");
		if(IsPlayerConnected(playerid))
		{
		    itemSetInt(itemGetByID(Player, playerid), "SelfVehicle", 0);
		}
	}
	return DestroyVehicle(vehicleid);
}
#if defined _ALS_DestroyVehicle
	#undef DestroyVehicle
#else
	#define _ALS_DestroyVehicle
#endif
#define DestroyVehicle sveh_DestroyVehicle

