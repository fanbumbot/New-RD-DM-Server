#if defined inc_vehneon
	#endinput
#endif
#define inc_vehneon

enum Neon
{
    NEON_NONE,
	NEON_RED,
	NEON_BLUE,
	NEON_GREEN,
	NEON_YELLOW,
	NEON_PINK,
	NEON_WHITE
}

public OnScriptInit()
{
	itemInfoAdd(Vehicle, "NeonObj1", -1);
	itemInfoAdd(Vehicle, "NeonObj2", -1);

#if defined vehneon_OnScriptInit
    vehneon_OnScriptInit();
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
#define OnScriptInit vehneon_OnScriptInit
#if defined vehneon_OnScriptInit
forward vehneon_OnScriptInit();
#endif

stock GetNeonModelByType(Neon:neontype)
{
	if(neontype == NEON_NONE)   return 0;
	return _:neontype + 18646;
}

stock IsVehicleNeonExist(vehicleid)
{
	if(!IsValidVehicle(vehicleid))  return 0;
	new vehicle = itemGetByID(Vehicle, vehicleid);
	if(IsValidDynamicObject(itemGetInt(vehicle, "NeonObj1")) || IsValidDynamicObject(itemGetInt(vehicle, "NeonObj2")))
	{
	    return 1;
	}
	return 0;
}

stock RemoveVehicleNeon(vehicleid)
{
	if(!IsValidVehicle(vehicleid))  return 0;
	new Item:vehicle = itemGetByID(Vehicle, vehicleid);
	new neon1 = itemGetInt(vehicle, "NeonObj1");
	new neon2 = itemGetInt(vehicle, "NeonObj2");
    if(IsValidDynamicObject(neon1))
    {
    	DestroyDynamicObject(neon1);
    	itemSetInt(vehicle, "NeonObj1", 0);
	}
    if(IsValidDynamicObject(neon2))
    {
    	DestroyDynamicObject(neon2);
    	itemSetInt(vehicle, "NeonObj2", 0);
	}
	return 1;
}

stock AddVehicleNeon(vehicleid, Neon:type)
{
	if(!IsValidVehicle(vehicleid))  return 0;
	
    new modelid = GetNeonModelByType(type);
    if(modelid == 0)    return 0;
	
	new Item:vehicle = itemGetByID(Vehicle, vehicleid);
	new neon1 = itemGetInt(vehicle, "NeonObj1");
	new neon2 = itemGetInt(vehicle, "NeonObj2");
    if(IsValidDynamicObject(neon1))
    {
    	DestroyDynamicObject(neon1);
    	itemSetInt(vehicle, "NeonObj1", 0);
	}
    if(IsValidDynamicObject(neon2))
    {
    	DestroyDynamicObject(neon2);
    	itemSetInt(vehicle, "NeonObj2", 0);
	}

	neon1 = CreateDynamicObject(modelid, 0.0,0.0,0.0,0.0,0.0,0.0, .drawdistance = 100.0);
	neon2 = CreateDynamicObject(modelid, 0.0,0.0,0.0,0.0,0.0,0.0, .drawdistance = 100.0);

	if(!IsValidDynamicObject(neon1) || !IsValidDynamicObject(neon2))    return 0;

	AttachDynamicObjectToVehicle(neon1, vehicleid, -0.8, 0.0, -0.70, 0.0, 0.0, 0.0);
	AttachDynamicObjectToVehicle(neon2, vehicleid, 0.8, 0.0, -0.70, 0.0, 0.0, 0.0);
	
	itemSetInt(vehicle, "NeonObj1", neon1);
	itemSetInt(vehicle, "NeonObj2", neon2);
	return 1;
}

stock vehneon_DestroyVehicle(vehicleid)
{
    RemoveVehicleNeon(vehicleid);
	return DestroyVehicle(vehicleid);
}
#if defined _ALS_DestroyVehicle
	#undef DestroyVehicle
#else
	#define _ALS_DestroyVehicle
#endif
#define DestroyVehicle vehneon_DestroyVehicle

