#if defined inc_security
	#endinput
#endif
#define inc_security

#if !defined AUTORESTART_HOUR
	#define AUTORESTART_HOUR 3
#endif

static secbuffer[256];

static Lobby:prisonlobby;

static security_second_timer;

static autorestart;

new ORMTable:BanAccountTable;
new ORMTable:BanIPTable;
new ORMTable:BanGroupIPTable;

public OnScriptInit()
{
    BanAccountTable = ormTableCreate(maindb, "BanAccount", true,
		typeString, MAX_PLAYER_NAME, "Name", "",
		typeInteger, "IP", 0,
		typeString, MAX_PLAYER_NAME, "Admin", "",
		typeString, 256, "Reason", "",
		typeInteger, "UnixTimestampBan", 0,
		typeInteger, "UnixTimestampUnBan", cellmax
	);
    BanIPTable = ormTableCreate(maindb, "BanIP", true,
		typeInteger, "IP", 0,
		typeString, MAX_PLAYER_NAME, "Admin", "",
		typeString, 256, "Reason", "",
		typeInteger, "UnixTimestampBan", 0,
		typeInteger, "UnixTimestampUnBan", cellmax
	);
    BanGroupIPTable = ormTableCreate(maindb, "BanGroupIP", true,
		typeInteger, "IPStart", 0,
		typeInteger, "IPEnd", 0,
		typeString, MAX_PLAYER_NAME, "Admin", "",
		typeString, 256, "Reason", "",
		typeInteger, "UnixTimestampBan", 0,
		typeInteger, "UnixTimestampUnBan", cellmax
	);
	itemInfoAdd(Player, "Prisoned", 0);
	itemInfoAdd(Player, "Muted", 0);
	itemInfoAdd(Player, "Freezed", 0);

    security_second_timer = SetTimer("SecurityTimer", 1000, 1);
    
    AutoUnBan();
    autorestart = 0;
	
#if defined security_OnScriptInit
    security_OnScriptInit();
#endif

    itemInfoAdd(Player, "EnableSync", 1);

    prisonlobby = lobbyGetByID(worldGetByName("Hub"), 0);

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
#define OnScriptInit security_OnScriptInit
#if defined security_OnScriptInit
forward security_OnScriptInit();
#endif

public OnScriptExit()
{
#if defined security_OnScriptExit
    security_OnScriptExit();
#endif
	ormTableDestroy(BanAccountTable);
	ormTableDestroy(BanIPTable);
	ormTableDestroy(BanGroupIPTable);
	
	KillTimer(security_second_timer);
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
#define OnScriptExit security_OnScriptExit
#if defined security_OnScriptExit
forward security_OnScriptExit();
#endif

public OnIncomingConnection(playerid, ip_address[], port)
{
	static IP:ip;
	IPFromString(ip, ip_address);
	if(IsIPBanned(ip))
	{
	    KickPlayer(playerid);
	    return 1;
	}
	if(IsGroupIPBanned(ip))
	{
	    KickPlayer(playerid);
	    return 1;
	}
#if defined security_OnIncomingConnection
    security_OnIncomingConnection(playerid, ip_address, port);
#endif
	return 1;
}
#if defined _ALS_OnIncomingConnection
	#undef OnIncomingConnection
#else
    #define _ALS_OnIncomingConnection
#endif
#define OnIncomingConnection security_OnIncomingConnection
#if defined security_OnIncomingConnection
forward security_OnIncomingConnection(playerid, ip_address[], port);
#endif

public OnPlayerConnect(playerid)
{
	if(IsAccountBanned(PlayerName(playerid)))
	{
	    KickPlayer(playerid);
	    return 1;
	}
#if defined security_OnPlayerConnect
    security_OnPlayerConnect(playerid);
#endif
	return 1;
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect security_OnPlayerConnect
#if defined security_OnPlayerConnect
forward security_OnPlayerConnect(playerid);
#endif

public OnPlayerDisconnect(playerid, reason)
{
#if defined security_OnPlayerDisconnect
    security_OnPlayerDisconnect(playerid, reason);
#endif
	if(itemGetInt(itemGetByID(Player, playerid), "Muted"))
	{
	    SendMessageToAll("mDisconnectMuted", PlayerName(playerid), PlayerGetInt(playerid, "MuteSec"));
	}
	if(itemGetInt(itemGetByID(Player, playerid), "Prisoned"))
	{
	    SendMessageToAll("mDisconnectPrisoned", PlayerName(playerid), PlayerGetInt(playerid, "PrisonSec"));
	}
	return 1;
}
#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect security_OnPlayerDisconnect
#if defined security_OnPlayerDisconnect
forward security_OnPlayerDisconnect(playerid, reason);
#endif

public OnPlayerText(playerid, text[])
{
	new Item:player = itemGetByID(Player, playerid);
	if(!itemGetInt(player, "Login"))
	{
	    SendMessage(playerid, "wNotLogged");
	    return 0;
	}
	if(itemGetInt(player, "Muted"))
	{
	    SendMessage(playerid, "wMuted", PlayerGetInt(playerid, "MuteSec"));
	    return 0;
	}
#if defined security_OnPlayerText
    return security_OnPlayerText(playerid, text);
#else
	return 1;
#endif
}
#if defined _ALS_OnPlayerText
	#undef OnPlayerText
#else
    #define _ALS_OnPlayerText
#endif
#define OnPlayerText security_OnPlayerText
#if defined security_OnPlayerText
forward security_OnPlayerText(playerid, text[]);
#endif

public OnPlayerCommandReceived(playerid, cmd[], params[], flags)
{
#if defined security_OnPlayerCommandReceive
    if(!security_OnPlayerCommandReceive(playerid, cmd, params, flags))  return 0;
#endif
	if(!itemGetInt(itemGetByID(Player, playerid), "Login"))
	{
	    SendMessage(playerid, "wNotLogged");
	    return 0;
	}
	
	return 1;
}
#if defined _ALS_OnPlayerCommandReceived
	#undef OnPlayerCommandReceived
#else
    #define _ALS_OnPlayerCommandReceived
#endif
#define OnPlayerCommandReceived security_OnPlayerCommandReceive
#if defined security_OnPlayerCommandReceive
forward security_OnPlayerCommandReceive(playerid, cmd[], params[], flags);
#endif

public OnPlayerDialogShow(playerid, dialog[])
{
	if(!itemGetInt(itemGetByID(Player, playerid), "Login") && strlen(dialog) &&
		strcmp(dialog, Dialog:ChoiseLanguage) && strcmp(dialog, Dialog:Registration) &&
		strcmp(dialog, Dialog:Login) && strcmp(dialog, Dialog:LoginSkin)
	)
	{
	    SendMessage(playerid, "wNotLogged");
	    return 0;
	}

#if defined security_OnPlayerDialogShow
    return security_OnPlayerDialogShow(playerid, dialog);
#else
	return 1;
#endif
}
#if defined _ALS_OnPlayerDialogShow
	#undef OnPlayerDialogShow
#else
    #define _ALS_OnPlayerDialogShow
#endif
#define OnPlayerDialogShow security_OnPlayerDialogShow
#if defined security_OnPlayerDialogShow
forward security_OnPlayerDialogShow(playerid, dialog[]);
#endif

public OnIncomingPacket(playerid, packetid, BitStream:bs)
{
	if(packetid != 24 && !itemGetInt(itemGetByID(Player, playerid), "EnableSync"))    return 0;
#if defined security_OnIncomingPacket
    return security_OnIncomingPacket(playerid, packetid, bs);
#else
	return 1;
#endif
}
#if defined _ALS_OnIncomingPacket
	#undef OnIncomingPacket
#else
    #define _ALS_OnIncomingPacket
#endif
#define OnIncomingPacket security_OnIncomingPacket
#if defined security_OnIncomingPacket
forward security_OnIncomingPacket(playerid, packetid, BitStream:bs);
#endif

public OnIncomingRPC(playerid, rpcid, BitStream:bs)
{
	if(!itemGetInt(itemGetByID(Player, playerid), "EnableSync"))    return 0;
#if defined security_OnIncomingPacket
    return security_OnIncomingPacket(playerid, packetid, bs);
#else
	return 1;
#endif
}
#if defined _ALS_OnIncomingRPC
	#undef OnIncomingRPC
#else
    #define _ALS_OnIncomingRPC
#endif
#define OnIncomingRPC security_OnIncomingRPC
#if defined security_OnIncomingRPC
forward security_OnIncomingRPC(playerid, rpcid, BitStream:bs);
#endif

public OnPlayerSpawn(playerid)
{
#if defined security_OnPlayerSpawn
    security_OnPlayerSpawn(playerid);
#endif
    if(itemGetInt(itemGetByID(Player, playerid), "Prisoned"))
    {
    	PutPlayerInPrison(playerid);
	}
	return 1;
}
#if defined _ALS_OnPlayerSpawn
	#undef OnPlayerSpawn
#else
    #define _ALS_OnPlayerSpawn
#endif
#define OnPlayerSpawn security_OnPlayerSpawn
#if defined security_OnPlayerSpawn
forward security_OnPlayerSpawn(playerid);
#endif

stock security_PlayerLogin(playerid)
{
	new ret = PlayerLogin(playerid);
	if(ret)
	{
	    new Item:player = itemGetByID(Player, playerid);
	    new second;
		second = PlayerGetInt(playerid, "MuteSec");
		if(second < 0)	PlayerSetInt(playerid, "MuteSec", 0);
		if(second > 0)
		{
		    itemSetInt(player, "Muted", 1);
		}

		second = PlayerGetInt(playerid, "PrisonSec");
		if(second < 0)	PlayerSetInt(playerid, "PrisonSec", 0);
		if(second > 0)
		{
		    PutPlayerInPrison(playerid);
		    itemSetInt(player, "Prisoned", 1);
		}
	}
	return ret;
}
#if defined _ALS_PlayerLogin
	#undef PlayerLogin
#else
	#define _ALS_PlayerLogin
#endif
#define PlayerLogin security_PlayerLogin

stock KickPlayer(playerid)
{
	if(!IsPlayerConnected(playerid))    return 0;
	itemSetInt(itemGetByID(Player, playerid), "EnableSync", 0);
	SetTimerEx("KickPlayerTimer", 100, 0, "i", playerid);
	return 1;
}

forward KickPlayerTimer(playerid);
public KickPlayerTimer(playerid)
{
	Kick(playerid);
	return 1;
}

stock IsAccountBanned(name[])
{
	if(!strlen(name))   return 0;
	format(secbuffer, sizeof(secbuffer), "Name = '%s'", name);
	new ORMResult:result = ormSelect(BanAccountTable, secbuffer, "id");
	if(ormResultNumRows(result))
	{
	    ormResultClear(result);
	    ormResultDelete(result);
	    return 1;
	}
    ormResultClear(result);
    ormResultDelete(result);
	return 0;
}

stock IsIPBanned(IP:ip)
{
	format(secbuffer, sizeof(secbuffer), "IP = %i", _:ip);
	new ORMResult:result = ormSelect(BanAccountTable, secbuffer, "id");
	if(!ormResultNumRows(result))
	{
	    ormResultClear(result);
	    ormResultDelete(result);
	    
	    result = ormSelect(BanAccountTable, secbuffer, "id");
	    if(!ormResultNumRows(result))
	    {
		    ormResultClear(result);
		    ormResultDelete(result);
		    return 0;
	    }
	}
    ormResultClear(result);
    ormResultDelete(result);
	return 1;
}

stock IsGroupIPBanned(IP:ip)
{
	format(secbuffer, sizeof(secbuffer), "IP = %i", _:ip);
	new ORMResult:result = ormSelect(BanAccountTable, secbuffer, "id");

	new num = ormResultNumRows(result);

	new IP:ipstart, IP:ipend;

	for(new i = 0; i < num; i++, ormResultNext(result))
	{
	    ipstart = IP:ormGetFieldAssocInt(result, "IPStart", i);
	    ipend = IP:ormGetFieldAssocInt(result, "IPEnd", i);
	    
	    if(ipstart <= ip <= ipend)
	    {
		    ormResultClear(result);
		    ormResultDelete(result);
		    return 1;
	    }
	}

    ormResultClear(result);
    ormResultDelete(result);
	return 0;
}

stock BanAccount(name[], admin[] = "System", reason[] = "", second=-1)
{
	if(!strlen(name))   return 0;
	if(IsAccountBanned(name))   return 0;

	new playerid = FindPlayer(name);
	if(IsPlayerConnected(playerid)) KickPlayer(playerid);
	
	new bantime = gettime();
	new unbantime = bantime+second;
	if(second == -1)	unbantime = cellmax;
	
	new IP:ip = IP:AccountGetInt(name, "LastIP");
	
	IPToString(secbuffer, ip);
	format(secbuffer, sizeof(secbuffer), "banip %s", secbuffer);
	SendRconCommand(secbuffer);
	
	return ormCacheDestroy(ormInsertMaster(BanAccountTable,
		"Name", name,
		"IP", _:ip,
		"Admin", admin,
		"Reason", reason,
		"UnixTimestampBan", bantime,
		"UnixTimestampUnBan", unbantime
	));
}

stock BanPlayer(playerid, admin[] = "*System*", reason[] = "", second=-1)
{
	return BanAccount(PlayerName(playerid), admin, reason, second);
}

stock BanIP(IP:ip, admin[] = "System", reason[] = "", second=-1)
{
	new bantime = gettime();
	new unbantime = bantime+second;
	if(second == -1)	unbantime = cellmax;

	IPToString(secbuffer, ip);
	format(secbuffer, sizeof(secbuffer), "banip %s", secbuffer);
	SendRconCommand(secbuffer);

	return ormCacheDestroy(ormInsertMaster(BanIPTable,
		"IP", _:ip,
		"Admin", admin,
		"Reason", reason,
		"UnixTimestampBan", bantime,
		"UnixTimestampUnBan", unbantime
	));
}

stock BanGroupIP(IP:ipstart, IP:ipend, admin[] = "System", reason[] = "", second=-1)
{
	if(ipstart == ipend)    return 0;
	if(ipstart > ipend)
	{
	    new IP:temp = ipend;
	    ipend = ipstart;
	    ipstart = temp;
	}

	new bantime = gettime();
	new unbantime = bantime+second;
	if(second == -1)	unbantime = cellmax;
	
	return ormCacheDestroy(ormInsertMaster(BanGroupIPTable,
		"IPStart", _:ipstart,
		"IPEnd", _:ipstart,
		"Admin", admin,
		"Reason", reason,
		"UnixTimestampBan", bantime,
		"UnixTimestampUnBan", unbantime
	));
	return 1;
}

stock GetBanAccountInfo(name[], admin[], reason[], &IP:ip, &bantime, &unbantime, admin_size = sizeof(admin), reason_size = sizeof(reason))
{
	if(!strlen(name))   return 0;
	format(secbuffer, sizeof(secbuffer), "Name = '%s'", name);
	new ORMResult:result = ormSelect(BanAccountTable, secbuffer);
	if(!ormResultNumRows(result))
	{
	    ormResultClear(result);
	    ormResultDelete(result);
	    return 0;
	}
	
	ormGetFieldAssoc(result, "Admin", admin, admin_size);
	ormGetFieldAssoc(result, "Reason", reason, reason_size);
	ip = IP:ormGetFieldAssoc(result, "IP");
	bantime = ormGetFieldAssoc(result, "UnixTimestampBan");
	unbantime = ormGetFieldAssoc(result, "UnixTimestampUnBan");
	
    ormResultClear(result);
    ormResultDelete(result);
	return 1;
}

stock GetBanIPInfo(IP:ip, admin[], reason[], account[], &bantime, &unbantime, admin_size = sizeof(admin), reason_size = sizeof(reason),
account_size = sizeof(account))
{
	format(secbuffer, sizeof(secbuffer), "IP = %i", _:ip);
	new ORMResult:result = ormSelect(BanAccountTable, secbuffer);
	if(!ormResultNumRows(result))
	{
	    ormResultClear(result);
	    ormResultDelete(result);
	    
	    result = ormSelect(BanAccountTable, secbuffer);
		if(!ormResultNumRows(result))
		{
	    	ormResultClear(result);
	    	ormResultDelete(result);
	    	return 0;
		}
		ormGetFieldAssoc(result, "Name", account, account_size);
	}

	ormGetFieldAssoc(result, "Admin", admin, admin_size);
	ormGetFieldAssoc(result, "Reason", reason, reason_size);
	ip = IP:ormGetFieldAssoc(result, "IP");
	bantime = ormGetFieldAssoc(result, "UnixTimestampBan");
	unbantime = ormGetFieldAssoc(result, "UnixTimestampUnBan");

    ormResultClear(result);
    ormResultDelete(result);
	return 1;
}

stock UnBanAccount(name[])
{
	if(!strlen(name))   return 0;
	format(secbuffer, sizeof(secbuffer), "Name = '%s'", name);
	new ORMResult:result = ormSelect(BanAccountTable, secbuffer);
	if(!ormResultNumRows(result))
	{
	    ormResultClear(result);
	    ormResultDelete(result);
	    return 0;
	}
	
	IPToString(secbuffer, IP:ormGetFieldAssocInt(result, "IP"));
	format(secbuffer, sizeof(secbuffer), "unbanip %s", secbuffer);
	SendRconCommand(secbuffer);

    ormResultClear(result);
    ormResultDelete(result);
	return 1;
}

stock UnBanIP(IP:ip)
{
	format(secbuffer, sizeof(secbuffer), "IP = %i", _:ip);
	ormUpdate(BanAccountTable, secbuffer, true, "IP", _:IP_NONE);
	ormDelete(BanIPTable, secbuffer, true);
	
	IPToString(secbuffer, ip);
	format(secbuffer, sizeof(secbuffer), "unbanip %s", secbuffer);
	SendRconCommand(secbuffer);
	return 1;
}

stock PutPlayerInPrison(playerid)
{
	if(!IsPlayerConnected(playerid))    return 0;

    if(GetPlayerLobby(playerid) != prisonlobby)
    {
        PlayerEnterLobby(playerid, prisonlobby);
    }
    
	SetPlayerInterior(playerid, 3);
	SetPlayerPos(playerid, 193.31, 173.87, 1003.02);
	SetPlayerFacingAngle(playerid, 0.44);
	SetCameraBehindPlayer(playerid);
	return 1;
}

stock PlayerUnPrison(playerid)
{
	if(!IsPlayerConnected(playerid))    return 0;
	PlayerEnterLobby(playerid, prisonlobby);//Отправление на рандомный спавн
	return 1;
}

stock MuteAccount(name[], time)
{
    if(!strlen(name) || time < 0)   return 0;
    
	new playerid = FindPlayer(name);
	if(IsPlayerRegistered(playerid))
	{
	    new Item:player = itemGetByID(Player, playerid);
	    if(time == 0 && itemGetInt(player, "Muted"))
	    {
			PlayerSetInt(playerid, "MuteSec", 0);
			if(IsPlayerLogin(playerid))	itemSetInt(player, "Muted", 0);
	    }
	    else
	    {
			PlayerSetInt(playerid, "MuteSec", time);
			PlayerSetInt(playerid, "Mute", PlayerGetInt(playerid, "Mute")+1);
			if(IsPlayerLogin(playerid))	itemSetInt(player, "Muted", 1);
		}
	}

	format(secbuffer, sizeof(secbuffer), "Name = '%s'", name);
	new ORMResult:result = ormSelect(AccountTable, secbuffer);
	if(!ormResultNumRows(result))
	{
	    ormResultClear(result);
	    ormResultDelete(result);
	    return 0;
	}
	new mute = ormGetFieldAssocInt(result, "Mute");
	if(time != 0)	mute++;
    ormResultClear(result);
    ormResultDelete(result);
 	ormUpdate(AccountTable, secbuffer, true, "MuteSec", time, "Mute", mute);
	return 1;
}

stock PrisonAccount(name[], time)
{
    if(!strlen(name) || time < 0)   return 0;
    
	new playerid = FindPlayer(name);
	if(IsPlayerRegistered(playerid))
	{
	    new Item:player = itemGetByID(Player, playerid);
	    if(time == 0 && itemGetInt(player, "Prisoned"))
	    {
			PlayerSetInt(playerid, "PrisonSec", 0);
            if(IsPlayerLogin(playerid))
            {
				itemSetInt(player, "Prisoned", 0);
				PlayerUnPrison(playerid);
			}
		}
	    else
	    {
			PlayerSetInt(playerid, "PrisonSec", time);
			PlayerSetInt(playerid, "Prison", PlayerGetInt(playerid, "Prison")+1);
			if(IsPlayerLogin(playerid))
			{
				itemSetInt(player, "Prisoned", 1);
				PutPlayerInPrison(playerid);
			}
		}
	}

	format(secbuffer, sizeof(secbuffer), "Name = '%s'", name);
	new ORMResult:result = ormSelect(AccountTable, secbuffer);
	if(!ormResultNumRows(result))
	{
	    ormResultClear(result);
	    ormResultDelete(result);
	    return 0;
	}
	new prison = ormGetFieldAssocInt(result, "Prison");
	if(time != 0)	prison++;
    ormResultClear(result);
    ormResultDelete(result);
 	ormUpdate(AccountTable, secbuffer, true, "PrisonSec", time, "Prison", prison);
	return 1;
}

stock MutePlayer(playerid, time)
{
	return MuteAccount(PlayerName(playerid), time);
}

stock PrisonPlayer(playerid, time)
{
	return PrisonAccount(PlayerName(playerid), time);
}

stock IsPlayerFreezed(playerid)
{
	if(!IsPlayerConnected(playerid))    return 0;
	return itemGetInt(itemGetByID(Player, playerid), "Freezed");
}

stock FreezPlayer(playerid)
{
	if(!IsPlayerConnected(playerid))    return 0;
	new Item:player = itemGetByID(Player, playerid);
	if(itemGetInt(player, "Freezed"))   return 0;
	TogglePlayerControllable(playerid, 0);
	itemSetInt(player, "Freezed", 1);
	return 1;
}

stock UnFreezPlayer(playerid)
{
	if(!IsPlayerConnected(playerid))    return 0;
	new Item:player = itemGetByID(Player, playerid);
	if(!itemGetInt(player, "Freezed"))   return 0;
	TogglePlayerControllable(playerid, 1);
	itemSetInt(player, "Freezed", 0);
	return 1;
}

stock AutoUnBan()
{
	new time = gettime();
	
	new ORMResult:result = ormSelect(BanAccountTable, "", "id", "UnixTimestampUnBan", "IP");
	
	new num = ormResultNumRows(result);
	new IP:ip;
	for(new i = 0; i < num; i++, ormResultNext(result))
	{
	    if(time >= ormGetFieldInt(result, 1, i))
	    {
	        ip = IP:ormGetFieldInt(result, 2, i);
	        IPToString(secbuffer, ip);
	        format(secbuffer, sizeof(secbuffer), "unbanip %s", secbuffer);
	        SendRconCommand(secbuffer);
	        format(secbuffer, sizeof(secbuffer), "id = %i", ormGetFieldInt(result, 0, i));
	        ormDelete(BanAccountTable, secbuffer, true);
	    }
	}
	
	ormResultClear(result);
	ormResultDelete(result);
		
	return 1;
}

stock AutoRestart()
{
	SetTimer("AutoRestartTimer", 120000, 0);
	SendMessageToAll("m2mRestart");
	return 1;
}

forward AutoRestartTimer();
public AutoRestartTimer()
{
	SendMessageToAll("mRestart");
	new List:list = GetListPlayers();
	new Item:player;
	new ORMCache:cache;
	listForeach(playerid:list)
	{
	    player = itemGetByID(Player, playerid);
	    if(!itemGetInt(player, "Login"))    continue;
	    cache = ORMCache:itemGetInt(player, "Cache");
	    ormCacheSave(cache);
	}
	SendRconCommand("gmx");
	return 1;
}

forward SecurityTimer();
public SecurityTimer()
{
	new List:list = GetListPlayers();
	new Item:player;
	new second;
	new Float:x, Float:y, Float:z;
	
	new hour, mi;
	gettime(hour, mi);
	if(hour == AUTORESTART_HOUR && mi == 0 && autorestart == 0)
	{
	    AutoRestart();
	}
	
	listForeach(playerid:list)
	{
		player = itemGetByID(Player, playerid);
		if(itemGetInt(player, "Muted"))
		{
		    second = PlayerGetInt(playerid, "MuteSec")-1;
			if(second <= 0)
			{
			    second = 0;
			    itemSetInt(player, "Muted", 0);
				SendMessageToAll("mUnMuted", PlayerName(playerid));
			}
			PlayerSetInt(playerid, "MuteSec", second);
		}
		if(itemGetInt(player, "Prisoned"))
		{
		    second = PlayerGetInt(playerid, "PrisonSec")-1;
			if(second <= 0)
			{
			    second = 0;
			    itemSetInt(player, "Prisoned", 0);
			    PlayerUnPrison(playerid);
			    SendMessageToAll("mUnPrison", PlayerName(playerid));
			}
			else
			{
			    GetPlayerPos(playerid, x, y, z);

			    if(x < 190.0 || x > 197.0 || y < 170.0 || y > 178.0 || z < 1001.0 || z > 1008)
			    {
					PutPlayerInPrison(playerid);
			    }
			    if(GetPlayerInterior(playerid) != 3)
			    {
			        SetPlayerInterior(playerid, 3);
			    }
			    if(GetPlayerLobby(playerid) != prisonlobby)
			    {
					PlayerEnterLobby(playerid, prisonlobby);
			    }
			}
			PlayerSetInt(playerid, "PrisonSec", second);
		}
		/*
	    if(GetAFKTime(playerid) >= 900000 && !GetPlayerAdmin(playerid))//15 минут
	    {
	        KickPlayer(playerid);
        	SendMessageToAll("mAFKPlayerKick", PlayerName(playerid), playerid);
        	printf("Игрок %s был кикнут за нахождение АФК 15 минут", PlayerName(playerid));
		}
		*/
	}
	return 1;
}

















