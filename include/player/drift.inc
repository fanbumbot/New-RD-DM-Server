#if defined inc_pl_drift
	#endinput
#endif
#define inc_pl_drift

#if !defined DRIFT_CANCELLATION_TIME
	#define DRIFT_CANCELLATION_TIME 2000
#endif
#if !defined DRIFT_MINKAT
	#define DRIFT_MINKAT 5.0
#endif
#if !defined DRIFT_MAXKAT
	#define DRIFT_MAXKAT 120.0
#endif
#if !defined DRIFT_SPEED
	#define DRIFT_SPEED 0.15
#endif

static drifttimer;

public OnScriptInit()
{
    itemInfoAdd(Player, "Drifting", 1);
    itemInfoAdd(Player, "DriftScore", 0);
    itemInfoAdd(Player, "DriftCancelTimer", 0);

    drifttimer = SetTimer("DriftTimer", 300, 1);

#if defined pl_drift_OnScriptInit
    pl_drift_OnScriptInit();
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
#define OnScriptInit pl_drift_OnScriptInit
#if defined pl_drift_OnScriptInit
forward pl_drift_OnScriptInit();
#endif

public OnScriptExit()
{
#if defined pl_drift_OnScriptExit
    pl_drift_OnScriptExit();
#endif
    KillTimer(drifttimer);
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
#define OnScriptExit pl_drift_OnScriptExit
#if defined pl_drift_OnScriptExit
forward pl_drift_OnScriptExit();
#endif

stock IsCar(model)
{
    switch(model)
    {
        case 446,432,448,452,424,453,454,461,462,463,468,471,430,472,449,473,481,
		484,493,495,509, 510,521,538,522,523,532,537,570,581,586,590,569,595,604,611: return 0;
    }
    return 1;
}

stock StopDrift(playerid)
{
	if(!IsPlayerConnected(playerid))    return 0;
	
	new Item:player = itemGetByID(Player, playerid);
	
	KillTimer(itemGetInt(player, "DriftCancelTimer"));
	itemSetInt(player, "DriftCancelTimer", 0)
	itemSetInt(player, "DriftScore", 0);
	
	return 1;
}

stock PlayerDriftEnable(playerid)
{
	if(!IsPlayerConnected(playerid))    return 0;
	itemSetInt(itemGetByID(Player, playerid), "Drifting", 1);
	return 1;
}

stock PlayerDriftDisable(playerid)
{
	if(!IsPlayerConnected(playerid) || !IsPlayerAutoRepair(playerid))   return 0;
	itemSetInt(itemGetByID(Player, playerid), "Drifting", 0);
	return 1;
}

forward DriftTimer();
public DriftTimer()
{
	static Item:player;
	static List:list;
	static vehicleid;
	static score;
	static timer;

	static Float:ang;
	static Float:velocityang;
	static Float:velocity[4];
	static Float:raznost;
	
	static string[64];
	
	list = GetListPlayers();

	listForeach(playerid:list)
	{
	    player = itemGetByID(Player, playerid);
	    if(!itemGetInt(player, "Drifting")) continue;
	    if(GetPlayerVehicleSeat(playerid) != 0) continue;
	    vehicleid = GetPlayerVehicleID(playerid);
	    if(!IsValidVehicle(vehicleid))  continue;
		if(!IsCar(GetVehicleModel(vehicleid)))  continue;

	    score = itemGetInt(player, "DriftScore");
	    
		GetVehicleZAngle(vehicleid, ang);

		GetVehicleVelocity(vehicleid, velocity[0], velocity[1], velocity[2]);
		velocity[3] = VectorSize(velocity[0], velocity[1], 0.0);

		//Вычисление угла скорости транспорта игрока
	    if(velocity[3] > 0.001)	velocityang = acos(velocity[0]/velocity[3]);
	        else    velocityang = ang;

		//Синхронизация угла в 0 градусов у угла скорости с углом поворота
	    if(velocity[1] < 0.0)	velocityang = 360 - velocityang;
		velocityang -= 90.0;
		if(velocityang < 0.0)   velocityang = 360 + velocityang;

		raznost = floatabs(ang - velocityang);
		if(raznost > 180.0)	raznost = 360-raznost;

		//Если разница в углах скорости и поворота больше определённого значения
		if(DRIFT_MINKAT < raznost < DRIFT_MAXKAT && velocity[3] > DRIFT_SPEED)
		{
		    //Игрок дрифтит
	     	score += floatround((floatabs(velocityang-ang)*velocity[3]*3.6),floatround_tozero)/10;
	     	itemSetInt(player, "DriftScore", score);
			format(string, sizeof(string), "~n~~n~~n~~n~~n~~n~~n~~n~~w~Drifting Score:~b~ %i~w~ !", score);
			GameTextForPlayer(playerid, string, 3000, 3);

			//Обновление таймера отмены дрифта
			timer = itemGetInt(player, "DriftCancelTimer");
			if(timer > 0)
			{
				KillTimer(timer);
			}
			itemSetInt(player, "DriftCancelTimer", SetTimerEx("DriftCancellationTimer", DRIFT_CANCELLATION_TIME, 0, "i", playerid));
		}

	}
	return 1;
}

forward DriftCancellationTimer(playerid);
public DriftCancellationTimer(playerid)
{
	static string[64];

    if(!IsPlayerConnected(playerid))    return 0;

	new Item:player = itemGetByID(Player, playerid);
	itemSetInt(player, "DriftCancelTimer", 0);
	new score = itemGetInt(player, "DriftScore");
	PlayerSetInt(playerid, "Money", PlayerGetInt(playerid, "Money")+score);
	itemSetInt(player, "DriftScore", 0);
	format(string, sizeof(string), "~n~~n~~n~~n~~n~~n~~n~~n~~w~~w~Drifting Bonus:~b~ %i~w~ !", score);
	GameTextForPlayer(playerid, string, 3000, 3);
	return 1;
}
