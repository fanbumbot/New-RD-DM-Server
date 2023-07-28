#if defined inc_cmd_admin
	#endinput
#endif
#define inc_cmd_admin

cmdResponse(iadminset)
{
	if(!IsPlayerAdmin(playerid))
	{
	    SendMessage(playerid, "wANeedRCON");
		return 1;
	}
	if(GetPlayerAdmin(playerid) >= 4)
	{
	    SendMessage(playerid, "wAAlready4LVL");
		return 1;
	}
    SetPlayerAdmin(playerid, 4);
	SendMessage(playerid, "mAGet4LVL");
	return 1;
}
cmdResponse(ahelp)
{
	if(GetPlayerAdmin(playerid) < 1)
	{
	    SendMessage(playerid, "wANoAccessCmd");
	    return 1;
	}
	if(GetPlayerAdmin(playerid) >= 1)
	{
	    SendMessage(playerid, "mAHelp1");
	    SendMessage(playerid, "mAHelp11");
	}
	if(GetPlayerAdmin(playerid) >= 2)
	{
	    SendMessage(playerid, "mAHelp2");
	    SendMessage(playerid, "mAHelp21");
	}
	if(GetPlayerAdmin(playerid) >= 3)
	{
	    SendMessage(playerid, "mAHelp3");
	    SendMessage(playerid, "mAHelp31");
	}
	if(GetPlayerAdmin(playerid) >= 4)
	{
	    SendMessage(playerid, "mAHelp4");
	    SendMessage(playerid, "mAHelp41");
	}
	if(IsPlayerAdmin(playerid))
	{
	    SendMessage(playerid, "mAHelpRCON");
		return 1;
	}
	return 1;
}

cmdResponse(a)
{
	if(GetPlayerAdmin(playerid) < 1)
	{
	    SendMessage(playerid, "wANoAccessCmd");
	    return 1;
	}
	new count = SendAdminMessage("mAdminChat", PlayerName(playerid), playerid,
	GetPlayerAdmin(playerid), params);
	if(count == 0)
	{
	    SendMessage(playerid, "wANoAdmins");
	    return 1;
	}
	printf("<AC> Админ %s [%d] (%d LVL): %s", PlayerName(playerid), playerid, GetPlayerAdmin(playerid), params);
	return 1;
}

cmdResponse(makeadmin)
{
	if(GetPlayerAdmin(playerid) < 4)
	{
	    SendMessage(playerid, "wANoAccessCmd");
	    return 1;
	}
	cmdInit(2);
	if(cmdGetNum() != 2)
	{
		SendMessage(playerid, "mMakeAdminInfo");
	    return 1;
	}
	new para1 = cmdGetInt(0);
	if(!IsPlayerConnected(para1))
	{
	    SendMessage(playerid, "wANoAdminOnline");
	    return 1;
	}
	if(!IsPlayerLogin(para1))
	{
	    SendMessage(playerid, "wANoAdminLogin");
	    return 1;
	}
	new para2 = cmdGetInt(1);
	if(para2 < 0 || para2 > 4)
	{
		SendMessage(playerid, "wANoAdminLevel");
	    return 1;
	}
	if(para2 == 4)
	{
		SendMessage(playerid, "wANoMakeAdmin");
	    return 1;
	}
	SetPlayerAdmin(para1, para2);
	if(para2 == 0)
	{
		SendMessage(para1, "mNoAdmin", PlayerName(playerid), para2);//Допиши текст для игрока
		SendMessage(playerid, "mANoAdmin", PlayerName(para1), para2);//Допиши текст для админа

		printf("%s снял админку с игрока %s", PlayerName(playerid), PlayerName(para1));
	}
	else
	{
		SendMessage(para1, "mAdmin", PlayerName(playerid), para2);
		SendMessage(playerid, "mAAdmin", PlayerName(para1), para2);//Допиши текст для админа

		printf("%s выдал админку %i уровня игроку %s", PlayerName(playerid), para2, PlayerName(para1));
	}
	return 1;
}

cmdResponse(makevip)
{
	if(GetPlayerAdmin(playerid) < 4)
	{
	    SendMessage(playerid, "wANoAccessCmd");
	    return 1;
	}
	cmdInit(2);
	if(cmdGetNum() != 2)
	{
		SendMessage(playerid, "mMakeVIPInfo");
	    return 1;
	}
	new para1 = cmdGetInt(0);
	if(!IsPlayerConnected(para1))
	{
	    SendMessage(playerid, "wANoVIPOnline");
	    return 1;
	}
	if(!IsPlayerLogin(para1))
	{
	    SendMessage(playerid, "wANoVIPLogin");
	    return 1;
	}
	new para2 = cmdGetInt(1);
	if(para2 < 0 || para2 > 2)
	{
		SendMessage(playerid, "wANoVIPLevel");
	    return 1;
	}
	SetPlayerVIP(para1, para2);
	if(para2 == 0)
	{
		SendMessage(para1, "mNoVIP", PlayerName(playerid));
		SendMessage(playerid, "mANoVIP", PlayerName(para1));
	}
	else
	{
		SendMessage(para1, "mVIP", PlayerName(playerid), para2);
		SendMessage(playerid, "mAVIP", PlayerName(playerid), PlayerName(para1), para2);
	}
	if(para2 >= 1)
	{
		SendMessage(para1, "mVIP1");
	}
	if(para2 >= 2)
	{
		SendMessage(para1, "mVIP2");
	}
	if(para2 == 0)
	{
	    printf("%s снял VIP с игрока %s", PlayerName(playerid), PlayerName(para1));
	}
	else
	{
		printf("%s выдал VIP %i уровня игроку %s", PlayerName(playerid), para2, PlayerName(para1));
	}
	return 1;
}

cmdResponse(cord)
{
	if(GetPlayerAdmin(playerid) < 1)
	{
	    SendMessage(playerid, "wANoAccessCmd");
	    return 1;
	}
	new vehicleid = GetPlayerVehicleID(playerid);
	new Float:x, Float:y, Float:z, Float:rz;
	if(!IsValidVehicle(vehicleid))
	{
	    GetPlayerPos(playerid, x, y, z);
		GetPlayerFacingAngle(playerid, rz);
	}
	else
	{
	    GetVehiclePos(vehicleid, x, y, z);
	    GetVehicleZAngle(vehicleid, rz);
	}
	new interior = GetPlayerInterior(playerid);
	SendMessage(playerid, "X: %0.3f | Y: %0.3f | Z: %0.3f | RZ: %0.3f | Interior: %i",
	x, y, z, rz, interior);
	return 1;
}

cmdResponse(tpset)
{
	if(GetPlayerAdmin(playerid) < 2)
	{
	    SendMessage(playerid, "wANoAccessCmd");
	    return 1;
	}
	cmdInit(3);
	if(cmdGetNum() != 3)
	{
		SendMessage(playerid, "mATPSetInfo");
	    return 1;
	}
	SetPlayerPos(playerid, cmdGetFloat(0), cmdGetFloat(1), cmdGetFloat(2));
	return 1;
}

cmdResponse(playtp)
{
	if(GetPlayerAdmin(playerid) < 2)
	{
	    SendMessage(playerid, "wANoAccessCmd");
	    return 1;
	}
	cmdInit(2);
	if(cmdGetNum() != 2)
	{
		SendMessage(playerid, "mAPlayTPInfo");
	    return 1;
	}
	new para1 = cmdGetInt(0);
	if(!IsPlayerConnected(para1))
	{
	    SendMessage(playerid, "wAPlayTPFirstOffline");
	    return 1;
	}
	if(!IsPlayerLogin(para1))
	{
	    SendMessage(playerid, "wAPlayTPFirstLogin");
	    return 1;
	}
	new para2 = cmdGetInt(1);
	if(!IsPlayerConnected(para2))
	{
	    SendMessage(playerid, "wAPlayTPSecondOffline");
	    return 1;
	}
	if(!IsPlayerLogin(para2))
	{
	    SendMessage(playerid, "wAPlayTPSecondLogin");
	    return 1;
	}
	if(para1 == para2)
	{
	    SendMessage(playerid, "wAPlayTPSchizo");
	    return 1;
	}
	UniversalTeleportToPlayer(para1, para2, 1.0);
	return 1;
}

cmdResponse(money)
{
	if(GetPlayerAdmin(playerid) < 1)
	{
	    SendMessage(playerid, "wANoAccessCmd");
	    return 1;
	}
	cmdInit(2);
	if(cmdGetNum() != 2)
	{
		SendMessage(playerid, "mAUseMoney");
	    return 1;
	}
	new para1 = cmdGetInt(0);
	if(!IsPlayerConnected(para1))
	{
	    SendMessage(playerid, "wAMenuNoPlayer");
	    return 1;
	}
	new para2 = cmdGetInt(1);
	AdminGiveMoney(playerid, para1, para2);
	return 1;
}

cmdResponse(ban)
{
	static string[144];
	if(GetPlayerAdmin(playerid) < 4)
	{
	    SendMessage(playerid, "wANoAccessCmd");
	    return 1;
	}
	cmdInit(2);
	if(cmdGetNum() != 2)
	{
		SendMessage(playerid, "mABanInfo");
	    return 1;
	}
	new targetid = cmdGetInt(0);
	if(!IsPlayerConnected(targetid))
	{
	    SendMessage(playerid, "wABanNoOnline");
	    return 1;
	}
	cmdGet(1, string);
	AdminBanPlayer(playerid, targetid, string);
	return 1;
}

cmdResponse(banakk)
{
	static name[MAX_PLAYER_NAME];
	static string[144];
	if(GetPlayerAdmin(playerid) < 4)
	{
	    SendMessage(playerid, "wANoAccessCmd");
	    return 1;
	}
 	cmdInit(2);
 	if(cmdGetNum() != 2)
	{
		SendMessage(playerid, "mABanAkkInfo");
	    return 1;
	}
 	cmdGet(0, name);
	cmdGet(1, string);
 	if(!AdminBanAccount(playerid, name, string))
 	{
		SendMessage(playerid, "wABanAkk");
 	}
	return 1;
}

cmdResponse(banakktm)
{
	static name[MAX_PLAYER_NAME];
	static string[144];
	if(GetPlayerAdmin(playerid) < 4)
	{
	    SendMessage(playerid, "wANoAccessCmd");
	    return 1;
	}
 	cmdInit(3);
 	if(cmdGetNum() != 3)
	{
		SendMessage(playerid, "mABanAkkTmInfo");
	    return 1;
	}
 	cmdGet(0, name);
 	new day = cmdGetInt(1);
 	if(day <= 0 || day > 30)
 	{
 	    SendMessage(playerid, "WABanAkkTmDay");
 	    return 1;
 	}
	cmdGet(2, string);
 	if(!AdminBanAccount(playerid, name, string, day))
 	{
		SendMessage(playerid, "WABanAkkTm");
 	}
	return 1;
}

cmdResponse(unbanakk)
{
	static name[MAX_PLAYER_NAME];
	if(GetPlayerAdmin(playerid) < 4)
	{
	    SendMessage(playerid, "wANoAccessCmd");
	    return 1;
	}
 	cmdInit(1);
 	if(cmdGetNum() != 1)
	{
		SendMessage(playerid, "mAUnBanAkkInfo");
	    return 1;
	}
 	cmdGet(0, name);
 	if(!AdminUnBanAccount(playerid, name))
 	{
 	    SendMessage(playerid, "wAUnBanAkk");
 	}
	return 1;
}

cmdResponse(banip)
{
	static string[20];
	if(GetPlayerAdmin(playerid) < 4)
	{
	    SendMessage(playerid, "wANoAccessCmd");
	    return 1;
	}
 	cmdInit(1);
 	if(cmdGetNum() != 1)
	{
		SendMessage(playerid, "mABanIPInfo");
	    return 1;
	}
	cmdGet(0, string);
	new IP:ip;
	if(!IPFromString(ip, string))
	{
	    SendMessage(playerid, "wABanIPWrongFormat");
	    return 1;
	}
 	if(!AdminBanIP(playerid, ip))
 	{
 	    SendMessage(playerid, "wABanIP");
 	}
	return 1;
}

cmdResponse(unbanip)
{
	static string[20];
	if(GetPlayerAdmin(playerid) < 4)
	{
	    SendMessage(playerid, "wANoAccessCmd");
	    return 1;
	}
 	cmdInit(1);
 	if(cmdGetNum() != 1)
	{
		SendMessage(playerid, "mAUnBanIPInfo");
	    return 1;
	}
	cmdGet(0, string);
	new IP:ip;
	if(!IPFromString(ip, string))
	{
	    SendMessage(playerid, "wAUnBanIPWrongFormat");
	    return 1;
	}
 	if(!AdminUnBanIP(playerid, ip))
 	{
 	    SendMessage(playerid, "wAUnBanIP");
 	}
	return 1;
}

cmdResponse(muteakk)
{
	static name[MAX_PLAYER_NAME];
	static string[144];
	if(GetPlayerAdmin(playerid) < 4)
	{
	    SendMessage(playerid, "wANoAccessCmd");
	    return 1;
	}
 	cmdInit(3);
 	if(cmdGetNum() != 3)
	{
		SendMessage(playerid, "mAMuteAkkInfo");
	    return 1;
	}
 	cmdGet(0, name);
 	new time = cmdGetInt(1);
 	if(time <= 0)
 	{
 	    SendMessage(playerid, "wAMuteAkkTime");
 	    return 1;
 	}
	cmdGet(2, string);
 	if(!AdminMuteAccount(playerid, name, time, string))
 	{
		SendMessage(playerid, "wAMuteAkk");
 	}
	return 1;
}

cmdResponse(prisonakk)
{
	static name[MAX_PLAYER_NAME];
	static string[144];
	if(GetPlayerAdmin(playerid) < 4)
	{
	    SendMessage(playerid, "wANoAccessCmd");
	    return 1;
	}
 	cmdInit(3);
 	if(cmdGetNum() != 3)
	{
		SendMessage(playerid, "mAPrisonAkkInfo");
	    return 1;
	}
 	cmdGet(0, name);
 	new time = cmdGetInt(1);
 	if(time <= 0)
 	{
 	    SendMessage(playerid, "wAPrisonAkkTime");
 	    return 1;
 	}
	cmdGet(2, string);
 	if(!AdminPrisonAccount(playerid, name, time, string))
 	{
		SendMessage(playerid, "wAPrisonAkk");
 	}
	return 1;
}


