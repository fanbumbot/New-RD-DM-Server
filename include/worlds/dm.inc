#if defined inc_world_dm
	#endinput
#endif
#define inc_world_dm

#if !defined AttachDynamic3DTextLabelPlayer
stock AttachDynamic3DTextLabelPlayer(Text3D:labelid, playerid, Float:offsetx, Float:offsety, Float:offsetz)
{
    Streamer_SetIntData(STREAMER_TYPE_3D_TEXT_LABEL, labelid, E_STREAMER_ATTACHED_PLAYER, playerid);
    Streamer_SetFloatData(STREAMER_TYPE_3D_TEXT_LABEL, labelid, E_STREAMER_ATTACH_OFFSET_X, offsetx);
    Streamer_SetFloatData(STREAMER_TYPE_3D_TEXT_LABEL, labelid, E_STREAMER_ATTACH_OFFSET_Y, offsety);
    return Streamer_SetFloatData(STREAMER_TYPE_3D_TEXT_LABEL, labelid, E_STREAMER_ATTACH_OFFSET_Z, offsetz);
}
#endif

public OnScriptInit()
{   
    new World:world = worldCreate("DM");
    WorldAddAllGroup(world);
    
    lobbyCreateArray(world, 10);
    
    itemInfoAdd(Player, "DMTDKills");
    itemInfoAdd(Player, "DMTDDeaths");
    itemInfoAdd(Player, "DMKills", 0);
    itemInfoAdd(Player, "DMDeaths", 0);
    itemInfoAdd(Player, "DMText3D");
    
#if defined wdm_OnScriptInit
    wdm_OnScriptInit();
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
#define OnScriptInit wdm_OnScriptInit
#if defined wdm_OnScriptInit
forward wdm_OnScriptInit();
#endif

stock OnPlayerSpawnWorldDMZ(playerid)
{
    UpdatePlayerGeneral(playerid);

    SetPlayerHealth(playerid, 100.0);
    SetPlayerArmour(playerid, 100.0);

    static Float:x, Float:y, Float:z, Float:rz, interior, rnd;
    rz = float(random(360));
    interior = 0;

	new World:world = worldGetByName("DM");
    new Lobby:lobby = GetPlayerLobby(playerid);
	if(lobby == lobbyGetByID(world, 0))
	{
		rnd = random(5);
		switch(rnd)
		{
			case 0:	x = 2139.15, y = -2289.38, z = 14.78;
			case 1:	x = 2119.97, y = -2271.68, z = 14.78;
			case 2:	x = 2150.60, y = -2257.30, z = 13.29;
			case 3:	x = 2160.69, y = -2266.93, z = 16.72;
			case 4:	x = 2153.12, y = -2258.14, z = 20.66;
		}
	}
	if(lobby == lobbyGetByID(world, 1))
	{
	    rnd = random(6);
	    switch(rnd)
		{
			case 0:	x = 2845.57, y = -2509.37, z = 20.41;
			case 1:	x = 2831.22, y = -2500.49, z = 20.41;
			case 2:	x = 2845.84, y = -2491.64, z = 20.41;
			case 3:	x = 2830.99, y = -2484.85, z = 19.19;
			case 4:	x = 2838.41, y = -2478.27, z = 20.41;
			case 5:	x = 2846.13, y = -2465.37, z = 20.41;
 	    }
	}
	if(lobby == lobbyGetByID(world, 2))
	{
		rnd = random(4);
		switch(rnd)
		{
			case 0:	x = -1514.65, y = 1368.89, z = 2.48;
			case 1:	x = -1517.02, y = 1380.84, z = 2.58;
			case 2:	x = -1496.61, y = 1382.95, z = 2.64;
			case 3:	x = -1502.36, y = 1368.49, z = 2.83;
		}
	}
	if(lobby == lobbyGetByID(world, 3))
	{
		rnd = random(10);
		switch(rnd)
		{
			case 0:	x = -1442.44, y = 1489.40, z = 7.10;
			case 1:	x = -1435.90, y = 1493.27, z = 7.10;
			case 2:	x = -1416.64, y = 1489.05, z = 7.10;
			case 3:	x = -1393.63, y = 1490.71, z = 7.10;
			case 4:	x = -1400.90, y = 1485.25, z = 7.10;
			case 5:	x = -1450.81, y = 1485.64, z = 11.80;
			case 6:	x = -1423.21, y = 1485.08, z = 11.80;
			case 7:	x = -1394.00, y = 1485.78, z = 11.80;
			case 8:	x = -1413.75, y = 1496.59, z = 11.80;
			case 9:	x = -1385.85, y = 1490.39, z = 13.83;
		}
	}
	if(lobby == lobbyGetByID(world, 4))
	{
		rnd = random(12);
		switch(rnd)
		{
			case 0:	x = -2380.78, y = 1551.16, z = 31.85;
			case 1:	x = -2395.42, y = 1535.90, z = 31.85;
			case 2:	x = -2413.93, y = 1543.41, z = 31.85;
			case 3:	x = -2432.24, y = 1543.50, z = 31.85;
			case 4:	x = -2454.44, y = 1554.87, z = 28.94;
			case 5:	x = -2455.74, y = 1533.41, z = 28.94;
			case 6:	x = -2417.62, y = 1554.17, z = 26.04;
			case 7:	x = -2417.43, y = 1538.70, z = 26.04;
			case 8:	x = -2401.23, y = 1532.22, z = 26.04;
			case 9:	x = -2411.08, y = 1548.06, z = 26.04;
			case 10:	x = -2404.20, y = 1557.33, z = 26.04;
			case 11:	x = -2391.16, y = 1541.80, z = 26.04;
		}
	}
	if(lobby == lobbyGetByID(world, 5))
	{
		rnd = random(7);
		switch(rnd)
		{
			case 0:	x = -1295.95, y = 2491.33, z = 87.04;
			case 1:	x = -1309.97, y = 2492.18, z = 89.86;
			case 2:	x = -1321.82, y = 2511.98, z = 92.54;
			case 3:	x = -1290.32, y = 2517.22, z = 87.18;
			case 4:	x = -1292.83, y = 2529.00, z = 90.38;
			case 5:	x = -1300.20, y = 2527.61, z = 87.59;
			case 6:	x = -1317.27, y = 2526.60, z = 87.55;
		}
	}
	if(lobby == lobbyGetByID(world, 6))
	{
		rnd = random(7);
		switch(rnd)
		{
			case 0:	x = -325.60, y = 1514.07, z = 75.35;
			case 1:	x = -305.68, y = 1513.70, z = 75.35;
			case 2:	x = -286.21, y = 1523.14, z = 75.35;
			case 3:	x = -293.92, y = 1537.10, z = 75.56;
			case 4:	x = -304.23, y = 1541.54, z = 80.22;
			case 5:	x = -319.39, y = 1539.48, z = 79.89;
			case 6:	x = -313.41, y = 1543.23, z = 75.56;
		}
	}
	if(lobby == lobbyGetByID(world, 7))
	{
		rnd = random(8);
		switch(rnd)
		{
			case 0:	x = 2514.95, y = -1680.98, z = 13.43;
			case 1:	x = 2515.04, y = -1662.01, z = 13.84;
			case 2:	x = 2497.83, y = -1650.86, z = 13.52;
			case 3:	x = 2495.02, y = -1687.08, z = 13.51;
			case 4:	x = 2482.32, y = -1686.94, z = 13.50;
			case 5:	x = 2469.13, y = -1679.14, z = 13.50;
			case 6:	x = 2467.40, y = -1659.07, z = 13.28;
			case 7:	x = 2485.51, y = -1649.94, z = 13.47;
		}
	}
	if(lobby == lobbyGetByID(world, 8))
	{
		rnd = random(9);
		switch(rnd)
		{
			case 0:	x = 229.99, y = 1875.07, z = 17.64;
			case 1:	x = 230.29, y = 1908.39, z = 17.64;
			case 2:	x = 214.66, y = 1908.52, z = 17.64;
			case 3:	x = 198.04, y = 1908.06, z = 17.64;
			case 4:	x = 175.11, y = 1886.63, z = 20.90;
			case 5:	x = 179.57, y = 1866.66, z = 20.69;
			case 6:	x = 212.43, y = 1860.75, z = 20.64;
			case 7:	x = 241.37, y = 1863.43, z = 20.63;
			case 8:	x = 249.16, y = 1907.49, z = 20.60;
		}
	}
	if(lobby == lobbyGetByID(world, 9))
	{
		rnd = random(8);
		interior = 10;
		switch(rnd)
		{
			case 0:		x = 245.67, y = 112.98, z = 1003.88, rz = 20.90;
			case 1:		x = 273.36, y = 119.31, z = 1004.61, rz = 230.00;
			case 2:		x = 261.26, y = 110.52, z = 1004.61, rz = 228.00;
			case 3:		x = 266.48, y = 119.91, z = 1004.61, rz = 261.27;
			case 4:		x = 257.95, y = 111.18, z = 1003.21, rz = 90.00;
			case 5:		x = 277.76, y = 126.08, z = 1004.61, rz = 150.00;
			case 6:		x = 277.41, y = 118.83, z = 1008.81, rz = 45.00;
			case 7:		x = 259.41, y = 116.39, z = 1008.82, rz = 0.00;
			case 8:		x = 277.75, y = 116.74, z = 1008.81, rz = 110.00;
			case 9:		x = 274.63, y = 109.90, z = 1008.82, rz = 90.00;
			case 10:	x = 260.69, y = 108.90, z = 1008.82, rz = 305.00;
			case 11:	x = 247.49, y = 121.12, z = 1010.21, rz = 0.00;
			case 12:	x = 227.13, y = 127.02, z = 1010.21, rz = 230.00;
			case 13:	x = 223.83, y = 126.92, z = 1010.21, rz = 175.00;
			case 14:	x = 218.68, y = 114.94, z = 1010.21, rz = 275.00;
			case 15:	x = 238.20, y = 110.53, z = 1010.21, rz = 105.00;
			case 16:	x = 237.80, y = 114.85, z = 1010.21, rz = 75.00;
			case 17:	x = 216.05, y = 123.45, z = 1007.41, rz = 275.00;
			case 18:	x = 219.12, y = 113.48, z = 1003.21, rz = 310.00;
			case 19:	x = 215.38, y = 125.97, z = 1003.21, rz = 200.00;
			case 20:	x = 225.11, y = 121.19, z = 999.07, rz = 90.00;
			case 21:	x = 215.07, y = 114.02, z = 999.01, rz = 314.00;
			case 22:	x = 215.64, y = 120.26, z = 999.01, rz = 285.00;
		}
	}
	SetPlayerInterior(playerid, interior);
	SetPlayerPos(playerid, x, y, z);
	SetPlayerFacingAngle(playerid, rz);
	SetCameraBehindPlayer(playerid);
	return 1;
}

public OnPlayerConnect(playerid)
{
#if defined wdm_OnPlayerConnect
    wdm_OnPlayerConnect(playerid);
#endif

	new PlayerText:tdkills = CreatePlayerTextDraw(playerid,500.000000,375.000000,"_");
	PlayerTextDrawAlignment(playerid,tdkills,0);
	PlayerTextDrawBackgroundColor(playerid,tdkills,0x000000ff);
	PlayerTextDrawFont(playerid,tdkills,1);
	PlayerTextDrawLetterSize(playerid,tdkills,0.499999,2.599999);
	PlayerTextDrawColor(playerid,tdkills,0x00ff00ff);
	PlayerTextDrawSetOutline(playerid,tdkills,1);
	PlayerTextDrawSetProportional(playerid,tdkills,1);
	PlayerTextDrawSetShadow(playerid,tdkills,1);

	new PlayerText:tddeaths = CreatePlayerTextDraw(playerid,500.000000,400.000000,"_");
	PlayerTextDrawAlignment(playerid,tddeaths,0);
	PlayerTextDrawBackgroundColor(playerid,tddeaths,0x000000ff);
	PlayerTextDrawFont(playerid,tddeaths,1);
	PlayerTextDrawLetterSize(playerid,tddeaths,0.499999,2.599999);
	PlayerTextDrawColor(playerid,tddeaths,0xff0000ff);
	PlayerTextDrawSetOutline(playerid,tddeaths,1);
	PlayerTextDrawSetProportional(playerid,tddeaths,1);
	PlayerTextDrawSetShadow(playerid,tddeaths,1);
	
	new Item:player = itemGetByID(Player, playerid);
	
	itemSetInt(player, "DMTDKills", _:tdkills);
	itemSetInt(player, "DMTDDeaths", _:tddeaths);
	return 1;
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect wdm_OnPlayerConnect
#if defined wdm_OnPlayerConnect
forward wdm_OnPlayerConnect(playerid);
#endif

public OnPlayerDisconnect(playerid, reason)
{
	new Item:player = itemGetByID(Player, playerid);

	PlayerTextDrawHide(playerid, PlayerText:itemGetInt(player, "DMTDKills"));
	PlayerTextDrawHide(playerid, PlayerText:itemGetInt(player, "DMTDDeaths"));
	PlayerTextDrawDestroy(playerid, PlayerText:itemGetInt(player, "DMTDKills"));
	PlayerTextDrawDestroy(playerid, PlayerText:itemGetInt(player, "DMTDDeaths"));
	Delete3DTextLabel(Text3D:itemGetInt(player, "DMText3D"));
	
#if defined wdm_OnPlayerDisconnect
    wdm_OnPlayerDisconnect(playerid, reason);
#endif
	return 1;
}
#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect wdm_OnPlayerDisconnect
#if defined wdm_OnPlayerDisconnect
forward wdm_OnPlayerDisconnect(playerid, reason);
#endif

public OnPlayerEnterLobby(playerid, World:world, Lobby:lobby)
{
#if defined wdm_OnPlayerEnterLobby
    wdm_OnPlayerEnterLobby(playerid, world, lobby);
#endif
	if(world == worldGetByName("DM"))
    {
        new Item:player = itemGetByID(Player, playerid);
        
		VehicleSpeedometerDisable(playerid);
	    PlayerInvulnerableDisable(playerid);
	    PlayerDriftDisable(playerid);
	    TogglePlayerControllable(playerid, 1);
	    PlayerNoSDisable(playerid);
	    PlayerAutoRepairDisable(playerid);
	    GeneralEnable(playerid);
	    
		SendMessage(playerid, "mInDMZone1");
		SendMessage(playerid, "mInDMZone2");
		SendMessage(playerid, "mInDMZone3");

		itemSetInt(player, "DMKills", 0);
		itemSetInt(player, "DMDeaths", 0);

		new PlayerText:tdkills = PlayerText:itemGetInt(player, "DMTDKills");
		new PlayerText:tddeaths = PlayerText:itemGetInt(player, "DMTDDeaths");
		PlayerTextDrawSetString(playerid, tdkills, "Kills: 0");
		PlayerTextDrawSetString(playerid, tddeaths, "Deaths: 0");
		PlayerTextDrawShow(playerid, tdkills);
		PlayerTextDrawShow(playerid, tddeaths);
		new Text3D:text = Create3DTextLabel("{00FF00}Kills: 0\n{FF0000}Deaths: 0", 0xFFFFFFAA, 0.000, 0.000, -4.000, 18.0, 0, 1);
		Attach3DTextLabelToPlayer(text, playerid, 0.0, 0.0, 1.00);
		itemSetInt(player, "DMText3D", _:text);
	}
	return 1;
}
#if defined _ALS_OnPlayerEnterLobby
	#undef OnPlayerEnterLobby
#else
    #define _ALS_OnPlayerEnterLobby
#endif
#define OnPlayerEnterLobby wdm_OnPlayerEnterLobby
#if defined wdm_OnPlayerEnterLobby
forward wdm_OnPlayerEnterLobby(playerid, World:world, Lobby:lobby);
#endif

public OnPlayerExitLobby(playerid, World:world, Lobby:lobby, reason)
{
#if defined wdm_OnPlayerExitLobby
    wdm_OnPlayerExitLobby(playerid, world, lobby, reason);
#endif
	if(world == worldGetByName("DM"))
	{
	    new Item:player = itemGetByID(Player, playerid);
		PlayerTextDrawHide(playerid, PlayerText:itemGetInt(player, "DMTDKills"));
		PlayerTextDrawHide(playerid, PlayerText:itemGetInt(player, "DMTDDeaths"));
		Delete3DTextLabel(Text3D:itemGetInt(player, "DMText3D"));
	}
	return 1;
}
#if defined _ALS_OnPlayerExitLobby
	#undef OnPlayerExitLobby
#else
    #define _ALS_OnPlayerExitLobby
#endif
#define OnPlayerExitLobby wdm_OnPlayerExitLobby
#if defined wdm_OnPlayerExitLobby
forward wdm_OnPlayerExitLobby(playerid, World:world, Lobby:lobby, reason);
#endif

public OnPlayerDeath(playerid, killerid, reason)
{
#if defined wdm_OnPlayerDeath
    wdm_OnPlayerDeath(playerid, killerid, reason);
#endif
	if(GetPlayerWorld(playerid) == worldGetByName("DM"))
	{
	    static strloc[128];
	    new Item:player = itemGetByID(Player, playerid);
	    new Item:killer = itemGetByID(Player, killerid);
	    new kills, deaths;
	    if(IsPlayerConnected(killerid))
	    {
	    	GivePlayerServerMoney(killerid, 1800+random(400));

			kills = itemGetInt(killer, "DMKills")+1;
			itemSetInt(killer, "DMKills", kills);

			format(strloc, sizeof(strloc), "Kills: %i", kills);
			PlayerTextDrawSetString(killerid, PlayerText:itemGetInt(killer, "DMTDKills"), strloc);
			
			format(strloc, sizeof(strloc), "{00FF00}Kills: %i\n{FF0000}Deaths: %i", kills, itemGetInt(killer, "DMDeaths"));
			Update3DTextLabelText(Text3D:itemGetInt(killer, "DMText3D"), 0x00FF00FF, strloc);
		}
		deaths = itemGetInt(player, "DMDeaths")+1;
		itemSetInt(player, "DMDeaths", deaths);
		
		format(strloc, sizeof(strloc), "Deaths: %i", deaths);
		PlayerTextDrawSetString(playerid, PlayerText:itemGetInt(player, "DMTDDeaths"), strloc);
		format(strloc, sizeof(strloc), "{00FF00}Kills: %i\n{FF0000}Deaths: %i", itemGetInt(player, "DMKills"), deaths);
		Update3DTextLabelText(Text3D:itemGetInt(player, "DMText3D"), 0x00FF00FF, strloc);
	}

	return 1;
}
#if defined _ALS_OnPlayerDeath
	#undef OnPlayerDeath
#else
    #define _ALS_OnPlayerDeath
#endif
#define OnPlayerDeath wdm_OnPlayerDeath
#if defined wdm_OnPlayerDeath
forward wdm_OnPlayerDeath(playerid, killerid, reason);
#endif

public OnPlayerSpawn(playerid)
{
	if(GetPlayerWorld(playerid) == worldGetByName("DM"))
	{
	    OnPlayerSpawnWorldDMZ(playerid);
	}
#if defined wdm_OnPlayerSpawn
    wdm_OnPlayerSpawn(playerid);
#endif
	return 1;
}
#if defined _ALS_OnPlayerSpawn
	#undef OnPlayerSpawn
#else
    #define _ALS_OnPlayerSpawn
#endif
#define OnPlayerSpawn wdm_OnPlayerSpawn
#if defined wdm_OnPlayerSpawn
forward wdm_OnPlayerSpawn(playerid);
#endif


