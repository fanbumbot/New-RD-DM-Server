#if defined inc_veh_u
	#endinput
#endif
#define inc_veh_u

enum Tire(<<=1)
{
	TireBackRight = 0,
	TireFrontRight,
	TireBackLeft,
	TireFrontLeft
}

stock IsVehicleTireValid(Tire:tire)
{
	return tire & 0b1111 != 0 ? 0 : 1;
}

stock AddVehicleHydraulic(vehicleid)
{
	if(!IsValidVehicle(vehicleid))  return 0;
    return AddVehicleComponent(vehicleid, 1087);
}

stock RemoveVehicleHydraulic(vehicleid)
{
	if(!IsValidVehicle(vehicleid))  return 0;
	new comp = GetVehicleComponentInSlot(vehicleid, 9);
	if(comp == 0)    return 0;
    return RemoveVehicleComponent(vehicleid, comp);
}

stock VehicleTire(vehicleid, Tire:tire, {bool, _}:enable)
{
	if(!IsValidVehicle(vehicleid) || !IsValidVehicleTire(tire))  return 0;
	static panels, doors, lights, Tire:tires, temp;
	GetVehicleDamageStatus(vehicleid, panels, doors, lights, temp);
	if(enable)
	{
		tires = Tire:((temp | _:tire) & 0b1111);
	}
	else
	{
	    tires = Tire:((temp & ~(_:tire)) & 0b1111);
	}
	SetVehicleDamageStatus(vehicleid, panels, doors, lights, _:tires);
	return 1;
}

stock VehicleEngineEnable(vehicleid)
{
    if(!IsValidVehicle(vehicleid))  return 0;
    static engine, lights, alarm, doors, bonnet, boot, objective;
    GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
    SetVehicleParamsEx(vehicleid, 1, lights, alarm, doors, bonnet, boot, objective);
    return 1;
}

stock VehicleEngineDisable(vehicleid)
{
    if(!IsValidVehicle(vehicleid))  return 0;
    static engine, lights, alarm, doors, bonnet, boot, objective;
    GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
    SetVehicleParamsEx(vehicleid, 0, lights, alarm, doors, bonnet, boot, objective);
    return 1;
}

stock VehicleLightsEnable(vehicleid)
{
    if(!IsValidVehicle(vehicleid))  return 0;
    static engine, lights, alarm, doors, bonnet, boot, objective;
    GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
    SetVehicleParamsEx(vehicleid, engine, 1, alarm, doors, bonnet, boot, objective);
    return 1;
}

stock VehicleLightsDisable(vehicleid)
{
    if(!IsValidVehicle(vehicleid))  return 0;
    static engine, lights, alarm, doors, bonnet, boot, objective;
    GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
    SetVehicleParamsEx(vehicleid, engine, 0, alarm, doors, bonnet, boot, objective);
    return 1;
}

stock VehicleAlarmEnable(vehicleid)
{
    if(!IsValidVehicle(vehicleid))  return 0;
    static engine, lights, alarm, doors, bonnet, boot, objective;
    GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
    SetVehicleParamsEx(vehicleid, engine, lights, 1, doors, bonnet, boot, objective);
    return 1;
}

stock VehicleAlarmDisable(vehicleid)
{
    if(!IsValidVehicle(vehicleid))  return 0;
    static engine, lights, alarm, doors, bonnet, boot, objective;
    GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
    SetVehicleParamsEx(vehicleid, engine, lights, 0, doors, bonnet, boot, objective);
    return 1;
}

stock VehicleDoorsOpen(vehicleid)
{
    if(!IsValidVehicle(vehicleid))  return 0;
    static engine, lights, alarm, doors, bonnet, boot, objective;
    GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
    SetVehicleParamsEx(vehicleid, engine, lights, alarm, 1, bonnet, boot, objective);
    return 1;
}

stock VehicleDoorsClose(vehicleid)
{
    if(!IsValidVehicle(vehicleid))  return 0;
    static engine, lights, alarm, doors, bonnet, boot, objective;
    GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
    SetVehicleParamsEx(vehicleid, engine, lights, alarm, 0, bonnet, boot, objective);
    return 1;
}

stock VehicleBonnetOpen(vehicleid)
{
    if(!IsValidVehicle(vehicleid))  return 0;
    static engine, lights, alarm, doors, bonnet, boot, objective;
    GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
    SetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, 1, boot, objective);
    return 1;
}

stock VehicleBonnetClose(vehicleid)
{
    if(!IsValidVehicle(vehicleid))  return 0;
    static engine, lights, alarm, doors, bonnet, boot, objective;
    GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
    SetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, 0, boot, objective);
    return 1;
}

stock VehicleBootOpen(vehicleid)
{
    if(!IsValidVehicle(vehicleid))  return 0;
    static engine, lights, alarm, doors, bonnet, boot, objective;
    GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
    SetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, 1, objective);
    return 1;
}

stock VehicleBootClose(vehicleid)
{
    if(!IsValidVehicle(vehicleid))  return 0;
    static engine, lights, alarm, doors, bonnet, boot, objective;
    GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
    SetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, 0, objective);
    return 1;
}

