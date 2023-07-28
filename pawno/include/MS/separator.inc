#if defined mdg_sep_inc
	#endinput
#endif
#define mdg_sep_inc

#include <a_samp>
#include <MS/list>

//fix
stock IsStringPacked(string[])
{
    #emit lref.s.pri string
    #emit const.alt  ucharmax
    #emit geq
    #emit retn
    return 0;
}

stock strCastExcessSpace(string[], start = 0, end = sizeof(string))
{
	static mark, len, state_;
	mark = 0;
	state_ = 0;
	len = min(strlen(string), end);
	if(!IsStringPacked(string))
	{
		for(new i = start; i < len; i++)
		{
		    if(string[i] != ' ')
		    {
				if(state_ == 0)
				{
				    state_ = 1;
				}
				if(state_ == 2)
				{
				    state_ = 1;
				}
				if(state_ == 1)
				{
				    string[mark] = string[i];
				    mark++;
				}
		    }
		    else
		    {
		        if(state_ == 1)
		        {
		            string[mark] = string[i];
		            mark++;
		            state_ = 2;
		        }
		    }
		}
		if(state_ == 0)  	string[0] = ' ';
		if(state_ == 2)		mark--;
  		string[mark] = '\0';
	}
	else
	{
		for(new i = start; i < len; i++)
		{
		    if(string{i} != ' ')
		    {
				if(state_ == 0)
				{
				    state_ = 1;
				}
				if(state_ == 2)
				{
				    state_ = 1;
				}
				if(state_ == 1)
				{
				    string{mark} = string{i};
				    mark++;
				}
		    }
		    else
		    {
		        if(state_ == 1)
		        {
		            string{mark} = string{i};
		            mark++;
		            state_ = 2;
		        }
		    }
		}
		if(state_ == 0)  string{0} = ' ';
		if(state_ == 2)		mark--;
  		string{mark} = '\0';
	}
	
	return 1;
}

stock IsStringNoSpaces(key[])
{
	return (strfind(key, " ") != -1 || strfind(key, "\r") != -1 || strfind(key, "\n") != -1 || strfind(key, "\t") != -1) ? 0 : 1;
}

stock IsCharSpace(chr)
{
	return (chr == ' ' || chr == '\r' || chr == '\n' || chr == '\t' || chr == '\0') ? 1 : 0;
}

#define templateCut<%0|%1|%2|%3> \
	if(!packed) \
	{ \
		while(%2IsCharSpace(string[%0]) && %1) \
		{ \
			%0%3; \
		} \
	} \
	else \
 	{ \
		while(%2IsCharSpace(string{%0}) && %1) \
		{ \
			%0%3; \
		} \
	}

stock strCut(string[], out[], start = 0, end = sizeof(out), out_size = sizeof(out))
{
	static packed, mark_start, mark_end;
	packed = IsStringPacked(string);

	//find start
	mark_start = start;
	templateCut<mark_start|mark_start < end||++>
	if(mark_start == end)  return 0;

	//find end
	mark_end = end-1;
	templateCut<mark_end|mark_end > mark_start||-->
	mark_end++;

    strmid(out, string, mark_start, mark_end, out_size);
	return 1;
}

#undef templateCut

stock strKeyValueSeparator(string[], key[], value[], terminator[] = "=",
	key_size = sizeof(key), value_size = sizeof(value))
{
	new mark = strfind(string, terminator);
	if(mark == -1)  return 0;
	
	if(!strCut(string, key, 0, mark, key_size))	return 0;
	if(!strCut(string, value, mark+1, strlen(string), value_size))	return -1;
	return 1;
}

stock List:strMarker(string[], terminator[] = " ")
{
	new len = strlen(terminator);
	if(len == 1 && IsCharSpace(terminator[0]))   return List:0;
	new List:list = listCreate();
	new mark = -len;
	listPush(list, 0);
	while(mark == mark)
	{
	    mark = strfind(string, terminator, false, mark+len);
	    if(mark == -1)  break;
	    listPush(list, mark);
	    listPush(list, mark+len);
	    
	}
	listPush(list, strlen(string));
	return list;
}

stock List:strtokMarker(string[], maxcount = -1)
{
	if(maxcount <= 0 && maxcount != -1)   return List:0;

	new List:list = listCreate();
	new len = strlen(string);
	new state_ = 0;
	new count = 0;

	for(new i = 0; i < len; i++)
	{
	    if(!IsCharSpace(string[i]))
	    {
	        if(state_ == 0)
	        {
	            count++;

	            listPush(list, i);
				state_ = 1;
	            if(count == maxcount && maxcount != -1)
				{
					break;
				}
			}
		}
	    else
	    {
	        if(state_ == 1)
	        {
	            listPush(list, i);
				state_ = 0;
			}
	    }
	}
	if(state_ == 1)
	{
	    listPush(list, len);
	}
	return list;
}

stock strSeparator(string[], out[], number, List:marker, cut = false, out_size = sizeof(out))
{
	if(number < 0)	number = listLen(marker)+number+1;
	if(number < 0 || number > listLen(marker))  return 0;
	
	out[0] = '\0';
	
	new Pointer:node = listGetNode(marker, number*2);
	new start = MEM_get_val(node, 0);
	node = Pointer:MEM_get_val(node, 1);
	new end = MEM_get_val(node, 0);
	
	if(!cut)
	{
    	strmid(out, string, start, end, out_size);
	}
	else
	{
	    strCut(string, out, start, end, out_size);
	}
	return 1;
}


















