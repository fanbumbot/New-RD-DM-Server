#if defined inc_vehicle
	#endinput
#endif
#define inc_vehicle

stock VehicleEnterLobby(vehicleid, Lobby:lobby)
{
	if(!IsValidVehicle(vehicleid) || !lobbyIsExist(lobby))    return 0;
	new Item:vehicle = itemGetByID(Vehicle, vehicleid);
    lobbyEnterItem(lobby, "Vehicle", vehicle);
    new vw = lobbyGetVirtualWorld(lobby);
    SetVehicleVirtualWorld(vehicleid, vw);
	return 1;
}

stock World:GetVehicleWorld(vehicleid)
{
    if(!IsValidVehicle(vehicleid))    return World:0;
    new Item:vehicle = itemGetByID(Vehicle, vehicleid);
    new Lobby:lobby = Lobby:itemGetInt(vehicle, "Lobby");
    if(!lobbyIsExist(lobby))    return World:0;
    new World:world = lobbyGetWorld(lobby);
    if(!worldIsExist(world))    return World:0;
    return world;
}

stock Lobby:GetVehicleLobby(vehicleid)
{
    if(!IsPlayerConnected(playerid))    return Lobby:0;
    new Item:vehicle = itemGetByID(Vehicle, vehicleid);
    new Lobby:lobby = Lobby:itemGetInt(vehicle, "Lobby");
    if(!lobbyIsExist(lobby))    return Lobby:0;
    return lobby;
}


