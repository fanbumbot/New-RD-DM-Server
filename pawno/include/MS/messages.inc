#if defined mdg_messages_inc
	#endinput
#endif
#define mdg_messages_inc

#include <a_samp>
#include <MS/lang>
#include <MS/extargs>

static messagebuff[256];

stock SendFormatMessage(playerid, color, text[], {_, Float}:...)
{
	argInit(3);
	argFormat(messagebuff, sizeof(messagebuff), text);
	SendClientMessage(playerid, color, messagebuff);
	return 1;
}

stock SendFormatMessageToAll(color, text[], {_, Float}:...)
{
	argInit(2);
	argFormat(messagebuff, sizeof(messagebuff), text);
	SendClientMessageToAll(color, messagebuff);
	return 1;
}

stock SendMessage(playerid, text[], {_, Float}:...)
{
    langGetPlayer(playerid, text, messagebuff);
    argInit(2);
    argFormat(messagebuff, sizeof(messagebuff), messagebuff);
    SendClientMessage(playerid, -1, messagebuff);
    return 1;
}

stock SendMessageToAll(text[], {_, Float}:...)
{
    for(new playerid = 0; playerid < MAX_PLAYERS; playerid++)
    {
        if(!IsPlayerConnected(playerid))    continue;
        
    	langGetPlayer(playerid, text, messagebuff);
    	argInit(1);
    	argFormat(messagebuff, sizeof(messagebuff), messagebuff);
    	SendClientMessage(playerid, -1, messagebuff);
	}
	return 1;
}
