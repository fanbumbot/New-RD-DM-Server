#if defined mdg_ip_inc
	#endinput
#endif
#define mdg_ip_inc

#define IP_NONE IP:0

stock bool:operator>=(IP:oper1, IP:oper2)
{
	#emit load.s.pri oper1
	#emit load.s.alt oper2
	#emit geq
	#emit retn
	return bool:0;
}

stock bool:operator<=(IP:oper1, IP:oper2)
{
	#emit load.s.pri oper1
	#emit load.s.alt oper2
	#emit leq
	#emit retn
	return bool:0;
}

stock bool:operator>(IP:oper1, IP:oper2)
{
	#emit load.s.pri oper1
	#emit load.s.alt oper2
	#emit grtr
	#emit retn
	return bool:0;
}

stock bool:operator<(IP:oper1, IP:oper2)
{
	#emit load.s.pri oper1
	#emit load.s.alt oper2
	#emit less
	#emit retn
	return bool:0;
}

#define IPSet(%0,%1.%2.%3.%4) %0 = ip:((%1 << 24) | (%2 << 16) | (%3 << 8) | %4);

stock IPFromString(&IP:ip, string[])
{
	new value;
	new pos1 = 0;
	new pos2;
	new string2[8];
	new temp;
	for(new i = 0; i < 4; i++)
	{
		pos2 = strfind(string, ".", .pos = pos1);
		if((pos2 == -1 && i != 3) || pos2-pos1 == 0)  return 0;
		if(pos2 == -1)
		{
		    pos2 = strlen(string);
		}
		
		strmid(string2, string, pos1, pos2);
		temp = strval(string2);
		if(temp > 0xFF) return 0;
		value |= temp << ((3-i) << 3);
		pos1 = pos2+1;
	}
	ip = IP:value;
	return 1;
}

stock IPToString(string[], IP:ip, string_s = sizeof(string))
{
	string[0] = '\0';
	for(new i = 0; i < 4; i++)
	{
	    format(string, string_s, "%s%i.", string, (_:ip >> ((3-i) << 3)) & 0xFF);
	}
	string[strlen(string)-1] = '\0';
	return 1;
}

stock IsIPInGroup(IP:ip, IP:groupstart, IP:groupend)
{
	return ip >= groupstart && ip <= groupend ? 1 : 0;
}