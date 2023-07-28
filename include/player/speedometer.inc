#if defined inc_pl_spdmtr
	#endinput
#endif
#define inc_pl_spdmtr

static Text:KMH;
static timer;

public OnScriptInit()
{
    itemInfoAdd(Player, "SpeedometerState", 0);
	itemInfoAdd(Player, "SpeedometerTD");
	itemInfoAdd(Player, "SpeedometerTarget", -1);
	itemInfoAdd(Player, "SpeedometerTargeted", -1);

	KMH = TextDrawCreate(498.000000,97.000000,"KM/H:");
	TextDrawAlignment(KMH,0);
	TextDrawBackgroundColor(KMH,0x000000ff);
	TextDrawFont(KMH,1);
	TextDrawLetterSize(KMH,0.599999,2.599999);
	TextDrawColor(KMH,0x7777ffff);
	TextDrawSetOutline(KMH,1);
	TextDrawSetProportional(KMH,1);
	TextDrawSetShadow(KMH,1);
	
	timer = SetTimer("ATimer443ms", 443, 1);
	
#if defined pl_spdmtr_OnScriptInit
    pl_spdmtr_OnScriptInit();
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
#define OnScriptInit pl_spdmtr_OnScriptInit
#if defined pl_spdmtr_OnScriptInit
forward pl_spdmtr_OnScriptInit();
#endif

public OnScriptExit()
{
#if defined pl_spdmtr_OnScriptExit
    pl_spdmtr_OnScriptExit();
#endif
	new List:list = GetListPlayers();
	new Item:player;
	new temp;
	listForeach(playerid:list)
	{
	    player = itemGetByID(Player, playerid);
	    temp = itemGetInt(player, "SpeedometerTD");
	    PlayerTextDrawHide(playerid, PlayerText:temp);
	    PlayerTextDrawDestroy(playerid, PlayerText:temp);
	}
	TextDrawHideForAll(KMH);
	TextDrawDestroy(KMH);
	
	KillTimer(timer);
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
#define OnScriptExit pl_spdmtr_OnScriptExit
#if defined pl_spdmtr_OnScriptExit
forward pl_spdmtr_OnScriptExit();
#endif

public OnPlayerConnect(playerid)
{
	new PlayerText:vehspeed;
	vehspeed = CreatePlayerTextDraw(playerid,562.000000,96.000000,"_");
	PlayerTextDrawAlignment(playerid,vehspeed,0);
	PlayerTextDrawBackgroundColor(playerid,vehspeed,0x0000ff66);
	PlayerTextDrawFont(playerid,vehspeed,2);
	PlayerTextDrawLetterSize(playerid,vehspeed,0.699999,2.699999);
	PlayerTextDrawColor(playerid,vehspeed,0xffffffff);
	PlayerTextDrawSetOutline(playerid,vehspeed,1);
	PlayerTextDrawSetProportional(playerid,vehspeed,1);
	PlayerTextDrawSetShadow(playerid,vehspeed,1);
	
	itemSetInt(itemGetByID(Player, playerid), "SpeedometerTD", _:vehspeed);

#if defined pl_spdmtr_OnPlayerConnect
    pl_spdmtr_OnPlayerConnect(playerid);
#endif
	return 1;
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect pl_spdmtr_OnPlayerConnect
#if defined pl_spdmtr_OnPlayerConnect
forward pl_spdmtr_OnPlayerConnect(playerid);
#endif

public OnPlayerDisconnect(playerid, reason)
{
#if defined pl_spdmtr_OnPlayerDisconnect
    pl_spdmtr_OnPlayerDisconnect(playerid, reason);
#endif
    new PlayerText:vehspeed;

	new Item:player = itemGetByID(Player, playerid);
	new targetedid = itemGetInt(player, "SpeedometerTargeted");
	if(IsPlayerConnected(targetedid))
	{
	    new Item:targeted = itemGetByID(Player, targetedid);
	    itemSetInt(targeted, "SpeedometerTarget", -1);
	    itemSetInt(targeted, "SpeedometerState", 0);
	    itemSetInt(player, "SpeedometerTargeted", -1);
	    
	    TextDrawHideForPlayer(targetedid, KMH);
		PlayerTextDrawHide(targetedid, PlayerText:itemGetInt(targeted, "SpeedometerTD"));
	}

	vehspeed = PlayerText:itemGetInt(player, "SpeedometerTD");

    TextDrawHideForPlayer(playerid, KMH);
	PlayerTextDrawHide(playerid, vehspeed);
    PlayerTextDrawDestroy(playerid, vehspeed);
	return 1;
}
#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect pl_spdmtr_OnPlayerDisconnect
#if defined pl_spdmtr_OnPlayerDisconnect
forward pl_spdmtr_OnPlayerDisconnect(playerid, reason);
#endif

stock VehicleSpeedometerEnable(playerid, targetid)
{
	if(!IsPlayerConnected(playerid) || !IsPlayerConnected(targetid))    return 0;
	itemSetInt(itemGetByID(Player, playerid), "SpeedometerTarget", targetid);
	itemSetInt(itemGetByID(Player, targetid), "SpeedometerTargeted", playerid);
	return 1;
}

stock VehicleSpeedometerDisable(playerid)
{
	if(!IsPlayerConnected(playerid))	return 0;
	new Item:player = itemGetByID(Player, playerid);
	new targetid = itemGetInt(player, "SpeedometerTarget");
 	itemSetInt(player, "SpeedometerTarget", -1);
 	itemSetInt(itemGetByID(Player, targetid), "SpeedometerTargeted", -1);
	return 1;
}

forward ATimer443ms();
public ATimer443ms()
{
    static Float:vx, Float:vy, Float:vz;
	static vehicleid;
    static string[16];
    
    static List:list;
    static targetid;
    static statemode;
    static Item:player;
    static PlayerText:vehspeed;
    
    list = GetListPlayers();
    
    listForeach(playerid:list)
    {
        player = itemGetByID(Player, playerid);
        targetid = itemGetInt(player, "SpeedometerTarget");
        if(!IsPlayerConnected(targetid))    continue;
        
        statemode = itemGetInt(player, "SpeedometerState");
        vehspeed = PlayerText:itemGetInt(player, "SpeedometerTD");
        
        vehicleid = GetPlayerVehicleID(targetid);
		if(!IsValidVehicle(vehicleid))
		{
		    if(statemode)
			{
			    itemSetInt(player, "SpeedometerState", 0);
			    PlayerTextDrawHide(playerid, vehspeed);
			    TextDrawHideForPlayer(playerid, KMH);
			}
		}
		else
		{
		    if(!statemode)
			{
			    itemSetInt(player, "SpeedometerState", 1);
			    PlayerTextDrawShow(playerid, vehspeed);
			    TextDrawShowForPlayer(playerid, KMH);
			}
		    GetVehicleVelocity(vehicleid, vx, vy, vz);
			format(string, sizeof(string), "%i", floatround(VectorSize(vx, vy, vz)*187.0));
			PlayerTextDrawSetString(playerid, vehspeed, string);
		}
    }
	return 1;
}

















































































