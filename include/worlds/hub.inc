#if defined inc_world_hub
	#endinput
#endif
#define inc_world_hub

//Координаты случайного спавна
new Float:playspa[][4] = {
	{-312.7, 1532.72, 75.36, 182.77},
	{417.29, 2503.67, 16.48, 89.17},
	{2169.01, 1676.55, 10.82, 82.77},
	{1243.24, 118.28, 20.39, 338.63},
	{2492.34, -1666.33, 13.34, 83.28},
	{-1981.07, 444.14, 35.17, 359.52}
};

stock OnPlayerSpawnWorldHub(playerid)
{
    UpdatePlayerGeneral(playerid);
    
    SetPlayerHealth(playerid, 100.0);
    SetPlayerArmour(playerid, 100.0);
    
	new gangid = GetPlayerGang(playerid);
	
    new Float:x, Float:y, Float:z, Float:rz, interior = -1;
	
	if(IsGangExist(gangid))
	{
	    GetGangSpawn(gangid, x, y, z, rz, interior);
	    printf("interior: %i", interior);
	}
	if(interior == -1)
	{
	    interior = 0;
	    new rnd = random(sizeof(playspa));
	    x = playspa[rnd][0];
	    y = playspa[rnd][1];
	    z = playspa[rnd][2];
	    rz = playspa[rnd][3];
	}
    SetPlayerInterior(playerid, interior);
    SetPlayerPos(playerid, x, y, z);
    SetPlayerFacingAngle(playerid, rz);
	SetCameraBehindPlayer(playerid);
	return 1;
}

public OnScriptInit()
{   
    new World:world = worldCreate("Hub");
    WorldAddAllGroup(world);
    
    lobbyCreateArray(world, 1000);
    
#if defined whub_OnScriptInit
    whub_OnScriptInit();
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
#define OnScriptInit whub_OnScriptInit
#if defined whub_OnScriptInit
forward whub_OnScriptInit();
#endif

public OnPlayerEnterLobby(playerid, World:world, Lobby:lobby)
{
#if defined whub_OnPlayerEnterLobby
    whub_OnPlayerEnterLobby(playerid, world, lobby);
#endif
	if(world == worldGetByName("Hub"))
    {
        if(lobby == lobbyGetByID(world, 0))
		{
			ResetPlayerWeapons(playerid);
		}
		VehicleSpeedometerEnable(playerid, playerid);
	    PlayerInvulnerableDisable(playerid);
	    PlayerDriftEnable(playerid);
	    TogglePlayerControllable(playerid, 1);
	    PlayerNoSEnable(playerid);
	    PlayerAutoRepairEnable(playerid);
	    OnPlayerSpawnWorldHub(playerid);
		GeneralEnable(playerid);
	}
	return 1;
}
#if defined _ALS_OnPlayerEnterLobby
	#undef OnPlayerEnterLobby
#else
    #define _ALS_OnPlayerEnterLobby
#endif
#define OnPlayerEnterLobby whub_OnPlayerEnterLobby
#if defined whub_OnPlayerEnterLobby
forward whub_OnPlayerEnterLobby(playerid, World:world, Lobby:lobby);
#endif

public OnPlayerSpawn(playerid)
{
#if defined whub_OnPlayerSpawn
    whub_OnPlayerSpawn(playerid);
#endif

	if(GetPlayerWorld(playerid) == worldGetByName("Hub"))
	{
	    OnPlayerSpawnWorldHub(playerid);
	}

	return 1;
}
#if defined _ALS_OnPlayerSpawn
	#undef OnPlayerSpawn
#else
    #define _ALS_OnPlayerSpawn
#endif
#define OnPlayerSpawn whub_OnPlayerSpawn
#if defined whub_OnPlayerSpawn
forward whub_OnPlayerSpawn(playerid);
#endif


