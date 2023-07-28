#if defined inc_main
	#endinput
#endif
#define inc_main

#include <a_samp>
#include <streamer>
#include <Pawn.CMD>
#include <Pawn.RakNet>
#include <MS/ms>

stock SetPlayerMarkerForAll(playerid, color)
{
	for(new i = 0; i < MAX_PLAYERS; i++)
	{
		SetPlayerMarkerForPlayer(i, playerid, color);
	}
}

stock GetWeaponSlot(weaponid)
{
    switch(weaponid)
    {
        case 1: return 0;
        case 2..9: return 1;
        case 22..24: return 2;
        case 25..27: return 3;
        case 28, 29, 32: return 4;
        case 30, 31: return 5;
        case 33, 34: return 6;
        case 35..38: return 7;
        case 16..18, 39: return 8;
        case 41..43: return 9;
        case 10..15: return 10;
        case 44..46: return 11;
        case 40: return 12;
    }
    return -1;
}

#include "../include/database.inc"
#include "../include/lang.inc"
#include "../include/player.inc"
#include "../include/vehicle.inc"

#include "../include/vehicle/neon.inc"
#include "../include/vehicle/selfvehicle.inc"
#include "../include/vehicle/utils.inc"

#include "../include/player/autorepair.inc"
#include "../include/player/drift.inc"
#include "../include/player/invulnerability.inc"
#include "../include/player/NoS.inc"
#include "../include/player/spec.inc"
#include "../include/player/speedometer.inc"

#include "../include/gang.inc"

stock UniversalTeleport(playerid, Float:x, Float:y, Float:z, Float:rz = -1.0, interior = -1, Lobby:lobby = Lobby:MEM_NULLPTR, camreset = true)
{
	if(!IsPlayerConnected(playerid))    return 0;
	new vehicleid = GetPlayerVehicleID(playerid);
	new Lobby:previoslobby = GetPlayerLobby(playerid);
	if(GetPlayerVehicleSeat(playerid) == 0 && IsValidVehicle(vehicleid))
	{
	    new List:playerlist;
	    new List:templist;
        new Pointer:tempptr;
		if(lobbyIsExist(lobby) && lobbyIsExist(previoslobby))
		{
		    playerlist = lobbyGroupGet(previoslobby, "Player");
		    templist = listCreate();
		    
		    listForeach(plid:playerlist)
		    {
		        if(GetPlayerVehicleID(plid) != vehicleid) continue;
		        tempptr = MEM_new(2);
		        MEM_set_val(tempptr, 0, plid);
		        MEM_set_val(tempptr, 1, GetPlayerVehicleSeat(plid));
	         	listPush(templist, tempptr);
		    }
		    VehicleEnterLobby(vehicleid, lobby);
		}
		if(interior != -1)
		{
		    LinkVehicleToInterior(vehicleid, interior);
		    SetPlayerInterior(playerid, interior);
		}
	    SetVehiclePos(vehicleid, x, y, z);
	    if(rz != -1.0)
	    {
			SetVehicleZAngle(vehicleid, rz);
		}
		if(lobbyIsExist(lobby) && lobbyIsExist(previoslobby))
		{
		    new plid;
		    new seatid;
		    listForeachPtr(node:templist)
		    {
		        plid = MEM_get_val(node, 0);
		        seatid = MEM_get_val(node, 1);
		        PlayerEnterLobby(plid, lobby);
		        PutPlayerInVehicle(plid, vehicleid, seatid);
		        if(interior != -1 && GetPlayerInterior(plid) != interior)
		        {
		            SetPlayerInterior(plid, interior);
		        }
		    }
			listDestroyPtr(templist);
		}
	}
	else
	{
		if(interior != -1)
		{
		    SetPlayerInterior(playerid, interior);
		}
		if(lobbyIsExist(lobby))
		{
		    PlayerEnterLobby(playerid, lobby);
		}
 	    SetPlayerPos(playerid, x, y, z);
	    if(rz != -1.0)
	    {
			SetPlayerFacingAngle(playerid, rz);
		}
	}
	if(camreset)
	{
		SetCameraBehindPlayer(playerid);
	}
	return 1;
}

stock UniversalTeleportToPlayer(playerid, targetid, Float:offsetz, {bool, _}:turnface = true, changeworld = true, camreset = true)
{
    if(!IsPlayerConnected(playerid) || !IsPlayerConnected(targetid))    return 0;
	new Float:x, Float:y, Float:z;
	new Float:rz = -1.0;
	new vehicleid = GetPlayerVehicleID(targetid);
	new Lobby:lobby = Lobby:MEM_NULLPTR;
    if(changeworld)
    {
	    lobby = GetPlayerLobby(targetid);
    }
	if(IsValidVehicle(vehicleid))
	{
	    GetVehiclePos(vehicleid, x, y, z);
	    if(turnface)
	    {
	        GetVehicleZAngle(vehicleid, rz);
	    }
	}
	else
	{
	    GetPlayerPos(targetid, x, y, z);
	    if(turnface)
	    {
	        GetPlayerFacingAngle(targetid, rz);
	    }
	}
	return UniversalTeleport(playerid, x, y, z+offsetz, rz, GetPlayerInterior(targetid), lobby, camreset);
}

#include "../include/security.inc"

#include "../include/admin.inc"
#include "../include/mapping.inc"

#include "../include/worlds/main.inc"

#include "../include/chat.inc"
#include "../include/dia/main.inc"
#include "../include/cmd/main.inc"

public OnScriptInit()
{
#if defined main_OnScriptInit
    main_OnScriptInit();
#endif

    EnableStuntBonusForAll(0);
    SetWeather(1);
	SetWorldTime(12);
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
#define OnScriptInit main_OnScriptInit
#if defined main_OnScriptInit
forward main_OnScriptInit();
#endif

