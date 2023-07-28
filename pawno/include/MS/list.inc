#if defined mdg_list_inc
	#endinput
#endif
#define mdg_list_inc

/*
	struct:
		main node:
			1) count of common nodes
			2) ptr to first common node
			3) ptr to last common node
		common node:
		    1) data (value or ptr with _: tag)
		    2) ptr to next node
*/

#include <memory>

#define listForeach(%0:%1) \
	for(\
		new Pointer:%0_cN = Pointer:MEM_get_val(Pointer:%1, 1), %0 = MEM_get_val(%0_cN, 0);\
	 	%0_cN != MEM_NULLPTR;\
	 	%0_cN = Pointer:MEM_get_val(%0_cN, 1), %0 = MEM_get_val(%0_cN, 0)\
	)
	
#define listForeachPtr(%0:%1) \
	for(\
		new Pointer:%0_cN = Pointer:MEM_get_val(Pointer:%1, 1), Pointer:%0 = Pointer:MEM_get_val(%0_cN, 0);\
	 	%0_cN != MEM_NULLPTR;\
	 	%0_cN = Pointer:MEM_get_val(%0_cN, 1), %0 = Pointer:MEM_get_val(%0_cN, 0)\
	)
	
#define listIsLastNode(%0) (Pointer:MEM_get_val(%0_cN, 1) == MEM_NULLPTR)

stock listIsValid(List:list)
{
	return MEM_is_valid_ptr(Pointer:list);
}

stock List:listCreate()
{
    new Pointer:nodeMain = MEM_new(3);
    MEM_set_val(nodeMain, 0, 0);
    MEM_set_val(nodeMain, 1, _:MEM_NULLPTR);
    MEM_set_val(nodeMain, 2, _:MEM_NULLPTR);
	return List:nodeMain;
}

stock listDestroy(List:list)
{
	new Pointer:curNode;
	new Pointer:lastNode = Pointer:list;
	while(lastNode != MEM_NULLPTR)
	{
	    curNode = Pointer:MEM_get_val(lastNode, 1);
	    MEM_delete(lastNode);
	    lastNode = curNode;
	}
	return 1;
}

stock listDestroyPtr(List:list)//Where data is pointers
{
	new Pointer:curNode;
	new Pointer:lastNode = Pointer:list;
	while(lastNode != MEM_NULLPTR)
	{
	    curNode = Pointer:MEM_get_val(lastNode, 1);
	    MEM_delete(Pointer:MEM_get_val(curNode, 0));//delete ptr
	    MEM_delete(lastNode);
	    lastNode = curNode;
	}
	return 1;
}

stock Pointer:listInsertAtEnd(List:list, {_, Pointer}:value)
{
    new Pointer:nodeOldLast = Pointer:MEM_get_val(Pointer:list, 2);
    if(nodeOldLast == MEM_NULLPTR)
    {
        nodeOldLast = Pointer:list;
    }
	new Pointer:nodeNewLast = MEM_new(2);
    MEM_set_val(nodeNewLast, 0, _:value);
    MEM_set_val(nodeNewLast, 1, _:MEM_NULLPTR);
    MEM_set_val(nodeOldLast, 1, _:nodeNewLast);
	MEM_set_val(Pointer:list, 2, _:nodeNewLast);
	MEM_set_val(Pointer:list, 0, MEM_get_val(Pointer:list, 0)+1);
    return nodeNewLast;
}

stock Pointer:listInsertAtHead(List:list, {_, Pointer}:value)
{
    new Pointer:nodeOldFirst = Pointer:MEM_get_val(Pointer:list, 1);
	new Pointer:nodeNewFirst = MEM_new(2);
    MEM_set_val(nodeNewFirst, 0, _:value);
    MEM_set_val(nodeNewFirst, 1, _:nodeOldFirst);
	MEM_set_val(Pointer:list, 1, _:nodeNewFirst);
	MEM_set_val(Pointer:list, 0, MEM_get_val(Pointer:list, 0)+1);
    return nodeNewFirst;
}

stock Pointer:listInsert(List:list, index, {_, Pointer}:value)
{
	if(index < 0 || index > MEM_get_val(Pointer:list, 0))   return MEM_NULLPTR;

    MEM_set_val(Pointer:list, 0, MEM_get_val(Pointer:list, 0)+1);

	new Pointer:nextNode = Pointer:list;
	new Pointer:lastNode;

	for(new i = -1; i < index; i++)
	{
	    lastNode = nextNode;
	    if(lastNode == MEM_NULLPTR) return MEM_NULLPTR;
	    nextNode = Pointer:MEM_get_val(lastNode, 1);
	}
	
	new Pointer:curNode = MEM_new(2);
    MEM_set_val(curNode, 0, _:value);
    MEM_set_val(curNode, 1, _:nextNode);
    MEM_set_val(lastNode, 1, _:curNode);
	return curNode;
}

stock Pointer:listGetNode(List:list, index)
{
	if(index < 0 || index >= MEM_get_val(Pointer:list, 0))	return MEM_NULLPTR;
	new Pointer:node = Pointer:list;
	for(new i = -1; i < index; i++)
	{
	    node = Pointer:MEM_get_val(node, 1);
	    if(node == MEM_NULLPTR) return MEM_NULLPTR;
	}
	return node;
}

stock listGet(List:list, index, &{_, Pointer}:value)
{
	new Pointer:node = listGetNode(List:list, index);
	if(node == MEM_NULLPTR) return 0;
	value = MEM_get_val(node, 0);
	return 1;
}

stock listSet(List:list, index, {_, Pointer}:value)
{
	new Pointer:node = listGetNode(List:list, index);
	if(node == MEM_NULLPTR) return 0;
	MEM_set_val(node, 0, value);
	return 1;
}

stock listDelete(List:list, index, &{_, Pointer}:value = 0, bool:deleteptr)
{
	if(index < 0 || index >= MEM_get_val(Pointer:list, 0))	return 0;
	
	new Pointer:curNode = Pointer:list;
	new Pointer:lastNode;

	for(new i = -1; i < index; i++)
	{
	    lastNode = curNode;
	    curNode = Pointer:MEM_get_val(lastNode, 1);
	    if(curNode == MEM_NULLPTR) return 0;
	}
	MEM_set_val(lastNode, 1, MEM_get_val(curNode, 1));
	if(deleteptr)
	{
	    MEM_delete(Pointer:MEM_get_val(curNode, 0));
	}
	value = MEM_get_val(curNode, 0);
	MEM_delete(curNode);
	
	MEM_set_val(Pointer:list, 0, MEM_get_val(Pointer:list, 0)-1);
	if(curNode == Pointer:MEM_get_val(Pointer:list, 2))
	{
	    MEM_set_val(Pointer:list, 2, _:lastNode);
	}
	return 1;
}

stock listDeleteValue(List:list, {_, Pointer}:value, bool:deleteptr)
{
	new Pointer:curNode = Pointer:list;
	new Pointer:lastNode;

	while(value == value)
	{
	    lastNode = curNode;
	    curNode = Pointer:MEM_get_val(lastNode, 1);
	    if(curNode == MEM_NULLPTR) return 0;
	    if(_:value == MEM_get_val(curNode, 0))  break;
	}
	MEM_set_val(lastNode, 1, MEM_get_val(curNode, 1));
	if(deleteptr)
	{
	    MEM_delete(Pointer:MEM_get_val(curNode, 0));
	}
	MEM_delete(curNode);
	
	MEM_set_val(Pointer:list, 0, MEM_get_val(Pointer:list, 0)-1);
	if(curNode == Pointer:MEM_get_val(Pointer:list, 2))
	{
	    MEM_set_val(Pointer:list, 2, _:lastNode);
	}
	return 1;
}

stock Pointer:listPush(List:list, {_, Pointer}:value)
{
	return listInsertAtEnd(list, _:value);
}

stock listPop(List:list, index, &{_, Pointer}:value = 0, bool:deleteptr)
{
	return listDelete(list, index, value, deleteptr);
}

stock listLen(List:list)
{
	return MEM_get_val(Pointer:list, 0);
}

stock listIsEmpty(List:list)
{
	return listLen(list) == 0 ? 1 : 0;
}

stock listFind(List:list, {_, Pointer}:value)
{
	new Pointer:node = Pointer:MEM_get_val(Pointer:list, 1);

	for(new i = 0; node != MEM_NULLPTR; i++)
	{
	    if(MEM_get_val(node, 0) == _:value) return i;
	    node = Pointer:MEM_get_val(node, 1);
	}
	return -1;
}

stock listClear(List:list)
{
	new Pointer:curNode = Pointer:MEM_get_val(Pointer:list, 1);
	new Pointer:lastNode = curNode;

	while(lastNode != MEM_NULLPTR)
	{
	    curNode = Pointer:MEM_get_val(lastNode, 1);
	    MEM_delete(lastNode);
	    lastNode = curNode;
	}
	
	MEM_set_val(Pointer:list, 0, 0);
	return 1;
}

stock listClearPtr(List:list)
{
	new Pointer:curNode = Pointer:MEM_get_val(Pointer:list, 1);
	new Pointer:lastNode = curNode;

	while(lastNode != MEM_NULLPTR)
	{
	    curNode = Pointer:MEM_get_val(lastNode, 1);
	    MEM_delete(Pointer:MEM_get_val(curNode, 0));//delete ptr
	    MEM_delete(lastNode);
	    lastNode = curNode;
	}
	
	MEM_set_val(Pointer:list, 0, 0);
	return 1;
}
