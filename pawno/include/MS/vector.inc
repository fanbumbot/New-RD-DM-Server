#if defined mdg_vector_inc
	#endinput
#endif
#define mdg_vector_inc

#include <memory>

/*
	struct:
	    Main node:
	        first capacity
			len
			ptr to array
*/

#define vectorForeach(%0:%1) \
	for(new Pointer:%0_d = Pointer:MEM_get_val(Pointer:%1, 2), %0 = MEM_get_val(%0_d, 0), %0_i = 0, %0_l = MEM_get_val(Pointer:%1, 1); %0_i < %0_l; %0_i++, %0 = MEM_get_val(%0_d, %0_i))

#define vectorForeachPtr(%0:%1) \
	for(new Pointer:%0_d = Pointer:MEM_get_val(Pointer:%1, 2), Pointer:%0 = Pointer:MEM_get_val(%0_d, 0), %0_i = 0, %0_l = MEM_get_val(Pointer:%1, 1); %0_i < %0_l; %0_i++, %0 = Pointer:MEM_get_val(%0_d, %0_i))

stock vectorLen(Vector:vector)
{
	return MEM_get_val(Pointer:vector, 1);
}

stock vectorCapacity(Vector:vector)
{
	return MEM_get_size(Pointer:MEM_get_val(Pointer:vector, 2));
}

stock vectorIsValid(Vector:vector)
{
	return MEM_is_valid_ptr(Pointer:vector);
}

stock Vector:vectorCreate(initsize = 1)
{
	if(initsize <= 0)   return Vector:0;
	
	new Vector:vector = Vector:MEM_new(3);
	MEM_set_val(Pointer:vector, 0, initsize);
	MEM_set_val(Pointer:vector, 1, 0);
	MEM_set_val(Pointer:vector, 2, _:MEM_new(initsize));
	return vector;
}

stock vectorDestroy(Vector:vector)
{
	MEM_delete(Pointer:MEM_get_val(Pointer:vector, 3));
	MEM_delete(Pointer:vector);
	return 1;
}

stock vectorDestroyPtr(Vector:vector)
{
	new Pointer:data = Pointer:MEM_get_val(Pointer:vector, 3);
	new len = MEM_get_size(data);
	for(new i = 0; i < len; i++)
	{
	    MEM_delete(Pointer:MEM_get_val(data, i));
	}
	MEM_delete(data);
	MEM_delete(Pointer:vector);
	return 1;
}

stock vectorGet(Vector:vector, index, &{_, Pointer}:value)
{
	if(index < 0 || index >= MEM_get_val(Pointer:vector, 1))    return 0;
	new Pointer:data = Pointer:MEM_get_val(Pointer:vector, 2);
	value = MEM_get_val(data, index);
	return 1;
}

stock vectorSet(Vector:vector, index, {_, Pointer}:value)
{
	if(index < 0 || index >= MEM_get_val(Pointer:vector, 1))    return 0;
	new Pointer:data = Pointer:MEM_get_val(Pointer:vector, 2);
	MEM_set_val(data, index, value);
	return 1;
}

stock vectorLenUp(Vector:vector, size, newzero = false)
{
	if(size <= 0)   return 0;

	new Pointer:data = Pointer:MEM_get_val(Pointer:vector, 2);

	new oldlen = MEM_get_val(Pointer:vector, 1);

	new newlen = oldlen+size;
	new capacity = MEM_get_size(data);
	while(newlen > capacity)
	{
	    new Pointer:newdata = MEM_new(capacity*2);
	    MEM_copy(newdata, data, capacity);
	    MEM_delete(data);
	    data = newdata;
	    capacity = MEM_get_size(data);
	}
	MEM_set_val(Pointer:vector, 2, _:data);
	MEM_set_val(Pointer:vector, 1, newlen);
	
	if(newzero)
	{
		MEM_zero(data, newlen-oldlen, oldlen);
	}
	return 1;
}

stock vectorLenDown(Vector:vector, size)
{
    if(size <= 0)   return 0;
    new newlen = max(MEM_get_val(Pointer:vector, 1)-size, 0);
    MEM_set_val(Pointer:vector, 1, newlen);
	return 0;
}

stock vectorReserve(Vector:vector, size)
{
	size -= MEM_get_val(Pointer:vector, 1);
	if(size > 0)
	{
	    return vectorLenUp(vector, size);
	}
	else if(size < 0)
	{
	    return vectorLenDown(vector, -size);
	}
	return 1;
}

stock vectorPush(Vector:vector, {_, Pointer}:value)
{
    vectorLenUp(vector, 1);
    new Pointer:data = Pointer:MEM_get_val(Pointer:vector, 2);
    MEM_set_val(data, MEM_get_val(Pointer:vector, 1)-1, value);
	return 1;
}

stock Vector:vectorCopy(Vector:source)
{
	new Vector:vector = Vector:MEM_new(3);
	MEM_copy(Pointer:vector, Pointer:source, MEM_get_size(Pointer:source));
	new Pointer:datasource = Pointer:MEM_get_val(Pointer:source, 2);
	new Pointer:data = MEM_new(MEM_get_size(datasource));
	MEM_copy(data, datasource, MEM_get_size(datasource));
	MEM_set_val(Pointer:vector, 2, _:data);
	return vector;
}

stock Pointer:vectorGetData(Vector:vector)
{
	return Pointer:MEM_get_val(Pointer:vector, 2);
}

