#if defined inc_admin
	#endinput
#endif
#define inc_admin

static List:alladmins;
static List:allvips;
static admintemp[256];

public OnScriptInit()
{
    alladmins = listCreate();
    allvips = listCreate();
    itemInfoAdd(Player, "IsAdminSpec");
    itemInfoAdd(Player, "IsAdminInvulnerable");
    itemInfoAdd(Player, "SpecExitX");
    itemInfoAdd(Player, "SpecExitY");
    itemInfoAdd(Player, "SpecExitZ");
    itemInfoAdd(Player, "SpecExitRZ");
    itemInfoAdd(Player, "SpecExitInterior");
#if defined admin_OnScriptInit
    admin_OnScriptInit();
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
#define OnScriptInit admin_OnScriptInit
#if defined admin_OnScriptInit
forward admin_OnScriptInit();
#endif

public OnScriptExit()
{
#if defined admin_OnScriptExit
    admin_OnScriptExit();
#endif
	listDestroy(alladmins);
	listDestroy(allvips);
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
#define OnScriptExit admin_OnScriptExit
#if defined admin_OnScriptExit
forward admin_OnScriptExit();
#endif

public OnPlayerConnect(playerid)
{
	if(IsPlayerRegistered(playerid))
	{
	    if(PlayerGetInt(playerid, "Admin") > 0)
	    {
			listPush(alladmins, playerid);
	    }
	    if(PlayerGetInt(playerid, "VIP") > 0)
	    {
	        listPush(allvips, playerid);
	    }
	}
#if defined admin_OnPlayerConnect
    admin_OnPlayerConnect(playerid);
#endif
	return 1;
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect admin_OnPlayerConnect
#if defined admin_OnPlayerConnect
forward admin_OnPlayerConnect(playerid);
#endif

public OnPlayerDisconnect(playerid, reason)
{
#if defined admin_OnPlayerDisconnect
    admin_OnPlayerDisconnect(playerid, reason);
#endif
	if(IsPlayerRegistered(playerid))
	{
	    if(PlayerGetInt(playerid, "Admin") > 0)
	    {
	        listDeleteValue(alladmins, playerid, false);
		}
	    if(PlayerGetInt(playerid, "VIP") > 0)
	    {
	        listDeleteValue(allvips, playerid, false);
		}
	}
	return 1;
}
#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect admin_OnPlayerDisconnect
#if defined admin_OnPlayerDisconnect
forward admin_OnPlayerDisconnect(playerid, reason);
#endif

public OnPlayerClickPlayer(playerid, clickedplayerid, source)
{
	if(!GetPlayerAdmin(playerid) && !GetPlayerVIP(playerid))
	{
	    SendMessage(playerid, "wAMenuNoAccess");
#if defined admin_OnPlayerClickPlayer
    	admin_OnPlayerClickPlayer(playerid, clickedplayerid, source);
#endif
		return 1;
	}
	if(!IsPlayerConnected(clickedplayerid))
	{
	    SendMessage(playerid, "wAMenuNoPlayer");
#if defined admin_OnPlayerClickPlayer
    	admin_OnPlayerClickPlayer(playerid, clickedplayerid, source);
#endif
		return 1;
	}
	itemSetInt(itemGetByID(Player, playerid), "doplistitem", clickedplayerid);
	ShowDialog(playerid, Dialog:AdminMenu);
	
#if defined admin_OnPlayerClickPlayer
    admin_OnPlayerClickPlayer(playerid, clickedplayerid, source);
#endif
	return 1;
}
#if defined _ALS_OnPlayerClickPlayer
	#undef OnPlayerClickPlayer
#else
    #define _ALS_OnPlayerClickPlayer
#endif
#define OnPlayerClickPlayer admin_OnPlayerClickPlayer
#if defined admin_OnPlayerClickPlayer
forward admin_OnPlayerClickPlayer(playerid, clickedplayerid, source);
#endif

stock OnPlayerSpecDisconnect(playerid, targetid, reason)
{
	if(IsAdminSpec(playerid))
	{
	    AdminSpecDisable(playerid);
	    SendMessage(playerid, "wASpecTargetExit");
	    return 1;
	}

#if defined admin_OnPlayerSpecDisconnect
	return admin_OnPlayerSpecDisconnect(playerid, targetid, reason);
#else
	return 0;
#endif
}
#if defined _ALS_OnPlayerSpecDisconnect
	#undef OnPlayerSpecDisconnect
#else
    #define _ALS_OnPlayerSpecDisconnect
#endif
#define OnPlayerSpecDisconnect admin_OnPlayerSpecDisconnect
#if defined admin_OnPlayerSpecDisconnect
forward admin_OnPlayerSpecDisconnect(playerid, targetid, reason);
#endif

public OnPlayerSpawn(playerid)
{
	new temp;
	new Item:player = itemGetByID(Player, playerid);
	if(IsExitSpec(playerid) && itemGetInt(player, "IsAdminSpec"))
	{
	    temp = 1;
	    itemSetInt(player, "IsAdminSpec", 1);
	}

#if defined admin_OnPlayerSpawn
    admin_OnPlayerSpawn(playerid);
#endif

	if(temp == 1)
	{
	    AdminSpecLoadEnter(playerid);
	}
	return 1;
}
#if defined _ALS_OnPlayerSpawn
	#undef OnPlayerSpawn
#else
    #define _ALS_OnPlayerSpawn
#endif
#define OnPlayerSpawn admin_OnPlayerSpawn
#if defined admin_OnPlayerSpawn
forward admin_OnPlayerSpawn(playerid);
#endif

stock List:GetListAdmins()   return alladmins;
stock List:GetListVIPs()   return allvips;

stock GetPlayerAdmin(playerid)
{
    if(!IsPlayerRegistered(playerid))   return 0;
	return PlayerGetInt(playerid, "Admin");
}

stock GetPlayerVIP(playerid)
{
    if(!IsPlayerRegistered(playerid))   return 0;
	return PlayerGetInt(playerid, "VIP");
}

stock SetPlayerAdmin(playerid, adminlvl)
{
	if(!IsPlayerRegistered(playerid))   return 0;
	if(adminlvl < 0)    return 0;
	
	if(adminlvl != 0)
	{
	    listPush(alladmins, playerid);
	}
	else
	{
	    listDeleteValue(alladmins, playerid, false);
	}
	
	new ORMCache:cache = ORMCache:itemGetInt(itemGetByID(Player, playerid), "Cache");
	ormCacheSetInt(cache, "Admin", adminlvl);
	ormCacheSave(cache, "Admin");
	return 1;
}

stock SetPlayerVIP(playerid, viplvl)
{
	if(!IsPlayerRegistered(playerid))   return 0;
	if(viplvl < 0)    return 0;
	
	if(viplvl != 0)
	{
	    listPush(allvips, playerid);
	}
	else
	{
	    listDeleteValue(allvips, playerid, false);
	}

	new ORMCache:cache = ORMCache:itemGetInt(itemGetByID(Player, playerid), "Cache");
	ormCacheSetInt(cache, "VIP", viplvl);
	ormCacheSave(cache, "VIP");
	return 1;
}

stock AdminKickPlayer(adminid, targetid, reason[] = "")
{
	if(!IsPlayerLogin(adminid) || !IsPlayerConnected(targetid)) return 0;
	SendMessageToAll("mAdminKick", PlayerName(adminid), PlayerName(targetid), reason);
	printf("Админ %s кикнул игрока %s. Причина: %s", PlayerName(adminid), PlayerName(targetid), reason);
    return KickPlayer(targetid);
}

stock AdminBanAccount(adminid, name[], reason[] = "", day = -1)
{
    if(!IsPlayerLogin(adminid) || (day < 0 && day != -1))	return 0;
    new ret = BanAccount(name, PlayerName(adminid), reason, day*86400);
    if(ret)
    {
	    if(day != -1)
	    {
	    	SendMessageToAll("mAdminBan", PlayerName(adminid), name, reason);
	    	printf("Админ %s забанил навсегда аккаунт %s. Причина: %s", PlayerName(adminid), name, reason);
		}
		else
		{
		    SendMessageToAll("mAdminBanTm", PlayerName(adminid), name, day, reason);
		    printf("Админ %s забанил аккаунт %s на %i дня (дней/день). Причина: %s", PlayerName(adminid), name, day, reason);
		}
	}
	return ret;
}

stock AdminBanPlayer(adminid, targetid, reason[] = "", day = -1)
{
    if(!IsPlayerLogin(adminid) || !IsPlayerConnected(targetid) || (day < 0 && day != -1)) return 0;
    new ret = BanPlayer(targetid, PlayerName(adminid), reason, day);
    if(ret)
    {
	    if(day != -1)
		{
			day *= 86400;
			SendMessageToAll("mAdminBanPlayer", PlayerName(adminid), PlayerName(targetid), reason);
			printf("Админ %s забанил навсегда игрока %s. Причина: %s", PlayerName(adminid), PlayerName(targetid), reason);
		}
		else
		{
		    SendMessageToAll("mAdminBanPlayerTm", PlayerName(adminid), PlayerName(targetid), day, reason);
		    printf("Админ %s забанил игрока %s на %i дня (дней/день). Причина: %s", PlayerName(adminid), PlayerName(targetid), day, reason);
		}
	}
	return ret;
}

stock AdminBanIP(adminid, IP:ip, day = -1)
{
    if(!IsPlayerLogin(adminid)) return 0;
    IPToString(admintemp, ip);
    new ret = BanIP(ip, PlayerName(adminid), "", day);
    if(ret)
    {
	    if(day != -1)
		{
			day *= 86400;
			printf("Админ %s забанил навсегда IP (%s)", PlayerName(adminid), admintemp);
		}
		else
		{
		    printf("Админ %s забанил IP (%s) на %i дня (дней/день)", PlayerName(adminid), admintemp, day);
		}
	}
	return ret;
}

stock AdminUnBanAccount(adminid, name[])
{
    if(!IsPlayerLogin(adminid))	return 0;
	new ret = UnBanAccount(name);
	if(ret)
	{
    	SendMessageToAll("mAdminUnBan", PlayerName(adminid), name);
    	printf("Админ %s разбанил аккаунт %s", PlayerName(adminid), name);
	}
	return ret;
}

stock AdminUnBanIP(adminid, IP:ip)
{
    if(!IsPlayerLogin(adminid))	return 0;
    new ret = UnBanIP(ip);
    if(ret)
    {
	    IPToString(admintemp, ip);
	    printf("Админ %s разбанил IP (%s)", PlayerName(adminid), admintemp);
	}
    return ret;
}

stock AdminMuteAccount(adminid, name[], time, reason[] = "")
{
    if(!IsPlayerLogin(adminid))	return 0;
    new ret = MuteAccount(name, time);
    if(ret)
    {
	    if(time != 0)
	    {
	    	SendMessageToAll("mAdminMute", PlayerName(adminid), name, time, reason);
	    	printf("Админ %s заткнул аккаунт %s на %i секунд. Причина: %s", PlayerName(adminid), name, time, reason);
		}
		else
		{
		    SendMessageToAll("mAdminUnMute", PlayerName(adminid), name);
		    printf("Админ %s разоткнул аккаунт %s", PlayerName(adminid), name);
		}
	}
	return ret;
}

stock AdminPrisonAccount(adminid, name[], time, reason[] = "")
{
    if(!IsPlayerLogin(adminid))	return 0;
    new ret = PrisonAccount(name, time);
    if(ret)
	{
	    if(time != 0)
	    {
	    	SendMessageToAll("mAdminPrison", PlayerName(adminid), name, time, reason);
	    	printf("Админ %s посадил в тюрьму аккаунт %s на %i секунд. Причина: %s", PlayerName(adminid), name, time, reason);
		}
		else
		{
		    SendMessageToAll("mAdminUnPrison", PlayerName(adminid), name);
		    printf("Админ %s освободил из тюрьмы аккаунт %s", PlayerName(adminid), name);
		}
	}
	return ret;
}

stock AdminMutePlayer(adminid, targetid, time, reason[] = "")
{
    if(!IsPlayerLogin(adminid) || !IsPlayerConnected(targetid))	return 0;
    new ret = MutePlayer(targetid, time);
    if(ret)
    {
	    if(time != 0)
	    {
	    	SendMessageToAll("mAdminMutePlayer", PlayerName(adminid), PlayerName(targetid), time, reason);
	    	printf("Админ %s заткнул игрока %s на %i секунд. Причина: %s", PlayerName(adminid), PlayerName(targetid), time, reason);
		}
		else
		{
		    SendMessageToAll("mAdminUnMutePlayer", PlayerName(adminid), PlayerName(targetid));
		    printf("Админ %s разоткнул игрока %s", PlayerName(adminid), PlayerName(targetid));
		}
	}
	return ret;
}

stock AdminPrisonPlayer(adminid, targetid, time, reason[] = "")
{
    if(!IsPlayerLogin(adminid) || !IsPlayerConnected(targetid))	return 0;
    new ret = PrisonPlayer(targetid, time);
    if(ret)
    {
	    if(time != 0)
	    {
	    	SendMessageToAll("mAdminPlayerPrison", PlayerName(adminid), PlayerName(targetid), time, reason);
	    	printf("Админ %s посадил в тюрьму игрока %s на %i секунд. Причина: %s", PlayerName(adminid), PlayerName(targetid), time, reason);
		}
		else
		{
		    SendMessageToAll("mAdminPlayerUnPrison", PlayerName(adminid), PlayerName(targetid));
		    printf("Админ %s освободил из тюрьмы игрока %s", PlayerName(adminid), PlayerName(targetid));
		}
	}
	return ret;
}

stock AdminGiveMoney(adminid, targetid, money)
{
    if(!IsPlayerLogin(adminid) || !IsPlayerConnected(targetid))	return 0;
    new ret = GivePlayerServerMoney(targetid, money);
    if(ret)
    {
    	printf("Админ %s дал игроку %s %i $", PlayerName(adminid), PlayerName(targetid), money);
	}
	return 1;
}

stock AdminSetMoney(adminid, targetid, money)
{
    if(!IsPlayerLogin(adminid) || !IsPlayerConnected(targetid))	return 0;
    new ret = SetPlayerServerMoney(targetid, money);
    if(ret)
    {
    	printf("Админ %s установил игроку %s %i $", PlayerName(adminid), PlayerName(targetid), money);
	}
	return 1;
}

stock IsAdminSpec(adminid)
{
	if(!IsPlayerConnected(adminid) || !itemGetInt(itemGetByID(Player, adminid), "IsAdminSpec")) return 0;
	return 1;
}

stock AdminSpecEnable(adminid, targetid)
{
    if(!IsPlayerConnected(adminid) || !IsPlayerConnected(targetid)) return 0;
    new Item:player = itemGetByID(Player, adminid);
	if(!IsPlayerSpec(adminid))
	{
	    new Float:x, Float:y, Float:z, Float:rz;
		new vehicleid = GetPlayerVehicleID(adminid);
		if(IsValidVehicle(vehicleid))
		{
		    GetVehiclePos(vehicleid, x, y, z);
		    GetVehicleZAngle(vehicleid, rz);
		}
		else
		{
		    GetPlayerPos(adminid, x, y, z);
		    GetPlayerFacingAngle(adminid, rz);
		}
		itemSetFloat(player, "SpecExitX", x);
		itemSetFloat(player, "SpecExitY", y);
		itemSetFloat(player, "SpecExitZ", z);
		itemSetFloat(player, "SpecExitRZ", rz);
	    itemSetInt(player, "SpecExitInterior", GetPlayerInterior(adminid));
	}
    new result = SpectatePlayerEnable(adminid, targetid);
    itemSetInt(player, "IsAdminSpec", result);
    return result;
}

stock AdminSpecDisable(adminid)
{
    if(!IsPlayerConnected(adminid) || !IsAdminSpec(adminid)) return 0;
    new result;
    if(itemSetInt(itemGetByID(Player, adminid), "IsAdminSpec", result))
    {
	    result = SpectatePlayerDisable(adminid);
    }
    else
    {
        result = 1;
    }
    return result;
}

stock AdminSpecLoadEnter(adminid)
{
    if(!IsPlayerConnected(adminid)) return 0;
    
    new Item:player = itemGetByID(Player, adminid);
    
    SetPlayerPos(adminid, itemGetFloat(player, "SpecExitX"), itemGetFloat(player, "SpecExitY"), itemGetFloat(player, "SpecExitZ"));
    SetPlayerFacingAngle(adminid, itemGetFloat(player, "SpecExitRZ"));
    SetPlayerInterior(adminid, itemGetInt(player, "SpecExitInterior"));
    return 1;
}

stock IsAdminInvulnerable(adminid)
{
	if(!IsPlayerConnected(adminid)) return 0;
	return itemGetInt(itemGetByID(Player, adminid), "IsAdminInvulnerable");
}

stock AdminInvulnerableEnable(adminid)
{
	if(!IsPlayerConnected(adminid)) return 0;
	itemSetInt(itemGetByID(Player, adminid), "IsAdminInvulnerable", 1);
	PlayerInvulnerableEnable(playerid);
	return 1;
}

stock AdminInvulnerableDisable(adminid)
{
    if(!IsPlayerConnected(adminid) || !IsAdminInvulnerable(adminid)) return 0;
    result = InvulnerablePlayerDisable(adminid);
    if(result)
	{
		itemSetInt(itemGetByID(Player, adminid), "IsAdminInvulnerable", 0);
	}
	return result;
}




