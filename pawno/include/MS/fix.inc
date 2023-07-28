#if defined mdg_fix_inc
	#endinput
#endif
#define mdg_fix_inc

static opd_check;

public OnPlayerDisconnect(playerid, reason)
{
    opd_check = 1;
#if defined mdg_fix_OnPlayerDisconnect
    mdg_fix_OnPlayerDisconnect(playerid, reason);
#endif
    opd_check = 0;
	return 1;
}
#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect mdg_fix_OnPlayerDisconnect
#if defined mdg_fix_OnPlayerDisconnect
forward mdg_fix_OnPlayerDisconnect(playerid, reason);
#endif

stock mdg_fix_ispacked(const string[])
{
    #emit lref.s.pri string
    #emit const.alt  ucharmax
    #emit geq
    #emit retn
    return 0;
}
#if defined _ALS_ispacked
	#undef ispacked
#else
	#define _ALS_ispacked
#endif
#define ispacked mdg_fix_ispacked

stock mdg_fix_GetPlayerIp(playerid, ip[], len)
{
	if(opd_check)
	{
	    return IPToString(ip, IP:itemGetInt(itemGetByID(Player, playerid), "IP"), len);
	}
	else
	{
	    return GetPlayerIp(playerid, ip, len);
	}
}
#if defined _ALS_GetPlayerIp
	#undef GetPlayerIp
#else
	#define _ALS_GetPlayerIp
#endif
#define GetPlayerIp mdg_fix_GetPlayerIp
