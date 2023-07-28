#if defined inc_cmd_global
	#endinput
#endif
#define inc_cmd_global

#define MIN_HEAL_TICK 4000

public OnScriptInit()
{
	itemInfoAdd(Player, "svtis", 0);
	itemInfoAdd(Player, "svtx");
	itemInfoAdd(Player, "svty");
	itemInfoAdd(Player, "svtz");
	itemInfoAdd(Player, "svtrz");
	itemInfoAdd(Player, "svtinterior");
	itemInfoAdd(Player, "heallasttick");

#if defined cmd_global_OnScriptInit
    cmd_global_OnScriptInit();
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
#define OnScriptInit cmd_global_OnScriptInit
#if defined cmd_global_OnScriptInit
forward cmd_global_OnScriptInit();
#endif

cmdResponse(lang)
{
    ShowDialog(playerid, Dialog:ChoiseLanguage);
	return 1;
}

cmdResponse(langinfo)
{
	SendMessage(playerid, "mCurrentLanguage");
	return 1;
}

cmdResponse(menu)
{
    ShowDialog(playerid, Dialog:GlobalMenu);
	return 1;
}

cmdResponse(admins)
{
	ShowDialog(playerid, Dialog:GMenuAdmins);
	return 1;
}

cmdResponse(kill)
{
    SetPlayerHealth(playerid, 0.0);
	return 1;
}

cmdResponse(spawn)
{
	SpawnPlayer(playerid);
	return 1;
}

cmdResponse(svt)
{
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
	new Item:player = itemGetByID(Player, playerid);
	itemSetInt(player, "svtis", 1);
	itemSetFloat(player, "svtx", x);
	itemSetFloat(player, "svty", y);
	itemSetFloat(player, "svtz", z);
	itemSetFloat(player, "svtrz", rz);
	itemSetInt(player, "svtinterior", GetPlayerInterior(playerid));
	return 1;
}

cmdResponse(rdt)
{
    new Item:player = itemGetByID(Player, playerid);
    if(!itemGetInt(player, "svtis"))
    {
	    SendMessage(playerid, "wNoSVT");
	    return 1;
	}
	UniversalTeleport(playerid, itemGetFloat(player, "svtx"), itemGetFloat(player, "svty"),
		itemGetFloat(player, "svtz"), itemGetFloat(player, "svtrz"));
	SetPlayerInterior(playerid, itemGetInt(player, "svtinterior"));
	return 1;
}

cmdResponse(heal)
{
    new Item:player = itemGetByID(Player, playerid);
    new dif = GetTickCount()-itemGetInt(player, "heallasttick");
	if(dif <= MIN_HEAL_TICK)
	{
		SendMessage(playerid, "wHealSpam", (MIN_HEAL_TICK-dif)/1000);
	    return 1;
	}
	SetPlayerHealth(playerid, 100.0);
	SetPlayerArmour(playerid, 100.0);
	SendMessage(playerid, "mHealed");
	return 1;
}

cmdResponse(givemoney)
{
	cmdInit(2);
	if(cmdGetNum() != 2)
	{
	    SendMessage(playerid, "mGiveMoneyInfo");
		return 1;
	}
	new para1 = cmdGetInt(0);
	if(!IsPlayerConnected(para1))
	{
	    SendMessage(playerid, "wGiveMoney1");
		return 1;
	}
	if(para1 == playerid)
	{
	    SendMessage(playerid, "wGiveMoney2");
	    return 1;
	}
	
	new para2 = cmdGetInt(1);
	if(para2 < 0)
	{
	    SendMessage(playerid, "wGiveMoney3");
		return 1;
	}
	new money = GetPlayerServerMoney(playerid)-para2;
	if(money < 0)
	{
	    SendMessage(playerid, "wGiveMoney3");
		return 1;
	}
	SetPlayerServerMoney(playerid, money);
	GivePlayerServerMoney(para1, para2);
	SendMessage(playerid, "wGiveMoney4", PlayerName(para1), para2);
	SendMessage(para1, "wGiveMoney5", PlayerName(playerid), para2);
	printf("Игрок %s передал игроку %s сумму %i $", PlayerName(playerid), PlayerName(para1), para2);
	return 1;
}

cmdResponse(pm)
{
	static string[144];

    cmdInit(2);
	new Item:player = itemGetByID(Player, playerid);
	if(itemGetInt(player, "Muted"))
	{
	    SendMessage(playerid, "wMuted", PlayerGetInt(playerid, "MuteSec"));
	    return 0;
	}
	if(cmdGetNum() != 2)
	{
	    SendMessage(playerid, "mPMInfo");
		return 1;
	}
	new para1 = cmdGetInt(0);
	if(!IsPlayerConnected(para1))
	{
	    SendMessage(playerid, "wPMOffline");
		return 1;
	}
	if(para1 == playerid)
	{
	    SendMessage(playerid, "wPMSchizo");
	    return 1;
	}
	cmdGet(1, string);
	
	SendMessage(para1, "mPMSentFrom", PlayerName(playerid), playerid, string);
	SendMessage(playerid, "mPMSentFor", PlayerName(para1), para1, string);
	printf("<PM> %s [%i] --> %s [%i]: %s", PlayerName(playerid), playerid, PlayerName(para1), para1, string);
	SendAdminMessage("mAPM", PlayerName(playerid), playerid, PlayerName(para1), para1, string);
	if(IsPlayerAFK(para1))
	{
	    new time = floatround(GetAFKTime(para1)/1000, floatround_floor);
	    SendMessage(playerid, "wPMAFK", PlayerName(para1), para1, time);
	    printf("<PM> игрок-получатель сообщения %s [%i] в AFK %i секунд", PlayerName(para1), para1, time);
 	}
	return 1;
}

cmdResponse(dt)
{
	cmdInit(1);
	new World:hub = worldGetByName("Hub");
	if(GetPlayerWorld(playerid) != hub)
	{
	    SendMessage(playerid, "wUnusualWorld");
	    return 1;
	}
	if(cmdGetNum() != 1)
	{
	    SendMessage(playerid, "mVWInfo");
		return 1;
	}
	new para1 = cmdGetInt(0);
	if(para1 < 0 || para1 >= worldGetSize(hub))
	{
	    SendMessage(playerid, "wVWNumber");
	    return 1;
	}
	new vehicleid = GetPlayerVehicleID(playerid);

	new model = GetVehicleModel(vehicleid);
	new Lobby:lobby = lobbyGetByID(hub, para1);
	if(model == 425 || model == 432 || model == 520)
	{
        PlayerEnterLobby(playerid, lobby);
	}
	else
	{
	    new Float:x, Float:y, Float:z;
	    GetPlayerPos(playerid, x, y, z);
		UniversalTeleport(playerid, x, y, z, .lobby = lobby, .camreset = false);
	}
	SendMessage(playerid, "wVWConnected", para1);
	return 1;
}

cmd:mmd(playerid, params[])
{
	new menu = !PlayerGetInt(playerid, "MenuButton");
	PlayerSetInt(playerid, "MenuButton", menu);
    if(!menu)
    {
        SendMessage(playerid, "wMMD1");
    }
    else
    {
        SendMessage(playerid, "wMMD2");
    }
	return 1;
}

cmdResponse(help)
{
	SendMessage(playerid, "mHelp1");
	SendMessage(playerid, "mHelp2");
	SendMessage(playerid, "mHelp3");
	ShowDialog(playerid, Dialog:HelpCMD);
	return 1;
}

cmdResponse(rules)
{
	ShowDialog(playerid, Dialog:RulesCMD);
	return 1;
}

cmdResponse(radio)
{
	ShowDialog(playerid, Dialog:Radio);
	return 1;
}













































































