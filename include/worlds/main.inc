#if defined inc_world_main
	#endinput
#endif
#define inc_world_main

stock SetPlayerGeneralSkin(playerid, skin)
{
	if(!IsPlayerConnected(playerid) || skin < 0 || skin > 311)  return 0;
	new Item:player = itemGetByID(Player, playerid);
	itemSetInt(player, "GeneralSkin", skin);
	if(itemGetInt(player, "GeneralUse"))
	{
		SetPlayerSkin(playerid, skin);
	}
	return 1;
}

stock SetPlayerGeneralColor(playerid, color)
{
	if(!IsPlayerConnected(playerid))  return 0;
	new Item:player = itemGetByID(Player, playerid);
	itemSetInt(player, "GeneralColor", color);
	if(itemGetInt(player, "GeneralUse"))
	{
		SetPlayerColor(playerid, color);
		SetPlayerMarkerForAll(playerid, color);
	}
	return 1;
}

stock UpdatePlayerGeneralWeapon(playerid)
{
    if(!IsPlayerConnected(playerid))    return 0;
    new Pointer:node = itemGetPointer(itemGetByID(Player, playerid), "GeneralWeapon");

    if(itemGetInt(itemGetByID(Player, playerid), "GeneralUse"))
    {
	    ResetPlayerWeapons(playerid);
	    for(new i = 0; i < 12; i++)
	    {
	    	GivePlayerWeapon(playerid, MEM_get_val(node, i*2), MEM_get_val(node, i*2+1));
	    }
	}
    return 1;
}

stock ResetPlayerGeneralWeapon(playerid)
{
	if(!IsPlayerConnected(playerid))    return 0;

	new Pointer:node = itemGetPointer(itemGetByID(Player, playerid), "GeneralWeapon");
    for(new i = 0; i < 24; i++)
    {
    	MEM_set_val(node, i, 0);
    }
	return 1;
}

stock ClearSlotGeneralWeapon(playerid, slot)
{
	if(!IsPlayerConnected(playerid) || slot < 0 || slot > 12)   return 0;
	new Pointer:node = itemGetPointer(itemGetByID(Player, playerid), "GeneralWeapon");
	MEM_set_val(node, slot*2, 0);
	MEM_set_val(node, slot*2+1, 0);
	return 1;
}

stock GivePlayerGeneralWeapon(playerid, weaponid, ammo)
{
	if(!IsPlayerConnected(playerid) || ammo < 0)    return 0;
	new slot = GetWeaponSlot(weaponid);
	if(slot == -1)  return 0;

	new Pointer:node = itemGetPointer(itemGetByID(Player, playerid), "GeneralWeapon");
	new weap = MEM_get_val(node, slot*2);
	if(weap != weaponid)
	{
	    MEM_set_val(node, slot*2+1, ammo);
	}
	else
	{
	    MEM_set_val(node, slot*2+1, MEM_get_val(node, slot*2+1)+ammo);
	}
	MEM_set_val(node, slot*2, weaponid);
	return 1;
}

stock SetPlayerGeneralWeapon(playerid, weaponid, ammo)
{
	if(!IsPlayerConnected(playerid) || ammo < 0)    return 0;
	new slot = GetWeaponSlot(weaponid);
	if(slot == -1)  return 0;
	
	new Pointer:node = itemGetPointer(itemGetByID(Player, playerid), "GeneralWeapon");
	MEM_set_val(node, slot*2, weaponid);
	MEM_set_val(node, slot*2+1, ammo);
	return 1;
}

stock GetPlayerGeneralWeapon(playerid, slot, &weaponid, &ammo)
{
    if(!IsPlayerConnected(playerid))    return 0;
    new Pointer:node = itemGetPointer(itemGetByID(Player, playerid), "GeneralWeapon");
    weaponid = MEM_get_val(node, slot*2);
    ammo = MEM_get_val(node, slot*2+1);
    return 1;
}

stock UpdatePlayerGeneral(playerid)
{
	if(!IsPlayerConnected(playerid))    return 0;
	new Item:player = itemGetByID(Player, playerid);
	new color = itemGetInt(player, "GeneralColor");
	SetPlayerColor(playerid, color);
	SetPlayerMarkerForAll(playerid, color);
	SetPlayerSkin(playerid, itemGetInt(player, "GeneralSkin"));
	UpdatePlayerGeneralWeapon(playerid);
	return 1;
}

stock GeneralEnable(playerid)
{
    if(!IsPlayerConnected(playerid))    return 0;
    itemSetInt(itemGetByID(Player, playerid), "GeneralUse", 1);
    UpdatePlayerGeneral(playerid);
	return 1;
}

stock GeneralDisable(playerid)
{
    if(!IsPlayerConnected(playerid))    return 0;
    itemSetInt(itemGetByID(Player, playerid), "GeneralUse", 0);
	return 1;
}

stock IsPlayerUseGeneral(playerid)
{
    if(!IsPlayerConnected(playerid))    return 0;
    return itemGetInt(itemGetByID(Player, playerid), "GeneralUse");
}

stock WorldAddAllGroup(World:world)
{
    lobbyGroupAdd(world, "Player");
    lobbyGroupAdd(world, "Vehicle");
    return 1;
}

public OnScriptInit()
{
	itemInfoAdd(Player, "GeneralColor", -1);
	itemInfoAdd(Player, "GeneralSkin", 0);
	itemInfoAdd(Player, "GeneralWeapon", 0);
	itemInfoAdd(Player, "GeneralUse", 0);
	itemInfoAdd(Player, "SpawnSpecFix", 0);

#if defined wmain_OnScriptInit
    wmain_OnScriptInit();
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
#define OnScriptInit wmain_OnScriptInit
#if defined wmain_OnScriptInit
forward wmain_OnScriptInit();
#endif

public OnPlayerConnect(playerid)
{
#if defined wmain_OnPlayerConnect
    wmain_OnPlayerConnect(playerid);
#endif
    new color;
    new skin;
	if(IsPlayerRegistered(playerid))
    {
	    new gangid = GetPlayerGang(playerid);
	    if(IsGangExist(gangid))
	    {
	        color = (GetGangColor(gangid)<<8)|0xFF;
			new rang = GetPlayerGangLevel(playerid);
			skin = GetGangSkin(gangid, rang);
			if(skin == -1)
			{
				skin = PlayerGetInt(playerid, "Skin");
			}
	    }
	    else
	    {
	        color = 0x404040FF + (random(0xBFBFC0) << 8);
	        skin = PlayerGetInt(playerid, "Skin");
		}
	}
	else
	{
	    color = 0x404040FF + (random(0xBFBFC0) << 8);
	    skin = 0;
	}
	SetPlayerGeneralColor(playerid, color);
	SetPlayerGeneralSkin(playerid, skin);
	
    itemSetPointer(itemGetByID(Player, playerid), "GeneralWeapon", MEM_new_zero(24), true);
	return 1;
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect wmain_OnPlayerConnect
#if defined wmain_OnPlayerConnect
forward wmain_OnPlayerConnect(playerid);
#endif

public OnPlayerEnterLobby(playerid, World:world, Lobby:lobby)
{
	new check = 0;
	if(IsPlayerUseGeneral(playerid))
	{
	    check = 1;
	}
#if defined wmain_OnPlayerEnterLobby
    wmain_OnPlayerEnterLobby(playerid, world, lobby);
#endif
	if(check == 1 && !IsPlayerUseGeneral(playerid))
	{
	    ResetPlayerGeneralWeapon(playerid);
	    new bufweap, bufammo;
	    for(new i = 0; i < 12; i++)
	    {
			GetPlayerWeaponData(playerid, i, bufweap, bufammo);
			SetPlayerGeneralWeapon(playerid, bufweap, bufammo);
		}
	}
	return 1;
}
#if defined _ALS_OnPlayerEnterLobby
	#undef OnPlayerEnterLobby
#else
    #define _ALS_OnPlayerEnterLobby
#endif
#define OnPlayerEnterLobby wmain_OnPlayerEnterLobby
#if defined wmain_OnPlayerEnterLobby
forward wmain_OnPlayerEnterLobby(playerid, World:world, Lobby:lobby);
#endif
/*
public OnPlayerSpawn(playerid)
{
	new Item:player = itemGetByID(Player, playerid);
	if(!itemGetInt(player, "SpawnSpecFix"))
	{
	    SetTimerEx("SpawnSpecFixTimer", 100, 0, "i", playerid);
	    itemSetInt(player, "SpawnSpecFix", 1);
	}
	else
	{
#if defined wmain_OnPlayerSpawn
    	wmain_OnPlayerSpawn(playerid);
#endif
	}
	return 1;
}
#if defined _ALS_OnPlayerSpawn
	#undef OnPlayerSpawn
#else
    #define _ALS_OnPlayerSpawn
#endif
#define OnPlayerSpawn wmain_OnPlayerSpawn
#if defined wmain_OnPlayerSpawn
forward wmain_OnPlayerSpawn(playerid);
#endif
*/
public OnPlayerDeath(playerid, killerid, reason)
{
	new vehicle = GetPlayerVehicleID(playerid);
	if(vehicle)
	{
        new Float:x, Float:y, Float:z;
        GetVehiclePos(vehicle, x, y, z),
        SetPlayerPos(playerid, x, y, z);
	}
#if defined wmain_OnPlayerDeath
	return wmain_OnPlayerDeath(playerid, killerid, reason);
#endif
}
#if defined _ALS_OnPlayerDeath
	#undef OnPlayerDeath
#else
    #define _ALS_OnPlayerDeath
#endif
#define OnPlayerDeath wmain_OnPlayerDeath
#if defined wmain_OnPlayerDeath
forward wmain_OnPlayerDeath(playerid, killerid, reason);
#endif

forward SpawnSpecFixTimer(playerid);
public SpawnSpecFixTimer(playerid)
{
	if(!IsPlayerConnected(playerid))    return 0;
    itemSetInt(itemGetByID(Player, playerid), "SpawnSpecFix", 1);
	return 1;
}

#include "../include/worlds/reglog"
#include "../include/worlds/hub"
#include "../include/worlds/dm"
