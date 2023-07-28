#if defined mdg_veh_inc
	#endinput
#endif
#define mdg_veh_inc

#if !defined IsValidVehicle
	native IsValidVehicle(vehicleid);
#endif

new ItemInfo:Vehicle;
static List:allvehicles;

public OnScriptInit()
{
    Vehicle = itemInfoCreate();
    allvehicles = listCreate();
    
#if defined mdg_veh_OnScriptInit
    mdg_veh_OnScriptInit();
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
#define OnScriptInit mdg_veh_OnScriptInit
#if defined mdg_veh_OnScriptInit
forward mdg_veh_OnScriptInit();
#endif

public OnScriptExit()
{
#if defined mdg_veh_OnScriptExit
    mdg_veh_OnScriptExit();
#endif
    itemInfoDestroy(Vehicle);
	listForeach(vehicleid:allvehicles)
	{
	    DestroyVehicle(vehicleid);
	}
	listDestroy(allvehicles);
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
#define OnScriptExit mdg_veh_OnScriptExit
#if defined mdg_veh_OnScriptExit
forward mdg_veh_OnScriptExit();
#endif

stock mdg_veh_CreateVehicle(vehicletype, Float:x, Float:y, Float:z, Float:rotation, color1, color2, respawn_delay, addsiren=0)
{
	new vehicleid = CreateVehicle(vehicletype, x, y, z, rotation, color1, color2, respawn_delay, addsiren);
	if(IsValidVehicle(vehicleid))
	{
	    listPush(allvehicles, vehicleid);
		itemCreate(Vehicle, vehicleid);
	}
	return vehicleid;
}
#if defined _ALS_CreateVehicle
	#undef CreateVehicle
#else
	#define _ALS_CreateVehicle
#endif
#define CreateVehicle mdg_veh_CreateVehicle

stock mdg_veh_AddStaticVehicle(modelid, Float:spawn_x, Float:spawn_y, Float:spawn_z, Float:z_angle, color1, color2)
{
	new vehicleid = AddStaticVehicle(modelid, spawn_x, spawn_y, spawn_z, z_angle, color1, color2);
	if(IsValidVehicle(vehicleid))
	{
	    listPush(allvehicles, vehicleid);
		itemCreate(Vehicle, vehicleid);
	}
	return vehicleid;
}
#if defined _ALS_AddStaticVehicle
	#undef AddStaticVehicle
#else
	#define _ALS_AddStaticVehicle
#endif
#define AddStaticVehicle mdg_veh_AddStaticVehicle

stock mdg_veh_AddStaticVehicleEx(modelid, Float:spawn_x, Float:spawn_y, Float:spawn_z, Float:z_angle, color1, color2, respawn_delay, addsiren=0)
{
	new vehicleid = AddStaticVehicle(modelid, spawn_x, spawn_y, spawn_z, z_angle, color1, color2, respawn_delay, addsiren);
	if(IsValidVehicle(vehicleid))
	{
	    listPush(allvehicles, vehicleid);
		itemCreate(Vehicle, vehicleid);
	}
	return vehicleid;
}
#if defined _ALS_AddStaticVehicleEx
	#undef AddStaticVehicleEx
#else
	#define _ALS_AddStaticVehicleEx
#endif
#define AddStaticVehicleEx mdg_veh_AddStaticVehicleEx

stock mdg_veh_DestroyVehicle(vehicleid)
{
    listDeleteValue(allvehicles, vehicleid, false);
	itemDestroyByID(Vehicle, vehicleid);
	return DestroyVehicle(vehicleid);
}
#if defined _ALS_DestroyVehicle
	#undef DestroyVehicle
#else
	#define _ALS_DestroyVehicle
#endif
#define DestroyVehicle mdg_veh_DestroyVehicle

stock List:GetListVehicles()   return allvehicles;



