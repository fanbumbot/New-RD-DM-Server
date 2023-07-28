#if defined mdg_world_inc
	#endinput
#endif
#define mdg_world_inc

#include <a_samp>
#include <streamer>

#include <MS/item>

/*
	World
	    struct:
	        Item:
		        list of lobbys

		        general timer - returned from SetTimer/SetTimerEx
		        function name on timer
		        time on repeat timer

				world item info
				lobby item info
				lobby lists item info
				
				world item
	Lobby
		struct:
		    Item:
				world
		        virtual world
		        
		        lobby item
		        lobby lists (item)
*/

forward OnItemEnterLobby(Item:item, World:world, Lobby:lobby);
forward OnItemExitLobby(Item:item, World:world, Lobby:lobby);

new ItemInfo:World;
new ItemInfo:Lobby;

static Map:Worlds;

static firstemptyvw;

stock worldSystemInit()
{
    firstemptyvw = 0;
    
    Worlds = mapCreate();

    World = itemInfoCreate();
    itemInfoAdd(World, "Lobbys");
    itemInfoAdd(World, "Timer");
    itemInfoAdd(World, "FuncTimer");
    itemInfoAdd(World, "TimeRepeat");
 	itemInfoAdd(World, "WorldItemInfo");
 	itemInfoAdd(World, "LobbyItemInfo");
 	itemInfoAdd(World, "LobbyListsItemInfo");
 	itemInfoAdd(World, "WorldItem");
    
    Lobby = itemInfoCreate();
    itemInfoAdd(Lobby, "World");
    itemInfoAdd(Lobby, "VirtualWorld");
    itemInfoAdd(Lobby, "LobbyItem");
    itemInfoAdd(Lobby, "LobbyLists");
	return 1;
}

stock worldSystemExit()
{
	mapDestroy(Worlds);
	itemInfoDestroy(World);
	itemInfoDestroy(Lobby);
	return 1;
}

stock worldIsExist(World:world)
{
	return itemIsValid(Item:world);
}

stock lobbyInfoAdd(World:world, name[])
{
	if(!worldIsExist(world) || !strlen(name))   return 0;
    new ItemInfo:iteminfo = ItemInfo:itemGetInt(Item:world, "LobbyItemInfo");
    itemInfoAdd(iteminfo, name);
	return 1;
}

stock worldInfoAdd(World:world, name[])
{
    if(!worldIsExist(world) || !strlen(name))   return 0;
    new ItemInfo:iteminfo = ItemInfo:itemGetInt(Item:world, "WorldItemInfo");
    itemInfoAdd(iteminfo, name);
	return 1;
}

stock lobbyGroupAdd(World:world, group[])
{
    if(!worldIsExist(world) || !strlen(group))   return 0;
    new ItemInfo:iteminfo = ItemInfo:itemGetInt(Item:world, "LobbyListsItemInfo");
    itemInfoAdd(iteminfo, group);
	return 1;
}

stock List:lobbyGroupGet(Lobby:lobby, group[])
{
    if(!lobbyIsExist(lobby) || !strlen(group))   return List:0;
	new Item:lobbylists = itemGetItem(Item:lobby, "LobbyLists");
	return List:itemGetInt(lobbylists, group);
}

stock World:worldCreate(name[])
{
	if(mapIsKeyExist(Worlds, name))    return World:0;

	new Item:world = itemPush(World);
	itemSetInt(world, "Lobbys", _:listCreate());
	itemSetInt(world, "Timer", 0);
	itemSetInt(world, "FuncTimer", 0);
 	itemSetInt(world, "TimeRepeat", 0);
 	
 	new ItemInfo:iteminfo = itemInfoCreate();
 	itemSetInt(world, "WorldItemInfo", _:iteminfo);
 	itemSetInt(world, "LobbyItemInfo", _:itemInfoCreate());
 	itemSetInt(world, "LobbyListsItemInfo", _:itemInfoCreate());
 	itemSetItem(world, "WorldItem", itemPush(iteminfo));
 	
 	mapAdd(Worlds, name, _:world);
 	
	return World:world;
}

stock worldDestroy(World:world)
{
	if(!worldIsExist(world))    return 0;

    new List:list = List:itemGetInt(Item:world, "Lobbys");
    listDestroy(list);
    
    new ItemInfo:iteminfo;
	iteminfo = ItemInfo:itemGetInt(Item:world, "LobbyItemInfo");
	itemInfoDestroy(iteminfo);//Автоудаление лобби
	iteminfo = ItemInfo:itemGetInt(Item:world, "LobbyListsItemInfo");
	itemInfoDestroy(iteminfo);//Автоудаление списков лобби
	iteminfo = ItemInfo:itemGetInt(Item:world, "WorldItemInfo");
	itemInfoDestroy(iteminfo);//Автоудаление мира
	return 1;
}

stock World:worldGetByName(name[])
{
	new World:world;
	mapGet(Worlds, name, _:world);
	return world;
}

stock worldSetTimer(World:world, funcname[], time)
{
	if(!worldIsExist(world) || !strlen(funcname) || time == 0)  return 0;
	
	itemSetInt(Item:world, "Timer", SetTimer(funcname, time, 1));
	itemSet(Item:world, "FuncTimer", funcname);
 	itemSetInt(Item:world, "TimeRepeat", time);
	return 1;
}

stock worldKillTimer(World:world)
{
    if(!worldIsExist(world))    return 0;
	new timer = itemGetInt(Item:world, "Timer");
	return KillTimer(timer);
}

stock worldGetSize(World:world)
{
	if(!worldIsExist(world))    return 0;
	return listLen(List:itemGetInt(Item:world, "Lobbys"));
}

stock worldLobbySize(World:world)
{
    if(!worldIsExist(world))    return 0;
    return listLen(List:itemGetInt(Item:world, "Lobbys"));
}

stock List:worldGetLobbyList(World:world)
{
	if(!worldIsExist(world))	return List:0;
	return List:itemGetInt(Item:world, "Lobbys");
}

stock lobbyIsExist(Lobby:lobby)
{
	return itemIsValid(Item:lobby);
}

stock Lobby:lobbyCreate(World:world)
{
	if(!worldIsExist(world))    return Lobby:0;
	
	new List:list = List:itemGetInt(Item:world, "Lobbys");
	
	new Item:lobby = itemPush(Lobby);
	itemSetInt(lobby, "World", _:world);
	itemSetInt(lobby, "VirtualWorld", firstemptyvw);
	itemSetItem(lobby, "LobbyItem", itemPush(ItemInfo:itemGetInt(Item:world, "LobbyItemInfo")));
	itemSetItem(lobby, "LobbyLists", itemPush(ItemInfo:itemGetInt(Item:world, "LobbyListsItemInfo")));
	
	firstemptyvw++;
	
	listPush(list, _:lobby);
	
	return Lobby:lobby;
}

stock lobbyCreateArray(World:world, count)
{
    if(!worldIsExist(world) || count < 0)    return 0;
    for(new i = 0; i < count; i++)
    {
        lobbyCreate(world);
    }
	return 1;
}

stock lobbyDestroy(Lobby:lobby)
{
	if(!lobbyIsExist(lobby))    return 0;
	
	new List:templist;
	
	new Item:item = Item:itemGetInt(Item:lobby, "LobbyItem");
	itemDestroy(item);
	new Item:world = Item:itemGetInt(Item:lobby, "World");
	new Item:item = Item:itemGetInt(Item:lobby, "LobbyLists");
	new Vector:vector = itemGetVector(Item:lobby);
	vectorForeachPtr(ptr:vector)
	{
	    templist = List:MEM_get_val(ptr, 1);
	    if(MEM_get_val(ptr, 0) == 1)    continue;//if item
    	listDestroy(templist);
	}

	list = List:itemGetInt(world, "Lobbys");
	listDeleteValue(list, Pointer:lobby, false);
	
	itemDestroy(Item:lobby);
	return 1;
}

stock World:lobbyGetWorld(Lobby:lobby)
{
	if(!lobbyIsExist(lobby))    return World:0;
    return World:itemGetInt(Item:lobby, "World");
}

stock lobbyGetVirtualWorld(Lobby:lobby)
{
	if(!lobbyIsExist(lobby))    return -1;
    return itemGetInt(Item:lobby, "VirtualWorld");
}

stock Lobby:lobbyGetByID(World:world, index)
{
    if(!worldIsExist(world))    return Lobby:0;
    new List:list = List:itemGetInt(Item:world, "Lobbys");
    
    new Lobby:lobby;
    if(!listGet(list, index, _:lobby))	return Lobby:0;
    return lobby;
}

stock lobbyEnterItem(Lobby:lobby, group[], Item:item)
{
    if(!lobbyIsExist(lobby) || !itemIsValid(item) || !strlen(group))    return 0;
    
    new Item:lobbylists = Item:itemGetInt(Item:lobby, "LobbyLists");
	new List:list = List:itemGetInt(lobbylists, group);
	listPush(list, _:item);

    new Lobby:lastlobby = Lobby:itemGetItem(item, "Lobby");
    if(lobbyIsExist(lastlobby))
    {
    	CallLocalFunction("OnItemExitLobby", "iii", _:item, _:lobbyGetWorld(lastlobby), _:lastlobby);
    }
    else
    {
        lastlobby = Lobby:0;
    }
    
    
    itemInfoAdd(itemGetItemInfo(item), "Lobby");
    itemInfoAdd(itemGetItemInfo(item), "LastLobby");
    itemSetInt(item, "Lobby", _:lobby);
    itemSetInt(item, "LastLobby", _:lastlobby);
	CallLocalFunction("OnItemEnterLobby", "iii", _:item, _:lobbyGetWorld(lobby), _:lobby);
    
	return 1;
}

public OnScriptInit()
{
	worldSystemInit();
#if defined mdg_world_OnScriptInit
    mdg_world_OnScriptInit();
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
#define OnScriptInit mdg_world_OnScriptInit
#if defined mdg_world_OnScriptInit
forward mdg_world_OnScriptInit();
#endif

public OnScriptExit()
{
#if defined mdg_world_OnScriptExit
    mdg_world_OnScriptExit();
#endif
    worldSystemExit();
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
#define OnScriptExit mdg_world_OnScriptExit
#if defined mdg_world_OnScriptExit
forward mdg_world_OnScriptExit();
#endif
