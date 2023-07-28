#if defined mdg_cmd_inc
	#endinput
#endif
#define mdg_cmd_inc

#include <Pawn.CMD>

#include <MS/separator>
#include <MS/messages>
#include <MS/check>

#define cmdResponse(%0) cmd:%0(playerid, params[])

static cmdtextbuf[144];
static cmdbuf[144];
static List:cmdtextlist;

stock cmdInit(num=-1)
{
	if(!listIsValid(cmdtextlist))
	{
    	cmdtextlist = strtokMarker(cmdtextbuf, num);
	}
	return 1;
}

stock cmdGet(num, out[], out_size = sizeof(out))
{
    return strSeparator(cmdtextbuf, out, num, cmdtextlist, false, out_size);
}

stock cmdGetInt(num)
{
    strSeparator(cmdtextbuf, cmdbuf, num, cmdtextlist, false, sizeof(cmdbuf));
    return strval(cmdbuf);
}

stock Float:cmdGetFloat(num)
{
    strSeparator(cmdtextbuf, cmdbuf, num, cmdtextlist, false, sizeof(cmdbuf));
    return floatstr(cmdbuf);
}

stock cmdGetNum()
{
	return listLen(cmdtextlist)/2;
}

public OnPlayerCommandReceived(playerid, cmd[], params[], flags)
{
    cmdtextbuf[0] = '\0';
	strmid(cmdtextbuf, params, 0, strlen(params));

#if defined mdg_cmd_OnPlayerCommandReceived
    return mdg_cmd_OnPlayerCommandReceived(playerid, cmd, params, flags);
#else
	return 1;
#endif
}
#if defined _ALS_OnPlayerCommandReceived
	#undef OnPlayerCommandReceived
#else
    #define _ALS_OnPlayerCommandReceived
#endif
#define OnPlayerCommandReceived mdg_cmd_OnPlayerCommandReceived
#if defined mdg_cmd_OnPlayerCommandReceived
forward mdg_cmd_OnPlayerCommandReceived(playerid, cmd[], params[], flags);
#endif

public OnPlayerCommandPerformed(playerid, cmd[], params[], result, flags)
{
#if defined mdg_cmd_OnPlayerCommandPerforme
    mdg_cmd_OnPlayerCommandPerforme(playerid, cmd[], params[], result, flags);
#endif
	if(listIsValid(cmdtextlist))
	{
    	listDestroy(cmdtextlist);
	}
	return 1;
}
#if defined _ALS_OnPlayerCommandPerformed
	#undef OnPlayerCommandPerformed
#else
    #define _ALS_OnPlayerCommandPerformed
#endif
#define OnPlayerCommandPerformed mdg_cmd_OnPlayerCommandPerforme
#if defined mdg_cmd_OnPlayerCommandPerforme
forward mdg_cmd_OnPlayerCommandPerforme(playerid, cmd[], params[], result, flags);
#endif

