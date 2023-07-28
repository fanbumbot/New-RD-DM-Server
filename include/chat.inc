#if defined inc_chat
	#endinput
#endif
#define inc_chat

static chatbuffer[144];
static chatbuffer_gangname[144];

public OnPlayerText(playerid, text[])
{
	new ret = 1;
#if defined chat_OnPlayerText
    ret = chat_OnPlayerText(playerid, text);
#endif
	if(ret)
	{
	    new color;
	    new gangid;
	    new gangtype;
	    new Lang:lang = GetPlayerLang(playerid);
	    
	    SetPlayerChatBubble(playerid, text, -1, 15.0, 6000);
	    
		gangid = GetPlayerGang(playerid);
		if(!IsGangExist(gangid))
	    {
     		color = GetPlayerColor(playerid) | 0xFF;
			format(chatbuffer, sizeof(chatbuffer), "%s [%i]: {FFFFFF}%s", PlayerName(playerid), playerid, text);
		}
		else
		{
		    GetGangName(gangid, chatbuffer_gangname);
		    gangtype = GetGangType(gangid);
		    color = (GetGangColor(gangid) << 8) | 0xFF;
		    if(gangtype <= 0)
		    {
				format(chatbuffer, sizeof(chatbuffer), "%s %s [%i]: {FFFFFF}%s", chatbuffer_gangname, PlayerName(playerid), playerid, text);
		    }
		    else
		    {
			    format(chatbuffer, sizeof(chatbuffer), "gangTypeName%i", gangtype);
			    if(langGet(lang, chatbuffer, chatbuffer, sizeof(chatbuffer), chatbuffer_gangname))
			    {
                    format(chatbuffer, sizeof(chatbuffer), "%s %s [%i]: {FFFFFF}%s", chatbuffer, PlayerName(playerid), playerid, text);
			    }
			    else
			    {
                    format(chatbuffer, sizeof(chatbuffer), "%s %s [%i]: {FFFFFF}%s", chatbuffer_gangname, PlayerName(playerid), playerid, text);
			    }
			}
		}
		
		
	    SendClientMessageToAll(color, chatbuffer);
	}
	return 0;
}
#if defined _ALS_OnPlayerText
	#undef OnPlayerText
#else
    #define _ALS_OnPlayerText
#endif
#define OnPlayerText chat_OnPlayerText
#if defined chat_OnPlayerText
forward chat_OnPlayerText(playerid, text[]);
#endif

stock SendLobbyMessage(Lobby:lobby, text[], {_, Float}:...)
{
	if(!lobbyIsExist(lobby))    return 0;
	new List:list = lobbyGroupGet(lobby, "Players");
	listForeach(playerid:list)
	{
		langGet(GetPlayerLang(playerid), text, chatbuffer);
		argInit(2);
	    argFormat(chatbuffer, sizeof(chatbuffer), chatbuffer);
	    SendMessage(playerid, chatbuffer);
	}
	return listLen(list);
}

stock SendAdminMessage(text[], {_, Float}:...)
{
	new List:list = GetListAdmins();
	listForeach(playerid:list)
	{
		langGet(GetPlayerLang(playerid), text, chatbuffer);
		argInit(1);
	    argFormat(chatbuffer, sizeof(chatbuffer), chatbuffer);
	    SendMessage(playerid, chatbuffer);
	}
	return listLen(list);
}

stock SendVIPMessage(text[], {_, Float}:...)
{
	new List:list = GetListVIPs();
	listForeach(playerid:list)
	{
		langGet(GetPlayerLang(playerid), text, chatbuffer);
		argInit(1);
	    argFormat(chatbuffer, sizeof(chatbuffer), chatbuffer);
	    SendMessage(playerid, chatbuffer);
	}
	return listLen(list);
}

stock SendGangMessage(gangid, text[], {_, Float}:...)
{
	if(!IsGangExist(gangid))    return 0;
	new List:list = itemGetList(itemGetByID(Gang, gangid), "ListPlayers");
	listForeach(playerid:list)
	{
		langGet(GetPlayerLang(playerid), text, chatbuffer);
		argInit(2);
	    argFormat(chatbuffer, sizeof(chatbuffer), chatbuffer);
	    SendMessage(playerid, chatbuffer);
	}
	return listLen(list);
}

stock chat_AdminBanIP(adminid, IP:ip, day = -1)
{
	new ret = AdminBanIP(adminid, ip, day);
	if(ret)
	{
	    IPToString(chatbuffer, ip);
	    if(day != -1)
		{
			SendAdminMessage("mAdminBanIP", PlayerName(adminid), chatbuffer);
		}
		else
		{
		    SendAdminMessage("mAdminBanIPTm", PlayerName(adminid), chatbuffer, day);
		}
	}
	return ret;
}
#if defined _ALS_AdminBanIP
	#undef AdminBanIP
#else
	#define _ALS_AdminBanIP
#endif
#define AdminBanIP chat_AdminBanIP

stock chat_AdminUnBanIP(adminid, IP:ip)
{
    new ret = AdminUnBanIP(adminid, IP:ip);
    if(ret)
    {
    	IPToString(chatbuffer, ip);
    	SendAdminMessage("mAdminUnBanIP", PlayerName(adminid), chatbuffer);
	}
	return ret;
}
#if defined _ALS_AdminUnBanIP
	#undef AdminUnBanIP
#else
	#define _ALS_AdminUnBanIP
#endif
#define AdminUnBanIP chat_AdminUnBanIP





