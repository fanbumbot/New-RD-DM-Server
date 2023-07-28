#if defined inc_wreglog
	#endinput
#endif
#define inc_wreglog

public OnScriptInit()
{
    WorldAddAllGroup(worldCreate("Reglog"));
#if defined wreglog_OnScriptInit
    wreglog_OnScriptInit();
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
#define OnScriptInit wreglog_OnScriptInit
#if defined wreglog_OnScriptInit
forward wreglog_OnScriptInit();
#endif

public OnPlayerEnterLobby(playerid, World:world, Lobby:lobby)
{
#if defined wreglog_OnPlayerEnterLobby
    wreglog_OnPlayerEnterLobby(playerid, world, lobby);
#endif
	if(world == worldGetByName("Reglog"))
	{
	    ResetPlayerWeapons(playerid);
	    PlayerInvulnerableEnable(playerid);
        TogglePlayerControllable(playerid, 0);

		new states = GetPlayerState(playerid);
		if(states == 0 || states == 7 || states == 9)
		{
		    SpawnPlayer(playerid);
		}
		else
		{
		    SetPlayerSkin(playerid, 0);
		    SetPlayerFacingAngle(playerid, -90.0);
			SetPlayerCameraPos(playerid, 2501.3, -1666.5, 14.34);
			SetPlayerCameraLookAt(playerid, 2497.3, -1666.5, 13.34);
			SetPlayerPos(playerid, 2497.3, -1666.5, 13.34);
		    
		    SetPlayerHealth(playerid, 100.0);
		    SetPlayerArmour(playerid, 100.0);
		}

	    SetPlayerColor(playerid, 0x7F7F7FFF);
	    SetPlayerMarkerForAll(playerid, 0x7F7F7FFF);

		if(IsPlayerRegistered(playerid))
		{
		    SetPlayerSkin(playerid, PlayerGetInt(playerid, "Skin"));
		    if(Lang:PlayerGetInt(playerid, "Lang") == LANG_NONE)
		    {
		        ShowDialog(playerid, Dialog:ChoiseLanguage);
		    }
		    else
			{
				if(!IsPlayerLogin(playerid))
				{
				    ShowDialog(playerid, Dialog:Login);
				}
				else
				{
				    SetPlayerWorld(playerid, "Hub", 0);
				}
			}
		}
		else
		{
		    PlayerSetInt(playerid, "Lang", _:LANG_NONE);
			ShowDialog(playerid, Dialog:ChoiseLanguage);
		}
	}
	return 1;
}
#if defined _ALS_OnPlayerEnterLobby
	#undef OnPlayerEnterLobby
#else
    #define _ALS_OnPlayerEnterLobby
#endif
#define OnPlayerEnterLobby wreglog_OnPlayerEnterLobby
#if defined wreglog_OnPlayerEnterLobby
forward wreglog_OnPlayerEnterLobby(playerid, World:world, Lobby:lobby);
#endif

public OnPlayerExitLobby(playerid, World:world, Lobby:lobby, reason)
{

#if defined wreglog_OnPlayerExitLobby
    wreglog_OnPlayerExitLobby(playerid, world, lobby, reason);
#endif
	return 1;
}
#if defined _ALS_OnPlayerExitLobby
	#undef OnPlayerExitLobby
#else
    #define _ALS_OnPlayerExitLobby
#endif
#define OnPlayerExitLobby wreglog_OnPlayerExitLobby
#if defined wreglog_OnPlayerExitLobby
forward wreglog_OnPlayerExitLobby(playerid, World:world, Lobby:lobby, reason);
#endif

public OnPlayerSpawn(playerid)
{
#if defined wreglog_OnPlayerSpawn
    wreglog_OnPlayerSpawn(playerid);
#endif
	if(GetPlayerWorld(playerid) == worldGetByName("Reglog"))
	{
	    SetPlayerSkin(playerid, 0);
	    SetPlayerFacingAngle(playerid, -90.0);
		SetPlayerCameraPos(playerid, 2501.3, -1666.5, 14.34);
		SetPlayerCameraLookAt(playerid, 2497.3, -1666.5, 13.34);
		SetPlayerPos(playerid, 2497.3, -1666.5, 13.34);
	    
	    SetPlayerHealth(playerid, 100.0);
	    SetPlayerArmour(playerid, 100.0);
	    
	    SetPlayerColor(playerid, 0x7F7F7FFF);
	    SetPlayerMarkerForAll(playerid, 0x7F7F7FFF);
	}
	return 1;
}
#if defined _ALS_OnPlayerSpawn
	#undef OnPlayerSpawn
#else
    #define _ALS_OnPlayerSpawn
#endif
#define OnPlayerSpawn wreglog_OnPlayerSpawn
#if defined wreglog_OnPlayerSpawn
forward wreglog_OnPlayerSpawn(playerid);
#endif

/*
    SetPlayerSpawn(playerid, 108, 0.0, 0.0, 3.0, 0.0, 0, 0);
    SetSpawnClassRequest(playerid, 0);
    ClearPlayerSpawnWeap(playerid);

    PlayerNoSEnable(playerid);
    PlayerAutoRepairEnable(playerid);
*/





