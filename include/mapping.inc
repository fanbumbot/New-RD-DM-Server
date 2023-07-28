#if defined inc_mapping
	#endinput
#endif
#define inc_mapping

/*
	Mapping:
	    struct:
	        list of objects info
	        
	 Map Object info:
	    struct:
	        modelid
	        x
	        y
	        z
	        rx
	        ry
	        rz
	        drawdistance
	        interiorid
	        streamdistance
	        list of objectmaterials
	        
	 ObjectMaterial:
	    struct:
	        materialindex
	        modelid
	        txdname
	        texturename
	        materialcolor
*/

static mappingbuffer[512];
static mappingbuffer2[128];
static mappingbuffer3[128];

static txdnamebuffer[256];
static texturenamebuffer[256];

static Lobby:mappingnulllobby;

public OnScriptInit()
{
    mappingnulllobby = lobbyCreate(worldCreate("MappingNullWorld"));
#if defined mapping_OnScriptInit
    mapping_OnScriptInit();
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
#define OnScriptInit mapping_OnScriptInit
#if defined mapping_OnScriptInit
forward mapping_OnScriptInit();
#endif

stock Lobby:GetMappingNullLobby() return mappingnulllobby;

stock MapObjectInfo:MapObjectInfoCreate(modelid, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz,
	Float:drawdistance = 0.0, interiorid = -1, Float:streamdistance = 300.0, List:materials = List:MEM_NULLPTR)
{
    new Pointer:ptr = MEM_new(11);
    MEM_set_val(ptr, 0, modelid);
    MEM_set_val(ptr, 1, _:x);
    MEM_set_val(ptr, 2, _:y);
    MEM_set_val(ptr, 3, _:z);
    MEM_set_val(ptr, 4, _:rx);
    MEM_set_val(ptr, 5, _:ry);
    MEM_set_val(ptr, 6, _:rz);
    MEM_set_val(ptr, 7, _:drawdistance);
    MEM_set_val(ptr, 8, interiorid);
    MEM_set_val(ptr, 9, _:streamdistance);
    MEM_set_val(ptr, 10, _:materials);
    return MapObjectInfo:ptr;
}

stock MapObjectInfoDestroy(MapObjectInfo:info)
{
	new List:materials = List:MEM_set_val(Pointer:info, 10);
	if(listIsValid(materials))
	{
		listForeach(material:materials)
		{
		    ObjectMaterialDestroy(ObjectMaterial:material);
		}
		listDestroy(materials);
	}
	MEM_delete(Pointer:info);
	return 1;
}

stock MapObjectCreate(MapObjectInfo:info)
{
	if(!MEM_is_valid_ptr(Pointer:info)) return 0;
	new objectid = CreateDynamicObject(MEM_get_val(Pointer:info, 0), Float:MEM_get_val(Pointer:info, 1),
	Float:MEM_get_val(Pointer:info, 2), Float:MEM_get_val(Pointer:info, 3), Float:MEM_get_val(Pointer:info, 4),
	Float:MEM_get_val(Pointer:info, 5), Float:MEM_get_val(Pointer:info, 6), .interiorid = MEM_get_val(Pointer:info, 8),
	.streamdistance = Float:MEM_get_val(Pointer:info, 9), .drawdistance = Float:MEM_get_val(Pointer:info, 7));
	if(IsValidDynamicObject(objectid))
	{
	    new List:list = List:MEM_get_val(Pointer:info, 10);
	    if(listIsValid(list))
	    {
	        listForeach(material:list)
	        {
	            ObjectMaterialApply(ObjectMaterial:material, objectid);
	        }
	    }
	}
	return objectid;
}

stock IsValidMapping(Mapping:mapping)
{
	return listIsValid(List:mapping);
}

stock IsValidMappingInfo(MappingInfo:mapping)
{
	return listIsValid(List:mapping);
}

stock Mapping:MappingCreate(MappingInfo:info, lobbynull = true)
{
	new List:list = List:info;
	new Mapping:mapping = Mapping:listCreate();
	if(lobbynull)
	{
	    new object;
	    new vw = lobbyGetVirtualWorld(mappingnulllobby);
		listForeach(objectinfo:list)
		{
		    object = MapObjectCreate(MapObjectInfo:objectinfo);
            Streamer_AppendArrayData(STREAMER_TYPE_OBJECT, object, E_STREAMER_WORLD_ID, vw);
		    listPush(List:mapping, object);
		}
	}
	else
	{
		listForeach(objectinfo:list)
		{
		    listPush(List:mapping, _:MapObjectCreate(MapObjectInfo:objectinfo));
		}
	}
	return mapping;
}

stock MappingDestroy(Mapping:mapping)
{
	if(!IsValidMapping(mapping))    return 0;
	new List:list = List:mapping;
	listForeach(object:list)
	{
	    DestroyDynamicObject(object);
	}
	return 1;
}

stock MappingAddToLobby(Mapping:mapping, Lobby:lobby)
{
	if(!IsValidMapping(mapping) || !lobbyIsExist(lobby))    return 0;
	
	new List:list = List:mapping;
	new vw = lobbyGetVirtualWorld(lobby);
	listForeach(object:list)
	{
	    Streamer_AppendArrayData(STREAMER_TYPE_OBJECT, object, E_STREAMER_WORLD_ID, vw);
	}
	return 1;
}

stock MappingAddToWorld(Mapping:mapping, World:world)
{
	if(!IsValidMapping(mapping) || !worldIsExist(world))    return 0;
	
	new List:lobbys = worldGetLobbyList(world);
	new List:list = List:mapping;

	new vw;
    listForeach(lobby:lobbys)
    {
        vw = lobbyGetVirtualWorld(Lobby:lobby);
	 	listForeach(object:list)
		{
	    	Streamer_AppendArrayData(STREAMER_TYPE_OBJECT, object, E_STREAMER_WORLD_ID, vw);
		}
	}
	return 1;
}

stock MappingRemoveFromLobby(Mapping:mapping, Lobby:lobby)
{
	if(!IsValidMapping(mapping) || !lobbyIsExist(lobby))    return 0;

	new List:list = List:mapping;
	new vw = lobbyGetVirtualWorld(lobby);
	listForeach(object:list)
	{
	    Streamer_RemoveArrayData(STREAMER_TYPE_OBJECT, object, E_STREAMER_WORLD_ID, vw);
	}
	return 1;
}

stock MappingRemoveFromWorld(Mapping:mapping, World:world)
{
	if(!IsValidMapping(mapping) || !worldIsExist(world))    return 0;

	new List:lobbys = worldGetLobbyList(world);
	new List:list = List:mapping;

	new vw;
    listForeach(lobby:lobbys)
    {
        vw = lobbyGetVirtualWorld(Lobby:lobby);
	 	listForeach(object:list)
		{
	    	Streamer_RemoveArrayData(STREAMER_TYPE_OBJECT, object, E_STREAMER_WORLD_ID, vw);
		}
	}
	return 1;
}

stock ObjectMaterial:ObjectMaterialCreate(materialindex, modelid, txdname[], texturename[], materialcolor = 0)
{
    new Pointer:ptr = MEM_new(5);
    MEM_set_val(ptr, 0, materialindex);
    MEM_set_val(ptr, 1, modelid);
    MEM_set_val(ptr, 2, _:MEM_new_arr(txdname, strlen(txdname)));
    MEM_set_val(ptr, 3, _:MEM_new_arr(texturename, strlen(texturename)));
    MEM_set_val(ptr, 4, materialcolor);
	return ObjectMaterial:ptr;
}

stock ObjectMaterialDestroy(ObjectMaterial:material)
{
	MEM_delete(Pointer:MEM_get_val(Pointer:material, 2));
	MEM_delete(Pointer:MEM_get_val(Pointer:material, 3));
	MEM_delete(Pointer:material);
	return 1;
}

stock ObjectMaterialApply(ObjectMaterial:material, objectid)
{
    if(!MEM_is_valid_ptr(Pointer:material)) return 0;
    new Pointer:ptr;
    
    ptr = Pointer:MEM_get_val(Pointer:material, 2);
    MEM_get_arr(ptr, 0, txdnamebuffer, MEM_get_size(ptr));
    txdnamebuffer[MEM_get_size(ptr)] = '\0';
    
    ptr = Pointer:MEM_get_val(Pointer:material, 3);
    MEM_get_arr(ptr, 0, texturenamebuffer, MEM_get_size(ptr));
    texturenamebuffer[MEM_get_size(ptr)] = '\0';
    
    return SetDynamicObjectMaterial(objectid, MEM_get_val(Pointer:material, 0),
	MEM_get_val(Pointer:material, 1), txdnamebuffer, texturenamebuffer, MEM_get_val(Pointer:material, 4));
}

stock MappingInfo:MappingReadFromFile(filename[])
{
    new MappingInfo:mapping;
    new MapObjectInfo:object;
    new ObjectMaterial:material;
    new List:materials = List:MEM_NULLPTR;

	new List:marker;
	new mark1, mark2;
	new count;
	
	//Создание объекта
	new modelid;
	new Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz;
	new Float:drawdistance;
	new interiorid, Float:streamdistance;
	
	//Ретекстур
	new materialindex, materialcolor;
	
	if(!fmFileExists(filename))	return MappingInfo:0;
	new File:file = fmFileOpen(filename, "r");

    mapping = MappingInfo:listCreate();
	
	while(fmFileRead(file, mappingbuffer))
	{
	    marker = listCreate();
	    mark1 = strfind(mappingbuffer, "(");
	    
	    mark2 = strfind(mappingbuffer, "=");
	    
	    if(mark2 > mark1 || mark2 == -1)
	    {
	        strmid(mappingbuffer2, mappingbuffer, 0, mark1);
	    }
	    else
	    {
	        strmid(mappingbuffer2, mappingbuffer, mark2+1, mark1);
	    }
	    
	    mark2 = strfind(mappingbuffer, ")", .pos = mark1+1);
	    
	    strCut(mappingbuffer2, mappingbuffer2);
	    
	    strmid(mappingbuffer, mappingbuffer, mark1+1, mark2);
	    marker = strMarker(mappingbuffer, ",");

	    count = listLen(marker)/2;
    
	    drawdistance = 0.0;
	    streamdistance = 300.0;
	    interiorid = -1;
	    materialcolor = 0;
	    
	    if(!strcmp(mappingbuffer2, "CreateObject"))
	    {
		    if(count < 7 || count > 8)	continue;

		    strSeparator(mappingbuffer, mappingbuffer2, 0, marker, true);
		    modelid = strval(mappingbuffer2);

		    strSeparator(mappingbuffer, mappingbuffer2, 1, marker, true);
		    x = floatstr(mappingbuffer2);

		    strSeparator(mappingbuffer, mappingbuffer2, 2, marker, true);
		    y = floatstr(mappingbuffer2);

		    strSeparator(mappingbuffer, mappingbuffer2, 3, marker, true);
		    z = floatstr(mappingbuffer2);

		    strSeparator(mappingbuffer, mappingbuffer2, 4, marker, true);
		    rx = floatstr(mappingbuffer2);

		    strSeparator(mappingbuffer, mappingbuffer2, 5, marker, true);
		    ry = floatstr(mappingbuffer2);

		    strSeparator(mappingbuffer, mappingbuffer2, 6, marker, true);
		    rz = floatstr(mappingbuffer2);

		    if(count == 8)
		    {
			    strSeparator(mappingbuffer, mappingbuffer2, 7, marker, true);
			    drawdistance = floatstr(mappingbuffer2);
		    }
			object = MapObjectInfoCreate(modelid, x, y, z, rx, ry, rz,
			    drawdistance, interiorid, streamdistance, materials);

		    listPush(List:mapping, _:object);
		    
			if(listIsValid(materials))
			{
			    materials = List:MEM_NULLPTR;
			}
		}
		if(!strcmp(mappingbuffer2, "CreateDynamicObject"))
		{
		    if(count < 7)	continue;
		    strSeparator(mappingbuffer, mappingbuffer2, 0, marker, true);
		    modelid = strval(mappingbuffer2);

		    strSeparator(mappingbuffer, mappingbuffer2, 1, marker, true);
		    x = floatstr(mappingbuffer2);

		    strSeparator(mappingbuffer, mappingbuffer2, 2, marker, true);
		    y = floatstr(mappingbuffer2);

		    strSeparator(mappingbuffer, mappingbuffer2, 3, marker, true);
		    z = floatstr(mappingbuffer2);

		    strSeparator(mappingbuffer, mappingbuffer2, 4, marker, true);
		    rx = floatstr(mappingbuffer2);

		    strSeparator(mappingbuffer, mappingbuffer2, 5, marker, true);
		    ry = floatstr(mappingbuffer2);

		    strSeparator(mappingbuffer, mappingbuffer2, 6, marker, true);
		    rz = floatstr(mappingbuffer2);

			for(new i = 7; i < count; i++)
			{
			    strSeparator(mappingbuffer, mappingbuffer2, i, marker, true);
				mark1 = strfind(mappingbuffer2, ".");
				mark2 = strfind(mappingbuffer2, "=", .pos = mark1+1);
				if(mark1 != -1 && mark2 != -1)
				{
				    strmid(mappingbuffer3, mappingbuffer2, mark2+1, strlen(mappingbuffer2));
				    strmid(mappingbuffer2, mappingbuffer2, mark1+1, mark2);
				    
				    if(!strcmp(mappingbuffer2, "interiorid"))		interiorid = strval(mappingbuffer3);
				    if(!strcmp(mappingbuffer2, "streamdistance"))	streamdistance = floatstr(mappingbuffer3);
				    if(!strcmp(mappingbuffer2, "drawdistance"))		drawdistance = floatstr(mappingbuffer3);
				}
				else
				{
				    if(i == 8)	interiorid = 		strval(mappingbuffer2);
					if(i == 10) streamdistance = 	floatstr(mappingbuffer2);
					if(i == 11) drawdistance = 		floatstr(mappingbuffer2);
				}
				
			}
			object = MapObjectInfoCreate(modelid, x, y, z, rx, ry, rz,
			    drawdistance, interiorid, streamdistance, materials);

		    listPush(List:mapping, _:object);
		    
			if(listIsValid(materials))
			{
			    materials = List:MEM_NULLPTR;
			}
		}
		if(!strcmp(mappingbuffer2, "SetObjectMaterial") || !strcmp(mappingbuffer2, "SetObjectDynamicMaterial"))
		{
		    if(count < 5 || count > 6)	continue;
		    strSeparator(mappingbuffer, mappingbuffer2, 1, marker, true);
		    materialindex = strval(mappingbuffer2);
		    
		    strSeparator(mappingbuffer, mappingbuffer2, 2, marker, true);
		    modelid = strval(mappingbuffer2);
		    
		    strSeparator(mappingbuffer, txdnamebuffer, 3, marker, true);
		    
		    strSeparator(mappingbuffer, texturenamebuffer, 4, marker, true);
		    
		    if(count == 6)
		    {
		    	strSeparator(mappingbuffer, mappingbuffer2, 5, marker, true);
		    	materialcolor = strval(mappingbuffer2);
			}
			material = ObjectMaterialCreate(materialindex, modelid, txdnamebuffer, texturenamebuffer, materialcolor);
			
			if(!listIsValid(materials))
			{
			    materials = listCreate();
			}
			listPush(materials, _:material);
		}
	    listDestroy(marker);
	}
	
	fmFileClose(file);
	return mapping;
}



