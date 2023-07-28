#if defined mdg_item_inc
	#endinput
#endif
#define mdg_item_inc

#include <MS/list>
#include <MS/vector>
#include <MS/map>

/*
	Item Info
	    struct:
	        max id in vector of item data
	        map of item data:
	            key - name of item data
	            value - id in vector of item data
			vector of items:
		    	item
            default item vector

	Item
	    struct:
	        item info
	        id in vector of items
	        vector of data (item data):
	            node:
	                autodelete (0 - none, 1 - item, 2 - pointer, 3 - list, 4 - vector)
	                ptr data
			
	Item Array
		struct:
		    vector

	Item List
	    strcut:
	        list
*/

stock itemInit()
{
    return 1;
}

stock itemIsValid(Item:item)
{
	return MEM_is_valid_ptr(Pointer:item);
}

stock itemInfoIsValid(ItemInfo:iteminfo)
{
	return MEM_is_valid_ptr(Pointer:iteminfo);
}

stock Item:itemPush(ItemInfo:iteminfo)
{
    if(!itemInfoIsValid(iteminfo))  return Item:0;

    new maxid = MEM_get_val(Pointer:iteminfo, 0);
    if(maxid == 0)  return Item:0;

	new Item:item = Item:MEM_new(3);
	MEM_set_val(Pointer:item, 0, _:iteminfo);

//	new Vector:data = vectorCopy(Vector:MEM_get_val(Pointer:iteminfo, 3));
//	MEM_set_val(Pointer:item, 2, _:data);

	//Copy default values
	new Vector:source = Vector:MEM_get_val(Pointer:iteminfo, 3);
	new Vector:data = vectorCreate(vectorLen(source));
	vectorLenUp(data, vectorLen(source));
	MEM_set_val(Pointer:item, 2, _:data);
	
	new Pointer:node;
	new Pointer:tempdata = vectorGetData(data);
	
	new index = 0;
	vectorForeach(dat:source)
	{
	    node = MEM_new(2);
	    MEM_set_val(node, 0, 0);
	    MEM_set_val(node, 1, _:MEM_new_val(dat));
		MEM_set_val(tempdata, index, _:node);
		index++;
	}
	
    new Vector:vector = Vector:MEM_get_val(Pointer:iteminfo, 2);
    vectorPush(vector, _:item);
	MEM_set_val(Pointer:item, 1, vectorLen(vector)-1);
	return item;
}

stock Item:itemCreate(ItemInfo:iteminfo, itemid)
{
    if(!itemInfoIsValid(iteminfo))  return Item:0;
    
    new maxid = MEM_get_val(Pointer:iteminfo, 0);
    if(maxid == 0)  return Item:0;

	new Item:item = Item:MEM_new(3);
	MEM_set_val(Pointer:item, 0, _:iteminfo);
	MEM_set_val(Pointer:item, 1, itemid);

	//Copy default values
	new Vector:source = Vector:MEM_get_val(Pointer:iteminfo, 3);
	new Vector:data = vectorCreate(vectorLen(source));
	vectorLenUp(data, vectorLen(source));
	MEM_set_val(Pointer:item, 2, _:data);

	new Pointer:node;
	new Pointer:tempdata = vectorGetData(data);

	new index = 0;
	vectorForeach(dat:source)
	{
	    node = MEM_new(2);
	    MEM_set_val(node, 0, 0);
	    MEM_set_val(node, 1, _:MEM_new_val(dat));
		MEM_set_val(tempdata, index, _:node);
		index++;
	}

    new Vector:vector = Vector:MEM_get_val(Pointer:iteminfo, 2);
	new len = vectorLen(vector);
	if(len <= itemid)
	{
	    vectorLenUp(vector, itemid-len+1, true);
	}
	vectorSet(vector, itemid, _:item);
	return item;
}

stock itemDestroy(Item:item)
{
    if(!itemIsValid(item))  return 0;
	new ItemInfo:iteminfo = ItemInfo:MEM_get_val(Pointer:item, 0);
	if(!itemInfoIsValid(iteminfo))  return 0;
	
	new Vector:vector = Vector:MEM_get_val(Pointer:item, 2);
	new autodelete;
	vectorForeachPtr(node:vector)
	{
	    if(node == MEM_NULLPTR) continue;
	    autodelete = MEM_get_val(node, 0);
	    switch(autodelete)
	    {
	        case 0: MEM_delete(Pointer:MEM_get_val(node, 1));
	        case 1: itemDestroy(Item:MEM_get_val(node, 1));
	        case 2: MEM_delete(Pointer:MEM_get_val(node, 1));
	        case 3: listDestroy(List:MEM_get_val(node, 1));
	        case 4: vectorDestroy(Vector:MEM_get_val(node, 1));
	    }
		MEM_delete(node);
	}
	vectorDestroy(vector);
	vector = Vector:MEM_get_val(Pointer:iteminfo, 2);
	vectorSet(vector, MEM_get_val(Pointer:item, 1), 0);
	MEM_delete(Pointer:item);
	return 1;
}

stock itemDestroyByID(ItemInfo:iteminfo, itemid)
{
	if(!itemInfoIsValid(iteminfo))  return 0;

	new Vector:vector = Vector:MEM_get_val(Pointer:iteminfo, 2);
	new Item:item;
	vectorGet(vector, itemid, _:item);
	itemDestroy(item);
	return 1;
}

stock Item:itemGetByID(ItemInfo:iteminfo, itemid)
{
	if(!itemInfoIsValid(iteminfo))  return Item:0;
	
	new Item:item;
	
	new Vector:vector = Vector:MEM_get_val(Pointer:iteminfo, 2);
	if(!vectorGet(vector, itemid, _:item))    return Item:0;
	if(!itemIsValid(item))  return Item:0;
	return item;
}

stock itemGetID(Item:item)
{
	return MEM_get_val(Pointer:item, 1);
}

stock ItemInfo:itemGetItemInfo(Item:item)
{
	return ItemInfo:MEM_get_val(Pointer:item, 0);
}

stock itemGetDataID(Item:item, name[])
{
    if(!itemIsValid(item))  return -1;

    new ItemInfo:iteminfo = ItemInfo:MEM_get_val(Pointer:item, 0);
	new Map:map = Map:MEM_get_val(Pointer:iteminfo, 1);

    new id = -1;
    if(!mapGet(map, name, id))  return -1;
	return id;
}

stock Pointer:itemGetDataPointer(Item:item, name[])
{
    if(!itemIsValid(item))  return Pointer:0;
    
    new Pointer:ptr;
    
	new id = itemGetDataID(item, name);
    new Vector:vector = Vector:MEM_get_val(Pointer:item, 2);
    vectorGet(vector, id, ptr);
    ptr = Pointer:MEM_get_val(ptr, 1);
	return ptr;
}

stock Vector:itemGetData(Item:item)
{
	new Vector:vector = Vector:MEM_get_val(Pointer:item, 2);
	return vector;
}

stock itemGet(Item:item, name[], output[], maxlenght = sizeof(output))
{
    new Pointer:ptr = itemGetDataPointer(item, name);
	if(ptr == MEM_NULLPTR)   return 0;
	
	new len = min(maxlenght-1, MEM_get_size(ptr));
	MEM_get_arr(ptr, 0, output, len);
	output[len] = '\0';
	return 1;
}

stock itemGetArray(Item:item, name[], output[])
{
    new Pointer:ptr = itemGetDataPointer(item, name);
	if(ptr == MEM_NULLPTR)   return 0;
	MEM_get_arr(ptr, 0, output, MEM_get_size(ptr));
	return 1;
}

stock itemGetInt(Item:item, name[])
{
    new Pointer:ptr = itemGetDataPointer(item, name);
	if(ptr == MEM_NULLPTR)   return 0;
	return MEM_get_val(ptr, 0);
}

stock Float:itemGetFloat(Item:item, name[])
{
    new Pointer:ptr = itemGetDataPointer(item, name);
	if(ptr == MEM_NULLPTR)   return Float:0;
	return Float:MEM_get_val(ptr, 0);
}

stock Pointer:itemGetPointer(Item:item, name[])
{
    new Pointer:ptr = itemGetDataPointer(item, name);
	if(ptr == MEM_NULLPTR)   return Pointer:0;
	return Pointer:MEM_get_val(ptr, 0);
}

stock Item:itemGetItem(Item:item, name[])
{
    new Pointer:ptr = itemGetDataPointer(item, name);
	if(ptr == MEM_NULLPTR)   return Item:0;
	return Item:MEM_get_val(ptr, 0);
}

stock List:itemGetList(Item:item, name[])
{
    new Pointer:ptr = itemGetDataPointer(item, name);
	if(ptr == MEM_NULLPTR)   return List:0;
	return List:MEM_get_val(ptr, 0);
}

stock Vector:itemGetVector(Item:item, name[])
{
    new Pointer:ptr = itemGetDataPointer(item, name);
	if(ptr == MEM_NULLPTR)   return Vector:0;
	return Vector:MEM_get_val(ptr, 0);
}

stock itemSet(Item:item, name[], input[])
{
    if(!itemIsValid(item))  return 0;

    new Pointer:ptr;

    new Vector:vector = Vector:MEM_get_val(Pointer:item, 2);
    new id = itemGetDataID(item, name);
    vectorGet(vector, id, ptr);
    if(MEM_is_valid_ptr(ptr))
    {
	    new Pointer:ptr2 = Pointer:MEM_get_val(ptr, 1);
	    if(MEM_is_valid_ptr(ptr2))   MEM_delete(ptr2);
	}
	else
	{
     	ptr = MEM_new(2);
	    vectorSet(vector, id, ptr);
	}
    MEM_set_val(ptr, 0, 0);
    MEM_set_val(ptr, 1, _:MEM_new_arr(input, strlen(input)));
	
	return 1;
}

stock itemSetArray(Item:item, name[], input[], len = sizeof(input), bool:autodelete = true)
{
    if(!itemIsValid(item))  return 0;

    new Pointer:ptr;

    new Vector:vector = Vector:MEM_get_val(Pointer:item, 2);
    new id = itemGetDataID(item, name);
    vectorGet(vector, id, ptr);
    if(MEM_is_valid_ptr(ptr))
    {
	    new Pointer:ptr2 = Pointer:MEM_get_val(ptr, 1);
	    if(MEM_is_valid_ptr(ptr2))   MEM_delete(ptr2);
	}
	else
	{
     	ptr = MEM_new(2);
	    vectorSet(vector, id, ptr);
	}
    MEM_set_val(ptr, 0, autodelete);
    MEM_set_val(ptr, 1, _:MEM_new_arr(input, len));

	return 1;
}

stock itemSetInt(Item:item, name[], input)
{
    if(!itemIsValid(item))  return 0;

    new Pointer:ptr;

    new Vector:vector = Vector:MEM_get_val(Pointer:item, 2);
    new id = itemGetDataID(item, name);
    vectorGet(vector, id, ptr);
    if(MEM_is_valid_ptr(ptr))
    {
	    new Pointer:ptr2 = Pointer:MEM_get_val(ptr, 1);
	    if(MEM_is_valid_ptr(ptr2))   MEM_delete(ptr2);
	}
	else
	{
	    ptr = MEM_new(2);
	    vectorSet(vector, id, ptr);
	}
    MEM_set_val(ptr, 0, 0);
    MEM_set_val(ptr, 1, _:MEM_new_val(_:input));

	return 1;
}

stock itemSetFloat(Item:item, name[], Float:input)
{
    new Pointer:ptr;

    new Vector:vector = Vector:MEM_get_val(Pointer:item, 2);
    new id = itemGetDataID(item, name);
    vectorGet(vector, id, ptr);
    if(MEM_is_valid_ptr(ptr))
    {
	    new Pointer:ptr2 = Pointer:MEM_get_val(ptr, 1);
	    if(MEM_is_valid_ptr(ptr2))   MEM_delete(ptr2);
	}
	else
	{
	    ptr = MEM_new(2);
	    vectorSet(vector, id, ptr);
	}
    MEM_set_val(ptr, 0, 0);
    MEM_set_val(ptr, 1, _:MEM_new_val(_:input));

	return 1;
}

stock itemSetPointer(Item:item, name[], Pointer:input, bool:autodelete = true)
{
    new Pointer:ptr;

    new Vector:vector = Vector:MEM_get_val(Pointer:item, 2);
    new id = itemGetDataID(item, name);
    vectorGet(vector, id, ptr);
    if(MEM_is_valid_ptr(ptr))
    {
	    new Pointer:ptr2 = Pointer:MEM_get_val(ptr, 1);
	    if(MEM_is_valid_ptr(ptr2))   MEM_delete(ptr2);
	}
	else
	{
	    ptr = MEM_new(2);
	    vectorSet(vector, id, ptr);
	}
	if(!autodelete)	MEM_set_val(ptr, 0, 0);
	    else    MEM_set_val(ptr, 0, 2);
    MEM_set_val(ptr, 1, _:MEM_new_val(_:input));

	return 1;
}

stock itemSetItem(Item:item, name[], Item:input, bool:autodelete = true)
{
    new Pointer:ptr;

    new Vector:vector = Vector:MEM_get_val(Pointer:item, 2);
    new id = itemGetDataID(item, name);
    vectorGet(vector, id, ptr);
    if(MEM_is_valid_ptr(ptr))
    {
	    new Pointer:ptr2 = Pointer:MEM_get_val(ptr, 1);
	    if(MEM_is_valid_ptr(ptr2))   MEM_delete(ptr2);
	}
	else
	{
	    ptr = MEM_new(2);
	    vectorSet(vector, id, ptr);
	}
	if(!autodelete)	MEM_set_val(ptr, 0, 0);
	    else    MEM_set_val(ptr, 0, 1);
    MEM_set_val(ptr, 1, _:MEM_new_val(_:input));

	return 1;
}

stock itemSetList(Item:item, name[], List:input, bool:autodelete = true)
{
    new Pointer:ptr;

    new Vector:vector = Vector:MEM_get_val(Pointer:item, 2);
    new id = itemGetDataID(item, name);
    vectorGet(vector, id, ptr);
    if(MEM_is_valid_ptr(ptr))
    {
	    new Pointer:ptr2 = Pointer:MEM_get_val(ptr, 1);
	    if(MEM_is_valid_ptr(ptr2))   MEM_delete(ptr2);
	}
	else
	{
	    ptr = MEM_new(2);
	    vectorSet(vector, id, ptr);
	}
	if(!autodelete)	MEM_set_val(ptr, 0, 0);
	    else    MEM_set_val(ptr, 0, 3);
    MEM_set_val(ptr, 1, _:MEM_new_val(_:input));

	return 1;
}

stock itemSetVector(Item:item, name[], Vector:input, bool:autodelete = true)
{
    new Pointer:ptr;

    new Vector:vector = Vector:MEM_get_val(Pointer:item, 2);
    new id = itemGetDataID(item, name);
    vectorGet(vector, id, ptr);
    if(MEM_is_valid_ptr(ptr))
    {
	    new Pointer:ptr2 = Pointer:MEM_get_val(ptr, 1);
	    if(MEM_is_valid_ptr(ptr2))   MEM_delete(ptr2);
	}
	else
	{
	    ptr = MEM_new(2);
	    vectorSet(vector, id, ptr);
	}
	if(!autodelete)	MEM_set_val(ptr, 0, 0);
	    else    MEM_set_val(ptr, 0, 4);
    MEM_set_val(ptr, 1, _:MEM_new_val(_:input));

	return 1;
}

stock ItemInfo:itemInfoCreate()
{
	new ItemInfo:iteminfo = ItemInfo:MEM_new(4);

    MEM_set_val(Pointer:iteminfo, 0, 0);
	MEM_set_val(Pointer:iteminfo, 1, _:mapCreate());
	MEM_set_val(Pointer:iteminfo, 2, _:vectorCreate());
	MEM_set_val(Pointer:iteminfo, 3, _:vectorCreate());
	
	return ItemInfo:iteminfo;
}

stock itemInfoDestroy(ItemInfo:iteminfo)
{
	if(!itemInfoIsValid(iteminfo))  return 0;
	
	new Map:map = Map:MEM_get_val(Pointer:iteminfo, 1);
	new Vector:vector = Vector:MEM_get_val(Pointer:iteminfo, 2);
	
	mapDestroy(map);
	
	vectorForeach(item:vector)
	{
		itemDestroy(Item:item);
	}
	vectorDestroy(vector);
	vectorDestroyPtr(Vector:MEM_get_val(Pointer:iteminfo, 3));
	MEM_delete(Pointer:iteminfo);
	
	return 1;
}

stock itemInfoAdd(ItemInfo:iteminfo, name[], {_, Float, Pointer}:defaultvalue = 0)
{
    if(!itemInfoIsValid(iteminfo) || !strlen(name))  return 0;
    
	new maxid = MEM_get_val(Pointer:iteminfo, 0);
	new Map:map = Map:MEM_get_val(Pointer:iteminfo, 1);
	
	if(mapIsKeyExist(map, name))    return 0;
	
	mapAdd(map, name, maxid, false);
	maxid++;
	MEM_set_val(Pointer:iteminfo, 0, maxid);
	
	//Update items
	new Vector:vector = Vector:MEM_get_val(Pointer:iteminfo, 2);
	new Vector:data;
	new len;

	vectorForeach(item:vector)
	{
	    if(!itemIsValid(Item:item)) continue;
	    data = Vector:MEM_get_val(Pointer:item, 2);
		len = vectorLen(data);
		if(len < maxid)
		{
		    vectorLenUp(data, maxid-len);
		    vectorSet(data, maxid-1, _:defaultvalue);
		}
	}
	vector = Vector:MEM_get_val(Pointer:iteminfo, 3);
	vectorLenUp(vector, 1);
	vectorSet(vector, vectorLen(vector)-1, _:defaultvalue);
	return 1;
}

stock itemInfoGetDefault(ItemInfo:iteminfo, name[])
{
	if(!itemInfoIsValid(iteminfo) || !strlen(name))  return 0;
	new Map:map = Map:MEM_get_val(Pointer:iteminfo, 1);
	if(!mapIsExist(map))	return 0;
	new index;
	if(!mapGet(map, name, index)) return 0;
	new Vector:vector = Vector:MEM_get_val(Pointer:iteminfo, 3);
	if(!vectorIsValid(vector))  return 0;
	new value;
	vectorGet(vector, index, value);
	return value;
}

stock itemInfoSetDefault(ItemInfo:iteminfo, name[], defaultvalue)
{
	if(!itemInfoIsValid(iteminfo) || !strlen(name))  return 0;
	new Map:map = Map:MEM_get_val(Pointer:iteminfo, 1);
	if(!mapIsExist(map))    return 0;
	new index;
	if(!mapGet(map, name, index)) return 0;
	new Vector:vector = Vector:MEM_get_val(Pointer:iteminfo, 3);
	if(!vectorIsValid(vector))  return 0;
	vectorSet(vector, index, defaultvalue);
	return 1;
}

public OnScriptInit()
{
	itemInit();
#if defined mdg_item_OnScriptInit
    mdg_item_OnScriptInit();
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
#define OnScriptInit mdg_item_OnScriptInit
#if defined mdg_item_OnScriptInit
forward mdg_item_OnScriptInit();
#endif
